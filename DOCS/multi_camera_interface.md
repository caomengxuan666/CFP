# CFP 多相机接口设计文档

## 文档概述

本文档详细描述了 CFP 系统中多相机接口的设计方案。虽然当前系统运行在单机单相机模式下，但已预留了完整的多相机接口，为未来扩展做好准备。

## 设计背景

当前系统采用单机单相机模式，即一台电脑控制一个相机，满足大多数现场部署需求。但在某些场景下，可能需要多相机协同工作以提高检测效率或覆盖更大区域。为此，系统已实现多相机接口框架，可在需要时快速启用。

## 接口设计

### 1. MultiCameraCoordinator

#### 职责

- 协调多个相机的工作流程
- 管理相机间的同步和通信
- 处理多相机数据融合

#### 接口定义

```cpp
class MultiCameraCoordinator {
public:
    // 添加相机到协调器
    virtual bool addCamera(std::shared_ptr<DvpCameraCapture> camera) = 0;
    
    // 移除相机
    virtual bool removeCamera(const std::string& cameraId) = 0;
    
    // 开始所有相机捕获
    virtual bool startAllCameras() = 0;
    
    // 停止所有相机捕获
    virtual bool stopAllCameras() = 0;
    
    // 获取所有相机状态
    virtual std::vector<CameraStatus> getAllCameraStatus() = 0;
    
    // 设置相机同步模式
    virtual void setSyncMode(SyncMode mode) = 0;
    
    // 获取融合处理器
    virtual std::shared_ptr<FrameProcessor> getFusionProcessor() = 0;
};
```

#### 同步模式

- `FreeRun`: 相机自由运行，无同步
- `HardwareTrigger`: 硬件触发同步
- `SoftwareTrigger`: 软件触发同步

### 2. FrameProcessor

#### 职责

- 处理来自多个相机的图像帧
- 实现图像拼接、去重、对齐
- 执行算法检测

#### 接口定义

```cpp
class FrameProcessor {
public:
    // 处理单个相机帧
    virtual ProcessResult processSingleCameraFrame(
        const CapturedFrame& frame, 
        const std::string& cameraId) = 0;
    
    // 处理多相机融合帧
    virtual ProcessResult processMultiCameraFrame(
        const std::vector<CapturedFrame>& frames) = 0;
    
    // 设置融合策略
    virtual void setFusionStrategy(FusionStrategy strategy) = 0;
    
    // 获取处理统计信息
    virtual ProcessingStats getStats() const = 0;
};
```

### 3. CameraManager

#### 职责

- 管理多个相机实例
- 处理相机配置和状态

#### 接口定义

```cpp
class CameraManager {
public:
    // 初始化多相机环境
    virtual bool initializeMultiCameraEnvironment() = 0;
    
    // 获取可用相机列表
    virtual std::vector<CameraInfo> getAvailableCameras() = 0;
    
    // 根据配置创建多个相机
    virtual std::vector<std::shared_ptr<DvpCameraCapture>> 
        createCameras(const std::vector<CameraConfig>& configs) = 0;
    
    // 配置相机同步参数
    virtual bool configureSyncParameters(
        const SyncParameters& params) = 0;
};
```

## 实现现状

### 已实现组件

#### MultiCameraCoordinator 实现

- [MultiCameraCoordinator](file:///d:/codespace/CFP/include/MultiCameraCoordinator.hpp#L1-L33) 类已定义基本接口
- 支持添加和管理多个相机实例
- 提供基本的同步机制

#### FrameProcessor 扩展

- [FrameProcessor](file:///d:/codespace/CFP/include/cameras/FrameProcessor.hpp#L1-L138) 支持多相机帧处理
- 定义了 `MultiCameraFusionProcessor` 扩展类
- 预留图像拼接、去重、对齐接口

#### Camera 管理增强

- [CameraManager](file:///d:/codespace/CFP/include/cameras/CameraManager.hpp#L1-L21) 支持多相机管理
- [CameraFactory](file:///d:/codespace/CFP/include/cameras/CameraFactory.hpp#L1-L16) 支持不同类型相机创建

### 待完善功能

#### 图像融合策略

```cpp
// TODO: 实现图像拼接、去重、对齐
// 在MultiCameraFusionProcessor中实现图像融合功能
// 目前来说的话暂时不用设计融合策略，直接使用单相机
// 未来即使使用融合策略，我们也是通过设置ROI的方式来达成这个目的
```

#### 相机元信息管理

```cpp
// TODO: 未来需要用union来存储来自不同相机的元信息
// 改进CapturedFrame结构以支持不同相机的元信息
```

## 配置管理

### CameraConfig 扩展

- 当前 [CameraConfig](file:///d:/codespace/CFP/include/config/CameraConfig.hpp#L1-L28) 已预留多相机配置字段
- 支持为每个相机配置不同的 ROI、曝光等参数
- 配置同步机制确保多相机参数一致性

### 同步参数配置

- 触发信号配置
- 时序同步参数
- 数据采集同步设置

## 通信机制

### 本地通信

在多相机模式下，所有相机运行在同一主机上，通过以下方式进行通信：

1. **共享内存**: 用于高效传输图像数据
2. **队列机制**: 用于异步处理图像帧
3. **事件总线**: 用于状态同步和控制信号传输

### Redis 移除

- 当切换到多相机模式时，不再需要 Redis 进行跨主机同步
- 所有通信在本地完成，提高响应速度和可靠性

## 启用多相机模式

### 步骤

1. 修改构建配置启用多相机支持
2. 配置 [MultiCameraCoordinator](file:///d:/codespace/CFP/include/MultiCameraCoordinator.hpp#L1-L33) 实例
3. 初始化多个相机实例
4. 配置同步参数
5. 启动多相机捕获

### 配置示例

```cpp
// 创建相机管理器
auto cameraManager = std::make_shared<CameraManager>();

// 获取可用相机
auto availableCameras = cameraManager->getAvailableCameras();

// 创建多相机配置
std::vector<CameraConfig> configs;
for (const auto& camInfo : availableCameras) {
    configs.push_back(createCameraConfig(camInfo));
}

// 创建相机实例
auto cameras = cameraManager->createCameras(configs);

// 创建协调器
auto coordinator = std::make_shared<MultiCameraCoordinator>();
for (auto& camera : cameras) {
    coordinator->addCamera(camera);
}

// 配置同步参数
SyncParameters syncParams;
syncParams.mode = SyncMode::SoftwareTrigger;
cameraManager->configureSyncParameters(syncParams);

// 开始捕获
coordinator->startAllCameras();
```

## 性能考虑

### 并发处理

- 使用线程池处理多个相机的图像数据
- 异步处理算法检测，避免阻塞图像捕获

### 内存管理

- 复用图像缓冲区，减少内存分配
- 使用智能指针管理相机生命周期

### 负载均衡

- 根据相机数量动态调整处理线程数
- 平衡 CPU 和 GPU 负载

## 扩展性

### 相机类型支持

- 通过 [CameraFactory](file:///d:/codespace/CFP/include/cameras/CameraFactory.hpp#L1-L16) 支持不同类型相机
- 插件化相机驱动，易于扩展

### 算法扩展

- 支持为不同相机应用不同算法
- 可配置的算法参数

## 测试策略

### 单元测试

- 测试多相机协调逻辑
- 验证图像融合算法

### 集成测试

- 模拟多相机环境
- 验证同步机制

### 性能测试

- 测试多相机并发处理能力
- 验证资源使用效率
