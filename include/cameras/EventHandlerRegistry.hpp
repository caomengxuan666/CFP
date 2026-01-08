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
 *  - File: EventHandlerRegistry.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "cameras/Dvp/DvpEventManager.hpp"
#include "cameras/Ikap/IkapEventManager.hpp"

class EventHandlerRegistry {
 public:
  using IkapHandler = std::function<void(const IkapEventContext&)>;
  using DvpHandler = std::function<void(const DvpEventContext&)>;

  // 单例
  static EventHandlerRegistry& instance() {
    static EventHandlerRegistry inst;
    return inst;
  }

  // 注册 IKAP handler
  void register_ikap_handler(const std::string& name, IkapHandler handler) {
    ikap_handlers_[name] = std::move(handler);
  }

  // 注册 DVP handler
  void register_dvp_handler(const std::string& name, DvpHandler handler) {
    dvp_handlers_[name] = std::move(handler);
  }

  // 获取 IKAP handler
  IkapHandler get_ikap_handler(const std::string& name) const {
    auto it = ikap_handlers_.find(name);
    return (it != ikap_handlers_.end()) ? it->second : nullptr;
  }

  // 获取 DVP handler
  DvpHandler get_dvp_handler(const std::string& name) const {
    auto it = dvp_handlers_.find(name);
    return (it != dvp_handlers_.end()) ? it->second : nullptr;
  }

 private:
  EventHandlerRegistry() = default;  // 私有构造
  std::unordered_map<std::string, IkapHandler> ikap_handlers_;
  std::unordered_map<std::string, DvpHandler> dvp_handlers_;
};
