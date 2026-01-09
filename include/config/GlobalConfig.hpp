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
 *  - File: GlobalConfig.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */
#pragma once
#include <atomic>
#include <exception>
#include <filesystem>  //NOLINT
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "config/CameraConfig.hpp"
#include "config/ConfigMacros.hpp"
#include "config/ConfigObserver.hpp"
#include "utils/executable_path.h"
#include "utils/inicpp.hpp"

// ===========================================================================
// 我们提供的配置项
// 这个部分额外可以给python提供CODEGEN解析
// 点击这里快速跳转到具体的配置项部分
CONFIG_FORWARD_DECLARE(GlobalConfig)            // 全局配置
CONFIG_FORWARD_DECLARE(HoleDetectionConfig)     // 孔洞检测配置项
CONFIG_FORWARD_DECLARE(SurfaceDetectionConfig)  // 表面检测配置项
CONFIG_FORWARD_DECLARE(LoggingConfig)           // 日志配置项
CONFIG_FORWARD_DECLARE(CameraEntry)             // 相机参数设置
// ===========================================================================
namespace config {
constexpr const char *CONFIG_FILE = "config.ini";
inline std::string g_config_file_path;

// ── COMPONENT: ConfigFilePath
// ────────────────────────────────────────────────────────
inline std::string get_config_file_path() {
  static std::string config_file_path = []() {
    std::filesystem::path exe_dir(CFPUtils::getExecutableDirectory());
    return (exe_dir / CONFIG_FILE).string();
  }();
  return config_file_path;
}

inline void set_config_file_path(const std::string &path) {
  g_config_file_path = path;
}

inline std::string get_default_config_path() {
  if (!g_config_file_path.empty()) {
    return g_config_file_path;
  }
  return get_config_file_path();
}

// ==============================具体配置项=========================================

// ==========================================
//  孔洞检测的配置项
// ==========================================
struct HoleDetectionConfig {
  float pixel_per_mm;
  bool enable_real_world_calculation;
  int min_defect_area;
  int edge_margin;
  int merge_distance_threshold;
  float pixel_to_mm_width;
  float pixel_to_mm_height;
  std::string partition_params;

  static HoleDetectionConfig load(inicpp::IniManager &ini) {
    try {
      auto hole_section = ini["hole_detection"];
      HoleDetectionConfig config;

      config.pixel_per_mm =
          hole_section["pixel_per_mm"].String().empty()
              ? 50.0f
              : std::stof(hole_section["pixel_per_mm"].String());

      config.enable_real_world_calculation =
          hole_section["enable_real_world_calculation"].String().empty()
              ? true
              : static_cast<bool>(
                    hole_section["enable_real_world_calculation"]);

      config.min_defect_area =
          hole_section["min_defect_area"].String().empty()
              ? 1
              : std::stoi(hole_section["min_defect_area"].String());

      config.edge_margin = hole_section["edge_margin"].String().empty()
                               ? 10
                               : static_cast<int>(hole_section["edge_margin"]);

      config.merge_distance_threshold =
          hole_section["merge_distance_threshold"].String().empty()
              ? 20
              : static_cast<int>(hole_section["merge_distance_threshold"]);

      config.pixel_to_mm_width =
          hole_section["pixel_to_mm_width"].String().empty()
              ? 0.05586f
              : std::stof(hole_section["pixel_to_mm_width"].String());

      config.pixel_to_mm_height =
          hole_section["pixel_to_mm_height"].String().empty()
              ? 0.061f
              : std::stof(hole_section["pixel_to_mm_height"].String());

      config.partition_params =
          hole_section["partition_params"].String().empty()
              ? "0.3,0.4,0.3,20,23,20"
              : hole_section["partition_params"].String();

      return config;
    } catch (const std::exception &e) {
      std::cerr << "Exception: " << e.what()
                << " when loading hole detection params" << std::endl;
      throw;
    } catch (...) {
      std::cerr
          << "Unknown exception when loading hole detection params from ini"
          << std::endl;
      throw;
    }
  }

