/*
 *  Copyright © 2025 [caomengxuan666]
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
 *  - File: messages.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025
 */

// Copyright (c) 2025 caomengxuan666
#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace protocol {

/// @brief 服务器下发配置（兼容 7000/19800/常铝/紫金 所有现场）
struct ServerConfig {
  // 通用字段（所有现场都发送）
  std::string roll_id;                // 72字节
  std::string brand;                  // 20字节
  std::string thickness_str;          // 5字节字符串
  std::string min_defect_length_str;  // 5字节
  std::string min_defect_area_str;    // 5字节
  float head_length = 0.0f;           // 4字节

  // 可选字段（各现场差异）
  std::optional<int32_t> material_type;  // 4字节（常铝/紫金不发送）
  std::optional<std::array<float, 20>> segmentation_params;  // 80字节
  std::optional<int32_t> upper_surface_id;                   // 4字节
  std::optional<std::array<float, 16>> upper_large_params;   // 4*4字节
  std::optional<int32_t> lower_surface_id;                   // 4字节
  std::optional<std::array<float, 16>> lower_large_params;   // 4*4字节
  std::optional<int32_t> cutting_count;                      // 4字节
  // ... 分切方案（暂时应该不需要）
};

/// @brief 前端机上报特征
struct FeatureReport {
  std::string roll_id;                              // 72字节
  std::vector<std::pair<int32_t, float>> features;  // 特征列表
  std::array<float, 20> special_images;             // 20个特殊图片信息
  std::vector<uint8_t> image_data;                  // 图片数据
};

/// @brief 前端机状态（32位）
struct FrontendStatus {
  bool self_check = false;     // index1: 系统自检状态
  bool capture = false;        // index2: 采集状态
  bool file_io = false;        // index3: 文件读写
  bool image_anomaly = false;  // index6: 图像异常

  /// 转换为 32 位状态整数（按协议要求）
  uint32_t to_uint32() const {
    uint32_t status = 0;
    if (self_check) status |= (1U << 1);
    if (capture) status |= (1U << 2);
    if (file_io) status |= (1U << 3);
    if (image_anomaly) status |= (1U << 6);
    return status;
  }
};

}  // namespace protocol
