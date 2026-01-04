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
#include <vector>

#include "../utils/executable_path.h"
#include "../utils/inicpp.hpp"
#include "ConfigMacros.hpp"
#include "ConfigObserver.hpp"

// ===========================================================================
// 我们提供的配置项
// 点击这里快速跳转到具体的配置项部分
CONFIG_FORWARD_DECLARE(GlobalConfig)            // 全局配置
CONFIG_FORWARD_DECLARE(HoleDetectionConfig)     // 孔洞检测配置项
CONFIG_FORWARD_DECLARE(SurfaceDetectionConfig)  // 表面检测配置项
CONFIG_FORWARD_DECLARE(LoggingConfig)           // 日志配置项
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

struct LoggingConfig {
  std::string file_level;
  std::string console_level;
  bool async_enabled;
  size_t queue_size;
  bool tcp_send_enabled{false};            // 是否启用TCP日志传输
  bool udp_send_enabled{false};            // 是否启用UDP日志传输
  std::string tcp_server_ip{"127.0.0.1"};  // TCP服务器IP
  uint16_t tcp_server_port{8080};          // TCP服务器端口
  uint32_t tcp_timeout_ms{3000};           // TCP连接超时时间
  std::string udp_server_ip{"127.0.0.1"};  // UDP服务器IP
  uint16_t udp_server_port{8080};          // UDP服务器端口
  std::string network_level{"err"};        // 网络传输日志级别

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
      config.tcp_server_ip = "127.0.0.1";
      config.tcp_server_port = 8080;
      config.tcp_timeout_ms = 3000;
      config.udp_server_ip = "127.0.0.1";
      config.udp_server_port = 8080;
      config.network_level = "err";
      return config;
    } catch (...) {
      std::cerr
          << "Unknown exception when loading logging config params from ini"
          << std::endl;
      // 返回默认值
      LoggingConfig config;
      config.file_level = "info";
      config.console_level = "critical";
      config.async_enabled = true;
      config.queue_size = 32768;
      config.tcp_send_enabled = false;
      config.udp_send_enabled = false;
      config.tcp_server_ip = "127.0.0.1";
      config.tcp_server_port = 8080;
      config.tcp_timeout_ms = 3000;
      config.udp_server_ip = "127.0.0.1";
      config.udp_server_port = 8080;
      config.network_level = "err";
      return config;
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
    ini.set("logging", "tcp_server_ip", "127.0.0.1", "TCP日志服务器IP地址");
    ini.set("logging", "tcp_server_port", 8080, "TCP日志服务器端口");
    ini.set("logging", "tcp_timeout_ms", 3000, "TCP连接超时时间(毫秒)");
    ini.set("logging", "udp_server_ip", "127.0.0.1", "UDP日志服务器IP地址");
    ini.set("logging", "udp_server_port", 8080, "UDP日志服务器端口");
    ini.set("logging", "network_level", "err", "网络传输日志级别");
  }
};

// ==========================================
//  全局检测的配置项
// ==========================================
struct GlobalConfig {
  std::string title;
  HoleDetectionConfig hole_detection;        // 针孔检测
  SurfaceDetectionConfig surface_detection;  // 表面检测
  LoggingConfig logging_settings;            // 日志配置
  static GlobalConfig load();

  static void saveDefaults(inicpp::IniManager &ini) {
    ini.set("", "title", "CFP", "应用标题");
    HoleDetectionConfig::saveDefaults(ini);
    // SurfaceDetectionConfig::saveDefaults(ini);
    LoggingConfig::saveDefaults(ini);
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
      std::filesystem::file_time_type last_write = {};
      while (monitoring_active.load()) {
        try {
          auto path = std::filesystem::path(get_default_config_path());
          if (std::filesystem::exists(path)) {
            auto curr_time = std::filesystem::last_write_time(path);
            if (curr_time != last_write) {
              std::cout << "Config file changed, Reloading..." << std::endl;
              auto newConfig = loadFromStaticFile();
              if (newConfig) {
                std::lock_guard<std::mutex> lock(g_config_mutex);
                *config = *newConfig;
                notifyObservers(*config);
                last_write = curr_time;
              }
            }
          }
          std::this_thread::sleep_for(std::chrono::seconds(2));
        } catch (...) {
          if (monitoring_active.load()) {
            std::cout << "Error while monitoring config file" << std::endl;
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
