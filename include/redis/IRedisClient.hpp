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
 *  - File: IRedisClient.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once
#include <functional>
#include <string>

namespace redis {

/// @brief Redis 客户端抽象接口（用于解耦和测试）
class IRedisClient {
 public:
  virtual ~IRedisClient() = default;

  // 发布消息
  virtual void publish(const std::string& channel,
                       const std::string& message) = 0;

  // 订阅通道（精确匹配）
  using MessageCallback = std::function<void(const std::string& channel,
                                             const std::string& message)>;
  virtual void subscribe(const std::string& channel,
                         MessageCallback callback) = 0;

  // 模式订阅（用于 telemetry/*）
  virtual void psubscribe(const std::string& pattern,
                          MessageCallback callback) = 0;

  // 连接 Redis
  virtual void connect(const std::string& host = "127.0.0.1",
                       uint16_t port = 6380) = 0;
};

}  // namespace redis
