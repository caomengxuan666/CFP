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
 *  - File: ProtocolSession.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once
#include <functional>
#include <memory>
#include <string>

#include "TransportAdapter.hpp"
#include "codec.hpp"
#include "messages.hpp"

namespace protocol {

struct TelemetryData {
  std::string roll_id;
  float length = 0.0f;
  float width = 0.0f;
  float speed = 0.0f;
  uint32_t status_bits = 0;
};

class ProtocolSession {
 public:
  using ConfigCallback = std::function<void(std::shared_ptr<ServerConfig>)>;
  using FeaturesCallback = std::function<void(std::shared_ptr<FeatureReport>)>;
  using StatusCallback = std::function<void(std::shared_ptr<FrontendStatus>)>;
  using SendCallback = std::function<void(std::error_code)>;
  // 发送遥测数据
  void async_send_telemetry(const TelemetryData& telemetry,
                            SendCallback callback);

  ProtocolSession(std::unique_ptr<ICodec> codec,
                  std::unique_ptr<ITransportAdapter> config_transport,
                  std::unique_ptr<ITransportAdapter> report_transport);
  ~ProtocolSession();

  void async_connect(const std::string& ip, uint16_t port,
                     std::function<void(std::error_code)> callback);
  void async_receive_features(FeaturesCallback callback);
  void async_receive_status(StatusCallback callback);
  void async_send_features(const FeatureReport& report, SendCallback callback);
  void async_send_status(const FrontendStatus& status, SendCallback callback);

 private:
  std::unique_ptr<ICodec> codec_;
  std::unique_ptr<ITransportAdapter> config_transport_;
  std::unique_ptr<ITransportAdapter> report_transport_;
};

}  // namespace protocol
