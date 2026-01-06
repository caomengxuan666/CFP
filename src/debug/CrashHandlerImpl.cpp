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
 *  - File: CrashHandlerImpl.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
// src/debug/CrashHandlerImpl.cpp
#include "debug/CrashHandlerImpl.hpp"

#include <atomic>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "logging/CaponLogging.hpp"
#include "logging/CrashLogger.hpp"

// Windows 调试帮助
#include <dbghelp.h>   // NOLINT
#include <winsock2.h>  //NOLINT
#include <ws2tcpip.h>  //NOLINT

#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "ws2_32.lib")

// 防递归
static std::atomic<bool> in_crash_handler{false};

bool CrashHandlerImpl::initialized = false;

std::terminate_handler CrashHandlerImpl::original_terminate_ = nullptr;

CrashHandlerImpl::CrashHandlerImpl() { initializeImpl(); }

CrashHandlerImpl::~CrashHandlerImpl() { SymCleanup(GetCurrentProcess()); }

// ===============================
// Helper: 安全格式化
// ===============================

void CrashHandlerImpl::safeFormat(char* buffer, size_t size, const char* format,
                                  ...) {
  if (size == 0) {
    return;
  }

  va_list args;
  va_start(args, format);
  vsnprintf(buffer, size, format, args);
  va_end(args);

  buffer[size - 1] = '\0';
}

// ===============================
// Initialization
// ===============================

void CrashHandlerImpl::initializeImpl() {
  if (initialized) {
    return;
  }

  // 保存原始terminate处理器
  original_terminate_ = std::get_terminate();

  // 设置我们的terminate处理器
  std::set_terminate(&CrashHandlerImpl::terminateHandler);

  // signal(SIGSEGV, signalHandler);
  // signal(SIGFPE, signalHandler);
  // signal(SIGILL, signalHandler);
  // signal(SIGABRT, signalHandler);

  SetUnhandledExceptionFilter(unhandledExceptionHandler);

  _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

  SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
  SymInitialize(GetCurrentProcess(), nullptr, TRUE);

  initialized = true;
  LOG_INFO("崩溃处理器内部初始化完成");
}

// ===============================
// Stack Trace
// ===============================

std::string CrashHandlerImpl::getStackTrace() {
  std::ostringstream out;

  constexpr int kMaxFrames = 62;
  void* stack[kMaxFrames]{};

  WORD frames = RtlCaptureStackBackTrace(0, kMaxFrames, stack, nullptr);
  if (frames == 0) {
    return "无法获取调用栈\n";
  }

  out << "调用栈 (" << frames << " 帧):\n";

  SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(
      calloc(sizeof(SYMBOL_INFO) + 256, 1));  // NOLINT
  if (!symbol) {
    return "符号内存分配失败\n";
  }

  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  symbol->MaxNameLen = 255;

  HANDLE process = GetCurrentProcess();

  for (WORD i = 0; i < frames; ++i) {
    DWORD64 displacement = 0;
    if (!SymFromAddr(process, reinterpret_cast<DWORD64>(stack[i]),  // NOLINT
                     &displacement, symbol)) {
      continue;
    }

    out << "  #" << std::setw(2) << i << " " << symbol->Name << " (0x"
        << std::hex << symbol->Address << std::dec << ")";

    IMAGEHLP_LINE64 line{};
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    DWORD line_disp = 0;

    if (SymGetLineFromAddr64(process,
                             reinterpret_cast<DWORD64>(stack[i]),  // NOLINT
                             &line_disp, &line) &&
        line.FileName && line.LineNumber > 0 && line.LineNumber < 1'000'000) {
      out << " at " << line.FileName << ":" << line.LineNumber;
    } else {
      out << " (no line info)";
    }

    out << "\n";
  }

  free(symbol);
  return out.str();
}

void CrashHandlerImpl::terminateHandler() {
  // 防递归
  if (in_crash_handler.exchange(true)) {
    // 如果已经在崩溃处理中，调用原始的terminate处理器
    if (original_terminate_) {
      original_terminate_();
    } else {
      std::abort();
    }
    return;
  }

  // 1. 获取异常信息
  std::exception_ptr ex = std::current_exception();
  char message[512] = {0};

  if (ex) {
    try {
      std::rethrow_exception(ex);
    } catch (const std::exception& e) {
      safeFormat(message, sizeof(message), "C++异常: %s", e.what());
    } catch (...) {
      safeFormat(message, sizeof(message), "非标准C++异常");
    }
  } else {
    safeFormat(message, sizeof(message), "std::terminate被调用（无活动异常）");
  }

  // 2. 记录C++异常
  logCppException(message);

  // 3. 生成Minidump（如果需要）
  writeMiniDump(nullptr);

  // 4. 调用原始的terminate处理器
  if (original_terminate_) {
    original_terminate_();
  } else {
    std::abort();
  }
}

// ===============================
// Signal Handler (极限安全路径)
// ===============================

