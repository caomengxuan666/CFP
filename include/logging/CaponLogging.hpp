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
 *  - File: CaponLogging.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "config/ConfigMacros.hpp"
#include "spdlog/logger.h"

#ifdef NDEBUG  // Release模式
#define MIN_LOG_LEVEL SPDLOG_LEVEL_INFO
#define FIELD_DEPLOYMENT 1
#else  // Debug模式
#define MIN_LOG_LEVEL SPDLOG_LEVEL_TRACE
#define FIELD_DEPLOYMENT 0
#endif

#ifndef FIELD_DEPLOYMENT
#define FIELD_DEPLOYMENT 0
#endif

// 前向声明LoggingConfig结构体
CONFIG_FORWARD_DECLARE(LoggingConfig)

class CaponLogger {
 private:
  // 服务器配置结构
  struct TcpServerConfig {
    std::string ip{"127.0.0.1"};
    uint16_t port{8080};
    uint32_t timeout_ms{3000};
    bool lazy_connect_enable{true};
    spdlog::level::level_enum min_level{
        spdlog::level::err};  // 只发送ERROR及以上
  };

  struct UdpServerConfig {
    std::string ip{"127.0.0.1"};
    uint16_t port{8080};
    spdlog::level::level_enum min_level{
        spdlog::level::err};  // 只发送ERROR及以上
  };

  struct IpcServerConfig {
    std::string ip{"127.0.0.1"};
    uint16_t port{5141};
    std::string protocol{"udp"};  // 从主程序到IPC的连接
    spdlog::level::level_enum min_level{spdlog::level::info};

    bool isTcp() const { return protocol == "tcp"; }
    bool isUdp() const { return protocol == "udp"; }
  };

  // 运行时配置
  struct RuntimeConfig {
    std::atomic<spdlog::level::level_enum> file_level{spdlog::level::info};
    std::atomic<spdlog::level::level_enum> console_level{
        spdlog::level::critical};
    bool async_enabled{true};
    bool tcp_send_enabled{false};
    bool udp_send_enabled{false};
    bool ipc_send_enabled{false};
    size_t queue_size{32768};

    // 支持 TCP / UDP / IPC
    std::variant<TcpServerConfig, UdpServerConfig, IpcServerConfig>
        server_config{UdpServerConfig{}};

    // 获取当前配置类型
    bool isTcpConfig() const { return server_config.index() == 0; }
    bool isUdpConfig() const { return server_config.index() == 1; }

    bool isIpcConfig() const { return server_config.index() == 2; }

    // 获取配置引用
    TcpServerConfig& getTcpConfig() {
      return std::get<TcpServerConfig>(server_config);
    }
    UdpServerConfig& getUdpConfig() {
      return std::get<UdpServerConfig>(server_config);
    }

    IpcServerConfig& getIpcConfig() {
      return std::get<IpcServerConfig>(server_config);
    }
    const TcpServerConfig& getTcpConfig() const {
      return std::get<TcpServerConfig>(server_config);
    }
    const UdpServerConfig& getUdpConfig() const {
      return std::get<UdpServerConfig>(server_config);
    }

    const IpcServerConfig& getIpcConfig() const {
      return std::get<IpcServerConfig>(server_config);
    }
  };

  RuntimeConfig config_;
  std::atomic<bool> initialized_{false};        // 添加初始化标志
  std::atomic<bool> first_init_called_{false};  // 标记是否已调用过首次初始化

  // 主要的logger（组合所有sink）
  std::shared_ptr<spdlog::logger> main_logger_;

  // 单独的console logger（用于特殊需求）
  std::shared_ptr<spdlog::logger> console_logger_;

 public:
  static CaponLogger& instance() {
    static CaponLogger instance;
    return instance;
  }

  // 日志记录函数
  template <typename... Args>
  void logInternal(spdlog::level::level_enum level,
                   spdlog::format_string_t<Args...> fmt, Args&&... args) {
    if (main_logger_ && shouldLog(level)) {
      main_logger_->log(level, fmt, std::forward<Args>(args)...);
    }
  }

  // 带位置信息的日志记录函数
  template <typename... Args>
  void logInternalWithLocation(spdlog::level::level_enum level,
                               const char* file, int line,
                               spdlog::format_string_t<Args...> fmt,
                               Args&&... args) {
    if (main_logger_ && shouldLog(level)) {
      main_logger_->log(spdlog::source_loc{file, line, SPDLOG_FUNCTION}, level,
                        fmt, std::forward<Args>(args)...);
    }
  }

  // 配置相关函数
  void setFileLevel(spdlog::level::level_enum level);
  void setNetworkLevel(spdlog::level::level_enum level);
  bool shouldLog(spdlog::level::level_enum level) const;
  void setConsoleLevel(spdlog::level::level_enum level);
  void enableDebugLogging(bool enable);

