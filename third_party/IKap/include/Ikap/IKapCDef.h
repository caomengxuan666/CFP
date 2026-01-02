// clang-format off
/**
\~chinese
 * @file IKapCDef.h
 * @brief IKapC库相关定义
 */
 /**
\~english
 * @file IKapCDef.h
 * @brief IKapC library related definition
 */

#ifndef _IKAPCDEF_H
#define _IKAPCDEF_H
#include "IKapCStat.h"

#if defined(_MSC_VER) || defined(__BORLANDC__)
#    ifdef IKAPC_EXPORTS
#        define IKAPC_API __declspec(dllexport)
#    else
#        define IKAPC_API __declspec(dllimport)
#    endif
#    define IKAPC_CC      __stdcall
#    define IKAPC_PACKING 8
#elif defined(__GNUC__) || defined(SWIG)
#    define IKAPC_API
#    define IKAPC_CC
#    define IKAPC_PACKING 8
#else
#    error Unsupported platform
#endif

#define IKAPC_DECLARE_HANDLE(name)                                                                                          \
    typedef struct name##_ {                                                                                                \
        int unused;                                                                                                         \
    } * name;

/**
\~chinese
 * @name 无效句柄
*/
 /**
\~english
 * @name Invalid handle
 */
#ifdef __cplusplus
#    define GENAPIC_INVALID_HANDLE (0)
#else
#    define GENAPIC_INVALID_HANDLE ((void *)0)
#endif

///@{
/**
\~chinese
 * @name 全局参数
 * @see ItkManGetPrm ItkManSetPrm
*/
 /**
\~english
 * @name Manager parameter
 * @see ItkManGetPrm ItkManSetPrm
 */
#define ITKMANAGER_PRM_FIRST    0u
#define ITKMANAGER_PRM(n, size) ((((n) + ITKMANAGER_PRM_FIRST) << 16) | (size))

/**
\~chinese
 * @brief 是否使用CXP12协议
 * @note 本参数表示是否使用CXP12协议【RW】。
 * 1. 本参数默认支持CXP12协议。
 * 2. 参数取值及描述如下所示：
 * | 参数取值 | 描述            |
 * |----------|-----------------|
 * | 0        | 不使用CXP12协议 |
 * | 1        | 使用CXP12协议   |
*/
 /**
\~english
 * @brief Whether to use CXP12 protocol
 * @note This parameter indicates whether to use CXP12 protocol [RW].
 * 1. This parameter supports CXP12 protocol by default.
 * 2. The parameter values and descriptions are as follows:
 * | Parameter value  | Description              |
 * |------------------|--------------------------|
 * | 0                | Not using CXP12 protocol |
 * | 1                | Using CXP12 protocol     |
 */
#define ITKMANAGER_COAXPRESS_12_SUPPORT ITKMANAGER_PRM(0, 4)

/**
\~chinese
 * @brief 是否支持CXP指令Tag
 * @note 本参数表示是否支持CXP指令Tag【RW】。
 * 1. 参数取值及描述如下所示：
 * | 参数取值 | 描述             |
 * |----------|------------------|
 * | 0        | 不支持CXP指令Tag |
 * | 1        | 支持CXP指令Tag   |
*/
 /**
\~english
 * @brief Whether to support CXP instruction Tag
 * @note This parameter indicates whether CXP instruction Tag is supported [RW].
 * 1. The parameter values and descriptions are as follows:
 * | Parameter value | Description                      |
 * |-----------------|----------------------------------|
 * | 0               | Does not support CXP command Tag |
 * | 1               | Support CXP command Tag          |
 */
#define ITKMANAGER_COAXPRESS_COMMUNICATION_TAG_SUPPORT ITKMANAGER_PRM(1, 4)

/**
\~chinese
 * @brief CXP控制包最大尺寸
 * @note 本参数表示CXP控制包最大尺寸【RW】。
*/
 /**
\~english
 * @brief CXP control packet maximum size
 * @note This parameter indicates the maximum size of CXP control packet [RW].
 */
#define ITKMANAGER_COAXPRESS_CONTROL_PACKET_MAX_SIZE ITKMANAGER_PRM(2, 4)

/**
\~chinese
 * @brief CXP流包最大尺寸
 * @note 本参数表示CXP流包最大尺寸【RW】。
*/
 /**
\~english
 * @brief CXP stream packet maximum size
 * @note This parameter indicates the maximum size of CXP stream packet [RW].
 */
#define ITKMANAGER_COAXPRESS_STREAM_PACKET_MAX_SIZE ITKMANAGER_PRM(3, 4)

/**
\~chinese
 * @brief CXP读写寄存器最大次数
 * @note 本参数表示CXP读写寄存器最大次数【RW】。
*/
 /**
\~english
 * @brief CXP maximum number of read and write register operations
 * @note This parameter indicates the maximum number of CXP read and write register operations [RW].
 */
#define ITKMANAGER_COAXPRESS_RW_REGISTER_MAX_TIME ITKMANAGER_PRM(4, 4)

/**
\~chinese
 * @brief 使能合并相同序列号相机
 * @note 本参数表示是否使能合并相同序列号相机功能，仅XGV采集卡具有合并相同序列号相机功能【RW】。
*/
 /**
\~english
 * @brief Enable to merge the same serial number camera
 * @note This parameter indicates if enable to merge the same serial number camera, only XGVB can merge the same serial number camera [RW].
 */
#define ITKMANAGER_ENABLE_MERGE_SAME_SERIAL_NUMBER_CAMERA  ITKMANAGER_PRM(6, 4)

/**
\~chinese
 * @brief 设置GV相机探测时的超时时间
 * @note GV相机探测时的超时时间【RW】。
*/
 /**
\~english
 * @brief Set the timeout time for GV camera detection
 * @note The timeout time for GV camera detection [RW].
 */
#define ITKMANAGER_GV_CONTROL_DISCORY_TIMEOUT  ITKMANAGER_PRM(7, 8)

/**
\~chinese
 * @brief 接口模块列表
 * @note 接口模块列表。
 * @see ITKMANAGER_ENABLED_MANAGER_MODULES
*/
 /**
\~english
 * @brief Manager module list
 * @note Manager module list.
 * @see ITKMANAGER_ENABLED_MANAGER_MODULES
 */
enum MANAGER_TYPE_LIST
{
    MANAGER_TYPE_CL   = 1<<1, ///< @brief \~chinese CL设备 \~english CL device
    MANAGER_TYPE_GV   = 1<<2, ///< @brief \~chinese GV设备 \~english GV device
    MANAGER_TYPE_CXP  = 1<<3, ///< @brief \~chinese CXP设备 \~english CXP device
    MANAGER_TYPE_U3V  = 1<<4, ///< @brief \~chinese USB设备 \~english USB device
    MANAGER_TYPE_XGVB = 1<<5, ///< @brief \~chinese XGV设备 \~english XGV device
    MANAGER_TYPE_PCIE = 1<<6,  ///< @brief \~chinese PCIe设备 \~english PCIe device
    MANAGER_TYPE_GENTL = 1<<7,  ///< @brief \~chinese GenTL设备 \~english GenTL device
    MANAGER_TYPE_VIRTUAL = 1<<8  ///< @brief \~chinese VIRTUAL设备 \~english VIRTUAL device
};

///@}

///@{
/**
\~chinese
 * @name 设备参数
 * @see ItkDevGetPrm ItkDevSetPrm
*/
 /**
\~english
 * @name Device parameter
 * @see ItkDevGetPrm ItkDevSetPrm
 */
#define ITKDEV_PRM_FIRST    0u
#define ITKDEV_PRM(n, size) ((((n) + ITKDEV_PRM_FIRST) << 16) | (size))

/**
\~chinese
 * @brief 设备心跳包超时时间
 * @note 本参数表示设备心跳包超时时间，单位ms【RW】。
 * 1. 当调试千兆网相机设备时，可以增大该参数，保证相机不会因为未及时接受心跳包而停止工作。
*/
 /**
\~english
 * @brief Device heartbeat timeout
 * @note This parameter indicates device heartbeat timeout, measured in milliseconds [RW].
 * 1. When debugging a gigabit network camera device, users can increase this parameter to ensure that the camera
 * does not stop working due to not receiving heartbeat in a timely manner.
 */
#define ITKDEV_PRM_HEARTBEAT_TIMEOUT ITKDEV_PRM(0, 4)

/**
\~chinese
 * @brief 设备接口类型
 * @note 本参数表示设备接口类型【RO】。
*/
 /**
\~english
 * @brief Device interface type
 * @note This parameter indicates device interface type [RO].
 */
#define ITKDEV_PRM_INTERFACE_TYPE ITKDEV_PRM(1, 4)

/**
\~chinese
 * @brief 禁用心跳线程
 * @note 本参数表示禁用设备心跳线程，需要同时开启设备的DisableHeartBeat特征，否则设备会拒绝访问【RW】。
*/
 /**
\~english
 * @brief Disable heartbeat thread
 * @note This parameter indicates that to disable the device heartbeat thread, 
 * and needs to simultaneously activate the DisableHeartBeat features of the device, otherwise the device will deny access [RW].
 */
#define ITKDEV_PRM_DISABLE_HEARTBEAT_THREAD ITKDEV_PRM(2, 4)

/**
\~chinese
 * @brief 禁用控制通道等待ACK
 * @note 本参数表示禁用控制通道等待ACK.【RW】。
*/
 /**
\~english
 * @brief Disable control channel waiting for ACK.
 * @note This parameter indicates that to disable control channel waiting for ACK. [RW].
 */
#define ITKDEV_PRM_DISABLE_CONTROL_CHANNEL_WAITING_ACK ITKDEV_PRM(3, 4)

/**
\~chinese
 * @brief 相机控制通道操作超时时间 
*/
 /**
\~english
 * @brief Camera control channel operation timeout time
 */
#define ITKDEV_PRM_CONTROL_CHANNEL_TIMEOUT ITKDEV_PRM(4, 4)

/**
\~chinese
 * @brief 设备只读访问模式
 * @note 本标志不能与其他参数组合，它通常用于网口相机的多播/广播应用。
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
*/
 /**
\~english
 * @brief Device read-only access mode
 * @note This flag cannot be combined with other parameters and is typically used for multicast/broadcast applications on Ethernet cameras.
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
 */
#define ITKDEV_VAL_ACCESS_MODE_MONITOR 0

/**
\~chinese
 * @brief 设备参数读写访问模式
 * @note 允许对相机参数进行读写来配置相机。
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
*/
 /**
\~english
 * @brief Device parameter read/write access mode
 * @note Allow reading and writing of camera parameters to configure camera.
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
 */
#define ITKDEV_VAL_ACCESS_MODE_CONTROL (1 << 0)

/**
\~chinese
 * @brief 设备数据流只读访问模式
 * @note 允许从相机的数据流中读取图像数据。
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
*/
 /**
\~english
 * @brief Read only access mode for device data stream
 * @note Allow reading image data from camera's data stream.
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
 */
#define ITKDEV_VAL_ACCESS_MODE_STREAM (1 << 1)

/**
\~chinese
 * @brief 设备独占式访问模式
 * @note 指定本标志时，其他应用程序不能访问相机。
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
*/
 /**
\~english
 * @brief Exclusive access mode for device
 * @note When this flag is specified, other applications cannot access the camera.
 * @see ItkDevOpen ItkDevOpenWithXmlDescription
 */
#define ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE (1 << 2)

/**
\~chinese
 * @brief 设备掉线回调事件
 * @note 本事件在设备掉线时触发,通过"DeviceRemove"注册。
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english
 * @brief Device offline callback event
 * @note This event is triggered when the device is offline, registered by "DeviceRemove".
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
#define ITKDEV_VAL_EVENT_TYPE_DEV_REMOVE 0x00010000u

/**
\~chinese
 * @brief 设备参数变化回调事件
 * @note 本事件在设备参数变化时触发,通过"FeatureValueChanged"注册。
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english
 * @brief Device parameter changed callback event
 * @note This event is triggered when the device parameter is changed, registered by "FeatureValueChanged".
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
#define ITKDEV_VAL_EVENT_TYPE_FEATURE_VALUE_CHANGED 0x00020000u

/**
\~chinese
 * @brief 消息通道回调事件
 * @note 本事件在设备通过消息通道发送event时触发，通过"MessageChannel"注册。
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english
 * @brief Message channel callback event
 * @note This event is triggered during device send events by message channel, registered by "MessageChannel".
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
#define ITKDEV_VAL_EVENT_TYPE_MESSAGE_CHANNEL 0x00200000u

#define ITKDEV_VAL_EVENT_TYPE_MASK 0xFFFF0000u
///@}

///@{
/**
\~chinese
 * @name 特征参数
*/
 /**
\~english
 * @name Feature parameter
 */

/**
\~chinese
 * @name 特征类型列表
*/
 /**
\~english
 * @name Feature type list
 */
enum ITKFEATURE_VAL_TYPE_LIST
{
    /**
    \~chinese
    * @brief 数据类型未定义
    * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief Data type undefined
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_UNDEFINED = 0u,

    /**
    \~chinese
     * @brief 32位整数类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief 32bit integer type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_INT32 = 1u,

    /**
    \~chinese
     * @brief 64位整数类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief 64bit integer type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_INT64 = 2u,

    /**
    \~chinese
     * @brief 32位浮点数类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief 32bit floating type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_FLOAT = 3u,

    /**
    \~chinese
     * @brief 64位浮点数类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief 64bit floating type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_DOUBLE = 4u,

    /**
    \~chinese
     * @brief 布尔数据类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief Boolean data type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_BOOL = 5u,

    /**
    \~chinese
     * @brief 枚举数据类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief Enumeration data type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_ENUM = 6u,

    /**
    \~chinese
     * @brief 字符串数据类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief String data type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_STRING = 7u,

    /**
    \~chinese
     * @brief 命令数据类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief Command data type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_COMMAND = 8u,

    /**
    \~chinese
     * @brief 类别类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief Category type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_CATEGORY = 9u,

    /**
    \~chinese
     * @brief 寄存器类型
     * @see ItkFeatureGetType ItkDevGetType
    */
    /**
    \~english
    * @brief Register type
    * @see ItkFeatureGetType ItkDevGetType
    */
    ITKFEATURE_VAL_TYPE_REGISTER = 10u,
};

/**
\~chinese
 * @name 特征访问权限列表
*/
 /**
\~english
 * @name Feature access mode list
 */
enum ITKFEATURE_VAL_ACCESS_MODE_LIST
{
    /**
    \~chinese
    * @brief 访问模式未定义
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    /**
    \~english
    * @brief Access mode undefined
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    ITKFEATURE_VAL_ACCESS_MODE_UNDEFINED = 0u,

    /**
    \~chinese
     * @brief 读写访问模式
     * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    /**
    \~english
    * @brief Read write access mode
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    ITKFEATURE_VAL_ACCESS_MODE_RW = 1u,

    /**
    \~chinese
     * @brief 只读访问模式
     * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    /**
    \~english
    * @brief Read only access mode
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    ITKFEATURE_VAL_ACCESS_MODE_RO = 2u,

    /**
    \~chinese
     * @brief 只写访问模式
     * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    /**
    \~english
    * @brief Write only access mode
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    ITKFEATURE_VAL_ACCESS_MODE_WO = 3u,

    /**
    \~chinese
     * @brief 该特征未实现，无法访问
     * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    /**
    \~english
    * @brief This feature is not implemented and cannot be accessed
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    ITKFEATURE_VAL_ACCESS_MODE_NI = 4u,

    /**
    \~chinese
     * @brief 该特征当前状态下不可用，无法访问
     * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    /**
    \~english
    * @brief This feature is currently unavailable and cannot be accessed
    * @see ItkFeatureGetAccessMode ItkDevGetAccessMode
    */
    ITKFEATURE_VAL_ACCESS_MODE_NA = 5u,
};

/**
\~chinese
 * @name 特征命名空间列表
*/
/**
\~english
* @name Feature namespace list
*/
enum ITKFEATURE_VAL_NAME_SPACE_LIST
{
    /**
    \~chinese
    * @brief 命名空间未定义
    * @see ItkFeatureGetNameSpace ItkFeatureGetNameSpace
    */
    /**
    \~english
    * @brief Namespace undefined
    * @see ItkFeatureGetNameSpace ItkFeatureGetNameSpace
    */
    ITKFEATURE_VAL_NAME_SPACE_UNDEFINED = 0u,

    /**
    \~chinese
     * @brief 自定义命名空间
     * @see ItkFeatureGetNameSpace ItkFeatureGetNameSpace
    */
    /**
    \~english
    * @brief Custom namespace
    * @see ItkFeatureGetNameSpace ItkFeatureGetNameSpace
    */
    ITKFEATURE_VAL_NAME_SPACE_CUSTOM = 1u,

    /**
    \~chinese
     * @brief 标准命名空间
     * @see ItkFeatureGetNameSpace ItkFeatureGetNameSpace
    */
    /**
    \~english
    * @brief Standard namespace
    * @see ItkFeatureGetNameSpace ItkFeatureGetNameSpace
    */
    ITKFEATURE_VAL_NAME_SPACE_STANDARD = 2u,
};

