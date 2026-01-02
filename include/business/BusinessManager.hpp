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
 *  - File: BusinessManager.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// BusinessManager.hpp
#pragma once

#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "asio/ip/tcp.hpp"
#include "protocol/ProtocolSession.hpp"
#include "redis/RedisClient.hpp"

namespace business {

class BusinessManager {
 public:
  BusinessManager(asio::io_context& io_ctx, const std::string& local_ip);
  ~BusinessManager() = default;

  void start();
  void update_telemetry(float width, float length, float speed,
                        const std::string& roll_id);
  bool is_master() const { return is_master_; }
  const std::string& get_machine_id() const { return machine_id_; }

  // 开始信号回调
  using StartCallback = std::function<void(const std::string& roll_id)>;
  void set_start_callback(StartCallback callback);

  // 获取上报会话（用于 19300）
  std::shared_ptr<protocol::ProtocolSession> get_report_session() const {
    return report_session_;
  }

 private:
  void start_master();
  void start_worker();

  // 7000 监听
  void start_listening_7000();
  void handle_7000_connection(std::shared_ptr<asio::ip::tcp::socket> socket);

  // 19800 监听
  void start_listening_19800();
  void handle_19800_connection(std::shared_ptr<asio::ip::tcp::socket> socket);

  // 遥测聚合
  void aggregate_telemetry(const std::string& machine_id,
                           const std::string& msg);
  void send_aggregated_telemetry();

  asio::io_context& io_ctx_;
  bool is_master_;
  std::string machine_id_;
  std::string redis_host_;

  std::unique_ptr<redis::RedisClient> redis_;

  // 协议会话
  std::shared_ptr<protocol::ProtocolSession> report_session_;     // 19300
  std::shared_ptr<protocol::ProtocolSession> telemetry_session_;  // 19700

  // 监听器
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_7000_;
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_19800_;

  // 回调
  StartCallback start_callback_;

  // 遥测数据存储（machine_id -> telemetry_msg）
  mutable std::shared_mutex telemetry_mutex_;
  std::unordered_map<std::string, std::string> telemetry_data_;
};

}  // namespace business
