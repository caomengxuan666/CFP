use anyhow::Result;
use chrono::{DateTime, Utc};
use rusqlite::{params, Connection, Row};
use serde::{Deserialize, Serialize};
use std::sync::{Arc, Mutex};
//use uuid::Uuid;

// 添加这一行以导入OptionalExtension trait
use rusqlite::OptionalExtension;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Minidump {
    pub id: String,
    pub timestamp: String,                 // 崩溃时间
    pub exe_version: String,               // EXE版本号
    pub exe_guid: String,                  // EXE的GUID
    pub exe_age: i32,                      // PDB的Age值
    pub pid: i32,                          // 崩溃进程ID
    pub tid: i32,                          // 崩溃线程ID
    pub exception_code: String,            // 异常代码
    pub exception_address: Option<String>, // 异常地址
    pub file_path: String,                 // Minidump文件存储路径
    pub created_at: String,                // 服务器接收时间
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Pdb {
    pub id: String,
    pub exe_version: String,  // EXE版本号
    pub exe_guid: String,     // EXE的GUID
    pub exe_age: i32,         // PDB的Age值
    pub pdb_filename: String, // PDB文件名
    pub file_path: String,    // PDB文件存储路径
    pub uploaded_at: String,  // 上传时间
}

pub struct Database {
    conn: Arc<Mutex<Connection>>,
}

impl Database {
    pub fn new(db_path: &str) -> Result<Self> {
        std::fs::create_dir_all(std::path::Path::new(db_path).parent().unwrap())?;

        let conn = Connection::open(db_path)?;

        // 创建表
        Self::init_db(&conn)?;

        Ok(Database {
            conn: Arc::new(Mutex::new(conn)),
        })
    }

    fn init_db(conn: &Connection) -> Result<()> {
        // 创建minidumps表
        conn.execute(
            "CREATE TABLE IF NOT EXISTS minidumps (
                id TEXT PRIMARY KEY,
                timestamp TEXT NOT NULL,
                exe_version TEXT NOT NULL,
                exe_guid TEXT NOT NULL,
                exe_age INTEGER NOT NULL,
                pid INTEGER NOT NULL,
                tid INTEGER NOT NULL,
                exception_code TEXT NOT NULL,
                exception_address TEXT,
                file_path TEXT NOT NULL,
                created_at TEXT NOT NULL
            )",
            [],
        )?;

        // 创建pdbs表
        conn.execute(
            "CREATE TABLE IF NOT EXISTS pdbs (
                id TEXT PRIMARY KEY,
                exe_version TEXT NOT NULL,
                exe_guid TEXT NOT NULL,
                exe_age INTEGER NOT NULL,
                pdb_filename TEXT NOT NULL,
                file_path TEXT NOT NULL,
                uploaded_at TEXT NOT NULL
            )",
            [],
        )?;

        // 创建索引
        conn.execute(
            "CREATE INDEX IF NOT EXISTS idx_minidumps_version ON minidumps(exe_version)",
            [],
        )?;

        conn.execute(
            "CREATE INDEX IF NOT EXISTS idx_minidumps_guid_age ON minidumps(exe_guid, exe_age)",
            [],
        )?;

        conn.execute(
            "CREATE INDEX IF NOT EXISTS idx_minidumps_timestamp ON minidumps(timestamp)",
            [],
        )?;

        conn.execute(
            "CREATE INDEX IF NOT EXISTS idx_pdbs_guid_age ON pdbs(exe_guid, exe_age)",
            [],
        )?;

        conn.execute(
            "CREATE INDEX IF NOT EXISTS idx_pdbs_version ON pdbs(exe_version)",
            [],
        )?;

        Ok(())
    }

    pub fn save_minidump(&self, minidump: &Minidump) -> Result<()> {
        let conn = self.conn.lock().unwrap();

        conn.execute(
            "INSERT INTO minidumps (id, timestamp, exe_version, exe_guid, exe_age, pid, tid, exception_code, exception_address, file_path, created_at)
             VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11)",
            params![
                minidump.id,
                minidump.timestamp,
                minidump.exe_version,
                minidump.exe_guid,
                minidump.exe_age,
                minidump.pid,
                minidump.tid,
                minidump.exception_code,
                minidump.exception_address,
                minidump.file_path,
                minidump.created_at
            ],
        )?;

        Ok(())
    }

    pub fn save_pdb(&self, pdb: &Pdb) -> Result<()> {
        let conn = self.conn.lock().unwrap();

        conn.execute(
            "INSERT INTO pdbs (id, exe_version, exe_guid, exe_age, pdb_filename, file_path, uploaded_at)
             VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7)",
            params![
                pdb.id,
                pdb.exe_version,
                pdb.exe_guid,
                pdb.exe_age,
                pdb.pdb_filename,
                pdb.file_path,
                pdb.uploaded_at
            ],
        )?;

        Ok(())
    }

    pub fn get_minidump_by_id(&self, id: &str) -> Result<Option<Minidump>> {
        let conn = self.conn.lock().unwrap();

        let minidump = conn.query_row(
            "SELECT id, timestamp, exe_version, exe_guid, exe_age, pid, tid, exception_code, exception_address, file_path, created_at
             FROM minidumps WHERE id = ?1",
            params![id],
            |row| self.minidump_from_row(row),
        ).optional()?;

        Ok(minidump)
    }

    pub fn get_pdb_by_id(&self, id: &str) -> Result<Option<Pdb>> {
        let conn = self.conn.lock().unwrap();

        let pdb = conn
            .query_row(
                "SELECT id, exe_version, exe_guid, exe_age, pdb_filename, file_path, uploaded_at
             FROM pdbs WHERE id = ?1",
                params![id],
                |row| self.pdb_from_row(row),
            )
            .optional()?;

        Ok(pdb)
    }

    pub fn find_matching_pdb(
        &self,
        exe_version: &str,
        exe_guid: &str,
        exe_age: i32,
    ) -> Result<Option<Pdb>> {
        let conn = self.conn.lock().unwrap();

        let pdb = conn
            .query_row(
                "SELECT id, exe_version, exe_guid, exe_age, pdb_filename, file_path, uploaded_at
             FROM pdbs WHERE exe_version = ?1 AND exe_guid = ?2 AND exe_age = ?3",
                params![exe_version, exe_guid, exe_age],
                |row| self.pdb_from_row(row),
            )
            .optional()?;

        Ok(pdb)
    }

    pub fn find_minidumps(
        &self,
        version: Option<&str>,
        guid: Option<&str>,
        start_time: Option<DateTime<Utc>>,
        end_time: Option<DateTime<Utc>>,
        page: u32,
        page_size: u32,
    ) -> Result<Vec<Minidump>> {
        // 构建查询语句和参数
        let (sql, params) = {
            let mut base_sql = String::from(
                "SELECT id, timestamp, exe_version, exe_guid, exe_age, pid, tid, exception_code, exception_address, file_path, created_at
                 FROM minidumps",
            );

            let mut conditions = Vec::new();
            let mut params: Vec<String> = Vec::new();

            if let Some(ver) = version {
                conditions.push("exe_version = ?");
                let ver = ver.to_string();
                params.push(ver);
            }

            if let Some(g) = guid {
                conditions.push("exe_guid = ?");
                let g = g.to_string();
                params.push(g);
            }

            if let Some(start) = start_time {
                conditions.push("timestamp >= ?");
                let start_str = start.to_rfc3339();
                params.push(start_str); // 直接存储 String
            }
            if let Some(start) = start_time {
                conditions.push("timestamp >= ?");
                let start_str = start.to_rfc3339();
                params.push(start_str); // 直接存储 String
            }
            if !conditions.is_empty() {
                base_sql.push_str(" WHERE ");
                base_sql.push_str(&conditions.join(" AND "));
            }

            base_sql.push_str(" ORDER BY timestamp DESC");

            // 计算偏移量
            let offset = (page - 1) * page_size;
            base_sql.push_str(&format!(" LIMIT {} OFFSET {}", page_size.min(100), offset));

            (base_sql, params)
        };

        let conn = self.conn.lock().unwrap();
        let mut stmt = conn.prepare(&sql)?;

        // 根据参数数量执行不同的查询
        let rows = match params.len() {
            0 => stmt.query([])?,
            1 => stmt.query(params![params[0]])?,
            2 => stmt.query(params![params[0], params[1]])?,
            3 => stmt.query(params![params[0], params[1], params[2]])?,
            _ => stmt.query(params![params[0], params[1], params[2], params[3]])?,
        };

        let minidumps = rows
            .mapped(|row| self.minidump_from_row(row))
            .collect::<std::result::Result<Vec<Minidump>, _>>()?;

        Ok(minidumps)
    }

    fn minidump_from_row(&self, row: &Row) -> rusqlite::Result<Minidump> {
        Ok(Minidump {
            id: row.get(0)?,
            timestamp: row.get(1)?,
            exe_version: row.get(2)?,
            exe_guid: row.get(3)?,
            exe_age: row.get(4)?,
            pid: row.get(5)?,
            tid: row.get(6)?,
            exception_code: row.get(7)?,
            exception_address: row.get(8).ok(),
            file_path: row.get(9)?,
            created_at: row.get(10)?,
        })
    }

    fn pdb_from_row(&self, row: &Row) -> rusqlite::Result<Pdb> {
        Ok(Pdb {
            id: row.get(0)?,
            exe_version: row.get(1)?,
            exe_guid: row.get(2)?,
            exe_age: row.get(3)?,
            pdb_filename: row.get(4)?,
            file_path: row.get(5)?,
            uploaded_at: row.get(6)?,
        })
    }
}
