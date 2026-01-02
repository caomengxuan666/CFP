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
 *  - File: BusinessManager.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// BusinessManager.cpp
#include "business/BusinessManager.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "protocol/AsioTcpTransport.hpp"
#include "protocol/LegacyCodec.hpp"

namespace business {

BusinessManager::BusinessManager(asio::io_context& io_ctx,
                                 const std::string& local_ip,
                                 const std::string& main_server_ip,
                                 const std::string& backup_server_ip)
    : io_ctx_(io_ctx),
      main_server_ip_(main_server_ip),
      backup_server_ip_(backup_server_ip) {
  size_t last_dot = local_ip.find_last_of('.');
  machine_id_ = (last_dot != std::string::npos) ? local_ip.substr(last_dot + 1)
                                                : "unknown";
  is_master_ = (machine_id_ == "101");
  redis_host_ = local_ip.substr(0, last_dot + 1) + "101";
  redis_ = std::make_unique<redis::RedisClient>(io_ctx_);
}

void BusinessManager::start() {
  redis_->connect(redis_host_);
  if (is_master_) {
    start_master();
  } else {
    start_worker();
  }
}

void BusinessManager::start_master() {
  // 1. 启动 7000 监听
  start_listening_7000();
  start_listening_19800();

  // 2. 创建主服务器 19300 上报会话
  auto report_codec = std::make_unique<protocol::LegacyCodec>();
  auto report_transport = std::make_unique<protocol::AsioTcpTransport>(io_ctx_);
  report_session_ = std::make_shared<protocol::ProtocolSession>(
      std::move(report_codec), nullptr, std::move(report_transport));
  report_session_->async_connect(
      main_server_ip_, 19300, [](std::error_code ec) {
        if (ec) {
          std::cerr << "Main server 19300 connect failed: " << ec.message()
                    << "\n";
        } else {
          std::cout << "Main server 19300 connected successfully\n";
        }
      });

  // 3. 创建备份服务器 19300 上报会话（如果配置了备份服务器IP）
  if (!backup_server_ip_.empty()) {
    auto backup_report_codec = std::make_unique<protocol::LegacyCodec>();
    auto backup_report_transport =
        std::make_unique<protocol::AsioTcpTransport>(io_ctx_);
    backup_report_session_ = std::make_shared<protocol::ProtocolSession>(
        std::move(backup_report_codec), nullptr,
        std::move(backup_report_transport));
    backup_report_session_->async_connect(
        backup_server_ip_, 19300, [](std::error_code ec) {
          if (ec) {
            std::cerr << "Backup server 19300 connect failed: " << ec.message()
                      << "\n";
          } else {
            std::cout << "Backup server 19300 connected successfully\n";
          }
        });
  }

  // 4. 创建主服务器 19700 遥测会话
  auto telemetry_codec = std::make_unique<protocol::LegacyCodec>();
  auto telemetry_transport =
      std::make_unique<protocol::AsioTcpTransport>(io_ctx_);
  telemetry_session_ = std::make_shared<protocol::ProtocolSession>(
      std::move(telemetry_codec), std::move(telemetry_transport), nullptr);
  telemetry_session_->async_connect(
      main_server_ip_, 19700, [](std::error_code ec) {
        if (ec) {
          std::cerr << "Main server 19700 connect failed: " << ec.message()
                    << "\n";
        } else {
          std::cout << "Main server 19700 connected successfully\n";
        }
      });

  // 5. 创建备份服务器 19700 遥测会话（如果配置了备份服务器IP）
  if (!backup_server_ip_.empty()) {
    auto backup_telemetry_codec = std::make_unique<protocol::LegacyCodec>();
    auto backup_telemetry_transport =
        std::make_unique<protocol::AsioTcpTransport>(io_ctx_);
    backup_telemetry_session_ = std::make_shared<protocol::ProtocolSession>(
        std::move(backup_telemetry_codec),
        std::move(backup_telemetry_transport), nullptr);
    backup_telemetry_session_->async_connect(
        backup_server_ip_, 19700, [](std::error_code ec) {
          if (ec) {
            std::cerr << "Backup server 19700 connect failed: " << ec.message()
                      << "\n";
          } else {
            std::cout << "Backup server 19700 connected successfully\n";
          }
        });
  }

  // 6. 订阅所有遥测数据
  redis_->psubscribe("telemetry/*", [this](const std::string& channel,
                                           const std::string& msg) {
    std::string machine_id = channel.substr(11);  // "telemetry/102" -> "102"
    aggregate_telemetry(machine_id, msg);
  });
}

void BusinessManager::start_worker() {
  // 1. 订阅开始信号
  redis_->subscribe("control/start", [this](const std::string& channel,
                                            const std::string& roll_id) {
    if (start_callback_) {
      start_callback_(roll_id);
    }
  });

  // 2. 创建主服务器 19300 上报会话
  auto report_codec = std::make_unique<protocol::LegacyCodec>();
  auto report_transport = std::make_unique<protocol::AsioTcpTransport>(io_ctx_);
  report_session_ = std::make_shared<protocol::ProtocolSession>(
      std::move(report_codec), nullptr, std::move(report_transport));
  report_session_->async_connect(
      main_server_ip_, 19300, [](std::error_code ec) {
        if (ec) {
          std::cerr << "Main server 19300 connect failed: " << ec.message()
                    << "\n";
        } else {
          std::cout << "Main server 19300 connected successfully\n";
        }
      });

  // 3. 创建备份服务器 19300 上报会话（如果配置了备份服务器IP）
  if (!backup_server_ip_.empty()) {
    auto backup_report_codec = std::make_unique<protocol::LegacyCodec>();
    auto backup_report_transport =
        std::make_unique<protocol::AsioTcpTransport>(io_ctx_);
    backup_report_session_ = std::make_shared<protocol::ProtocolSession>(
        std::move(backup_report_codec), nullptr,
        std::move(backup_report_transport));
    backup_report_session_->async_connect(
        backup_server_ip_, 19300, [](std::error_code ec) {
          if (ec) {
            std::cerr << "Backup server 19300 connect failed: " << ec.message()
                      << "\n";
          } else {
            std::cout << "Backup server 19300 connected successfully\n";
          }
        });
  }
}

void BusinessManager::start_listening_7000() {
  acceptor_7000_ = std::make_unique<asio::ip::tcp::acceptor>(io_ctx_);
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 7000);
  acceptor_7000_->open(endpoint.protocol());
  acceptor_7000_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_7000_->bind(endpoint);
  acceptor_7000_->listen();

