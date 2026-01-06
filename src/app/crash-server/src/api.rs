use axum::{
    extract::{Multipart, Path, Query, State},
    http::StatusCode,
    response::IntoResponse,
    routing::{get, post},
    Json, Router,
};
use chrono::{DateTime, Utc};
use serde::{Deserialize, Serialize};
use std::sync::Arc;
use tokio::fs::File;
use tokio::io::AsyncWriteExt;
use tokio_util::io::ReaderStream;
use uuid::Uuid;

use crate::{
    config::Settings,
    database::{Database, Minidump, Pdb},
    storage::FileStorage,
};

// 全局状态
#[derive(Clone)]
pub struct AppState {
    pub db: Arc<Database>,
    pub storage: Arc<FileStorage>,
    pub config: Arc<Settings>,
}

// 统一响应格式
#[derive(Serialize)]
pub struct ApiResponse<T> {
    success: bool,
    data: Option<T>,
    error: Option<String>,
}

impl<T> ApiResponse<T> {
    pub fn success(data: T) -> Self {
        Self {
            success: true,
            data: Some(data),
            error: None,
        }
    }

    pub fn error(error: String) -> Self {
        Self {
            success: false,
            data: None,
            error: Some(error),
        }
    }
}

// Minidump上传元数据
#[derive(Deserialize)]
struct MinidumpUploadMetadata {
    timestamp: String,
    exe_version: String,
    exe_guid: String,
    exe_age: i32,
    pid: i32,
    tid: i32,
    exception_code: String,
    exception_address: Option<String>,
}

// PDB上传元数据
#[derive(Deserialize)]
struct PdbUploadMetadata {
    exe_version: String,
    exe_guid: String,
    exe_age: i32,
    pdb_filename: String,
}

// 查询参数
#[derive(Deserialize)]
pub struct MinidumpQueryParams {
    version: Option<String>,
    guid: Option<String>,
    start_time: Option<DateTime<Utc>>,
    end_time: Option<DateTime<Utc>>,
    page: Option<u32>,
    page_size: Option<u32>,
}

pub async fn start_server(state: AppState) -> anyhow::Result<()> {
    // 从 state.config 中获取配置
    let host = &state.config.server.host;
    let port = state.config.server.port;

    // 设置API密钥环境变量（如果是 Some）
    if let Some(api_key) = &state.config.server.api_key {
        std::env::set_var("API_KEY", api_key);
    }

    // 创建路由
    let app = create_app(state.clone());

    // 运行服务器
    let listener = tokio::net::TcpListener::bind(format!("{}:{}", host, port)).await?;

    println!("Server running on http://{}:{}", host, port);
    axum::serve(listener, app).await?;

    Ok(())
}

// 创建路由应用
pub fn create_app(state: AppState) -> Router {
    Router::new()
        // 健康检查
        .route("/health", get(health_check))
        // 上传接口
        .route("/upload/minidump", post(upload_minidump))
        .route("/upload/pdb", post(upload_pdb))
        // 下载接口
        .route("/download/minidump/:id", get(download_minidump))
        .route("/download/pdb/:id", get(download_pdb))
        // 查询接口
        .route("/list/minidumps", get(list_minidumps))
        .route("/find/matching-pdb", get(find_matching_pdb))
        // 中间件（API密钥验证）
        .layer(axum::middleware::from_fn_with_state(
            state.clone(),
            crate::middleware::auth::api_key_auth,
        ))
        .with_state(state)
}

// 健康检查端点
async fn health_check() -> impl IntoResponse {
    Json(serde_json::json!({
        "status": "ok",
        "timestamp": chrono::Utc::now().to_rfc3339()
    }))
}

