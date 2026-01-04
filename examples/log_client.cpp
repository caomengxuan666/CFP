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
 *  - File: log_client.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
#include <chrono>
#include <iostream>
#include <thread>

#include "config/GlobalConfig.hpp"
#include "logging/CaponLogging.hpp"
#include "logging/LoggingConfigManager.hpp"

using namespace std::chrono_literals;

int main() {
  try {
    // 加载全局配置
    auto global_config = config::GlobalConfig::load();

    // 应用日志配置
    logging::LoggingConfigManager::getInstance().applyGlobalConfig(
        global_config);

    std::cout << "日志客户端已启动..." << std::endl;

    // 模拟发送不同级别的日志
    for (int i = 0; i < 10; ++i) {
      LOG_TRACE("这是第 {} 条跟踪日志", i);
      LOG_DEBUG("这是第 {} 条调试日志", i);
      LOG_INFO("这是第 {} 条信息日志", i);
      LOG_WARN("这是第 {} 条警告日志", i);
      LOG_ERROR("这是第 {} 条错误日志", i);
      LOG_CRITICAL("这是第 {} 条严重日志", i);

      // 如果启用了网络日志，则会发送到远程服务器
      std::this_thread::sleep_for(1s);
    }

    std::cout << "日志客户端执行完成" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "日志客户端执行出错: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
