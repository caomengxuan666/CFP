use anyhow::Result;
use std::sync::Arc;

mod api;
mod config;
mod database;
mod middleware;
mod storage;

use crate::database::Database;
use crate::storage::FileStorage;

#[tokio::main]
async fn main() -> Result<()> {
    // 加载配置
    let settings = config::Settings::new()?;

    // 根据配置初始化日志
    std::fs::create_dir_all(
        std::path::Path::new(&settings.logging.file)
            .parent()
            .unwrap(),
    )?;
    env_logger::Builder::from_env(
        env_logger::Env::default().default_filter_or(&settings.logging.level),
    )
    .format_timestamp_secs()
    .init();

    // 初始化数据库
    let db = Arc::new(Database::new(&settings.database.path)?);

    // 初始化文件存储，使用配置中的最大文件大小
    let storage = Arc::new(
        FileStorage::new_async(
            &settings.storage.base_path,
            settings.storage.max_minidump_size,
            settings.storage.max_pdb_size,
        )
        .await?,
    );

    // 创建共享状态
    let shared_state = api::AppState {
        db: db.clone(),
        storage: storage.clone(),
        config: Arc::new(settings),
    };

    // 启动服务器
    api::start_server(shared_state).await?;

    Ok(())
}
