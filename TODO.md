# Redis PUB/SUB 通信架构实施计划

## 项目概述

基于Redis PUB/SUB协议构建分布式前端机通信架构，实现主前端机（101）与其他前端机之间的协调通信。101号前端机作为主控节点，负责发送控制信号和聚合遥测数据，其他前端机作为从属节点，接收控制信号并发送遥测数据。

## 待办事项列表

## 功能开发

- [x] 实现完整的IK和MIND的SDK的封装
- [x] 实现基础相机控制与图像采集
- [x] 完成孔洞检测算法核心逻辑
- [x] 构建 GUI 主窗口与相机视图组件
- [x] 实现配置持久化与动态刷新机制
- [x] 完成多相机接口框架（已就绪，等待启用）

## Redis 通信架构

- [ ] 实现 Redis 客户端连接管理（IRedisClient 接口）
- [ ] 完成 Redis PUB/SUB 通信机制
- [ ] 设计并实现分布式状态同步协议
- [ ] 集成 Redis 与 TCP 通信的协调机制
- [ ] 实现故障恢复与重连机制

## 动态算法库

- [ ] 定义统一的算法接口规范
- [ ] 实现动态库加载与卸载机制
- [ ] 设计算法元数据管理系统
- [ ] 实现算法版本控制与验证
- [ ] 创建算法插件开发文档
- [ ] 实现远程算法推送功能

## 文档完善

- [ ] 编写详细的通信协议文档（参考 `README` 中表格）
- [ ] 创建 `DOCS/communication_protocol.md` 描述各端口用途
- [ ] 补充算法流程图与状态机说明
- [ ] 更新 API 接口文档（如有）
- [ ] 编写动态算法库使用指南

## 架构优化

- [ ] 重构事件总线以支持多相机并发
- [ ] 增加日志分级与性能监控模块
- [ ] 实现相机状态自动上报与异常告警

## 测试与验证

- [ ] 编写多相机模拟测试用例（Mock）
- [ ] 验证高帧率下的图像处理延迟
- [ ] 增加压力测试脚本（模拟多相机同时工作）

## 其他

- [ ] 将部分复杂逻辑拆分为独立模块（如 `FrameProcessor`, `AlgoEngine`）
- [ ] 添加 CI/CD 流程支持（GitHub Actions 或 Jenkins）

## 现有项目待办事项

以下是需要完成的现有待办事项：

### 1. 相机配置相关

- [ ] **[include/config/CameraConfig.hpp]** - 文件开头注释提到："TODO 由于我还没有看具体的需求，先复制了一个DvpConfig,实际不会有这么多共同参数"，需要根据实际需求精简配置参数

- [ ] **[src/DvpCameraCapture.cpp]** - 第322行："TODO(cmx) 这个一定要重新写,只是因为现在还没确定好CameraConfig的通用范围。"，需要重新实现相机配置的通用范围。当且仅当我们需要使用多相机的时候
才需要使用这个CameraConfig，目前优先级没那么高。

- [ ] **[src/cameras/CameraManager.cpp]** - 第69行："TODO: 实现相机配置保存逻辑"，需要实现相机配置的保存功能

- [ ] **[src/cameras/CameraManager.cpp]** - 第82行："TODO: 实现相机配置加载逻辑"，需要实现相机配置的加载功能

### 2. 图像处理相关

- [ ] **[include/FrameProcessor.hpp]** - 第42行："TODO 未来需要用union来存储来自不同相机的元信息"，需要改进CapturedFrame结构以支持不同相机的元信息

- [ ] **[src/FrameProcessor.hpp]** - 第137行："TODO: 实现图像拼接、去重、对齐"，在MultiCameraFusionProcessor中实现图像融合功能。目前来说的话暂时不用设计融合策略，直接使用单相机。未来即使使用
融合策略，我们也是通过设置ROI的方式来达成这个目的。

