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
 *  - File: CrashHandlerImpl.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
// src/debug/CrashHandlerImpl.hpp
#pragma once

// Windows 头文件只在实现文件中包含
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00  // Windows 10
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <string>

// Windows minidump支持
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

class CrashHandlerImpl {
 private:
  static bool initialized;
  // 保存原始terminate处理器
  static std::terminate_handler original_terminate_;

  // 生成崩溃文件名
  static std::string generateCrashFilename(const char* extension);

  // 使用指定文件名写入minidump
  static std::string writeMiniDumpWithFilename(PEXCEPTION_POINTERS p,
                                               const std::string& filename);

 public:
  // 构造函数和析构函数
  CrashHandlerImpl();
  ~CrashHandlerImpl();

  // 初始化
  void initializeImpl();

  // 信号处理器
  static void signalHandler(int signal);

  // C++异常处理器
  static void terminateHandler();

  // Windows 未处理异常过滤器
  static LONG WINAPI
  unhandledExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo);

  // 获取调用栈
  static std::string getStackTrace();

  // 获取异常描述
  static std::string getExceptionDescription(DWORD exceptionCode);

  // 写Minidump文件
  static void writeMiniDump(PEXCEPTION_POINTERS p);

  // 发送最小崩溃信息给IPC/TCP/UDP的日志服务器（安全方式）
  static void sendCrashMinimal(PEXCEPTION_POINTERS p);

  // 发送最小SEH信息
  static void sendCrashMinimalSEH(PEXCEPTION_POINTERS p,
                                  const std::string& dump_filename = "");

  // 记录详细的SEH信息
  static void logCrashDetailedSEH(PEXCEPTION_POINTERS p);

  // 启动子进程发送Minidump给崩溃处理服务器
  static void launchCrashReporter(PEXCEPTION_POINTERS p,
                                  const char* minidump_path);
  // 记录C++异常
  static void logCppException(const char* message);
  static void logCppException(const std::exception& e);
  static void safeFormat(char* buffer, size_t size, const char* format, ...);

  // 检查PDB是否匹配
  static bool isPdbMatched();
};
