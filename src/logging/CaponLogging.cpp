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
 *  - File: CaponLogging.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
#include "logging/CaponLogging.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "config/GlobalConfig.hpp"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/tcp_sink.h"
#include "spdlog/sinks/udp_sink.h"
#include "spdlog/spdlog.h"
#include "utils/executable_path.h"

void CaponLogger::initialize() {
  // 检查是否已经初始化，防止重复初始化
  if (initialized_.exchange(true)) {
    if (console_logger_) {
      console_logger_->warn("日志系统已经初始化，跳过重复初始化");
    }
    return;
  }

  first_init_called_.store(true);  // 标记首次初始化已执行
  // 创建sinks集合
  std::vector<spdlog::sink_ptr> sinks;

  // 统一的日志格式
  std::string common_pattern = "[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v";

  // 控制台的加上颜色
  std::string color_pattern = "%^[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#]%$ %v";
  // 1. 控制台sink（彩色输出）

  auto console_sink =
      std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
  console_sink->set_level(config_.console_level.load());
  console_sink->set_pattern(color_pattern);
  // 创建单独的console logger
  console_logger_ =
      std::make_shared<spdlog::logger>("capon_console", console_sink);

  // 控制台sink也加入到主sinks集合
  sinks.push_back(console_sink);

  // 2. 文件sink（异步或同步）
  const std::string log_name =
      CFPUtils::getExecutableDirectory() + "/log/capon/main.log";

  auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      log_name, 1024 * 1024 * 100, 10);
  file_sink->set_pattern(common_pattern);
  file_sink->set_level(config_.file_level.load());
  sinks.push_back(file_sink);

  // 3. 网络sink（TCP/UDP）
  setupNetworkSink(sinks);

  // ===== 关键修改：按Issue #1173 要求创建线程池 =====
  if (config_.async_enabled) {
    // 只初始化一次全局线程池（避免重复创建）
    static bool global_pool_inited = false;
    if (!global_pool_inited) {
      spdlog::init_thread_pool(config_.queue_size,
                               2);  // 全局线程池，只创建一次
      global_pool_inited = true;
    }
    // 获取全局线程池的weak_ptr（符合async_logger要求）
    auto tp = spdlog::thread_pool();

    // 创建异步logger，使用全局线程池（weak_ptr）
    main_logger_ = std::make_shared<spdlog::async_logger>(
        "capon_main", sinks.begin(), sinks.end(), tp,  // 用全局线程池的weak_ptr
        spdlog::async_overflow_policy::block);
  } else {
    // 同步模式
    main_logger_ = std::make_shared<spdlog::logger>("capon_main", sinks.begin(),
                                                    sinks.end());
  }

  // 设置主logger的全局级别为最低，各个sink有自己的过滤级别
  main_logger_->set_level(spdlog::level::trace);

  // 注册为默认logger（关键：让注册表管理logger生命周期）
  spdlog::register_logger(main_logger_);

  if (main_logger_) {
    main_logger_->info("Logger initialized with {} sink(s)", sinks.size());
  }
  if (config_.tcp_send_enabled || config_.udp_send_enabled) {
    std::cout << "NetWork Logger Transport on" << std::endl;
  }
}
void CaponLogger::reinitialize() {
  // 对于重新初始化，我们直接清理现有资源并重新创建
  if (main_logger_) {
    spdlog::drop(main_logger_->name());
    main_logger_.reset();
  }
  if (console_logger_) {
    spdlog::drop(console_logger_->name());
    console_logger_.reset();
  }
  // 标记为未初始化
  initialized_.store(false);
  // 重新初始化
  initialize();
}

void CaponLogger::cleanup() {
  // 1. 强制刷新所有日志（确保最后一条日志能刷出去）
  flush();

  // 2.
  // 只销毁logger，不销毁全局线程池（关键！按Issue要求，线程池交给spdlog::shutdown()处理）
  if (main_logger_) {
    spdlog::drop(main_logger_->name());  // 从注册表移除logger
    main_logger_.reset();
  }
  if (console_logger_) {
    spdlog::drop(console_logger_->name());
    console_logger_.reset();
  }

  // 3. 标记为未初始化（仅重置状态，不碰线程池）
  initialized_.store(false);
  first_init_called_.store(false);
}

