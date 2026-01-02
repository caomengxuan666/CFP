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
 *  - File: IkapCameraBuilder.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// IkapCameraBuilder.hpp
#pragma once
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "cameras/FrameProcessor.hpp"
#include "cameras/Ikap/IkapCameraCapture.hpp"
#include "cameras/Ikap/IkapConfig.hpp"
#include "cameras/Ikap/IkapEventManager.hpp"

class IkapCameraBuilder {
 public:
  // 静态工厂方法（和DVP一致：fromUserId/fromFriendlyName）
  static IkapCameraBuilder fromUserId(const std::string& id);
  static IkapCameraBuilder fromDeviceName(const std::string& id);
  static IkapCameraBuilder fromFriendlyName(const std::string& id);
  static IkapCameraBuilder fromSerialNumber(const std::string& id);
  static IkapCameraBuilder fromUserDefinedName(const std::string& name);

  // === 链式配置API：完全对齐你DvpCameraBuilder的方法名，参数一致
  IkapCameraBuilder& roi(int x, int y, int w, int h);
  IkapCameraBuilder& exposure(double us);
  IkapCameraBuilder& gain(float gain);
  IkapCameraBuilder& hardwareIsp(bool enable);
  IkapCameraBuilder& triggerMode(bool enable);
  IkapCameraBuilder& autoExposure(bool enable);
  IkapCameraBuilder& autoGain(bool enable);
  IkapCameraBuilder& aeTargetBrightness(int brightness);
  IkapCameraBuilder& antiFlickerMode(int mode);
  IkapCameraBuilder& acquisitionMode(int mode);
  IkapCameraBuilder& contrast(int contrast);
  IkapCameraBuilder& gamma(int gamma);
  IkapCameraBuilder& saturation(int saturation);
  IkapCameraBuilder& sharpness(int sharpness);
  IkapCameraBuilder& sharpnessEnable(bool enable);
  IkapCameraBuilder& inverseState(bool enable);
  IkapCameraBuilder& flipHorizontalState(bool enable);
  IkapCameraBuilder& flipVerticalState(bool enable);
  IkapCameraBuilder& rotateState(bool enable);
  IkapCameraBuilder& rotateOpposite(bool enable);
  IkapCameraBuilder& coolerState(bool enable);
  IkapCameraBuilder& bufferQueueSize(int size);

  // === IKAP特有参数链式配置（扩展方法，命名风格和DVP一致）
  IkapCameraBuilder& startMode(uint32_t mode);         // 阻塞/非阻塞
  IkapCameraBuilder& transferMode(uint32_t mode);      // 传输模式
  IkapCameraBuilder& grabStrategy(uint32_t strategy);  // 取图策略
  IkapCameraBuilder& autoClear(bool enable);
  IkapCameraBuilder& flipX(bool enable);
  IkapCameraBuilder& flipY(bool enable);
  IkapCameraBuilder& zoomMethod(int method);
  IkapCameraBuilder& windowTitle(const std::string& title);
  IkapCameraBuilder& batchMode(bool enable);
  IkapCameraBuilder& batchFrameCount(uint32_t count);
  IkapCameraBuilder& streamFlowCtrl(uint32_t value);
  IkapCameraBuilder& linkTimeout(uint32_t timeout);

  // === 事件/帧处理器配置（和DVP完全一致）
  IkapCameraBuilder& onFrame(const FrameProcessor& proc);
  IkapCameraBuilder& onEvent(const IkapEventType event,
                             const IkapEventHandler& handler);

  // === 构建Capture对象（核心方法，和DVP一致）
  std::unique_ptr<IkapCameraCapture> build();

  // === 转IkapConfig（和DVP的toDvpConfig()完全一致）
  IkapConfig toIkapConfig() const;

 private:
  // 内部配置存储（和DVP一致，用optional保存未配置的参数）
  struct Config {
    std::string user_id;
    std::string device_name;
    std::string friendly_name;
    std::string serial_number;
    std::string user_defined_name;
    std::optional<double> exposure_us;
    std::optional<float> gain;
    std::optional<int> roi_x, roi_y, roi_w, roi_h;
    std::optional<bool> hardware_isp;
    std::optional<bool> trigger_mode;
    std::optional<bool> auto_exposure;
    std::optional<bool> auto_gain;
    std::optional<int> ae_target_brightness;
    std::optional<int> anti_flicker_mode;
    std::optional<int> acquisition_mode;
    std::optional<int> contrast;
    std::optional<int> gamma;
    std::optional<int> saturation;
    std::optional<int> sharpness;
    std::optional<bool> sharpness_enable;
    std::optional<bool> inverse_state;
    std::optional<bool> flip_horizontal_state;
    std::optional<bool> flip_vertical_state;
    std::optional<bool> rotate_state;
    std::optional<bool> rotate_opposite;
    std::optional<bool> cooler_state;
    std::optional<int> buffer_queue_size;

    // IKAP特有参数
    std::optional<uint32_t> start_mode;
    std::optional<uint32_t> transfer_mode;
    std::optional<uint32_t> grab_strategy;
    std::optional<bool> auto_clear;
    std::optional<bool> flip_x;
    std::optional<bool> flip_y;
    std::optional<int> zoom_method;
    std::optional<std::string> window_title;
    std::optional<bool> batch_mode;
    std::optional<uint32_t> batch_frame_count;
    std::optional<uint32_t> stream_flow_ctrl;
    std::optional<uint32_t> link_timeout;

    // 事件/帧处理器
    std::vector<FrameProcessor> frame_processors;
    std::unordered_map<IkapEventType, IkapEventHandler> event_handlers;
  };
  Config config_;
};