// 上传Minidump
async fn upload_minidump(
    State(state): State<AppState>,
    mut multipart: Multipart,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let mut metadata: Option<MinidumpUploadMetadata> = None;
    let mut minidump_file_bytes = Vec::new();

    while let Some(field) = multipart
        .next_field()
        .await
        .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?
    {
        let name = field.name().unwrap_or("unknown").to_string();

        if name == "metadata" {
            let data = field
                .text()
                .await
                .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?;
            metadata = Some(
                serde_json::from_str(&data)
                    .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?,
            );
        } else if name == "minidump" {
            minidump_file_bytes = field
                .bytes()
                .await
                .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?
                .to_vec();

            // 验证文件类型 - Minidump文件头应该是MDMP (0x4D444D50)
            if minidump_file_bytes.len() < 4 {
                return Err((StatusCode::BAD_REQUEST, "Invalid minidump file".to_string()));
            }
            let header = &minidump_file_bytes[0..4];
            if header != b"MDMP" {
                return Err((
                    StatusCode::BAD_REQUEST,
                    "File is not a valid minidump".to_string(),
                ));
            }
        }
    }

    let metadata = metadata.ok_or((StatusCode::BAD_REQUEST, "Missing metadata".to_string()))?;

    // 生成唯一ID和文件路径
    let minidump_id = Uuid::new_v4().to_string();
    let timestamp_str = metadata
        .timestamp
        .replace("-", "")
        .replace(":", "")
        .replace("T", "")
        .replace("Z", "");
    let filename = format!(
        "crash_{}_{}_{}.dmp",
        timestamp_str, metadata.pid, metadata.tid
    );
    let file_path = state.storage.get_minidump_path(
        &metadata.exe_version,
        &metadata.exe_guid,
        metadata.exe_age,
        &filename,
    );

    // 使用storage的save_file方法，该方法会确保父目录存在
    let file_path_clone = file_path.clone();
    let result = state
        .storage
        .save_file(
            &file_path_clone,
            std::io::Cursor::new(minidump_file_bytes),
            state.storage.get_max_minidump_size(),
        )
        .await;

    if let Err(e) = result {
        return Err((StatusCode::INTERNAL_SERVER_ERROR, e.to_string()));
    }

    // 创建Minidump记录
    let minidump = Minidump {
        id: minidump_id.clone(),
        timestamp: metadata.timestamp,
        exe_version: metadata.exe_version,
        exe_guid: metadata.exe_guid,
        exe_age: metadata.exe_age,
        pid: metadata.pid,
        tid: metadata.tid,
        exception_code: metadata.exception_code,
        exception_address: metadata.exception_address,
        file_path: file_path.to_string_lossy().to_string(),
        created_at: Utc::now().to_rfc3339(),
    };

    // 保存到数据库
    state
        .db
        .save_minidump(&minidump)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    // 准备响应
    let response = ApiResponse::success(serde_json::json!({
        "minidump_id": minidump_id,
        "file_path": file_path.to_string_lossy().to_string()
    }));

    Ok((StatusCode::OK, Json(response)))
}

// 上传PDB
async fn upload_pdb(
    State(state): State<AppState>,
    mut multipart: Multipart,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let mut metadata: Option<PdbUploadMetadata> = None;
    let mut pdb_file_bytes = Vec::new();

    while let Some(field) = multipart
        .next_field()
        .await
        .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?
    {
        let name = field.name().unwrap_or("unknown").to_string();

        if name == "metadata" {
            let data = field
                .text()
                .await
                .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?;
            metadata = Some(
                serde_json::from_str(&data)
                    .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?,
            );
        } else if name == "pdb" {
            pdb_file_bytes = field
                .bytes()
                .await
                .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?
                .to_vec();

            // 验证文件类型 - PDB文件头应该是4字节的0x00000000或特定的PDB头
            // 简单检查：确保文件有内容
            // TODO 未来需要完善
            if pdb_file_bytes.is_empty() {
                return Err((StatusCode::BAD_REQUEST, "Invalid PDB file".to_string()));
            }
        }
    }

    let metadata = metadata.ok_or((StatusCode::BAD_REQUEST, "Missing metadata".to_string()))?;

    // 生成唯一ID和文件路径
    let pdb_id = Uuid::new_v4().to_string();
    let file_path = state.storage.get_pdb_path(
        &metadata.exe_version,
        &metadata.exe_guid,
        metadata.exe_age,
        &metadata.pdb_filename,
    );

    // 使用storage的save_file方法，该方法会确保父目录存在
    let file_path_clone = file_path.clone();
    let result = state
        .storage
        .save_file(
            &file_path_clone,
            std::io::Cursor::new(pdb_file_bytes),
            state.storage.get_max_pdb_size(),
        )
        .await;

    if let Err(e) = result {
        return Err((StatusCode::INTERNAL_SERVER_ERROR, e.to_string()));
    }

    // 创建Pdb记录
    let pdb = Pdb {
        id: pdb_id.clone(),
        exe_version: metadata.exe_version,
        exe_guid: metadata.exe_guid,
        exe_age: metadata.exe_age,
        pdb_filename: metadata.pdb_filename,
        file_path: file_path.to_string_lossy().to_string(),
        uploaded_at: Utc::now().to_rfc3339(),
    };

    // 保存到数据库
    state
        .db
        .save_pdb(&pdb)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    // 准备响应
    let response = ApiResponse::success(serde_json::json!({
        "pdb_id": pdb_id,
        "file_path": file_path.to_string_lossy().to_string()
    }));

    Ok((StatusCode::OK, Json(response)))
}