  static void saveDefaults(inicpp::IniManager &ini) {
    ini.set("hole_detection", "pixel_per_mm", 50.0f, "每毫米像素数");
    ini.set("hole_detection", "enable_real_world_calculation", true,
            "是否启用真实世界尺寸计算");
    ini.set("hole_detection", "min_defect_area", 1, "最小缺陷面积(像素)");
    ini.set("hole_detection", "edge_margin", 10, "边缘边距(像素)");
    ini.set("hole_detection", "merge_distance_threshold", 20,
            "合并距离阈值(像素)");
    ini.set("hole_detection", "pixel_to_mm_width", 0.05586f,
            "像素到毫米宽度转换系数");
    ini.set("hole_detection", "pixel_to_mm_height", 0.061f,
            "像素到毫米高度转换系数");
    ini.set("hole_detection", "partition_params", "0.3,0.4,0.3,20,23,20",
            "分区参数(左中右比例和阈值)");
  }
};

// ==========================================
//  表面检测的配置项
// ==========================================
struct SurfaceDetectionConfig {
  // todo: 添加表面检测参数
};

// ==========================================
//  日志的配置项
// ==========================================
struct LoggingConfig {
  std::string file_level;
  std::string console_level;
  bool async_enabled;
  size_t queue_size;
  bool tcp_send_enabled{false};            // 是否启用TCP日志传输
  bool udp_send_enabled{false};            // 是否启用UDP日志传输
  bool ipc_send_enabled{false};            // 是否启用IPC日志传输
  std::string ipc_protocol{"tcp"};         // 新增：tcp 或 udp
  std::string tcp_server_ip{"127.0.0.1"};  // TCP服务器IP
  uint16_t tcp_server_port{8080};          // TCP服务器端口
  uint32_t tcp_timeout_ms{3000};           // TCP连接超时时间
  std::string udp_server_ip{"127.0.0.1"};  // UDP服务器IP
  uint16_t udp_server_port{8080};          // UDP服务器端口
  std::string ipc_server_ip{"127.0.0.1"};  // IPC服务器IP
  uint16_t ipc_server_port{5141};          // IPC服务器端口
  std::string network_level{"err"};        // 网络传输日志级别

  bool operator==(const LoggingConfig &other) const {
    return file_level == other.file_level &&
           console_level == other.console_level &&
           async_enabled == other.async_enabled &&
           queue_size == other.queue_size &&
           tcp_send_enabled == other.tcp_send_enabled &&
           udp_send_enabled == other.udp_send_enabled &&
           ipc_send_enabled == other.ipc_send_enabled &&
           ipc_protocol == other.ipc_protocol &&
           tcp_server_ip == other.tcp_server_ip &&
           tcp_server_port == other.tcp_server_port &&
           tcp_timeout_ms == other.tcp_timeout_ms &&
           udp_server_ip == other.udp_server_ip &&
           udp_server_port == other.udp_server_port &&
           ipc_server_ip == other.ipc_server_ip &&
           ipc_server_port == other.ipc_server_port &&
           network_level == other.network_level;
  }

  // 不等判断（方便使用）
  bool operator!=(const LoggingConfig &other) const {
    return !(*this == other);
  }

  static LoggingConfig load(inicpp::IniManager &ini) {
    try {
      auto logging_section = ini["logging"];
      LoggingConfig config;

      std::string file_level_str = logging_section["file_level"].String();
      if (file_level_str == "trace" || file_level_str == "debug" ||
          file_level_str == "info" || file_level_str == "warn" ||
          file_level_str == "err" || file_level_str == "critical" ||
          file_level_str == "off") {
        config.file_level = file_level_str;
      } else {
        config.file_level = "info";  // 默认值
      }

      std::string console_level_str = logging_section["console_level"].String();
      if (console_level_str == "trace" || console_level_str == "debug" ||
          console_level_str == "info" || console_level_str == "warn" ||
          console_level_str == "err" || console_level_str == "critical" ||
          console_level_str == "off") {
        config.console_level = console_level_str;
      } else {
        config.console_level = "critical";  // 默认值
      }

      config.async_enabled =
          logging_section["async_enabled"].String().empty()
              ? true
              : static_cast<bool>(logging_section["async_enabled"]);

      config.queue_size = logging_section["queue_size"].String().empty()
                              ? 32768
                              : static_cast<size_t>(std::stoi(
                                    logging_section["queue_size"].String()));

      config.tcp_send_enabled =
          logging_section["tcp_send_enabled"].String().empty()
              ? false
              : static_cast<bool>(logging_section["tcp_send_enabled"]);

      config.udp_send_enabled =
          logging_section["udp_send_enabled"].String().empty()
              ? false
              : static_cast<bool>(logging_section["udp_send_enabled"]);

      config.ipc_send_enabled =
          logging_section["ipc_send_enabled"].String().empty()
              ? false
              : static_cast<bool>(logging_section["ipc_send_enabled"]);

      config.ipc_protocol = logging_section["ipc_protocol"].String().empty()
                                ? "tcp"
                                : logging_section["ipc_protocol"].String();

      if (config.ipc_protocol != "tcp" && config.ipc_protocol != "udp") {
        config.ipc_protocol = "tcp";  // 默认使用TCP
      }

      config.tcp_server_ip = logging_section["tcp_server_ip"].String().empty()
                                 ? "127.0.0.1"
                                 : logging_section["tcp_server_ip"].String();

      config.tcp_server_port =
          logging_section["tcp_server_port"].String().empty()
              ? 8080
              : static_cast<uint16_t>(
                    std::stoi(logging_section["tcp_server_port"].String()));

      config.tcp_timeout_ms =
          logging_section["tcp_timeout_ms"].String().empty()
              ? 3000
              : static_cast<uint32_t>(
                    std::stoi(logging_section["tcp_timeout_ms"].String()));

      config.udp_server_ip = logging_section["udp_server_ip"].String().empty()
                                 ? "127.0.0.1"
                                 : logging_section["udp_server_ip"].String();

      config.udp_server_port =
          logging_section["udp_server_port"].String().empty()
              ? 8080
              : static_cast<uint16_t>(
                    std::stoi(logging_section["udp_server_port"].String()));

      config.ipc_server_ip = logging_section["ipc_server_ip"].String().empty()
                                 ? "127.0.0.1"
                                 : logging_section["ipc_server_ip"].String();

      config.ipc_server_port =
          logging_section["ipc_server_port"].String().empty()
              ? 5141
              : static_cast<uint16_t>(
                    std::stoi(logging_section["ipc_server_port"].String()));

      config.network_level = logging_section["network_level"].String().empty()
                                 ? "err"
                                 : logging_section["network_level"].String();

      return config;
    } catch (const std::exception &e) {
      std::cerr << "Exception: " << e.what()
                << " when loading logging config params" << std::endl;
      // 返回默认值
      LoggingConfig config;
      config.file_level = "info";
      config.console_level = "critical";
      config.async_enabled = true;
      config.queue_size = 32768;
      config.tcp_send_enabled = false;
      config.udp_send_enabled = false;
      config.ipc_send_enabled = false;
      config.ipc_protocol = "tcp";
      config.tcp_server_ip = "127.0.0.1";
      config.tcp_server_port = 8080;
      config.tcp_timeout_ms = 3000;
      config.udp_server_ip = "127.0.0.1";
      config.udp_server_port = 8080;
      config.ipc_server_ip = "127.0.0.1";
      config.ipc_server_port = 5141;
      config.network_level = "err";
      return config;
    } catch (...) {
      std::cerr
          << "Unknown exception when loading hole detection params from ini"
          << std::endl;
      throw;
    }
  }

