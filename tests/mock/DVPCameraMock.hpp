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
 *  - File: DVPCameraMock.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */
#pragma once
#include <gmock/gmock.h>

#include <memory>

#include "DVPCamera.h"
#include "IDVPCamera.h"
#include "cameras/Dvp/DvpCameraCapture.hpp"
#include "cameras/Dvp/DvpConfig.hpp"

// 模拟一下dvpRegion结构
struct MockDvpRegion {
  int X, Y, W, H;
  MockDvpRegion(int x, int y, int w, int h) : X(x), Y(y), W(w), H(h) {}
};

// 模拟相机设备信息
struct MockCameraInfo {
  std::string user_id;
  std::string friendly_name;
  bool is_connected = false;
};

// Mock相机接口
class DVPCameraMock : public IDVPCamera {
 public:
  virtual ~DVPCameraMock() = default;

  MOCK_METHOD(bool, connect, (const std::string& id), (override));
  MOCK_METHOD(void, disconnect, (), (override));
  MOCK_METHOD(bool, isConnected, (), (const, override));

  MOCK_METHOD(void, setExposure, (double us), (override));
  MOCK_METHOD(double, getExposure, (), (const, override));

  MOCK_METHOD(void, setGain, (float gain), (override));
  MOCK_METHOD(float, getGain, (), (const, override));

  MOCK_METHOD(void, setHardwareIsp, (bool enable), (override));
  MOCK_METHOD(bool, getHardwareIsp, (), (const, override));

  MOCK_METHOD(void, setTriggerMode, (bool enable), (override));
  MOCK_METHOD(bool, getTriggerMode, (), (const, override));

  MOCK_METHOD(void, setConfig, (const DvpConfig& config), (override));
  MOCK_METHOD(DvpConfig, getConfig, (), (const, override));

  MOCK_METHOD(void, startCapture, (), (override));
  MOCK_METHOD(void, stopCapture, (), (override));
  MOCK_METHOD(bool, isCapturing, (), (const, override));

  MOCK_METHOD(std::vector<uint8_t>, getFrameData, (), (override));
  MOCK_METHOD(void, registerFrameCallback,
              (const std::shared_ptr<FrameProcessor>& callback), (override));
};

// 模拟DvpCapture
class MockDvpCapture : public DvpCameraCapture {
 public:
  // 添加构造函数以解决默认构造函数被删除的问题
  MockDvpCapture() : DvpCameraCapture(0) {}  // 传递一个默认的dvpHandle值

  // 使用MOCK_METHOD宏定义模拟方法，不再使用override关键字
  MOCK_METHOD(void, set_config, (const DvpConfig& config), ());
  MOCK_METHOD(DvpConfig, get_config, (), (const));
  MOCK_METHOD(bool, start, (const FrameProcessor& processor), ());
  MOCK_METHOD(void, stop, (), ());
  MOCK_METHOD(bool, is_running, (), (const));
  MOCK_METHOD(void, register_frame_callback, (const FrameProcessor& proc), ());
  MOCK_METHOD(void, register_event_callback,
              (DvpEventType type, const DvpEventHandler& handler), ());
};