- [ ] **[src/example.cpp]** - 第296行："TODO(cmx): 根据需求的变化，前端机只负责采集一个相机图像并且处理"，需要根据需求调整前端机的相机处理逻辑

- [ ] **[src/example.cpp]** - 第284行："TODO: 实现拼接、去重、对齐"，在融合策略中实现图像处理功能

### 3. 算法实现相关

- [ ] **[src/algos/HoleDetection.cpp]** - 第989行："TODO(cmx) 在此处发送特征数据"，需要实现特征数据的发送功能

### 4. 客户端界面相关

- [ ] **[src/client/DvpCameraView.cpp]** - 第98行："TODO 我们需要在这里实现真正的视频预览区域，从队列中获取图像"，需要实现视频预览功能

- [ ] **[src/client/DvpCameraView.cpp]** - 第123行："TODO: 实现相机选择变更处理逻辑"，需要实现相机选择变更的处理

- [ ] **[src/client/DvpMainWindow.cpp]** - 第667行："TODO: 实现导航按钮点击逻辑"，需要实现导航按钮功能

- [ ] **[src/client/DvpMainWindow.cpp]** - 第671行："TODO: 实现页面索引改变逻辑"，需要实现页面索引改变功能

- [ ] **[src/client/DvpMainWindow.cpp]** - 第675行："TODO: 实现侧边栏调整大小逻辑"，需要实现侧边栏调整功能

- [ ] **[src/client/DvpMainWindow.cpp]** - 第679行："TODO: 实现断开相机连接逻辑"，需要实现断开相机连接功能

- [ ] **[src/client/DvpMainWindow.cpp]** - 第683行："TODO: 实现开始捕获逻辑"，需要实现开始捕获功能

- [ ] **[src/client/DvpMainWindow.cpp]** - 第687行："TODO: 实现停止捕获逻辑"，需要实现停止捕获功能

- [ ] **[src/client/DvpMainWindow.cpp]** - 第757行："TODO: 停止所有相机"，需要在关闭事件中停止所有相机

### 5. 相机工厂相关

- [ ] **[src/cameras/CameraFactory.cpp]** - 第31行："TODO: 实现IK相机的创建逻辑"，需要实现IK相机的创建

- [ ] **[src/cameras/CameraFactory.cpp]** - 第36行："TODO: 实现Mind相机的创建逻辑"，需要实现Mind相机的创建

### 6. 数据处理相关

- [ ] **[src/DvpCameraCapture.cpp]** - 第137行："TODO 检查文件读写状态(示例：如果队列超过200帧了，则标记 file_io 错误)"，需要实现文件读写状态检查

- [ ] **[src/DvpCameraCapture.cpp]** - 第75行："TODO 暂时假设启动成功即自检通过"，需要实现真正的自检功能

## 架构设计

### 通信角色

| 前端机角色 | 职责 |
|------------|------|
| 101 主前端机 | - 监听 7000/19800：接收服务器控制信号<br>- PUBLISH control/start：广播开始信号给其他前端机<br>- SUBSCRIBE telemetry/+：收集所有前端机（包括自己）的遥测数据<br>- 连接 19300：发送自己的特征/状态<br>- 连接 19700：聚合遥测数据后统一上报服务器 |
| 其他前端机 | - SUBSCRIBE control/start：接收 101 的开始信号<br>- PUBLISH telemetry/<id>：发布自己的遥测数据<br>- 连接 19300：发送自己的特征/状态<br>- （不连接 19700） |

### Redis 通道定义

| 通道 | 方向 | 内容 | 示例 |
|------|------|------|------|
| control/start | 101 → 其他 | 开始信号 + 卷号 | `{ "roll_id": "ROLL_001" }` |
| telemetry/101 | 101 → Redis | 101 自身遥测数据 | `{ "width": 1500, "length": 2000, ... }` |
| telemetry/102 | 102 → Redis | 102 遥测数据 | `{ "width": 800, "length": 2000, ... }` |

## 实施步骤

### 第一阶段：Redis 客户端实现

