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
 *  - File: IkapCameraCapture.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#include "cameras/IKap/IkapCameraCapture.hpp"

#include <iostream>
#include <memory>

#include "IKapC.h"
#include "cameras/Ikap/IkapConfig.hpp"
#include "protocol/messages.hpp"

IkapCameraCapture::IkapCameraCapture(ITKDEVICE handle) : handle_(handle) {
  if (handle_) {
    config_ = std::make_shared<IkapConfig>();  // 初始化为IkapConfig
    event_manager_ = std::make_unique<IkapEventManager>(handle_);
  }
}

void IkapCameraCapture::set_config(const CameraConfig& cfg) {
  IkapConfig ikap_cfg(cfg);  // 用继承构造
  set_config(ikap_cfg);
}

void IkapCameraCapture::set_config(const IkapConfig& cfg) {
  {
    std::unique_lock lock(config_mutex_);
    *config_ = cfg;
  }
  update_camera_params();
}
IkapConfig IkapCameraCapture::get_config() const {
  std::shared_lock lock(config_mutex_);
  return *config_;
}

IkapCameraCapture::~IkapCameraCapture() {
  if (handle_) {
    stop();
    ItkDevClose(handle_);
  }
}

bool IkapCameraCapture::start() {
  if (!handle_ || running_.load()) {
    return false;
  }

  uint32_t stream_count = 0;
  if (ItkDevGetStreamCount(handle_, &stream_count) != ITKSTATUS_OK ||
      stream_count == 0) {
    std::cerr << "No streams available\n";
    return false;
  }

  if (ItkDevAllocStreamEx(handle_, 0, 10, &stream_handle_) != ITKSTATUS_OK) {
    std::cerr << "Failed to allocate stream\n";
    return false;
  }

  // 循环注册所有流事件
  for (const auto& [sdk_event, ikap_event] :
       event_manager_->get_stream_event_map()) {
    ItkStreamRegisterCallback(stream_handle_, sdk_event,
                              event_manager_->get_stream_event_callback(),
                              this);  // 传Capture指针给回调
  }

  // 注册帧处理器的流回调事件,当这个函数被调用，说明已经获取到一帧数据了,直接处理
  event_manager_->register_handler(
      IkapEventType::FrameAvailable, [this](const IkapEventContext& ctx) {
        if (ctx.stream_event_type == ITKSTREAM_VAL_EVENT_TYPE_END_OF_FRAME &&
            running_.load()) {
          ITKBUFFER buffer = nullptr;
          if (ItkStreamGetCurrentBuffer(stream_handle_, &buffer) ==
                  ITKSTATUS_OK &&
              buffer) {
            process_frame(buffer);
          }
        }
      });

  if (ItkStreamStart(stream_handle_, ITKSTREAM_CONTINUOUS) != ITKSTATUS_OK) {
    std::cerr << "Failed to start stream\n";
    ItkDevFreeStream(stream_handle_);
    stream_handle_ = nullptr;
    return false;
  }

  running_.store(true);
  protocol::FrontendStatus initial_status{};
  initial_status.self_check = true;
  initial_status.capture = true;
  initial_status.file_io = true;
  update_status(initial_status);

  return true;
}

bool IkapCameraCapture::start(const FrameProcessor& processor) {
  user_processor_ = processor;

  return start();
}

void IkapCameraCapture::stop() {
  if (!running_.load()) {
    return;
  }
  running_.store(false);

  if (stream_handle_) {
    ItkStreamStop(stream_handle_);
    // 注销所有流事件
    for (const auto& [sdk_event, ikap_event] :
         event_manager_->get_stream_event_map()) {
      ItkStreamUnregisterCallback(stream_handle_, sdk_event);
    }
    ItkDevFreeStream(stream_handle_);
    stream_handle_ = nullptr;
  }

  auto status = get_status();
  status.capture = false;
  update_status(status);
}

void IkapCameraCapture::set_roi(int x, int y, int width, int height) {
  std::unique_lock lock(config_mutex_);
  config_->roi_x = x;
  config_->roi_y = y;
  config_->roi_w = width;
  config_->roi_h = height;
  update_camera_params();
}

protocol::FrontendStatus IkapCameraCapture::get_status() const {
  std::shared_lock lock(status_mutex_);
  return current_status_;
}

void IkapCameraCapture::update_status(
    const protocol::FrontendStatus& new_status) {
  std::unique_lock lock(status_mutex_);
  current_status_ = new_status;
}