  auto socket = std::make_shared<asio::ip::tcp::socket>(io_ctx_);
  acceptor_7000_->async_accept(*socket,
                               [this, socket](const std::error_code& ec) {
                                 if (!ec) {
                                   handle_7000_connection(socket);
                                 }
                                 if (acceptor_7000_->is_open()) {
                                   start_listening_7000();
                                 }
                               });
}

void BusinessManager::handle_7000_connection(
    std::shared_ptr<asio::ip::tcp::socket> socket) {
  auto buffer = std::make_shared<std::vector<uint8_t>>(73);
  asio::async_read(
      *socket, asio::buffer(*buffer),
      [this, socket, buffer](const std::error_code& ec,
                             std::size_t bytes_transferred) {
        if (!ec && bytes_transferred == 73 && (*buffer)[0] == 'O') {
          // 这里需要一个临时的 codec 来解码
          // TODO(cmx) 后续升级成GRPC的时候需要替换这里面
          protocol::LegacyCodec temp_codec;
          auto config = temp_codec.decode_config(*buffer);
          if (config) {
            redis_->publish("control/start", config->roll_id);
          }
        }
      });
}

void BusinessManager::start_listening_19800() {
  acceptor_19800_ = std::make_unique<asio::ip::tcp::acceptor>(io_ctx_);
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 19800);
  acceptor_19800_->open(endpoint.protocol());
  acceptor_19800_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_19800_->bind(endpoint);
  acceptor_19800_->listen();

  auto socket = std::make_shared<asio::ip::tcp::socket>(io_ctx_);
  acceptor_19800_->async_accept(*socket,
                                [this, socket](const std::error_code& ec) {
                                  if (!ec) {
                                    handle_19800_connection(socket);
                                  }
                                  if (acceptor_19800_->is_open()) {
                                    start_listening_19800();
                                  }
                                });
}

void BusinessManager::handle_19800_connection(
    std::shared_ptr<asio::ip::tcp::socket> socket) {
  // 读取完整数据（长度可变）
  auto buffer = std::make_shared<std::vector<uint8_t>>(1024);  // 最大长度

  asio::async_read(*socket, asio::buffer(*buffer),
                   [this, socket, buffer](const std::error_code& ec,
                                          std::size_t bytes_transferred) {
                     if (!ec && bytes_transferred > 0) {
                       // 数据格式：'O' + 72卷号 + ... + 分割定位参数
                       std::vector<uint8_t> full_data(bytes_transferred);
                       std::memcpy(full_data.data(), buffer->data(),
                                   bytes_transferred);

                       // 使用 LegacyCodec 解码为 ServerConfig
                       // TODO(cmx) 未来升级GRPC需要手动替换
                       protocol::LegacyCodec temp_codec;
                       auto config = temp_codec.decode_config(full_data);
                       if (config) {
                         // 应用配置到算法
                         // TODO(cmx)
                         // 未来要实现从这里到算法层的通知以改变配置，目前我提供了多种方案
                         // apply_config_to_algorithms(*config);
                       }
                     }
                   });
}

