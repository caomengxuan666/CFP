use config::{Config, File};
use serde::Deserialize;

#[derive(Debug, Deserialize, Clone)]
pub struct DatabaseConfig {
    pub path: String,
}

#[derive(Debug, Deserialize, Clone)]
pub struct Settings {
    pub server: ServerConfig,
    pub database: DatabaseConfig,
    pub storage: StorageConfig,
    pub logging: LoggingConfig,
}

#[derive(Debug, Deserialize, Clone)]
pub struct ServerConfig {
    pub port: u16,
    pub host: String,
    pub api_key: Option<String>, // 改为 Option<String>
}

#[derive(Debug, Deserialize, Clone)]
pub struct StorageConfig {
    pub base_path: String,
    pub max_minidump_size: u64,
    pub max_pdb_size: u64,
}

#[derive(Debug, Deserialize, Clone)]
pub struct LoggingConfig {
    pub level: String,
    pub file: String,
}

impl Settings {
    pub fn new() -> Result<Self, config::ConfigError> {
        let s = Config::builder()
            .add_source(File::with_name("config/config").required(false))
            .set_default("server.host", "127.0.0.1")?
            .set_default("server.port", 3410)?
            .set_default("server.api_key", "your-secret-api-key")? // 这里还是 String
            .set_default("database.path", "./data/db.sqlite")?
            .set_default("storage.base_path", "./data")?
            .set_default("storage.max_minidump_size", 10 * 1024 * 1024)?
            .set_default("storage.max_pdb_size", 100 * 1024 * 1024)?
            .set_default("logging.level", "info")?
            .set_default("logging.file", "./logs/app.log")?
            .build()?;

        // config crate 会自动将默认的 String 转为 Option<String>
        s.try_deserialize()
    }
}
