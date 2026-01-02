include_directories(${CMAKE_SOURCE_DIR}/include)
# 创建测试可执行文件

# 1. 递归遍历所有 .cpp 文件（先获取全部文件）
file(GLOB_RECURSE CFP_SOURCE
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  # 可选：使路径相对当前目录，更清晰
    "*.cpp"
)

# 2. 过滤掉所有包含 "/client/" 路径的文件（核心步骤）
list(FILTER CFP_SOURCE EXCLUDE REGEX "client/")

# 过滤文件,example.cpp
list(FILTER CFP_SOURCE EXCLUDE REGEX "example.cpp")

# 打印文件列表
message("CFP_SOURCE: ${CFP_SOURCE}")


add_library(CFP STATIC ${CFP_SOURCE})
if(MSVC)
    target_compile_options(CFP PRIVATE
        /source-charset:utf-8
        /execution-charset:utf-8
    )
endif()

target_include_directories(CFP PUBLIC ${PROJECT_SOURCE_DIR}/third_party/cpp_redis/includes)
target_include_directories(CFP PUBLIC ${PROJECT_SOURCE_DIR}/third_party/cpp_redis/tacopie/includes)

target_link_libraries(CFP PUBLIC 
    DVP2::DVPCamera 
    DVP2::dvpir
    IKap::IKapC 
    IKap::IKapBoard 
    concurrentqueue
    BS_thread_pool
    cpp_redis tacopie
    ${OpenCV_LIBS}
    DVPUtils
    )

if(SAVE_RESULT_IMG)
    target_compile_definitions(CFP PRIVATE SAVE_RESULT_IMAGE_QUEUE)
endif()

add_executable(main ${CFP_SOURCE})

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/third_party/cpp_redis/includes)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/third_party/cpp_redis/tacopie/includes)

if(MSVC)
    target_compile_options(main PRIVATE
        /source-charset:utf-8
        /execution-charset:utf-8
    )
endif()


# 链接DVP库和第三方库
target_link_libraries(main PRIVATE 
    CFP
)


if(SAVE_RESULT_IMG)
    target_compile_definitions(main PRIVATE SAVE_RESULT_IMAGE_QUEUE)
endif()

add_subdirectory(utils)
# client暂时不着急制作，需要统一多相机接口之后再慢慢看怎么开发
#add_subdirectory(client)