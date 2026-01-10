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
#include "version/capon_version.h"

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

  SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(  // NOLINT
      calloc(sizeof(SYMBOL_INFO) + 256, 1));             // NOLINT
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

  // 3. 生成带时间戳的Minidump文件
  SYSTEMTIME st;
  GetLocalTime(&st);
  char timestamp[32];
  _snprintf_s(timestamp, sizeof(timestamp), _TRUNCATE,
              "%04d%02d%02d_%02d%02d%02d_%03d_%lu_%lu", st.wYear, st.wMonth,
              st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
              GetCurrentProcessId(), GetCurrentThreadId());

  // 获取当前可执行文件目录
  char exePath[MAX_PATH];
  GetModuleFileNameA(NULL, exePath, MAX_PATH);
  std::string exeDir = exePath;
  exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));

  std::string dump_filename = std::string("crash_") + timestamp + ".dmp";
  std::string full_dump_path =
      exeDir + "\\" + dump_filename;  // Windows路径分隔符
  std::string actual_dump_path =
      writeMiniDumpWithFilename(nullptr, full_dump_path);

  // 4. 发送崩溃报告
  sendCrashMinimal(nullptr);

  // 5. 启动独立崩溃报告进程
  if (!actual_dump_path.empty()) {
    launchCrashReporter(nullptr, actual_dump_path.c_str());
  }

  // 6. 调用原始的terminate处理器
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

  // 生成唯一的文件名前缀（使用时间戳）
  SYSTEMTIME st;
  GetLocalTime(&st);
  char timestamp[32];
  _snprintf_s(timestamp, sizeof(timestamp), _TRUNCATE,
              "%04d%02d%02d_%02d%02d%02d_%03d_%lu_%lu", st.wYear, st.wMonth,
              st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
              GetCurrentProcessId(), GetCurrentThreadId());

  // 1. 记录详细日志
  logCrashDetailedSEH(p);

  // 2. 生成minidump文件（获取文件名）
  std::string dump_filename = std::string("crash_") + timestamp + ".dmp";
  std::string actual_dump_path = writeMiniDumpWithFilename(p, dump_filename);

  // 3. 发送崩溃报告（可以包含文件名）
  sendCrashMinimalSEH(p, actual_dump_path);

  // 4. 启动独立崩溃报告进程
  if (!actual_dump_path.empty()) {
    launchCrashReporter(p, actual_dump_path.c_str());
  }

  // 立刻终止进程,保存原始的崩溃点
  return EXCEPTION_EXECUTE_HANDLER;
}

// ===============================
// Minidump
// ===============================

void CrashHandlerImpl::writeMiniDump(PEXCEPTION_POINTERS p) {
  std::string filename = generateCrashFilename(".dmp");
  writeMiniDumpWithFilename(p, filename);
}

// 生成崩溃文件名
std::string CrashHandlerImpl::generateCrashFilename(const char* extension) {
  SYSTEMTIME st;
  GetLocalTime(&st);

  char filename[256];
  _snprintf_s(filename, sizeof(filename), _TRUNCATE,
              "crash_%04d%02d%02d_%02d%02d%02d_%03d_%lu_%lu%s", st.wYear,
              st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
              st.wMilliseconds, GetCurrentProcessId(), GetCurrentThreadId(),
              extension);
  return filename;
}

// 使用指定文件名写入minidump
std::string CrashHandlerImpl::writeMiniDumpWithFilename(
    PEXCEPTION_POINTERS p, const std::string& filename) {
  HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, nullptr,
                             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (hFile == INVALID_HANDLE_VALUE) {
    return "";
  }

  MINIDUMP_EXCEPTION_INFORMATION mei{};
  mei.ThreadId = GetCurrentThreadId();
  mei.ExceptionPointers = p;
  mei.ClientPointers = FALSE;
  // 为了保证程序大小不太离谱。这么多应该也够了
  MINIDUMP_TYPE dump_type = static_cast<MINIDUMP_TYPE>(
      MiniDumpNormal |                          // 基本信息
      MiniDumpWithThreadInfo |                  // 线程信息（必须）
      MiniDumpWithUnloadedModules |             // 卸载的模块（有用）
      MiniDumpWithCodeSegs |                    // 代码段（用于符号解析）
      MiniDumpWithIndirectlyReferencedMemory |  // 间接引用的内存（用于栈追踪）
      MiniDumpIgnoreInaccessibleMemory |        // 忽略不可访问内存
      MiniDumpWithProcessThreadData);           // 进程/线程数据
  BOOL success =
      MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
                        dump_type, p ? &mei : nullptr, nullptr, nullptr);

  CloseHandle(hFile);
  return success ? filename : "";
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

void CrashHandlerImpl::sendCrashMinimalSEH(PEXCEPTION_POINTERS p,
                                           const std::string& dump_filename) {
  static const auto& ipc_cfg = CaponLogger::instance().getIpcConfig();

  char buf[768];  // 增大缓冲区以容纳文件名

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

    // 如果有dump文件名，包含在消息中
    if (!dump_filename.empty()) {
      _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                  "CRASH pid=%lu tid=%lu code=0x%08X (%s) addr=%p dump=%s",
                  GetCurrentProcessId(), GetCurrentThreadId(), code,
                  desc.c_str(), p->ExceptionRecord->ExceptionAddress,
                  dump_filename.c_str());
    } else {
      _snprintf_s(buf, sizeof(buf), _TRUNCATE,
                  "CRASH pid=%lu tid=%lu code=0x%08X (%s) addr=%p",
                  GetCurrentProcessId(), GetCurrentThreadId(), code,
                  desc.c_str(), p->ExceptionRecord->ExceptionAddress);
    }
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

