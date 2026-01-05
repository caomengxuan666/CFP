# CFP 客户端设计文档

## 文档概述

本文档详细描述了 CFP 系统中客户端的设计方案，包括用户界面、信号订阅机制以及与图像算法的交互方式。

## 客户端架构

### 整体架构

客户端采用 Model-View-Delegate 模式设计，主要包含以下组件：

- **DvpMainWindow**: 主窗口，负责界面布局和导航控制
- **DvpCameraView**: 相机视图，展示图像预览和控制面板
- **DvpCameraModel**: 数据模型，管理视觉参数和相机配置
- **DvpCameraDelegate**: 委托组件，处理参数编辑逻辑
- **DvpConfigModel**: 配置模型，管理算法参数

### UI 组件设计

```
DvpMainWindow
├── 导航栏 (Navigation)
├── 侧边栏 (Sidebar)
├── 主内容区 (Main Content Area)
    ├── DvpCameraView
    │   ├── 图像预览区 (Preview Area)
    │   └── 控制面板 (Control Panel)
    │       ├── 视觉参数设置 (Vision Parameters)
    │       ├── 相机选择 (Camera Selection)
    │       └── 算法配置 (Algorithm Configuration)
```

## 信号订阅机制

### ImageSignalBus 信号总线

客户端通过 [ImageSignalBus](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L20-L88) 订阅图像处理结果，实现 UI 与算法的解耦。

#### 订阅接口

```cpp
class ImageSignalBus {
 public:
  using ImageCallback = std::function<void(const cv::Mat&)>;

  // 单例模式获取实例
  static ImageSignalBus& instance();

  // 订阅特定信号
  void subscribe(const std::string& signal_name, ImageCallback callback);
};
```

### 客户端订阅流程

1. **信号声明**: 算法类在 [get_signal_info()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L66-L66) 中声明可提供的信号源
2. **信号订阅**: 客户端在 UI 组件中订阅特定信号
3. **图像显示**: 通过回调函数接收图像并更新 UI

### 订阅示例

```
// 在客户端组件中订阅图像信号
auto& signalBus = ImageSignalBus::instance();
signalBus.subscribe("processed_frame", [this](const cv::Mat& img) {
    // 将 OpenCV Mat 转换为 Qt 图像格式
    QImage qimg = convertOpenCVMatToQImage(img);
    
    // 更新 UI 组件
    updatePreviewWidget(qimg);
});
```

## 算法信号处理

### 信号源管理

每个算法类通过 [get_signal_info()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L66-L66) 方法声明可提供的信号源：

```cpp
std::vector<AlgoSignalInfo> get_signal_info() const override {
    return {
        {"original_frame", "原始图像"},
        {"processed_frame", "处理后图像"},
        {"binary_frame", "二值化图像"},
        {"hole_count", "检测到的孔洞数量"}
    };
}
```

### 信号发布

算法内部通过 [emit_image()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L95-L101) 方法发布处理结果：

```cpp
// 在算法处理流程中发布图像
void HoleDetection::process(const CapturedFrame& frame) {
    // ... 图像处理逻辑 ...
    
    // 发布处理后的图像
    emit_image("processed_frame", processed_mat);
    emit_image("binary_frame", binary_mat);
    
    // 发布统计信息
    emit_signal("hole_count", hole_data.size());
}
```

## 客户端与算法交互

### 信号源选择机制

客户端支持动态选择要显示的信号源：

1. **获取可用信号源**: 从算法实例获取所有可用的信号源
2. **用户选择**: 提供 UI 界面让用户选择要订阅的信号源
3. **动态订阅**: 根据用户选择订阅相应的信号

### 多相机信号管理

在多相机场景下，客户端需要管理来自不同相机的信号：

```cpp
class SignalSourceManager {
public:
    // 添加相机信号源
    void addCameraSignalSource(const std::string& cameraId, 
                              const std::vector<AlgoSignalInfo>& signals);
    
    // 订阅特定相机的特定信号
    void subscribeCameraSignal(const std::string& cameraId,
                              const std::string& signalName,
                              ImageSignalBus::ImageCallback callback);
    
    // 获取所有可用信号源
    std::vector<std::pair<std::string, std::string>> getAllSignalSources();
};
```

## UI 实现细节

### 图像预览区域

- **图像显示**: 使用 QLabel 或自定义控件显示图像
- **格式转换**: OpenCV Mat → QImage → QPixmap
- **缩放适配**: 根据控件大小自动缩放图像
- **实时更新**: 高效更新图像内容，避免界面卡顿

### 控制面板

