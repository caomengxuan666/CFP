/*
 *  Copyright © 2026 [caomengxuan666]
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
 *  - File: LoggingConfigManager.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */

#pragma once
#include <string>
#include <vector>

#include "config/ConfigManager.hpp"
#include "config/GlobalConfig.hpp"
#include "config/ModuleIniter.hpp"
#include "logging/CaponLogging.hpp"

namespace logging {
// 直接继承了观察者接口
// 因为这里的日志类的重新应用的接口是完备的所以非常方便
/**
 * @brief 日志的管理器类,继承了观察者接口
 * @note
 * 使用的时候可以直接传入globalConfig对象,他会自动更新loggingConfig到Logger中
 */
class LoggingConfigManager : public config::ConfigObserver {
 public:
  static LoggingConfigManager& getInstance() {
    static LoggingConfigManager instance;
    return instance;
  }

  void applyLoggingConfig(const config::LoggingConfig& loggingConfig) {
    // 获取新旧差异
    auto& old_config = CaponLogger::instance().getCurrentLoggingConfig();
    if (loggingConfig == old_config) {
      return;
    }
    PrintDiff(old_config, loggingConfig);
    CaponLogger::instance().applyConfig(loggingConfig);
  }

  void applyGlobalConfig(const config::GlobalConfig& globalConfig) {
    applyLoggingConfig(globalConfig.logging_settings);
  }

  // 实现 ConfigObserver 接口
  void onConfigReloaded(const config::GlobalConfig& new_config) override {
    applyGlobalConfig(new_config);
  }

 private:
  LoggingConfigManager() = default;

  void PrintDiff(const config::LoggingConfig& new_config,
                 const config::LoggingConfig& old_config) {
    // ========== 1. 定义字段描述结构体：封装字段名+取值Lambda+类型适配
    // ==========
    struct FieldDesc {
      std::string name;  // 字段名（用于日志打印）
      // 取值Lambda：接收LoggingConfig，返回该字段的字符串表示
      std::function<std::string(const config::LoggingConfig&)> get_value;
    };

    // ========== 2. 定义所有需要对比的字段（一次性配置，扩展只需加新项）
    // ==========
    const std::vector<FieldDesc> fields = {
        // 字符串类型字段
        {"file_level", [](const auto& cfg) { return cfg.file_level; }},
        {"console_level", [](const auto& cfg) { return cfg.console_level; }},
        {"ipc_protocol", [](const auto& cfg) { return cfg.ipc_protocol; }},
        {"tcp_server_ip", [](const auto& cfg) { return cfg.tcp_server_ip; }},
        {"udp_server_ip", [](const auto& cfg) { return cfg.udp_server_ip; }},
        {"ipc_server_ip", [](const auto& cfg) { return cfg.ipc_server_ip; }},
        {"network_level", [](const auto& cfg) { return cfg.network_level; }},

        // 布尔类型字段（转为"true"/"false"）
        {"async_enabled",
         [](const auto& cfg) { return cfg.async_enabled ? "true" : "false"; }},
        {"tcp_send_enabled",
         [](const auto& cfg) {
           return cfg.tcp_send_enabled ? "true" : "false";
         }},
        {"udp_send_enabled",
         [](const auto& cfg) {
           return cfg.udp_send_enabled ? "true" : "false";
         }},
        {"ipc_send_enabled",
         [](const auto& cfg) {
           return cfg.ipc_send_enabled ? "true" : "false";
         }},

        // 数值类型字段（转为字符串）
        {"queue_size",
         [](const auto& cfg) { return std::to_string(cfg.queue_size); }},
        {"tcp_server_port",
         [](const auto& cfg) { return std::to_string(cfg.tcp_server_port); }},
        {"tcp_timeout_ms",
         [](const auto& cfg) { return std::to_string(cfg.tcp_timeout_ms); }},
        {"udp_server_port",
         [](const auto& cfg) { return std::to_string(cfg.udp_server_port); }},
        {"ipc_server_port",
         [](const auto& cfg) { return std::to_string(cfg.ipc_server_port); }},
    };

    // ========== 3. 遍历所有字段，自动对比并打印差异（核心逻辑） ==========
    for (const auto& field : fields) {
      // 用Lambda捕捉新旧配置，获取字段值的字符串表示
      std::string old_val = field.get_value(old_config);
      std::string new_val = field.get_value(new_config);

      // 只有值不同时才打印差异
      if (old_val != new_val) {
        LOG_INFO("[LOGGING DIFF] {}: {} -> {}", field.name, old_val, new_val);
      }
    }
  }
};

REGISTER_CONFIG_MODULE([](config::ConfigManager& mgr) {
  // 注册 LoggingConfigManager 到 ConfigManager
  mgr.addObserver(&logging::LoggingConfigManager::getInstance());
  LOG_INFO("日志模块已注册到配置管理器");
});

}  // namespace logging
