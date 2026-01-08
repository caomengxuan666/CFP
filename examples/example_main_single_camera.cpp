/*
 *  Copyright © 2025-2026 [caomengxuan666]
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
 *  - File: example_main.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */

// Copyright (c) 2025 caomengxuan666
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <atomic>   // 新增：用于退出标志
#include <chrono>   // 新增：用于休眠
#include <csignal>  // 新增：用于捕获退出信号
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>  // 新增：用于线程操作

// 其他头文件
#include "algo/AlgoBase.hpp"
#include "algo/HoleDetection.hpp"
#include "cameras/CameraCapture.hpp"
#include "cameras/CameraFactory.hpp"
#include "cameras/EventHandlers.hpp"
#include "config/ConfigManager.hpp"
#include "debug/CrashHandler.hpp"
#include "logging/CaponLogging.hpp"
#include "logging/LoggingConfigManager.hpp"
#include "spdlog/spdlog.h"

int main() {
  // 为了捕获所有异常，使用 try-catch 包裹整个程序逻辑
  try {
    // 首先加载全局配置并应用日志配置
    auto global_config = config::GlobalConfig::load();
    logging::LoggingConfigManager::getInstance().applyGlobalConfig(
        global_config);
    LOG_INFO("日志初始化完成");

    // 初始化崩溃处理器
    CrashHandler::initialize();

    // 初始化相机其他事件默认的所有handlers
    register_all_handlers();

    // 初始配置管理器 (可以在运行时从配置文件直接动态更新算法)
    auto& config_manager = config::ConfigManager::instance();
    config_manager.start();
    LOG_INFO("配置管理器已启动，开始监听配置文件变化...");

    inicpp::IniManager ini(config::get_default_config_path());
    auto cameras =
        create_cameras_from_config(global_config.camera_entries, ini);

    // 检查相机是否存在
    if (cameras.empty()) {
      LOG_WARN("没有找到可用的IKap相机，程序进入阻塞模式,按下Enter退出");
      std::cin.get();
    } else {
      const auto& camera0 = cameras[0];
      // 启动相机
      LOG_INFO("启动第一个相机");
      camera0->start();

      // 未来这里的算法取决于配置文件，从里面读取并且创建
      auto holeDetection =
          config_manager.create_algorithm<algo::HoleDetection>();
      algo::AlgoAdapter adapter(holeDetection);  // 栈上对象，持有 shared_ptr
                                                 // AlgoAdapter
      camera0->add_frame_processor(adapter);
      // 延迟启动算法的lambda,可以做到开始信号来了之后在绑定

      std::cout << "Running single_camera system... Press Enter to exit.\n";
      std::cin.get();
      // 清理相机资源
      LOG_INFO("停止相机");
      camera0->stop();
    }

    // ========== 按Issue #1173 要求的清理顺序 ==========
    // 1. 先刷新日志（确保所有日志入队）
    CaponLogger::instance().flush();
    // 2. 销毁所有logger（从注册表移除，释放对线程池的weak_ptr引用）
    CaponLogger::instance().cleanup();
    // 3. 短暂休眠，让线程池处理完剩余日志（保险）
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } catch (const std::runtime_error& e) {
    LOG_ERROR("运行时异常: {}", e.what());
    std::cerr << "运行时异常: " << e.what() << std::endl;
    CaponLogger::instance().flush();
    CaponLogger::instance().cleanup();
    return 1;
  } catch (const spdlog::spdlog_ex& e) {
    std::cerr << "日志系统异常: " << e.what() << std::endl;
    CaponLogger::instance().flush();
    CaponLogger::instance().cleanup();
    return 2;
  } catch (const std::exception& e) {
    LOG_ERROR("通用异常: {}", e.what());
    std::cerr << "通用异常: " << e.what() << std::endl;
    CaponLogger::instance().flush();
    CaponLogger::instance().cleanup();
    return 3;
  } catch (...) {
    std::cerr << "未知异常发生" << std::endl;
    CaponLogger::instance().flush();
    CaponLogger::instance().cleanup();
    return 4;
  }

  // 清理崩溃处理器
  CrashHandler::cleanup();
  // ========== 最后一步：调用spdlog::shutdown()销毁全局线程池（按Issue要求）
  // ==========
  spdlog::shutdown();

  LOG_INFO("程序已完全退出");
  return 0;
}
