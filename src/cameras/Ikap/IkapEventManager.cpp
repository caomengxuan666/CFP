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
 *  - File: IkapEventManager.cpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#include "cameras/Ikap/IkapEventManager.hpp"

#include <algorithm>
#include <unordered_map>

#include "IKapC.h"
#include "cameras/Ikap/IkapCameraCapture.hpp"
#include "utils/magic_enum.hpp"

template <typename E>
static constexpr const char* enum_to_cstr(E e) noexcept {
  static_assert(std::is_enum_v<E>, "E must be an enum type");
  return magic_enum::enum_name(e).data();
}

IkapEventManager::IkapEventManager(ITKDEVICE handle) : handle_(handle) {}

IkapEventManager::~IkapEventManager() {
  for (const auto& pair : device_event_map_) {
    ItkDevUnregisterCallback(handle_, enum_to_cstr(pair.second));
  }
}

// 模板化设备事件回调
// 在之前注册的时候就已经把事件类型和回调函数关联起来,避免动态开销
// 传入到这个函数里面的时候就已经是确定的事件类型了
template <IkapEventType EventType>
void __stdcall IkapEventManager::device_event_callback_template(
    void* context, ITKEVENTINFO event_info) {
  auto* self = static_cast<IkapEventManager*>(context);
  if (auto handler_it = self->handlers_.find(EventType);
      handler_it != self->handlers_.end()) {
    IkapEventContext ctx{self->handle_, EventType, event_info, nullptr, 0};
    handler_it->second(ctx);
  }
}

void IkapEventManager::register_handler(IkapEventType event,
                                        const IkapEventHandler& handler) {
  auto it =
      std::find_if(device_event_map_.begin(), device_event_map_.end(),
                   [event](const auto& pair) { return pair.second == event; });
  if (it != device_event_map_.end()) {
    handlers_[event] = handler;

    using CallbackType = void(__stdcall*)(void*, ITKEVENTINFO);  // NOLINT
    CallbackType callback = nullptr;

    // 根据事件类型选择不同的模板实例
    // 注册的时候就绑定，避免每次事件触发都得要动态解析一次TYPE名称
    if (event == IkapEventType::DeviceRemove) {
      callback = &device_event_callback_template<IkapEventType::DeviceRemove>;
    } else if (event == IkapEventType::FeatureChanged) {
      callback = &device_event_callback_template<IkapEventType::FeatureChanged>;
    } else if (event == IkapEventType::MessageChannel) {
      callback = &device_event_callback_template<IkapEventType::MessageChannel>;
    }

    if (callback) {
      ItkDevRegisterCallback(handle_, enum_to_cstr(event), callback, this);
    }
  }
}

IkapEventManager::StreamEventCallbackType
IkapEventManager::get_stream_event_callback() const {
  return &IkapEventManager::stream_event_callback;
}

const std::unordered_map<uint16_t, IkapEventType>&
IkapEventManager::get_stream_event_map() const {
  return stream_event_map_;
}

void __stdcall IkapEventManager::stream_event_callback(uint32_t event_type,
                                                       void* context) {
  auto* capture = static_cast<class IkapCameraCapture*>(context);
  auto* self = capture->get_event_manager();

  auto it = self->stream_event_map_.find(event_type);
  if (it == self->stream_event_map_.end()) {
    return;
  }

  IkapEventContext ctx{
      self->handle_, it->second, {}, capture->stream_handle_, event_type};

  if (auto handler_it = self->handlers_.find(it->second);
      handler_it != self->handlers_.end()) {
    handler_it->second(ctx);
  }
}

// 显式实例化模板，避免链接错误
template void __stdcall IkapEventManager::device_event_callback_template<
    IkapEventType::DeviceRemove>(void*, ITKEVENTINFO);
template void __stdcall IkapEventManager::device_event_callback_template<
    IkapEventType::FeatureChanged>(void*, ITKEVENTINFO);
template void __stdcall IkapEventManager::device_event_callback_template<
    IkapEventType::MessageChannel>(void*, ITKEVENTINFO);
