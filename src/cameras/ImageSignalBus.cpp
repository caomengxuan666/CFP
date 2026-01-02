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

#include "cameras/ImageSignalBus.hpp"

#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::unordered_map<std::string, std::unique_ptr<ImageSignalBus>>
    ImageSignalBus::instances_;
std::shared_mutex ImageSignalBus::instances_mutex_;

ImageSignalBus& ImageSignalBus::instance(const std::string& ns) {
  std::unique_lock<std::shared_mutex> lock(instances_mutex_);
  auto it = instances_.find(ns);
  if (it == instances_.end()) {
    // 手动创建，不会调用默认构造函数
    it = instances_
             .emplace(ns, std::unique_ptr<ImageSignalBus>(new ImageSignalBus()))
             .first;
  }
  return *it->second;
}

void ImageSignalBus::declare_signal(const std::string& signal_name) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
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
    for (const auto& callback : it->second) {
      if (callback) {
        callback(img.clone());
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
