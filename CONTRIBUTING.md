# 贡献指南 (Contributing Guide)

感谢您对CFP项目的关注和贡献！本文档将指导您如何参与项目开发。

## 项目概述

CFP (CAPON Frontend Processor) 是一个基于 C++ 的工业视觉检测应用，专注于 DVP/IK/MIND相机控制、图像采集与处理（如孔洞检测）。本项目采用模块化设计，支持 OpenCV 图像处理功能按需启用，适用于工业自动化场景。

## 分支策略

我们采用简化的 Git Flow 分支策略，适用于小团队开发（2-3人）：

- **master**: 主分支，存放稳定版本的代码，仅在发布新版本时合并代码
- **develop**: 开发分支，日常开发的主要分支，所有功能分支和修复分支最终合并到此分支
- **feature分支**: 临时功能分支，命名格式为 `feature/功能名称`，用于开发新功能
- **hotfix分支**: 修复分支，命名格式为 `hotfix/修复内容`，用于紧急修复

### 分支操作流程

1. 从 develop 分支创建功能分支：

   ```bash
   git checkout develop
   git pull origin develop
   git checkout -b feature/功能名称
   ```

2. 在功能分支上进行开发，提交代码：

   ```bash
   git add .
   git commit -m "描述提交内容"
   git push origin feature/功能名称
   ```

3. 在代码托管平台创建 Pull Request (PR) 到 develop 分支，请求其他成员进行代码审查

4. 审查通过后，将功能分支合并到 develop 分支

5. 当 develop 分支的代码稳定并准备好发布时，创建 PR 合并到 master 分支

## 开发环境要求

### 必需工具

- CMake 3.15+
- vcpkg
- Qt (路径由QT_DIR环境变量指定)
- Visual Studio (MSVC编译器)
- DVP SDK
- IKap SDK

### 可选工具

- Python (用于脚本执行)

### 搭建开发环境

1. 安装vcpkg到D:/vcpkg
2. 设置QT_DIR环境变量指向Qt安装目录
3. 确保DVP SDK和IKap SDK正确安装
4. 克隆项目到d:\codespace\CFP

## 构建项目

| 请注意，在根目录的CMake下修改自己的OpenCV的库路径，并且提交的时候请勿将修改后的路径提交，未来这部分将采用更好的方式如环境变量来统一，避免项目冲突。(原因是传统方式找OPENCV在这台主机需要长达一分多钟的搜索时间，大量浪费时间在CMAKE的配置阶段)如果要保持一致，可以在D盘下下载VCPKG并且安装OPENCV，这同样可以避免这个问题。

```bash
# 更新submodule
git submodule update --init --recursive

# 创建构建目录
mkdir build && cd build

# 配置项目（启用OpenCV和测试）
cmake .. -DENABLE_OPENCV=ON -DENABLE_TESTS=ON -DVCPKG_ROOT="D:/vcpkg" -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake"

# 编译项目
cmake --build . --config Release

# 或使用make/ninja等生成器
cmake --build . --config Debug --target all
```

### 运行测试

```bash
# 运行测试
ctest --config Debug

# 重新配置
cmake .. -DENABLE_OPENCV=OFF
```

## 代码规范

### C++ 代码规范

1. 遵循 C++20 标准
2. 使用 Google C++ Style Guide
3. 代码格式采用 clang-format 统一格式化，请在提交到仓库之前使用 [scripts/format.py](file:///d:/codespace/CFP/script/format.py) 进行格式化
4. 使用 NOMINMAX 宏避免 Windows min/max 冲突
5. UTF-8 编码

### 代码结构要求

1. 模块化设计，便于算法替换
2. 高度抽象的相机接口设计
3. 支持多类型工业相机（DVP/IK/MIND）
4. 保留多相机接口框架

### 提交信息规范

提交信息应遵循以下格式：

```
[type]: 简短描述 (不超过50个字符)

更详细的描述（如果需要），每行不超过72个字符
```

类型包括：

- `feat`: 新功能
- `fix`: 修复bug
- `docs`: 文档更新
- `style`: 代码格式调整（不影响代码逻辑）
- `refactor`: 重构（不修复bug或添加功能）
- `test`: 测试相关
- `chore`: 构建过程或辅助工具的变动

## 项目架构

### 核心模块

- **cameras**: 相机相关类 (Dvp, Ikap)
- **algo**: 算法相关头文件
- **business**: 业务逻辑
- **config**: 配置管理
- **protocol**: 通信协议
- **redis**: Redis客户端
- **utils**: 工具函数

### 设计模式

- **工厂模式**: CameraFactory 创建不同类型的相机实例
- **适配器模式**: AlgoAdapter 将 AlgoBase 适配为 FrameProcessor 接口
- **发布-订阅模式**: ImageSignalBus 实现图像信号的发布与订阅
- **策略模式**: 不同的 Codec 和 TransportAdapter 实现
- **单例模式**: 配置管理和状态同步组件

## 开发流程

1. 从 `develop` 分支创建功能分支
2. 实现功能或修复bug
3. 编写单元测试（如适用）
4. 运行测试确保没有破坏现有功能
5. 提交代码并推送到远程仓库
6. 创建 Pull Request 请求合并

## 测试要求

在提交代码前，请确保：

1. 运行所有单元测试并确保通过
2. 如有需要，添加新的测试用例
3. 运行集成测试（如果存在）

测试目录: `tests/`

- **camera_builder**: 相机构建测试
- **hole_detection**: 孔洞检测测试
- **mock**: 模拟对象

## 代码审查

所有代码在合并前都需要经过至少一名其他开发者的审查。审查时会关注：

1. 代码是否符合规范
2. 是否包含适当的测试
3. 是否有适当的注释
4. 代码逻辑是否正确
5. 是否存在潜在的性能或安全问题
6. 是否与项目架构保持一致

## 技术约束和限制

1. **性能要求**:
   - 实时图像处理能力
   - 低延迟网络通信
   - 高效的内存管理

2. **安全要求**:
   - 线程安全的数据访问
   - 安全的内存操作
   - 输入验证

## 详细文档

更多技术细节请参见 `DOCS/` 目录下的设计文档：

- [DOCS/communication_protocol.md](file:///d:/codespace/CFP/DOCS/communication_protocol.md): 详细描述系统中各个组件之间的通信协议
- [DOCS/system_architecture.md](file:///d:/codespace/CFP/DOCS/system_architecture.md): 详细描述系统整体架构设计
- [DOCS/multi_camera_interface.md](file:///d:/codespace/CFP/DOCS/multi_camera_interface.md): 详细描述多相机接口设计和实现方案
- [DOCS/dynamic_algorithm_design.md](file:///d:/codespace/CFP/DOCS/dynamic_algorithm_design.md): 详细描述动态算法库的设计方案
- [DOCS/client_design.md](file:///d:/codespace/CFP/DOCS/client_design.md): 详细描述客户端界面设计和信号订阅机制
- [DOCS/algorithm_signal_mechanism.md](file:///d:/codespace/CFP/DOCS/algorithm_signal_mechanism.md): 详细描述算法信号发布与订阅机制

## 联系方式

如有问题，请联系项目维护者@caomengxuan666。