void CrashHandlerImpl::signalHandler(int sig) {
  if (in_crash_handler.exchange(true)) {
    _exit(1);
  }

  writeMiniDump(nullptr);
  sendCrashMinimal(nullptr);

  _exit(sig);
}

void CrashHandlerImpl::logCrashDetailedSEH(PEXCEPTION_POINTERS p) {
  if (!p || !p->ExceptionRecord) {
    return;
  }

  const DWORD code = p->ExceptionRecord->ExceptionCode;
  const void* addr = p->ExceptionRecord->ExceptionAddress;

  std::string desc;
  if (code == 0xE06D7363) {
    // MSVC C++ 异常
    auto ex = std::current_exception();
    if (ex) {
      try {
        std::rethrow_exception(ex);
      } catch (const std::exception& e) {
        desc = std::string("C++异常: ") + e.what();
      } catch (...) {
        desc = "C++异常: 非标准异常";
      }
    } else {
      desc = "C++异常: 无活动异常";
    }
  } else {
    desc = getExceptionDescription(code);
  }
  std::string stack = getStackTrace();

  LOG_CRITICAL(R"(\n=== 程序崩溃 ===
异常代码: 0x{:08X} ({})
异常地址: {}
线程ID: {}
进程ID: {}
{}
)",
               code, desc, addr, GetCurrentThreadId(), GetCurrentProcessId(),
               stack);
}

// ===============================
// Windows SEH
// ===============================

LONG WINAPI CrashHandlerImpl::unhandledExceptionHandler(PEXCEPTION_POINTERS p) {
  if (in_crash_handler.exchange(true)) {
    return EXCEPTION_EXECUTE_HANDLER;
  }

  logCrashDetailedSEH(p);

  writeMiniDump(p);

  // NEW：SEH 专用，允许带异常描述
  sendCrashMinimalSEH(p);

  // 立刻终止进程,保存原始的崩溃点
  return EXCEPTION_EXECUTE_HANDLER;
}

// ===============================
// Minidump
// ===============================

void CrashHandlerImpl::writeMiniDump(PEXCEPTION_POINTERS p) {
  SYSTEMTIME st;
  GetLocalTime(&st);

  char dump_filename[256];
  _snprintf_s(dump_filename, sizeof(dump_filename), _TRUNCATE,
              "crash_%04d%02d%02d_%02d%02d%02d.dmp", st.wYear, st.wMonth,
              st.wDay, st.wHour, st.wMinute, st.wSecond);

  HANDLE hFile = CreateFileA(dump_filename, GENERIC_WRITE, 0, nullptr,
                             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (hFile == INVALID_HANDLE_VALUE) {
    return;
  }

  MINIDUMP_EXCEPTION_INFORMATION mei{};
  mei.ThreadId = GetCurrentThreadId();
  mei.ExceptionPointers = p;
  mei.ClientPointers = FALSE;

  MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
                    static_cast<MINIDUMP_TYPE>(
                        MiniDumpWithThreadInfo | MiniDumpWithFullMemoryInfo |
                        MiniDumpWithHandleData | MiniDumpWithUnloadedModules),
                    p ? &mei : nullptr, nullptr, nullptr);

  CloseHandle(hFile);
}

// ===============================
// Crash Send (Signal-safe)
// ===============================

void CrashHandlerImpl::sendCrashMinimal(PEXCEPTION_POINTERS p) {
  static const auto& ipc_cfg = CaponLogger::instance().getIpcConfig();

  char buf[512];

  if (p && p->ExceptionRecord) {
    _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                "CRASH pid=%lu tid=%lu code=0x%08X addr=%p",
                GetCurrentProcessId(), GetCurrentThreadId(),
                p->ExceptionRecord->ExceptionCode,
                p->ExceptionRecord->ExceptionAddress);
  } else {
    _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                "CRASH pid=%lu tid=%lu code=SIGNAL", GetCurrentProcessId(),
                GetCurrentThreadId());
  }

  LOG_CRASH_RAW(ipc_cfg.ip, ipc_cfg.port, buf);
}

// ===============================
// ⭐ NEW: Crash Send (SEH only)
// ===============================

void CrashHandlerImpl::sendCrashMinimalSEH(PEXCEPTION_POINTERS p) {
  static const auto& ipc_cfg = CaponLogger::instance().getIpcConfig();

  char buf[512];

  if (p && p->ExceptionRecord) {
    DWORD code = p->ExceptionRecord->ExceptionCode;
    std::string desc;
    if (code == 0xE06D7363) {
      auto ex = std::current_exception();
      if (ex) {
        try {
          std::rethrow_exception(ex);

        } catch (const std::exception& e) {
          desc = std::string("C++异常: ") + e.what();

        } catch (...) {
          desc = "C++异常: 非标准异常";
        }

      } else {
        desc = "C++异常: 无活动异常";
      }

    } else {
      desc = getExceptionDescription(code);
    }

    _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                "CRASH pid=%lu tid=%lu code=0x%08X (%s) addr=%p",
                GetCurrentProcessId(), GetCurrentThreadId(), code, desc.c_str(),
                p->ExceptionRecord->ExceptionAddress);
  } else {
    _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                "CRASH pid=%lu tid=%lu code=UNKNOWN", GetCurrentProcessId(),
                GetCurrentThreadId());
  }

  LOG_CRASH_RAW(ipc_cfg.ip, ipc_cfg.port, buf);
}

