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
 *  - File: AsioTcpTransport.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// Copyright (c) 2025 caomengxuan666
#pragma once
#include <string>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/streambuf.hpp"
#include "protocol/TransportAdapter.hpp"

namespace protocol {

class AsioTcpTransport : public ITransportAdapter {
  enum class ReceiveState {
    WaitingForStartSignal,  // 等待开始信号
    ReceivingFeatureData,   // 接收特征数据
    ReceivingStatusData     // 接收状态数据
  };

 public:
  explicit AsioTcpTransport(asio::io_context& io_ctx);
  ~AsioTcpTransport() override;

  void async_connect(const std::string& ip, uint16_t port,
                     std::function<void(std::error_code)> callback) override;
  void async_send(std::span<const uint8_t> data,
                  SendCallback callback) override;
  void async_receive(ReceiveCallback callback) override;
  void close() override;
  asio::io_context& get_io_context() { return io_ctx_; }

 private:
  void receive_status_data(ReceiveCallback callback);
  void receive_feature_data(ReceiveCallback callback);
  asio::io_context& io_ctx_;
  asio::ip::tcp::socket socket_;
  asio::streambuf read_buffer_;
  bool is_connected_ = false;
  ReceiveState state_ = ReceiveState::WaitingForStartSignal;

  void async_read_exact(
      size_t size,
      std::function<void(std::error_code, std::span<const uint8_t>)> handler);
};

}  // namespace protocol
