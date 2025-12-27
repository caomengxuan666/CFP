/*
 *  Copyright © 2025 [caomengxuan666]
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
 *  - File: TransportAdapter.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025
 */

// Copyright (c) 2025 caomengxuan666
#pragma once
#include <functional>
#include <memory>
#include <span>
#include <system_error>

#include "asio.hpp"

namespace protocol {

class ITransportAdapter {
 public:
  virtual ~ITransportAdapter() = default;

  using SendCallback = std::function<void(std::error_code)>;
  using ReceiveCallback =
      std::function<void(std::error_code, std::vector<uint8_t>)>;

  virtual void async_connect(const std::string& ip, uint16_t port,
                             std::function<void(std::error_code)> callback) = 0;
  virtual void async_send(std::span<const uint8_t> data,
                          SendCallback callback) = 0;
  virtual void async_receive(ReceiveCallback callback) = 0;
  virtual void close() = 0;
};

}  // namespace protocol