void CaponLogger::setupNetworkSink(std::vector<spdlog::sink_ptr>& sinks) {
  // 统一的日志格式
  std::string common_pattern = "[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v";

  try {
    if (config_.tcp_send_enabled && config_.isTcpConfig()) {
      auto& tcp_config = config_.getTcpConfig();

      // 使用构造函数初始化 tcp_sink_config
      spdlog::sinks::tcp_sink_config tcp_sink_config(
          tcp_config.ip,
          tcp_config.port);  // NOLINT
      // 设置其他参数
      tcp_sink_config.timeout_ms = tcp_config.timeout_ms;  // NOLINT
      tcp_sink_config.lazy_connect = tcp_config.lazy_connect_enable;

      auto tcp_sink =
          std::make_shared<spdlog::sinks::tcp_sink_mt>(tcp_sink_config);
      tcp_sink->set_pattern(common_pattern);      // 统一格式
      tcp_sink->set_level(tcp_config.min_level);  // 只记录ERROR及以上

      sinks.push_back(tcp_sink);

      if (console_logger_) {
        console_logger_->info("TCP sink configured to {}:{}", tcp_config.ip,
                              tcp_config.port);
      }

    } else if (config_.udp_send_enabled && config_.isUdpConfig()) {
      auto& udp_config = config_.getUdpConfig();

      // 根据源码，使用 udp_sink_config 结构体
      spdlog::sinks::udp_sink_config udp_sink_config(udp_config.ip,
                                                     udp_config.port);

      auto udp_sink =
          std::make_shared<spdlog::sinks::udp_sink_mt>(udp_sink_config);
      udp_sink->set_pattern(common_pattern);      // 统一格式
      udp_sink->set_level(udp_config.min_level);  // 只记录ERROR及以上

      sinks.push_back(udp_sink);

      if (console_logger_) {
        console_logger_->info("UDP sink configured to {}:{}", udp_config.ip,
                              udp_config.port);
      }
    } else if (config_.ipc_send_enabled && config_.isIpcConfig()) {
      auto& ipc_config = config_.getIpcConfig();

      if (ipc_config.isTcp()) {
        // 创建一个本地 TCP sink（作为 IPC 客户端）
        spdlog::sinks::tcp_sink_config ipc_sink_config(ipc_config.ip,
                                                       ipc_config.port);
        ipc_sink_config.timeout_ms = 3000;
        ipc_sink_config.lazy_connect = true;

        auto ipc_sink =
            std::make_shared<spdlog::sinks::tcp_sink_mt>(ipc_sink_config);
        ipc_sink->set_pattern(common_pattern);
        ipc_sink->set_level(ipc_config.min_level);

        sinks.push_back(ipc_sink);

        if (console_logger_) {
          console_logger_->info("IPC sink configured to {}:{}", ipc_config.ip,
                                ipc_config.port);
        }
      } else if (ipc_config.isUdp()) {
        // UDP sink
        spdlog::sinks::udp_sink_config ipc_sink_config(ipc_config.ip,
                                                       ipc_config.port);

        auto ipc_sink =
            std::make_shared<spdlog::sinks::udp_sink_mt>(ipc_sink_config);
        ipc_sink->set_pattern(common_pattern);
        ipc_sink->set_level(ipc_config.min_level);

        sinks.push_back(ipc_sink);

        if (console_logger_) {
          console_logger_->info("IPC UDP sink configured to {}:{}",
                                ipc_config.ip, ipc_config.port);
        }
      } else {
        if (console_logger_) {
          console_logger_->error("Unsupported IPC protocol: {}",
                                 ipc_config.protocol);
        }
      }
    }
  } catch (const std::exception& e) {
    // 使用单独的console logger记录错误，避免递归
    if (console_logger_) {
      console_logger_->error("Failed to setup network sink: {}", e.what());
    }
  }
}

bool CaponLogger::shouldLog(spdlog::level::level_enum level) const {
  // 👉 优先判断IPC/TCP/UDP（核心：让网络日志优先生效）
  if (config_.ipc_send_enabled && config_.isIpcConfig()) {
    if (level >= config_.getIpcConfig().min_level) {
      return true;
    }
  }
  if (config_.tcp_send_enabled && config_.isTcpConfig()) {
    if (level >= config_.getTcpConfig().min_level) {
      return true;
    }
  }
  if (config_.udp_send_enabled && config_.isUdpConfig()) {
    if (level >= config_.getUdpConfig().min_level) {
      return true;
    }
  }

  // 再判断文件/控制台
  if (level >= config_.file_level.load(std::memory_order_relaxed)) {
    return true;
  }
  if (level >= config_.console_level.load(std::memory_order_relaxed)) {
    return true;
  }

  return false;
}

void CaponLogger::setFileLevel(spdlog::level::level_enum level) {
  config_.file_level.store(level, std::memory_order_release);
  updateLoggerLevels();
}

void CaponLogger::setNetworkLevel(spdlog::level::level_enum level) {
  if (config_.tcp_send_enabled && config_.isTcpConfig()) {
    config_.getTcpConfig().min_level = level;
  }
  if (config_.udp_send_enabled && config_.isUdpConfig()) {
    config_.getUdpConfig().min_level = level;
  }
  updateLoggerLevels();
}

void CaponLogger::setConsoleLevel(spdlog::level::level_enum level) {
  config_.console_level.store(level, std::memory_order_release);
  updateLoggerLevels();
}

