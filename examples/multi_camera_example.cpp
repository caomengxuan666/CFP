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
 *  - File: multi_camera_example.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "algo/AlgoBase.hpp"
#include "algo/HoleDetection.hpp"
#include "cameras/CameraManager.hpp"
#include "cameras/Dvp/DvpCameraBuilder.hpp"
#include "cameras/Dvp/DvpCameraCapture.hpp"
#include "cameras/FrameProcessor.hpp"
#include "cameras/Ikap/IkapCameraCapture.hpp"
#include "cameras/Ikap/IkapCameraManager.hpp"
#include "cameras/MultiCameraCoordinator.hpp"

// 示例：创建一个简单的帧处理器
class SimpleFrameProcessor {
 public:
  explicit SimpleFrameProcessor(const std::string& name) : name_(name) {}

  void operator()(const CapturedFrame& frame) {
    std::cout << name_ << " processed frame of size: " << frame.data.size()
              << ", width: " << frame.width() << ", height: " << frame.height()
              << std::endl;
  }

  // 添加转换为FrameProcessor的函数
  operator FrameProcessor() {
    auto lambda = [this](const CapturedFrame& frame) {
      this->operator()(frame);
    };
    return make_function_processor(std::move(lambda));
  }

 private:
  std::string name_;
};

int main() {
  std::cout << "=== Multi-Camera Coordinator Example ===" << std::endl;

  // 示例1: 创建多相机协调器
  std::cout << "\n--- Multi-Camera Coordinator with Operator[] ---"
            << std::endl;

  // 创建融合策略
  auto fusion_strategy =
      [](const std::vector<CapturedFrame>& frames) -> CapturedFrame {
    std::cout << "Fusion strategy called with " << frames.size() << " frames"
              << std::endl;

    // 这里只是一个占位符实现
    if (!frames.empty()) {
      std::cout << "Fusing frames with sizes: ";
      for (size_t i = 0; i < frames.size(); ++i) {
        std::cout << frames[i].data.size() << " ";
      }
      std::cout << std::endl;

      return frames[0];  // 返回第一帧作为示例
    }
    return CapturedFrame{};  // 返回空帧
  };

  // 创建协调器 - 管理3个相机
  MultiCameraCoordinator coordinator(3, fusion_strategy);

  // 使用operator[]为每个相机获取帧处理器
  // 这是利用MultiCameraCoordinator的operator[]重载功能
  auto processor1 = coordinator[0];  // 等价于 coordinator.make_processor_for(0)
  auto processor2 = coordinator[1];  // 等价于 coordinator.make_processor_for(1)
  auto processor3 = coordinator[2];  // 等价于 coordinator.make_processor_for(2)

  std::cout << "Created 3 frame processors using operator[]:" << std::endl;
  std::cout << "- Processor for camera 0" << std::endl;
  std::cout << "- Processor for camera 1" << std::endl;
  std::cout << "- Processor for camera 2" << std::endl;

  // 示例2: 使用泛型相机管理器管理多台DVP相机
  std::cout << "\n--- Multi-DVP Camera Management with Generic Manager ---"
            << std::endl;

  // 使用泛型相机管理器管理多个DVP相机
  GenericCameraManager<DvpCameraCapture> dvpManager(8);  // 支持最多8个DVP相机

  // 创建多个DVP相机并添加到管理器
  for (int i = 0; i < 3; ++i) {
    auto camera =
        DvpCameraBuilder::fromUserId("DVP_MULTI_" + std::to_string(i))
            .onFrame(SimpleFrameProcessor("DVP_Camera_" + std::to_string(i)))
            .build();

    if (camera) {
      dvpManager.add_camera(std::move(camera));
      std::cout << "Added DVP camera " << i
                << ", total: " << dvpManager.camera_count() << std::endl;
    } else {
      std::cerr << "Failed to create DVP camera " << i << std::endl;
    }
  }

  // 启动所有DVP相机
  std::cout << "Starting all DVP cameras..." << std::endl;
  dvpManager.start_all();

  // 示例3: 演示相机状态监控
  std::cout << "\n--- Camera Status Monitoring ---" << std::endl;

  //   if (dvpManager.camera_count() > 0) {
  //     auto first_camera = dvpManager.get_camera(0);
  //     if (first_camera) {
  //       auto status = first_camera->get_status();
  //       std::cout << "First camera status: Device State="
  //                 << static_cast<int>(status.device_state)
  //                 << ", Connection State="
  //                 << static_cast<int>(status.connection_state)
  //                 << ", Working State=" <<
  //                 static_cast<int>(status.working_state)
  //                 << std::endl;
  //     }
  //   }

  // 示例4: 混合品牌相机管理
  std::cout << "\n--- Mixed Brand Camera Management ---" << std::endl;

  // 演示如何管理混合品牌相机
  // 创建一个用于演示的相机管理器
  CameraManager mixed_manager;

  // 添加DVP相机
  auto camera = DvpCameraBuilder::fromUserId("MIXED_DVP_001")
                    .onFrame(SimpleFrameProcessor("Mixed_DVP_Camera"))
                    .build();

  if (camera) {
    mixed_manager.add_camera("mixed_dvp", std::move(camera));
    std::cout << "Added mixed brand DVP camera" << std::endl;
  }

  // 注意：IKap相机的创建需要特定的设备句柄，这里仅展示概念
  // 在实际应用中，你需要根据IKap SDK获取设备句柄
  std::cout << "Mixed brand management supports both DVP and IKap cameras"
            << std::endl;

  // 等待一段时间以观察效果
  std::cout << "\nRunning cameras for 500ms..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // 停止所有DVP相机
  std::cout << "\nStopping all DVP cameras..." << std::endl;
  dvpManager.stop_all();

  std::cout << "\n=== Multi-Camera Coordinator Example completed ==="
            << std::endl;

  return 0;
}
