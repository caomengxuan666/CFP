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
#include <spdlog/sinks/tcp_sink.h>
//#include <spdlog/sinks/udp_sink.h>
#include <spdlog/spdlog.h>

#include <asio.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

class LogServer {
 public:
  LogServer(uint16_t port)
      : port_(port), acceptor_(io_context_), socket_(io_context_) {
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    std::cout << "日志服务器启动，监听端口: " << port_ << std::endl;
  }

  void start() {
    std::cout << "等待客户端连接..." << std::endl;
    acceptor_.async_accept(socket_, [this](std::error_code ec) {
      if (!ec) {
        std::cout << "客户端已连接: " << socket_.remote_endpoint() << std::endl;

        // 开始接收数据
        startReceive();
      } else {
        std::cerr << "接受连接时出错: " << ec.message() << std::endl;
      }
    });

    // 在另一个线程中运行io_context
    server_thread_ = std::thread([this]() { io_context_.run(); });
  }

  void waitForStop() { server_thread_.join(); }

 private:
  void startReceive() {
    socket_.async_read_some(
        asio::buffer(data_, max_length),
        [this](std::error_code ec, std::size_t length) {
          if (!ec) {
            // 输出接收到的日志
            std::string log_message(data_, length);
            std::cout << "[接收到日志] " << log_message << std::endl;

            // 继续接收下一条日志
            startReceive();
          } else {
            std::cout << "客户端断开连接: " << ec.message() << std::endl;

            // 重新开始接受连接
            socket_ = asio::ip::tcp::socket(io_context_);
            acceptor_.async_accept(socket_, [this](std::error_code ec) {
              if (!ec) {
                std::cout << "新客户端已连接: " << socket_.remote_endpoint()
                          << std::endl;
                startReceive();
              }
            });
          }
        });
  }

  uint16_t port_;
  asio::io_context io_context_;
  asio::ip::tcp::acceptor acceptor_;
  asio::ip::tcp::socket socket_;
  std::thread server_thread_;

  enum { max_length = 1024 };
  char data_[max_length];
};

int main() {
  try {
    LogServer server(8096);
    server.start();

    std::cout << "按 Ctrl+C 停止服务器..." << std::endl;

    // 模拟服务器运行
    for (int i = 0; i < 30; ++i) {
      std::this_thread::sleep_for(1s);
      std::cout << "服务器运行中... (" << i + 1 << "s)" << std::endl;
    }

    server.waitForStop();
  } catch (const std::exception& e) {
    std::cerr << "日志服务器执行出错: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
