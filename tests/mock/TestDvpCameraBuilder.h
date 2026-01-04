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
 *  - File: TestDvpCameraBuilder.h
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */
// tests/mock/TestDvpCameraBuilder.h
#include "DVPCameraMock.hpp"
#include "TestDvpCamera.h"
#include "cameras/Dvp/DvpCameraBuilder.hpp"

class TestDvpCameraBuilder : public DvpCameraBuilder {
 public:
  TestDvpCameraBuilder() = default;

  TestDvpCameraBuilder& with_mock_camera(
      std::shared_ptr<DVPCameraMock> mock_camera) {
    mock_camera_ = mock_camera;
    return *this;
  }

  std::unique_ptr<TestDvpCamera> build_test() {
    if (!mock_camera_) {
      mock_camera_ = std::make_shared<DVPCameraMock>();
    }

    // 连接相机
    mock_camera_->connect("TestCamera");

    auto camera = std::make_unique<TestDvpCamera>(mock_camera_);

    // 获取配置并应用到 Mock 相机
    auto config = this->toDvpConfig();
    if (config.exposure_us > 0) {
      mock_camera_->setExposure(config.exposure_us);
    }
    if (config.gain > 0) {
      mock_camera_->setGain(config.gain);
    }
    if (config.hardware_isp) {
      mock_camera_->setHardwareIsp(config.hardware_isp);
    }
    if (config.trigger_mode) {
      mock_camera_->setTriggerMode(config.trigger_mode);
    }

    // 注册帧回调
    mock_camera_->registerFrameCallback(nullptr);

    // 启动捕获
    mock_camera_->startCapture();

    return camera;
  }

  // 显式声明继承的配置方法
  TestDvpCameraBuilder& fromUserId(const std::string& userId) {
    DvpCameraBuilder::fromUserId(userId);
    return *this;
  }

  TestDvpCameraBuilder& exposure(double value) {
    DvpCameraBuilder::exposure(value);
    return *this;
  }

  TestDvpCameraBuilder& gain(float value) {
    DvpCameraBuilder::gain(value);
    return *this;
  }

  TestDvpCameraBuilder& hardwareIsp(bool value) {
    DvpCameraBuilder::hardwareIsp(value);
    return *this;
  }

  TestDvpCameraBuilder& triggerMode(bool value) {
    DvpCameraBuilder::triggerMode(value);
    return *this;
  }

  TestDvpCameraBuilder& onFrame(const FrameProcessor& proc) {
    DvpCameraBuilder::onFrame(proc);
    return *this;
  }

 private:
  std::shared_ptr<DVPCameraMock> mock_camera_;
};
