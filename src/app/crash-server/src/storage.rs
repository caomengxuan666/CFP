use anyhow::Result;
use std::path::{Path, PathBuf};
use tokio::fs;
use tokio::io::{AsyncRead, AsyncWriteExt};

pub struct FileStorage {
    base_path: PathBuf,
    max_minidump_size: u64,
    max_pdb_size: u64,
}

impl FileStorage {
    // 方法1：使用同步方法创建目录（推荐）
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

    // 方法2：或者改为 async 方法
    pub async fn new_async(
        base_path: &str,
        max_minidump_size: u64,
        max_pdb_size: u64,
    ) -> Result<Self> {
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

    // 读取文件
    pub async fn read_file(&self, path: &Path) -> Result<Vec<u8>> {
        let content = fs::read(path).await?;
        Ok(content)
    }

    // 检查文件是否存在
    pub fn file_exists(&self, path: &Path) -> bool {
        path.exists()
    }

    // 获取完整路径（防止路径遍历攻击）
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

    // 获取minidump最大大小
    pub fn get_max_minidump_size(&self) -> u64 {
        self.max_minidump_size
    }

    // 获取pdb最大大小
    pub fn get_max_pdb_size(&self) -> u64 {
        self.max_pdb_size
    }
}
