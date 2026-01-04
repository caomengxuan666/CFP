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
 *  - File: IkapCameraBuilder.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// IkapCameraBuilder.cpp
#include "cameras/Ikap/IkapCameraBuilder.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "IKapC.h"
#include "cameras/Ikap/IkapCameraCapture.hpp"

// 静态工厂方法
IkapCameraBuilder IkapCameraBuilder::fromUserId(const std::string& id) {
  IkapCameraBuilder builder;
  builder.config_.user_id = id;
  return builder;
}

IkapCameraBuilder IkapCameraBuilder::fromDeviceName(const std::string& id) {
  IkapCameraBuilder builder;
  builder.config_.device_name = id;
  return builder;
}

IkapCameraBuilder IkapCameraBuilder::fromFriendlyName(const std::string& id) {
  IkapCameraBuilder builder;
  builder.config_.friendly_name = id;
  return builder;
}

IkapCameraBuilder IkapCameraBuilder::fromSerialNumber(const std::string& id) {
  IkapCameraBuilder builder;
  builder.config_.serial_number = id;
  return builder;
}

IkapCameraBuilder IkapCameraBuilder::fromUserDefinedName(
    const std::string& name) {
  IkapCameraBuilder builder;
  builder.config_.user_defined_name = name;
  return builder;
}

