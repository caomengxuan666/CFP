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
 *  - File: LoggingConfigManager.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */

#pragma once

#include "config/GlobalConfig.hpp"
#include "logging/CaponLogging.hpp"

namespace logging {
// 直接继承了观察者接口
// 因为这里的日志类的重新应用的接口是完备的所以非常方便
/**
 * @brief 日志的管理器类,继承了观察者接口
 * @note 使用的时候可以直接传入globalConfig对象,他会自动更新loggingConfig到Logger中
 */
class LoggingConfigManager : public config::ConfigObserver {
 public:
  static LoggingConfigManager& getInstance() {
    static LoggingConfigManager instance;
    return instance;
  }

  void applyLoggingConfig(const config::LoggingConfig& loggingConfig) {
    CaponLogger::instance().applyConfig(loggingConfig);
  }

  void applyGlobalConfig(const config::GlobalConfig& globalConfig) {
    applyLoggingConfig(globalConfig.logging_settings);
  }

  // 实现 ConfigObserver 接口
  void onConfigReloaded(const config::GlobalConfig& new_config) override {
    applyGlobalConfig(new_config);
  }

 private:
  LoggingConfigManager() = default;
};

}  // namespace logging
