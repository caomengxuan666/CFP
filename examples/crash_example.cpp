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
 *  - File: crash_example.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
// examples/crash_example.cpp 的完整修正版本
#include <windows.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "config/GlobalConfig.hpp"
#include "debug/CrashHandler.hpp"
#include "logging/CaponLogging.hpp"
#include "logging/LoggingConfigManager.hpp"

void testSegfault() {
  std::cout << "测试1: 段错误 (SIGSEGV) - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发段错误...");
  int* ptr = nullptr;
  *ptr = 42;  // 这里会触发段错误
}

void testDivideByZero() {
  std::cout << "测试2: 除零错误 (SIGFPE) - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发除零错误...");
  volatile int x = 1;
  volatile int y = 0;
  volatile int z = x / y;  // 这里会触发除零错误
}

void testAccessViolation() {
  std::cout << "测试3: Windows访问违规异常 - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发访问违规...");
  // 尝试写入只读内存
  const char* read_only = "read-only string";
  char* ptr = const_cast<char*>(read_only);
  ptr[0] = 'X';  // 这里会触发访问违规
}

void testCppException() {
  std::cout << "测试4: C++异常 - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发C++异常...");
  throw std::runtime_error("测试C++异常 - 这应该被崩溃处理器捕获并记录");
}

void testAbort() {
  std::cout << "测试5: 程序中止 (abort) - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将调用 abort()...");
  abort();  // 触发 SIGABRT
}

void testStackOverflow() {
  std::cout << "测试6: 栈溢出 - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发栈溢出...");
  // 使用递归导致栈溢出
  volatile char large_array[1024 * 1024];  // 分配大数组在栈上
  std::cout << "分配了栈数组，开始递归..." << std::endl;
  testStackOverflow();  // 无限递归
}

void testRaiseException() {
  std::cout << "测试7: 使用 RaiseException - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发 RaiseException...");
  // 触发一个自定义异常代码
  RaiseException(0xDEADBEEF, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}

void testNormalOperation() {
  std::cout << "测试正常操作 - 持续发送心跳日志" << std::endl;

  for (int i = 0; i < 10; ++i) {
    LOG_INFO("心跳日志 #{}: 程序正常运行中...", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  LOG_INFO("正常操作测试完成");
}

void testMemoryCorruption() {
  std::cout << "测试内存损坏 - 3秒后触发" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));

  LOG_INFO("即将触发内存损坏...");

  // 分配内存
  int* array = new int[10];

  // 故意越界写入
  for (int i = 0; i < 100; ++i) {
    array[i] = i;  // 越界写入，可能损坏堆
  }

  // 尝试使用损坏的内存
  delete[] array;

  // 触发崩溃
  int* ptr = nullptr;
  *ptr = 42;
}

void showMenu() {
  std::cout << "\n=== 崩溃捕获测试程序 ===" << std::endl;
  std::cout << "请选择测试类型：" << std::endl;
  std::cout << "1. 段错误 (SIGSEGV)" << std::endl;
  std::cout << "2. 除零错误 (SIGFPE)" << std::endl;
  std::cout << "3. Windows访问违规" << std::endl;
  std::cout << "4. C++异常" << std::endl;
  std::cout << "5. 程序中止 (abort)" << std::endl;
  std::cout << "6. 栈溢出" << std::endl;
  std::cout << "7. 使用 RaiseException" << std::endl;
  std::cout << "8. 内存损坏" << std::endl;
  std::cout << "9. 正常操作测试" << std::endl;
  std::cout << "0. 退出程序" << std::endl;
  std::cout << "\n请选择 (0-9): ";
}

int main(int argc, char* argv[]) {
  try {
    // 初始化日志系统
    auto global_config = config::GlobalConfig::load();
    logging::LoggingConfigManager::getInstance().applyGlobalConfig(
        global_config);

    // 初始化崩溃处理器
    CrashHandler::initialize();

    LOG_INFO("崩溃测试程序启动");
    LOG_INFO("日志级别: 控制台={}, 文件={}, 网络={}",
             global_config.logging_settings.console_level,
             global_config.logging_settings.file_level,
             global_config.logging_settings.network_level);

    if (argc > 1) {
      // 命令行参数直接测试
      std::string test_type = argv[1];

      if (test_type == "segfault") {
        testSegfault();
      } else if (test_type == "dividebyzero") {
        testDivideByZero();
      } else if (test_type == "access") {
        testAccessViolation();
      } else if (test_type == "cpp") {
        testCppException();
      } else if (test_type == "abort") {
        testAbort();
      } else if (test_type == "stack") {
        testStackOverflow();
      } else if (test_type == "raise") {
        testRaiseException();
      } else if (test_type == "memory") {
        testMemoryCorruption();
      } else if (test_type == "normal") {
        testNormalOperation();
      } else if (test_type == "all") {
        // 运行所有测试
        testSegfault();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        testDivideByZero();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        testAccessViolation();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        testCppException();
      } else {
        std::cout << "未知测试类型: " << test_type << std::endl;
        std::cout << "可用选项: segfault, dividebyzero, access, cpp, abort, "
                     "stack, raise, memory, normal, all"
                  << std::endl;
        return 1;
      }
    } else {
      // 交互式菜单
      while (true) {
        showMenu();

        int choice;
        if (!(std::cin >> choice)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << "无效输入，请重试。" << std::endl;
          continue;
        }

        if (choice == 0) {
          LOG_INFO("测试程序正常退出");
          break;
        }

        try {
          switch (choice) {
            case 1:
              testSegfault();
              break;
            case 2:
              testDivideByZero();
              break;
            case 3:
              testAccessViolation();
              break;
            case 4:
              testCppException();
              break;
            case 5:
              testAbort();
              break;
            case 6:
              testStackOverflow();
              break;
            case 7:
              testRaiseException();
              break;
            case 8:
              testMemoryCorruption();
              break;
            case 9:
              testNormalOperation();
              break;
            default:
              std::cout << "无效选择，请重试。" << std::endl;
              continue;
          }
        } catch (const std::exception& e) {
          std::cout << "\n测试过程中捕获到C++异常: " << e.what() << std::endl;
          std::cout << "此异常已被崩溃处理器记录并发送到IPC。" << std::endl;
        } catch (...) {
          std::cout << "\n测试过程中捕获到未知异常" << std::endl;
          std::cout << "此异常已被崩溃处理器记录并发送到IPC。" << std::endl;
        }

        if (choice >= 1 && choice <= 8) {
          // 崩溃测试后程序会终止，所以这里不会执行
          std::cout << "\n如果看到这条消息，说明崩溃处理器可能没有正常工作。"
                    << std::endl;
        } else {
          // 正常操作测试后等待
          std::cout << "\n测试完成。检查IPC日志是否接收到测试信息。"
                    << std::endl;
          std::cout << "按回车键继续...";
          std::cin.ignore();
          std::cin.get();
        }
      }
    }

    // 清理（只有正常退出才会执行到这里）
    CrashHandler::cleanup();
    return 0;

  } catch (const std::exception& e) {
    std::cerr << "程序启动失败: " << e.what() << std::endl;
    return 1;
  }
}
