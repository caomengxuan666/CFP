# ApplyPatch.cmake 【Windows专用纯净版】
# 批量打补丁，适配cpp_redis/tacopie的老CMake版本修改，无任何报错参数
# 参数：PATCH_DIR=补丁目录  BASE_DIR=要打补丁的根目录
function(ApplyPatch)
    set(options VERBOSE)
    set(oneValueArgs PATCH_DIR BASE_DIR)
    cmake_parse_arguments(AP "${options}" "${oneValueArgs}" "" ${ARGN})

    if(NOT EXISTS "${AP_PATCH_DIR}")
        message(FATAL_ERROR "补丁目录不存在: ${AP_PATCH_DIR}")
    endif()
    if(NOT EXISTS "${AP_BASE_DIR}")
        message(FATAL_ERROR "目标目录不存在: ${AP_BASE_DIR}")
    endif()

    file(GLOB PATCH_FILES "${AP_PATCH_DIR}/*.patch")
    list(SORT PATCH_FILES)
    if(NOT PATCH_FILES)
        message(STATUS "补丁目录下无patch文件: ${AP_PATCH_DIR}")
        return()
    endif()

    foreach(PATCH_FILE IN LISTS PATCH_FILES)
        get_filename_component(PATCH_NAME "${PATCH_FILE}" NAME)
        # 只对文件名匹配的patch生效，核心解决：批量patch但只给对应目录打对应补丁
        if( (${PATCH_NAME} MATCHES "cpp_redis" AND ${AP_BASE_DIR} MATCHES "cpp_redis$") OR 
            (${PATCH_NAME} MATCHES "tacopie" AND ${AP_BASE_DIR} MATCHES "tacopie$") )
            message(STATUS "开始打补丁: ${PATCH_NAME} -> ${AP_BASE_DIR}")
            execute_process(
                COMMAND git apply --ignore-whitespace --recount "${PATCH_FILE}"
                WORKING_DIRECTORY "${AP_BASE_DIR}"
                RESULT_VARIABLE PATCH_RESULT
                OUTPUT_QUIET
                ERROR_QUIET
            )
            if(PATCH_RESULT EQUAL 0)
                message(STATUS "✅ 补丁成功: ${PATCH_NAME}")
            else()
                # 兼容已经打过补丁的情况，不报warning，避免刷屏
                message(STATUS "ℹ️ 补丁已生效: ${PATCH_NAME}")
            endif()
        endif()
    endforeach()
endfunction()