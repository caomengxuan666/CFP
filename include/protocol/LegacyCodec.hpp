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
 *  - File: LegacyCodec.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

// Copyright (c) 2025 caomengxuan666
#pragma once
#include <span>
#include <string>
#include <vector>

#include "protocol/codec.hpp"

namespace protocol {

/// @brief 老协议编解码器（兼容所有现场）
class LegacyCodec : public ICodec {
 public:
  std::vector<uint8_t> encode_config(const ServerConfig& config) override;
  std::optional<ServerConfig> decode_config(
      std::span<const uint8_t> data) override;

  std::vector<uint8_t> encode_features(const FeatureReport& report) override;
  std::optional<FeatureReport> decode_features(
      std::span<const uint8_t> data) override;

  std::vector<uint8_t> encode_status(const FrontendStatus& status) override;
  std::optional<FrontendStatus> decode_status(
      std::span<const uint8_t> data) override;
  std::optional<SegmentationParams> decode_segmentation_params(
      std::span<const uint8_t> data);

 private:
  void trim_trailing_spaces(std::string& s);
  void parse_optional_fields(std::span<const uint8_t> data, size_t offset,
                             ServerConfig& config);
};

}  // namespace protocol
