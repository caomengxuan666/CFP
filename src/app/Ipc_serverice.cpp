/*
 *  Copyright © 2026 [caomengxuan666]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND.
 */

// NOLINTBEGIN
#include <tchar.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "asio.hpp"  // NOLINT

// asio must come before windows.h
#include <windows.h>

#include "spdlog/spdlog.h"
//we must include spdlog before concrete module
#include "spdlog/sinks/udp_sink.h"
// NOLINTEND

#define SERVICE_NAME _T("IpcRelayService")

// ============================
// Windows Service Globals
// ============================

SERVICE_STATUS g_ServiceStatus{};
SERVICE_STATUS_HANDLE g_StatusHandle = nullptr;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

static std::atomic<bool> g_Running{true};

// ============================
// Forward Declarations
// ============================

void ServiceMain(DWORD argc, LPTSTR* argv);
void ServiceCtrlHandler(DWORD ctrl);

// ============================
// Relay Logger (UDP only)
// ============================

std::shared_ptr<spdlog::logger> createRelayLogger(
    const std::string& remote_host, uint16_t remote_port) {
  spdlog::sinks::udp_sink_config cfg{remote_host, remote_port};
  auto sink = std::make_shared<spdlog::sinks::udp_sink_st>(cfg);

  auto logger = std::make_shared<spdlog::logger>("ipc-relay", sink);

  // ⚠ 关键点：只转发内容，不二次格式化
  logger->set_pattern("%v");
  logger->set_level(spdlog::level::info);
  logger->flush_on(spdlog::level::off);

  return logger;
}

// ============================
// TCP Relay Server
// ============================

void runTcpServer(const std::string& listen_ip, uint16_t listen_port,
                  const std::shared_ptr<spdlog::logger>& relay_logger) {
  try {
    asio::io_context io;
    asio::ip::tcp::acceptor acceptor(
        io, asio::ip::tcp::endpoint(asio::ip::make_address(listen_ip),
                                    listen_port));

    while (g_Running.load()) {
      asio::ip::tcp::socket socket(io);
      acceptor.accept(socket);

      std::thread([s = std::move(socket), relay_logger]() mutable {
        char buf[8192];
        while (true) {
          std::error_code ec;
          size_t len = s.read_some(asio::buffer(buf), ec);
          if (ec || len == 0) {
            break;
          }

          // 原样转发，不 reinterpret
          relay_logger->info(std::string_view(buf, len));
        }
      }).detach();
    }
  } catch (const std::exception& e) {
    std::cerr << "[IPC][TCP] fatal error: " << e.what() << std::endl;
  }
}

// ============================
// UDP Relay Server
// ============================

void runUdpServer(const std::string& listen_ip, uint16_t listen_port,
                  const std::shared_ptr<spdlog::logger>& relay_logger) {
  try {
    asio::io_context io;
    asio::ip::udp::socket socket(
        io, asio::ip::udp::endpoint(asio::ip::make_address(listen_ip),
                                    listen_port));

    char buf[8192];
    asio::ip::udp::endpoint sender;

    while (g_Running.load()) {
      size_t len = socket.receive_from(asio::buffer(buf), sender);
      if (len == 0) {
        continue;
      }

      relay_logger->info(std::string_view(buf, len));
    }
  } catch (const std::exception& e) {
    std::cerr << "[IPC][UDP] fatal error: " << e.what() << std::endl;
  }
}

// ============================
// Console Mode
// ============================

void runConsoleMode() {
  std::cout << "[IPC] relay starting (console mode)\n";

  auto relay_logger = createRelayLogger("127.0.0.1", 514);

  std::thread tcp_thread(runTcpServer, "127.0.0.1", 5140, relay_logger);
  std::thread udp_thread(runUdpServer, "127.0.0.1", 5141, relay_logger);

  tcp_thread.join();
  udp_thread.join();
}

// ============================
// Windows Service Entry
// ============================

int _tmain(int argc, TCHAR* argv[]) {
  if (argc > 1 && (_tcscmp(argv[1], _T("--console")) == 0 ||
                   _tcscmp(argv[1], _T("-c")) == 0)) {
    runConsoleMode();
    return 0;
  }

  SERVICE_TABLE_ENTRY serviceTable[] = {
      {(LPTSTR)SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
      {nullptr, nullptr}};

  if (!StartServiceCtrlDispatcher(serviceTable)) {
    return GetLastError();  // NOLINT
  }
  return 0;
}

// ============================
// Windows Service Logic
// ============================

void ServiceMain(DWORD /*argc*/, LPTSTR* /*argv*/) {
  g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
  if (!g_StatusHandle) {
    return;
  }

  g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  g_ServiceStatus.dwControlsAccepted =
      SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
  g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;

  SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

  g_ServiceStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
  if (!g_ServiceStopEvent) {
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
    return;
  }

  g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
  SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

  auto relay_logger = createRelayLogger("127.0.0.1", 514);

  std::thread tcp_thread(runTcpServer, "127.0.0.1", 5140, relay_logger);
  std::thread udp_thread(runUdpServer, "127.0.0.1", 5141, relay_logger);

  WaitForSingleObject(g_ServiceStopEvent, INFINITE);

  g_Running.store(false);

  tcp_thread.join();
  udp_thread.join();

  CloseHandle(g_ServiceStopEvent);

  g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
  SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

void ServiceCtrlHandler(DWORD ctrl) {
  switch (ctrl) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
      g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
      SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
      SetEvent(g_ServiceStopEvent);
      break;
    default:
      break;
  }
}
