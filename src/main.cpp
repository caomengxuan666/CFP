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
#define _WIN32_WINNT 0x0601

#include <atomic>
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
#include "cameras/Dvp/DvpCameraBuilder.hpp"
#include "cameras/ImageSignalBus.hpp"
#include "config/ConfigManager.hpp"
#include "utils/get_local_ip.h"

int main() {
  asio::io_context io_context;
  auto work_guard = asio::make_work_guard(io_context.get_executor());
  std::thread asio_thread([&io_context]() { io_context.run(); });

  // 创建业务管理器（它会自动创建所有需要的会话）
  std::string local_ip = utils::get_local_ip();
  business::BusinessManager business_mgr(io_context, local_ip);
  business_mgr.start();

  // 从 BusinessManager 获取 19300 会话
  auto report_session = business_mgr.get_report_session();

  // 初始化算法
  auto& config_manager = config::ConfigManager::instance();
  config_manager.start();
  auto holeDetection = config_manager.create_algorithm<algo::HoleDetection>();

  std::shared_ptr<DvpCameraCapture> camera = nullptr;
  std::atomic<bool> camera_running{false};

  auto create_camera = [&holeDetection]() {
    return DvpCameraBuilder::fromUserId("123")
        .onFrame(algo::AlgoAdapter(holeDetection))
        .build();
  };

  auto start_camera = [&camera, &camera_running, &create_camera]() {
    if (camera_running.exchange(true)) {
      return;
    }
    camera = create_camera();
    if (!camera || !camera->start()) {
      std::cerr << "Camera start failed!" << std::endl;
      camera_running = false;
      return;
    }
    std::cout << "Camera started!" << std::endl;
  };

  // 设置开始信号回调
  // 设置开始信号回调 - 适配BusinessManager的回调参数类型
  business_mgr.set_start_callback(
      [start_camera](const std::string& roll_id) { start_camera(); });

  // 特征数据回调
  ImageSignalBus::instance().subscribe_feature(
      "hole_features",
      [report_session, &camera](const ImageSignalBus::FeatureData& data) {
        if (!report_session || !camera) {
          return;
        }

        std::shared_ptr<CapturedFrame> frame;
        if (camera->get_frame_queue().try_dequeue(frame) && frame) {
          protocol::FeatureReport report;
          report.roll_id = data.roll_id;
          report.features = data.features;
          report.special_images = data.special_images;
          report.image_data = frame->data;

          report_session->async_send_features(report, [](std::error_code ec) {
            if (ec) {
              std::cerr << "Feature send failed: " << ec.message() << "\n";
            }
          });
        }
      });

  // 遥测数据由 BusinessManager 自动处理
  // 不需要手动创建 telemetry_thread

  // 状态发送线程，发送给服务器
  std::thread status_thread([report_session, &camera, &camera_running]() {
    while (true) {
      if (camera_running && camera) {
        auto status = camera->get_status();
        report_session->async_send_status(status, [](std::error_code ec) {
          if (ec) {
            std::cerr << "Status send failed: " << ec.message() << "\n";
          }
        });
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  std::cout << "Frontend node initialized. ";
  if (business_mgr.is_master()) {
    std::cout << "Running as MASTER (101).";
  } else {
    std::cout << "Running as WORKER.";
  }
  std::cout << " Waiting for start signal...\n";
  std::cout << "Press Enter to exit.\n";

  std::cin.get();

  // 清理
  if (camera) {
    camera->stop();
  }
  work_guard.reset();
  io_context.stop();

  if (asio_thread.joinable()) {
    asio_thread.join();
  }

  if (status_thread.joinable()) {
    status_thread.join();
  }

  return 0;
}
