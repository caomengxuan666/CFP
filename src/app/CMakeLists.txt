# IPC服务器，负责作为中继，收集本地程序崩溃情况并且发送给服务器
add_executable(Ipc_service IpcServer/Ipc_serverice.cpp)

target_link_libraries(Ipc_service PRIVATE spdlog)

add_executable(crash-reporter crash-reporter/crash_reporter.cpp)

target_compile_options(crash-reporter PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
target_compile_options(crash-reporter PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
