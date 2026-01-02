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
 *  - File: IkapConfig.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once

#include "IKapC.h"
#include "config/CameraConfig.hpp"

struct IkapConfig : public CameraConfig {
  // IKAP特有参数（从IKap SDK宏定义提取）
  uint32_t start_mode =
      ITKSTREAM_VAL_START_MODE_NON_BLOCK;  // 采集模式：阻塞/非阻塞
  uint32_t transfer_mode =
      ITKSTREAM_VAL_TRANSFER_MODE_ASYNCHRONOUS;                     // 传输模式
  uint32_t grab_strategy = ITKSTREAM_VAL_GRAB_STRATEGY_ONE_BY_ONE;  // 取图策略
  bool auto_clear = true;  // 自动清空缓冲区

  // 视图参数（ITKVIEW_PRM_*系列）
  bool flip_x = false;
  bool flip_y = false;
  int lsb = 0, msb = 12;
  int zoom_method = ITKVIEW_VAL_ZOOM_NN;  // 缩放算法
  std::string window_title = "IKAP_Camera_View";

  // 批量采集/流恢复相关
  bool batch_mode = false;
  uint32_t batch_frame_count = ITKSTREAM_CONTINUOUS;

  // IKAP特有参数
  uint32_t stream_flow_ctrl = 0;  // 流控开关
  uint32_t link_timeout = 5000;   // 链接超时
  bool cooler_state = false;      // 制冷开关
  int buffer_queue_size = 10;     // 缓冲区队列大小

  IkapConfig clone() const {
    IkapConfig result = *this;
    // 复制基类部分
    static_cast<CameraConfig&>(result) =
        static_cast<const CameraConfig&>(*this);
    return result;
  }
};
