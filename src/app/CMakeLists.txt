# IPC服务器，负责作为中继，收集本地程序崩溃情况并且发送给服务器
add_executable(Ipc_service Ipc_serverice.cpp)

target_link_libraries(Ipc_service PRIVATE spdlog)