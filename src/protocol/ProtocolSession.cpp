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
 *  - File: ProtocolSession.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// Copyright (c) 2025 caomengxuan666
#include "protocol/ProtocolSession.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace protocol {

ProtocolSession::ProtocolSession(
    std::unique_ptr<ICodec> codec,
    std::unique_ptr<ITransportAdapter> config_transport,
    std::unique_ptr<ITransportAdapter> report_transport)
    : codec_(std::move(codec)),
      config_transport_(std::move(config_transport)),
      report_transport_(std::move(report_transport)) {}

ProtocolSession::~ProtocolSession() {}

void ProtocolSession::async_connect(
    const std::string& ip, uint16_t port,
    std::function<void(std::error_code)> callback) {
  config_transport_->async_connect(
      ip, port, [this, callback](std::error_code ec) {
        if (ec) {
          std::cerr << "Connect failed: " << ec.message() << std::endl;
        }
        callback(ec);
      });
}

void ProtocolSession::async_receive_features(FeaturesCallback callback) {
  report_transport_->async_receive(
      [this, callback](std::error_code ec, auto data) {
        if (ec) {
          callback(nullptr);
          return;
        }

        auto report = codec_->decode_features(data);
        if (report) {
          callback(std::make_shared<FeatureReport>(*report));
        } else {
          callback(nullptr);
        }
      });
}

void ProtocolSession::async_receive_status(StatusCallback callback) {
  report_transport_->async_receive(
      [this, callback](std::error_code ec, auto data) {
        if (ec) {
          callback(nullptr);
          return;
        }

        auto status = codec_->decode_status(data);
        if (status) {
          callback(std::make_shared<FrontendStatus>(*status));
        } else {
          callback(nullptr);
        }
      });
}

void ProtocolSession::async_send_features(const FeatureReport& report,
                                          SendCallback callback) {
  auto data = codec_->encode_features(report);
  report_transport_->async_send(data, callback);
}

void ProtocolSession::async_send_status(const FrontendStatus& status,
                                        SendCallback callback) {
  auto data = codec_->encode_status(status);
  report_transport_->async_send(data, callback);
}

void ProtocolSession::async_send_telemetry(const TelemetryData& telemetry,
                                           SendCallback callback) {
  // 构建 ServerConfig 用于 19700 遥测数据
  ServerConfig config;
  config.roll_id = telemetry.roll_id;
  config.head_length = telemetry.length;  // 复用 head_length 字段存储料长

  // 编码使用 ICodec 接口
  auto data = codec_->encode_config(config);

  // 通过 config_transport_ 发送到 19700
  config_transport_->async_send(data, callback);
}

}  // namespace protocol
