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

// cameras/CameraFactory.cpp
#include "cameras/CameraFactory.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cameras/Dvp/DvpCameraBuilder.hpp"
#include "cameras/EventHandlerRegistry.hpp"
#include "cameras/Ikap/IkapCameraBuilder.hpp"
#include "config/GlobalConfig.hpp"
#include "logging/CaponLogging.hpp"
#include "utils/magic_enum.hpp"

// ==============================
// Helper: 应用 CameraConfig 中的通用参数到任意 Builder
// ==============================
template <typename Builder>
static void apply_common_config(Builder& builder, const CameraConfig& config) {
  // 基本图像参数
  builder.exposure(config.exposure_us)
      .gain(config.gain)
      .roi(config.roi_x, config.roi_y, config.roi_w, config.roi_h)
      .hardwareIsp(config.hardware_isp);

  // 模式设置
  builder.triggerMode(config.trigger_mode);

  // 自动调节参数a
  builder.autoExposure(config.auto_exposure)
      .autoGain(config.auto_gain)
      .aeTargetBrightness(config.ae_target_brightness);

  // 抗频闪设置
  builder.antiFlickerMode(config.anti_flicker_mode);

  // 采集模式
  builder.acquisitionMode(config.acquisition_mode);

  // 图像增强参数
  builder.contrast(config.contrast)
      .gamma(config.gamma)
      .saturation(config.saturation)
      .sharpnessEnable(config.sharpness_enable)
      .sharpness(config.sharpness);

  // 图像处理参数
  builder.inverseState(config.inverse_state)
      .flipHorizontalState(config.flip_horizontal_state)
      .flipVerticalState(config.flip_vertical_state)
      .rotateState(config.rotate_state)
      .rotateOpposite(config.rotate_opposite)
      .coolerState(config.cooler_state)
      .bufferQueueSize(config.buffer_queue_size);

  // 注意：
  // - DVP 有 blackLevel/colorTemperature/flatFieldState 等，IKap 没有
  // - IKap 有 flipX/flipY，DVP 用 flipHorizontal/Vertical
  // 所以这些特有参数不在 helper 中，保留在各自分支里
}

