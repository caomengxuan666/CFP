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
 *  - File: IkapEventManager.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "IKapCDef.h"

enum class IkapEventType {
  // ------ 设备事件回调 ------ //
  DeviceRemove,    // <设备回调> 掉线的回调事件
  FeatureChanged,  // <设备回调> 设备参数变化的回调事件
  MessageChannel,  // <设备回调> 消息通道回调事件

  // ------ 流事件回调 ------  //

  FrameAvailable,         // <流回调> 帧采集完成(帧可用)回调事件
  StreamStarted,          // <流回调> 流开始采集回调事件
  StreamStopped,          // <流回调> 流停止采集回调事件
  StreamRecovered,        // <流回调> 被终止的流恢复回调事件
  StreamRecoveredFailed,  // <流回调> 被终止的流恢复失败回调事件
  StreamTimeOut,          // <流回调> 帧超时回调事件
  StreamFrameLost,        // <流回调> 帧丢失回调事件
  ImageDataError,         // <流回调> 图像信息错误回调事件
  DeviceError,            // <流回调> 设备出错流终止回调事件
  EndLine,                // <流回调> n行数据采集完成可用回调事件
  EndBatch,               // <流回调> 批量采集完成可用回调事件
};

struct IkapEventContext {
  ITKDEVICE device_handle;
  IkapEventType type;
  ITKEVENTINFO event_info;     // 设备事件使用
  ITKSTREAM stream_handle;     // 流事件使用
  uint32_t stream_event_type;  // SDK流事件类型
};

using IkapEventHandler = std::function<void(const IkapEventContext&)>;

class IkapEventManager {
 public:
  explicit IkapEventManager(ITKDEVICE handle);
  ~IkapEventManager();

  void register_handler(IkapEventType event, const IkapEventHandler& handler);

  using StreamEventCallbackType = void(__stdcall*)(uint32_t, void*);  // NOLINT
  StreamEventCallbackType get_stream_event_callback() const;

  const std::unordered_map<uint16_t, IkapEventType>& get_stream_event_map()
      const;

 private:
  // 模板化设备事件回调
  template <IkapEventType EventType>
  static void __stdcall device_event_callback_template(void* context,
                                                       ITKEVENTINFO event_info);

  static void __stdcall stream_event_callback(uint32_t event_type,
                                              void* context);

  ITKDEVICE handle_;
  std::unordered_map<IkapEventType, IkapEventHandler> handlers_;

  const std::unordered_map<uint16_t, IkapEventType> stream_event_map_ = {
      {ITKSTREAM_VAL_EVENT_TYPE_START_OF_STREAM, IkapEventType::StreamStarted},
      {ITKSTREAM_VAL_EVENT_TYPE_END_OF_STREAM, IkapEventType::StreamStopped},
      {ITKSTREAM_VAL_EVENT_TYPE_END_OF_FRAME, IkapEventType::FrameAvailable},
      {ITKSTREAM_VAL_EVENT_TYPE_TIME_OUT, IkapEventType::StreamTimeOut},
      {ITKSTREAM_VAL_EVENT_TYPE_FRAME_LOST, IkapEventType::StreamFrameLost},
      {ITKSTREAM_VAL_EVENT_TYPE_IMAGE_DATA_ERROR,
       IkapEventType::ImageDataError},
      {ITKSTREAM_VAL_EVENT_TYPE_DEVICE_ERROR, IkapEventType::DeviceError},
      {ITKSTREAM_VAL_EVENT_TYPE_END_OF_BATCH, IkapEventType::EndBatch},
      {ITKSTREAM_VAL_EVENT_TYPE_END_OF_LINE, IkapEventType::EndLine},
      {ITKSTREAM_VAL_EVENT_TYPE_STREAM_RECOVERED,
       IkapEventType::StreamRecovered},
      {ITKSTREAM_VAL_EVENT_TYPE_STREAM_RECOVER_FAILED,
       IkapEventType::StreamRecoveredFailed},
  };

  const std::unordered_map<uint16_t, IkapEventType> device_event_map_ = {
      {ITKDEV_VAL_EVENT_TYPE_DEV_REMOVE, IkapEventType::DeviceRemove},
      {ITKDEV_VAL_EVENT_TYPE_FEATURE_VALUE_CHANGED,
       IkapEventType::FeatureChanged},
      {ITKDEV_VAL_EVENT_TYPE_MESSAGE_CHANNEL, IkapEventType::MessageChannel}};
};
