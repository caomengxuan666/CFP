// clang-format off

#ifndef C57FDA8B_7A4D_4189_A490_17F8D6632F3E
#define C57FDA8B_7A4D_4189_A490_17F8D6632F3E
/**
\~chinese
 * @file IKapCType.h
 * @brief IKapC库数据类型定义
 * @note 在符合C99标准的系统上，定义由stdint.h头文件提供的类型和宏的子集，有助于提高便利性。
 */
/** 
 \~english
 * @file IKapCType.h
 * @brief IKapC library data type definition
 * @note Defining a subset of types and macros provided by stdint.h header file on C99 compliant system, which can help
improving convenience.
 */
#ifndef _IKAPC_TYPE_H
#define _IKAPC_TYPE_H

#if __STDC_VERSION__ >= 199901L

/* Compiler is c99-compliant.
 */
#    define __STDC_LIMIT_MACROS
#    define __STDC_CONSTANT_MACROS
#    include <inttypes.h>
#    include <stdint.h>

#elif defined(_WIN32)

#    if defined(_MSC_VER) && _MSC_VER >= 1600
// VS2010 provides stdint.h
#        include <stdint.h>
#    elif defined(__GNUC__) && __STDC_HOSTED__
#        include <stdint.h>
#    else
#        ifndef _STDINT || _STDINT_H  // only define if not already done

typedef __int8          int8_t;     ///< @brief \~chinese 8bit有符号整数类型 \~english 8bit signed integer type
typedef unsigned __int8 uint8_t;    ///< @brief \~chinese 8bit无符号整数类型 \~english 8bit unsigned integer type
typedef __int16          int16_t;   ///< @brief \~chinese 16bit有符号整数类型 \~english 16bit signed integer type
typedef unsigned __int16 uint16_t;  ///< @brief \~chinese 16bit无符号整数类型 \~english 16bit unsigned integer type
typedef __int32          int32_t;   ///< @brief \~chinese 32bit有符号整数类型 \~english 32bit signed integer type
typedef unsigned __int32 uint32_t;  ///< @brief \~chinese 32bit无符号整数类型 \~english 32bit unsigned integer type
typedef __int64          int64_t;   ///< @brief \~chinese 64bit有符号整数类型 \~english 64bit signed integer type
typedef unsigned __int64 uint64_t;  ///< @brief \~chinese 64bit无符号整数类型 \~english 64bit unsigned integer type

#            if defined(_WIN64)
typedef __int64          intptr_t;  ///< @brief \~chinese 64bit系统有符号整数类型指针 \~english 64bit system signed integer type pointer
typedef unsigned __int64 uintptr_t; ///< @brief \~chinese 64bit系统无符号整数类型指针 \~english 64bit system unsigned integer type pointer
#            else
typedef __int32          intptr_t;  ///< @brief \~chinese 32bit系统有符号整数类型指针 \~english 32bit system signed integer type pointer
typedef unsigned __int32 uintptr_t; ///< @brief \~chinese 32bit系统无符号整数类型指针 \~english 32bit system unsigned integer type pointer
#            endif

#        endif  // _STDINT
#    endif      // _MSC_VER >= 1600

#    if defined(__cplusplus)

typedef bool _Bool; ///< @brief \~chinese C99标准内置布尔类型 \~english C99 standard built-in boolean type
#        if defined(_MSC_VER)
#            define _Bool ::_Bool
#            define PYLONC_BOOL_DEFINED
#        endif
#    else
#        if (defined(_MSC_VER) && _MSC_VER <= 1700) || defined(__BORLANDC__)
typedef unsigned char _Bool; ///< @brief \~chinese C99标准内置布尔类型 \~english C99 standard built-in boolean type
#        endif
#    endif

#else
#    error No platform-specific type definitions
#endif  // __STDC_VERSION__ >= 199901L

#ifndef INT64_MAX
#    define INT64_MAX 0x7fffffffffffffffLL	///< @brief \~chinese 64bit有符号整型数据最大值 \~english 64bit signed integer data maximum value
#endif

#ifndef INT64_MIN
#    define INT64_MIN 0x8000000000000000LL	///< @brief \~chinese 64bit有符号整型数据最小值 \~english 64bit signed integer data minimum value
#endif

#ifndef UINT64_MAX
#    define UINT64_MAX 0xffffffffffffffffULL	///< @brief \~chinese 64bit无符号整型数据最大值 \~english 64bit unsigned integer data maximum value
#endif

#ifndef INT32_MAX
#    define INT32_MAX 0x000000007fffffffLL	///< @brief \~chinese 32bit有符号整型数据最大值 \~english 32bit signed integer data maximum value
#endif

#ifndef INT32_MIN
#    define INT32_MIN 0xffffffff80000000LL	///< @brief \~chinese 32bit有符号整型数据最小值 \~english 32bit signed integer data minimum value
#endif

#ifndef UINT32_MAX
#    define UINT32_MAX 0x00000000ffffffffULL	///< @brief \~chinese 32bit无符号整型数据最大值 \~english 32bit unsigned integer data maximum value
#endif

#ifndef INT8_MAX
#    define INT8_MAX 0x000000000000007fLL	///< @brief \~chinese 8bit有符号整型数据最大值 \~english 8bit signed integer data maximum value
#endif

#ifndef INT8_MIN
#    define INT8_MIN 0xffffffffffffff80LL	///< @brief \~chinese 8bit有符号整型数据最小值 \~english 8bit signed integer data minimum value
#endif

#ifndef UINT8_MAX
#    define UINT8_MAX 0x00000000000000ffULL	///< @brief \~chinese 8bit无符号整型数据最大值 \~english 8bit unsigned integer data maximum value
#endif

#ifdef IKAPC_BOOL_DEFINED
//#undef _Bool
#    undef IKAPC_BOOL_DEFINED
#endif

#endif /* _IKAPC_TYPE_H
 */

// clang-format on

#endif /* C57FDA8B_7A4D_4189_A490_17F8D6632F3E */