  static void saveDefaults(inicpp::IniManager &ini) {
    ini.set("logging", "file_level", "info",
            "文件日志级别 (trace, debug, info, warn, err, critical, off)");
    ini.set("logging", "console_level", "critical",
            "控制台日志级别 (trace, debug, info, warn, err, critical, off)");
    ini.set("logging", "async_enabled", true, "是否启用异步日志");
    ini.set("logging", "queue_size", 32768, "异步日志队列大小");
    ini.set("logging", "tcp_send_enabled", false, "是否启用TCP日志传输");
    ini.set("logging", "udp_send_enabled", false, "是否启用UDP日志传输");
    ini.set("logging", "ipc_send_enabled", false, "是否启用IPC日志传输");
    ini.set("logging", "ipc_protocol", "tcp",
            "IPC日志传输协议 (tcp[默认5140], udp[默认5141])");
    ini.set("logging", "tcp_server_ip", "127.0.0.1", "TCP日志服务器IP地址");
    ini.set("logging", "tcp_server_port", 8080, "TCP日志服务器端口");
    ini.set("logging", "tcp_timeout_ms", 3000, "TCP连接超时时间(毫秒)");
    ini.set("logging", "udp_server_ip", "127.0.0.1", "UDP日志服务器IP地址");
    ini.set("logging", "udp_server_port", 8080, "UDP日志服务器端口");
    ini.set("logging", "ipc_server_ip", "127.0.0.1", "IPC日志服务器IP地址");
    ini.set("logging", "ipc_server_port", 5141, "IPC日志服务器端口");
    ini.set("logging", "network_level", "err", "网络传输日志级别");
  }
};

// ==========================================
//  相机配置项
// ==========================================
struct CameraEntry {
  std::string id;  // 用于确定具体是哪一台，暂时没有定用哪个内部字段标识
  CameraBrand brand;
  CameraConfig config;
  std::string algorithm;                 // e.g. "HoleDetection"
  std::vector<std::string> event_specs;  // 事件类型处理器

