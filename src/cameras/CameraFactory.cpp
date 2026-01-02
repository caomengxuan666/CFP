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
 *  - File: CameraFactory.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#include "cameras/CameraFactory.hpp"

#include <memory>
#include <string>

#include "cameras/Dvp/DvpCameraBuilder.hpp"
#include "cameras/Ikap/IkapCameraBuilder.hpp"
#include "config/CameraConfig.hpp"

std::unique_ptr<CameraCapture> create_camera(const CameraBrand& brand,
                                             const std::string& identifier,
                                             const CameraConfig& config) {
  switch (brand) {
    case CameraBrand::DVP: {
      // 使用DvpCameraBuilder创建DVP相机
      auto builder = DvpCameraBuilder::fromUserId(identifier);
      // 应用配置
      builder.exposure(config.exposure_us)
          .gain(config.gain)
          .roi(config.roi_x, config.roi_y, config.roi_w, config.roi_h)
          .hardwareIsp(config.hardware_isp)
          .autoExposure(config.auto_exposure)
          .autoGain(config.auto_gain)
          .aeTargetBrightness(config.ae_target_brightness)
          .antiFlickerMode(config.anti_flicker_mode)
          .acquisitionMode(config.acquisition_mode)
          .contrast(config.contrast)
          .gamma(config.gamma)
          .saturation(config.saturation)
          .sharpness(config.sharpness);

      // 构建并返回相机
      return std::unique_ptr<CameraCapture>(builder.build());
    }
    case CameraBrand::IKap: {
      // 使用IkapCameraBuilder创建IKAP相机
      auto builder = IkapCameraBuilder::fromUserId(identifier);
      // 应用配置
      builder.exposure(config.exposure_us)
          .gain(config.gain)
          .roi(config.roi_x, config.roi_y, config.roi_w, config.roi_h)
          .hardwareIsp(config.hardware_isp)
          .autoExposure(config.auto_exposure)
          .autoGain(config.auto_gain)
          .aeTargetBrightness(config.ae_target_brightness)
          .antiFlickerMode(config.anti_flicker_mode)
          .acquisitionMode(config.acquisition_mode)
          .contrast(config.contrast)
          .gamma(config.gamma)
          .saturation(config.saturation)
          .sharpness(config.sharpness)
          .sharpnessEnable(config.sharpness_enable)
          .inverseState(config.inverse_state)
          .flipHorizontalState(config.flip_horizontal_state)
          .flipVerticalState(config.flip_vertical_state)
          .rotateState(config.rotate_state)
          .rotateOpposite(config.rotate_opposite)
          .coolerState(config.cooler_state)
          .bufferQueueSize(config.buffer_queue_size);

      // 构建并返回相机
      return std::unique_ptr<CameraCapture>(builder.build());
    }
    case CameraBrand::MIND: {
      // TODO: 实现Mind相机的创建逻辑
      // 这里需要Mind相机的具体实现
      break;
    }
  }
  return nullptr;
}
