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
 *  - File: codec.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025
 */

// Copyright (c) 2025 caomengxuan666
#pragma once
#include <optional>
#include <span>

#include "messages.hpp"

namespace protocol {

/// @brief 编解码器接口（未来 gRPC 可直接替换实现）
class ICodec {
 public:
  virtual ~ICodec() = default;

  virtual std::vector<uint8_t> encode_config(const ServerConfig& config) = 0;
  virtual std::optional<ServerConfig> decode_config(
      std::span<const uint8_t> data) = 0;

  virtual std::vector<uint8_t> encode_features(const FeatureReport& report) = 0;
  virtual std::optional<FeatureReport> decode_features(
      std::span<const uint8_t> data) = 0;

  virtual std::vector<uint8_t> encode_status(const FrontendStatus& status) = 0;
  virtual std::optional<FrontendStatus> decode_status(
      std::span<const uint8_t> data) = 0;
};

}  // namespace protocol
