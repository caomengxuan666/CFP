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
 *  - File: CameraManager.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cameras/CameraCapture.hpp"
#include "config/CameraConfig.hpp"

// 通用模板相机管理器，用于管理特定类型的相机捕获对象
template <typename T>
class GenericCameraManager {
  static_assert(std::is_base_of_v<CameraCapture, T>,
                "T must inherit from CameraCapture");

 public:
  // 禁用默认构造（或允许空管理器）
  GenericCameraManager() {
    // 大概率就4个，预留一点空间,最多的情况是8个
    cameras_.reserve(4);
  }

  explicit GenericCameraManager(size_t number_of_cameras) {
    // 提前预留，可以防止临时扩容造成的迭代器失效
    // 虽然我真的不确定我以后会不会用到这里的迭代器
    cameras_.reserve(number_of_cameras);
  }

  ~GenericCameraManager() {
    stop_all();
    cameras_.clear();
  }

  // 添加相机：接收 capture，立即构建并存储 shared_ptr
  void add_camera(std::unique_ptr<T> cam) {
    if (cam) {
      // 转为 shared_ptr 存储（因为需要共享给 GUI/算法）
      cameras_.emplace_back(std::move(cam));
    }
  }

  // 动态移除相机
  void remove_camera(size_t index) {
    if (index < cameras_.size()) {
      cameras_.erase(cameras_.begin() + static_cast<int>(index));
    }
  }

  // 获取相机（用于 GUI、算法等模块）
  std::shared_ptr<T> get_camera(size_t index) const {
    if (index < cameras_.size()) {
      return cameras_[index];
    }
    return nullptr;
  }

  size_t camera_count() const { return cameras_.size(); }

  // 启动所有相机
  void start_all() {
    for (auto& cam : cameras_) {
      // 我们这里直接启动，我们已经在builder中设置了他们的帧处理器
      if (!cam->start(cam->get_frame_processor())) {
        // 可选：记录失败，或抛异常
        std::cerr << "Failed to start camera\n";
      }
    }
  }

  void stop_all() {
    for (auto& cam : cameras_) {
      cam->stop();
    }
  }

 private:
  std::vector<std::shared_ptr<T>> cameras_;
};

// 传统的基于ID的相机管理器，用于需要配置和更复杂管理的场景
class CameraManager {
 public:
  CameraManager() = default;
  ~CameraManager() = default;

  // 相机管理
  bool add_camera(const std::string& camera_id,
                  std::unique_ptr<CameraCapture> capture);
  void remove_camera(const std::string& camera_id);
  bool has_camera(const std::string& camera_id) const;
  CameraCapture* get_camera(const std::string& camera_id) const;
  std::vector<std::string> get_all_camera_ids() const;

  // 相机配置管理
  void set_camera_config(const std::string& camera_id,
                         const CameraConfig& config);
  CameraConfig get_camera_config(const std::string& camera_id) const;
  bool save_camera_config(const std::string& camera_id,
                          const std::string& file_path) const;
  bool load_camera_config(const std::string& camera_id,
                          const std::string& file_path);

  // 相机操作
  bool start_camera(const std::string& camera_id,
                    const FrameProcessor& processor);
  void stop_camera(const std::string& camera_id);

 private:
  struct CameraData {
    std::unique_ptr<CameraCapture> capture;
    CameraConfig config;
    std::string config_path;

    // 添加默认构造函数
    CameraData() = default;
    explicit CameraData(std::unique_ptr<CameraCapture> cap)
        : capture(std::move(cap)) {}
  };

  std::unordered_map<std::string, std::unique_ptr<CameraData>> cameras_;
};
