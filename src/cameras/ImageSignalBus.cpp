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
 *  - File: ImageSignalBus.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// ImageSignalBus.cpp
#include "cameras/ImageSignalBus.hpp"  //NOLINT

#include <string>
#include <utility>
#include <vector>

// For mat
#include <opencv2/core/mat.hpp>

void ImageSignalBus::declare_signal(const std::string& signal_name) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  // 声明信号（即使没有订阅者也要记录，便于 UI 发现）
  if (subscribers_.find(signal_name) == subscribers_.end()) {
    subscribers_[signal_name] = std::vector<ImageCallback>();
  }
}

void ImageSignalBus::subscribe(const std::string& signal_name,
                               ImageCallback callback) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  subscribers_[signal_name].push_back(std::move(callback));
}

void ImageSignalBus::emit(const std::string& signal_name, const cv::Mat& img) {
  if (img.empty()) {
    return;
  }

  std::shared_lock<std::shared_mutex> lock(mutex_);
  auto it = subscribers_.find(signal_name);
  if (it != subscribers_.end()) {
    // 对每个订阅者发送**深拷贝**的图像
    for (const auto& callback : it->second) {
      if (callback) {
        callback(img.clone());  // 深拷贝确保生命周期安全
      }
    }
  }
}

void ImageSignalBus::subscribe_feature(const std::string& name,
                                       FeatureCallback cb) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  feature_subscribers_[name].push_back(std::move(cb));
}

void ImageSignalBus::subscribe_status(const std::string& name,
                                      StatusCallback cb) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  status_subscribers_[name].push_back(std::move(cb));
}

void ImageSignalBus::emit_feature(const std::string& name,
                                  const FeatureData& data) {
  std::shared_lock<std::shared_mutex> lock(mutex_);
  if (auto it = feature_subscribers_.find(name);
      it != feature_subscribers_.end()) {
    for (const auto& cb : it->second) {
      if (cb) {
        cb(data);
      }
    }
  }
}

void ImageSignalBus::emit_status(const std::string& name,
                                 const StatusData& data) {
  std::shared_lock<std::shared_mutex> lock(mutex_);
  if (auto it = status_subscribers_.find(name);
      it != status_subscribers_.end()) {
    for (const auto& cb : it->second) {
      if (cb) {
        cb(data);
      }
    }
  }
}