/**
\~chinese
 * @name 特征表现方式列表
*/
/**
\~english
* @name Feature representation list
*/
enum ITKFEATURE_VAL_REPRESENTATION_LIST
{
    /**
    \~chinese
    * @brief 方式未定义
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Representation undefined
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_UNDEFINED = 0u,

    /**
    \~chinese
     * @brief 线性
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Linear
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_LINEAR = 1u,

    /**
    \~chinese
     * @brief 对数
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Logarithm
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_LOGARITHMIC = 2u,

    /**
    \~chinese
     * @brief 布尔型
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Boolean type
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_BOOLEAN = 3u,

    /**
    \~chinese
     * @brief 十进制数
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Decimal number
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_PURENUMBER = 4u,

    /**
    \~chinese
     * @brief 十六进制数
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Hexadecimal number
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_HEXNUMBER = 5u,

    /**
    \~chinese
     * @brief IPV4地址
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief IPV4 address
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_IPV4ADDRESS = 6u,

    /**
    \~chinese
     * @brief 物理地址
     * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    /**
    \~english
    * @brief Physical address
    * @see ItkFeatureGetRepresentation ItkDevGetRepresentation
    */
    ITKFEATURE_VAL_REPRESENTATION_MACADDRESS = 7u,
};

/**
\~chinese
 * @name 可视等级列表
*/
/**
\~english
* @name Feature visibility list
*/
enum ITKFEATURE_VAL_VISIBILITY_LIST
{
    /**
    \~chinese
    * @brief 可视性未定义
    * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    /**
    \~english
    * @brief Visibility undefined
    * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    ITKFEATURE_VAL_VISIBILITY_UNDEFINED = 0u,

    /**
    \~chinese
     * @brief 初学者级别
     * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    /**
    \~english
    * @brief Beginner level
    * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    ITKFEATURE_VAL_VISIBILITY_BEGINNER = 1u,

    /**
    \~chinese
     * @brief 专家级别
     * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    /**
    \~english
    * @brief Expert level
    * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    ITKFEATURE_VAL_VISIBILITY_EXPERT = 2u,

    /**
    \~chinese
     * @brief 大师级别
     * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    /**
    \~english
    * @brief Master level
    * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    ITKFEATURE_VAL_VISIBILITY_GURU = 3u,

    /**
    \~chinese
     * @brief 特征不可见
     * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    /**
    \~english
    * @brief Feature invisible
    * @see ItkFeatureGetVisibility ItkDevGetVisibility
    */
    ITKFEATURE_VAL_VISIBILITY_INVISIBLE = 4u,
};

#define ITKFEATURE_VAL_SIGN_UNDEFINED 0u	///< @brief \~chinese 数据符号位未定义 \~english Data symbol bit undefined
#define ITKFEATURE_VAL_SIGN_SIGNED    1u	///< @brief \~chinese有符号类型数据  \~english Signed type data
#define ITKEFATURE_VAL_SIGN_UNSIGNED  2u	///< @brief \~chinese 无符号类型数据 \~english Unsigned type data

/**
\~chinese
 * @name 缓存模式
*/
/**
\~english
* @name Feature caching mode list
*/
enum ITKFEATURE_VAL_CACHING_MODE_LIST
{
    /**
    \~chinese
    * @brief 不写入缓存
    */
    /**
    \~english
    * @brief Not written to cache
    */
    ITKFEATURE_VAL_CACHING_MODE_NO_CACHE = 0,         
    
    /**
    \~chinese
    * @brief 写入数值的同时写入缓存
    */
    /**
    \~english
    * @brief Write the value into the cache when write it
    */
    ITKFEATURE_VAL_CACHING_MODE_WRITE_THROUGH = 1,         
    
    /**
    \~chinese
    * @brief 仅将读取的数值写入缓存
    */
    /**
    \~english
    * @brief Only the read values are written to the cache
    */
    ITKFEATURE_VAL_CACHING_MODE_WRITE_AROUND = 2,         
    
    /**
    \~chinese
    * @brief 缓存模式未定义
    */
    /**
    \~english
    * @brief The cache mode is undefined
    */
    ITKFEATURE_VAL_CACHING_MODE_UNDEFINED = 3,         
};
///@}


///@{
/**
\~chinese
 * @name 缓冲区参数
 * @see ItkBufferGetPrm ItkBufferSetPrm
*/
 /**
\~english
 * @name Buffer parameter
 * @see ItkBufferGetPrm ItkBufferSetPrm
 */
#define ITKBUFFFER_PRM_FIRST   0u
#define ITKBUFFER_PRM(n, size) ((((n) + ITKBUFFFER_PRM_FIRST) << 16) | (size))

/**
\~chinese
 * @brief 缓冲区数据格式
 * @note 本参数表示缓冲区数据格式【RW】。
*/
 /**
\~english
 * @brief Buffer data format
 * @note This parameter indicates buffer data format [RW].
 */
#define ITKBUFFER_PRM_FORMAT ITKBUFFER_PRM(0, 4)

/**
\~chinese
 * @brief 缓冲区宽度
 * @note 本参数表示缓冲区宽度【RW】。
*/
 /**
\~english
 * @brief Buffer width
 * @note This parameter indicates buffer width [RW].
 */
#define ITKBUFFER_PRM_WIDTH ITKBUFFER_PRM(3, 8)

/**
\~chinese
 * @brief 缓冲区高度
 * @note 本参数表示缓冲区高度【RW】。
*/
 /**
\~english
 * @brief Buffer height
 * @note This parameter indicates buffer height [RW].
 */
#define ITKBUFFER_PRM_HEIGHT ITKBUFFER_PRM(4, 8)

/**
\~chinese
 * @brief 缓冲区地址
 * @note 本参数表示缓冲区地址【RW】。
 * 1. 本参数表示缓冲区首地址，可以通过该地址直接访问缓冲区数据。
*/
 /**
\~english
 * @brief Buffer address
 * @note This parameter indicates buffer address [RW].
 * 1. This parameter indicates the first address of buffer, which allows direct access to buffer data.
 */
#define ITKBUFFER_PRM_ADDRESS ITKBUFFER_PRM(5, 8)

/**
\~chinese
 * @brief 缓冲区状态为满时的时间戳
 * @note 本参数表示缓冲区状态为满时的时间戳，该值受系统CPU时钟频率影响【RO】。
*/
 /**
\~english
 * @brief Timestamp when buffer status is full
 * @note This parameter indicates timestamp when buffer state is full, which is affected by system CPU clock frequency [RO].
 */
#define ITKBUFFER_PRM_HOST_COUNTER_STAMP ITKBUFFER_PRM(6, 8)

/**
\~chinese
 * @brief 缓冲区状态
 * @note 本参数表示缓冲区状态【RO】。
 * 1. 缓冲区状态定义了缓冲区为空或者满。当缓冲区为空时，数据流采集得到的图像数据可以存入该缓冲区中；
 当缓冲区为满时，如果数据流继续采集数据并放入缓冲区中，会导致缓冲区被覆盖。
*/
 /**
\~english
 * @brief Buffer status
 * @note This parameter indicates buffer status [RO].
 * 1. The buffer state defines whether the buffer is empty or full. When buffer is empty, image data grabbed by
data stream can be stored in it. When buffer is full, if data stream continues to collect data and put it into the buffer, it
will cause buffer overwritten.
 */
#define ITKBUFFER_PRM_STATE ITKBUFFER_PRM(7, 4)

/**
\~chinese
 * @brief 缓冲区像素值有无符号
 * @note 本参数表示缓冲区像素值有无符号【RO】。
 * 1. 本参数无法设置，数值由缓冲区像素格式 ITKBUFFER_PRM_FORMAT 决定。
*/
 /**
\~english
 * @brief Whether pixel value in buffer is signed or not
 * @note This parameter indicates whether pixel value in buffer is signed or not [RO].
 * 1. This parameter cannot be set, the value is determined by buffer pixel format ITKBUFFER_PRM_FORMAT .
 */
#define ITKBUFFER_PRM_SIGNED ITKBUFFER_PRM(8, 4)

/**
\~chinese
 * @brief 缓冲区大小
 * @note 本参数表示缓冲区大小【RO】。
 * 1. 数值由如下公式获取：ITKBUFFER_PRM_SIZE = ITKBUFFER_PRM_WIDTH * ITKBUFFER_PRM_HEIGHT * ITKBUFFER_PRM_DATA_BIT / 8。
*/
 /**
\~english
 * @brief Buffer size
 * @note This parameter indicates buffer size [RO].
 * 1. The value is obtained by the following formula: ITKBUFFER_PRM_SIZE = ITKBUFFER_PRM_WIDTH * ITKBUFFER_PRM_HEIGHT * ITKBUFFER_PRM_DATA_BIT / 8.
 */
#define ITKBUFFER_PRM_SIZE ITKBUFFER_PRM(9, 8)

/**
\~chinese
 * @brief 缓冲区索引
 * @note 本参数表示缓冲区索引【RO】。
 * 1. 对于网口相机，参数有效范围是[1,65535]，并且随着采集帧数的增加而顺序递增。
*/
 /**
\~english
 * @brief Buffer Index
 * @note This parameter indicates buffer index [RO].
 * 1. For Ethernet camera, the effective range of parameters is [1,65535], and it increases sequentially with the
increase of the number of grabbed frames.
 */
#define ITKBUFFER_PRM_BLOCK_ID ITKBUFFER_PRM(10, 8)

/**
\~chinese
 * @brief 缓冲区当前采集完成的行数
 * @note 本参数表示缓冲区当前采集完成的行数【RO】。
*/
 /**
\~english
 * @brief The current number of lines grabbed in buffer
 * @note This parameter indicates the current number of lines grabbed in buffer [RO].
 */
#define ITKBUFFER_PRM_READY_LINES ITKBUFFER_PRM(11, 8)

/**
\~chinese
 * @brief 缓冲区错误原因
 * @note 本参数表示缓冲区错误原因【RO】。
*/
 /**
\~english
 * @brief Reason for buffer error
 * @note This parameter indicates the reason for buffer error [RO].
 */
#define ITKBUFFER_PRM_ERROR_REASON ITKBUFFER_PRM(12, 4)

/**
\~chinese
 * @brief 当前帧图像重包数
 * @note 本参数表示当前帧图像重包数【RO】。
*/
 /**
\~english
 * @brief Number of image resend packet in the current frame
 * @note This parameter indicates the number of image resend packet in the current frame [RO].
 */
#define ITKBUFFER_PRM_RSND_PKT_COUNT ITKBUFFER_PRM(13, 8)

/**
\~chinese
 * @brief 当前帧图像丢包数
 * @note 本参数表示当前帧图像丢包数【RO】。
*/
 /**
\~english
 * @brief Current frame image packet loss count
 * @note This parameter indicates the current frame image packet loss count [RO].
 */
#define ITKBUFFER_PRM_LOST_PKT_COUNT ITKBUFFER_PRM(14, 8)

/**
\~chinese
 * @brief 获取寻边算法结果
 * @note 本参数表示获取寻边算法结果【RO】。
*/
 /**
\~english
 * @brief Obtain the results of the edge search algorithm
 * @note This parameter indicates obtaining the results of the edge finding algorithm [RO].
 */
#define ITKBUFFER_PRM_FIND_LINE_RESULT ITKBUFFER_PRM(15, 33 * 2 * 8)

/**
\~chinese
 * @brief 交换缓冲区宽高
 * @note 本参数表示交换缓冲区宽高【RO】。
*/
 /**
\~english
 * @brief Exchange buffer width and height
 * @note This parameter indicates exchanging buffer width and height.
 */
#define ITKBUFFER_PRM_EXCHANGE_WIDTH_HEIGHT ITKBUFFER_PRM(16, 4)

/**
\~chinese
 * @brief 批量缓冲区状态地址
 * @note 本参数表示批量缓冲区状态地址【RO】。
*/
 /**
\~english
 * @brief Batch buffer status address
 * @note This parameter indicates the batch buffer status address [RO].
 */
#define ITKBUFFER_PRM_BATCH_STATUS_ADDRESS ITKBUFFER_PRM(17, 8)

/**
\~chinese
 * @brief 毛刺检测结果
 * @note 本参数表示毛刺检测结果【RO】。
*/
 /**
\~english
 * @brief Burr detection result
 * @note This parameter indicates the burr detection result [RO].
 */
#define ITKBUFFER_PRM_BURR_DECTION_RESULT ITKBUFFER_PRM(18, 1)

/**
\~chinese
 * @brief 毛刺检测结果坐标
 * @note 本参数表示毛刺检测结果坐标【RO】。
*/
 /**
\~english
 * @brief Burr detection result coordinates
 * @note This parameter indicates the burr detection result coordinates [RO].
 */
#define ITKBUFFER_PRM_BURR_DECTION_RESULT_COORDINATE ITKBUFFER_PRM(19, 12)

/**
\~chinese
 * @brief 毛刺检测调试
 * @note 本参数表示毛刺检测调试【RO】。
*/
 /**
\~english
 * @brief Burr detection debug
 * @note This parameter indicates burr detection debug [RO].
 */
#define ITKBUFFER_PRM_BURR_DECTION_DEBUG ITKBUFFER_PRM(20, 8)

/**
\~chinese
 * @brief 缓冲区负载格式
 * @note  缓冲区负载格式【RO】。
*/
 /**
\~english
 * @brief Buffer payload type
 * @note This parameter indicates buffer payload type [RO].
 */
#define ITKBUFFER_PRM_PAYLOAD_TYPE ITKBUFFER_PRM(21, 4)

/**
\~chinese
 * @brief 缓冲区图像数据大小
 * @note  缓冲区图像数据大小【RO】。
*/
 /**
\~english
 * @brief Buffer image size
 * @note This parameter indicates buffer image size [RO].
 */
#define ITKBUFFER_PRM_IMAGE_SIZE               ITKBUFFER_PRM(22, 4)

/**
\~chinese
 * @brief 缓冲区像素总深度
 * @note 本参数表示缓冲区像素总深度【RO】。
 * 1. 本参数无法设置，数值由缓冲区像素格式 ITKBUFFER_PRM_FORMAT 决定。
*/
 /**
\~english
 * @brief Buffer pixel total depth
 * @note This parameter indicates buffer pixel total depth [RO].
 * 1. This parameter cannot be set, the value is determined by buffer pixel format ITKBUFFER_PRM_FORMAT .
 */
#define ITKBUFFER_PRM_PIXEL_BITS ITKBUFFER_PRM(23, 4)

/**
\~chinese
 * @brief 缓冲区通道深度
 * @note 本参数表示缓冲区通道深度【RO】。
 * 1. 本参数无法设置，数值由缓冲区像素格式 ITKBUFFER_PRM_FORMAT 决定。
*/
 /**
\~english
 * @brief Buffer channel depth
 * @note This parameter indicates buffer channel depth [RO].
 * 1. This parameter cannot be set, the value is determined by buffer pixel format ITKBUFFER_PRM_FORMAT .
 */
#define ITKBUFFER_PRM_CHANNEL_BITS ITKBUFFER_PRM(24, 4)

/**
\~chinese
 * @brief Bayer转换算法【RW】
 * @note 本参数在ItkBufferConvert接口中生效。
 * 可选算法: ITKBUFFER_VAL_BAYER_METHOD_BILINEAR、ITKBUFFER_VAL_BAYER_METHOD_VNG、ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4、ITKBUFFER_VAL_BAYER_METHOD_ITEK_OLD（速度递减，效果递增），默认使用ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4。
*/
 /**
\~english
 * @brief Bayer Convert method [RW]
 * @note This parameter takes effect in the ItkBufferConvert interface.
 * Optional methods: ITKBUFFER_VAL_BAYER_METHOD_BILINEAR、ITKBUFFER_VAL_BAYER_METHOD_VNG、ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4、ITKBUFFER_VAL_BAYER_METHOD_ITEK_OLD(decreasing speed, increasing effect), default using ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4.
 */
#define ITKBUFFER_PRM_BAYER_CONVERT_METHOD ITKBUFFER_PRM(25, 4)

/**
\~chinese
 * @brief 缓冲区数据格式定义
 * @note 缓冲区数据格式是一个32bit数据，数据各位描述如下所示：
 * | 位数    | 描述                         |
 * |---------|------------------------------|
 * | [0:7]   | 由内部实现，用作函数表的索引 |
 * | [8:15]  | 像素深度                     |
 * | [16:23] | 有效通道深度                 |
 * | [24]    | 图像类型                     |
 * | [25]    | 像素格式有无符号             |
 * | [26:31] | 预留                         |
*/
 /**
\~english
 * @brief Buffer data format definition
 * @note The buffer data format is 32bit data, and each bit is described as follows:
 * | Bit     | Description                                                     |
 * |---------|-----------------------------------------------------------------|
 * | [0:7]   | Implemented internally and used as an index for function tables |
 * | [8:15]  | Pixel depth                                                     |
 * | [16:23] | Valid channel depth                                             |
 * | [24]    | Image type                                                      |
 * | [25]    | Pixel format with or without sign                               |
 * | [26:31] | Reserved                                                        |
 */