  static CameraEntry load(inicpp::IniManager &ini,
                          const std::string &section_name) {
    try {
      auto camera_section = ini[section_name];
      CameraEntry entry;

      // id（必填）
      entry.id = camera_section["id"].String();
      if (entry.id.empty()) {
        entry.id = "default_camera";
      }

      // brand
      std::string brand_str = camera_section["brand"].String();
      if (brand_str == "IKap") {
        entry.brand = CameraBrand::IKap;
      } else if (brand_str == "DVP") {
        entry.brand = CameraBrand::DVP;
      } else if (brand_str == "MIND") {
        entry.brand = CameraBrand::MIND;
      } else {
        entry.brand = CameraBrand::IKap;  // 默认
      }

      // algorithm
      entry.algorithm = camera_section["algorithm"].String();

      // events
      std::string events_str = camera_section["events"].String();
      if (!events_str.empty()) {
        std::istringstream iss(events_str);
        std::string spec;
        while (std::getline(iss, spec, ',')) {
          // 去掉空格
          spec.erase(std::remove(spec.begin(), spec.end(), ' '), spec.end());
          if (!spec.empty()) {
            entry.event_specs.push_back(spec);
          }
        }
      }

      // === 基本图像参数 ===
      entry.config.exposure_us =
          camera_section["exposure_us"].String().empty()
              ? 10000.0
              : std::stod(camera_section["exposure_us"].String());
      entry.config.gain =
          camera_section["gain"].String().empty()
              ? 1.0f
              : static_cast<float>(std::stof(camera_section["gain"].String()));
      entry.config.roi_x = camera_section["roi_x"].String().empty()
                               ? 0
                               : std::stoi(camera_section["roi_x"].String());
      entry.config.roi_y = camera_section["roi_y"].String().empty()
                               ? 0
                               : std::stoi(camera_section["roi_y"].String());
      entry.config.roi_w = camera_section["roi_w"].String().empty()
                               ? 0
                               : std::stoi(camera_section["roi_w"].String());
      entry.config.roi_h = camera_section["roi_h"].String().empty()
                               ? 0
                               : std::stoi(camera_section["roi_h"].String());

      // === 模式设置 ===
      entry.config.trigger_mode =
          camera_section["trigger_mode"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["trigger_mode"].String()));
      entry.config.hardware_isp =
          camera_section["hardware_isp"].String().empty()
              ? true
              : static_cast<bool>(
                    std::stoi(camera_section["hardware_isp"].String()));

      // === 自动调节参数 ===
      entry.config.auto_exposure =
          camera_section["auto_exposure"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["auto_exposure"].String()));
      entry.config.auto_gain = camera_section["auto_gain"].String().empty()
                                   ? false
                                   : static_cast<bool>(std::stoi(
                                         camera_section["auto_gain"].String()));
      entry.config.ae_target_brightness =
          camera_section["ae_target_brightness"].String().empty()
              ? 128
              : std::stoi(camera_section["ae_target_brightness"].String());

      // === 抗频闪设置 ===
      entry.config.anti_flicker_mode =
          camera_section["anti_flicker_mode"].String().empty()
              ? 0
              : std::stoi(camera_section["anti_flicker_mode"].String());

      // === 采集模式 ===
      entry.config.acquisition_mode =
          camera_section["acquisition_mode"].String().empty()
              ? 0
              : std::stoi(camera_section["acquisition_mode"].String());

      // === 图像增强参数 ===
      entry.config.contrast =
          camera_section["contrast"].String().empty()
              ? 0
              : std::stoi(camera_section["contrast"].String());
      entry.config.gamma = camera_section["gamma"].String().empty()
                               ? 100
                               : std::stoi(camera_section["gamma"].String());
      entry.config.saturation =
          camera_section["saturation"].String().empty()
              ? 100
              : std::stoi(camera_section["saturation"].String());
      entry.config.sharpness_enable =
          camera_section["sharpness_enable"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["sharpness_enable"].String()));
      entry.config.sharpness =
          camera_section["sharpness"].String().empty()
              ? 0
              : std::stoi(camera_section["sharpness"].String());