// ==============================
// 模板化工厂函数（使用 if constexpr 隔离类型）
// ==============================
template <typename ConfigT>
std::unique_ptr<CameraCapture> create_camera_typed(
    const CameraBrand& brand, const std::string& identifier,
    const ConfigT& config, const std::vector<std::string>& event_specs) {
  static_assert(std::is_base_of_v<CameraConfig, ConfigT>,
                "ConfigT must inherit from CameraConfig");

  if constexpr (std::is_same_v<ConfigT, DvpConfig>) {
    if (brand != CameraBrand::DVP) {
      throw std::invalid_argument("DvpConfig used with non-DVP brand");
    }
    const DvpConfig& dvp_cfg = config;
    auto builder = DvpCameraBuilder::fromUserId(identifier);

    // === 应用通用参数 ===
    apply_common_config(builder, dvp_cfg);

    // === DVP 特有参数（通用 Config 不含）===
    builder.blackLevel(dvp_cfg.black_level)
        .colorTemperature(dvp_cfg.color_temperature)
        .flatFieldState(dvp_cfg.flat_field_state)
        .defectFixState(dvp_cfg.defect_fix_state)
        .monoState(dvp_cfg.mono_state)
        .aeRoi(dvp_cfg.ae_roi_x, dvp_cfg.ae_roi_y, dvp_cfg.ae_roi_w,
               dvp_cfg.ae_roi_h)
        .awbRoi(dvp_cfg.awb_roi_x, dvp_cfg.awb_roi_y, dvp_cfg.awb_roi_w,
                dvp_cfg.awb_roi_h)
        .streamFlowCtrlSel(dvp_cfg.stream_flow_ctrl_sel)
        .linkTimeout(dvp_cfg.link_timeout)
        .awbOperation(dvp_cfg.awb_operation)
        .triggerActivation(dvp_cfg.trigger_activation)
        .triggerCount(dvp_cfg.trigger_count)
        .triggerDebouncer(dvp_cfg.trigger_debouncer)
        .strobeSource(dvp_cfg.strobe_source)
        .strobeDelay(dvp_cfg.strobe_delay)
        .strobeDuration(dvp_cfg.strobe_duration)
        .lineTrigEnable(dvp_cfg.line_trig_enable)
        .lineTrigSource(dvp_cfg.line_trig_source)
        .lineTrigFilter(dvp_cfg.line_trig_filter)
        .lineTrigEdgeSel(dvp_cfg.line_trig_edge_sel)
        .lineTrigDelay(dvp_cfg.line_trig_delay)
        .lineTrigDebouncer(dvp_cfg.line_trig_debouncer)
        .acquisitionFrameRate(dvp_cfg.acquisition_frame_rate)
        .acquisitionFrameRateEnable(dvp_cfg.acquisition_frame_rate_enable)
        .flatFieldEnable(dvp_cfg.flat_field_enable);

    // === DVP 特有 Builder 参数 ===
    builder.triggerSource(static_cast<dvpTriggerSource>(dvp_cfg.trigger_source))
        .triggerDelay(dvp_cfg.trigger_delay_us)
        .exposureRange(dvp_cfg.exposure_min_us, dvp_cfg.exposure_max_us)
        .gainRange(dvp_cfg.gain_min, dvp_cfg.gain_max);

    // === DVP 事件注册 ===
    for (const auto& spec : event_specs) {
      size_t pos = spec.find(':');
      std::string event_name = spec.substr(0, pos);
      std::string handler_name =
          (pos != std::string::npos) ? spec.substr(pos + 1) : "log";

      auto handler =
          EventHandlerRegistry::instance().get_dvp_handler(handler_name);
      if (!handler) {
        continue;
      }

      // 一行代码：magic_enum 转换
      if (auto event_type = magic_enum::enum_cast<DvpEventType>(event_name)) {
        builder.onEvent(*event_type, handler);
      }
    }

    return builder.build();

  } else if constexpr (std::is_same_v<ConfigT, IkapConfig>) {
    if (brand != CameraBrand::IKap) {
      throw std::invalid_argument("IkapConfig used with non-IKap brand");
    }
    const IkapConfig& ikap_cfg = config;
    auto builder = IkapCameraBuilder::fromUserId(identifier);

    // === 应用通用参数 ===
    apply_common_config(builder, ikap_cfg);

    // === IKAP 特有参数（通用 Config 不含）===
    // 注意：IKap 没有 blackLevel / colorTemperature / defectFixState 等
    // flipX/Y 已由 apply_common_config 中的 flipHorizontal/Vertical 覆盖？
    // 如果 IKapBuilder.flipX != flipHorizontalState，可额外设置：
    // builder.flipX(ikap_cfg.flip_x).flipY(ikap_cfg.flip_y);

    // === IKAP 特有 Builder 参数 ===
    builder.startMode(ikap_cfg.start_mode)
        .transferMode(ikap_cfg.transfer_mode)
        .grabStrategy(ikap_cfg.grab_strategy)
        .autoClear(ikap_cfg.auto_clear)
        .zoomMethod(ikap_cfg.zoom_method)
        .windowTitle(ikap_cfg.window_title)
        .batchMode(ikap_cfg.batch_mode)
        .batchFrameCount(ikap_cfg.batch_frame_count)
        .streamFlowCtrl(ikap_cfg.stream_flow_ctrl)
        .linkTimeout(ikap_cfg.link_timeout);

    // === IKAP 事件注册 ===
    for (const auto& spec : event_specs) {
      size_t pos = spec.find(':');
      std::string event_name = spec.substr(0, pos);
      std::string handler_name =
          (pos != std::string::npos) ? spec.substr(pos + 1) : "log";

      auto handler =
          EventHandlerRegistry::instance().get_ikap_handler(handler_name);
      if (!handler) {
        continue;
      }

      // magic_enum 转换
      if (auto event_type = magic_enum::enum_cast<IkapEventType>(event_name)) {
        builder.onEvent(*event_type, handler);
      }
    }

    return builder.build();

  } else {
    static_assert(sizeof(ConfigT) == 0, "Unsupported ConfigT type");
    return nullptr;
  }
}

