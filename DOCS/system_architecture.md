# CFP 系统架构设计文档

## 文档概述

本文档详细描述了 CFP 系统的整体架构设计，包括模块划分、组件关系、数据流向和关键接口。文档旨在为开发者提供系统架构的全面理解，以便进行开发、维护和扩展。

## 系统架构概览

CFP 是一个基于 C++ 和 Qt 的工业视觉检测应用，专注于 DVP/IK/MIND 相机控制、图像采集与处理。系统采用分层架构设计，具有良好的模块化和可扩展性。

```plain text
┌─────────────────────────────────────────────────────────────────────────────┐
│                         CFP Architecture                              │
├─────────────────────────────────────────────────────────────────────────────┤
│  Client Layer (UI)                                                          │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  DvpMainWindow  DvpCameraView  DvpConfigModel  DvpCameraManager         ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Communication & Protocol Layer                                             │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  ProtocolSession  AsioTcpTransport  LegacyCodec  TransportAdapter       ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Redis Communication Layer                                                  │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  IRedisClient  RedisClient  DistributedStateSync                       ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Multi-Camera Coordination Layer                                            │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  MultiCameraCoordinator  FrameProcessor  CapturedFrame                  ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Application Layer                                                          │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  DvpCameraBuilder  DvpCameraCapture  DvpEventManager  FrameProcessor    ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Camera Management Layer                                                    │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  CameraFactory  CameraManager  CameraCapture  DvpCameraManager          ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Configuration Layer                                                        │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  DvpConfig  ConfigManager  CameraConfig  AlogoParams                     ││
│  └─────────────────────────────────────────────────────────────────────────┘│
├─────────────────────────────────────────────────────────────────────────────┤
│  Algorithm Layer                                                            │
│  ┌─────────────────────────────────────────────────────────────────────────┐│
│  │  AlgoBase  GenericAlgorithmConfigObserver  HoleDetection                 ││
│  └─────────────────────────────────────────────────────────────────────────┘│
```

## 双服务器冗余架构设计

### 1. 架构概述

为了满足生产环境的高可用性和调试需求，系统实现了双服务器冗余架构：

