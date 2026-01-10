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
 *  - File: CameraCapture.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once

#include <memory>

#include "concurrentqueue.h"
#include "protocol/messages.hpp"
class FrameProcessor;
struct CameraConfig;  // 声明CameraConfig结构
class CapturedFrame;

class CameraCapture {
 public:
  virtual ~CameraCapture() = default;
  virtual bool start() = 0;
  virtual bool start(const FrameProcessor&) = 0;
  virtual void stop() = 0;
  virtual void set_config(const CameraConfig&) = 0;
  virtual void set_roi(int x, int y, int width, int height) = 0;
  virtual moodycamel::ConcurrentQueue<std::shared_ptr<CapturedFrame>>&
  get_frame_queue() = 0;
  virtual protocol::FrontendStatus get_status() const = 0;
  virtual void add_frame_processor(const FrameProcessor& processor) = 0;
};