- [ ] **创建 Redis 客户端类**
  - [ ] 创建 [include/redis/RedisClient.hpp](file:///d:/codespace/DvpDetect/include/protocol/messages.hpp#L1-L38) 头文件
  - [ ] 创建 [src/redis/RedisClient.cpp](file:///d:/codespace/DvpDetect/src/protocol/LegacyCodec.cpp#L1-L111) 实现文件
  - [ ] 实现基础连接功能
  - [ ] 实现 PUBLISH 操作
  - [ ] 实现 SUBSCRIBE 操作
  - [ ] 实现 PSUBSCRIBE 操作
  - [ ] 添加错误处理机制

### 第二阶段：通信逻辑实现

- [ ] **实现主前端机逻辑**
  - [ ] 实现控制信号监听（端口 7000/19800）
  - [ ] 实现开始信号广播（`control/start` 通道）
  - [ ] 实现遥测数据聚合（`telemetry/*` 通道）
  - [ ] 实现 19700 端口数据上报功能

- [ ] **实现从属前端机逻辑**
  - [ ] 实现控制信号订阅（`control/start` 通道）
  - [ ] 实现遥测数据发布（`telemetry/<id>` 通道）
  - [ ] 实现 19300 端口数据发送功能

### 第三阶段：节点识别与区分

- [ ] **实现节点识别机制**
  - [ ] 通过机器名识别主节点
  - [ ] 或通过配置文件区分节点角色
  - [ ] 或通过编译宏区分节点角色（推荐）

### 第四阶段：集成与测试

- [ ] **与现有系统集成**
  - [ ] 将 Redis 通信模块集成到现有相机管理系统中
  - [ ] 确保与 [DvpEventManager](file:///d:/codespace/DvpDetect/include/DvpEventManager.hpp#L15-L28) 的兼容性
  - [ ] 保证与图像采集和处理模块的协调

- [ ] **全面测试**
  - [ ] 单元测试 Redis 客户端功能
  - [ ] 集成测试多节点通信
  - [ ] 压力测试遥测数据聚合性能
  - [ ] 故障恢复测试

## 技术选型

- [ ] **Redis 客户端库选择**
  - [ ] 评估 hiredis + 线程方案
  - [ ] 评估 cpp-redis + asio 方案
  - [ ] 确定最终技术方案

- [ ] **线程安全实现**
  - [ ] 确保 Redis 客户端线程安全
  - [ ] 实现适当的锁机制
  - [ ] 处理异步操作的同步问题

## 依赖项

- [ ] **添加 Redis 客户端库到项目**
  - [ ] 在 CMakeLists.txt 中添加 Redis 库依赖
  - [ ] 更新 vcpkg 配置以包含 Redis 客户端库
  - [ ] 确保跨平台兼容性

## 注意事项

- [ ] **确保与现有协议栈兼容**
  - [ ] 与 [ProtocolSession](file:///d:/codespace/DvpDetect/include/protocol/ProtocolSession.hpp#L18-L39) 类的兼容性
  - [ ] 与 [AsioTcpTransport](file:///d:/codespace/DvpDetect/include/protocol/AsioTcpTransport.hpp#L14-L24) 的协调

- [ ] **性能考虑**
  - [ ] 评估 Redis 通信对系统性能的影响
  - [ ] 确保遥测数据传输的实时性
  - [ ] 优化数据序列化/反序列化过程

- [ ] **错误处理**
  - [ ] Redis 连接失败的处理
  - [ ] 网络中断时的恢复机制
  - [ ] 数据格式错误的处理

## 验收标准

- [ ] 主前端机能够正确广播控制信号
- [ ] 从属前端机能够正确接收控制信号
- [ ] 所有前端机能够正确发布遥测数据
- [ ] 主前端机能够正确聚合遥测数据
- [ ] 数据传输满足实时性要求
- [ ] 系统具备适当的错误处理和恢复能力
- [ ] 代码符合项目编码规范
