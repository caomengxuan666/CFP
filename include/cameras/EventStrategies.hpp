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
 *  - File: EventStrategies.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
#pragma once
#include "cameras/Dvp/DvpEventManager.hpp"
#include "cameras/Ikap/IkapEventManager.hpp"
#include "logging/CaponLogging.hpp"

// =============== 公共策略 ===============
struct TelemetryStrategy {
  template <typename Context>
  static void execute(const Context& ctx) {
    // 上报遥测数据到服务器
  }
};

// =============== IKAP 专属策略 ===============
struct IkapReconnectStrategy {
  static void execute(const IkapEventContext& ctx) {
    if (ctx.type == IkapEventType::DeviceRemove) {
      LOG_WARN("IKAP Camera Disconnected! Reconnecting...");
      // TODO(cmx)
    }
  }
};

// =============== DVP 专属策略 ===============
struct DvpReconnectStrategy {
  static void execute(const DvpEventContext& ctx) {
    if (ctx.type == DvpEventType::FrameTimeout ||
        ctx.type == DvpEventType::FrameLost) {
      LOG_WARN("DVP Frame Issue!");
    }
  }
};