// 显式实例化
template std::unique_ptr<CameraCapture> create_camera_typed<DvpConfig>(
    const CameraBrand&, const std::string&, const DvpConfig&,
    const std::vector<std::string>& event_specs);
template std::unique_ptr<CameraCapture> create_camera_typed<IkapConfig>(
    const CameraBrand&, const std::string&, const IkapConfig&,
    const std::vector<std::string>& event_specs);

// ==============================
// 从配置创建多相机
// ==============================
std::vector<std::shared_ptr<CameraCapture>> create_cameras_from_config(
    const std::vector<config::CameraEntry>& camera_entries,
    inicpp::IniManager& ini) {
  std::vector<std::shared_ptr<CameraCapture>> cameras;
  cameras.reserve(camera_entries.size());

  for (size_t i = 0; i < camera_entries.size(); ++i) {
    std::string section_name = "camera" + std::to_string(i);
    if (!ini.isSectionExists(section_name)) {
      LOG_WARN("Camera section {} not found", section_name);
      continue;
    }

    inicpp::section sec = ini[section_name];  // 值拷贝

    try {
      if (camera_entries[i].brand == CameraBrand::DVP) {
        DvpConfig cfg;
        static_cast<CameraConfig&>(cfg) = camera_entries[i].config;

        cfg.trigger_source = sec.toInt("trigger_source");
        cfg.trigger_delay_us = sec.toDouble("trigger_delay_us");
        cfg.exposure_min_us = sec.toDouble("exposure_min_us");
        cfg.exposure_max_us = sec.toDouble("exposure_max_us");
        cfg.gain_min = static_cast<float>(sec.toDouble("gain_min"));
        cfg.gain_max = static_cast<float>(sec.toDouble("gain_max"));

        auto ptr = create_camera_typed(CameraBrand::DVP, camera_entries[i].id,
                                       cfg, camera_entries[i].event_specs);
        if (ptr) {
          cameras.emplace_back(std::move(ptr));
        }

      } else if (camera_entries[i].brand == CameraBrand::IKap) {
        IkapConfig cfg;
        static_cast<CameraConfig&>(cfg) = camera_entries[i].config;

        cfg.start_mode = sec.toInt("start_mode");
        cfg.transfer_mode = sec.toInt("transfer_mode");
        cfg.grab_strategy = sec.toInt("grab_strategy");
        cfg.auto_clear = sec.toInt("auto_clear") != 0;
        cfg.flip_x = sec.toInt("flip_x") != 0;
        cfg.flip_y = sec.toInt("flip_y") != 0;
        cfg.zoom_method = sec.toInt("zoom_method");
        cfg.window_title = sec.toString("window_title");
        cfg.batch_mode = sec.toInt("batch_mode") != 0;
        cfg.batch_frame_count = sec.toInt("batch_frame_count");
        cfg.stream_flow_ctrl = sec.toInt("stream_flow_ctrl");
        cfg.link_timeout = sec.toInt("link_timeout");
        cfg.cooler_state = sec.toInt("cooler_state") != 0;
        cfg.buffer_queue_size = sec.toInt("buffer_queue_size");

        auto ptr = create_camera_typed(CameraBrand::IKap, camera_entries[i].id,
                                       cfg, camera_entries[i].event_specs);
        if (ptr) {
          cameras.emplace_back(std::move(ptr));
        } else {
          //throw std::runtime_error("Failed to create camera [" +
          //                         camera_entries[i].id + "]");
        }
      }
    } catch (const std::exception& e) {
      std::cerr << "Failed to create camera [" << camera_entries[i].id
                << "]: " << e.what() << std::endl;
      continue;
    }
  }

  return cameras;
}