void IkapCameraCapture::update_camera_params() {
  std::shared_lock lock(config_mutex_);
  if (!config_ || !handle_) return;

  // 1. 通用参数更新（新版接口）
  if (config_->exposure_us > 0)
    ItkDevSetDouble(handle_, "ExposureTime", config_->exposure_us);
  if (config_->gain > 0) ItkDevSetDouble(handle_, "Gain", config_->gain);
  if (config_->roi_w > 0 && config_->roi_h > 0) {
    ItkDevSetInt64(handle_, "OffsetX", config_->roi_x);
    ItkDevSetInt64(handle_, "OffsetY", config_->roi_y);
    ItkDevSetInt64(handle_, "Width", config_->roi_w);
    ItkDevSetInt64(handle_, "Height", config_->roi_h);
  }
  ItkDevSetInt64(handle_, "TriggerMode", config_->trigger_mode ? 1 : 0);

  // 2. IKAP特有参数更新（新版接口使用字符串特征名）
  ItkDevSetInt64(handle_, "StartMode", config_->start_mode);
  ItkDevSetInt64(handle_, "TransferMode", config_->transfer_mode);
  ItkDevSetInt64(handle_, "GrabStrategy", config_->grab_strategy);
  ItkDevSetInt64(handle_, "AutoClear", config_->auto_clear ? 1 : 0);

  // 3. 视图参数（新版接口直接用设备句柄访问）
  ItkDevSetInt64(handle_, "FlipX", config_->flip_x ? 1 : 0);
  ItkDevSetInt64(handle_, "FlipY", config_->flip_y ? 1 : 0);
  ItkDevSetInt64(handle_, "ZoomMethod", config_->zoom_method);
  ItkDevFromString(handle_, "WindowTitle", config_->window_title.c_str());

  // 4. 自动参数/图像增强参数
  ItkDevSetInt64(handle_, "AutoExposure", config_->auto_exposure ? 1 : 0);
  ItkDevSetInt64(handle_, "AutoGain", config_->auto_gain ? 1 : 0);
  ItkDevSetInt64(handle_, "Contrast", config_->contrast);
  ItkDevSetInt64(handle_, "Gamma", config_->gamma);
  ItkDevSetInt64(handle_, "Saturation", config_->saturation);
  ItkDevSetInt64(handle_, "Sharpness", config_->sharpness);
  ItkDevSetInt64(handle_, "SharpnessEnable", config_->sharpness_enable ? 1 : 0);
  ItkDevSetInt64(handle_, "InverseState", config_->inverse_state ? 1 : 0);
  ItkDevSetInt64(handle_, "FlipHorizontalState",
                 config_->flip_horizontal_state ? 1 : 0);
  ItkDevSetInt64(handle_, "FlipVerticalState",
                 config_->flip_vertical_state ? 1 : 0);
  ItkDevSetInt64(handle_, "RotateState", config_->rotate_state ? 1 : 0);
  ItkDevSetInt64(handle_, "RotateOpposite", config_->rotate_opposite ? 1 : 0);
  ItkDevSetInt64(handle_, "CoolerState", config_->cooler_state ? 1 : 0);
  ItkDevSetInt64(handle_, "BufferQueueSize", config_->buffer_queue_size);
}

void IkapCameraCapture::process_frame(ITKBUFFER buffer) {
  ITK_BUFFER_INFO info = {};
  if (ItkBufferGetInfo(buffer, &info) != ITKSTATUS_OK) {
    return;
  }
  if (!info.ImageAddress || info.ImageSize == 0) {
    return;
  }

  auto captured = std::make_shared<CapturedFrame>();
  captured->data.resize(info.ImageSize);
  std::memcpy(captured->data.data(), info.ImageAddress, info.ImageSize);

  captured->meta.iWidth = static_cast<int>(info.ImageWidth);
  captured->meta.iHeight = static_cast<int>(info.ImageHeight);
  captured->meta.fExposure = config_ ? config_->exposure_us : 0.0;
  captured->meta.fGain = config_ ? config_->gain : 1.0;
  captured->meta.uTimestamp = info.TimestampNs;
  captured->meta.cameraId = "IKAP_Camera";

  frame_queue_.enqueue(captured);
  thread_pool_.detach_task(
      [this, captured]() { user_processor_.process(*captured); });
}

void IkapCameraCapture::register_event_handler(IkapEventType type,
                                               IkapEventHandler handler) {
  event_manager_->register_handler(type, handler);
}

void IkapCameraCapture::add_frame_processor(const FrameProcessor& processor) {
  user_processor_ = processor;  // 和DVP完全一致，保存用户帧处理器
}
