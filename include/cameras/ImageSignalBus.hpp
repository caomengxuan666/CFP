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
 *  - File: ImageSignalBus.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// ImageSignalBus.hpp
#pragma once
#include <array>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>


// cv
#include <opencv2/core/mat.hpp>

class ImageSignalBus {
 public:
  using ImageCallback = std::function<void(const cv::Mat&)>;

  struct FeatureData {
    std::string roll_id;
    std::vector<std::pair<int, float>> features;
    std::array<float, 20> special_images;
    std::string camera_id;
  };

  struct StatusData {
    bool self_check = false;
    bool capture = false;
    bool file_io = false;
    bool image_anomaly = false;
  };

  using FeatureCallback = std::function<void(const FeatureData&)>;
  using StatusCallback = std::function<void(const StatusData&)>;

  // 单例
  /**
   * @brief 获取ImageSignalBus实例
   * 
   * @param ns 命名空间,默认为default
   * @note 当我们后续使用一台电脑控制多个相机实例的时候，可以更轻松区分信号的命名空间
   * @return ImageSignalBus& 
   */
  
  static ImageSignalBus& instance(const std::string& ns = "default");

  // 算法调用：声明自己能提供哪些信号
  void declare_signal(const std::string& signal_name);

  // UI 或其他模块调用：订阅某个信号
  void subscribe(const std::string& signal_name, ImageCallback callback);

  // 算法内部调用：广播图像（自动深拷贝）
  void emit(const std::string& signal_name, const cv::Mat& img);

  // 与服务器之间通信：订阅特征和状态
  void subscribe_feature(const std::string& name, FeatureCallback cb);
  void subscribe_status(const std::string& name, StatusCallback cb);
  void emit_feature(const std::string& name, const FeatureData& data);
  void emit_status(const std::string& name, const StatusData& data);

 private:
  ImageSignalBus() = default;
  ImageSignalBus(const ImageSignalBus&) = delete;
  ImageSignalBus& operator=(const ImageSignalBus&) = delete;

  // 图像信号
  std::unordered_map<std::string, std::vector<ImageCallback>> subscribers_;
  // 数据信号
  std::unordered_map<std::string, std::vector<FeatureCallback>>
      feature_subscribers_;
  std::unordered_map<std::string, std::vector<StatusCallback>>
      status_subscribers_;

  mutable std::shared_mutex mutex_;

  // 改成存储 unique_ptr
  static std::unordered_map<std::string, std::unique_ptr<ImageSignalBus>>
      instances_;
  static std::shared_mutex instances_mutex_;
};