// 启动独立崩溃报告进程
void CrashHandlerImpl::launchCrashReporter(PEXCEPTION_POINTERS p,
                                           const char* minidump_path) {
  // === 新增：解析主模块 PE 头 ===
  DWORD exe_time_date_stamp = 0;
  DWORD exe_size_of_image = 0;

  HMODULE hModule = GetModuleHandle(nullptr);
  if (hModule) {
    char exe_path[MAX_PATH];
    if (GetModuleFileNameA(hModule, exe_path, MAX_PATH)) {
      HANDLE hFile = CreateFileA(exe_path, GENERIC_READ, FILE_SHARE_READ,
                                 nullptr, OPEN_EXISTING, 0, nullptr);
      if (hFile != INVALID_HANDLE_VALUE) {
        HANDLE hMapping =
            CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (hMapping) {
          LPVOID base = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
          if (base) {
            PIMAGE_DOS_HEADER dos = static_cast<PIMAGE_DOS_HEADER>(base);
            if (dos->e_magic == IMAGE_DOS_SIGNATURE) {
              // NOLINTBEGIN
              PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>(
                  reinterpret_cast<BYTE*>(base) + dos->e_lfanew);
              // NOLINTEND
              if (nt->Signature == IMAGE_NT_SIGNATURE) {
                exe_time_date_stamp = nt->FileHeader.TimeDateStamp;
                exe_size_of_image = nt->OptionalHeader.SizeOfImage;
              }
            }
            UnmapViewOfFile(base);
          }
          CloseHandle(hMapping);
        }
        CloseHandle(hFile);
      }
    }
  }
  // === PE 解析结束 ===
  // 构建命令行参数（栈上分配）
  char cmd_line[4096];
  char exe_path[MAX_PATH];

  // 获取当前exe所在目录，找到crash-reporter.exe
  GetModuleFileNameA(NULL, exe_path, MAX_PATH);
  char* last_slash = strrchr(exe_path, '\\');
  if (last_slash) {
    *last_slash = '\0';
    strcat_s(exe_path, sizeof(exe_path), "\\crash-reporter.exe");
  } else {
    strcpy_s(exe_path, sizeof(exe_path), "crash-reporter.exe");
  }

  // 获取版本号和GUID
  auto& version_instance = capon::version::version();
  const char* version = version_instance.versionString();
  const char* guid = version_instance.guid();
  const char* server_url =
      "http://localhost:3410/upload/minidump";  // TODO(cmx): 从配置获取
  const char* api_key = "test-api-key-123";     // TODO(cmx): 从配置获取
  int exe_age = 0;  // TODO(cmx): 如果需要可以计算exe文件年龄

  // 构建参数 - 注意参数顺序：
  // 1. 服务器URL
  // 2. API密钥
  // 3. minidump路径
  // 4. 版本
  // 5. GUID
  // 6. PID
  // 7. TID
  // 8. 异常码（十六进制，不带0x前缀）
  // 9. 异常地址（十六进制，不带0x前缀）
  // 10. exe_age
  // 11. exe_time_date_stamp（十进制）
  // 12. exe_size_of_image（十进制）
  std::string cmd_line_str = std::format(
      "\"{}\" "  // 1.crash_reporter.exe路径
      "\"{}\" "  // 2.服务器URL
      "\"{}\" "  // 3.API Key
      "\"{}\" "  // 4.minidump文件路径
      "\"{}\" "  // 5.版本号
      "\"{}\" "  // 6.GUID
      "{} "      // 7.pid
      "{} "      // 8.tid
      "{:08X} "  // 9.异常代码（不带0x前缀）
      "{} "      // 10.异常地址（自动格式化为0x...）
      "{} "      // 11.exe_age
      "{} "      // 12.exe_time_date_stamp（十进制）
      "{}",      // 13.exe_size_of_image（十进制）
      exe_path, server_url, api_key, minidump_path, version, guid,
      GetCurrentProcessId(), GetCurrentThreadId(),
      p ? p->ExceptionRecord->ExceptionCode : 0xC0000005,
      // NOLINTNEXTLINE
      p ? p->ExceptionRecord->ExceptionAddress : (void*)0x7FFE12345678, exe_age,
      exe_time_date_stamp, exe_size_of_image);
  // 转换为 C 风格字符串用于 CreateProcessA
  if (cmd_line_str.size() >= sizeof(cmd_line)) {
    printf("错误: 命令行过长 (%zu 字节)\n", cmd_line_str.size());
    return;
  }
  strcpy_s(cmd_line, cmd_line_str.c_str());

  printf("启动崩溃报告器: %s\n", cmd_line);

  // 启动独立进程
  STARTUPINFOA si = {sizeof(si)};
  PROCESS_INFORMATION pi = {0};

  // 关键参数：
  // CREATE_NO_WINDOW: 不创建控制台窗口
  // DETACHED_PROCESS: 独立进程，不附加到控制台
  // CREATE_BREAKAWAY_FROM_JOB: 允许从作业中脱离
  DWORD flags = CREATE_NO_WINDOW | DETACHED_PROCESS | CREATE_BREAKAWAY_FROM_JOB;

  if (CreateProcessA(NULL, cmd_line, NULL, NULL, FALSE, flags, NULL, NULL, &si,
                     &pi)) {
    // 立即关闭句柄，让子进程完全独立
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
  } else {
    DWORD error = GetLastError();
    printf("创建进程失败，错误码: %lu\n", error);
  }
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
