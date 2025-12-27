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
 *  - File: main.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025
 */

#define _WIN32_WINNT 0x0601
/*
 *  Copyright © 2025 [caomengxuan666]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the “Software"), to
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
 *  - File: main.cpp
 *  - CreationYear: 2025
 *  - Date: Sat Dec 20 2025
 *  - Username: Administrator
 *  - CopyrightYear: 2025
 */
#include <iostream>
#include <memory>
#include <utility>

// we must include asio first wo avoid winsock include error
#include "asio.hpp"
// and then we could include others
#include "DvpCameraBuilder.hpp"
#include "algo/AlgoBase.hpp"
#include "algo/HoleDetection.hpp"
#include "config/ConfigManager.hpp"
#include "protocol/AsioTcpTransport.hpp"
#include "protocol/LegacyCodec.hpp"
#include "protocol/ProtocolSession.hpp"

int main() {
  asio::io_context io_context;
  auto work_guard = asio::make_work_guard(io_context.get_executor());
  // 创建协议组件
  auto codec = std::make_unique<protocol::LegacyCodec>();
  auto config_transport =
      std::make_unique<protocol::AsioTcpTransport>(io_context);  // 19700
  auto report_transport =
      std::make_unique<protocol::AsioTcpTransport>(io_context);  // 19300
  auto session = std::make_shared<protocol::ProtocolSession>(
      std::make_unique<protocol::LegacyCodec>(),
      std::make_unique<protocol::AsioTcpTransport>(io_context),
      std::make_unique<protocol::AsioTcpTransport>(io_context));

  // 启动 Asio 事件循环线程
  std::thread asio_thread([&io_context]() { io_context.run(); });

  // 连接服务器 (19700 端口接收配置)
  session->async_connect("192.1.53.9", 19700, [session](std::error_code ec) {
    if (ec) {
      std::cerr << "Connect failed: " << ec.message() << "\n";
      return;
    }
    session->async_receive_config(
        [](std::shared_ptr<protocol::ServerConfig> config) {
          if (config) {
            std::cout << "Config: " << config->roll_id << "\n";
          }
        });
  });

  auto &config_manager = config::ConfigManager::instance();
  config_manager.start();
  auto initial_config = config_manager.get_current_config();

  auto holedetection = config_manager.create_algorithm<algo::HoleDetection>();

  auto camera = std::make_shared<DvpCameraCapture>(
      DvpCameraBuilder::fromUserId("123")
          .bufferQueueSize(10)
          .linkTimeout(5000)
          .onFrame(algo::AlgoAdapter(holedetection))
          .build());

  camera->start();

  std::thread status_thread([session, camera]() {
    while (true) {
      auto status = camera->get_status();
      session->async_send_status(status, [](std::error_code ec) {
        if (ec) { /* log */
        }
      });
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  std::cout << "Running multi-camera system... Press Enter to exit.\n";
  std::cin.get();

  // 清理
  camera->stop();
  work_guard.reset();
  io_context.stop();
  asio_thread.join();
  status_thread.join();  // 等待状态线程退出

  return 0;
}
