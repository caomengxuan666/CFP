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
 *  - File: IkapCameraCapture.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once
#include <atomic>
#include <memory>
#include <shared_mutex>
#include <thread>

#include "BS_thread_pool.hpp"
#include "IKapCDef.h"
#include "cameras/CameraCapture.hpp"
#include "cameras/FrameProcessor.hpp"
#include "cameras/Ikap/IkapConfig.hpp"
#include "cameras/Ikap/IkapEventManager.hpp"
#include "concurrentqueue.h"
#include "protocol/messages.hpp"

class IkapCameraCapture : public CameraCapture {
 public:
  explicit IkapCameraCapture(ITKDEVICE handle);
  ~IkapCameraCapture() override;

  bool start() override;
  bool start(const FrameProcessor& processor) override;
  void stop() override;
  void set_config(const CameraConfig& cfg) override;  // 通用接口，兼容基类
  void set_config(const IkapConfig& cfg);             // 专属接口，和DVP一致
  IkapConfig get_config() const;                      // 专属获取，和DVP一致
  void set_roi(int x, int y, int width, int height) override;

  void register_event_handler(IkapEventType type, IkapEventHandler handler);
  void add_frame_processor(const FrameProcessor& processor) override;
  protocol::FrontendStatus get_status() const override;

  IkapEventManager* get_event_manager() const { return event_manager_.get(); }
  moodycamel::ConcurrentQueue<std::shared_ptr<CapturedFrame>>& get_frame_queue()
      override {
    return frame_queue_;
  }

  ITKSTREAM stream_handle_ = nullptr;  // 暴露给事件回调使用

 private:
  void process_frame(ITKBUFFER buffer);
  void update_camera_params();
  void update_status(const protocol::FrontendStatus& status);

  protocol::FrontendStatus current_status_;
  ITKDEVICE handle_ = nullptr;
  std::atomic<bool> running_{false};

  std::unique_ptr<IkapEventManager> event_manager_;
  std::shared_ptr<IkapConfig> config_;  // 替换为IkapConfig，和DVP的config_一致
  mutable std::shared_mutex config_mutex_;
  mutable std::shared_mutex status_mutex_;
  moodycamel::ConcurrentQueue<std::shared_ptr<CapturedFrame>> frame_queue_;

  BS::thread_pool<> thread_pool_{std::thread::hardware_concurrency()};
  FrameProcessor user_processor_;
};
