# CFP 动态算法库设计文档

## 文档概述

本文档详细描述了 CFP 系统中动态算法库的设计方案。通过实现插件化的算法架构，系统将支持动态加载、替换和更新算法库，无需重新编译整个应用程序。

## 设计背景

在工业视觉检测应用中，不同场景和客户需求可能导致对算法性能和准确性的不同要求。传统的静态链接算法在部署后难以更改，需要重新编译整个应用程序。为了提高系统的灵活性和可维护性，我们设计了动态算法库机制，允许：

- 在运行时动态加载和替换算法
- 服务器远程推送新算法到前端机
- 现场根据性能表现替换特定算法
- 无需远程访问客户主机即可进行算法优化

## 接口设计

### 1. IAlgorithm 接口

#### 职责

- 定义算法的标准接口
- 提供统一的算法调用方式

#### 接口定义

```cpp
class IAlgorithm {
public:
    virtual ~IAlgorithm() = default;
    
    // 算法执行接口
    virtual ProcessResult execute(const cv::Mat& input) = 0;
    
    // 算法配置接口
    virtual bool configure(const std::unordered_map<std::string, double>& params) = 0;
    
    // 获取算法元信息
    virtual AlgorithmMetadata getMetadata() const = 0;
    
    // 验证输入数据
    virtual bool validateInput(const cv::Mat& input) const = 0;
    
    // 算法版本
    virtual std::string getVersion() const = 0;
    
    // 算法名称
    virtual std::string getName() const = 0;
};
```

### 2. AlgorithmMetadata 结构

#### 职责

- 存储算法的元数据信息
- 包括参数列表、处理步骤、版本等

#### 结构定义

```cpp
struct AlgorithmParameter {
    std::string name;
    std::string description;
    double defaultValue;
    double minValue;
    double maxValue;
};

struct AlgorithmStep {
    std::string name;
    std::string description;
    int executionOrder;
};

struct AlgorithmMetadata {
    std::string name;                    // 算法名称
    std::string version;                 // 版本号
    std::string description;             // 描述
    std::vector<AlgorithmParameter> parameters;  // 参数列表
    std::vector<AlgorithmStep> steps;    // 处理步骤
    std::string author;                  // 作者
    std::string license;                 // 许可证
    std::chrono::system_clock::time_point createdTime;  // 创建时间
    std::chrono::system_clock::time_point modifiedTime; // 修改时间
};
```

### 3. AlgorithmManager

#### 职责

- 管理动态算法库的加载和卸载
- 维护可用算法列表

#### 接口定义

```cpp
class AlgorithmManager {
public:
    // 加载动态算法库
    virtual bool loadAlgorithm(const std::string& libraryPath) = 0;
    
    // 卸载算法库
    virtual bool unloadAlgorithm(const std::string& algorithmName) = 0;
    
    // 获取算法实例
    virtual std::shared_ptr<IAlgorithm> getAlgorithm(const std::string& name) = 0;
    
    // 获取所有可用算法
    virtual std::vector<AlgorithmMetadata> getAvailableAlgorithms() const = 0;
    
    // 验证算法库文件
    virtual bool validateAlgorithmLibrary(const std::string& libraryPath) const = 0;
    
    // 更新算法库（从服务器下载）
    virtual bool updateAlgorithmFromServer(const std::string& algorithmName, 
                                         const std::string& serverUrl) = 0;
    
    // 获取算法管理器单例
    static AlgorithmManager& getInstance();
};
```

## 实现方案

### 1. 动态库接口设计

每个算法动态库必须导出以下函数：

```cpp
// 创建算法实例
extern "C" IAlgorithm* create_algorithm();

// 销毁算法实例
extern "C" void destroy_algorithm(IAlgorithm* algorithm);

// 获取算法元数据
extern "C" AlgorithmMetadata get_metadata();
```

### 2. 跨平台动态库加载

使用平台特定的 API 实现动态库加载：

- Windows: `LoadLibrary` / `GetProcAddress`
- Linux: `dlopen` / `dlsym`
- macOS: `dlopen` / `dlsym`

封装为统一的 [DynamicLibrary](file:///d:/codespace\CFP\include\utils\executable_path.h#L38-L42) 类进行管理。

### 3. 算法工厂模式

```cpp
class AlgorithmFactory {
public:
    // 注册算法创建函数
    static void registerAlgorithm(const std::string& name, 
                                std::function<IAlgorithm*()> creator);
    
    // 创建算法实例
    static std::unique_ptr<IAlgorithm> create(const std::string& name);
    
private:
    static std::unordered_map<std::string, std::function<IAlgorithm*()>> creators_;
};
```

## 安全考虑

### 1. 算法验证

- 在加载算法库前验证文件完整性（如 SHA256 校验）
- 检查数字签名确保算法来源可信
- 验证算法接口兼容性

### 2. 沙箱执行

- 限制算法执行的资源使用（CPU 时间、内存）
- 监控算法执行过程中的异常行为
- 实现算法执行超时机制

### 3. 版本管理

- 维护算法版本信息
- 支持向后兼容性检查
- 实现版本回滚机制

## 远程更新机制

### 1. 服务器端

- 提供算法库下载接口
- 维护算法版本信息
- 实现算法验证机制

### 2. 客户端

- 定期检查服务器上的新算法版本
- 下载并验证新算法库
- 动态替换运行中的算法

### 3. 更新策略

- 支持静默更新（后台下载，下次重启时应用）
- 支持热更新（立即替换运行中的算法）
- 提供更新回滚机制

## 性能优化

### 1. 缓存机制

- 缓存已加载的算法实例
- 预加载常用算法以减少延迟

### 2. 资源管理

- 实现算法资源的自动管理
- 防止内存泄漏和资源竞争

### 3. 并发处理

- 支持多线程访问不同算法实例
- 实现算法执行的线程安全

## 错误处理

### 1. 加载错误

- 处理动态库加载失败
- 提供备用算法或降级方案

### 2. 执行错误

- 捕获算法执行异常
- 提供错误恢复机制

### 3. 版本冲突

- 检测算法版本不兼容
- 提供版本兼容性警告

## 扩展性

### 1. 算法类型支持

- 支持多种算法类型（检测、分类、分割等）
- 可扩展的元数据结构

### 2. 配置管理

- 支持算法参数的动态配置
- 配置文件与算法库分离管理

### 3. 插件机制

- 支持算法插件的开发和集成
- 提供插件开发工具包

## 测试策略

### 1. 单元测试

- 测试算法接口的正确实现
- 验证元数据的完整性

### 2. 集成测试

- 测试动态库加载和卸载
- 验证算法替换过程

### 3. 性能测试

- 测试算法加载性能
- 验证执行性能不受影响

## 部署考虑

### 1. 目录结构

```
algorithms/
├── hole_detection_v1.dll      # Windows
├── hole_detection_v2.so       # Linux
├── edge_detection_v1.dll
└── metadata/
    ├── hole_detection_v1.json
    └── edge_detection_v1.json
```

### 2. 权限管理

- 确保算法库文件的正确访问权限
- 防止未授权的算法替换

### 3. 兼容性

- 确保不同平台的兼容性
- 提供跨平台的 API 接口
