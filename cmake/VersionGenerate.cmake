# VersionGenerate.cmake
# 版本信息生成模块
# 使用方式:capon_generate_version(MAJOR 1 MINOR 0 PATCH 0 BUILD 1)

function(generate_version)
    set(options)
    set(oneValueArgs MAJOR MINOR PATCH BUILD PRODUCT_NAME COMPANY_NAME)
    set(multiValueArgs)
    cmake_parse_arguments(CAPON_VER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if("${CAPON_VER_MAJOR}" STREQUAL "")
        message(STATUS "MAJOR 参数为空，使用默认值 1")
        set(CAPON_VER_MAJOR 1)
    else()
        message(STATUS "MAJOR 参数不为空，值为: ${CAPON_VER_MAJOR}")
    endif()
    
    if("${CAPON_VER_MINOR}" STREQUAL "")
        message(STATUS "MINOR 参数为空，使用默认值 0")
        set(CAPON_VER_MINOR 0)
    endif()
    
    if("${CAPON_VER_PATCH}" STREQUAL "")
        message(STATUS "PATCH 参数为空，使用默认值 0")
        set(CAPON_VER_PATCH 0)
    endif()
    
    if("${CAPON_VER_BUILD}" STREQUAL "")
        message(STATUS "BUILD 参数为空，使用默认值 1")
        set(CAPON_VER_BUILD 1)
    endif()

    set(CAPON_VERSION_MAJOR ${CAPON_VER_MAJOR})
    set(CAPON_VERSION_MINOR ${CAPON_VER_MINOR})
    set(CAPON_VERSION_PATCH ${CAPON_VER_PATCH})
    set(CAPON_VERSION_BUILD ${CAPON_VER_BUILD})

    # 生成GUID
    string(TIMESTAMP TIMESTAMP_SEC "%s")

    # 生成随机部分 - 不使用种子，避免可能的问题
    string(RANDOM LENGTH 8 ALPHABET "0123456789abcdef" PART1)
    string(RANDOM LENGTH 4 ALPHABET "0123456789abcdef" PART2)
    string(RANDOM LENGTH 4 ALPHABET "0123456789abcdef" PART3)
    string(RANDOM LENGTH 4 ALPHABET "0123456789abcdef" PART4)

    # 时间戳部分确保唯一性，并格式化为12位
    string(TIMESTAMP TIMESTAMP_MS "%f")
    string(SUBSTRING "${TIMESTAMP_MS}" 0 3 TIMESTAMP_MS_SHORT)

    # 组合时间戳和随机数
    math(EXPR TS_NUM "${TIMESTAMP_SEC} % 1000000000000")
    string(LENGTH "${TS_NUM}" TS_LEN)

    # 补零到12位
    if(TS_LEN LESS 12)
        math(EXPR PADDING_LEN "12 - ${TS_LEN}")
        if(PADDING_LEN GREATER 0)
            string(REPEAT "0" ${PADDING_LEN} TS_PADDING)
        else()
            set(TS_PADDING "")
        endif()
        set(TIMESTAMP_PART "${TS_PADDING}${TS_NUM}")
    else()
        string(SUBSTRING "${TS_NUM}" 0 12 TIMESTAMP_PART)
    endif()

    # 最终的12位部分：时间戳 + 毫秒
    string(SUBSTRING "${TIMESTAMP_PART}${TIMESTAMP_MS_SHORT}" 0 12 PART5)

    set(CAPON_APP_GUID "${PART1}-${PART2}-${PART3}-${PART4}-${PART5}")

    set(CAPON_APP_GUID "${PART1}-${PART2}-${PART3}-${PART4}-${PART5}")

    # 获取构建时间
    string(TIMESTAMP CAPON_BUILD_DATE "%Y-%m-%d %H:%M:%S" UTC)
    string(TIMESTAMP CAPON_COPYRIGHT_YEAR "%Y")

    # 设置变量供父作用域使用
    set(CAPON_VERSION_MAJOR ${CAPON_VER_MAJOR} PARENT_SCOPE)
    set(CAPON_VERSION_MINOR ${CAPON_VER_MINOR} PARENT_SCOPE)
    set(CAPON_VERSION_PATCH ${CAPON_VER_PATCH} PARENT_SCOPE)
    set(CAPON_VERSION_BUILD ${CAPON_VER_BUILD} PARENT_SCOPE)
    set(CAPON_PRODUCT_NAME "${CAPON_VER_PRODUCT_NAME}" PARENT_SCOPE)
    set(CAPON_COMPANY_NAME "${CAPON_VER_COMPANY_NAME}" PARENT_SCOPE)
    set(CAPON_APP_GUID "${CAPON_APP_GUID}" PARENT_SCOPE)
    set(CAPON_BUILD_DATE "${CAPON_BUILD_DATE}" PARENT_SCOPE)
    set(CAPON_COPYRIGHT_YEAR "${CAPON_COPYRIGHT_YEAR}" PARENT_SCOPE)

    # 设置变量用于configure_file - 在当前作用域中设置
    set(CAPON_VERSION_MAJOR ${CAPON_VER_MAJOR})
    set(CAPON_VERSION_MINOR ${CAPON_VER_MINOR})
    set(CAPON_VERSION_PATCH ${CAPON_VER_PATCH})
    set(CAPON_VERSION_BUILD ${CAPON_VER_BUILD})
    set(CAPON_PRODUCT_NAME "${CAPON_VER_PRODUCT_NAME}")
    set(CAPON_COMPANY_NAME "${CAPON_VER_COMPANY_NAME}")
    set(CAPON_APP_GUID "${CAPON_APP_GUID}")
    set(CAPON_BUILD_DATE "${CAPON_BUILD_DATE}")
    set(CAPON_COPYRIGHT_YEAR "${CAPON_COPYRIGHT_YEAR}")

    # 创建构建目录
    set(VERSION_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/version")
    file(MAKE_DIRECTORY "${VERSION_BUILD_DIR}")

    # 获取模板目录路径 - 修复这里！
    # 假设模板文件在 cmake/templates 目录下
    set(TEMPLATE_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates")
    
    # 调试信息，检查模板文件是否存在
    message(STATUS "模板目录: ${TEMPLATE_DIR}")
    message(STATUS "模板文件是否存在: ${TEMPLATE_DIR}/version.h.in")
    
    # 配置头文件
    configure_file(
        "${TEMPLATE_DIR}/version.h.in"
        "${VERSION_BUILD_DIR}/capon_version.h"
        @ONLY
    )

    # 配置源文件
    configure_file(
        "${TEMPLATE_DIR}/version.cpp.in"
        "${VERSION_BUILD_DIR}/version.cpp"
        @ONLY
    )
    

    # 创建版本库
    add_library(capon_version STATIC)
    target_compile_options(capon_version PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    target_compile_options(capon_version PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

    # 添加源文件（在构建目录中）
    set(VERSION_SRC "${VERSION_BUILD_DIR}/version.cpp")
    set_source_files_properties("${VERSION_SRC}" PROPERTIES GENERATED TRUE)

    # 添加源文件到库
    target_sources(capon_version PRIVATE "${VERSION_SRC}")

    # 设置包含目录
    target_include_directories(capon_version
        PUBLIC
        "${VERSION_BUILD_DIR}"
    )

    # 设置C++标准
    set_target_properties(capon_version PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
    )

    # 设置输出目录
    set_target_properties(capon_version PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        OUTPUT_NAME "capon_version"
    )

    # 添加编译定义（用于跨平台导出）
    target_compile_definitions(capon_version
        PRIVATE
        CAPON_VERSION_BUILDING
        INTERFACE
        CAPON_VERSION_USING
    )

    # 安装规则（可选，如果不需要可以删除）
    if(FALSE)  # 暂时禁用安装
        install(TARGETS capon_version
            EXPORT CaponVersionTargets
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin
        )

        install(FILES "${VERSION_BUILD_DIR}/capon_version.h"
            DESTINATION include/capon
        )

        # 创建版本信息文件
        configure_file(
            "${TEMPLATE_DIR}/CaponVersionConfig.cmake.in"
            "${CMAKE_BINARY_DIR}/CaponVersionConfig.cmake"
            @ONLY
        )

        # 导出目标
        export(EXPORT CaponVersionTargets
            FILE "${CMAKE_BINARY_DIR}/CaponVersionTargets.cmake"
        )
    endif()

    # 显示信息
    message(STATUS "=========================================")
    message(STATUS "Capon Version Libraries Generated")
    message(STATUS "Product: ${CAPON_VER_PRODUCT_NAME}")
    message(STATUS "Version: ${CAPON_VER_MAJOR}.${CAPON_VER_MINOR}.${CAPON_VER_PATCH}.${CAPON_VER_BUILD}")
    message(STATUS "GUID: ${CAPON_APP_GUID}")
    message(STATUS "Build Time: ${CAPON_BUILD_DATE}")
    message(STATUS "Headers: ${VERSION_BUILD_DIR}/capon_version.h")
    message(STATUS "Libs: capon_version")
    message(STATUS "=========================================")

    # 添加清理目标（可选）
    add_custom_target(clean_version
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${VERSION_BUILD_DIR}"
        COMMENT "清理版本生成文件"
    )
endfunction()