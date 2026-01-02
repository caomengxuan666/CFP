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
 *  - File: get_local_ip.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#include "utils/get_local_ip.h"

#include <iostream>
#include <string>
#include <vector>

#include "asio/io_context.hpp"
#include "asio/ip/host_name.hpp"
#include "asio/ip/tcp.hpp"

namespace utils {
std::vector<std::string> get_all_local_ips() {
  std::vector<std::string> ips;

  try {
    asio::io_context io;
    asio::ip::tcp::resolver resolver(io);

    std::string hostname = asio::ip::host_name();
    auto endpoints = resolver.resolve(hostname, "");

    for (const auto& endpoint : endpoints) {
      auto addr = endpoint.endpoint().address();
      if (!addr.is_loopback()) {
        ips.push_back(addr.to_string());
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "get_all_local_ips error: " << e.what() << std::endl;
  }

  return ips;
}

std::vector<std::string> get_local_ipv4() {
  std::vector<std::string> ips;

  try {
    asio::io_context io;
    asio::ip::tcp::resolver resolver(io);

    std::string hostname = asio::ip::host_name();
    auto endpoints = resolver.resolve(hostname, "");

    for (const auto& endpoint : endpoints) {
      auto addr = endpoint.endpoint().address();
      if (addr.is_v4() && !addr.is_loopback()) {
        ips.push_back(addr.to_string());
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "get_local_ipv4 error: " << e.what() << std::endl;
  }

  return ips;
}

std::string get_local_ip() {
  auto ips = get_local_ipv4();
  if (!ips.empty()) {
    return ips[0];
  }

  // 如果没有找到IPv4，尝试获取所有IP
  auto all_ips = get_all_local_ips();
  if (!all_ips.empty()) {
    return all_ips[0];
  }

  return "127.0.0.1";
}
}  // namespace utils
