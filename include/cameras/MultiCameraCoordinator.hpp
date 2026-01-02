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
 *  - File: MultiCameraCoordinator.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "cameras/FrameProcessor.hpp"

class CapturedFrame;

// MultiCameraCoordinator.hpp
class MultiCameraCoordinator {
 public:
  using FusionFunc =
      std::function<CapturedFrame(const std::vector<CapturedFrame>&)>;

  MultiCameraCoordinator(size_t num_cams, FusionFunc fuse_func)
      : frames_(num_cams),
        received_(num_cams, false),
        fuse_func_(std::move(fuse_func)) {}

  // 返回 FrameProcessor（实际是 FunctionFrameProcessor）
  FrameProcessor make_processor_for(size_t cam_index) {
    // 创建 lambda
    auto lambda = [this, cam_index](const CapturedFrame& frame) {
      this->on_frame(cam_index, frame);
    };
    // 用工具函数包装成 FrameProcessor
    return make_function_processor(std::move(lambda));
  }

  // 重载make_processor_for的括号函数
  FrameProcessor operator[](size_t cam_index) {
    return make_processor_for(cam_index);
  }

 private:
  void on_frame(size_t cam_index, const CapturedFrame& frame) {
    std::lock_guard lock(mutex_);
    frames_[cam_index] = frame;
    received_[cam_index] = true;

    if (std::all_of(received_.begin(), received_.end(),
                    [](bool b) { return b; })) {
      // 所有帧到齐，执行融合
      auto fused = fuse_func_(frames_);

      // 融合后做什么？例如：
      // - 交给算法处理
      // - 发送到 ImageSignalBus
      // - 入队供 GUI 显示

      // 处理完后，最后还是要交由通用算法处理
      if (downstream_processor_) {
        downstream_processor_->process(fused);
      }

      // 重置状态（或加超时机制）
      std::fill(received_.begin(), received_.end(), false);
    }
  }

  std::vector<CapturedFrame> frames_;
  std::vector<bool> received_;
  FusionFunc fuse_func_;
  std::unique_ptr<FrameProcessor> downstream_processor_;
  std::mutex mutex_;

 public:
  void set_downstream_processor(std::unique_ptr<FrameProcessor> proc) {
    downstream_processor_ = std::move(proc);
  }
};