// 下载Minidump
async fn download_minidump(
    State(state): State<AppState>,
    Path(id): Path<String>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    // 从数据库获取minidump信息
    let minidump = state
        .db
        .get_minidump_by_id(&id)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?
        .ok_or((
            StatusCode::NOT_FOUND,
            format!("Minidump not found with ID: {}", id),
        ))?;

    // 检查文件是否存在
    let file_path = std::path::Path::new(&minidump.file_path);
    if !state.storage.file_exists(file_path) {
        return Err((
            StatusCode::NOT_FOUND,
            "Minidump file not found on disk".to_string(),
        ));
    }

    // 打开文件
    let file = File::open(file_path)
        .await
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    let stream = ReaderStream::new(file);
    let body = axum::body::Body::from_stream(stream);

    Ok(axum::response::Response::builder()
        .header(axum::http::header::CONTENT_TYPE, "application/octet-stream")
        .header(
            axum::http::header::CONTENT_DISPOSITION,
            format!("attachment; filename=\"{}.dmp\"", id),
        )
        .body(body)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?)
}

// 下载PDB
async fn download_pdb(
    State(state): State<AppState>,
    Path(id): Path<String>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    // 从数据库获取pdb信息
    let pdb = state
        .db
        .get_pdb_by_id(&id)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?
        .ok_or((
            StatusCode::NOT_FOUND,
            format!("PDB not found with ID: {}", id),
        ))?;

    // 检查文件是否存在
    let file_path = std::path::Path::new(&pdb.file_path);
    if !state.storage.file_exists(file_path) {
        return Err((
            StatusCode::NOT_FOUND,
            "PDB file not found on disk".to_string(),
        ));
    }

    // 打开文件
    let file = File::open(file_path)
        .await
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    let stream = ReaderStream::new(file);
    let body = axum::body::Body::from_stream(stream);

    Ok(axum::response::Response::builder()
        .header(axum::http::header::CONTENT_TYPE, "application/octet-stream")
        .header(
            axum::http::header::CONTENT_DISPOSITION,
            format!("attachment; filename=\"{}\"", pdb.pdb_filename),
        )
        .body(body)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?)
}

// 查询Minidump列表
async fn list_minidumps(
    State(state): State<AppState>,
    Query(params): Query<MinidumpQueryParams>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let page = params.page.unwrap_or(1).max(1);
    let page_size = params.page_size.unwrap_or(20).min(100);

    let minidumps = state
        .db
        .find_minidumps(
            params.version.as_deref(),
            params.guid.as_deref(),
            params.start_time,
            params.end_time,
            page,
            page_size,
        )
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    let response = ApiResponse::success(serde_json::json!({
        "minidumps": minidumps,
        "page": page,
        "page_size": page_size,
        "total": minidumps.len(), //TODO 实际应用中这里应该返回总数，而不是当前页数量
    }));

    Ok((StatusCode::OK, Json(response)))
}

// 根据Minidump的元数据查找匹配的PDB
#[derive(Deserialize)]
struct FindPdbParams {
    exe_version: String,
    exe_guid: String,
    exe_age: i32,
}

// 查找匹配的PDB文件
async fn find_matching_pdb(
    State(state): State<AppState>,
    Query(params): Query<FindPdbParams>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let pdb = state
        .db
        .find_matching_pdb(&params.exe_version, &params.exe_guid, params.exe_age)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    match pdb {
        Some(pdb) => {
            let response = ApiResponse::success(serde_json::json!({
                "pdb": pdb,
                "message": "Matching PDB found"
            }));
            Ok((StatusCode::OK, Json(response)))
        }
        None => {
            let response =
                ApiResponse::error("No matching PDB found for the given parameters".to_string());
            Ok((StatusCode::NOT_FOUND, Json(response)))
        }
    }
}