IkapCameraBuilder& IkapCameraBuilder::exposure(double us) {
  config_.exposure_us = us;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::gain(float gain) {
  config_.gain = gain;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::roi(int x, int y, int w, int h) {
  config_.roi_x = x;
  config_.roi_y = y;
  config_.roi_w = w;
  config_.roi_h = h;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::hardwareIsp(bool enable) {
  config_.hardware_isp = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::triggerMode(bool enable) {
  config_.trigger_mode = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::autoExposure(bool enable) {
  config_.auto_exposure = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::autoGain(bool enable) {
  config_.auto_gain = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::aeTargetBrightness(int brightness) {
  config_.ae_target_brightness = brightness;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::antiFlickerMode(int mode) {
  config_.anti_flicker_mode = mode;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::acquisitionMode(int mode) {
  config_.acquisition_mode = mode;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::contrast(int contrast) {
  config_.contrast = contrast;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::gamma(int gamma) {
  config_.gamma = gamma;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::saturation(int saturation) {
  config_.saturation = saturation;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::sharpness(int sharpness) {
  config_.sharpness = sharpness;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::sharpnessEnable(bool enable) {
  config_.sharpness_enable = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::inverseState(bool enable) {
  config_.inverse_state = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::flipHorizontalState(bool enable) {
  config_.flip_horizontal_state = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::flipVerticalState(bool enable) {
  config_.flip_vertical_state = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::rotateState(bool enable) {
  config_.rotate_state = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::rotateOpposite(bool enable) {
  config_.rotate_opposite = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::coolerState(bool enable) {
  config_.cooler_state = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::bufferQueueSize(int size) {
  config_.buffer_queue_size = size;
  return *this;
}

// IKAP特有参数配置
IkapCameraBuilder& IkapCameraBuilder::startMode(uint32_t mode) {
  config_.start_mode = mode;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::transferMode(uint32_t mode) {
  config_.transfer_mode = mode;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::grabStrategy(uint32_t strategy) {
  config_.grab_strategy = strategy;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::autoClear(bool enable) {
  config_.auto_clear = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::flipX(bool enable) {
  config_.flip_x = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::flipY(bool enable) {
  config_.flip_y = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::zoomMethod(int method) {
  config_.zoom_method = method;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::windowTitle(const std::string& title) {
  config_.window_title = title;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::batchMode(bool enable) {
  config_.batch_mode = enable;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::batchFrameCount(uint32_t count) {
  config_.batch_frame_count = count;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::streamFlowCtrl(uint32_t value) {
  config_.stream_flow_ctrl = value;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::linkTimeout(uint32_t timeout) {
  config_.link_timeout = timeout;
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::onFrame(const FrameProcessor& proc) {
  config_.frame_processors.push_back(proc);
  return *this;
}

IkapCameraBuilder& IkapCameraBuilder::onEvent(const IkapEventType event,
                                              const IkapEventHandler& handler) {
  config_.event_handlers[event] = handler;
  return *this;
}

// 转换为IkapConfig
IkapConfig IkapCameraBuilder::toIkapConfig() const {
  IkapConfig cfg;
  // NOLINTBEGIN
  // 基础参数
  if (config_.exposure_us.has_value()) cfg.exposure_us = *config_.exposure_us;
  if (config_.gain.has_value()) cfg.gain = *config_.gain;
  if (config_.roi_x.has_value()) cfg.roi_x = *config_.roi_x;
  if (config_.roi_y.has_value()) cfg.roi_y = *config_.roi_y;
  if (config_.roi_w.has_value()) cfg.roi_w = *config_.roi_w;
  if (config_.roi_h.has_value()) cfg.roi_h = *config_.roi_h;
  if (config_.hardware_isp.has_value())
    cfg.hardware_isp = *config_.hardware_isp;
  if (config_.trigger_mode.has_value())
    cfg.trigger_mode = *config_.trigger_mode;
  if (config_.auto_exposure.has_value())
    cfg.auto_exposure = *config_.auto_exposure;
  if (config_.auto_gain.has_value()) cfg.auto_gain = *config_.auto_gain;
  if (config_.ae_target_brightness.has_value())
    cfg.ae_target_brightness = *config_.ae_target_brightness;
  if (config_.anti_flicker_mode.has_value())
    cfg.anti_flicker_mode = *config_.anti_flicker_mode;
  if (config_.acquisition_mode.has_value())
    cfg.acquisition_mode = *config_.acquisition_mode;
  if (config_.contrast.has_value()) cfg.contrast = *config_.contrast;
  if (config_.gamma.has_value()) cfg.gamma = *config_.gamma;
  if (config_.saturation.has_value()) cfg.saturation = *config_.saturation;
  if (config_.sharpness.has_value()) cfg.sharpness = *config_.sharpness;
  if (config_.sharpness_enable.has_value())
    cfg.sharpness_enable = *config_.sharpness_enable;
  if (config_.inverse_state.has_value())
    cfg.inverse_state = *config_.inverse_state;
  if (config_.flip_horizontal_state.has_value())
    cfg.flip_horizontal_state = *config_.flip_horizontal_state;
  if (config_.flip_vertical_state.has_value())
    cfg.flip_vertical_state = *config_.flip_vertical_state;
  if (config_.rotate_state.has_value())
    cfg.rotate_state = *config_.rotate_state;
  if (config_.rotate_opposite.has_value())
    cfg.rotate_opposite = *config_.rotate_opposite;
  if (config_.cooler_state.has_value())
    cfg.cooler_state = *config_.cooler_state;
  if (config_.buffer_queue_size.has_value())
    cfg.buffer_queue_size = *config_.buffer_queue_size;

  // IKAP特有参数
  if (config_.start_mode.has_value()) cfg.start_mode = *config_.start_mode;
  if (config_.transfer_mode.has_value())
    cfg.transfer_mode = *config_.transfer_mode;
  if (config_.grab_strategy.has_value())
    cfg.grab_strategy = *config_.grab_strategy;
  if (config_.auto_clear.has_value()) cfg.auto_clear = *config_.auto_clear;
  if (config_.flip_x.has_value()) cfg.flip_x = *config_.flip_x;
  if (config_.flip_y.has_value()) cfg.flip_y = *config_.flip_y;
  if (config_.zoom_method.has_value()) cfg.zoom_method = *config_.zoom_method;
  if (config_.window_title.has_value())
    cfg.window_title = *config_.window_title;
  if (config_.batch_mode.has_value()) cfg.batch_mode = *config_.batch_mode;
  if (config_.batch_frame_count.has_value())
    cfg.batch_frame_count = *config_.batch_frame_count;
  if (config_.stream_flow_ctrl.has_value())
    cfg.stream_flow_ctrl = *config_.stream_flow_ctrl;
  if (config_.link_timeout.has_value())
    cfg.link_timeout = *config_.link_timeout;
  // NOLINTEND
  return cfg;
}

// 构建方法
std::unique_ptr<IkapCameraCapture> IkapCameraBuilder::build() {
  ITKDEVICE handle = nullptr;
  ITKSTATUS status = ITKSTATUS_OK;

  // 初始化 IKapC 运行环境
  status = ItkManInitialize();
  if (status != ITKSTATUS_OK) {
    std::cerr << "Failed to initialize IKapC runtime environment, error code: "
              << status << std::endl;
    return nullptr;
  }

  // 枚举可用设备的数量
  uint32_t numDevices = 0;
  status = ItkManGetDeviceCount(&numDevices);
  if (status != ITKSTATUS_OK) {
    std::cerr << "Failed to get device count, error code: " << status
              << std::endl;
    ItkManTerminate();
    return nullptr;
  }

  // 当没有连接的设备时
  if (numDevices == 0) {
    std::cerr << "No IKap cameras found" << std::endl;
    ItkManTerminate();
    return nullptr;
  }

  // 遍历设备查找匹配的用户ID
  uint32_t deviceIndex = 0;
  bool found = false;

  for (uint32_t i = 0; i < numDevices; ++i) {
    ITKDEV_INFO di;
    status = ItkManGetDeviceInfo(i, &di);
    if (status != ITKSTATUS_OK) {
      std::cerr << "Failed to get device info for device " << i
                << ", error code: " << status << std::endl;
      continue;
    }

    // 打印设备信息
    // Print information.
    printf(
        "i:%d \n Device Full Name:%s\n Friendly Name:%s\n Vendor Name:%s\n "

        "Model Name:%s\n Serial Name:%s\n Device Class:%s\n "

        "Device Version:%s\n User Defined Name:%s\n\n",

        i, di.FullName, di.FriendlyName, di.VendorName, di.ModelName,

        di.SerialNumber, di.DeviceClass, di.DeviceVersion,

        di.UserDefinedName);

    // 检查是否和我们的标识字段匹配
    if (config_.user_id == di.UserDefinedName ||
        config_.serial_number == di.SerialNumber ||
        config_.user_defined_name == di.UserDefinedName ||
        config_.device_name == di.FullName ||
        config_.friendly_name == di.FriendlyName) {
      deviceIndex = i;
      found = true;
      break;
    }
  }

  if (!found) {
    std::cerr << "No camera found with user ID: " << config_.user_id
              << std::endl;

    // 如果没找到指定ID的相机，尝试使用第一个可用的相机
    std::cerr << "Trying to open the first available camera as fallback"
              << std::endl;
    deviceIndex = 0;
  }

  // 打开相机设备
  status = ItkDevOpen(deviceIndex, ITKDEV_VAL_ACCESS_MODE_CONTROL, &handle);
  if (status != ITKSTATUS_OK) {
    std::cerr << "Failed to open IKap camera at index " << deviceIndex
              << ", error code: " << status << std::endl;
    ItkManTerminate();
    return nullptr;
  }

  // 转换配置
  IkapConfig cfg = toIkapConfig();

  // 创建捕获对象
  auto capture = std::make_unique<IkapCameraCapture>(handle);

  // 应用基础配置到SDK
  if (cfg.exposure_us > 0) {
    ITKSTATUS exp_status =
        ItkDevSetDouble(handle, "ExposureTime", cfg.exposure_us);
    if (exp_status != ITKSTATUS_OK) {
      std::cerr << "Failed to set exposure, error code: " << exp_status
                << std::endl;
    }
  }

  if (cfg.gain > 0) {
    ITKSTATUS gain_status = ItkDevSetDouble(handle, "Gain", cfg.gain);
    if (gain_status != ITKSTATUS_OK) {
      std::cerr << "Failed to set gain, error code: " << gain_status
                << std::endl;
    }
  }

  if (cfg.roi_w > 0 && cfg.roi_h > 0) {
    ITKSTATUS roi_status;
    roi_status = ItkDevSetInt64(handle, "Width", cfg.roi_w);
    if (roi_status != ITKSTATUS_OK) {
      std::cerr << "Failed to set ROI width, error code: " << roi_status
                << std::endl;
    }

    roi_status = ItkDevSetInt64(handle, "Height", cfg.roi_h);
    if (roi_status != ITKSTATUS_OK) {
      std::cerr << "Failed to set ROI height, error code: " << roi_status
                << std::endl;
    }

    roi_status = ItkDevSetInt64(handle, "OffsetX", cfg.roi_x);
    if (roi_status != ITKSTATUS_OK) {
      std::cerr << "Failed to set ROI offset X, error code: " << roi_status
                << std::endl;
    }

    roi_status = ItkDevSetInt64(handle, "OffsetY", cfg.roi_y);
    if (roi_status != ITKSTATUS_OK) {
      std::cerr << "Failed to set ROI offset Y, error code: " << roi_status
                << std::endl;
    }
  }

  // 应用IKAP特有配置（新版接口）
  ItkDevSetInt64(handle, "StartMode", cfg.start_mode);
  ItkDevSetInt64(handle, "TransferMode", cfg.transfer_mode);
  ItkDevSetInt64(handle, "GrabStrategy", cfg.grab_strategy);
  ItkDevSetInt64(handle, "AutoClear", cfg.auto_clear ? 1 : 0);

  // 视图参数（新版接口，直接用设备句柄访问）
  ItkDevSetInt64(handle, "FlipX", cfg.flip_x ? 1 : 0);
  ItkDevSetInt64(handle, "FlipY", cfg.flip_y ? 1 : 0);
  ItkDevSetInt64(handle, "ZoomMethod", cfg.zoom_method);
  ItkDevFromString(handle, "WindowTitle", cfg.window_title.c_str());

  // 应用其他参数（新版接口）
  ItkDevSetInt64(handle, "TriggerMode", cfg.trigger_mode ? 1 : 0);
  ItkDevSetInt64(handle, "AutoExposure", cfg.auto_exposure ? 1 : 0);
  ItkDevSetInt64(handle, "AutoGain", cfg.auto_gain ? 1 : 0);

  // 将配置传给Capture对象
  capture->set_config(cfg);

  // 注册帧处理器
  if (!config_.frame_processors.empty()) {
    capture->start(config_.frame_processors.back());
  } else {
    capture->start();
  }

  // 注册事件处理器
  for (const auto& [event, handler] : config_.event_handlers) {
    capture->register_event_handler(event, handler);
  }

  return capture;
}