      // === 图像处理参数 ===
      entry.config.inverse_state =
          camera_section["inverse_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["inverse_state"].String()));
      entry.config.flip_horizontal_state =
          camera_section["flip_horizontal_state"].String().empty()
              ? false
              : static_cast<bool>(std::stoi(
                    camera_section["flip_horizontal_state"].String()));
      entry.config.flip_vertical_state =
          camera_section["flip_vertical_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["flip_vertical_state"].String()));
      entry.config.rotate_state =
          camera_section["rotate_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["rotate_state"].String()));
      entry.config.rotate_opposite =
          camera_section["rotate_opposite"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["rotate_opposite"].String()));
      entry.config.black_level =
          camera_section["black_level"].String().empty()
              ? 0.0f
              : static_cast<float>(
                    std::stof(camera_section["black_level"].String()));
      entry.config.color_temperature =
          camera_section["color_temperature"].String().empty()
              ? 6500
              : std::stoi(camera_section["color_temperature"].String());
      entry.config.flat_field_state =
          camera_section["flat_field_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["flat_field_state"].String()));
      entry.config.defect_fix_state =
          camera_section["defect_fix_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["defect_fix_state"].String()));

      // === 其他高级参数 ===
      entry.config.mono_state =
          camera_section["mono_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["mono_state"].String()));
      entry.config.ae_roi_x =
          camera_section["ae_roi_x"].String().empty()
              ? 0
              : std::stoi(camera_section["ae_roi_x"].String());
      entry.config.ae_roi_y =
          camera_section["ae_roi_y"].String().empty()
              ? 0
              : std::stoi(camera_section["ae_roi_y"].String());
      entry.config.ae_roi_w =
          camera_section["ae_roi_w"].String().empty()
              ? 0
              : std::stoi(camera_section["ae_roi_w"].String());
      entry.config.ae_roi_h =
          camera_section["ae_roi_h"].String().empty()
              ? 0
              : std::stoi(camera_section["ae_roi_h"].String());
      entry.config.awb_roi_x =
          camera_section["awb_roi_x"].String().empty()
              ? 0
              : std::stoi(camera_section["awb_roi_x"].String());
      entry.config.awb_roi_y =
          camera_section["awb_roi_y"].String().empty()
              ? 0
              : std::stoi(camera_section["awb_roi_y"].String());
      entry.config.awb_roi_w =
          camera_section["awb_roi_w"].String().empty()
              ? 0
              : std::stoi(camera_section["awb_roi_w"].String());
      entry.config.awb_roi_h =
          camera_section["awb_roi_h"].String().empty()
              ? 0
              : std::stoi(camera_section["awb_roi_h"].String());
      entry.config.cooler_state =
          camera_section["cooler_state"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["cooler_state"].String()));
      entry.config.buffer_queue_size =
          camera_section["buffer_queue_size"].String().empty()
              ? 8
              : std::stoi(camera_section["buffer_queue_size"].String());
      entry.config.stream_flow_ctrl_sel =
          camera_section["stream_flow_ctrl_sel"].String().empty()
              ? true
              : static_cast<bool>(
                    std::stoi(camera_section["stream_flow_ctrl_sel"].String()));
      entry.config.link_timeout =
          camera_section["link_timeout"].String().empty()
              ? 5000u
              : static_cast<unsigned int>(
                    std::stoul(camera_section["link_timeout"].String()));

      // === 白平衡相关参数 ===
      entry.config.awb_operation =
          camera_section["awb_operation"].String().empty()
              ? 0
              : std::stoi(camera_section["awb_operation"].String());

      // === 触发相关参数 ===
      entry.config.trigger_activation =
          camera_section["trigger_activation"].String().empty()
              ? 0
              : std::stoi(camera_section["trigger_activation"].String());
      entry.config.trigger_count =
          camera_section["trigger_count"].String().empty()
              ? 0
              : std::stoi(camera_section["trigger_count"].String());
      entry.config.trigger_debouncer =
          camera_section["trigger_debouncer"].String().empty()
              ? 0.0
              : std::stod(camera_section["trigger_debouncer"].String());
      entry.config.strobe_source =
          camera_section["strobe_source"].String().empty()
              ? 0
              : std::stoi(camera_section["strobe_source"].String());
      entry.config.strobe_delay =
          camera_section["strobe_delay"].String().empty()
              ? 0.0
              : std::stod(camera_section["strobe_delay"].String());
      entry.config.strobe_duration =
          camera_section["strobe_duration"].String().empty()
              ? 1000.0
              : std::stod(camera_section["strobe_duration"].String());

      // === 线扫相机专用参数 ===
      entry.config.line_trig_enable =
          camera_section["line_trig_enable"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["line_trig_enable"].String()));
      entry.config.line_trig_source =
          camera_section["line_trig_source"].String().empty()
              ? 0
              : std::stoi(camera_section["line_trig_source"].String());
      entry.config.line_trig_filter =
          camera_section["line_trig_filter"].String().empty()
              ? 0.0
              : std::stod(camera_section["line_trig_filter"].String());
      entry.config.line_trig_edge_sel =
          camera_section["line_trig_edge_sel"].String().empty()
              ? 0
              : std::stoi(camera_section["line_trig_edge_sel"].String());
      entry.config.line_trig_delay =
          camera_section["line_trig_delay"].String().empty()
              ? 0.0
              : std::stod(camera_section["line_trig_delay"].String());
      entry.config.line_trig_debouncer =
          camera_section["line_trig_debouncer"].String().empty()
              ? 0.0
              : std::stod(camera_section["line_trig_debouncer"].String());

      // === 其他高级参数 ===
      entry.config.acquisition_frame_rate =
          camera_section["acquisition_frame_rate"].String().empty()
              ? 0.0
              : std::stod(camera_section["acquisition_frame_rate"].String());
      entry.config.acquisition_frame_rate_enable =
          camera_section["acquisition_frame_rate_enable"].String().empty()
              ? false
              : static_cast<bool>(std::stoi(
                    camera_section["acquisition_frame_rate_enable"].String()));
      entry.config.flat_field_enable =
          camera_section["flat_field_enable"].String().empty()
              ? false
              : static_cast<bool>(
                    std::stoi(camera_section["flat_field_enable"].String()));

      return entry;
    } catch (const std::exception &e) {
      std::cerr << "Exception: " << e.what()
                << " when loading camera params from ini" << std::endl;
      throw;
    } catch (...) {
      std::cerr << "Unknown exception when loading camera params from ini"
                << std::endl;
      throw;
    }
  }

  static void saveDefaults(inicpp::IniManager &ini,
                           const std::string &section_name) {
    ini.set(section_name, "id", "cam1", "相机唯一标识");
    ini.set(section_name, "brand", "IKap", "相机品牌 (IKap/DVP/MIND)");
    ini.set(section_name, "algorithm", "HoleDetection", "绑定的算法名");

    // 基本图像参数
    ini.set(section_name, "exposure_us", 10000.0, "曝光时间（微秒）");
    ini.set(section_name, "gain", 1.0f, "增益");
    ini.set(section_name, "roi_x", 0, "ROI X坐标");
    ini.set(section_name, "roi_y", 0, "ROI Y坐标");
    ini.set(section_name, "roi_w", 0, "ROI 宽度");
    ini.set(section_name, "roi_h", 0, "ROI 高度");

    // 模式设置
    ini.set(section_name, "trigger_mode", false, "触发模式");
    ini.set(section_name, "hardware_isp", true, "硬件ISP开关");

    // 自动调节参数
    ini.set(section_name, "auto_exposure", false, "自动曝光");
    ini.set(section_name, "auto_gain", false, "自动增益");
    ini.set(section_name, "ae_target_brightness", 128, "AE目标亮度 (0-255)");

    // 抗频闪设置
    ini.set(section_name, "anti_flicker_mode", 0,
            "抗频闪模式 (0:关,1:50Hz,2:60Hz)");

    // 采集模式
    ini.set(section_name, "acquisition_mode", 0,
            "采集模式 (0:连续,1:单帧,2:多帧)");

    // 图像增强参数
    ini.set(section_name, "contrast", 0, "对比度");
    ini.set(section_name, "gamma", 100, "伽马值");
    ini.set(section_name, "saturation", 100, "饱和度");
    ini.set(section_name, "sharpness_enable", false, "锐度使能");
    ini.set(section_name, "sharpness", 0, "锐度值");

    // 图像处理参数
    ini.set(section_name, "inverse_state", false, "反转状态");
    ini.set(section_name, "flip_horizontal_state", false, "水平翻转");
    ini.set(section_name, "flip_vertical_state", false, "垂直翻转");
    ini.set(section_name, "rotate_state", false, "旋转状态");
    ini.set(section_name, "rotate_opposite", false, "旋转反向");
    ini.set(section_name, "black_level", 0.0f, "黑电平");
    ini.set(section_name, "color_temperature", 6500, "色温");
    ini.set(section_name, "flat_field_state", false, "平场校正状态");
    ini.set(section_name, "defect_fix_state", false, "缺陷校正状态");

    // 其他高级参数
    ini.set(section_name, "mono_state", false, "单色模式");
    ini.set(section_name, "ae_roi_x", 0, "AE ROI X坐标");
    ini.set(section_name, "ae_roi_y", 0, "AE ROI Y坐标");
    ini.set(section_name, "ae_roi_w", 0, "AE ROI 宽度");
    ini.set(section_name, "ae_roi_h", 0, "AE ROI 高度");
    ini.set(section_name, "awb_roi_x", 0, "AWB ROI X坐标");
    ini.set(section_name, "awb_roi_y", 0, "AWB ROI Y坐标");
    ini.set(section_name, "awb_roi_w", 0, "AWB ROI 宽度");
    ini.set(section_name, "awb_roi_h", 0, "AWB ROI 高度");
    ini.set(section_name, "cooler_state", false, "制冷器状态");
    ini.set(section_name, "buffer_queue_size", 8, "缓存队列大小");
    ini.set(section_name, "stream_flow_ctrl_sel", true, "流控制选择");
    ini.set(section_name, "link_timeout", 5000, "链接超时时间 (毫秒)");

    // 白平衡相关参数
    ini.set(section_name, "awb_operation", 0, "AWB操作模式");

    // 触发相关参数
    ini.set(section_name, "trigger_activation", 0, "触发激活方式");
    ini.set(section_name, "trigger_count", 0, "触发计数");
    ini.set(section_name, "trigger_debouncer", 0.0, "触发消抖时间 (毫秒)");
    ini.set(section_name, "strobe_source", 0, "闪光源");
    ini.set(section_name, "strobe_delay", 0.0, "闪光延迟 (毫秒)");
    ini.set(section_name, "strobe_duration", 1000.0, "闪光持续时间 (微秒)");

    // 线扫相机专用参数
    ini.set(section_name, "line_trig_enable", false, "线触发使能");
    ini.set(section_name, "line_trig_source", 0, "线触发源");
    ini.set(section_name, "line_trig_filter", 0.0, "线触发滤波");
    ini.set(section_name, "line_trig_edge_sel", 0, "线触发边沿选择");
    ini.set(section_name, "line_trig_delay", 0.0, "线触发延迟 (毫秒)");
    ini.set(section_name, "line_trig_debouncer", 0.0, "线触发消抖 (毫秒)");

    // 其他高级参数
    ini.set(section_name, "acquisition_frame_rate", 0.0, "采集帧率 (fps)");
    ini.set(section_name, "acquisition_frame_rate_enable", false,
            "采集帧率使能");
    ini.set(section_name, "flat_field_enable", false, "平场校正使能");
  }
};

// 加载所有相机配置（从 camera0, camera1, ...）
inline static std::vector<CameraEntry> load_cameras_from_ini(
    inicpp::IniManager &ini) {
  std::vector<CameraEntry> cameras;
  int index = 0;
  while (true) {
    std::string section = "camera" + std::to_string(index);
    if (!ini.isSectionExists(section)) {
      break;
    }
    try {
      CameraEntry entry = CameraEntry::load(ini, section);
      cameras.push_back(std::move(entry));
      index++;
    } catch (...) {
      // 如果无法加载某个相机配置，则跳出循环
      break;
    }
  }
  return cameras;
}

// 保存多相机默认配置
inline static void save_camera_defaults(inicpp::IniManager &ini) {
  // camera0 - IKap
  CameraEntry::saveDefaults(ini, "camera0");
  ini.set("camera0", "id", "ikap_bright", "IKap 明场相机");
  ini.set("camera0", "brand", "IKap", "品牌");
  ini.set("camera0", "algorithm", "HoleDetection", "算法");

  // camera1 - DVP
  // CameraEntry::saveDefaults(ini, "camera1");
  // ini.set("camera1", "id", "dvp_back", "DVP 后置相机");
  // ini.set("camera1", "brand", "DVP", "品牌");
  // ini.set("camera1", "algorithm", "SurFaceDetection", "算法");
}

// ==========================================
//  全局检测的配置项
// ==========================================
struct GlobalConfig {
  std::string title;
  HoleDetectionConfig hole_detection;        // 针孔检测
  SurfaceDetectionConfig surface_detection;  // 表面检测
  LoggingConfig logging_settings;            // 日志配置
  std::vector<CameraEntry> camera_entries;   // 相机配置列表
  static GlobalConfig load();