- **主服务器**: 用于生产环境，必须稳定、低延迟
- **副服务器**: 用于调试（如大模型分析），不能影响主服务器的性能
- **双协议会话**: 主服务器和副服务器分别维护独立的协议会话
- **独立网络堆栈**: 主服务器和副服务器使用独立的 [asio::io_context](file:///d:/codespace/CFP/third_party/asio/include/asio/io_context.hpp#L61-L64) 确保故障隔离

### 2. BusinessManager 中的双会话管理

在 [BusinessManager](file:///d:/codespace/CFP/include/business/BusinessManager.hpp#L32-L114) 类中，实现了主服务器和副服务器的会话管理：

```
// 主协议会话
std::shared_ptr<protocol::ProtocolSession> report_session_;     // 19300 主服务器
std::shared_ptr<protocol::ProtocolSession> telemetry_session_;  // 19700 主服务器

// 备份协议会话
std::shared_ptr<protocol::ProtocolSession> backup_report_session_;     // 19300 备份服务器
std::shared_ptr<protocol::ProtocolSession> backup_telemetry_session_;  // 19700 备份服务器
```

### 3. 数据发送策略

- **主服务器**: 必须成功，失败需重试/告警
- **副服务器**: 尽力而为，失败可忽略，绝不阻塞主流程
- **并行发送**: 数据同时异步发送到两个服务器，避免串行阻塞影响实时性

### 4. 独立的 I/O 上下文

- **main_io_ctx_**: 用于主服务器（生产环境）
- **backup_io_ctx_**: 用于副服务器（调试环境）
- 彻底隔离网络堆栈，避免副服务器影响主线程

## 模块详细设计

### 1. 客户端层 (Client Layer)

#### 1.1 DvpMainWindow

- **职责**: 主窗口管理，协调各个 UI 组件
- **主要功能**:
  - 界面布局管理
  - 页面导航控制
  - 相机管理接口调用
- **依赖**: [DvpCameraView](file:///d:/codespace/CFP/src/client/DvpCameraView.h#L10-L34), [DvpConfigModel](file:///d:/codespace/CFP/src/client/DvpConfigModel.h#L14-L29)

#### 1.2 DvpCameraView

- **职责**: 相机视图显示，包括视频预览和图像处理结果展示
- **主要功能**:
  - 视频帧显示
  - 处理结果可视化
  - 用户交互响应
- **依赖**: [ImageSignalBus](file:///d:/codespace/CFP/include/cameras/ImageSignalBus.hpp#L1-L45)

#### 1.3 DvpConfigModel

- **职责**: 配置数据模型，管理相机和算法参数
- **主要功能**:
  - 参数存储和更新
  - 配置验证
  - 参数持久化
- **依赖**: [AlogoParams](file:///d:/codespace/CFP/include/config/AlogoParams.hpp#L1-L15), [CameraConfig](file:///d:/codespace/CFP/include/config/CameraConfig.hpp#L1-L28)

### 2. 通信协议层 (Communication & Protocol Layer)

#### 2.1 ProtocolSession

- **职责**: 管理单个通信会话的生命周期
- **主要功能**:
  - 会话建立和销毁
  - 数据收发处理
  - 协议编解码
- **依赖**: [AsioTcpTransport](file:///d:/codespace/CFP/include/protocol/AsioTcpTransport.hpp#L1-L35), [LegacyCodec](file:///d:/codespace/CFP/include/protocol/LegacyCodec.hpp#L1-L26)

#### 2.2 AsioTcpTransport

- **职责**: 基于 ASIO 库的 TCP 传输实现
- **主要功能**:
  - TCP 连接管理
  - 数据传输
  - 错误处理
- **依赖**: ASIO 库

#### 2.3 LegacyCodec

- **职责**: 旧版协议编解码器
- **主要功能**:
  - 数据包编码
  - 数据包解码
  - 协议兼容性处理

### 3. Redis 通信层 (Redis Communication Layer)

#### 3.1 IRedisClient

- **职责**: Redis 客户端接口定义
- **主要功能**:
  - 定义 Redis 操作接口
  - 支持依赖注入和单元测试
- **依赖**: cpp_redis 库

#### 3.2 RedisClient

- **职责**: Redis 客户端具体实现
- **主要功能**:
  - Redis 连接管理
  - PUB/SUB 操作
  - 键值存储操作

### 4. 多相机协调层 (Multi-Camera Coordination Layer)

#### 4.1 MultiCameraCoordinator

- **职责**: 协调多个相机的工作流程
- **主要功能**:
  - 相机调度
  - 数据融合
  - 负载均衡
- **依赖**: [FrameProcessor](file:///d:/codespace/CFP/include/cameras/FrameProcessor.hpp#L1-L138)

#### 4.2 FrameProcessor

- **职责**: 图像帧处理
- **主要功能**:
  - 图像预处理
  - 算法执行
  - 结果输出
- **依赖**: [HoleDetection](file:///d:/codespace/CFP/include/algo/HoleDetection.hpp#L1-L28)

### 5. 应用层 (Application Layer)

#### 5.1 DvpCameraBuilder

- **职责**: 构建和配置 DVP 相机实例
- **主要功能**:
  - 相机参数配置
  - 相机实例创建
  - 相机功能初始化
- **依赖**: [DvpConfig](file:///d:/codespace/CFP/include/cameras/Dvp/DvpConfig.hpp#L1-L35)

#### 5.2 DvpCameraCapture

- **职责**: 相机图像采集
- **主要功能**:
  - 图像捕获
  - 图像队列管理
  - 捕获状态监控
- **依赖**: [DvpCameraBuilder](file:///d:/codespace/CFP/include/cameras/Dvp/DvpCameraBuilder.hpp#L1-L25)

#### 5.3 DvpEventManager

- **职责**: 事件管理和分发
- **主要功能**:
  - 事件监听
  - 事件分发
  - 事件处理
- **依赖**: [ImageSignalBus](file:///d:/codespace/CFP/include/cameras/ImageSignalBus.hpp#L1-L45)

### 6. 相机管理层 (Camera Management Layer)

#### 6.1 CameraFactory

- **职责**: 相机实例工厂
- **主要功能**:
  - 不同类型相机的创建
  - 相机类型注册
  - 相机实例管理
- **依赖**: [CameraManager](file:///d:/codespace/CFP/include/cameras/CameraManager.hpp#L1-L21)

#### 6.2 CameraManager

- **职责**: 相机管理器
- **主要功能**:
  - 相机生命周期管理
  - 相机配置保存/加载
  - 相机状态监控
- **依赖**: [CameraConfig](file:///d:/codespace/CFP/include/config/CameraConfig.hpp#L1-L28)

### 7. 配置层 (Configuration Layer)

#### 7.1 DvpConfig

- **职责**: 系统配置管理
- **主要功能**:
  - 系统参数存储
  - 配置加载/保存
  - 配置变更通知
- **依赖**: [ConfigManager](file:///d:/codespace/CFP/include/config/ConfigManager.hpp#L1-L16)

#### 7.2 ConfigManager

- **职责**: 配置管理器
- **主要功能**:
  - 配置文件读写
  - 配置验证
  - 配置变更事件
- **依赖**: [ConfigObserver](file:///d:/codespace/CFP/include/config/ConfigObserver.hpp#L1-L12)

### 8. 算法层 (Algorithm Layer)

#### 8.1 HoleDetection

- **职责**: 孔洞检测算法
- **主要功能**:
  - 图像处理
  - 特征提取
  - 孔洞识别
- **依赖**: [AlgoBase](file:///d:/codespace/CFP/include/algo/AlgoBase.hpp#L1-L16), OpenCV

## 数据流向

### 图像数据流向

```
物理相机 → [DvpCameraCapture](file:///d:/codespace/CFP/include/cameras/Dvp/DvpCameraCapture.hpp#L1-L58) → [ImageSignalBus](file:///d:/codespace/CFP/include/cameras/ImageSignalBus.hpp#L1-L45) → [FrameProcessor](file:///d:/codespace/CFP/include/cameras/FrameProcessor.hpp#L1-L138) → [HoleDetection](file:///d:/codespace/CFP/include/algo/HoleDetection.hpp#L1-L28) → [DvpCameraView](file:///d:/codespace/CFP/src/client/DvpCameraView.h#L10-L34)
```

### 控制信号流向

``PlainText
[DvpMainWindow](file:///d:/codespace/CFP/src/client/DvpMainWindow.h#L15-L42) → [DvpCameraManager](file:///d:/codespace/CFP/include/DvpCameraManager.hpp#L1-L21) → [DvpCameraCapture](file:///d:/codespace/CFP/include/cameras/Dvp/DvpCameraCapture.hpp#L1-L58) → 物理相机

```

### 配置数据流向

``PlainText
[DvpConfigModel](file:///d:/codespace/CFP/src/client/DvpConfigModel.h#L14-L29) → [DvpConfig](file:///d:/codespace/CFP/include/cameras/Dvp/DvpConfig.hpp#L1-L35) → [DvpCameraBuilder](file:///d:/codespace/CFP/include/cameras/Dvp/DvpCameraBuilder.hpp#L1-L25) → 物理相机
```

## 关键接口

### 相机接口

- `ICamera`: 定义相机的基本操作接口
- `ICameraEvent`: 定义相机事件接口

### 通信接口

- `ITransport`: 定义传输层接口
- `IMessageCodec`: 定义消息编解码接口

### 算法接口

- `IAlgorithm`: 定义算法执行接口
- `IAlgorithmConfig`: 定义算法配置接口

## 扩展性设计

### 多相机支持

- 通过 [MultiCameraCoordinator](file:///d:/codespace/CFP/include/MultiCameraCoordinator.hpp#L1-L33) 实现多相机协调
- 通过 [CameraFactory](file:///d:/codespace/CFP/include/cameras/CameraFactory.hpp#L1-L16) 支持不同类型相机的扩展

### 算法扩展

- 通过算法接口实现算法的插件化
- 通过配置系统实现算法参数的动态调整

### 通信协议扩展

- 通过协议接口实现不同通信协议的插拔
- 支持 TCP、UDP、Redis 等多种通信方式

## 性能考虑

### 内存管理

- 使用智能指针管理对象生命周期
- 通过对象池减少频繁分配/释放

### 并发处理

- 使用线程池处理图像处理任务
- 通过队列解耦生产和消费过程

### 异步操作

- 异步处理 I/O 操作
- 非阻塞通信实现

## 安全性考虑

### 输入验证

- 对所有外部输入进行验证
- 防止缓冲区溢出

### 访问控制

- 实现权限管理
- 数据访问审计

## 测试策略

### 单元测试

- 对每个模块进行独立测试
- 使用 Mock 对象模拟依赖

### 集成测试

- 测试模块间接口
- 验证数据流向正确性

### 系统测试

- 验证整体功能
- 性能和压力测试
