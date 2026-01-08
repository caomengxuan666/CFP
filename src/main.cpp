/*
 *  Copyright © 2025-2026 [caomengxuan666]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *  - File: main.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// Copyright (c) 2025 caomengxuan666
#include "cameras/CameraCapture.hpp"
#include "logging/CaponLogging.hpp"
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <iostream>
#include <memory>
#include <string>
#include <thread>

// 必须先包含 asio
// #include "asio.hpp"
#include "asio/io_context.hpp"
#include "protocol/ProtocolSession.hpp"

// 其他头文件
#include "algo/AlgoBase.hpp"
#include "algo/HoleDetection.hpp"
#include "business/BusinessManager.hpp"
#include "cameras/CameraFactory.hpp"
#include "cameras/EventHandlers.hpp"
#include "cameras/ImageSignalBus.hpp"
#include "config/ConfigManager.hpp"
#include "logging/LoggingConfigManager.hpp"
#include "utils/get_local_ip.h"

int main() {
  try {
    // 首先加载全局配置并应用日志配置
    auto global_config = config::GlobalConfig::load();
    logging::LoggingConfigManager::getInstance().applyGlobalConfig(
        global_config);
    LOG_INFO("日志初始化完成");

    // 初始化相机其他事件默认的所有handlers
    register_all_handlers();
    // 1. 创建两个独立的 io_context
    asio::io_context main_io_ctx;    // 用于主服务器（生产环境）
    asio::io_context backup_io_ctx;  // 用于副服务器（调试环境）

    auto main_guard = asio::make_work_guard(main_io_ctx.get_executor());
    auto backup_guard = asio::make_work_guard(backup_io_ctx.get_executor());

    // 2. 启动两个独立的事件循环线程
    std::thread main_thread([&main_io_ctx]() { main_io_ctx.run(); });
    std::thread backup_thread([&backup_io_ctx]() { backup_io_ctx.run(); });

    // 创建业务管理器（它会自动创建所有需要的会话）
    std::string local_ip = utils::get_local_ip();
    business::BusinessManager business_mgr(
        main_io_ctx, local_ip, "192.1.53.9",
        "192.1.53.10");  // 主服务器和备份服务器IP
    business_mgr.start();

    // 从 BusinessManager 获取主服务器和备份服务器的会话
    auto report_session = business_mgr.get_report_session();
    auto backup_report_session =
        business_mgr
            .get_backup_report_session();  // 可能为空，如果未配置备份服务器

    // 初始配置管理器 (可以在运行时从配置文件直接动态更新算法)
    auto& config_manager = config::ConfigManager::instance();
    config_manager.start();

    inicpp::IniManager ini(config::get_default_config_path());
    auto cameras =
        create_cameras_from_config(global_config.camera_entries, ini);

    // 启动相机
    for (auto& cam : cameras) {
      cam->start();
    }

    // 未来这里的算法取决于配置文件，从里面读取并且创建
    auto holeDetection = config_manager.create_algorithm<algo::HoleDetection>();
    algo::AlgoAdapter adapter(holeDetection);  // 栈上对象，持有 shared_ptr
    // AlgoAdapter

    // 延迟启动算法的lambda,可以做到开始信号来了之后在绑定
    auto start_camera_algo = [cameras, adapter]() {
      for (const auto& cam : cameras) {
        cam->add_frame_processor(adapter);
      }
    };

    // 设置开始信号回调 - 适配BusinessManager的回调参数类型
    business_mgr.set_start_callback(
        [start_camera_algo](const std::string& roll_id) {
          start_camera_algo();
        });

    // 特征数据回调 - 同时发送到主服务器和备份服务器
    ImageSignalBus::instance().subscribe_feature(
        "hole_features", [report_session, backup_report_session,
                          &cameras](const ImageSignalBus::FeatureData& data) {
          if (!report_session || !cameras.empty()) {
            return;
          }

          std::shared_ptr<CapturedFrame> frame;
          // TODO(cmx) 目前是单相机，就直接用第一个相机采集发送了
          if (cameras[0]->get_frame_queue().try_dequeue(frame) && frame) {
            protocol::FeatureReport report;
            report.roll_id = data.roll_id;
            report.features = data.features;
            report.special_images = data.special_images;
            // 从业务逻辑上看貌似是不需要整个的原始图像信息的?
            // report.image_data = frame->data;

            // 发送到主服务器
            report_session->async_send_features(report, [](std::error_code ec) {
              if (ec) {
                LOG_ERROR("Feature send to main server failed: {}",
                          ec.message());
              }
            });

            // 发送到备份服务器（如果存在）
            if (backup_report_session) {
              backup_report_session->async_send_features(
                  report, [](std::error_code ec) {
                    if (ec) {
                      LOG_ERROR("Feature send to backup server failed: {}",
                                ec.message());
                    }
                  });
            }
          }
        });

    // 遥测数据由 BusinessManager 自动处理
    // 不需要手动创建 telemetry_thread

    // 状态发送线程，同时发送给主服务器和备份服务器
    std::thread status_thread([report_session, backup_report_session,
                               &cameras]() {
      while (true) {
        if (cameras[0]) {
          auto status = cameras[0]->get_status();

          // 发送到主服务器
          report_session->async_send_status(status, [](std::error_code ec) {
            if (ec) {
              LOG_ERROR("Status send to main server failed: {}", ec.message());
            }
          });

          // 发送到备份服务器（如果存在）
          if (backup_report_session) {
            backup_report_session->async_send_status(
                status, [](std::error_code ec) {
                  if (ec) {
                    LOG_ERROR("Status send to backup server failed: {}",
                              ec.message());
                  }
                });
          }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });

    LOG_INFO("Frontend node initialized.");
    if (business_mgr.is_master()) {
      LOG_INFO("Running as MASTER (101).");
    } else {
      LOG_INFO("Running as WORKER");
    }
    LOG_INFO("Waiting for start signal...");
    LOG_INFO("Press Enter to exit.");

    std::cin.get();

    // 清理
    if (cameras[0]) {
      cameras[0]->stop();
    }
    main_guard.reset();
    backup_guard.reset();
    main_io_ctx.stop();
    backup_io_ctx.stop();

    if (main_thread.joinable()) {
      main_thread.join();
    }

    if (backup_thread.joinable()) {
      backup_thread.join();
    }

    if (status_thread.joinable()) {
      status_thread.join();
    }

    return 0;
  } catch (const std::exception& e) {
    LOG_ERROR("Exception: {}", e.what());
    return -1;
  } catch (...) {
    LOG_ERROR("Unknown exception");
    return -1;
  }
}