  static void saveDefaults(inicpp::IniManager &ini) {
    ini.set("", "title", "CFP", "应用标题");
    HoleDetectionConfig::saveDefaults(ini);
    // SurfaceDetectionConfig::saveDefaults(ini);
    LoggingConfig::saveDefaults(ini);
    save_camera_defaults(ini);
  }
};

//   ▶ Initialization Logic
// ─────────────────────────────全局配置项的初始化逻辑都在这里─────────────────────────────
// 检查并创建默认配置
inline void check_and_create_default_config() {
  std::string config_path = get_default_config_path();
  std::filesystem::path config_file(config_path);

  // 1. 检查文件是否存在，存在则直接返回
  if (std::filesystem::exists(config_file)) {
    return;
  }

  // 2. 不存在则创建目录
  std::filesystem::path config_dir = config_file.parent_path();
  if (!std::filesystem::exists(config_dir)) {
    try {
      std::filesystem::create_directories(config_dir);
      std::cout << "Created config directory: " << config_dir << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Failed to create config directory: " << e.what()
                << std::endl;
      throw;
    }
  }

  try {
    // 这里传入路径，IniManager内部会创建空文件，然后通过set写入默认配置
    inicpp::IniManager ini(config_path);
    GlobalConfig::saveDefaults(ini);  // set方法会直接写入文件
    std::cout << "Config file not found, created default config: "
              << config_path << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Failed to create default config file: " << e.what()
              << std::endl;
    throw;
  }
}

inline GlobalConfig GlobalConfig::load() {
  try {
    // 加载前先检查并创建默认配置
    check_and_create_default_config();

    // 此时文件已存在，安全初始化IniManager
    inicpp::IniManager ini(get_default_config_path());

    GlobalConfig config;
    config.title =
        ini[""]["title"].String().empty() ? "CFP" : ini[""]["title"].String();
    config.hole_detection = HoleDetectionConfig::load(ini);
    // config.surface_detection = SurfaceDetectionConfig::load(ini);
    config.logging_settings = LoggingConfig::load(ini);
    config.camera_entries = load_cameras_from_ini(ini);  // 加载多相机配置
    return config;
  } catch (std::exception &e) {
    std::cerr << "Failed to load global config: " << e.what() << std::endl;
    throw;
  }
}

class ConfigLoader;

inline std::unique_ptr<ConfigLoader> g_config_loader;
inline std::unique_ptr<GlobalConfig> g_current_config;
inline std::mutex g_config_mutex;
inline std::atomic<bool> g_config_initialized{false};

// ──────────────────────────────────────────动态配置文件加载器───────────────────────────────────────────

class ConfigLoader {
 protected:
  std::vector<ConfigObserver *> observers_;
  std::atomic<bool> monitoring_active{false};
  std::thread monitor_thread;

