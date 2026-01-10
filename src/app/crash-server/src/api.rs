use axum::http::{HeaderName, HeaderValue, Method};
use axum::{
    extract::{DefaultBodyLimit, Multipart, Path, Query, State},
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
use tower_http::cors::CorsLayer;
use tower_http::limit::RequestBodyLimitLayer;
use uuid::Uuid;

use crate::{
    config::Settings,
    database::{Database, Exe, Minidump, Pdb}, // 添加Exe导入
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
    #[serde(default)]
    exe_time_date_stamp: Option<u32>,
    #[serde(default)]
    exe_size_of_image: Option<u32>,
}

// PDB上传元数据
#[derive(Deserialize)]
struct PdbUploadMetadata {
    exe_version: String,
    exe_guid: String,
    exe_age: i32,
    pdb_filename: String,
    #[serde(default)]
    exe_time_date_stamp: Option<u32>,
    #[serde(default)]
    exe_size_of_image: Option<u32>,
}

// EXE上传元数据
#[derive(Deserialize)]
struct ExeUploadMetadata {
    exe_version: String,
    exe_guid: String,
    exe_filename: String,
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

    axum::serve(listener, app).await?;

    Ok(())
}

// 创建路由应用
pub fn create_app(state: AppState) -> Router {
    let cors = CorsLayer::new()
        .allow_origin([
            HeaderValue::from_static("http://localhost:8081"),
            HeaderValue::from_static("http://127.0.0.1:8081"),
            HeaderValue::from_static("http://localhost:8082"),
            HeaderValue::from_static("http://127.0.0.1:8082"),
        ])
        // 允许的请求方法
        .allow_methods([
            Method::GET,
            Method::POST,
            Method::PUT,
            Method::DELETE,
            Method::OPTIONS, // 必须允许OPTIONS预检请求
        ])
        // 允许的请求头
        .allow_headers([
            HeaderName::from_static("content-type"),
            HeaderName::from_static("authorization"),
            HeaderName::from_static("x-api-key"), // 添加对x-api-key头部的支持
            HeaderName::from_static("api-key"),
        ])
        // 允许携带凭证（如cookie）
        .allow_credentials(true);

    Router::new()
        // 健康检查
        .route("/health", get(health_check))
        // 上传接口
        .route("/upload/minidump", post(upload_minidump))
        .route("/upload/pdb", post(upload_pdb))
        .route("/upload/exe", post(upload_exe)) // 添加EXE上传接口
        // 下载接口
        .route("/download/minidump/:id", get(download_minidump))
        .route("/download/pdb/:id", get(download_pdb))
        .route("/download/exe/:id", get(download_exe)) // 添加EXE下载接口
        // 查询接口
        .route("/list/minidumps", get(list_minidumps))
        .route("/find/matching-pdb", get(find_matching_pdb))
        .route("/find/exe-by-guid", get(find_exe_by_guid)) // 添加通过GUID查找EXE的接口
        //TODO(cmx) 后面可以给这些接口添加crashpad风格的路由别名
        // 中间件（API密钥验证）
        .layer(axum::middleware::from_fn_with_state(
            state.clone(),
            crate::middleware::auth::api_key_auth,
        ))
        .layer(cors)
        // 全局禁用默认限制
        .layer(DefaultBodyLimit::disable())
        // 设置自定义限制(250MB)
        .layer(RequestBodyLimitLayer::new(250 * 1024 * 1024))
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
// 流式上传Minidump
async fn upload_minidump(
    State(state): State<AppState>,
    mut multipart: Multipart,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let mut metadata: Option<MinidumpUploadMetadata> = None;
    let mut minidump_id = None;
    let mut file_path = None;
    let mut is_minidump_valid = false;
    let mut total_chunks = 0;

    while let Some(mut field) = multipart
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
                    .map_err(|e| (StatusCode::BAD_REQUEST, format!("元数据格式错误: {}", e)))?,
            );
        } else if name == "minidump" {
            // 验证元数据已存在
            let meta = metadata.as_ref().ok_or((
                StatusCode::BAD_REQUEST,
                "Metadata must be provided before minidump file".to_string(),
            ))?;

            // 生成文件ID和路径
            let id = Uuid::new_v4().to_string();
            let timestamp_str = meta
                .timestamp
                .replace("-", "")
                .replace(":", "")
                .replace("T", "")
                .replace("Z", "");
            let filename = format!("crash_{}_{}_{}.dmp", timestamp_str, meta.pid, meta.tid);
            let path = state.storage.get_minidump_path(
                &meta.exe_version,
                &meta.exe_guid,
                meta.exe_age,
                &filename,
            );

            minidump_id = Some(id.clone());
            file_path = Some(path.clone());

            // 创建文件（准备流式写入）
            let mut file_handle = tokio::fs::File::create(&path).await.map_err(|e| {
                (
                    StatusCode::INTERNAL_SERVER_ERROR,
                    format!("Failed to create file: {}", e),
                )
            })?;

            // 流式读取文件块并写入磁盘
            let mut total_size: u64 = 0;
            let max_size = state.storage.get_max_minidump_size();

            while let Some(chunk) = field
                .chunk()
                .await
                .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?
            {
                total_chunks += 1;
                // 检查文件大小限制
                total_size += chunk.len() as u64;

                if total_chunks % 100 == 0 {
                    // 每100个chunk打印一次日志
                }

                if total_size > max_size {
                    // 清理超出限制的文件
                    let _ = tokio::fs::remove_file(&path).await;
                    return Err((
                        StatusCode::PAYLOAD_TOO_LARGE,
                        format!(
                            "Minidump file exceeds maximum size of {} bytes (current: {} bytes)",
                            max_size, total_size
                        ),
                    ));
                }

                // 验证Minidump文件头（仅在第一个chunk检查）
                if total_size == chunk.len() as u64 && chunk.len() >= 4 {
                    let header = &chunk[0..4];
                    if header == b"MDMP" {
                        // 文件有效
                        is_minidump_valid = true;
                    } else {
                        // 清理无效文件
                        let _ = tokio::fs::remove_file(&path).await;
                        return Err((
                            StatusCode::BAD_REQUEST,
                            "File is not a valid minidump (invalid header)".to_string(),
                        ));
                    }
                }

                // 写入当前chunk到磁盘（流式核心）
                file_handle.write_all(&chunk)
                    .await
                    .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;
            }

            // 确保所有数据写入磁盘
            file_handle.sync_all()
                .await
                .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

            // 验证文件有效性（空文件检查）
            if !is_minidump_valid || total_size < 4 {
                let _ = tokio::fs::remove_file(&path).await;
                return Err((
                    StatusCode::BAD_REQUEST,
                    "Invalid or empty minidump file".to_string(),
                ));
            }

        }
    }

    // 验证必要数据
    let metadata = metadata.ok_or((StatusCode::BAD_REQUEST, "Missing metadata".to_string()))?;
    let minidump_id =
        minidump_id.ok_or((StatusCode::BAD_REQUEST, "Missing minidump file".to_string()))?;
    let file_path =
        file_path.ok_or((StatusCode::BAD_REQUEST, "Missing minidump file".to_string()))?;

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
        exe_time_date_stamp: metadata.exe_time_date_stamp,
        exe_size_of_image: metadata.exe_size_of_image,
    };

    // 保存到数据库
    match state.db.save_minidump(&minidump) {
        Ok(_) => {}
        Err(e) => {
            // 打印完整的错误链
            let mut error_msg = format!("Database error: {}", e);
            let mut source = e.source();
            while let Some(src) = source {
                error_msg.push_str(&format!(" -> {}", src));
                source = src.source();
            }
            return Err((StatusCode::INTERNAL_SERVER_ERROR, error_msg));
        }
    }

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
        exe_time_date_stamp: metadata.exe_time_date_stamp,
        exe_size_of_image: metadata.exe_size_of_image,
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