- **参数配置**: 提供算法参数的实时配置界面
- **信号源选择**: 下拉菜单选择要显示的信号源
- **相机选择**: 选择当前显示的相机

### 预览区实现

```cpp
class ImagePreviewWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit ImagePreviewWidget(QWidget* parent = nullptr);
    
public slots:
    void updateImage(const QImage& image);
    
private:
    void paintEvent(QPaintEvent* event) override;
    
private:
    QImage currentImage_;
    bool hasImage_ = false;
};
```

## 线程安全考虑

### UI 更新线程

- 所有 UI 更新必须在主线程中执行
- 使用 Qt 的信号槽机制跨线程安全更新 UI
- OpenCV 处理在工作线程，结果通过信号传递到主线程

### 信号总线线程安全

- [ImageSignalBus](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L20-L88) 内部使用线程安全的容器和锁机制
- 支持多线程并发访问
- 图像数据自动深拷贝，确保生命周期安全

## 扩展性设计

### 动态算法加载

- 支持运行时加载新的算法库
- 算法信号源自动注册到信号总线
- UI 自动更新可用信号源列表

### 自定义信号源

- 支持算法自定义信号源类型
- 可扩展不同类型的数据传输（如特征数据、状态信息）
- 保持接口的一致性

## 错误处理

### 订阅失败处理

- 检查信号源是否存在
- 处理无效的图像数据
- 提供错误反馈机制

### 图像显示错误

- 验证图像数据的有效性
- 提供默认显示内容
- 记录错误日志

## 性能优化

### 图像传输优化

- 使用深拷贝确保线程安全
- 限制图像更新频率避免界面卡顿
- 按需订阅，减少不必要的信号传输

### 内存管理

- 合理管理图像内存使用
- 及时释放不再使用的图像数据
- 避免内存泄漏

## 客户端日志与崩溃处理

### 日志记录机制

客户端集成了统一的日志记录机制，支持多级日志输出：

- **日志级别**: 支持TRACE、DEBUG、INFO、WARN、ERROR、CRITICAL等多级日志
- **输出目标**: 日志可同时输出到控制台、文件和网络（TCP/UDP）
- **异步记录**: 使用spdlog异步日志系统，确保高性能
- **配置化管理**: 通过配置文件动态控制日志级别和输出目标

#### 日志使用示例

```
// 在客户端组件中使用日志
#include "logging/CaponLogging.hpp"

void DvpMainWindow::onCameraConnected() {
    LOG_INFO("相机连接成功: {}", cameraId);
    
    if (someWarningCondition) {
        LOG_WARN("相机 {} 存在警告状态", cameraId);
    }
}

void DvpCameraView::onImageProcessingError() {
    LOG_ERROR("图像处理失败: {}", errorMessage);
}
```

### 崩溃处理集成

客户端集成了崩溃处理机制，确保在发生异常时能够：

- 捕获程序崩溃事件
- 生成崩溃报告
- 上传崩溃信息到监控服务器
- 安全退出程序

#### 崩溃处理初始化

```
// 在客户端启动时初始化崩溃处理
#include "debug/CrashHandler.hpp"

int main(int argc, char *argv[]) {
    // 初始化崩溃处理系统
    CrashHandler::initialize();
    
    // ... 其他初始化代码 ...
    
    // 应用退出前清理
    atexit(CrashHandler::cleanup);
    
    // ... 应用程序逻辑 ...
}
```

### 崩溃信息上报

在客户端运行过程中，除了自动崩溃捕获外，还支持手动上报致命错误：

```
void DvpCameraManager::onCriticalError(const std::string& errorMsg) {
    // 记录错误日志
    LOG_CRITICAL("严重错误: {}", errorMsg);
    
    // 上报致命错误
    CrashHandler::reportFatal(errorMsg.c_str());
}
```

### 日志配置

客户端支持通过配置文件管理日志行为：

``ini
[logging]
tcp_send_enabled=false
udp_send_enabled=false
ipc_send_enabled=true
ipc_protocol=udp
tcp_server_ip=127.0.0.1
tcp_server_port=8080
udp_server_ip=127.0.0.1
udp_server_port=8080
ipc_server_ip=127.0.0.1
ipc_server_port=5141
network_level=err
```

### 安全崩溃处理

客户端遵循工业级崩溃处理原则：

1. **最小安全路径**: 在崩溃时使用最小安全路径进行信息上报
2. **避免二次崩溃**: 不在崩溃处理中使用可能引发二次崩溃的库函数
3. **快速退出**: 生成崩溃信息后快速安全退出进程
4. **信息完整**: 收集完整的崩溃信息（进程ID、线程ID、异常代码、崩溃地址等）
