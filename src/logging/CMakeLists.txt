add_library(CaponLogging SHARED CaponLogging.cpp)

# 强制CMake生成导入库
set_target_properties(CaponLogging PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(CaponLogging PUBLIC spdlog CFPUtils)