  virtual std::unique_ptr<GlobalConfig> loadFromStaticFile() {
    return std::make_unique<GlobalConfig>(GlobalConfig::load());
  }
  virtual void startMonitoring(GlobalConfig *config) {
    if (monitoring_active.load()) {
      return;
    }
    monitoring_active = true;

    monitor_thread = std::thread([this, config]() {
      // ========== 修复核心：初始化last_write为文件当前修改时间 ==========
      std::filesystem::file_time_type last_write = {};
      std::filesystem::path config_path = get_default_config_path();

      auto to_time_t = [](const std::filesystem::file_time_type &file_time) {
        using namespace std::chrono;
        const auto sys_time = clock_cast<system_clock>(file_time);
        return system_clock::to_time_t(sys_time);
      };

      // 先读取文件的初始修改时间，避免第一次误判
      try {
        if (std::filesystem::exists(config_path)) {
          last_write = std::filesystem::last_write_time(config_path);
          // 可选：打印初始化日志，确认初始时间
          // 修复：改用通用的时间转换方法
          std::time_t init_time = to_time_t(last_write);
          std::cout << "Config monitor initialized, last write time: "
                    << init_time << std::endl;
        }
      } catch (const std::exception &e) {
        std::cerr << "Failed to get initial config file time: " << e.what()
                  << std::endl;
        last_write = {};  // 初始化失败则设为空，不影响后续检测
      }

      while (monitoring_active.load()) {
        try {
          if (std::filesystem::exists(config_path)) {
            auto curr_time = std::filesystem::last_write_time(config_path);

            // 只有当文件存在 + last_write已初始化 + 时间真的变化时，才触发重载
            if (last_write != std::filesystem::file_time_type{} &&
                curr_time != last_write) {
              // ========== 优化：将file_time_type转为可读时间 ==========
              // 修复核心：通用的file_time_type转time_t方法
              std::time_t curr_time_t = to_time_t(curr_time);
              std::string curr_time_str = ctime(&curr_time_t);  // NOLINT
              // 去掉换行符
              if (!curr_time_str.empty() && curr_time_str.back() == '\n') {
                curr_time_str.pop_back();
              }

              std::cout << "Config file changed at " << curr_time_str
                        << ", Reloading..." << std::endl;

              auto newConfig = loadFromStaticFile();
              if (newConfig) {
                std::lock_guard<std::mutex> lock(g_config_mutex);
                *config = *newConfig;
                notifyObservers(*config);
                last_write = curr_time;  // 更新最后修改时间
              }
            }
          }
          std::this_thread::sleep_for(std::chrono::seconds(2));
        } catch (const std::exception &e) {
          if (monitoring_active.load()) {
            std::cerr << "Error while monitoring config file: " << e.what()
                      << std::endl;
          }
          break;
        } catch (...) {
          if (monitoring_active.load()) {
            std::cout << "Unknown error while monitoring config file"
                      << std::endl;
          }
          break;
        }
      }
    });
  }