// 新增EXE上传功能
async fn upload_exe(
    State(state): State<AppState>,
    mut multipart: Multipart,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let mut metadata: Option<ExeUploadMetadata> = None;
    let mut exe_file_bytes = Vec::new();

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
        } else if name == "exe" {
            exe_file_bytes = field
                .bytes()
                .await
                .map_err(|e| (StatusCode::BAD_REQUEST, e.to_string()))?
                .to_vec();

            // 验证文件类型 - 检查是否为有效的PE文件头
            if exe_file_bytes.len() < 2 {
                return Err((StatusCode::BAD_REQUEST, "Invalid EXE file".to_string()));
            }
            let header = &exe_file_bytes[0..2];
            if header != b"MZ" {
                return Err((
                    StatusCode::BAD_REQUEST,
                    "File is not a valid EXE".to_string(),
                ));
            }
        }
    }

    let metadata = metadata.ok_or((StatusCode::BAD_REQUEST, "Missing metadata".to_string()))?;

    // 生成唯一ID和文件路径
    let exe_id = Uuid::new_v4().to_string();
    let file_path = state.storage.get_exe_path(
        &metadata.exe_version,
        &metadata.exe_guid,
        &metadata.exe_filename,
    );

    // 使用storage的save_file方法，该方法会确保父目录存在
    let result = state
        .storage
        .save_file(
            &file_path,
            std::io::Cursor::new(exe_file_bytes),
            state.storage.get_max_pdb_size(), // EXE文件大小限制与PDB相同
        )
        .await;

    if let Err(e) = result {
        return Err((StatusCode::INTERNAL_SERVER_ERROR, e.to_string()));
    }

    // 创建Exe记录
    let exe = Exe {
        id: exe_id.clone(),
        exe_version: metadata.exe_version,
        exe_guid: metadata.exe_guid,
        exe_filename: metadata.exe_filename,
        file_path: file_path.to_string_lossy().to_string(),
        uploaded_at: Utc::now().to_rfc3339(),
    };

    // 保存到数据库
    state
        .db
        .save_exe(&exe)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    // 准备响应
    let response = ApiResponse::success(serde_json::json!({
        "exe_id": exe_id,
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

// 新增EXE下载功能
async fn download_exe(
    State(state): State<AppState>,
    Path(id): Path<String>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    // 从数据库获取exe信息
    let exe = state
        .db
        .get_exe_by_id(&id)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?
        .ok_or((
            StatusCode::NOT_FOUND,
            format!("EXE not found with ID: {}", id),
        ))?;

    // 检查文件是否存在
    let file_path = std::path::Path::new(&exe.file_path);
    if !state.storage.file_exists(file_path) {
        return Err((
            StatusCode::NOT_FOUND,
            "EXE file not found on disk".to_string(),
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
            format!("attachment; filename=\"{}\"", exe.exe_filename),
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

    // 获取符合条件的总数
    let total = state
        .db
        .count_minidumps(
            params.version.as_deref(),
            params.guid.as_deref(),
            params.start_time,
            params.end_time,
        )
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    let response = ApiResponse::success(serde_json::json!({
        "minidumps": minidumps,
        "page": page,
        "page_size": page_size,
        "total": total, // 现在返回实际总数
        "pages": ((total as f64) / (page_size as f64)).ceil() as u32 // 可选：返回总页数
    }));

    Ok((StatusCode::OK, Json(response)))
}

// 根据Minidump的元数据查找匹配的PDB
#[derive(Deserialize)]
struct FindPdbParams {
    exe_version: Option<String>,
    exe_guid: Option<String>,
    exe_age: Option<i32>,
    exe_time_date_stamp: Option<u32>,
    exe_size_of_image: Option<u32>,
}

async fn find_matching_pdb(
    State(state): State<AppState>,
    Query(params): Query<FindPdbParams>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    // 先尝试权威匹配
    if let (Some(ts), Some(size)) = (params.exe_time_date_stamp, params.exe_size_of_image) {
        match state.db.find_pdb_by_pe_info(ts, size) {
            Ok(Some(pdb)) => {
                let response = ApiResponse::success(serde_json::json!({
                    "pdb": pdb,
                    "message": "Matching PDB found (by PE info)"
                }));
                return Ok((StatusCode::OK, Json(response)));
            }
            Ok(None) => {
                // 权威匹配失败，继续尝试回退
            }
            Err(e) => {
                return Err((StatusCode::INTERNAL_SERVER_ERROR, e.to_string()));
            }
        }
    }

    // 回退到 GUID + Age 匹配
    if let (Some(guid), Some(age)) = (params.exe_guid, params.exe_age) {
        match state
            .db
            .find_matching_pdb(&params.exe_version.unwrap_or_default(), &guid, age)
        {
            Ok(Some(pdb)) => {
                let response = ApiResponse::success(serde_json::json!({
                    "pdb": pdb,
                    "message": "Matching PDB found (by GUID+Age)"
                }));
                return Ok((StatusCode::OK, Json(response)));
            }
            Ok(None) => {}
            Err(e) => {
                return Err((StatusCode::INTERNAL_SERVER_ERROR, e.to_string()));
            }
        }
    }

    // 都没找到
    let response = ApiResponse::error("No matching PDB found".to_string());
    Ok((StatusCode::NOT_FOUND, Json(response)))
}

// 根据EXE的GUID查找EXE文件
#[derive(Deserialize)]
struct FindExeByGuidParams {
    exe_guid: String,
}

// 查找匹配的EXE文件
async fn find_exe_by_guid(
    State(state): State<AppState>,
    Query(params): Query<FindExeByGuidParams>,
) -> Result<impl IntoResponse, (StatusCode, String)> {
    let exe = state
        .db
        .find_exe_by_guid(&params.exe_guid)
        .map_err(|e| (StatusCode::INTERNAL_SERVER_ERROR, e.to_string()))?;

    match exe {
        Some(exe) => {
            let response = ApiResponse::success(serde_json::json!({
                "exe": exe,
                "message": "Matching EXE found"
            }));
            Ok((StatusCode::OK, Json(response)))
        }
        None => {
            let response =
                ApiResponse::error("No matching EXE found for the given GUID".to_string());
            Ok((StatusCode::NOT_FOUND, Json(response)))
        }
    }
}
