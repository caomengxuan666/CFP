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
 *  - File: RedisClient.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// Copyright (c) 2025 caomengxuan666
#pragma once
#include <asio.hpp>
#include <functional>
#include <memory>
#include <string>

// 前置声明
namespace cpp_redis {
class client;
class subscriber;
}  // namespace cpp_redis

namespace redis {

class RedisClient {
 public:
  using MessageCallback = std::function<void(const std::string& channel,
                                             const std::string& message)>;

  explicit RedisClient(asio::io_context& io_ctx);
  ~RedisClient();

  // 发布消息（使用 client）
  void publish(const std::string& channel, const std::string& message);

  // 订阅（使用 subscriber）
  void subscribe(const std::string& channel, MessageCallback callback);
  void psubscribe(const std::string& pattern, MessageCallback callback);

  // 连接
  void connect(const std::string& host = "127.0.0.1", uint16_t port = 6379);

 private:
  asio::io_context& io_ctx_;
  std::unique_ptr<cpp_redis::client> client_;
  std::unique_ptr<cpp_redis::subscriber> subscriber_;
};

}  // namespace redis