  void notifyObservers(const GlobalConfig &config) {
    for (auto *obs : observers_) {
      try {
        obs->onConfigReloaded(config);
      } catch (const std::exception &e) {
        std::cout << "Exception in observer: " << e.what() << std::endl;
      }
    }
  }

 public:
  virtual ~ConfigLoader() {
    monitoring_active = false;
    if (monitor_thread.joinable()) {
      monitor_thread.join();
    }
  }

  void addObserver(ConfigObserver *obs) {
    if (obs) {
      std::lock_guard<std::mutex> lock(g_config_mutex);
      if (std::find(observers_.begin(), observers_.end(), obs) ==
          observers_.end()) {
        observers_.push_back(obs);
      }
    }
  }

  void removeObserver(ConfigObserver *obs) {
    std::lock_guard<std::mutex> lock(g_config_mutex);
    observers_.erase(std::remove(observers_.begin(), observers_.end(), obs),
                     observers_.end());
  }

  std::unique_ptr<GlobalConfig> load() {
    std::unique_ptr<GlobalConfig> config;

    config = loadFromStaticFile();
    startMonitoring(config.get());
    notifyObservers(*config);
    return config;
  }
};

inline GlobalConfig get_current_config() {
  std::lock_guard<std::mutex> lock(g_config_mutex);
  return *g_current_config;
}

inline void update_current_config(const GlobalConfig &newConfig) {
  std::lock_guard<std::mutex> lock(g_config_mutex);
  *g_current_config = newConfig;
}

}  // namespace config
