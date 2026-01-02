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
 *  - File: generic_camera_example.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#include <iostream>
#include <memory>
#include <utility>

#include "algo/AlgoBase.hpp"
#include "algo/HoleDetection.hpp"
#include "cameras/CameraManager.hpp"
#include "cameras/Dvp/DvpCameraBuilder.hpp"
#include "cameras/Dvp/DvpCameraCapture.hpp"
#include "cameras/Ikap/IkapCameraCapture.hpp"
#include "cameras/Ikap/IkapCameraManager.hpp"

int main() {
  std::cout << "Generic Camera Manager Example" << std::endl;

  // 示例1: 使用泛型相机管理器管理DVP相机
  std::cout << "\n--- DVP Camera Manager Example ---" << std::endl;
  GenericCameraManager<DvpCameraCapture> dvpManager(4);  // 支持4个DVP相机

  // 创建DVP相机并添加到管理器
  auto dvpCamera =
      DvpCameraBuilder::fromUserId("DVP_CAMERA_001")
          .onFrame(algo::AlgoAdapter(std::make_shared<algo::HoleDetection>()))
          .build();

  if (dvpCamera) {
    dvpManager.add_camera(std::move(dvpCamera));
    std::cout << "Added DVP camera to manager, total cameras: "
              << dvpManager.camera_count() << std::endl;
  }

  // 示例2: 演示多相机管理
  std::cout << "\n--- Multiple Camera Management Example ---" << std::endl;

  // 添加更多DVP相机
  for (int i = 2; i <= 4; ++i) {
    auto camera =
        DvpCameraBuilder::fromUserId("DVP_CAMERA_00" + std::to_string(i))
            .onFrame(algo::AlgoAdapter(std::make_shared<algo::HoleDetection>()))
            .build();

    if (camera) {
      dvpManager.add_camera(std::move(camera));
      std::cout << "Added DVP camera " << i
                << ", total: " << dvpManager.camera_count() << std::endl;
    }
  }

  // 启动所有相机
  std::cout << "\nStarting all cameras..." << std::endl;
  dvpManager.start_all();

  // 演示获取特定相机
  auto firstCamera = dvpManager.get_camera(0);
  if (firstCamera) {
    std::cout << "Successfully retrieved first camera" << std::endl;
  }

  // 演示移除相机
  std::cout << "Removing camera at index 1..." << std::endl;
  dvpManager.remove_camera(1);
  std::cout << "Total cameras after removal: " << dvpManager.camera_count()
            << std::endl;

  std::cout << "\nGeneric Camera Manager example completed." << std::endl;

  return 0;
}
