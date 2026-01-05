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

CrashHandlerImpl::CrashHandlerImpl() { initializeImpl(); }

CrashHandlerImpl::~CrashHandlerImpl() { SymCleanup(GetCurrentProcess()); }

// ===============================
// Initialization
// ===============================

void CrashHandlerImpl::initializeImpl() {
  if (initialized) {
    return;
  }

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

  std::string desc = getExceptionDescription(code);
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
    std::string desc =
        getExceptionDescription(p->ExceptionRecord->ExceptionCode);

    _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                "CRASH pid=%lu tid=%lu code=0x%08X (%s) addr=%p",
                GetCurrentProcessId(), GetCurrentThreadId(),
                p->ExceptionRecord->ExceptionCode, desc.c_str(),
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
    default:
      return "未知异常";
  }
}
