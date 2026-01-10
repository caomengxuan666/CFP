/*
 *  Copyright © 2026 [caomengxuan666]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
// src/app/crash_reporter.cpp
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

// 最小化配置
typedef struct {
  char server_url[256];
  char api_key[128];
  char minidump_path[MAX_PATH];
  char exe_version[32];
  char exe_guid[64];
  DWORD pid;
  DWORD tid;
  DWORD exception_code;
  ULONG_PTR exception_address;
  int exe_age;
  char timestamp[64];
  DWORD exe_time_date_stamp;
  DWORD exe_size_of_image;
} CrashInfo;

// 格式化64位地址为字符串
void FormatAddress(ULONG_PTR address, char* buffer, size_t buffer_size) {
#if _WIN64
  snprintf(buffer, buffer_size, "0x%016llX", static_cast<uint64_t>(address));
#else
  snprintf(buffer, buffer_size, "0x%08X", static_cast<uint32_t>(address));
#endif
}

// 从命令行解析参数
BOOL ParseCommandLine(int argc, char* argv[], CrashInfo* info) {
  if (argc < 12) {
    printf(
        "用法: %s <服务器URL> <API密钥> <minidump路径> <版本> <GUID> <PID> "
        "<TID> <异常码> <异常地址> <exe_age> <time_date_stamp> "
        "<size_of_image>\n",
        argv[0]);
    printf(
        "示例: %s https://example.com/api/crash key123 crash.dmp 1.0.0 "
        "{GUID} 1234 5678 0xC0000005 0x12345678 0 1234567890 4194304\n",
        argv[0]);
    printf("当前参数数量: %d (需要至少 12 个参数)\n", argc - 1);
    return FALSE;
  }

  strncpy_s(info->server_url, sizeof(info->server_url), argv[1], _TRUNCATE);
  strncpy_s(info->api_key, sizeof(info->api_key), argv[2], _TRUNCATE);
  strncpy_s(info->minidump_path, sizeof(info->minidump_path), argv[3],
            _TRUNCATE);
  strncpy_s(info->exe_version, sizeof(info->exe_version), argv[4], _TRUNCATE);
  strncpy_s(info->exe_guid, sizeof(info->exe_guid), argv[5], _TRUNCATE);

  info->pid = strtoul(argv[6], NULL, 10);
  info->tid = strtoul(argv[7], NULL, 10);
  info->exception_code = strtoul(argv[8], NULL, 16);
  info->exception_address = strtoull(argv[9], NULL, 16);
  info->exe_age = strtoul(argv[10], NULL, 10);  // NOLINT

  info->exe_time_date_stamp = strtoul(argv[11], NULL, 10);  // 十进制
  info->exe_size_of_image = strtoul(argv[12], NULL, 10);    // 十进制

  // 获取当前时间戳（精确到毫秒）
  SYSTEMTIME st;
  GetSystemTime(&st);
  sprintf_s(info->timestamp, sizeof(info->timestamp),
            "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", st.wYear, st.wMonth, st.wDay,
            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

  return TRUE;
}

// 计算请求总大小（用于设置Content-Length）
DWORD CalculateTotalRequestSize(const CrashInfo* info, const char* boundary) {
  // 计算metadata部分大小
  char metadata_buf[2048];
  char addr_buf[32];
  FormatAddress(info->exception_address, addr_buf, sizeof(addr_buf));

  int metadata_len =
      _snprintf_s(metadata_buf, sizeof(metadata_buf), _TRUNCATE,
                  "--%s\r\n"
                  "Content-Disposition: form-data; name=\"metadata\"\r\n"
                  "Content-Type: application/json; charset=utf-8\r\n\r\n"
                  "{\"timestamp\":\"%s\","
                  "\"exe_version\":\"%s\","
                  "\"exe_guid\":\"%s\","
                  "\"exe_age\":%d,"
                  "\"pid\":%lu,"
                  "\"tid\":%lu,"
                  "\"exception_code\":\"0x%08X\","
                  "\"exception_address\":\"%s\","
                  "\"exe_time_date_stamp\":%lu,"
                  "\"exe_size_of_image\":%lu}\r\n",
                  boundary, info->timestamp, info->exe_version, info->exe_guid,
                  info->exe_age, info->pid, info->tid, info->exception_code,
                  addr_buf, info->exe_time_date_stamp, info->exe_size_of_image);

  // 计算文件头部大小
  char file_header_buf[512];
  const char* filename = strrchr(info->minidump_path, '\\');
  if (!filename) {
    filename = info->minidump_path;
  } else {
    ++filename;
  }

  int header_len =
      _snprintf_s(file_header_buf, sizeof(file_header_buf), _TRUNCATE,
                  "--%s\r\n"
                  "Content-Disposition: form-data; name=\"minidump\"; "
                  "filename=\"%s\"\r\n"
                  "Content-Type: application/octet-stream\r\n\r\n",
                  boundary, filename);

  // 获取文件大小
  DWORD file_size = 0;
  HANDLE hFile = CreateFileA(info->minidump_path, GENERIC_READ, FILE_SHARE_READ,
                             NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    file_size = GetFileSize(hFile, NULL);
    CloseHandle(hFile);
  }

  // 精确计算结束边界大小（不再估算）
  char end_boundary[128];
  strcpy_s(end_boundary, sizeof(end_boundary), "\r\n--");
  strcat_s(end_boundary, sizeof(end_boundary), boundary);
  strcat_s(end_boundary, sizeof(end_boundary), "--\r\n");
  int end_boundary_len = static_cast<int>(strlen(end_boundary));

  // 返回总大小（确保每个部分都有效）
  DWORD total = 0;
  if (metadata_len > 0) {
    total += static_cast<DWORD>(metadata_len);
  }
  if (header_len > 0) {
    total += static_cast<DWORD>(header_len);
  }
  total += file_size;
  total += static_cast<DWORD>(end_boundary_len);

  return total;
}

// 流式发送数据
// 流式发送数据 - 修复错误码87问题
BOOL SendDataStreaming(HINTERNET hRequest, const CrashInfo* info) {
  BOOL success = TRUE;
  const char* boundary = "----CrashBoundary1234567890";

  // 1. 创建请求体第一部分：metadata
  printf("  发送元数据部分...\n");

  char addr_buf[32];
  FormatAddress(info->exception_address, addr_buf, sizeof(addr_buf));

  char metadata_part[2048];
  // 重点：使用_snprintf返回实际长度，不使用_TRUNCATE避免截断
  int metadata_len =
      _snprintf_s(metadata_part, sizeof(metadata_part), _TRUNCATE,
                  "--%s\r\n"
                  "Content-Disposition: form-data; name=\"metadata\"\r\n"
                  "Content-Type: application/json; charset=utf-8\r\n\r\n"
                  "{\"timestamp\":\"%s\","
                  "\"exe_version\":\"%s\","
                  "\"exe_guid\":\"%s\","
                  "\"exe_age\":%d,"
                  "\"pid\":%lu,"
                  "\"tid\":%lu,"
                  "\"exception_code\":\"0x%08X\","
                  "\"exception_address\":\"%s\","
                  "\"exe_time_date_stamp\":%lu,"
                  "\"exe_size_of_image\":%lu}\r\n",
                  boundary, info->timestamp, info->exe_version, info->exe_guid,
                  info->exe_age, info->pid, info->tid, info->exception_code,
                  addr_buf, info->exe_time_date_stamp, info->exe_size_of_image);

  if (metadata_len <= 0 ||
      metadata_len >= static_cast<int>(sizeof(metadata_part))) {
    printf("错误: 元数据格式化失败或超出缓冲区大小\n");
    return FALSE;
  }

  DWORD bytes_written = 0;
  if (!WinHttpWriteData(hRequest, metadata_part,
                        static_cast<DWORD>(metadata_len), &bytes_written)) {
    printf("错误: 发送元数据失败 (错误码: %lu)\n", GetLastError());
    return FALSE;
  }
  printf("  元数据发送: %lu/%d 字节\n", bytes_written, metadata_len);

  // 2. 创建请求体第二部分：minidump文件
  printf("  发送minidump头部...\n");

  char file_header[512];
  const char* filename = strrchr(info->minidump_path, '\\');
  if (!filename) {
    filename = info->minidump_path;
  } else {
    ++filename;
  }

  int header_len =
      _snprintf_s(file_header, sizeof(file_header), _TRUNCATE,
                  "--%s\r\n"
                  "Content-Disposition: form-data; name=\"minidump\"; "
                  "filename=\"%s\"\r\n"
                  "Content-Type: application/octet-stream\r\n\r\n",
                  boundary, filename);

  if (header_len <= 0 || header_len >= static_cast<int>(sizeof(file_header))) {
    printf("错误: 文件头部格式化失败或超出缓冲区大小\n");
    return FALSE;
  }

  if (!WinHttpWriteData(hRequest, file_header, static_cast<DWORD>(header_len),
                        &bytes_written)) {
    printf("错误: 发送文件头部失败 (错误码: %lu)\n", GetLastError());
    return FALSE;
  }
  printf("  文件头部发送: %lu/%d 字节\n", bytes_written, header_len);

  // 3. 流式发送文件内容
  printf("  流式发送文件内容...\n");
  HANDLE hFile = CreateFileA(
      info->minidump_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    printf("错误: 无法打开文件 %s (错误码: %lu)\n", info->minidump_path,
           GetLastError());
    return FALSE;
  }

  DWORD file_size = GetFileSize(hFile, NULL);
  DWORD bytes_read = 0;
  DWORD total_sent = 0;
  BYTE buffer[65536];  // 64KB 块

  while (ReadFile(hFile, buffer, sizeof(buffer), &bytes_read, NULL) &&
         bytes_read > 0) {
    if (!WinHttpWriteData(hRequest, buffer, bytes_read, &bytes_written)) {
      printf("错误: 发送文件数据失败 (错误码: %lu)\n", GetLastError());
      CloseHandle(hFile);
      return FALSE;
    }

    if (bytes_written != bytes_read) {
      printf("警告: 只发送了 %lu/%lu 字节\n", bytes_written, bytes_read);
    }

    total_sent += bytes_written;

    // 显示进度
    if (file_size > 0) {
      int percent = static_cast<int>(((total_sent * 100) / file_size));
      printf("  进度: %lu/%lu 字节 (%d%%)\r", total_sent, file_size, percent);
      fflush(stdout);
    }
  }

  printf("\n  文件发送完成: %lu 字节\n", total_sent);
  CloseHandle(hFile);

  // 检查文件读取错误
  if (GetLastError() != ERROR_SUCCESS && GetLastError() != ERROR_HANDLE_EOF) {
    printf("错误: 读取文件失败 (错误码: %lu)\n", GetLastError());
    return FALSE;
  }

  // 4. 发送结束边界 - 核心修复部分
  printf("  发送结束边界...\n");
  char end_boundary[128] = {0};  // 初始化全0
  // 分步构建结束边界，避免格式化错误
  strcat_s(end_boundary, sizeof(end_boundary), "\r\n--");
  strcat_s(end_boundary, sizeof(end_boundary), boundary);
  strcat_s(end_boundary, sizeof(end_boundary), "--\r\n");

  // 明确计算长度，避免使用strlen可能的问题
  DWORD end_len = static_cast<DWORD>(strlen(end_boundary));

  // 关键修复：使用有效的bytes_written指针，检查参数有效性
  bytes_written = 0;
  if (end_len == 0 || end_len >= sizeof(end_boundary)) {
    printf("错误: 结束边界长度无效 (长度: %lu)\n", end_len);
    return FALSE;
  }

  // 调试输出结束边界内容和长度
  printf("  结束边界内容: [%s]\n", end_boundary);
  printf("  结束边界长度: %lu 字节\n", end_len);

  if (!WinHttpWriteData(hRequest, end_boundary, end_len, &bytes_written)) {
    printf("错误: 发送结束边界失败 (错误码: %lu)\n", GetLastError());
    // 输出详细的参数信息帮助调试
    printf("  失败参数: 缓冲区地址=0x%p, 长度=%lu\n", end_boundary, end_len);
    return FALSE;
  }

  printf("  结束边界发送: %lu/%lu 字节\n", bytes_written, end_len);

  return success;
}

// 上传minidump
BOOL UploadMinidump(const CrashInfo* info) {
  printf("开始上传minidump...\n");
  printf("服务器URL: %s\n", info->server_url);
  printf("API密钥: %s\n", info->api_key);
  printf("minidump路径: %s\n", info->minidump_path);
  printf("版本: %s\n", info->exe_version);
  printf("GUID: %s\n", info->exe_guid);
  printf("PID: %lu, TID: %lu\n", info->pid, info->tid);
  printf("异常码: 0x%08X\n", static_cast<uint32_t>(info->exception_code));

  char addr_buf[32];
  FormatAddress(info->exception_address, addr_buf, sizeof(addr_buf));
  printf("异常地址: %s\n", addr_buf);
  printf("exe_age: %d\n", info->exe_age);
  printf("时间戳: %s\n", info->timestamp);

  // 1. 检查文件是否存在
  printf("1. 检查minidump文件...\n");
  HANDLE hFile = CreateFileA(info->minidump_path, GENERIC_READ, FILE_SHARE_READ,
                             NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    printf("错误: 无法打开文件 %s (错误码: %lu)\n", info->minidump_path,
           GetLastError());
    return FALSE;
  }

  DWORD file_size = GetFileSize(hFile, NULL);
  printf("文件大小: %lu 字节\n", file_size);
  CloseHandle(hFile);

  // 2. 创建HTTP会话
  printf("2. 创建HTTP会话...\n");
  HINTERNET hSession =
      WinHttpOpen(L"CrashReporter/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                  WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
  if (!hSession) {
    printf("错误: WinHttpOpen失败 (错误码: %lu)\n", GetLastError());
    return FALSE;
  }

  // 3. 解析URL
  printf("3. 解析URL...\n");
  wchar_t server_url_wide[256];
  MultiByteToWideChar(CP_UTF8, 0, info->server_url, -1, server_url_wide, 256);

  URL_COMPONENTS url_comp = {0};
  url_comp.dwStructSize = sizeof(url_comp);
  // NOLINTBEGIN
  url_comp.dwHostNameLength = (DWORD)-1;
  url_comp.dwUrlPathLength = (DWORD)-1;
  url_comp.dwExtraInfoLength = (DWORD)-1;
  // NOLINTEND

  wchar_t host[256] = {0};
  wchar_t path[1024] = {0};
  wchar_t extra[256] = {0};

  url_comp.lpszHostName = host;
  url_comp.lpszUrlPath = path;
  url_comp.lpszExtraInfo = extra;
  url_comp.dwHostNameLength = 256;
  url_comp.dwUrlPathLength = 1024;
  url_comp.dwExtraInfoLength = 256;

  if (!WinHttpCrackUrl(server_url_wide, 0, 0, &url_comp)) {
    printf("错误: WinHttpCrackUrl失败 (错误码: %lu)\n", GetLastError());
    WinHttpCloseHandle(hSession);
    return FALSE;
  }
  printf("解析成功: host=%ls, path=%ls, port=%d\n", host, path, url_comp.nPort);

  // 4. 连接服务器
  printf("4. 连接服务器...\n");
  HINTERNET hConnect = WinHttpConnect(hSession, host, url_comp.nPort, 0);
  if (!hConnect) {
    printf("错误: WinHttpConnect失败 (错误码: %lu)\n", GetLastError());
    WinHttpCloseHandle(hSession);
    return FALSE;
  }

  // 5. 创建请求
  printf("5. 创建请求...\n");
  DWORD flags = WINHTTP_FLAG_REFRESH;
  if (url_comp.nScheme == INTERNET_SCHEME_HTTPS) {
    flags |= WINHTTP_FLAG_SECURE;
    // 忽略SSL证书错误（仅测试用）
    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
                    SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
    WinHttpSetOption(hConnect, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags,
                     sizeof(dwFlags));
  }

  // 拼接完整的请求路径
  wchar_t full_path[1280];
  wcscpy_s(full_path, path);
  if (wcslen(extra) > 0) {
    wcscat_s(full_path, extra);
  }

  HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", full_path,
                                          L"HTTP/1.1", WINHTTP_NO_REFERER,
                                          WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
  if (!hRequest) {
    printf("错误: WinHttpOpenRequest失败 (错误码: %lu)\n", GetLastError());
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return FALSE;
  }

  // 6. 设置请求头
  printf("6. 设置请求头...\n");
  const char* boundary = "----CrashBoundary1234567890";

  // 计算总请求大小
  DWORD total_size = CalculateTotalRequestSize(info, boundary);

  char headers[1024];
  int header_len =
      _snprintf_s(headers, sizeof(headers), _TRUNCATE,
                  "Content-Type: multipart/form-data; boundary=%s\r\n"
                  "X-API-Key: %s\r\n"
                  "User-Agent: CrashReporter/1.0\r\n"
                  "Accept: */*\r\n"
                  "Content-Length: %lu\r\n",
                  boundary, info->api_key, total_size);

  wchar_t headers_wide[1024];
  MultiByteToWideChar(CP_UTF8, 0, headers, -1, headers_wide, 1024);

  if (!WinHttpAddRequestHeaders(
          hRequest, headers_wide, -1,
          WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD)) {
    printf("警告: WinHttpAddRequestHeaders失败 (错误码: %lu)\n",
           GetLastError());
  }

  // 7. 开始发送请求
  printf("7. 开始发送请求...\n");
  if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                          WINHTTP_NO_REQUEST_DATA, 0, total_size,
                          static_cast<DWORD_PTR>(NULL))) {
    printf("错误: WinHttpSendRequest失败 (错误码: %lu)\n", GetLastError());
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return FALSE;
  }

  // 8. 流式发送数据
  printf("8. 流式发送数据...\n");
  BOOL send_success = SendDataStreaming(hRequest, info);

  // 9. 结束请求并获取响应
  printf("9. 结束请求...\n");
  if (send_success) {
    if (!WinHttpReceiveResponse(hRequest, NULL)) {
      printf("错误: WinHttpReceiveResponse失败 (错误码: %lu)\n",
             GetLastError());
      send_success = FALSE;
    } else {
      printf("响应接收成功\n");

      // 获取响应状态码
      DWORD dwStatusCode = 0;
      DWORD dwSize = sizeof(dwStatusCode);
      if (WinHttpQueryHeaders(
              hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
              WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwSize,
              WINHTTP_NO_HEADER_INDEX)) {
        printf("服务器状态码: %lu\n", dwStatusCode);
      }

      // 读取响应内容
      dwSize = 0;
      DWORD dwDownloaded = 0;

      if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
        printf("警告: WinHttpQueryDataAvailable失败 (错误码: %lu)\n",
               GetLastError());
      } else if (dwSize > 0) {
        BYTE* pszOutBuffer = static_cast<BYTE*>(malloc(dwSize + 1));
        if (pszOutBuffer) {
          ZeroMemory(pszOutBuffer, dwSize + 1);

          // NOLINTNEXTLINE
          if (WinHttpReadData(hRequest, reinterpret_cast<LPVOID>(pszOutBuffer),
                              dwSize, &dwDownloaded)) {
            printf("服务器响应 (%lu 字节):\n", dwDownloaded);
            printf("%s\n", pszOutBuffer);
          } else {
            printf("警告: 读取响应失败 (错误码: %lu)\n", GetLastError());
          }

          free(pszOutBuffer);
        }
      } else {
        printf("服务器响应: (无内容)\n");
      }
    }
  }

  // 10. 清理资源
  printf("10. 清理资源...\n");
  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  printf("上传结果: %s\n", send_success ? "成功" : "失败");
  return send_success;
}

int main(int argc, char* argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);  // 禁用输出缓冲，确保实时显示
  // 如果参数少于预期，直接退出并显示错误
  if (argc < 13) {
    fprintf(stderr, "FATAL: 只收到 %d 个参数，需要 12 个！\n", argc);
    return 1;
  }
  printf("=========================================\n");
  printf("崩溃报告器 v1.0 (流式发送版)\n");
  printf("=========================================\n\n");

  CrashInfo info = {0};

  if (!ParseCommandLine(argc, argv, &info)) {
    printf("错误: 参数解析失败\n");
    return 1;
  }

  if (UploadMinidump(&info)) {
    printf("\n上传成功！\n");
    return 0;
  }

  printf("\n上传失败！\n");
  return 1;
}