void BusinessManager::update_telemetry(float width, float length, float speed,
                                       const std::string& roll_id) {
  std::string msg = "w:" + std::to_string(width) +
                    ",l:" + std::to_string(length) +
                    ",s:" + std::to_string(speed) + ",r:" + roll_id;
  redis_->publish("telemetry/" + machine_id_, msg);
}

void BusinessManager::aggregate_telemetry(const std::string& machine_id,
                                          const std::string& msg) {
  std::unique_lock lock(telemetry_mutex_);
  telemetry_data_[machine_id] = msg;
  send_aggregated_telemetry();
}

void BusinessManager::send_aggregated_telemetry() {
  if (!telemetry_session_ || !is_master()) {
    return;
  }

  std::shared_lock lock(telemetry_mutex_);
  if (telemetry_data_.empty()) {
    return;
  }

  float total_width = 0.0f;
  float max_length = 0.0f;
  float total_speed = 0.0f;
  std::string roll_id = "AGGREGATED_ROLL";
  size_t valid_machines = 0;

  // 聚合所有前端机的数据
  for (const auto& [machine_id, msg] : telemetry_data_) {
    // 解析宽度 w:1500.0
    size_t w_pos = msg.find("w:");
    float width = 0.0f;
    if (w_pos != std::string::npos) {
      size_t w_end = msg.find(",", w_pos);
      try {
        if (w_end != std::string::npos) {
          width = std::stof(msg.substr(w_pos + 2, w_end - w_pos - 2));
        } else {
          width = std::stof(msg.substr(w_pos + 2));
        }
        total_width += width;
        valid_machines++;
      } catch (const std::exception&) {
        // 解析失败，跳过
      }
    }

    // 解析长度 l:2000.0
    size_t l_pos = msg.find("l:");
    if (l_pos != std::string::npos) {
      size_t l_end = msg.find(",", l_pos);
      try {
        float length = 0.0f;
        if (l_end != std::string::npos) {
          length = std::stof(msg.substr(l_pos + 2, l_end - l_pos - 2));
        } else {
          length = std::stof(msg.substr(l_pos + 2));
        }
        max_length = std::max(max_length, length);
      } catch (const std::exception&) {
        // 解析失败，跳过
      }
    }

    // 解析速度 s:2.5
    size_t s_pos = msg.find("s:");
    if (s_pos != std::string::npos) {
      size_t s_end = msg.find(",", s_pos);
      try {
        float speed = 0.0f;
        if (s_end != std::string::npos) {
          speed = std::stof(msg.substr(s_pos + 2, s_end - s_pos - 2));
        } else {
          speed = std::stof(msg.substr(s_pos + 2));
        }
        total_speed += speed;
      } catch (const std::exception&) {
        // 解析失败，跳过
      }
    }

    // 获取卷号（使用第一个有效的卷号）
    if (roll_id == "AGGREGATED_ROLL") {
      size_t r_pos = msg.find("r:");
      if (r_pos != std::string::npos) {
        size_t r_end = msg.find(",", r_pos);
        if (r_end != std::string::npos) {
          roll_id = msg.substr(r_pos + 2, r_end - r_pos - 2);
        } else {
          roll_id = msg.substr(r_pos + 2);
        }
      }
    }
  }

  // 计算平均速度
  float avg_speed =
      (valid_machines > 0) ? total_speed / valid_machines : 0.0f;  // NOLINT

  // 构建聚合后的遥测数据
  protocol::TelemetryData telemetry;
  telemetry.roll_id = roll_id;
  telemetry.width = total_width;  // 总宽度
  telemetry.length = max_length;  // 最大长度
  telemetry.speed = avg_speed;    // 平均速度
  telemetry.status_bits = 0;      // 状态位可按需聚合

  // 发送到主服务器 19700
  telemetry_session_->async_send_telemetry(telemetry, [](std::error_code ec) {
    if (ec) {
      std::cerr << "Main server 19700 telemetry send failed: " << ec.message()
                << "\n";
    }
  });

  // 发送到备份服务器 19700（如果存在）
  if (backup_telemetry_session_) {
    backup_telemetry_session_->async_send_telemetry(
        telemetry, [](std::error_code ec) {
          if (ec) {
            std::cerr << "Backup server 19700 telemetry send failed: "
                      << ec.message() << "\n";
          }
        });
  }
}

void BusinessManager::set_start_callback(StartCallback callback) {
  start_callback_ = std::move(callback);
}

}  // namespace business