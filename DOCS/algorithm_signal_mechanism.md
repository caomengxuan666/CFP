# CFP 算法信号机制设计文档

## 文档概述

本文档详细描述了 CFP 系统中算法信号机制的设计方案，包括图像信号总线、算法接口规范、信号订阅与发布机制。

## 设计背景

在 CFP 系统中，图像处理算法需要将处理结果（如中间图像、最终结果、统计数据等）传递给客户端界面或其他组件进行显示或处理。为了解耦算法实现与显示逻辑，系统采用了信号总线机制，实现算法与客户端之间的松耦合通信。

## 核心组件

### 1. ImageSignalBus（图像信号总线）

#### 职责

- 管理信号订阅者
- 分发算法处理结果
- 提供线程安全的信号传输

#### 接口定义

```cpp
class ImageSignalBus {
 public:
  using ImageCallback = std::function<void(const cv::Mat&)>;

  struct FeatureData {
    std::string roll_id;
    std::vector<std::pair<int, float>> features;
    std::array<float, 20> special_images;
  };

  struct StatusData {
    bool self_check = false;
    bool capture = false;
    bool file_io = false;
    bool image_anomaly = false;
  };

  using FeatureCallback = std::function<void(const FeatureData&)>;
  using StatusCallback = std::function<void(const StatusData&)>;

  // 单例模式获取实例
  static ImageSignalBus& instance();

  // 算法调用：声明自己能提供哪些信号
  void declare_signal(const std::string& signal_name);

  // UI 或其他模块调用：订阅某个信号
  void subscribe(const std::string& signal_name, ImageCallback callback);

  // 算法内部调用：广播图像（自动深拷贝）
  void emit(const std::string& signal_name, const cv::Mat& img);
  
  // 与服务器之间通信：订阅特征和状态
  void subscribe_feature(const std::string& name, FeatureCallback cb);
  void subscribe_status(const std::string& name, StatusCallback cb);
  void emit_feature(const std::string& name, const FeatureData& data);
  void emit_status(const std::string& name, const StatusData& data);

 private:
  // 图像信号订阅者
  std::unordered_map<std::string, std::vector<ImageCallback>> subscribers_;
  // 特征数据信号订阅者
  std::unordered_map<std::string, std::vector<FeatureCallback>> feature_subscribers_;
  // 状态数据信号订阅者
  std::unordered_map<std::string, std::vector<StatusCallback>> status_subscribers_;

  mutable std::shared_mutex mutex_;
};
```

### 2. AlgoBase（算法基类）

#### 职责

- 定义算法接口规范
- 管理算法信号源
- 提供信号发布方法

#### 接口定义

```cpp
namespace algo {

struct AlgoParamInfo {
  std::string name;
  std::string type;        // "int", "float", "bool", "string"
  std::string description;
  std::string default_value;
  std::string current_value; // 可选，用于 UI 显示
};

struct AlgoSignalInfo {
  std::string name;
  std::string description;
};

class AlgoBase {
 public:
  virtual ~AlgoBase() = default;

  // 延迟初始化，绑定所有的信号源
  void initialize() {
    auto signals = get_signal_info();
    for (const auto& sig : signals) {
      ImageSignalBus::instance().declare_signal(sig.name);
      declared_signals_.insert(sig.name);
    }
  }

  // 处理一帧图像
  virtual void process(const CapturedFrame& frame) = 0;

  // 动态配置算法参数
  virtual void configure(const std::string& key, const std::string& value);

  // 获取参数值
  virtual std::string get_param(const std::string& key) const;

  // 获取算法名称和描述
  virtual std::string get_name() const = 0;
  virtual std::string get_description() const = 0;

  // 返回所有可配置参数的元信息
  virtual std::vector<AlgoParamInfo> get_parameter_info() const = 0;
  
  // 返回所有可输出的信号源
  virtual std::vector<AlgoSignalInfo> get_signal_info() const = 0;

  // 获取算法结果
  virtual void get_results() {}

 protected:
  // 发送处理结果
  void emit_image(const std::string& name, const cv::Mat& img) {
    if (declared_signals_.count(name)) {
      ImageSignalBus::instance().emit(name, img);
    }
  }

 private:
  // 配置映射表
  std::unordered_map<std::string, std::function<void(const std::string&)>> configMap_;
  // 声明的信号集合
  std::unordered_set<std::string> declared_signals_;
};

// 算法元信息宏定义
#define ALGO_METADATA(NAME, DESCRIPTION)                                      \
  static constexpr const char* algorithm_name() noexcept { return NAME; }     \
  static constexpr const char* algorithm_description() noexcept {             \
    return DESCRIPTION;                                                       \
  }                                                                           \
  std::string get_name() const noexcept override { return algorithm_name(); } \
  std::string get_description() const noexcept override {                     \
    return algorithm_description();                                           \
  }

} // namespace algo
```

### 3. AlgoAdapter（算法适配器）

#### 职责

- 将算法适配到 FrameProcessor 接口
- 确保算法的延迟初始化

#### 接口定义

