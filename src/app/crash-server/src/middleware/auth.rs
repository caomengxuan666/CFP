use axum::{
    body::Body,
    extract::State,
    http::{Request, StatusCode},
    middleware::Next,
    response::Response,
    Json,
};

use crate::{api::ApiResponse, api::AppState};

pub async fn api_key_auth(
    State(state): State<AppState>,
    request: Request<Body>,
    next: Next,
) -> Result<Response, (StatusCode, Json<ApiResponse<()>>)> {
    // 健康检查不需要认证
    if request.uri().path() == "/health" {
        return Ok(next.run(request).await);
    }

    // 检查是否有API密钥配置
    match &state.config.server.api_key {
        Some(expected_api_key) => {
            // 从请求头中获取API密钥
            let api_key = request
                .headers()
                .get("X-API-Key")
                .and_then(|value| value.to_str().ok());

            // 验证API密钥
            match api_key {
                Some(key) if key == expected_api_key => Ok(next.run(request).await),
                _ => {
                    let response =
                        ApiResponse::error("Unauthorized: Invalid or missing API key".to_string());
                    Err((StatusCode::UNAUTHORIZED, Json(response)))
                }
            }
        }
        None => {
            // 没有配置API密钥，直接通过
            Ok(next.run(request).await)
        }
    }
}
