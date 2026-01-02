# FindIkap.cmake
# 查找IKap SDK

set(IK_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/IKap")

# 查找头文件目录
find_path(IK_INCLUDE_DIR
    NAMES IKapC.h
    PATHS ${IK_SDK_ROOT}/include/IKap
    NO_DEFAULT_PATH
)

# 查找库文件
find_library(IK_C_LIBRARY
    NAMES IKapC
    PATHS ${IK_SDK_ROOT}/lib/x64
    NO_DEFAULT_PATH
)

find_library(IK_KAPBOARD_LIBRARY
    NAMES IKapBoard
    PATHS ${IK_SDK_ROOT}/lib/x64
    NO_DEFAULT_PATH
)


# 查找DLL
find_file(IK_C_RUNTIME
    NAMES IKapC.dll
    PATHS ${IK_SDK_ROOT}/bin/x64
    NO_DEFAULT_PATH
)

find_file(IK_KAPBOARD_RUNTIME
    NAMES IKapBoard.dll
    PATHS ${IK_SDK_ROOT}/bin/x64
    NO_DEFAULT_PATH
)

# 处理标准参数
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IKap
    REQUIRED_VARS IK_INCLUDE_DIR IK_C_LIBRARY IK_KAPBOARD_LIBRARY
)

# 设置公共变量
if(IKap_FOUND)
    set(IKap_INCLUDE_DIRS ${IK_INCLUDE_DIR})
    set(IKap_LIBRARIES ${IK_C_LIBRARY} ${IK_KAPBOARD_LIBRARY})

    if(NOT TARGET IKap::IKapC) 
        add_library(IKap::IKapC UNKNOWN IMPORTED)
        set_target_properties(IKap::IKapC PROPERTIES
            IMPORTED_LOCATION "${IK_C_LIBRARY}"
            IMPORTED_IMPLIB "${IK_C_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${IK_INCLUDE_DIR}"
        )
    endif()

    if(NOT TARGET IKap::IKapBoard) 
        add_library(IKap::IKapBoard UNKNOWN IMPORTED)
        set_target_properties(IKap::IKapBoard PROPERTIES
            IMPORTED_LOCATION "${IK_KAPBOARD_LIBRARY}"
            IMPORTED_IMPLIB "${IK_KAPBOARD_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${IK_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES IKap::IKapC
        )
    endif()
endif()

# 隐藏内部变量
mark_as_advanced(IK_INCLUDE_DIR IK_C_LIBRARY IK_KAPBOARD_LIBRARY)