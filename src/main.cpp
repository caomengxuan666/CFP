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
#include "logging/CaponLogging.hpp"
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

// 必须先包含 asio
// #include "asio.hpp"
#include "asio/io_context.hpp"
#include "protocol/ProtocolSession.hpp"

// 其他头文件
#include "algo/AlgoBase.hpp"
#include "algo/HoleDetection.hpp"
#include "business/BusinessManager.hpp"
// #include "cameras/Dvp/DvpCameraBuilder.hpp"
#include "cameras/CameraFactory.hpp"
#include "cameras/Ikap/IkapCameraBuilder.hpp"
#include "cameras/Ikap/IkapCameraCapture.hpp"
#include "cameras/ImageSignalBus.hpp"
#include "config/CameraConfig.hpp"
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

    // 初始化算法
    auto& config_manager = config::ConfigManager::instance();
    config_manager.start();

    // 直接用config创建相机工厂
    // TODO(cmx) 后续可以考虑在 @create_camera
    // 加一个直接返回shared_ptr的接口减少转换开销
    auto uni_camera = create_camera(
        CameraBrand::IKap, "cam1",
        CameraConfig{
            // 这里可以初始换配置文件
            // .trigger_mode=true
            // TODO(cmx) 后续我们需要在 @GlobalConfig 中添加相机的配置文件
            // 这样就可以达到工厂自动应用的效果
            // 同时我们也应该在 @CameraManager
            // 中增加这个工厂接口,利用配置文件直接创建相机
        });
    std::shared_ptr<CameraCapture> camera = std::move(uni_camera);
    auto holeDetection = config_manager.create_algorithm<algo::HoleDetection>();

    std::atomic<bool> camera_running{false};

    auto create_camera = [&holeDetection]() {
      return IkapCameraBuilder::fromUserId("123")
          .onFrame(algo::AlgoAdapter(holeDetection))
          .build();
    };

    auto start_camera = [&camera, &camera_running, &create_camera]() {
      if (camera_running.exchange(true)) {
        return;
      }
      camera = create_camera();
      if (!camera || !camera->start()) {
        LOG_ERROR("Camera failed to start!");
        camera_running = false;
        return;
      }
      LOG_INFO("Camera started successfully!");
    };

    // 设置开始信号回调 - 适配BusinessManager的回调参数类型
    // TODO(cmx)
    // 这里的业务逻辑实际上是启动和应用具体的算法，相机在此之前就要被启动了
    // 所以每一个相机都要有一个start_camera接口，确保没有算法实例的时候都能成功启动相机
    // 至于算法可以在收到这个信号之后延迟来绑定
    business_mgr.set_start_callback(
        [start_camera](const std::string& roll_id) { start_camera(); });

    // 特征数据回调 - 同时发送到主服务器和备份服务器
    ImageSignalBus::instance().subscribe_feature(
        "hole_features", [report_session, backup_report_session,
                          &camera](const ImageSignalBus::FeatureData& data) {
          if (!report_session || !camera) {
            return;
          }

          std::shared_ptr<CapturedFrame> frame;
          if (camera->get_frame_queue().try_dequeue(frame) && frame) {
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
    std::thread status_thread([report_session, backup_report_session, &camera,
                               &camera_running]() {
      while (true) {
        if (camera_running && camera) {
          // 将camera转换为具体的类
          camera = std::dynamic_pointer_cast<IkapCameraCapture>(camera);
          auto status = camera->get_status();

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
    if (camera) {
      camera->stop();
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