#define ITKBUFFER_FORMAT(nChannelBits, nPixelBits, index) (((nChannelBits) << 16) | ((nPixelBits) << 8) | index)

/**
\~chinese
 * @brief 黑白类型图像
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english
 * @brief Monochrome type image
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_MONO 0x00000000u

/**
\~chinese
 * @brief 彩色类型图像
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english
 * @brief Color type image
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_COLOR 0x01000000u

/**
\~chinese
 * @brief 无符号类型像素格式
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english
 * @brief Unsigned type pixel format
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_UNSIGNED 0x00000000u

/**
\~chinese
 * @brief 有符号类型像素格式
 * @see ITKBUFFER_FORMAT
*/
/**
\~english
 * @brief Signed type pixel format
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_SIGNED 0x02000000u

/**
\~chinese
 * @brief 黑白8bit
 * @note 灰度8bit图像数据，每个像素占据1个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome 8bit
 * @note Grayscale 8bit image data, with each pixel occupying 1 byte.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO8 (ITKBUFFER_FORMAT(8, 8, 0x01) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白10bit
 * @note 灰度10bit图像数据，每个像素占据2个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome 10bit
 * @note Grayscale 10bit image data, with each pixel occupying 2 byte.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO10 (ITKBUFFER_FORMAT(10, 16, 0x02) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白压缩10bit
 * @note 灰度10bit压缩图像数据。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome packed 10bit
 * @note Grayscale 10bit packed image data.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO10PACKED                                                                                   \
    (ITKBUFFER_FORMAT(10, 12, 0x03) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白12bit
 * @note 灰度12bit图像数据，每个像素占据2个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome 12bit
 * @note Grayscale 12bit image data, with each pixel occupying 2 byte.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO12 (ITKBUFFER_FORMAT(12, 16, 0x04) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白压缩12bit
 * @note 灰度12bit压缩图像数据。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome packed 12bit
 * @note Grayscale 12bit packed image data.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO12PACKED                                                                                   \
    (ITKBUFFER_FORMAT(12, 12, 0x05) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白14bit
 * @note 灰度14bit图像数据，每个像素占据2个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome 14bit
 * @note Grayscale 14bit image data, with each pixel occupying 2 byte.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO14 (ITKBUFFER_FORMAT(14, 16, 0x06) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白16bit
 * @note 灰度16bit图像数据，每个像素占据2个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome 16bit
 * @note Grayscale 16bit image data, with each pixel occupying 2 byte.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO16 (ITKBUFFER_FORMAT(16, 16, 0x07) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief RGB彩色8bit
 * @note 彩色8bit图像，像素值按照RGB顺序在内存中排列，每个像素占据3个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief RGB color 8bit
 * @note Color 8bit image, pixel values are arranged in memory in RGB order, with each pixel occupying 3 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_RGB888 (ITKBUFFER_FORMAT(8, 24, 0x08) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief RGB彩色10bit
 * @note 彩色10bit图像，像素值按照RGB顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief RGB color 10bit
 * @note Color 10bit image, pixel values are arranged in memory in RGB order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_RGB101010 (ITKBUFFER_FORMAT(10, 48, 0x09) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief RGB彩色12bit
 * @note 彩色12bit图像，像素值按照RGB顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief RGB color 12bit
 * @note Color 12bit image, pixel values are arranged in memory in RGB order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_RGB121212 (ITKBUFFER_FORMAT(12, 48, 0x0A) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief RGB彩色14bit
 * @note 彩色14bit图像，像素值按照RGB顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief RGB color 14bit
 * @note Color 14bit image, pixel values are arranged in memory in RGB order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_RGB141414 (ITKBUFFER_FORMAT(14, 48, 0x0B) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief RGB彩色16bit
 * @note 彩色16bit图像，像素值按照RGB顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief RGB color 16bit
 * @note Color 16bit image, pixel values are arranged in memory in RGB order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_RGB161616 (ITKBUFFER_FORMAT(16, 48, 0x0C) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief BGR彩色8bit
 * @note 彩色8bit图像，像素值按照BGR顺序在内存中排列，每个像素占据3个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief BGR color 8bit
 * @note Color 8bit image, pixel values are arranged in memory in BGR order, with each pixel occupying 3 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BGR888 (ITKBUFFER_FORMAT(8, 24, 0x0D) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief BGR彩色10bit
 * @note 彩色10bit图像，像素值按照BGR顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief BGR color 10bit
 * @note Color 10bit image, pixel values are arranged in memory in BGR order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BGR101010 (ITKBUFFER_FORMAT(10, 48, 0x0E) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief BGR彩色12bit
 * @note 彩色12bit图像，像素值按照BGR顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief BGR color 12bit
 * @note Color 12bit image, pixel values are arranged in memory in BGR order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BGR121212 (ITKBUFFER_FORMAT(12, 48, 0x0F) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief BGR彩色14bit
 * @note 彩色14bit图像，像素值按照BGR顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief BGR color 14bit
 * @note Color 14bit image, pixel values are arranged in memory in BGR order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BGR141414 (ITKBUFFER_FORMAT(14, 48, 0x10) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief BGR彩色16bit
 * @note 彩色16bit图像，像素值按照BGR顺序在内存中排列，每个像素占据6个字节。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief BGR color 16bit
 * @note Color 16bit image, pixel values are arranged in memory in BGR order, with each pixel occupying 6 bytes.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BGR161616 (ITKBUFFER_FORMAT(16, 48, 0x11) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列8bit
 * @note Bayer格式8bit图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG 8bit
 * @note Bayer 8bit image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GR8 (ITKBUFFER_FORMAT(8, 8, 0x12) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列8bit
 * @note Bayer格式8bit图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB 8bit
 * @note Bayer 8bit image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_RG8 (ITKBUFFER_FORMAT(8, 8, 0x13) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列8bit
 * @note Bayer格式8bit图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG 8bit
 * @note Bayer 8bit image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GB8 (ITKBUFFER_FORMAT(8, 8, 0x14) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列8bit
 * @note Bayer格式8bit图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR 8bit
 * @note Bayer 8bit image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_BG8 (ITKBUFFER_FORMAT(8, 8, 0x15) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列10bit
 * @note Bayer格式10bit图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG 10bit
 * @note Bayer 10bit image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GR10 (ITKBUFFER_FORMAT(10, 16, 0x16) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列10bit
 * @note Bayer格式10bit图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB 10bit
 * @note Bayer 10bit image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_RG10 (ITKBUFFER_FORMAT(10, 16, 0x17) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列10bit
 * @note Bayer格式10bit图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG 10bit
 * @note Bayer 10bit image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GB10 (ITKBUFFER_FORMAT(10, 16, 0x18) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列10bit
 * @note Bayer格式10bit图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR 10bit
 * @note Bayer 10bit image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_BG10 (ITKBUFFER_FORMAT(10, 16, 0x19) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列压缩10bit
 * @note Bayer格式10bit压缩图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG packed 10bit
 * @note Bayer 10bit packed image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GR10PACKED                                                                               \
    (ITKBUFFER_FORMAT(10, 12, 0x1A) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列压缩10bit
 * @note Bayer格式10bit压缩图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB packed 10bit
 * @note Bayer 10bit packed image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_RG10PACKED                                                                               \
    (ITKBUFFER_FORMAT(10, 12, 0x1B) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列压缩10bit
 * @note Bayer格式10bit压缩图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG packed 10bit
 * @note Bayer 10bit packed image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GB10PACKED                                                                               \
    (ITKBUFFER_FORMAT(10, 12, 0x1C) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列压缩10bit
 * @note Bayer格式10bit压缩图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR packed 10bit
 * @note Bayer 10bit packed image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_BG10PACKED                                                                               \
    (ITKBUFFER_FORMAT(10, 12, 0x1D) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列12bit
 * @note Bayer格式12bit图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GR12 (ITKBUFFER_FORMAT(12, 16, 0x1E) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列12bit
 * @note Bayer格式12bit图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_RG12 (ITKBUFFER_FORMAT(12, 16, 0x1F) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列12bit
 * @note Bayer格式12bit图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GB12 (ITKBUFFER_FORMAT(12, 16, 0x20) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列12bit
 * @note Bayer格式12bit图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_BG12 (ITKBUFFER_FORMAT(12, 16, 0x21) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief YUV图像
 * @note YUV图像（YUYV）。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief YUV image
 * @note YUV image（YUYV）.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_YUV422_8_YUYV                                                                                  \
    (ITKBUFFER_FORMAT(8, 16, 0x22) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief YUV图像
 * @note YUV图像（UYUV）。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief YUV image
 * @note YUV image（UYUV）.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_YUV422_8_UYUV                                                                                  \
    (ITKBUFFER_FORMAT(8, 16, 0x23) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 3D图像
 * @note 3D图像（C16）Z坐标数据16bit。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief 3D image
 * @note 3D image（C16）Z-coordinate data for 16bit.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_COORD3D_C16 (ITKBUFFER_FORMAT(16, 16, 0x24) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 3D图像
 * @note 3D图像（AC16）X坐标数据16bit Z坐标数据16bit。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief 3D image
 * @note 3D image（AC16）X-coordinate data for 16bit and Z-coordinate data for 16bit.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_COORD3D_AC16                                                                                   \
    (ITKBUFFER_FORMAT(16, 32, 0x25) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 3D图像
 * @note 3D图像（ACR16）X坐标数据16bit Z坐标数据16bit 亮度数据16bit。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief 3D image
 * @note 3D image（ACR16）X-coordinate data for 16bit and Z-coordinate data for 16bit Brightness data for the 16bit.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_COORD3D_ACR16                                                                                  \
    (ITKBUFFER_FORMAT(16, 48, 0x26) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief YUV图像
 * @note YUV图像（UYVY）。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief YUV image
 * @note YUV image（UYVY）.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_YUV422_8_UYVY                                                                                  \
    (ITKBUFFER_FORMAT(8, 16, 0x27) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief PCD图像
 * @note PCD图像。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief PCD image
 * @note PCD image.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_PCD (ITKBUFFER_FORMAT(32, 128, 0x2B) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief Bayer图像GRBG排列压缩12bit
 * @note Bayer格式12bit压缩图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GR12PACKED                                                                               \
    (ITKBUFFER_FORMAT(12, 12, 0x2C) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列压缩12bit
 * @note Bayer格式12bit压缩图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_RG12PACKED                                                                               \
    (ITKBUFFER_FORMAT(12, 12, 0x2D) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列压缩12bit
 * @note Bayer格式12bit压缩图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GB12PACKED                                                                               \
    (ITKBUFFER_FORMAT(12, 12, 0x2E) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列压缩12bit
 * @note Bayer格式12bit压缩图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR packed 12bit
 * @note Bayer 12bit packed image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_BG12PACKED                                                                               \
    (ITKBUFFER_FORMAT(12, 12, 0x2F) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 黑白压缩10bit（USB相机专用）
 * @note 灰度10bit压缩图像数据。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome packed 10bit (USB camera specific)
 * @note Grayscale 10bit compressed image data
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_U3V_MONO10PACKED                                                                               \
    (ITKBUFFER_FORMAT(10, 10, 0x30) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列压缩10bit（USB相机专用）
 * @note Bayer格式10bit压缩图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG packed 10bit (USB camera specific)
 * @note Bayer 10bit packed image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_GR10PACKED                                                                           \
    (ITKBUFFER_FORMAT(10, 10, 0x31) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列压缩10bit（USB相机专用）
 * @note Bayer格式10bit压缩图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB packed 10bit (USB camera specific)
 * @note Bayer 10bit packed image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_RG10PACKED                                                                           \
    (ITKBUFFER_FORMAT(10, 10, 0x32) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列压缩10bit（USB相机专用）
 * @note Bayer格式10bit压缩图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG packed 10bit (USB camera specific)
 * @note Bayer 10bit packed image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_GB10PACKED                                                                           \
    (ITKBUFFER_FORMAT(10, 10, 0x33) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列压缩10bit（USB相机专用）
 * @note Bayer格式10bit压缩图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR packed 10bit (USB camera specific)
 * @note Bayer 10bit packed image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_BG10PACKED                                                                           \
    (ITKBUFFER_FORMAT(10, 10, 0x34) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列14bit
 * @note Bayer格式14bit图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GRBG 14bit
 * @note Bayer 14bit image, pixel values are arranged in memory in GRBG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GR14 (ITKBUFFER_FORMAT(14, 16, 0x35) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列14bit
 * @note Bayer格式14bit图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer RGGB 14bit
 * @note Bayer 14bit image, pixel values are arranged in memory in RGGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_RG14 (ITKBUFFER_FORMAT(14, 16, 0x36) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列14bit
 * @note Bayer格式14bit图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer GBRG 14bit
 * @note Bayer 14bit image, pixel values are arranged in memory in GBRG order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_GB14 (ITKBUFFER_FORMAT(14, 16, 0x37) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列14bit
 * @note Bayer格式14bit图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Bayer BGGR 14bit
 * @note Bayer 14bit image, pixel values are arranged in memory in BGGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BAYER_BG14 (ITKBUFFER_FORMAT(14, 16, 0x38) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GRBG排列16bit
 * @note Bayer格式16bit图像，像素值按照GRBG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
* @brief Bayer GRBG 16bit
* @note Bayer 16bit image, pixel values are arranged in memory in GRBG order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_BAYER_GR16 (ITKBUFFER_FORMAT(16, 16, 0x39) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像RGGB排列16bit
 * @note Bayer格式16bit图像，像素值按照RGGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
* @brief Bayer RGGB 16bit
* @note Bayer 16bit image, pixel values are arranged in memory in RGGB order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_BAYER_RG16 (ITKBUFFER_FORMAT(16, 16, 0x3A) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像GBRG排列16bit
 * @note Bayer格式16bit图像，像素值按照GBRG顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
* @brief Bayer GBRG 16bit
* @note Bayer 16bit image, pixel values are arranged in memory in GBRG order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_BAYER_GB16 (ITKBUFFER_FORMAT(16, 16, 0x3B) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief Bayer图像BGGR排列16bit
 * @note Bayer格式16bit图像，像素值按照BGGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
* @brief Bayer BGGR 16bit
* @note Bayer 16bit image, pixel values are arranged in memory in BGGR order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_BAYER_BG16 (ITKBUFFER_FORMAT(16, 16, 0x3C) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
* @brief 黑白压缩12bit（USB相机专用）
* @note 灰度12bit压缩图像数据。
* @see ITKBUFFER_PRM_FORMAT
 */
/** 
\~english
* @brief Monochrome packed 12bit (USB camera specific)
* @note Grayscale 12bit compressed image data
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_U3V_MONO12PACKED                                                                               \
    (ITKBUFFER_FORMAT(12, 12, 0x3D) | ITKBUFFER_FORMAT_MONO | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
* @brief Bayer图像GRBG排列压缩12bit（USB相机专用）
* @note Bayer格式12bit压缩图像，像素值按照GRBG顺序在内存中排列。
* @see ITKBUFFER_PRM_FORMAT
*/
/** 
\~english
* @brief Bayer GRBG packed 12bit (USB camera specific)
* @note Bayer 12bit packed image, pixel values are arranged in memory in GRBG order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_GR12PACKED                                                                           \
    (ITKBUFFER_FORMAT(12, 12, 0x3E) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
* @brief Bayer图像RGGB排列压缩12bit（USB相机专用）
* @note Bayer格式12bit压缩图像，像素值按照RGGB顺序在内存中排列。
* @see ITKBUFFER_PRM_FORMAT
 */
/** 
\~english
* @brief Bayer RGGB packed 12bit (USB camera specific)
* @note Bayer 12bit packed image, pixel values are arranged in memory in RGGB order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_RG12PACKED                                                                           \
    (ITKBUFFER_FORMAT(12, 12, 0x3F) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
* @brief Bayer图像GBRG排列压缩12bit（USB相机专用）
* @note Bayer格式12bit压缩图像，像素值按照GBRG顺序在内存中排列。
* @see ITKBUFFER_PRM_FORMAT
 */
/** 
\~english
* @brief Bayer GBRG packed 12bit (USB camera specific)
* @note Bayer 12bit packed image, pixel values are arranged in memory in GBRG order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_GB12PACKED                                                                           \
    (ITKBUFFER_FORMAT(12, 12, 0x40) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
* @brief Bayer图像BGGR排列压缩12bit（USB相机专用）
* @note Bayer格式12bit压缩图像，像素值按照BGGR顺序在内存中排列。
* @see ITKBUFFER_PRM_FORMAT
 */
