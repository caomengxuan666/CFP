/*
 *  Copyright © 2026 [caomengxuan666]
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
 *  - File: log_server.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */

#include <iostream>
#include <string>

#include "asio.hpp"

int main() {
  try {
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context);
    asio::ip::udp::endpoint listen_endpoint(asio::ip::udp::v4(), 514);
    socket.open(listen_endpoint.protocol());
    socket.set_option(asio::ip::udp::socket::reuse_address(true));
    socket.bind(listen_endpoint);

    std::cout << "UDP 日志服务器启动，监听端口 514..." << std::endl;

    while (true) {
      char data[8192];
      asio::ip::udp::endpoint sender_endpoint;
      size_t length = socket.receive_from(asio::buffer(data), sender_endpoint);
      std::string log_message(data, length);
      std::cout << "[接收到日志] " << log_message << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "日志服务器出错: " << e.what() << std::endl;
  }
  return 0;
}

