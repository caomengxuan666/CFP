add_library(CaponLogging SHARED CaponLogging.cpp CrashLogger.cpp)

# 强制CMake生成导入库
set_target_properties(CaponLogging PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 复制到example文件夹下面
add_custom_command(TARGET CaponLogging POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:CaponLogging> ${CMAKE_BINARY_DIR}/examples
)

target_link_libraries(CaponLogging PUBLIC spdlog CFPUtils)