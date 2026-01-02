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
 *  - File: RedisClient.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// Copyright (c) 2025 caomengxuan666
#include "redis/RedisClient.hpp"

#include <condition_variable>
#include <cpp_redis/cpp_redis>
#include <iostream>
#include <mutex>
#include <tacopie/tacopie>

namespace redis {

RedisClient::RedisClient(asio::io_context& io_ctx)
    : io_ctx_(io_ctx),
      client_(std::make_unique<cpp_redis::client>()),
      subscriber_(std::make_unique<cpp_redis::subscriber>()) {}

RedisClient::~RedisClient() {
  if (client_) client_->disconnect(true);
  if (subscriber_) subscriber_->disconnect(true);
}

void RedisClient::connect(const std::string& host, uint16_t port) {
  // 启用日志（可选）
  cpp_redis::active_logger = std::make_unique<cpp_redis::logger>();

  // 连接 subscriber（用于订阅）
  subscriber_->connect(
      host, port,
      [host, port](const std::string& h, std::size_t p,
                   cpp_redis::subscriber::connect_state status) {
        if (status == cpp_redis::subscriber::connect_state::dropped) {
          std::cerr << "Redis subscriber disconnected from " << h << ":" << p
                    << std::endl;
        }
      });

  // 连接 client（用于发布）
  client_->connect(host, port,
                   [host, port](const std::string& h, std::size_t p,
                                cpp_redis::client::connect_state status) {
                     if (status == cpp_redis::client::connect_state::dropped) {
                       std::cerr << "Redis client disconnected from " << h
                                 << ":" << p << std::endl;
                     }
                   });
}

void RedisClient::publish(const std::string& channel,
                          const std::string& message) {
  if (!client_ || !client_->is_connected()) return;

  client_->publish(channel, message);
  client_->commit();  // v4 使用 commit() 提交命令
}

void RedisClient::subscribe(const std::string& channel,
                            MessageCallback callback) {
  if (!subscriber_ || !subscriber_->is_connected()) return;

  // 存储回调（需线程安全）
  static std::mutex cb_mutex;
  static std::function<void(const std::string&, const std::string&)> global_cb;

  {
    std::lock_guard lock(cb_mutex);
    global_cb = std::move(callback);
  }

  // 设置订阅回调
  subscriber_->subscribe(channel, [&io_ctx = io_ctx_](const std::string& ch,
                                                      const std::string& msg) {
    asio::post(io_ctx, [ch, msg]() {
      static std::mutex cb_mutex;
      std::lock_guard lock(cb_mutex);
      if (global_cb) {
        global_cb(ch, msg);
      }
    });
  });
  subscriber_->commit();
}

void RedisClient::psubscribe(const std::string& pattern,
                             MessageCallback callback) {
  if (!subscriber_ || !subscriber_->is_connected()) return;

  static std::mutex cb_mutex;
  static std::function<void(const std::string&, const std::string&)> global_pcb;

  {
    std::lock_guard lock(cb_mutex);
    global_pcb = std::move(callback);
  }

  subscriber_->psubscribe(pattern, [&io_ctx = io_ctx_](const std::string& ch,
                                                       const std::string& msg) {
    asio::post(io_ctx, [ch, msg]() {
      static std::mutex cb_mutex;
      std::lock_guard lock(cb_mutex);
      if (global_pcb) {
        global_pcb(ch, msg);
      }
    });
  });
  subscriber_->commit();
}

}  // namespace redis