  // 网络配置函数
  void enableTcpLogging(bool enable, const std::string& ip = "127.0.0.1",
                        uint16_t port = 5141, uint32_t timeout_ms = 3000);
  void enableUdpLogging(bool enable, const std::string& ip = "127.0.0.1",
                        uint16_t port = 5141);

  void enableIpcLogging(bool enable, const std::string& ip = "127.0.0.1",
                        uint16_t port = 5141);

  void applyConfig(const config::LoggingConfig& logging_config);

  // 获取logger实例
  std::shared_ptr<spdlog::logger> getConsoleLogger() { return console_logger_; }
  std::shared_ptr<spdlog::logger> getMainLogger() { return main_logger_; }

  // 获取IPC配置（只读接口）
  const IpcServerConfig& getIpcConfig() const { return config_.getIpcConfig(); }

  // 强制刷新所有日志
  void flush() {
    if (main_logger_) {
      main_logger_->flush();
    }
    if (console_logger_) {
      console_logger_->flush();
    }
  }

 private:
  CaponLogger() = default;  // 不再在构造函数中初始化
  ~CaponLogger() { flush(); }

  void initialize();
  void setupNetworkSink(std::vector<spdlog::sink_ptr>& sinks);
  void updateLoggerLevels();
  void reinitialize();  // 重新初始化函数

  // 禁用拷贝
  CaponLogger(const CaponLogger&) = delete;
  CaponLogger& operator=(const CaponLogger&) = delete;
};

// ========== 更新日志宏 ==========
// 现在所有的日志都通过main_logger_记录，它会自动分发到所有sink

// TRACE级别：编译期完全消除
#if MIN_LOG_LEVEL <= SPDLOG_LEVEL_TRACE && !FIELD_DEPLOYMENT
#define LOG_TRACE(...)                                             \
  do {                                                             \
    if (CaponLogger::instance().shouldLog(spdlog::level::trace)) { \
      CaponLogger::instance().logInternalWithLocation(             \
          spdlog::level::trace, __FILE__, __LINE__, __VA_ARGS__);  \
    }                                                              \
  } while (0)
#else
#define LOG_TRACE(...) ((void)0)
#endif

// DEBUG级别：有条件保留
#if MIN_LOG_LEVEL <= SPDLOG_LEVEL_DEBUG
#define LOG_DEBUG(...)                                             \
  do {                                                             \
    if (CaponLogger::instance().shouldLog(spdlog::level::debug)) { \
      CaponLogger::instance().logInternalWithLocation(             \
          spdlog::level::debug, __FILE__, __LINE__, __VA_ARGS__);  \
    }                                                              \
  } while (0)
#else
#define LOG_DEBUG(...) ((void)0)
#endif

// INFO及以上：始终存在，运行时控制
#define LOG_INFO(...)                                             \
  do {                                                            \
    if (CaponLogger::instance().shouldLog(spdlog::level::info)) { \
      CaponLogger::instance().logInternalWithLocation(            \
          spdlog::level::info, __FILE__, __LINE__, __VA_ARGS__);  \
    }                                                             \
  } while (0)

#define LOG_WARN(...)                                             \
  do {                                                            \
    if (CaponLogger::instance().shouldLog(spdlog::level::warn)) { \
      CaponLogger::instance().logInternalWithLocation(            \
          spdlog::level::warn, __FILE__, __LINE__, __VA_ARGS__);  \
    }                                                             \
  } while (0)

#define LOG_ERROR(...)                                           \
  do {                                                           \
    if (CaponLogger::instance().shouldLog(spdlog::level::err)) { \
      CaponLogger::instance().logInternalWithLocation(           \
          spdlog::level::err, __FILE__, __LINE__, __VA_ARGS__);  \
    }                                                            \
  } while (0)

#define LOG_CRITICAL(...)                                             \
  do {                                                                \
    if (CaponLogger::instance().shouldLog(spdlog::level::critical)) { \
      CaponLogger::instance().logInternalWithLocation(                \
          spdlog::level::critical, __FILE__, __LINE__, __VA_ARGS__);  \
    }                                                                 \
  } while (0)

// 性能优化宏：完全编译期消除
#ifdef NDEBUG
#define LOG_DEBUG_IF(cond, ...) ((void)0)
#define LOG_TRACE_IF(cond, ...) ((void)0)
#else
#define LOG_DEBUG_IF(cond, ...)                                              \
  do {                                                                       \
    if ((cond) && CaponLogger::instance().shouldLog(spdlog::level::debug)) { \
      CaponLogger::instance().logInternalWithLocation(                       \
          spdlog::level::debug, __FILE__, __LINE__, __VA_ARGS__);            \
    }                                                                        \
  } while (0)
#endif
