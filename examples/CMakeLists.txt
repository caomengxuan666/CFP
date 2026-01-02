# 示例项目CMakeLists.txt
# 查找项目中的源文件
file(GLOB EXAMPLE_SOURCES "*.cpp")

# 为每个示例源文件创建可执行文件
foreach(example_file ${EXAMPLE_SOURCES})
    # 获取文件名（不含扩展名）
    get_filename_component(example_name ${example_file} NAME_WE)
    
    # 创建可执行文件目标
    add_executable(${example_name} ${example_file})
    
    # 链接主项目库 - 使用正确的库名
    target_link_libraries(${example_name} PRIVATE     
        CFP
    )

    if(MSVC)
        target_compile_options(${example_name} PRIVATE
            /source-charset:utf-8
            /execution-charset:utf-8
        )
    endif()

    
    # 设置输出目录
    set_target_properties(${example_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
    
    # 添加调试信息
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${example_name} PRIVATE DEBUG)
    endif()
    
    # 设置头文件目录
    target_include_directories(${example_name} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/BS_THREAD_POOL/include
        ${CMAKE_SOURCE_DIR}/third_party/concurrentqueue
    )
endforeach()
