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
 *  - File: CrashHandler.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */

#include "debug/CrashHandler.hpp"

#include <string>

#include "debug/CrashHandlerImpl.hpp"
#include "logging/CaponLogging.hpp"
#include "logging/CrashLogger.hpp"

// 静态成员初始化
CrashHandlerImpl* CrashHandler::impl = nullptr;

void CrashHandler::initialize() {
  if (!impl) {
    impl = new CrashHandlerImpl();
    LOG_INFO("崩溃处理器已初始化");
  }
}

void CrashHandler::reportFatal(const char* msg) {
  if (!msg) {
    return;
  }
  // 只走最小 IPC，不碰 spdlog
  const auto& ipc_cfg = CaponLogger::instance().getIpcConfig();
  LOG_CRASH_RAW(ipc_cfg.ip, ipc_cfg.port, msg);
}
void CrashHandler::cleanup() {
  if (impl) {
    delete impl;
    impl = nullptr;
  }
}