void CaponLogger::updateLoggerLevels() {
  if (!main_logger_) {
    return;
  }
  auto& sinks = main_logger_->sinks();
  auto& ipc_cfg = config_.getIpcConfig();
  auto tcp_level = config_.isTcpConfig() ? config_.getTcpConfig().min_level
                                         : spdlog::level::off;
  auto udp_level = config_.isUdpConfig() ? config_.getUdpConfig().min_level
                                         : spdlog::level::off;

  for (auto& sink : sinks) {
    // 控制台Sink
    if (auto console_sink =
            std::dynamic_pointer_cast<spdlog::sinks::stdout_color_sink_mt>(
                sink)) {
      console_sink->set_level(
          config_.console_level.load(std::memory_order_relaxed));
    } else if (auto file_sink = std::dynamic_pointer_cast<
                   spdlog::sinks::rotating_file_sink_mt>(sink)) {
      file_sink->set_level(config_.file_level.load(std::memory_order_relaxed));
    } else if (auto tcp_sink =
                   std::dynamic_pointer_cast<spdlog::sinks::tcp_sink_mt>(
                       sink)) {
      tcp_sink->set_level(config_.tcp_send_enabled ? tcp_level
                                                   : ipc_cfg.min_level);
    } else if (auto udp_sink =
                   std::dynamic_pointer_cast<spdlog::sinks::udp_sink_mt>(
                       sink)) {
      udp_sink->set_level(config_.udp_send_enabled ? udp_level
                                                   : ipc_cfg.min_level);
    }
  }
  // 更新独立控制台logger
  if (console_logger_) {
    console_logger_->set_level(
        config_.console_level.load(std::memory_order_relaxed));
  }
}

void CaponLogger::enableDebugLogging(bool enable) {
  if (enable) {
    setFileLevel(spdlog::level::debug);
    setConsoleLevel(spdlog::level::debug);
  } else {
    setFileLevel(spdlog::level::info);
    setConsoleLevel(spdlog::level::critical);
  }
}

void CaponLogger::enableTcpLogging(bool enable, const std::string& ip,
                                   uint16_t port, uint32_t timeout_ms) {
  config_.tcp_send_enabled = enable;
  config_.udp_send_enabled = false;  // TCP和UDP不能同时启用
  config_.ipc_send_enabled = false;

  if (enable) {
    config_.server_config = TcpServerConfig{ip, port, timeout_ms};
  }

  // 重新初始化以应用新配置
  if (first_init_called_.load()) {
    reinitialize();
  }
}

void CaponLogger::enableUdpLogging(bool enable, const std::string& ip,
                                   uint16_t port) {
  config_.udp_send_enabled = enable;
  config_.tcp_send_enabled = false;  // TCP和UDP不能同时启用
  config_.ipc_send_enabled = false;

  if (enable) {
    config_.server_config = UdpServerConfig{ip, port};
  }

  // 重新初始化以应用新配置
  if (first_init_called_.load()) {
    reinitialize();
  }
}

void CaponLogger::enableIpcLogging(bool enable, const std::string& ip,
                                   uint16_t port) {
  config_.ipc_send_enabled = enable;
  config_.tcp_send_enabled = false;
  config_.udp_send_enabled = false;

  if (enable) {
    config_.server_config = IpcServerConfig{ip, port};
  }

  if (first_init_called_.load()) {
    reinitialize();
  }
}

void CaponLogger::applyConfig(const config::LoggingConfig& logging_config) {
  current_logging_config = logging_config;
  // 将字符串转换为spdlog级别
  spdlog::level::level_enum file_level =
      spdlog::level::from_str(logging_config.file_level);
  spdlog::level::level_enum console_level =
      spdlog::level::from_str(logging_config.console_level);
  spdlog::level::level_enum network_level =
      spdlog::level::from_str(logging_config.network_level);

  // 更新配置
  config_.file_level.store(file_level, std::memory_order_release);
  config_.console_level.store(console_level, std::memory_order_release);
  config_.async_enabled = logging_config.async_enabled;
  config_.queue_size = logging_config.queue_size;
  config_.tcp_send_enabled = logging_config.tcp_send_enabled;
  config_.udp_send_enabled = logging_config.udp_send_enabled;
  config_.ipc_send_enabled = logging_config.ipc_send_enabled;

  // 根据配置设置TCP或UDP服务器配置
  if (logging_config.tcp_send_enabled) {
    config_.server_config = TcpServerConfig{
        logging_config.tcp_server_ip, logging_config.tcp_server_port,
        logging_config.tcp_timeout_ms,
        true  // lazy_connect_enable
    };
    // 设置TCP配置的网络级别
    std::get<TcpServerConfig>(config_.server_config).min_level = network_level;
  } else if (logging_config.udp_send_enabled) {
    config_.server_config = UdpServerConfig{logging_config.udp_server_ip,
                                            logging_config.udp_server_port};
    // 设置UDP配置的网络级别
    std::get<UdpServerConfig>(config_.server_config).min_level = network_level;
  } else if (logging_config.ipc_send_enabled) {
    config_.server_config = IpcServerConfig{
        logging_config.ipc_server_ip, logging_config.ipc_server_port,
        logging_config.ipc_protocol, network_level};
    std::get<IpcServerConfig>(config_.server_config).min_level = network_level;
  }

  // 重新初始化以应用新配置
  if (first_init_called_.load()) {
    reinitialize();
  } else {
    // 如果首次初始化尚未执行，仍然调用initialize以确保初始化发生
    initialize();
  }
}
