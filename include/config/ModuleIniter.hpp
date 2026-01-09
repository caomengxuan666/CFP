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
 *  - File: ModuleIniter.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */
#pragma once
#include <functional>
#include <utility>
#include <vector>

namespace config {
// 模块初始化函数类型：接收 ConfigManager 实例，完成自身注册
using ModuleInitFunc = std::function<void(class ConfigManager&)>;

// 全局模块初始化器：管理所有模块的初始化函数
class ModuleIniter {
 public:
  static ModuleIniter& instance() {
    static ModuleIniter inst;
    return inst;
  }

  // 注册模块初始化函数
  void register_init_func(ModuleInitFunc func) {
    funcs_.push_back(std::move(func));
  }

  // 执行所有模块的初始化（由 ConfigManager 统一调用）
  void init_all(ConfigManager& mgr) {
    for (const auto& func : funcs_) {
      func(mgr);
    }
  }

  int get_module_count() const { return funcs_.size(); }

 private:
  ModuleIniter() = default;
  std::vector<ModuleInitFunc> funcs_;
};

// 便捷宏：注册模块初始化函数（一行代码搞定）
#define REGISTER_CONFIG_MODULE(func)                           \
  static bool _module_init_##__LINE__ = []() {                 \
    config::ModuleIniter::instance().register_init_func(func); \
    return true;                                               \
  }()
}  // namespace config