/** 
\~english
* @brief Bayer BGGR packed 12bit (USB camera specific)
* @note Bayer 12bit packed image, pixel values are arranged in memory in BGGR order.
* @see ITKBUFFER_PRM_FORMAT
*/
#define ITKBUFFER_VAL_FORMAT_U3V_BAYER_BG12PACKED                                                                           \
    (ITKBUFFER_FORMAT(12, 12, 0x41) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief 3D图像
 * @note 3D图像（Coord3D_CR32）。从高有效位开始，前8bit为亮度数据，后24bit为未标定的线中心数据。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief 3D image
 * @note 3D image（Coord3D_CR32）. Starting from the high effective bit, the first 8 bits are brightness data and the last 24 bits are uncalibrated line center data.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_COORD3D_CR32 (ITKBUFFER_FORMAT(32,32,0x42)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 3D图像
 * @note 3D图像（Calibrated_Coord3D_AC32）。4个连续的X坐标32bit，4个连续的32bit，其中从高有效位开始，高8bit是亮度信息，后24bit是Z坐标。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief 3D image
 * @note 3D image（Calibrated_Coord3D_AC32）. Four consecutive X coordinates 32 bits, four consecutive 32 bits, where starting from the most significant bit, the high 8 bits are the brightness information, and the last 24 bits are the Z coordinates.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_CALIBRATED_COORD3D_AC32 (ITKBUFFER_FORMAT(32,64,0x43)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 3D图像
 * @note 3D图像（Calibrated_Coord3D_C32）。32bit数据，其中从高有效位开始，高8bit是亮度信息，后24bit是Z坐标。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief 3D image
 * @note 3D image（Calibrated_Coord3D_C32）. 32bit data, where the first 8 bits from the high most significant bit are the brightness information and the last 24 bits are the Z coordinates.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_CALIBRATED_COORD3D_C32 (ITKBUFFER_FORMAT(32,32,0x44)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 32bit 无符号数。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief Data 32-bit unsigned.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_DATA_32 (ITKBUFFER_FORMAT(32,32,0x45)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 16bit 无符号数。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief Data 16-bit unsigned.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_DATA_16 (ITKBUFFER_FORMAT(16,16,0x46)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 32bit 有符号数。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief Data 32-bit signed.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_DATA_32S (ITKBUFFER_FORMAT(32,32,0x47)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_SIGNED)

/**
\~chinese
 * @brief 16bit 有符号数。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief Data 16-bit signed.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_DATA_16S (ITKBUFFER_FORMAT(16,16,0x48)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_SIGNED)

/**
\~chinese
 * @brief 16bit 无符号数和8bit无符号数。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief Data 16-bit unsigned with 8bit unsigned.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_DATA_16_WITH_8U (ITKBUFFER_FORMAT(24,24,0x49)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 32bit 无符号数和8bit无符号数。
 * @see ITKBUFFER_PRM_FORMAT
  */
/** 
\~english
 * @brief Data 32-bit unsigned with 8bit unsigned.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_DATA_32_WITH_8U (ITKBUFFER_FORMAT(40,40,0x50)|ITKBUFFER_FORMAT_MONO|ITKBUFFER_FORMAT_UNSIGNED)

/**
\~chinese
 * @brief 函数表索引
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english
 * @brief Function table index
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_INDEX(format) ((format)&0xFF)

/**
\~chinese
* @brief 单个像素深度
* @see ITKBUFFER_FORMAT
*/
/**
\~english
* @brief Single pixel size
* @see ITKBUFFER_FORMAT
*/
#define ITKBUFFER_FORMAT_PIXEL_BITS(format) (((format) >> 8) & 0xFF)

/**
\~chinese
* @brief 单个通道深度
* @see ITKBUFFER_FORMAT (nDepth, nBits, index)
*/
/**
\~english
* @brief Single channel size
* @see ITKBUFFER_FORMAT (nDepth, nBits, index)
*/
#define ITKBUFFER_FORMAT_CHANNEL_BITS(format) (((format) >> 16) & 0xFF)

/**
\~chinese
* @brief 获取图像通道数
* @see ITKBUFFER_FORMAT
*/
/**
\~english
* @brief Get the number of image channels
* @see ITKBUFFER_FORMAT
*/
#define ITKBUFFER_FORMAT_IMAGE_CHANNELS(format) (((ITKBUFFER_FORMAT_PIXEL_BITS(format)/ITKBUFFER_FORMAT_CHANNEL_BITS(format))>3&&ITKBUFFER_FORMAT_IS_COLOR(format))?3:(ITKBUFFER_FORMAT_PIXEL_BITS(format)/ITKBUFFER_FORMAT_CHANNEL_BITS(format)))

/**
\~chinese
 * @brief 像素格式是否为彩色
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english
 * @brief Whether the pixel format is color
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_IS_COLOR(format) (((format)&ITKBUFFER_FORMAT_COLOR) ? 1 : 0)

/**
\~chinese
 * @brief 像素格式有无符号
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english
 * @brief Pixel format with or without sign
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_IS_SIGNED(format) (((format)&ITKBUFFER_FORMAT_SIGNED) ? 1 : 0)

/**
\~chinese
 * @brief 缓冲区状态为空
 * @see ITKBUFFER_PRM_STATE
*/
 /**
\~english
 * @brief Buffer status is empty
 * @see ITKBUFFER_PRM_STATE
 */
#define ITKBUFFER_VAL_STATE_EMPTY 0x00000001u

/**
\~chinese
 * @brief 缓冲区状态为满
 * @see ITKBUFFER_PRM_STATE
*/
 /**
\~english
 * @brief Buffer status is full
 * @see ITKBUFFER_PRM_STATE
 */
#define ITKBUFFER_VAL_STATE_FULL 0x00000002u

/**
\~chinese
 * @brief 缓冲区被覆盖
 * @see ITKBUFFER_PRM_STATE
*/
 /**
\~english
 * @brief Buffer is overwritten
 * @see ITKBUFFER_PRM_STATE
 */
#define ITKBUFFER_VAL_STATE_OVERFLOW 0x00000004u

/**
\~chinese
 * @brief 缓冲区未完成
 * @see ITKBUFFER_PRM_STATE
*/
 /**
\~english
 * @brief Buffer is uncompleted
 * @see ITKBUFFER_PRM_STATE
 */
#define ITKBUFFER_VAL_STATE_UNCOMPLETED 0x00000008u

/**
\~chinese
 * @brief Bayer排列BGGR方式
 * @see ItkBufferBayerConvert
*/
 /**
\~english
 * @brief Bayer BGGR method
 * @see ItkBufferBayerConvert
 */
#define ITKBUFFER_VAL_BAYER_BGGR 0x00000001u

/**
\~chinese
 * @brief Bayer排列RGGB方式
 * @see ItkBufferBayerConvert
*/
 /**
\~english
 * @brief Bayer RGGB method
 * @see ItkBufferBayerConvert
 */
#define ITKBUFFER_VAL_BAYER_RGGB 0x00000002u

/**
\~chinese
 * @brief Bayer排列GBRG方式
 * @see ItkBufferBayerConvert
*/
 /**
\~english
 * @brief Bayer GBRG method
 * @see ItkBufferBayerConvert
 */
#define ITKBUFFER_VAL_BAYER_GBRG 0x00000004u

/**
\~chinese
 * @brief Bayer排列GRBG方式
 * @see ItkBufferBayerConvert
*/
 /**
\~english
 * @brief Bayer GRBG method
 * @see ItkBufferBayerConvert
 */
#define ITKBUFFER_VAL_BAYER_GRBG 0x00000008u

#define ITKBUFFER_VAL_BAYER_METHOD_DEFAULT                                                                                  \
    0x00000000u	///< @brief \~chinese Bayer转换默认方法, 当前是DFCD方法 \~english Bayer conversion default method, that is DFCD method
#define ITKBUFFER_VAL_BAYER_METHOD_BILINEAR                                                                                 \
    0x00000010u	///< @brief \~chinese Bayer转换bilinear方法 \~english Bayer conversion bilinear method
#define ITKBUFFER_VAL_BAYER_METHOD_VNG                                                                                      \
    0x00000020	///< @brief \~chinese Bayer转换vng方法 \~english Bayer conversion vng method
#define ITKBUFFER_VAL_BAYER_METHOD_DFSD                                                                                  \
    0x00000030u	///< @brief \~chinese Bayer转换DFSD方法 \~english Bayer conversion DFSD method
#define ITKBUFFER_VAL_BAYER_METHOD_ITEK_OLD                                                                                 \
    0x00000040u	///< @brief \~chinese Bayer转换ITEK_OLD方法 \~english Bayer conversion ITEK_OLD method
#define ITKBUFFER_VAL_BAYER_METHOD_DFCD                                                                                  \
    0x00000050u	///< @brief \~chinese Bayer转换DFCD方法 \~english Bayer conversion DFCD method
#define ITKBUFFER_VAL_BAYER_METHOD_DFCD2                                                                                  \
    0x00000060u	///< @brief \~chinese Bayer转换DFCD2方法 \~english Bayer conversion DFCD2 method


/**
\~chinese
 * @brief 数据格式缩小至至8bit，不能与拉伸参数同时使用
 * @see ItkBufferConvert
*/
 /**
\~english
 * @brief Data format shrink to 8bit，can't use with ITKBUFFER_VAL_CONVERT_OPTION_STRETCH at same time
 * @see ItkBufferConvert
 */
#define ITKBUFFER_VAL_CONVERT_OPTION_SHRINK (1 << 29)
 
 /**
\~chinese
 * @brief 数据格式拉伸至16bit，不能与缩小参数同时使用
 * @see ItkBufferConvert
*/
 /**
\~english
 * @brief Data format stretches to 16bit, can't use with ITKBUFFER_VAL_CONVERT_OPTION_SHRINK at same time
 * @see ItkBufferConvert
 */
#define ITKBUFFER_VAL_CONVERT_OPTION_STRETCH (1 << 30)

/**
\~chinese
 * @brief 数据格式自动
 * @note 主要会做三种转换:
 * 1. bayer格式转换为bgr
 * 2. yuv格式转换为bgr
 * 3. packed模式转换为unpacked
 * 4. rgb模式转化为bgr
 * @see ItkBufferConvert
*/
 /**
\~english
 * @brief Automatic data format
 * @note automatic convert:
 * 1. bayer to bgr
 * 2. yuv to bgr
 * 3. packed to unpacked
 * 4. rgb to bgr
 * @see ItkBufferConvert
 */
#define ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT (1 << 31)

/**
\~chinese
 * @brief 将RGB格式的图像按行重新交替排列三个通道
 * @note
 * ```
 *                             R1R2R3
 *                             G1G2G3
 *                             B1B2B3
 * (R1G1B1)(R2G2B2)(R3G3B3)    R4R5R6
 * (R4G4B4)(R5G5B5)(R6G6B6) -> G4G5G6
 * (R7G7B7)(R8G8B8)(R9G9B9)    B4B5B6
 *                             R7R8R9
 *                             G7G8G9
 *                             B7B8B9
 * ```
 * @see ItkBufferRGBSplitEx
 */
/**
\~english
 * @brief The images in RGB format alternate the three channels in rows
 * @note
 * ```
 *                             R1R2R3
 *                             G1G2G3
 *                             B1B2B3
 * (R1G1B1)(R2G2B2)(R3G3B3)    R4R5R6
 * (R4G4B4)(R5G5B5)(R6G6B6) -> G4G5G6
 * (R7G7B7)(R8G8B8)(R9G9B9)    B4B5B6
 *                             R7R8R9
 *                             G7G8G9
 *                             B7B8B9
 * ```
 * @see ItkBufferRGBSplitEx
 */
#define ITKBUFFER_VAL_RGB_SPLIT_INTO_ROW	(0x00000000u)

/**
\~chinese
 * @brief 将RGB格式的图像按通道重新排列成三个区块
 * @note
 * ```
 *                             R1R2R3
 *                             R4R5R6
 *                             R7R8R9
 * (R1G1B1)(R2G2B2)(R3G3B3)    G1G2G3
 * (R4G4B4)(R5G5B5)(R6G6B6) -> G4G5G6
 * (R7G7B7)(R8G8B8)(R9G9B9)    G7G8G9
 *                             B1B2B3
 *                             B4B5B6
 *                             B7B8B9
 * ```
 * @see ItkBufferResize
 */
 /**
\~english
 * @brief Images in RGB format were rearranged into three blocks by channel
 * @note
 * ```
 *                             R1R2R3
 *                             R4R5R6
 *                             R7R8R9
 * (R1G1B1)(R2G2B2)(R3G3B3)    G1G2G3
 * (R4G4B4)(R5G5B5)(R6G6B6) -> G4G5G6
 * (R7G7B7)(R8G8B8)(R9G9B9)    G7G8G9
 *                             B1B2B3
 *                             B4B5B6
 *                             B7B8B9
 * ```
 * @see ItkBufferResize
 */
#define ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK	(0x00000001u)

/**
\~chinese
 * @brief 将3D格式的图像按通道重新排列成N个区块
 * @note
 * ```
 *                             A1A2A3
 *                             A4A5A6
 *                             A7A8A9
 * (A1C1R1)(A2C2R2)(A3C3R3)    C1C2C3
 * (A4C4R4)(A5C5R5)(A6C6R6) -> C4C5C6
 * (A7C7R7)(A8C8R8)(A9C9R9)    C7C8C9
 *                             R1R2R3
 *                             R4R5R6
 *                             R7R8R9
 * ```
 * @see ItkBufferResize
 */
 /**
\~english
 * @brief Images in 3D format were rearranged into N blocks by channel
 * @note
 * ```
 *                             A1A2A3
 *                             A4A5A6
 *                             A7A8A9
 * (A1C1R1)(A2C2R2)(A3C3R3)    C1C2C3
 * (A4C4R4)(A5C5R5)(A6C6R6) -> C4C5C6
 * (A7C7R7)(A8C8R8)(A9C9R9)    C7C8C9
 *                             R1R2R3
 *                             R4R5R6
 *                             R7R8R9
 * ```
 * @see ItkBufferResize
 */
#define ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK	(0x00000000u)

/**
\~chinese
 * @brief 将3D格式的图像按通道重新排列成N个区块并按照batchCount分层
 * @note
 * ```
 * batchCount:2:
 *                                              A11A12A13
 *                                              A14A15A16
 *                                              A21A22A23
 * (A11C11R11)(A12C12R12)(A13C13R13)            A24A25A26
 * (A21C21R11)(A22C22R22)(A23C23R23) ->         C11C12C13
 * (A14C14R14)(A15C15R15)(A16C16R16)            C14C15C16
 * (A24C24R24)(A25C25R25)(A26C26R26)            C21C22C23
 *                                              C24C25C26
 *                                              R11R12R13
 *                                              R14R15R16
 *                                              R21R22R23
 *                                              R24R25R26
 * ```
 * @see ItkBufferResize
 */
 /**
\~english
 * @brief Images in 3D format were rearranged into N blocks by channel and were stratified according to the batchCount
 * @note
 * ```
 * batchCount:2:
 *                                              A11A12A13
 *                                              A14A15A16
 *                                              A21A22A23
 * (A11C11R11)(A12C12R12)(A13C13R13)            A24A25A26
 * (A21C21R11)(A22C22R22)(A23C23R23) ->         C11C12C13
 * (A14C14R14)(A15C15R15)(A16C16R16)            C14C15C16
 * (A24C24R24)(A25C25R25)(A26C26R26)            C21C22C23
 *                                              C24C25C26
 *                                              R11R12R13
 *                                              R14R15R16
 *                                              R21R22R23
 *                                              R24R25R26
 * ```
 * @see ItkBufferResize
 */
#define ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED	(0x00000001u)

/**
\~chinese
* @brief 图像缩放的最邻近方法
* @see ItkBufferResize
*/
/**
\~english
* @brief nearest neighbor interpolation
* @see ItkBufferResize
*/
#define ITKBUFFER_VAL_RESIZE_NEAREST		(0x00000000u)

/**
\~chinese
 * @brief 图像缩放的线性插值方法
 * @see ItkBufferResize
 */
 /**
\~english
 * @brief bilinear interpolation
 * @see ItkBufferResize
 */
#define ITKBUFFER_VAL_RESIZE_LINEAR			(0x00000001u)

/**
\~chinese
 * @brief 图像缩放的双三次插值方法
 * @see ItkBufferResize
 */
 /**
\~english
 * @brief bicubic interpolation
 * @see ItkBufferResize
 */
#define ITKBUFFER_VAL_RESIZE_CUBIC			(0x00000002u)

/**
\~chinese
 * @brief 图像缩放的区域重采样方法
 * @see ItkBufferResize
 */
 /**
\~english
 * @brief resampling using pixel area relation
 * @see ItkBufferResize
 */
#define ITKBUFFER_VAL_RESIZE_AREA			(0x00000003u)

/**
\~chinese
 * @brief Bmp图像格式
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief Bmp image format
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_BMP 0u

/**
\~chinese
 * @brief Tiff图像格式
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief Tiff image format
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_TIFF 1u

/**
\~chinese
 * @brief 原始图像格式
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief Raw image format
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_RAW 2u

/**
\~chinese
 * @brief Jpeg图像格式
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief Jpeg image format
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_JPEG 3u

/**
\~chinese
 * @brief Png图像格式
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief Png image format
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_PNG 4u

/**
\~chinese
 * @brief pcd图像格式
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief pcd image format
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_PCD 5u

/**
\~chinese
 * @brief 数据格式拉伸至16bit
 * @see ItkBufferLoad ItkBufferSave
*/
 /**
\~english
 * @brief Data format stretches to 16bit
 * @see ItkBufferLoad ItkBufferSave
 */
#define ITKBUFFER_VAL_STRETCH (1 << 30)

/**
\~chinese
 * @brief 负载类型
 * @see ItkBufferSizedNew ItkBufferNewWithPtr
*/
 /**
\~english
 * @brief paylaod type
 * @see ItkBufferSizedNew ItkBufferNewWithPtr
 */
enum PAYLOAD_TYPE
{
    IMAGE_PAYLOAD_TYPE                    = 0,  ///< @brief \~chinese 相机图像模式 \~english Camera image mode
    U3V_IMAGE_EXTENDED_CHUNK_PAYLOAD_TYPE = 1,  ///< @brief \~chinese U3V相机Chunk模式 \~english U3V camera chunk mode
    GV_CHUNK_DATA_PAYLOAD_TYPE            = 2,   ///< @brief \~chinese GV相机Chunk模式 \~english GV camera chunk mode
    RAW_DATA_PAYLOAD_TYPE                 = 3,
    FILE_PAYLOAD_TYPE                     = 4,
    MULTI_PART_PAYLOAD_TYPE               = 5,
    GENDC_PAYLOAD_TYPE                    = 6,
    DEVICE_SPECIFIC_PAYLOAD_TYPE          = 7,
};
typedef uint8_t PAYLOAD_TYPE_T;
///@}

///@{
/**
\~chinese
 * @name 数据流参数
 * @see ItkStreamGetPrm ItkStreamSetPrm
*/
 /**
\~english
 * @name Data stream parameter
 * @see ItkStreamGetPrm ItkStreamSetPrm
 */
#define ITKSTREAM_PRM_FIRST    0u
#define ITKSTREAM_PRM(n, size) ((((n) + ITKSTREAM_PRM_FIRST) << 16) | (size))

/**
\~chinese
 * @brief 数据流传输状态
 * @note 本参数表示数据流传输状态【RO】。
*/
 /**
\~english
 * @brief Data stream transmission status
 * @note This parameter indicates data stream transmission status [RO].
 */
#define ITKSTREAM_PRM_STATUS ITKSTREAM_PRM(0, 4)

/**
\~chinese
 * @brief 数据流支持的回调事件类型
 * @note 本参数表示数据流支持的回调事件类型【RO】。
*/
 /**
\~english
 * @brief The callback event types supported by data stream
 * @note This parameter indicates the callback event types supported by data stream [RO].
 */
#define ITKSTREAM_PRM_SUPPORT_EVENT_TYPE ITKSTREAM_PRM(1, 4)

/**
\~chinese
 * @brief 数据流采集启动模式
 * @note 本参数表示数据流采集启动模式【RW】。
 * 1. 本参数不会影响连续采集执行，连续采集过程始终采用非阻塞式启动方式。
*/
 /**
\~english
 * @brief Data stream acquisition start mode
 * @note This parameter indicates data stream acquisition start mode [RW].
 * 1. This parameter will not affect continuous acquisition, and continuous acquisition process will always use a
non blocking start mode.
 */
#define ITKSTREAM_PRM_START_MODE ITKSTREAM_PRM(2, 4)

/**
\~chinese
 * @brief 数据流传输模式
 * @note 本参数表示数据流传输模式，决定了数据传输过程中下一块传输缓冲区的索引【RW】。
*/
 /**
\~english
 * @brief Data stream transmission mode
 * @note This parameter indicates data stream transmission mode and determines the index of the next transmission buffer
during data transmission process [RW].
 */
#define ITKSTREAM_PRM_TRANSFER_MODE ITKSTREAM_PRM(3, 4)

/**
\~chinese
 * @brief 数据流自动清空标志
 * @note 本参数表示数据流自动清空标志【RW】。
*/
 /**
\~english
 * @brief Data stream automatic clearing flag
 * @note This parameter indicates data stream automatic clearing flag [RW].
 */
#define ITKSTREAM_PRM_AUTO_CLEAR ITKSTREAM_PRM(4, 4)

/**
\~chinese
 * @brief 数据流采集超时时间
 * @note 本参数表示数据流采集超时时间【RW】。
 * 1. 数据采集过程中，当连续两帧的时间间隔超过设定值，则会终止数据采集。
 * 2. 对于尺寸较大的图像，可能需要较长时间完成采集，增加超时时间可以确保图像正常采集。
*/
 /**
\~english
 * @brief Data stream acquisition timeout time
 * @note This parameter indicates data stream acquisition timeout time [RW].
 * 1. During data acquisition process, if the time interval between two consecutive frames exceeds the set value,
data acquisition will be terminated.
 * 2. For large images, it may take a longer time to complete acquisition, and increasing the timeout time can
ensure normal image acquisition.
 */
#define ITKSTREAM_PRM_TIME_OUT ITKSTREAM_PRM(5, 4)

/**
\~chinese
 * @brief 数据流实时采集帧率
 * @note 本参数表示数据流实时采集帧率【RO】。
*/
 /**
\~english
 * @brief Real time data stream acquisition frame rate
 * @note This parameter indicates real time data stream acquisition frame rate [RO].
 */
#define ITKSTREAM_PRM_FRAME_RATE ITKSTREAM_PRM(6, 4)

/**
\~chinese
 * @brief 千兆网单个数据包最大重发次数
 * @note 本参数表示千兆网单个数据包最大重发次数【RW】。
 * 1. 本参数仅对千兆网相机有效，当PC端丢失千兆网数据包后会重新请求丢失的数据包，
 单个数据包的最大请求次数不超过本参数。
*/
 /**
\~english
 * @brief Maximum retransmissions number for single packet
 * @note This parameter indicates maximum retransmissions number for single packet [RW].
 * 1. This parameter is only valid for gigabit network cameras. When PC loses a gigabit network packet, it will
request the lost packet again. The maximum number of requests for a single packet does not exceed this parameter.
 */
#define ITKSTREAM_PRN_GV_PACKET_MAX_RESEND_COUNT ITKSTREAM_PRM(7, 4)

/**
\~chinese
 * @brief 千兆网单个数据包超时时间
 * @note 本参数表示千兆网单个数据包超时时间【RW】。
 * 1. 本参数仅对千兆网相机有效，当PC端丢失千兆网数据包后会重新请求丢失的数据包，
 单个数据包请求的超时时间不超过本参数。
*/
 /**
\~english
 * @brief Gigabit network single packet timeout time
 * @note This parameter indicates gigabit network single packet timeout time [RW].
 * 1. This parameter is only valid for gigabit network cameras. When PC loses a gigabit network packet, it will
request the lost packet again. The timeout time for a single packet request does not exceed this parameter.
 */
#define ITKSTREAM_PRM_GV_PACKET_RESEND_TIMEOUT ITKSTREAM_PRM(8, 4)

/**
\~chinese
 * @brief 千兆网数据块最大重传包数
 * @note 本参数表示千兆网数据块最大重传包数【RW】。
 * 1. 本参数表示一帧图像允许的重传包数最大值。
 如果某帧图像统计发送重传包太多而超过此值，则不会再发送重传命令，而是直接判定残帧。
*/
 /**
\~english
 * @brief Maximum number of retransmitted packets for GigEVision data block
 * @note This parameter indicates maximum number of retransmitted packets for GigEVision data block [RW].
 * 1. This parameter indicates the maximum number of retransmission packets allowed for one frame of image. If a
certain frame of image statistics sends too many retransmission packets and exceeds this value, no retransmission command
will be sent again, but residual frames will be directly determined.
 */
#define ITKSTREAM_PRM_GV_BLOCK_MAX_RESEND_PACKET_COUNT ITKSTREAM_PRM(9, 4)

/**
\~chinese
 * @brief 千兆网数据块最大等待包数
 * @note 本参数表示千兆网数据块最大等待包数【RW】。
 * 1. 如果某帧图像还没接收完，但是下一帧图像已经到来，
 若下一帧图像接收了N个包，上一帧还没接收完，则强制结束上一帧，将其判断为残帧。
*/
 /**
\~english
 * @brief Maximum number of GigEVision data block to wait
 * @note This parameter indicates maximum number of GigEVision data block to wait [RW].
 * 1. If a certain frame of image has not been received yet, but the next frame of image has arrived, if the next
frame of image has received N packets and the previous frame has not been received yet, the previous frame will be forcibly
ended and judged as a residual frame.
 */
#define ITKSTREAM_PRM_GV_BLOCK_MAX_WAIT_PACKET_COUNT ITKSTREAM_PRM(10, 4)

/**
\~chinese
 * @brief 千兆网数据块重传窗口大小
 * @note 本参数表示千兆网数据块重传窗口大小【RW】。
*/
 /**
\~english
 * @brief Gigabit network data block retransmission window size
 * @note This parameter indicates gigabit network data block retransmission window size [RW].
 */
#define ITKSTREAM_PRM_GV_BLOCK_RESEND_WINODW_SIZE ITKSTREAM_PRM(11, 4)

/**
\~chinese
 * @brief 千兆网单个数据包超时时间
 * @note 本参数表示千兆网单个数据包超时时间【RW】。
*/
 /**
\~english
 * @brief Gigabit network single packet timeout time
 * @note This parameter indicates gigabit network single packet timeout time [RW].
 */
#define ITKSTREAM_PRM_GV_PACKET_INTER_TIMEOUT ITKSTREAM_PRM(12, 4)

/**
\~chinese
 * @brief 千兆网单个数据包轮询时间
 * @note 本参数表示千兆网单个数据包轮询时间【RW】。
*/
 /**
\~english
 * @brief Polling time of GigEVision single data packet
 * @note This parameter indicates polling time of GigEVision single data packet [RW].
 */
#define ITKSTREAM_PRM_GV_PACKET_POLLING_TIME ITKSTREAM_PRM(13, 4)

/**
\~chinese
 * @brief 千兆网内核缓冲区个数
 * @note 本参数表示千兆网内核缓冲区个数【RO】。
*/
 /**
\~english
 * @brief Count of GigEVision kernel buffer
 * @note This parameter indicates count of GigEVision kernel buffer [RO].
 */
#define ITKSTREAM_PRM_GV_KERNEL_BUFFER_COUNT ITKSTREAM_PRM(14, 4)

/**
\~chinese
 * @brief 数据流采集行频
 * @note 本参数表示数据流采集行频【RO】。
*/
 /**
\~english
 * @brief Data stream line rate
 * @note This parameter indicates data stream line rate [RO].
 */
#define ITKSTREAM_PRM_LINE_RATE ITKSTREAM_PRM(15, 4)

/**
\~chinese
 * @brief 毛刺检测保存速率
 * @note 本参数表示毛刺检测保存速率【RO】。
*/
 /**
\~english
 * @brief Burr detection save rate
 * @note This parameter indicates burr detection save rate [RO].
 */
#define ITKSTREAM_PRM_BURR_DECTION_IMAGE_SAVE_RATE ITKSTREAM_PRM(17, 4)

/**
\~chinese
 * @brief 毛刺检测保存路径
 * @note 本参数表示毛刺检测保存路径【RW】。
*/
 /**
\~english
 * @brief Burr detection save path
 * @note This parameter indicates burr detection save path [RW].
 */
#define ITKSTREAM_PRM_BURR_DECTION_IMAGE_SAVE_PATH ITKSTREAM_PRM(18, _MAX_PATH)

/**
\~chinese
 * @brief 毛刺检测保存缓冲区个数
 * @note 本参数表示毛刺检测保存缓冲区个数【RW】。
*/
 /**
\~english
 * @brief Number of buffer for burr detection
 * @note This parameter indicates number of buffer for burr detection [RW].
 */
#define ITKSTREAM_PRM_BURR_DECTION_IMAGE_SAVE_BUFFER_COUNT ITKSTREAM_PRM(19, 4)

/**
\~chinese
 * @brief 数据流批量缓冲区大小
 * @note 本参数表示数据流批量缓冲区大小【RW】。
*/
 /**
\~english
 * @brief Batch buffer size for data stream
 * @note This parameter indicates batch buffer size for data stream [RW].
 */
#define ITKSTREAM_PRM_BATCH_SIZE ITKSTREAM_PRM(20, 4)

/**
\~chinese
 * @brief 数据流参数是否自适应
 * @note 本参数表示数据流参数是否自适应【RW】。
*/
 /**
\~english
 * @brief Whether data stream parameter adaptive
 * @note This parameter indicates whether data stream parameter adaptive [RW].
 */
#define ITKSTREAM_PRM_SELF_ADAPTION ITKSTREAM_PRM(21, 4)

/**
\~chinese
 * @brief 是否使帧开始对应帧结束
 * @note 本参数表示是否使帧开始对应帧结束【RW】。
*/
 /**
\~english
 * @brief Whether let the frame start correspond to the frame end
 * @note This parameter indicates whether let the frame start correspond to the frame end [RW].
 */
#define ITKSTREAM_PRM_FRAME_FRAME_START_CORRESPOND_FRAME_END ITKSTREAM_PRM(22, 4)

/**
\~chinese
 * @brief 取图策略【RW】
 * @see ITKSTREAM_VAL_GRAB_STRATEGY_ONE_BY_ONE ITKSTREAM_VAL_GRAB_STRATEGY_LAST_ONE
*/
 /**
\~english
 * @brief Image acquisition strategy[RW]
 * @see ITKSTREAM_VAL_GRAB_STRATEGY_ONE_BY_ONE ITKSTREAM_VAL_GRAB_STRATEGY_LAST_ONE
 */
#define ITKSTREAM_PRM_GRAB_STRATEGY ITKSTREAM_PRM(23, 4)

/**
\~chinese
 * @brief 采图线程的CPU亲和性【WO】
*/
 /**
\~english
 * @brief Grab Thread CPU Affinity[RW]
 */
#define ITKSTREAM_PRM_GRAB_THREAD_AFFINITY ITKSTREAM_PRM(24, 4)

/**
\~chinese
 * @brief 数据流同步停止采集状态
 * @see ITKSTREAM_PRM_STATUS
*/
 /**
\~english
 * @brief Data stream synchronous stopped acquisition status
 * @see ITKSTREAM_PRM_STATUS
 */
#define ITKSTREAM_VAL_STATUS_STOPPED 0x00000000u

/**
\~chinese
 * @brief 数据流采集状态
 * @see ITKSTREAM_PRM_STATUS
*/
 /**
\~english
 * @brief Data stream acquisition status
 * @see ITKSTREAM_PRM_STATUS
 */
#define ITKSTREAM_VAL_STATUS_ACTIVE 0x00000001u

/**
\~chinese
 * @brief 数据流等待状态
 * @see ITKSTREAM_PRM_STATUS
*/
 /**
\~english
 * @brief Data stream waiting status
 * @see ITKSTREAM_PRM_STATUS
 */
#define ITKSTREAM_VAL_STATUS_PENDING 0x00000002u

/**
\~chinese
 * @brief 数据流异步停止采集状态
 * @see ITKSTREAM_PRM_STATUS
*/
 /**
\~english
 * @brief Data stream asynchronous stopped acquisition status
 * @see ITKSTREAM_PRM_STATUS
 */
#define ITKSTREAM_VAL_STATUS_ABORTED 0x00000003u

/**
\~chinese
 * @brief 数据流超时状态
 * @see ITKSTREAM_PRM_STATUS
*/
 /**
\~english
 * @brief Data stream timeout status
 * @see ITKSTREAM_PRM_STATUS
 */
#define ITKSTREAM_VAL_STATUS_TIMEOUT 0x00000004u

/**
\~chinese
 * @brief 数据流采集开始回调事件
 * @note 本事件在数据流采集开始时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Data stream acquisition start callback event
 * @note This event is triggered at the beginning of data stream acquisition.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_START_OF_STREAM 0x00010000u

/**
\~chinese
 * @brief 数据流采集停止回调事件
 * @note 本事件在数据流采集停止时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Data stream acquisition stop callback event
 * @note This event is triggered when data stream acquisition stops.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_END_OF_STREAM 0x00020000u

/**
\~chinese
 * @brief 帧采集开始回调事件
 * @note 本事件在帧采集开始时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Frame acquisition start callback event
 * @note This event is triggered at the beginning of frame acquisition.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_START_OF_FRAME 0x00040000u

/**
\~chinese
 * @brief 帧采集完成回调事件
 * @note 本事件在帧采集完成时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Frame acquisition completed callback event
 * @note This event is triggered when frame acquisition completed.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_END_OF_FRAME 0x00080000u

/**
\~chinese
 * @brief 采集超时回调事件
 * @note 本事件在采集超时时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
  */
 /**
\~english
 * @brief Acquisition timeout callback event
 * @note This event is triggered when acquisition timeout occurred.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_TIME_OUT 0x00100000u

/**
\~chinese
 * @brief 采集丢帧回调事件
 * @note 本事件在采集丢帧时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Acquisition frame loss callback event
 * @note This event is triggered when acquisition frame loss occurred.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_FRAME_LOST 0x00200000u

/**
\~chinese
 * @brief 图像数据错误回调事件
 * @note 本事件在图像数据错误时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Image data error callback event
 * @note This event is triggered when there is an image data error.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_IMAGE_DATA_ERROR 0x00400000u

/**
\~chinese
 * @brief 设备异常，取流被中止
 * @note 本事件在设备异常(如静电放电)情况下,取流异常时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Device Error, stream is aborted
 * @note This event is triggered when device error(eg.Electro-Static discharge).
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_DEVICE_ERROR 0x00800000u

/**
\~chinese
 * @brief n行数据采集完成回调事件
 * @note 本事件在n行数据采集完成时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief n lines of data acquisition completed callback event
 * @note This event is triggered when the data acquisition of n lines is completed.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_END_OF_LINE 0x01000000u

/**
\~chinese
 * @brief 批量采集完成回调事件
 * @note 本事件在批量采集完成时触发。
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief Batch acquisition completed callback event
 * @note This event is triggered when batch acquisition completed.
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_END_OF_BATCH 0x02000000u

/**
\~chinese
 * @brief 被中止的流成功恢复
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief The aborted stream was recovered successfully
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_STREAM_RECOVERED 0x04000000u

/**
\~chinese
 * @brief 被中止的流恢复失败
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english
 * @brief The aborted stream was recovered failed
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
#define ITKSTREAM_VAL_EVENT_TYPE_STREAM_RECOVER_FAILED 0x08000000u


#define ITKSTREAM_VAL_EVENT_TYPE_MASK 0xFFFF0000u

/**
\~chinese
 * @brief 非阻塞式采集模式
 * @note ItkStreamStart将会立刻返回。
 * @see ITKSTREAM_PRM_START_MODE
*/
 /**
\~english
 * @brief Non blocking acquisition mode
 * @note ItkStreamStart will return immediately.
 * @see ITKSTREAM_PRM_START_MODE
 */
#define ITKSTREAM_VAL_START_MODE_NON_BLOCK 0x00000000u

/**
\~chinese
 * @brief 阻塞式采集模式
 * @note ItkStreamStart将会等待所有图像传输完毕再返回。
 * @see ITKSTREAM_PRM_START_MODE
*/
 /**
\~english
 * @brief Blocking acquisition mode
 * @note ItkStreamStart will wait for all images to be transferred before returning.
 * @see ITKSTREAM_PRM_START_MODE
 */
#define ITKSTREAM_VAL_START_MODE_BLOCK 0x00000001u

/**
\~chinese
 * @brief 异步传输模式
 * @note 无论下一块缓冲区是否为满，始终传输数据到下一块缓冲区中。
 * @see ITKSTREAM_PRM_TRANSFER_MODE
*/
 /**
\~english
 * @brief Asynchronous transmission mode
 * @note Regardless of whether the next buffer is full or not, always transfer data to the next buffer.
 * @see ITKSTREAM_PRM_TRANSFER_MODE
 */
#define ITKSTREAM_VAL_TRANSFER_MODE_ASYNCHRONOUS 0x00000000u

/**
\~chinese
 * @brief 同步传输模式
 * @note 如果下一块缓冲区为空则传输到下一块缓冲区中，否则覆盖当前缓冲区。
 * @see ITKSTREAM_PRM_TRANSFER_MODE
*/
 /**
\~english
 * @brief Synchronous transmission mode
 * @note If the next buffer is empty, transfer to the next buffer; otherwise overwrite the current buffer.
 * @see ITKSTREAM_PRM_TRANSFER_MODE
 */
#define ITKSTREAM_VAL_TRANSFER_MODE_SYNCHRONOUS 0x00000001u

/**
\~chinese
 * @brief 同步保护模式
 * @note 如果下一块缓冲区为空则传输到下一块缓冲区中，否则丢弃图像数据。
 * @see ITKSTREAM_PRM_TRANSFER_MODE
*/
 /**
\~english
 * @brief Synchronous protection mode
 * @note If the next buffer is empty, transfer it to the next buffer, otherwise discard the image data.
 * @see ITKSTREAM_PRM_TRANSFER_MODE
 */
#define ITKSTREAM_VAL_TRANSFER_MODE_SYNCHRONOUS_WITH_PROTECT 0x00000002u

/**
\~chinese
 * @brief 禁用数据流自动清空机制
 * @see ITKSTREAM_PRM_AUTO_CLEAR
*/
 /**
\~english
 * @brief Disable data stream automatic clearing mechanism
 * @see ITKSTREAM_PRM_AUTO_CLEAR
 */
#define ITKSTREAM_VAL_AUTO_CLEAR_DISABLE 0x00000000u

/**
\~chinese
 * @brief 使能数据流自动清空机制
 * @see ITKSTREAM_PRM_AUTO_CLEAR
*/
 /**
\~english
 * @brief Enable data stream automatic clearing mechanism
 * @see ITKSTREAM_PRM_AUTO_CLEAR
 */
#define ITKSTREAM_VAL_AUTO_CLEAR_ENABLE 0x00000001u

/**
\~chinese
 * @brief 取图策略:从旧到新一帧一帧的从输出缓存列表中获取图像，打开设备后默认为该策略
 * @see ITKSTREAM_PRM_GRAB_STRATEGY
*/
 /**
\~english
 * @brief Image acquisition strategy: Obtain images from the output cache list frame by frame from old to new. This strategy is the default after opening the device.
 * @see ITKSTREAM_PRM_GRAB_STRATEGY
 */
#define ITKSTREAM_VAL_GRAB_STRATEGY_ONE_BY_ONE 0x00000000u


/**
\~chinese
 * @brief 取图策略:获取图像缓冲区列表中最后到达的一帧，同时清空缓冲区列表
 * @note 该取图策略必须搭配主动取图接口ItkStreamWaitOneFrameReady使用
 * @see ITKSTREAM_PRM_GRAB_STRATEGY ItkStreamWaitOneFrameReady
*/
 /**
\~english
 * @brief Image acquisition strategy: Obtain the last arriving frame in the image buffer list and clear the buffer list at the same time.
 * @note This image acquisition strategy must be used with the active image acquisition interface: ItkStreamWaitOneFrameReady
 * @see ITKSTREAM_PRM_GRAB_STRATEGY ItkStreamWaitOneFrameReady
 */
#define ITKSTREAM_VAL_GRAB_STRATEGY_LAST_ONE 0x00000001u

/**
\~chinese
 * @brief 连续采集标志
 * @see ItkStreamStart
*/
 /**
\~english
 * @brief Continuous acquisition flag
 * @see ItkStreamStart
 */
#define ITKSTREAM_CONTINUOUS ((uint32_t)-1)

///@}

///@{
/**
\~chinese
 * @name 视图参数
 * @see ItkViewGetPrm ItkViewSetPrm
*/
 /**
\~english
 * @name View parameter
 * @see ItkViewGetPrm ItkViewSetPrm
 */
#define ITKVIEW_PRM_FIRST    0u
#define ITKVIEW_PRM(n, size) ((((n) + ITKVIEW_PRM_FIRST) << 16) | (size))

/**
\~chinese
 * @brief 视图图像水平方向翻转
 * @note 本参数表示视图图像水平方向翻转【RW】。
*/
 /**
\~english
 * @brief Flip view image horizontally
 * @note This parameter indicates the horizontal flipping of view image [RW].
 */
#define ITKVIEW_PRM_FILP_X ITKVIEW_PRM(0, 4)

/**
\~chinese
 * @brief 视图图像垂直方向翻转
 * @note 本参数表示视图图像垂直方向翻转【RW】。
*/
 /**
\~english
 * @brief Flip view image vertically
 * @note This parameter indicates the vertical flipping of view image [RW].
 */
#define ITKVIEW_PRM_FILP_Y ITKVIEW_PRM(1, 4)

/**
\~chinese
 * @brief 视图图像最低bit位
 * @note 本参数表示视图图像最低bit位【RW】。
*/
 /**
\~english
 * @brief View image lowest bit
 * @note This parameter indicates the lowest bit of view image [RW].
 */
#define ITKVIEW_PRM_LSB ITKVIEW_PRM(2, 4)

/**
\~chinese
 * @brief 视图图像最高bit位
 * @note 本参数表示视图图像最高bit位【RW】。
*/
 /**
\~english
 * @brief View image highest bit
 * @note This parameter indicates the highest bit of view image [RW].
 */
#define ITKVIEW_PRM_MSB ITKVIEW_PRM(3, 4)

/**
\~chinese
 * @brief 视图图像感兴区域高度
 * @note 本参数表示视图图像感兴区域高度【RW】。
 * 1. 在创建视图窗口的感兴趣区域时，应先设置高度和宽度，再设置上边缘坐标和左边缘坐标。
*/
 /**
\~english
 * @brief View image ROI height
 * @note This parameter indicates view image ROI height [RW].
 * 1. When creating ROI of view window, the height and width should be set first, followed by the upper and left
edge coordinates.
 */
#define ITKVIEW_PRM_BUFFER_ROI_HEIGHT ITKVIEW_PRM(4, 4)

/**
\~chinese
 * @brief 视图图像感兴区域宽度
 * @note 本参数表示视图图像感兴区域宽度【RW】。
 * 1. 在创建视图窗口的感兴趣区域时，应先设置高度和宽度，再设置上边缘坐标和左边缘坐标。
*/
 /**
\~english
 * @brief View image ROI width
 * @note This parameter indicates view image ROI width [RW].
 * 1. When creating ROI of view window, the height and width should be set first, followed by the upper and left
edge coordinates.
 */
#define ITKVIEW_PRM_BUFFER_ROI_WIDTH ITKVIEW_PRM(5, 4)

/**
\~chinese
 * @brief 视图图像感兴区域左边缘坐标
 * @note 本参数表示视图图像感兴区域左边缘坐标【RW】。
 * 1. 在创建视图窗口的感兴趣区域时，应先设置高度和宽度，再设置上边缘坐标和左边缘坐标。
*/
 /**
\~english
 * @brief View image ROI left edge coordinates
 * @note This parameter indicates view image ROI left edge coordinates [RW].
 * 1. When creating ROI of view window, the height and width should be set first, followed by the upper and left
edge coordinates.
 */
#define ITKVIEW_PRM_BUFFER_ROI_LEFT ITKVIEW_PRM(6, 4)

/**
\~chinese
 * @brief 视图图像感兴区域上边缘坐标
 * @note 本参数表示视图图像感兴区域上边缘坐标【RW】。
 * 1. 在创建视图窗口的感兴趣区域时，应先设置高度和宽度，再设置上边缘坐标和左边缘坐标。
*/
 /**
\~english
 * @brief View image ROI upper edge coordinates
 * @note This parameter indicates view image ROI upper edge coordinates [RW].
 * 1. When creating ROI of view window, the height and width should be set first, followed by the upper and left
edge coordinates.
 */
#define ITKVIEW_PRM_BUFFER_ROI_TOP ITKVIEW_PRM(7, 4)

/**
\~chinese
 * @brief 视图图像放缩方法
 * @note 本参数表示视图图像放缩方法【RW】。
*/
 /**
\~english
 * @brief View image scaling method
 * @note This parameter indicates view image scaling method [RW].
 */
#define ITKVIEW_PRM_ZOOM_METHOD ITKVIEW_PRM(8, 4)

/**
\~chinese
 * @brief 视图图像最大放大倍数
 * @note 本参数表示视图图像最大放大倍数【RO】。
*/
 /**
\~english
 * @brief Maximum magnification of view image
 * @note This parameter indicates the maximum magnification of view image [RO].
 */
#define ITKVIEW_PRM_ZOOM_MAX_RATIO ITKVIEW_PRM(9, 8)

/**
\~chinese
 * @brief 视图图像最小缩小倍数
 * @note 本参数表示视图图像最小缩小倍数【RO】。
*/
 /**
\~english
 * @brief Minimum magnification of view image
 * @note This parameter indicates the minimum magnification of view image [RO].
 */
#define ITKVIEW_PRM_ZOOM_MIN_RATIO ITKVIEW_PRM(10, 8)

/**
\~chinese
 * @brief 视图窗口句柄
 * @note 本参数表示视图窗口句柄【RO】。
*/
 /**
\~english
 * @brief View window handle
 * @note This parameter indicates view window handle [RO].
 */
#define ITKVIEW_PRM_HWND ITKVIEW_PRM(11, 8)

/**
\~chinese
 * @brief 视图窗口标题
 * @note 本参数表示视图窗口标题【RW】。
*/
 /**
\~english
 * @brief View window title
 * @note This parameter indicates view window title [RW].
 */
#define ITKVIEW_PRM_HWND_TITLE ITKVIEW_PRM(12, 128)

/**
\~chinese
 * @brief 视图图像Bayer格式
 * @note 本参数表示视图图像Bayer格式【RW】。
*/
 /**
\~english
 * @brief View image Bayer format
 * @note This parameter indicates view image Bayer format [RW].
 */
#define ITKVIEW_PRM_BAYER_MODE ITKVIEW_PRM(13, 4)

/**
\~chinese
 * @brief 当前视图缓冲区索引
 * @note 本参数表示当前视图缓冲区索引【RO】。
*/
 /**
\~english
 * @brief Current view buffer index
 * @note This parameter indicates the current view buffer index [RO].
 */
#define ITKVIEW_PRM_BUFFER_CURRENT_INDEX ITKVIEW_PRM(14, 4)

/**
\~chinese
 * @brief 不使能视图图像水平翻转
 * @see ITKVIEW_PRM_FLIP_X
*/
 /**
\~english
 * @brief Disable horizontal flipping of view image
 * @see ITKVIEW_PRM_FLIP_X
 */
#define ITKVIEW_VAL_FLIP_X_DISABLE 0x00000000u

/**
\~chinese
 * @brief 使能视图图像水平翻转
 * @see ITKVIEW_PRM_FLIP_X
*/
 /**
\~english
 * @brief Enable horizontal flipping of view image
 * @see ITKVIEW_PRM_FLIP_X
 */
#define ITKVIEW_VAL_FLIP_X_ENABLE 0x00000001u

/**
\~chinese
 * @brief 不使能视图图像垂直翻转
 * @see ITKVIEW_PRM_FLIP_Y
*/
 /**
\~english
 * @brief Disable vertical flipping of view image
 * @see ITKVIEW_PRM_FLIP_Y
 */
#define ITKVIEW_VAL_FLIP_Y_DISABLE 0x00000000u

/**
\~chinese
 * @brief 使能视图图像垂直翻转
 * @see ITKVIEW_PRM_FLIP_Y
*/
 /**
\~english
 * @brief Enable vertical flipping of view image
 * @see ITKVIEW_PRM_FLIP_Y
 */
#define ITKVIEW_VAL_FLIP_Y_ENABLE 0x00000001u

/**
\~chinese
 * @brief 最邻近插值算法
 * @see ITKVIEW_PRM_ZOOM_METHOD
*/
 /**
\~english
 * @brief Nearest neighbor interpolation algorithm
 * @see ITKVIEW_PRM_ZOOM_METHOD
 */
#define ITKVIEW_VAL_ZOOM_NN 0x00000000u

/**
\~chinese
 * @brief 线性插值算法
 * @see ITKVIEW_PRM_ZOOM_METHOD
*/
 /**
\~english
 * @brief Linear interpolation algorithm
 * @see ITKVIEW_PRM_ZOOM_METHOD
 */
#define ITKVIEW_VAL_ZOOM_LINEAR 0x00000001u

/**
\~chinese
 * @brief 立方插值算法
 * @see ITKVIEW_PRM_ZOOM_METHOD
*/
 /**
\~english
 * @brief Cube interpolation algorithm
 * @see ITKVIEW_PRM_ZOOM_METHOD
 */
#define ITKVIEW_VAL_ZOOM_CUBIC 0x00000002u

/**
\~chinese
 * @brief 像素关系重采样算法
 * @see ITKVIEW_PRM_ZOOM_METHOD
*/
 /**
\~english
 * @brief Pixel relation resampling algorithm
 * @see ITKVIEW_PRM_ZOOM_METHOD
 */
#define ITKVIEW_VAL_ZOOM_AREA 0x00000003u

/**
\~chinese
 * @brief 无Bayer排列
 * @see ITKVIEW_PRM_BAYER_MODE
*/
 /**
\~english
 * @brief No Bayer arrangement
 * @see ITKVIEW_PRM_BAYER_MODE
 */
#define ITKVIEW_VAL_BAYER_MODE_NIL 0x00000000u

/**
\~chinese
 * @brief Bayer格式BGGR排列
 * @see ITKVIEW_PRM_BAYER_MODE
*/
 /**
\~english
 * @brief Bayer format BGGR arrangement
 * @see ITKVIEW_PRM_BAYER_MODE
 */
#define ITKVIEW_VAL_BAYER_MODE_BGGR 0x00000001u

/**
\~chinese
 * @brief Bayer格式RGGB排列
 * @see ITKVIEW_PRM_BAYER_MODE
*/
 /**
\~english
 * @brief Bayer format RGGB arrangement
 * @see ITKVIEW_PRM_BAYER_MODE
 */
#define ITKVIEW_VAL_BAYER_MODE_RGGB 0x00000002u

/**
\~chinese
 * @brief Bayer格式GBRG排列
 * @see ITKVIEW_PRM_BAYER_MODE
*/
 /**
\~english
 * @brief Bayer格式GBRG排列
 * @see ITKVIEW_PRM_BAYER_MODE
 */
#define ITKVIEW_VAL_BAYER_MODE_GBRG 0x00000003u

/**
\~chinese
 * @brief Bayer格式GRBG排列
 * @see ITKVIEW_PRM_BAYER_MODE
*/
 /**
\~english
 * @brief Bayer格式GRBG排列
 * @see ITKVIEW_PRM_BAYER_MODE
 */
#define ITKVIEW_VAL_BAYER_MODE_GRBG 0x00000004u
///@}

///@{
/**
\~chinese
 * @name 文件参数
*/
 /**
\~english
 * @name File parameter
 */

/**
\~chinese
 * @brief 以只读方式打开文件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Open files in read only mode
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_READ 0x01u

/**
\~chinese
 * @brief 以只写方式打开文件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Open files in write only mode
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_WRITE 0x02u

/**
\~chinese
 * @brief 打开存在的文件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Open existing files
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_OPEN_EXISTING 0x00u

/**
\~chinese
 * @brief 创建新文件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Create new file
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_CREATE_NEW 0x04u

/**
\~chinese
 * @brief 创建新文件，若当前文件已存在则打开当前文件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Create a new file, and if the current file already exists, open it
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_CREATE_ALWAYS 0x08u

/**
\~chinese
 * @brief 打开文件，若当前文件不存在则创建新文件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Open the file and create a new file if the current file does not exist
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_OPEN_ALWAYS 0x10u

/**
\~chinese
 * @brief 追加打开件
 * @see ItkFileOpen
*/
 /**
\~english
 * @brief Addend open files
 * @see ItkFileOpen
 */
#define ITKFILE_VAL_FA_OPEN_APPEND 0x30u

#define ITKFILE_VAL_AM_RDO 0x01u	///< @brief \~chinese 目录中的只读文件 \~english Read only files in the directory
#define ITKFILE_VAL_AM_HID 0x02u	///< @brief \~chinese目录中的隐藏文件  \~english Hidden files in the directory
#define ITKFILE_VAL_AM_SYS 0x04u	///< @brief \~chinese 目录中的系统文件 \~english System files in the directory
#define ITKFILE_VAL_AM_DIR 0x10u	///< @brief \~chinese 目录中的目录文件 \~english Directory files in the directory
#define ITKFILE_VAL_AM_ARC 0x20u	///< @brief \~chinese 目录中的实现文件 \~english Achievement files in the directory

/**
\~chinese
 * @name 录像文件格式
*/
/**
\~english
* @name The format of record file
*/
typedef enum _ITK_RECORD_FORMAT_TYPE_
{
    ITK_Record_FormatType_Undefined = 0,    ///< \~chinese 未定义的录像格式，默认为AVI格式             \~english Undefined Recode Format Type，default: AVI Recode Format
    ITK_Record_FormatType_AVI = 1,          ///< \~chinese AVI录像格式                  \~english AVI Recode Format Type
    ITK_Record_FormatType_MP4 = 2,          ///< \~chinese MP4录像格式                  \~english MP4 Recode Format Type

}ITK_RECORD_FORMAT_TYPE;
///@}

///@{
/**
\~chinese
 * @name 句柄
 */
/**
\~english
 * @name Handle
 */

/**
\~chinese
* @brief 相机设备句柄
*/
/**
\~english
* @brief Camera device handle
*/
IKAPC_DECLARE_HANDLE(ITKDEVICE);

/**
\~chinese
* @brief 采集卡设备句柄
*/
/**
\~english
* @brief Frame grabber device handle
*/
IKAPC_DECLARE_HANDLE(ITKBOARD);

/**
\~chinese
* @brief 相机特征句柄
*/
/**
\~english
* @brief Camera feature handle
*/
IKAPC_DECLARE_HANDLE(ITKFEATURE);

/**
\~chinese
* @brief 事件信息句柄
*/
/**
\~english
* @brief Event information handle
*/
IKAPC_DECLARE_HANDLE(ITKEVENTINFO);

/**
\~chinese
* @brief 数据流句柄
*/
/**
\~english
* @brief Data stream handle
*/
IKAPC_DECLARE_HANDLE(ITKSTREAM);

/**
\~chinese
* @brief 缓冲区句柄
*/
/**
\~english
* @brief Buffer handle
*/
IKAPC_DECLARE_HANDLE(ITKBUFFER);

/**
\~chinese
* @brief 视图句柄
*/
/**
\~english
* @brief View handle
*/
IKAPC_DECLARE_HANDLE(ITKVIEW);

/**
\~chinese
* @brief 文件句柄
*/
/**
\~english
* @brief File handle
*/
IKAPC_DECLARE_HANDLE(ITKFILE);

/**
\~chinese
* @brief 目录句柄
*/
/**
\~english
* @brief Directory handle
*/
IKAPC_DECLARE_HANDLE(ITKDIR);
///@}

///@{
/**
\~chinese
 * @name 固定成员字符串大小
*/
 /**
\~english
 * @name Fixed member string size
 */
#define ITK_MAX_DEVICE_NUM                (256) ///< @brief \~chinese 最大支持的设备个数       \~english The maximum number of supported devices
#define ITKDEV_INFO_ENTRY_MAX_LENGTH      (64)	///< @brief \~chinese ITKDEV_INFO结构体中字段固定长度64字节 \~english Fixed length of fields in ITKDEV_INFO structure is 64 bytes
#define ITKGIGEDEV_INFO_LENGTH_32_BYTE    (32)	///< @brief \~chinese ITKGIGEDEV_INFO结构体中字段长度32字节 \~english Fixed length of fields in ITKGIGEDEV_INFO structure is 32 bytes
#define ITKGIGEDEV_INFO_LENGTH_64_BYTE    (64)	///< @brief \~chinese ITKGIGEDEV_INFO结构体中字段长度64字节 \~english Fixed length of fields in ITKGIGEDEV_INFO structure is 64 bytes
#define ITKGIGEDEV_INFO_LENGTH_128_BTYE   (128)	///< @brief \~chinese ITKGIGEDEV_INFO结构体中字段长度128字节 \~english Fixed length of fields in ITKGIGEDEV_INFO structure is 128 bytes
#define ITKGIGEDEV_INFO_LENGTH_256_BYTE   (256)	///< @brief \~chinese ITKGIGEDEV_INFO结构体中字段长度256字节 \~english Fixed length of fields in ITKGIGEDEV_INFO structure is 256 bytes
#define ITK_ENTRY_LENGTH_512_BYTE         (512)	///< @brief \~chinese 结构体中字段长度512字节 \~english Fixed length of fields in  structure is 512 bytes
///@}

///@{
/**
\~chinese 
 * @brief 设备事件处理回调函数
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english 
 * @brief Device event handling callback function
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
typedef void(IKAPC_CC *PITKEVENTINFOCALLBACK)(void* pContext, ITKEVENTINFO hEventInfo);

/**
\~chinese 
 * @brief 数据流事件处理回调函数
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback
*/
 /**
\~english 
 * @brief Data stream event handling callback function
 * @see ItkStreamRegisterCallback ItkStreamUnregisterCallback
 */
typedef void(IKAPC_CC* PITKSTREAMCALLBACK)(uint32_t eventType, void* pContext);

/**
\~chinese 
 * @brief 视图事件处理回调函数
 * @see ItkViewRegisterCallback ItkViewUnregisterCallback
*/
 /**
\~english 
 * @brief View event handling callback function
 * @see ItkViewRegisterCallback ItkViewUnregisterCallback
 */
typedef void(IKAPC_CC* PITKVIEWCALLBACK)(uint32_t eventType, void* pContext);
///@}

/**
\~chinese 
 * @brief 相机设备基本信息结构体
 * @see ItkManGetDeviceInfo
*/
 /**
\~english 
 * @brief Basic information structure of camera device
 * @see ItkManGetDeviceInfo
 */
typedef struct tag_itk_dev_info {
    char FullName[ITKDEV_INFO_ENTRY_MAX_LENGTH];         ///< @brief \~chinese 用于识别设备的唯一名称 \~english Unique name used to identify the device
    char FriendlyName[ITKDEV_INFO_ENTRY_MAX_LENGTH];     ///< @brief \~chinese 用户界面显示的设备名称 \~english Device name displayed on user interface
    char VendorName[ITKDEV_INFO_ENTRY_MAX_LENGTH];       ///< @brief \~chinese 设备生产厂商 \~english Device manufacturer
    char ModelName[ITKDEV_INFO_ENTRY_MAX_LENGTH];        ///< @brief \~chinese 设备模型名称 \~english Device model name
    char SerialNumber[ITKDEV_INFO_ENTRY_MAX_LENGTH];     ///< @brief \~chinese 设备序列号 \~english Device serial number
    char DeviceClass[ITKDEV_INFO_ENTRY_MAX_LENGTH];      ///< @brief \~chinese 设备传输层类型 \~english Device transmission layer type
    char DeviceVersion[ITKDEV_INFO_ENTRY_MAX_LENGTH];    ///< @brief \~chinese 设备固件版本 \~english Device firmware version
    char UserDefinedName[ITKDEV_INFO_ENTRY_MAX_LENGTH];  ///< @brief \~chinese 用户自定义名称 \~english User defined name
} ITKDEV_INFO, *PITKDEV_INFO;

/**
\~chinese 
 * @brief 相机设备列表结构体
 * @see ItkManGetDeviceCountEx
*/
 /**
\~english 
 * @brief Structure of camera device list
 * @see ItkManGetDeviceCountEx
 */
typedef struct _ITK_DEVICE_INFO_LIST_ {
    uint32_t             nDevNum;                                ///< @brief \~chinese 设备数量 \~english Device Number                            
    PITKDEV_INFO         pItkDevInfo[ITK_MAX_DEVICE_NUM];        ///< @brief \~chinese 支持最多256个设备 \~english Support up to 256 devices
}ITK_DEVICE_INFO_LIST,*PITK_DEVICE_INFO_LIST;

/**
\~chinese 
 * @brief 千兆网相机设备专有信息结构体
 * @see ItkManGetGigEDeviceInfo
*/
 /**
\~english 
 * @brief Gigabit network camera device specific information structure
 * @see ItkManGetGigEDeviceInfo
 */
typedef struct tag_itk_gige_dev_info {
    char     MAC[ITKGIGEDEV_INFO_LENGTH_32_BYTE];                 ///< @brief \~chinese 设备MAC地址 \~english Device MAC address
    char     Ip[ITKGIGEDEV_INFO_LENGTH_32_BYTE];                  ///< @brief \~chinese 设备IP地址 \~english Device IP address
    char     SubNetMask[ITKGIGEDEV_INFO_LENGTH_32_BYTE];          ///< @brief \~chinese 设备子网掩码 \~english Device subnet mask
    char     GateWay[ITKGIGEDEV_INFO_LENGTH_32_BYTE];             ///< @brief \~chinese 设备默认网关 \~english Device default gateway
    char     NICMac[ITKGIGEDEV_INFO_LENGTH_32_BYTE];              ///< @brief \~chinese 通讯主机MAC地址 \~english Communication host MAC address
    char     NICIp[ITKGIGEDEV_INFO_LENGTH_32_BYTE];               ///< @brief \~chinese 通讯主机IP地址 \~english Communication host IP address
    char     NICSubNetMask[ITKGIGEDEV_INFO_LENGTH_32_BYTE];       ///< @brief \~chinese 通讯主机子网掩码 \~english Communication host subnet mask
    char     NICGateWay[ITKGIGEDEV_INFO_LENGTH_32_BYTE];          ///< @brief \~chinese 通讯主机默认网关 \~english Default gateway for communication host
    char     NICAdapterName[ITKGIGEDEV_INFO_LENGTH_256_BYTE + 4]; ///< @brief \~chinese 通讯主机网卡名称 \~english Name of communication host network card
    char     NICDescription[ITKGIGEDEV_INFO_LENGTH_128_BTYE + 4]; ///< @brief \~chinese 通讯主机网卡详细描述 \~english Detailed description of communication host network card
    uint32_t NICSpeed;                                            ///< @brief \~chinese 通讯主机网卡速度 \~english Communication host network card speed
    char     reserved[124];                                       ///< @brief \~chinese 预留字段 \~english Reserved field
} ITKGIGEDEV_INFO, *PITKGIGEDEV_INFO;

/**
\~chinese 
 * @brief Camera Link相机设备专有信息结构体
 * @see ItkManGetCLDeviceInfo
*/
 /**
\~english 
 * @brief Camera Link camera device specific information structure
 * @see ItkManGetCLDeviceInfo
 */
typedef struct tag_itk_cl_dev_info {
    uint32_t HostInterface;  ///< @brief \~chinese 主接口类型 \~english Main interface type
    uint32_t BoardIndex;     ///< @brief \~chinese 板卡索引 \~english Board index
    char     SerialPort[32]; ///< @brief \~chinese CL设备串口号 \~english CL device serial port number
    char     Reserved[224];  ///< @brief \~chinese 预留字段 \~english Reserved field
} ITK_CL_DEV_INFO, *PITK_CL_DEV_INFO;

/**
\~chinese 
 * @brief CoaXPress相机设备专有信息结构体
 * @see ItkManGetCXPDeviceInfo
*/
 /**
\~english 
 * @brief CoaXPress camera device specific information structure
 * @see ItkManGetCXPDeviceInfo
 */
typedef struct tag_itk_cxp_dev_info {
    uint32_t BoardIndex;    ///< @brief \~chinese 板卡索引 \~english Board index
    uint32_t MasterPort;    ///< @brief \~chinese 主端口号 \~english Main port
    uint32_t SlaveCount;    ///< @brief \~chinese 从端口数量 \~english Number of slave ports
    uint32_t SlavePort[7];  ///< @brief \~chinese 从端口号 \~english Slave port index
    uint32_t CameraId;      ///< @brief \~chinese 相机ID \~english Camera ID
    uint32_t Topology;      ///< @brief \~chinese 拓扑连接 \~english Topological connection
    char     Reserved[248]; ///< @brief \~chinese 预留字段 \~english Reserved field
} ITK_CXP_DEV_INFO, *PITK_CXP_DEV_INFO;

/**
\~chinese 
 * @brief USB30相机设备专有信息结构体
 * @see ItkManGetU3VDeviceInfo
*/
 /**
\~english 
 * @brief USB30 camera device specific information structure
 * @see ItkManGetU3VDeviceInfo
 */
typedef struct tag_itk_u3v_dev_info {
    uint32_t VID;           ///< @brief \~chinese 相机VID \~english Camera VID
    uint32_t PID;           ///< @brief \~chinese 相机PID \~english Camera PID
    uint32_t USBVersion;    ///< @brief \~chinese USB连接版本 \~english USB connection version
    char     Reserved[256]; ///< @brief \~chinese 预留字段 \~english Reserved field
} ITK_U3V_DEV_INFO, *PITK_U3V_DEV_INFO;

/**
\~chinese 
 * @brief GigEVision Board设备专有信息结构体
 * @see ItkManGetGVBDeviceInfo
*/
 /**
\~english 
 * @brief GigEVision Board device specific information structure
 * @see ItkManGetGVBDeviceInfo
 */
typedef struct tag_itk_gvb_dev_info {
    uint32_t BoardIndex;     ///< @brief \~chinese 板卡索引 \~english Board index
    uint32_t MasterPort;     ///< @brief \~chinese 主端口号 \~english Main port
    char     MAC[32];        ///< @brief \~chinese MAC地址 \~english MAC address
    char     Ip[32];         ///< @brief \~chinese IP地址 \~english IP address
    char     SubNetMask[32]; ///< @brief \~chinese 子网掩码 \~english Subnet mask
    char     GateWay[32];    ///< @brief \~chinese 默认网关 \~english Default gateway
    uint32_t SlaveCount;     ///< @brief \~chinese 从端口数量 \~english Number of slave ports
    uint32_t SlavePort[3];   ///< @brief \~chinese 从端口号 \~english Slave port index
    uint32_t EffectPixel;    ///< @brief \~chinese 有效像素个数 \~english Effective number of pixels
    char     Reserved2[140]; ///< @brief \~chinese 预留字段 \~english Reserved field
} ITK_GVB_DEV_INFO, *PITK_GVB_DEV_INFO;

/**
\~chinese 
 * @brief 采集卡设备专有信息结构体
 * @see ItkManGetBoardInfo
*/
 /**
\~english 
 * @brief Frame grabber Board device specific information structure
 * @see ItkManGetBoardInfo
 */
typedef struct tag_itk_board_info {
    char Name[ITKDEV_INFO_ENTRY_MAX_LENGTH]; ///< @brief \~chinese 用于标识的设备名称 \~english Device name used for identification
    char Reserved[256];                      ///< @brief \~chinese 预留字段 \~english Reserved field
} ITKBOARD_INFO, *PITKBOARD_INFO;

/**
\~chinese 
 * @brief 文件专有信息结构体
 * @see ItkDirRead ItkFileStat
*/
 /**
\~english 
 * @brief File specific information structure
 * @see ItkDirRead ItkFileStat
 */
#pragma pack(push, 1)
typedef struct tag_itk_file_info {
    uint32_t fsize;                                   ///< @brief \~chinese 文件大小 \~english File size
    uint8_t  fattrib;                                 ///< @brief \~chinese 文件属性 \~english File attribution
    char     fname[ITKDEV_INFO_ENTRY_MAX_LENGTH + 1]; ///< @brief \~chinese 文件名称 \~english File name
} ITKFILE_INFO;
#pragma pack(pop)

/**
\~chinese 
 * @brief 批量缓冲区专有信息结构体
*/
 /**
\~english 
 * @brief Batch buffer specific information structure
 */
typedef struct tag_itk_batch_buffer_status {
    uint64_t blockId;       ///< @brief \~chinese 区块ID \~english Block ID
    int64_t  ready_lines;   ///< @brief \~chinese 已完成行数 \~english Completed lines
    uint64_t RsndPktCount;  ///< @brief \~chinese 重传包数 \~english Resend packet count
    uint64_t LostPktCount;  ///< @brief \~chinese 丢包数 \~english Lost packet count
    char     Reserved[240]; ///< @brief \~chinese 预留字段 \~english Reserved field
} ITKBATCH_BUFFER_STATUS, *PITKBATCH_BUFFER_STATUS;

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief 特征基本信息结构体
 * @see ItkDevGetFeatureInfo
*/
 /**
\~english 
 * @brief Basic information structure of feature
 * @see ItkDevGetFeatureInfo
 */
typedef struct _itk_feature_base_info {
    uint8_t     IsStreamable;                       ///< @brief \~chinese 是否可以被输出到文件 \~english Indicate whether it is streamable
    uint8_t     IsDeprecated;                       ///< @brief \~chinese 是否被弃用 \~english Indicate whether it is deprecated
    uint8_t     IsSelector;                         ///< @brief \~chinese 是否是选择器 \~english Indicate whether it is selector 
    uint8_t     NameSpace;                          ///< @brief \~chinese 命名空间 \~english Namespace \~ @see ITKFEATURE_VAL_NAME_SPACE_LIST
    uint8_t     Type;                               ///< @brief \~chinese 特征类型 \~english Feature type \~ @see ITKFEATURE_VAL_TYPE_LIST
    uint8_t     Visibility;                         ///< @brief \~chinese 可视级别 \~english Visibilty \~ @see ITKFEATURE_VAL_VISIBILITY_LIST
    uint8_t     Representation;                     ///< @brief \~chinese 表现方式 \~english Representation \~ @see ITKFEATURE_VAL_REPRESENTATION_LIST
    uint8_t     CachingMode;                        ///< @brief \~chinese 缓存模式 \~english Cacheing mode \~ @see ITKFEATURE_VAL_CACHING_MODE_LIST
    uint8_t     EnumCount;                          ///< @brief \~chinese 枚举特征的枚举个数 \~english Enum Count of enum feature.
    uint8_t     Reserved_8[31];
    int64_t     Length;                             ///< @brief \~chinese 特征字节数 \~english Feature bytes
    int64_t     PollingTime;                        ///< @brief \~chinese 轮询时间 \~english Polling time suggested
    char        Unit[32];                           ///< @brief \~chinese 单位 \~english Unit
    char        EventID[32];                        ///< @brief \~chinese 事件ID \~english Event ID 
    char        Name[64];                           ///< @brief \~chinese 名字 \~english Name
    char        Category[64];                       ///< @brief \~chinese 所属目录 \~english Category
    char        DisplayName[64];                    ///< @brief \~chinese 显示名 \~english DisplayName
    char        Tooltip[512];                       ///< @brief \~chinese 提示信息 \~english Tooltip
    char        Description[512];                   ///< @brief \~chinese 描述 \~english Description
    char        Reserved_char[512];                  
} ITK_FEATURE_BASE_INFO, *PITK_FEATURE_BASE_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief Int64特征信息结构体
 * @see ItkDevGetInt64FeatureInfo
*/
 /**
\~english 
 * @brief Information structure of int64 feature
 * @see ItkDevGetInt64FeatureInfo
 */
typedef struct _itk_feature_int64_info {
    int64_t Min;                                    ///< @brief \~chinese 最小值 \~english Min
    int64_t Max;                                    ///< @brief \~chinese 最大值 \~english Max
    int64_t Inc;                                    ///< @brief \~chinese 步距 \~english Inc
    int64_t Reserved[5];
} ITK_FEATURE_INT64_INFO, *PITK_FEATURE_INT64_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief Double特征信息结构体
 * @see ItkDevGetDoubleFeatureInfo
*/
 /**
\~english 
 * @brief Information structure of double feature
 * @see ItkDevGetDoubleFeatureInfo
 */
typedef struct _itk_feature_double_info {
    double Min;                                             ///< @brief \~chinese 最小值 \~english Min
    double Max;                                             ///< @brief \~chinese 最大值 \~english Max
    double Inc;                                             ///< @brief \~chinese 步距 \~english Inc
    int64_t Precision;                                      ///< @brief \~chinese 精度 \~english Precision
    int64_t DisplayNotation;                                ///< @brief \~chinese 显示方式 \~english DisplayNotation
    int64_t Reserved[3];
} ITK_FEATURE_DOUBLE_INFO, *PITK_FEATURE_DOUBLE_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief Enum entry特征信息结构体
 * @see ItkDevGetEnumEntryFeatureInfo
*/
 /**
\~english 
 * @brief Information structure of enum entry feature
 * @see ItkDevGetEnumEntryFeatureInfo
 */
typedef struct _itk_feature_enum_entry_info {
    int64_t Value;                                                      ///< @brief \~chinese 整数值 \~english Int value
    int64_t AccessMode;                                                 ///< @brief \~chinese 访问权限 \~english Access mode
    char    ValueStr[64];                                               ///< @brief \~chinese 字符串值 \~english String value
    char    DisplayName[64];                                            ///< @brief \~chinese 显示名 \~english Display name
    char    Tooltip[512];                                               ///< @brief \~chinese 提示信息 \~english Tooltip
    char    Description[512];                                           ///< @brief \~chinese 描述信息 \~english Description
} ITK_FEATURE_ENUM_ENTRY_INFO, *PITK_FEATURE_ENUM_ENTRY_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief 缓冲区信息结构体
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english 
 * @brief Information structure of buffer
 * @see ITKBUFFER_FORMAT
 */
typedef struct _itk_buffer_info {
    uint8_t         State;                  ///< @brief \~chinese 缓冲区状态 \~english Buffer state
    uint8_t         HasChunkData;           ///< @brief \~chinese 是否有chunkdata \~english Indicate whether buffer has chunk data 
    uint8_t         NeedAutoConvert;        ///< @brief \~chinese 是否需要使用ItkBufferConvert自动转化 \~english Indicate whether buffer it need use ItkBufferConvert to auto convert.
    PAYLOAD_TYPE_T  PayloadType;            ///< @brief \~chinese 缓冲区负载格式 \~english Buffer payload type
    uint8_t         Reserved8[4];
    void *          BaseAddress;            ///< @brief \~chinese 缓冲区地址，仅在图像大小和缓冲区大小不一致时使用 \~english Buffer address, only used when image size and buffer size are not consistent
    void *          ImageAddress;           ///< @brief \~chinese 缓冲区图像地址 \~english Image address in buffer
    uint64_t        ValidImageHeight;       ///< @brief \~chinese 缓冲区当前采集完成的图像行数 \~english The current number of lines grabbed in image.
    uint64_t        ValidImageSize;         ///< @brief \~chinese 缓冲区当前采集完成的图像大小 \~english The current valid image size grabbed in buffer.
    uint64_t        ImageWidth;             ///< @brief \~chinese 缓冲区宽度 \~english Buffer width
    uint64_t        ImageHeight;            ///< @brief \~chinese 缓冲区高度 \~english Buffer height
    uint64_t        PixelFormat;            ///< @brief \~chinese 缓冲区数据格式 \~english Buffer data format
    uint64_t        ImagePixelDepth;        ///< @brief \~chinese 缓冲区像素总深度 \~english Buffer pixel total depth
    uint64_t        ImageSize;              ///< @brief \~chinese 缓冲区图像数据大小 \~english Buffer image size
    uint64_t        TotalSize;              ///< @brief \~chinese 缓冲区总大小，仅在图像大小和缓冲区大小不一致时使用 \~english Buffer total size, only used when image size and buffer size are not consistent
    uint64_t        BlockID;                ///< @brief \~chinese 缓冲区ID \~english Block ID 
    uint64_t        ErrorReason;            ///< @brief \~chinese 缓冲区错误原因 \~english Reason for buffer error
    uint64_t        TimestampNs;            ///< @brief \~chinese 缓冲区状态为满时的时间戳 \~english Timestamp when buffer status is full
    uint64_t        GevResendPacketCount;   ///< @brief \~chinese 当前帧图像重包数 \~english Number of image resend packet in the current frame
    uint64_t        GevLostPacketCount;     ///< @brief \~chinese 当前帧图像丢包数 \~english Current frame image packet loss count
    uint64_t        FrameStartTimestampFromCameraNs;  ///< @brief \~chinese 帧开始的相机时间戳 \~english Camera's FrameStartTimestamp
    uint64_t        PartNumber;             ///< @brief \~chinese 缓冲区负载为MULTI_PART_PAYLOAD_TYPE的情况下的part数量 \~english Part number when PayloadType is MULTI_PART_PAYLOAD_TYPE
    uint64_t        Reserved64[22];         
} ITK_BUFFER_INFO, *PITK_BUFFER_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief 缓冲区信息结构体
 * @see ITKBUFFER_FORMAT
*/
 /**
\~english 
 * @brief Information structure of buffer
 * @see ITKBUFFER_FORMAT
 */

#   pragma pack (push, 8)
typedef struct _itk_buffer_part_info {
    ITKBUFFER       Buffer;            ///< @brief \~chinese 表示缓冲区part的句柄 \~english Handle of buffer part
    uint64_t        SourceId;          ///< @brief \~chinese 标识符，用于对属于同一来源的数据部分进行分组 \~english Identifier allowing to group data parts belonging to the same source
    uint64_t        RegionId;          ///< @brief \~chinese 标识符，用于对属于同一区域的数据部分进行分组 \~english Identifier allowing to group data parts belonging to the same region
    uint64_t        PurposeId;         ///< @brief \~chinese 标识符，用于对属于同一目的的数据部分进行分组 \~english Identifier allowing to group data parts belonging to the same purpose
    uint64_t        Reserved[11];
    char            Name[64];          ///< @brief \~chinese 缓冲区名字 \~english Buffer name
    ITK_BUFFER_INFO BufferInfo;        ///< @brief \~chinese 缓冲区信息 \~english Buffer info
} ITK_BUFFER_PART_INFO, *PITK_BUFFER_PART_INFO;

#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief 动作命令信息结构体
 * @see ItkGigEDevIssueActionCommand
*/
 /**
\~english 
 * @brief Action Command Infomation
 * @see ItkGigEDevIssueActionCommand
 */
typedef struct _itk_action_cmd_info
{
    uint32_t        DeviceKey;                                 ///< @brief  \~chinese 设备密钥                                   \~english Device Key;
    uint32_t        GroupKey;                                  ///< @brief  \~chinese 组键                                       \~english Group Key
    uint32_t        GroupMask;                                 ///< @brief  \~chinese 组掩码                                     \~english Group Mask
    uint32_t        ActionTimeEnable;                          ///< @brief  \~chinese 只有设置成1时Action Time才有效，非1时无效  \~english Action Time Enable
    uint64_t        ActionTime;                                ///< @brief  \~chinese 预定的时间，和主频有关                     \~english Action Time
    char            BroadcastAddress[32];                      ///< @brief  \~chinese 广播包地址                                 \~english Broadcast Address
    uint32_t        TimeOut;                                   ///< @brief  \~chinese 等待ACK的超时时间，如果为0表示不需要ACK    \~english TimeOut
    uint32_t        SpecialNetEnable;                          ///< @brief  \~chinese 只有设置成1时指定的网卡IP才有效，非1时无效 \~english Special IP Enable
    uint32_t        SpecialNetIP;                              ///< @brief  \~chinese 指定的网卡IP                               \~english Special Net IP address
    uint32_t        Reserved[14];                              ///< @brief  \~chinese 预留                                       \~english Reserved
}ITK_ACTION_CMD_INFO, *PITK_ACTION_CMD_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief 动作命令返回信息结构体
 * @see ItkGigEDevIssueActionCommand
*/
 /**
\~english 
 * @brief Action Command Result Infomation
 * @see ItkGigEDevIssueActionCommand
 */
typedef struct _itk_action_cmd_result
{
    char                IpAddress[12 + 3 + 1];                     ///< @brief \~chinese 设备IP                 \~english IP address of the device
    int                 Status;                                    ///< @brief \~chinese 状态码                 \~english status code returned by the device
                                                                    //1.0x0000:success.
                                                                    //2.0x8001:Command is not supported by the device.
                                                                    //3.0x8013:The device is not synchronized to a master clock to be used as time reference.
                                                                    //4.0x8015:A device queue or packet data has overflowed.
                                                                    //5.0x8016:The requested scheduled action command was requested at a time that is already past.
    unsigned int        Reserved[4];                               ///< @brief     \~chinese 预留                \~english Reserved
}ITK_ACTION_CMD_RESULT, *PITK_ACTION_CMD_RESULT;
#   pragma pack (pop)

/**
\~chinese 
 * @brief 动作命令返回信息列表
 * @see ITK_ACTION_CMD_RESULT PITK_ACTION_CMD_RESULT
*/
 /**
\~english 
 * @brief Action Command Result List
 * @see ITK_ACTION_CMD_RESULT PITK_ACTION_CMD_RESULT
 */
typedef struct _itk_action_cmd_result_list
{
    uint32_t            NumResults;                            ///< @brief \~chinese 返回值个数             \~english Number of returned values
    PITK_ACTION_CMD_RESULT  Results;                           ///< @brief \~chinese 动作命令结果           \~english Reslut of action command
}ITK_ACTION_CMD_RESULT_LIST, *PITK_ACTION_CMD_RESULT_LIST;


#   pragma pack (push, 8)
/**
\~chinese 
 * @brief DevEvent info 结构体
 * @see ItkEventGetInfo
*/
 /**
\~english 
 * @brief Information structure of enum entry feature
 * @see ItkEventGetInfo
 */
typedef struct _itk_DevEvent_info {
    char        eventName[128];           ///< @brief  \~chinese 事件名称                                                       \~english Event name
    char        eventSource[128];         ///< @brief  \~chinese 事件触发源,"FeatureValueChanged/MessageChannel"事件下支持      \~english Event trigger source，supported in "FeatureValueChanged/MessageChannel" event

    uint64_t    blockID;                  ///< @brief  \~chinese 帧号                                                           \~english Block ID
                                           
    uint16_t    eventID;                  ///< @brief  \~chinese Event号                                                        \~english Event ID
    uint64_t    timestamp;                ///< @brief  \~chinese 事件时间戳                                                     \~english The timestamp of the event
    void*       eventData;                ///< @brief  \~chinese Event数据(暂无固件支持)                                        \~english Event data, not support
    uint32_t    eventDataLength;          ///< @brief  \~chinese Event数据长度 (暂无固件支持)                                   \~english Event data len, not support

    char        reversed[32];             ///< @brief  \~chinese 预留                                                           \~english Reserved
} ITK_DEVEVENT_INFO, *PITK_DEVEVENT_INFO;
#   pragma pack (pop)

#   pragma pack (push, 8)
/**
\~chinese 
 * @brief 录像信息结构体
 * @see ItkStreamStartRecord
*/
 /**
\~english 
 * @brief Information structure of record
 * @see ItkStreamStartRecord
 */
typedef struct _ITK_RECORD_PARAM_ {
    uint32_t                 nPixelFormat;         ///< @brief  \~chinese 输入数据的像素格式     \~english Pixel Type
    uint64_t                 nWidth;               ///< @brief  \~chinese 图像宽                 \~english Width
    uint64_t                 nHeight;              ///< @brief  \~chinese 图像高                 \~english Height

    float                    fFrameRate;           ///< @brief  \~chinese 帧率fps(大于1/16)      \~english The Rate of Frame
    ITK_RECORD_FORMAT_TYPE   nRecordFmtType;       ///< @brief  \~chinese 录像格式               \~english Recode Format Type
    const char*              strFilePath;          ///< @brief  \~chinese 录像文件存放路径       \~english File Path

    unsigned int            nRes[16];              ///< @brief  \~chinese 预留                   \~english Reserved
} ITK_RECORD_PARAM, *PITK_RECORD_PARAM;
#   pragma pack (pop)


#pragma pack(pop)

#endif
// clang-format on