// ===============================
// PDB Match
// ===============================

bool CrashHandlerImpl::isPdbMatched() {
  HMODULE hModule = GetModuleHandle(nullptr);
  if (!hModule) {
    return false;
  }

  char exe_path[MAX_PATH];
  GetModuleFileNameA(hModule, exe_path, MAX_PATH);

  DWORD64 base = SymLoadModuleEx(GetCurrentProcess(), nullptr, exe_path,
                                 nullptr, 0, 0, nullptr, 0);

  if (!base) {
    return false;
  }

  IMAGEHLP_MODULE64 mod{};
  mod.SizeOfStruct = sizeof(mod);

  return SymGetModuleInfo64(GetCurrentProcess(), base, &mod) &&
         !mod.PdbUnmatched;
}

// ===============================
// Exception Description
// ===============================

void CrashHandlerImpl::logCppException(const char* message) {
  if (!message) {
    return;
  }

  // 构建详细崩溃信息
  char buffer[1024];
  safeFormat(buffer, sizeof(buffer),
             "=== C++异常崩溃 ===\n"
             "异常: %s\n"
             "时间: %s %s\n"
             "线程ID: %lu\n"
             "进程ID: %lu\n",
             message, __DATE__, __TIME__, GetCurrentThreadId(),
             GetCurrentProcessId());
  // 使用安全的日志记录
  static const auto& ipc_cfg = CaponLogger::instance().getIpcConfig();

  LOG_CRASH_RAW(ipc_cfg.ip, ipc_cfg.port, buffer);
  // 同时写入本地文件（双重保险）
  SYSTEMTIME st;
  GetLocalTime(&st);

  char filename[256];
  safeFormat(filename, sizeof(filename),
             "cpp_crash_%04d%02d%02d_%02d%02d%02d.txt", st.wYear, st.wMonth,
             st.wDay, st.wHour, st.wMinute, st.wSecond);

  HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, nullptr);

  if (hFile != INVALID_HANDLE_VALUE) {
    DWORD written;
    WriteFile(hFile, buffer, static_cast<DWORD>(strlen(buffer)), &written,
              nullptr);

    // 添加调用栈
    std::string stack = getStackTrace();
    WriteFile(hFile, "\n调用栈:\n", 8, &written, nullptr);
    WriteFile(hFile, stack.c_str(), static_cast<DWORD>(stack.length()),
              &written, nullptr);

    CloseHandle(hFile);
  }
}

void CrashHandlerImpl::logCppException(const std::exception& e) {
  logCppException(e.what());
}

#ifndef NTSTATUS
typedef long NTSTATUS;  // NOLINT
#endif

#ifndef STATUS_POSSIBLE_DEADLOCK
#define STATUS_POSSIBLE_DEADLOCK ((NTSTATUS)0xC0000194L)
#endif

std::string CrashHandlerImpl::getExceptionDescription(DWORD exceptionCode) {
  switch (exceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
      return "访问违规";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
      return "数组边界检查失败";
    case EXCEPTION_BREAKPOINT:
      return "断点";
    case EXCEPTION_DATATYPE_MISALIGNMENT:
      return "数据类型未对齐";
    case EXCEPTION_FLT_DENORMAL_OPERAND:
      return "浮点操作数非规格化";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
      return "浮点除零";
    case EXCEPTION_FLT_INEXACT_RESULT:
      return "浮点结果不精确";
    case EXCEPTION_FLT_INVALID_OPERATION:
      return "浮点无效操作";
    case EXCEPTION_FLT_OVERFLOW:
      return "浮点上溢";
    case EXCEPTION_FLT_STACK_CHECK:
      return "浮点栈检查失败";
    case EXCEPTION_FLT_UNDERFLOW:
      return "浮点下溢";
    case EXCEPTION_GUARD_PAGE:
      return "保护页违规";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
      return "非法指令";
    case EXCEPTION_IN_PAGE_ERROR:
      return "页错误";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
      return "整数除零";
    case EXCEPTION_INT_OVERFLOW:
      return "整数溢出";
    case EXCEPTION_INVALID_DISPOSITION:
      return "无效的异常处理";
    case EXCEPTION_INVALID_HANDLE:
      return "无效句柄";
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
      return "不可继续的异常";
    case EXCEPTION_PRIV_INSTRUCTION:
      return "特权指令";
    case EXCEPTION_SINGLE_STEP:
      return "单步执行";
    case EXCEPTION_STACK_OVERFLOW:
      return "栈溢出";
    case STILL_ACTIVE:
      return "进程仍在活动";
    case CONTROL_C_EXIT:
      return "Control+C退出";
    case static_cast<DWORD>(EXCEPTION_POSSIBLE_DEADLOCK):
      return "可能的死锁";
    default:
      return "未知异常";
  }
}
