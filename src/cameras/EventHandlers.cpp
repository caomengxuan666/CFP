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
 *  - File: EventHandlers.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
// business/EventHandlers.cpp
#include "cameras/EventHandlers.hpp"

#include "cameras/EventHandlerRegistry.hpp"
#include "cameras/EventStrategies.hpp"

// TODO(cmx)
// 以后需要在这些Handler里面实现比如说相机状态异常,需要拼接协议状态码发送给服务器

// === 通用遥测 handler ===
static void telemetry_ikap(const IkapEventContext& ctx) {
  TelemetryStrategy::execute(ctx);
}

static void telemetry_dvp(const DvpEventContext& ctx) {
  TelemetryStrategy::execute(ctx);
}

// === IKAP 专用 handler ===
static void ikap_reconnect(const IkapEventContext& ctx) {
  IkapReconnectStrategy::execute(ctx);
}

// === DVP 专用 handler ===
static void dvp_reconnect(const DvpEventContext& ctx) {
  DvpReconnectStrategy::execute(ctx);
}

// === 注册所有 handler ===
void register_all_handlers() {
  auto& reg = EventHandlerRegistry::instance();

  // =============== 通用策略 ===============
  reg.register_ikap_handler("telemetry", telemetry_ikap);
  reg.register_dvp_handler("telemetry", telemetry_dvp);

  // =============== IKAP 事件专用策略 ===============
  reg.register_ikap_handler("reconnect_on_remove", ikap_reconnect);

  // =============== DVP 事件专用策略 ===============
  reg.register_dvp_handler("reconnect_on_timeout", dvp_reconnect);
}