```cpp
class AlgoAdapter : public FrameProcessor {
 public:
  explicit AlgoAdapter(AlgoPtr algo) : algo_(std::move(algo)) {
    // 初始化算法的信号源和配置信息
    algo_->initialize();
  }
  
  void process(const CapturedFrame& frame) override {
    if (algo_) {
      algo_->process(frame);
    }
  }

 private:
  AlgoPtr algo_;  // 智能指针保证生命周期
};
```

## 信号机制流程

### 1. 信号声明阶段

1. 算法实现 [get_signal_info()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L66-L66) 方法，返回可提供的信号源列表
2. 在 [AlgoAdapter](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L135-L149) 构造时调用算法的 [initialize()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L42-L49) 方法
3. [initialize()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L42-L49) 方法将信号源注册到 [ImageSignalBus](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L20-L88)

### 2. 信号订阅阶段

1. 客户端或其它组件调用 [ImageSignalBus::subscribe()](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L39-L39)
2. 提供信号名称和回调函数
3. [ImageSignalBus](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L20-L88) 将回调函数添加到订阅者列表

### 3. 信号发布阶段

1. 算法在处理过程中调用 [emit_image()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L95-L101) 方法
2. [emit_image()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L95-L101) 将图像数据发送到 [ImageSignalBus::emit()](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L41-L41)
3. [ImageSignalBus](file:///d:/codespace\CFP\include\ImageSignalBus.hpp#L20-L88) 遍历订阅者列表并调用回调函数

## 线程安全设计

### 1. 信号总线线程安全

- 使用 [std::shared_mutex](file:///C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.39.33519/include\shared_mutex#L20-L20) 实现读写锁
- 订阅和声明操作使用写锁
- 信号发布操作使用读锁，提高并发性能

### 2. 图像数据线程安全

- 所有图像数据通过深拷贝传递
- 算法内部处理在工作线程
- 信号回调确保数据生命周期安全

### 3. 算法处理线程安全

- 算法 [process()](file:///d:/codespace\CFP\include\algo\AlgoBase.hpp#L53-L53) 方法在工作线程执行
- 算法实现需保证线程安全（使用无状态设计或内部锁机制）

## 信号源类型

### 1. 图像信号

- **原始图像**: `original_frame` - 未经处理的原始图像
- **处理图像**: `processed_frame` - 经过算法处理的图像
- **中间图像**: `binary_frame`, `threshold_frame` - 算法中间步骤的图像
- **可视化图像**: `visualization_frame` - 带有标注的可视化结果

### 2. 数据信号

- **特征数据**: 特征向量、统计信息等
- **状态数据**: 算法运行状态、错误信息等

## 使用示例

### 1. 实现自定义算法

```cpp
class CustomDetection : public AlgoBase {
 public:
  ALGO_METADATA("CustomDetection", "自定义检测算法")
  
  void process(const CapturedFrame& frame) override {
    cv::Mat image = frame.image;
    
    // 执行图像处理
    cv::Mat processed = preprocess(image);
    cv::Mat result = detect(processed);
    
    // 发布处理结果
    emit_image("preprocessed", processed);
    emit_image("detection_result", result);
  }
  
  std::vector<AlgoSignalInfo> get_signal_info() const override {
    return {
        {"preprocessed", "预处理后的图像"},
        {"detection_result", "检测结果图像"}
    };
  }
  
  std::vector<AlgoParamInfo> get_parameter_info() const override {
    return {
        {"threshold", "float", "检测阈值", "0.7", "0.7"},
        {"min_area", "int", "最小面积", "100", "100"}
    };
  }
};
```

### 2. 订阅算法信号

```cpp
// 订阅特定信号
auto& signalBus = ImageSignalBus::instance();
signalBus.subscribe("detection_result", [](const cv::Mat& img) {
    // 在UI线程中处理图像
    QImage qimg = convertOpenCVMatToQImage(img);
    updateUI(qimg);
});

// 订阅多个信号
signalBus.subscribe("preprocessed", [](const cv::Mat& img) {
    // 处理预处理图像
    displayPreprocessedImage(img);
});
```

### 3. 使用算法适配器

```cpp
// 创建算法实例
auto algo = std::make_shared<CustomDetection>();

// 使用适配器包装算法
auto adapter = std::make_shared<AlgoAdapter>(algo);

// 将适配器连接到图像处理管道
frameProcessor = adapter;
```

## 性能考虑

### 1. 信号发布性能

- 避免发布不必要的图像数据
- 控制信号发布频率
- 使用高效的图像拷贝方法

### 2. 订阅者管理

- 及时取消不需要的订阅
- 避免重复订阅同一信号
- 合理管理订阅者生命周期

### 3. 内存使用

- 控制图像数据的内存占用
- 使用对象池减少内存分配
- 及时释放不需要的图像数据

## 扩展性设计

### 1. 动态算法加载

- 通过插件化架构支持运行时加载算法
- 算法信号源自动注册
- 无需重启应用即可使用新算法

### 2. 信号类型扩展

- 支持不同类型的数据信号
- 可扩展的信号数据结构
- 保持向后兼容性

### 3. 配置管理

- 统一的参数配置接口
- 运行时参数动态更新
- 参数验证和类型检查
