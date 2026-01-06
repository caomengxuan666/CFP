# 存储模块设计文档

## 概述

存储模块是crash-server的核心组件之一，负责管理Minidump和PDB文件的存储。该模块采用分层目录结构，基于EXE版本号、GUID和Age值组织文件，确保Minidump与PDB文件的天然关联。

## 核心功能

### 1. 文件路径生成

存储模块根据EXE的元数据生成唯一的文件存储路径：

- **Minidump文件路径**: `./storage/minidumps/{exe_version}/{exe_guid}_{exe_age}/{filename}`
- **PDB文件路径**: `./storage/pdbs/{exe_version}/{exe_guid}_{exe_age}/{filename}`

### 2. 目录结构管理

模块自动创建必要的目录结构，确保文件存储路径的完整性：

```rust
// 生成minidump文件的存储路径
pub fn get_minidump_path(
    &self,
    exe_version: &str,
    exe_guid: &str,
    exe_age: i32,
    filename: &str,
) -> PathBuf {
    self.base_path
        .join("minidumps")
        .join(exe_version)
        .join(format!("{}_{}", exe_guid, exe_age))
        .join(filename)
}

// 生成pdb文件的存储路径
pub fn get_pdb_path(
    &self,
    exe_version: &str,
    exe_guid: &str,
    exe_age: i32,
    filename: &str,
) -> PathBuf {
    self.base_path
        .join("pdbs")
        .join(exe_version)
        .join(format!("{}_{}", exe_guid, exe_age))
        .join(filename)
}
```

### 3. 安全的文件操作

模块提供异步文件操作功能，包含文件大小限制和路径安全检查：

```rust
// 保存上传的文件到指定路径
pub async fn save_file<T>(&self, path: &Path, mut file: T, max_size: u64) -> Result<()>
where
    T: AsyncRead + Unpin,
{
    // 确保目录存在
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).await?;
    }

    // 创建文件
    let mut out_file = fs::File::create(path).await?;

    // 读取并写入文件，同时检查大小
    let mut buffer = [0; 8192]; // 8KB buffer
    let mut total_size = 0u64;

    loop {
        let bytes_read = tokio::io::AsyncReadExt::read(&mut file, &mut buffer).await?;

        if bytes_read == 0 {
            break; // 文件读取完成
        }

        total_size += bytes_read as u64;

        if total_size > max_size {
            return Err(anyhow::anyhow!("File exceeds maximum size limit"));
        }

        out_file.write_all(&buffer[..bytes_read]).await?;
    }

    out_file.flush().await?;

    Ok(())
}
```

### 4. 路径安全验证

为防止路径遍历攻击，模块提供路径安全验证功能：

```rust
pub fn get_sanitized_path(&self, base_dir: &str, path: &str) -> Option<PathBuf> {
    // 防止路径遍历攻击
    let path = Path::new(path);

    // 检查路径中是否包含父目录引用
    if path
        .components()
        .any(|c| c == std::path::Component::ParentDir)
    {
        return None;
    }

    let full_path = self.base_path.join(base_dir).join(path);

    // 确保完整路径在基础目录下
    if full_path.starts_with(self.base_path.join(base_dir)) {
        Some(full_path)
    } else {
        None
    }
}
```

## 初始化方法

模块提供两种初始化方法：

1. **同步初始化** (`new`): 使用同步文件操作创建目录
2. **异步初始化** (`new_async`): 使用异步文件操作创建目录

```rust
// 同步初始化
pub fn new(base_path: &str, max_minidump_size: u64, max_pdb_size: u64) -> Result<Self> {
    let base_path = PathBuf::from(base_path);
    
    // 使用同步的文件操作创建目录
    std::fs::create_dir_all(&base_path)?;
    std::fs::create_dir_all(base_path.join("minidumps"))?;
    std::fs::create_dir_all(base_path.join("pdbs"))?;

    Ok(FileStorage {
        base_path,
        max_minidump_size,
        max_pdb_size,
    })
}

// 异步初始化
pub async fn new_async(base_path: &str, max_minidump_size: u64, max_pdb_size: u64) -> Result<Self> {
    let base_path = PathBuf::from(base_path);
    
    // 使用异步的文件操作创建目录
    fs::create_dir_all(&base_path).await?;
    fs::create_dir_all(base_path.join("minidumps")).await?;
    fs::create_dir_all(base_path.join("pdbs")).await?;

    Ok(FileStorage {
        base_path,
        max_minidump_size,
        max_pdb_size,
    })
}
```

## 文件大小限制

模块支持对不同类型的文件设置不同的大小限制：

- **Minidump文件**: 默认最大100MB（可通过配置修改）
- **PDB文件**: 默认最大500MB（可通过配置修改）

## 配置参数

| 参数 | 说明 | 默认值 |
|------|------|--------|
| `base_path` | 文件存储根目录 | ./storage |
| `max_minidump_size` | Minidump文件最大大小 | 104857600 bytes (100MB) |
| `max_pdb_size` | PDB文件最大大小 | 524288000 bytes (500MB) |

## 安全特性

1. **路径遍历防护**: 防止恶意路径访问系统文件
2. **文件大小限制**: 防止大文件上传导致的资源耗尽
3. **目录隔离**: Minidump和PDB文件存储在独立的目录中
4. **权限控制**: 通过文件系统权限控制访问

## 使用场景

- **Minidump上传**: 根据崩溃EXE的元数据生成存储路径
- **PDB上传**: 与对应的Minidump保持相同的目录结构
- **文件下载**: 通过安全路径验证提供文件访问
- **关联查询**: 基于目录结构快速定位相关文件

## 扩展性

存储模块设计考虑了未来的扩展需求：

- **云存储**: 可以扩展为支持S3、MinIO等对象存储
- **分布式存储**: 支持多节点共享存储
- **压缩存储**: 可添加文件压缩功能以节省空间
- **加密存储**: 可添加文件加密功能以提高安全性
