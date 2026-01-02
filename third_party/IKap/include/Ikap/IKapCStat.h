// clang-format off
/**
\~chinese
 * @file IKapCStat.h
 * @brief IKapC库函数返回值
 */
 /**
\~english
 * @file IKapCStat.h
 * @brief IKapC library function return value
 */
#ifndef _IKAPCSTAT_H
#define _IKAPCSTAT_H

#include "IKapCType.h"

/**
\~chinese 
 * @brief 函数返回值定义
 * @note 函数返回值是一个32bit数据，数据各位描述如下所示：
 * | 位数    | 描述       |
 * |---------|------------|
 * | [0:15]  | 错误码ID   |
 * | [16:19] | 错误类别ID |
 * | [20:23] | 模块ID     |
 * | [24:31] | 预留       |
 */ 
 /** 
 \~english 
 * @brief Function return value definition
 * @note The return value of function is a 32bit data, which is described as follows:
 * | Bit      | Description   |
 * |----------|---------------|
 * | [0:15]   | Error code ID |
 * | [16:19]  | Error type ID |
 * | [20:23]  | Module ID     |
 * | [24:31]  | Reversed      |
 */
#define ITKSTATUS uint32_t
#define ITKSTATUS_MODULE_BIT_OFFSET 20u	 ///< @brief \~chinese 模块ID偏移位置 \~english Module ID offset
#define ITKSTATUS_LEVEL_BIT_OFFSET  16u	 ///< @brief \~chinese 错误类别ID偏移位置 \~english Error type ID offset
#define ITKSTATUS_STATUS_BIT_OFFSET 0u	 ///< @brief \~chinese 错误码ID偏移位置 \~english Error code ID offset
#define ITKSTATUS_MODULE_BIT_MASK 0x0Fu	 ///< @brief \~chinese 模块ID切块 \~english Module ID mask
#define ITKSTATUS_LEVEL_BIT_MASK  0x0Fu	 ///< @brief \~chinese 错误类别ID切块 \~english Error type ID mask
#define ITKSTATUS_STATUS_BIT_MASK 0xFFFFu ///< @brief \~chinese 错误码ID切块 \~english Error code ID mask

///@{
/**
\~chinese 
 * @name 模块ID
 * @see ItkManGetStatusText
 */ 
 /**
 \~english 
 * @name Module ID
 * @see ItkManGetStatusText
 */
#define ITKSTATUS_MODULE(status) (((status) >> ITKSTATUS_MODULE_BIT_OFFSET) & ITKSTATUS_MODULE_BIT_MASK)
#define ITKSTATUS_MODULE_UNKNOWN   0x00u	///< @brief \~chinese 未指明的错误 \~english Unknown error
#define ITKSTATUS_MODULE_DEVICE    0x01u	///< @brief \~chinese 设备模块 \~english Device module
#define ITKSTATUS_MODULE_BUFFER    0x02u	///< @brief \~chinese 缓冲区模块 \~english Buffer module
#define ITKSTATUS_MODULE_LOG       0x03u	///< @brief \~chinese 日志模块 \~english Log module
#define ITKSTATUS_MODULE_MANAGER   0x04u	///< @brief \~chinese 全局模块 \~english Management module
#define ITKSTATUS_MODULE_STREAM    0x05u	///< @brief \~chinese 数据流模块 \~english Data stream module
#define ITKSTATUS_MODULE_PARAM     0x06u	///< @brief \~chinese 参数模块 \~english Parameter module
#define ITKSTATUS_MODULE_SERIAL    0x07u	///< @brief \~chinese 串口模块  \~english Serial port module
#define ITKSTATUS_MODULE_EVENTINFO 0x08u	///< @brief \~chinese 事件信息模块 \~english Event information module
#define ITKSTATUS_MODULE_FEATURE   0x09u	///< @brief \~chinese 特征模块 \~english Feature module
#define ITKSTATUS_MODULE_VIEW      0x0Au	///< @brief \~chinese 视图模块 \~english View module
#define ITKSTATUS_MODULE_BOARD     0x0Bu	///< @brief \~chinese 采集卡模块 \~english Frame grabber module
#define ITKSTATUS_MODULE_FILE      0x0Cu	///< @brief \~chinese 文件模块 \~english File module
///@}

///@{
/**
\~chinese 
 * @name 错误类别ID
 * @see ItkManGetStatusText
 */ 
 /**
 \~english 
 * @name Error type ID
 * @see ItkManGetStatusText
 */
#define ITKSTATUS_LEVEL(status) (((status) >> ITKSTATUS_LEVEL_BIT_OFFSET) & ITKSTATUS_LEVEL_BIT_MASK)
#define ITKSTATUS_LEVEL_FAT 0u	///< @brief \~chinese 致命错误 \~english Fatal error
#define ITKSTATUS_LEVEL_ERR 1u	///< @brief \~chinese 普通错误 \~english Ordinary error
#define ITKSTATUS_LEVEL_WRN 2u	///< @brief \~chinese 警告 \~english Warning
#define ITKSTATUS_LEVEL_INF 3u	///< @brief \~chinese 信息 \~english Information
///@}

///@{
/**
\~chinese 
 * @name 错误码ID
 * @see ItkManGetStatusText
 */ 
 /**
 \~english 
 * @name Error code ID
 * @see ItkManGetStatusText
 */
#define ITKSTATUS_ID(status) ((status)&ITKSTATUS_STATUS_BIT_MASK)
#define ITKSTATUS_OK                         0x0000u	///< @brief \~chinese 成功 \~english Successful
#define ITKSTATUS_INVALID_HANDLE             0x0001u	///< @brief \~chinese 无效的句柄 \~english Invalid handle
#define ITKSTATUS_INSUFFICIENT_RESOURCES     0x0002u	///< @brief \~chinese 资源不足 \~english Insufficient resources
#define ITKSTATUS_BUFFER_TOO_SMALL           0x0003u	///< @brief \~chinese 输入缓冲区太小 \~english Input buffer is too small
#define ITKSTATUS_MISSING_RESOURCE           0x0004u	///< @brief \~chinese 资源缺失 \~english Resources missing
#define ITKSTATUS_UNINITIALIZE               0x0005u	///< @brief \~chinese IKapC没有初始化运行环境 \~english IKapC has no initialized runtime environment
#define ITKSTATUS_DEVICE_ID_OUTOF_RANGE      0x0006u	///< @brief \~chinese 设备索引越界 \~english Device index is out of range
#define ITKSTATUS_SERAIL_PORT_NOT_AVAILABLE  0x0007u	///< @brief \~chinese 设备串口不可用 \~english Serial port is unavailable
#define ITKSTATUS_XML_NOT_FOUND              0x0008u	///< @brief \~chinese 设备XML描述文件丢失 \~english Device XML description file is lost
#define ITKSTATUS_DEVICE_NOT_ACCESSABLE      0x0009u	///< @brief \~chinese 设备无法访问 \~english Device is inaccessible
#define ITKSTATUS_DEVICE_PERMISSION_DENY     0x000Au	///< @brief \~chinese 设备访问被拒绝 \~english Device access is denied
#define ITKSTATUS_REGISTRY_NOT_FOUND         0x000Bu	///< @brief \~chinese 注册表丢失 \~english Registry missing
#define ITKSTATUS_XML_PARSE_ERROR            0x000Cu	///< @brief \~chinese XML文件无法解析 \~english XML file cannot be parsed
#define ITKSTATUS_INVALID_ARG                0x000Du	///< @brief \~chinese 无效的输入参数 \~english Invalid input parameter
#define ITKSTATUS_INVALID_NAME               0x000Eu	///< @brief \~chinese 无效的特征名称 \~english Invalid feature name
#define ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE  0x000Fu	///< @brief \~chinese 特征类型不匹配函数类型 \~english Feature type does not match function type
#define ITKSTATUS_TIME_OUT                   0x0010u	///< @brief \~chinese 操作超时 \~english Operation timed out
#define ITKSTATUS_COMMAND_CRASH              0x0011u	///< @brief \~chinese 相机命令冲突 \~english Camera command conflict
#define ITKSTATUS_COMMAND_PARAM_OUT_OF_RANGE 0x0012u	///< @brief \~chinese 相机参数越界 \~english Camera parameter is out of range
#define ITKSTATUS_COMMAND_NOT_ALLOW          0x0013u	///< @brief \~chinese 当前状态下不允许执行该命令 \~english Command cannot be executed in the current state
#define ITKSTATUS_COMMAND_NOT_PRASE          0x0014u	///< @brief \~chinese 相机命令无法解析 \~english Camera command cannot be parsed
#define ITKSTATUS_COMMAND_PENDING            0x0015u	///< @brief \~chinese 执行命令被挂起 \~english Execution command is suspended
#define ITKSTATUS_ARG_OUT_OF_RANGE           0x0016u	///< @brief \~chinese 输入参数越界 \~english Input parameter is out of bounds
#define ITKSTATUS_NOT_IMPLEMENT              0x0017u	///< @brief \~chinese 未实现 \~english Not realized
#define ITKSTATUS_NO_MEMORY                  0x0018u	///< @brief \~chinese 内存不足 \~english Out of memory
#define ITKSTATUS_INCOMPATIBLE_ARG_TYPE      0x0019u	///< @brief \~chinese 参数不匹配 \~english Parameter mismatch
#define ITKSTATUS_STREAM_IN_PROCESS          0x001Au	///< @brief \~chinese 数据流正在传输 \~english Data stream has being transferred
#define ITKSTATUS_PRM_READ_ONLY              0x001Bu	///< @brief \~chinese 参数只读 \~english Parameters are read-only
#define ITKSTATUS_STREAM_IS_OPENED           0x001Cu	///< @brief \~chinese 数据流已经被打开 \~english Data stream has been opened
#define ITKSTATUS_SYSTEM_ERROR               0x001Du	///< @brief \~chinese Windows系统函数调用错误 \~english Windows system function call error
#define ITKSTATUS_INVALID_ADDRESS            0x001Eu	///< @brief \~chinese 无效的地址 \~english Invalid address
#define ITKSTATUS_BAD_ALIGNMENT              0x001Fu	///< @brief \~chinese 参数边界不适配 \~english Parameter boundaries do not match
#define ITKSTATUS_DEVICE_BUSY                0x0020u	///< @brief \~chinese 设备正忙，无法响应命令 \~english Device is busy and cannot respond to commands
#define ITKSTATUS_DEVICE_IS_REMOVED          0x0021u	///< @brief \~chinese 设备已经被移除 \~english Device has been removed
#define ITKSTATUS_DEVICE_NOT_FOUND           0x0022u	///< @brief \~chinese 未找到设备 \~english Device not found
#define ITKSATTUS_BOARD_IS_OPENED            0x0023u	///< @brief \~chinese 设备已经打开，不允许重复打开相同的设备 \~english Device has been enabled. Do not enable the same device repeatedly
#define ITKSTATUS_BOARD_NO_OPENED            0x0024u    ///< @brief \~chinese 设备未打开 \~english Device is not opened
#define ITKSTATUS_PRM_WRITE_ONLY             0x0025u	///< @brief \~chinese 参数只支持写操作 \~english Parameter supports only write operations
#define ITKSTATUS_BOARD_CONNECTION_FAIL      0x0026u	///< @brief \~chinese 采集卡和相机连接断开 \~english Frame grabber is disconnected from camera
#define ITKSTATUS_RUNTIME_ERROR              0x0027u	///< @brief \~chinese 程序运行时错误 \~english Program runtime error
#define ITKSTATUS_IO_ERROR                   0x0028u	///< @brief \~chinese IO错误 \~english IO error
#define ITKSTATUS_BUFFER_OVERFLOW            0x0029u	///< @brief \~chinese 缓冲区溢出 \~english Buffer overflow
#define ITKSTATUS_COMMUNICATION_ERROR        0x0030u	///< @brief \~chinese 通信异常 \~english Abnormal communication
#define ITKSTATUS_CXP_CONTROL_CRC_ERROR      0x0031u	///< @brief \~chinese CXP相机CRC错误 \~english CXP camera CRC error
#define ITKSTATUS_ACK_ID_NOT_COMPATIABLE     0x0032u	///< @brief \~chinese gvcp的ack回复中acknowledge字段不正确或ack_id与cmd_id不匹配 \~english Acknowledge field in gvcp's ack reply is error or ack_id does not match cmd_id
#define ITKSTATUS_DEV_INVALID_HEADER         0x0033u	///< @brief \~chinese 命令头部无效 \~english Command header is invalid
#define ITKSTATUS_DEV_DSI_ENDPOINT_HALTED    0x0034u	///< @brief \~chinese 设备流接口端点已停止 \~english Endpoint of device stream interface has stopped
#define ITKSTATUS_DEV_DEI_ENDPOINT_HALTED    0x0035u	///< @brief \~chinese 设备事件接口端点已停止  \~english Device event interface endpoint has stopped
#define ITKSTATUS_DEV_DATA_DISCARDED         0x0036u	///< @brief \~chinese 当前块发生数据丢弃 \~english Data loss occurred in the current chunk
#define ITKSTATUS_DEV_DATA_OVERRUN           0x0037u	///< @brief \~chinese 设备无法发送所有数据  \~english Device is unable to send all data
#define ITKSTATUS_STREAM_ABORTED             0x0038u	///< @brief \~chinese 数据流被中止 \~english Data stream has been aborted
#define ITKSTATUS_DRIVER_NOT_MATCH           0x0039u	///< @brief \~chinese 驱动版本不匹配 \~english Driver version mismatch
#define ITKSTATUS_DEVICE_WRONG_USB_PORT      0x0040u	///< @brief \~chinese USB接口非3.0版本 \~english USB interface not version 3.0
#define ITKSTATUS_DEVICE_IS_FAULTY           0x0041u	///< @brief \~chinese 设备故障 \~english Device error
#define ITKSTATUS_CXP_TOPOLOGY_ERROR         0x0042u	///< @brief \~chinese CXP拓扑连接错误 \~english CXP topology connection error
#define ITKSTATUS_STREAM_PENDING             0x0043u	///< @brief \~chinese 数据流还未开始 \~english Data stream has not been started
#define ITKSTATUS_STREAM_TIMEOUT             0x0044u	///< @brief \~chinese 数据流超时 \~english Data stream has been timed out

#define ITKSTATUS_STREAM_ABORT               0x0045u    ///< @brief \~chinese 数据流终止 \~english Data stream has been abort
#define ITKSTATUS_INVALID_BUFFER             0x0046u    ///< @brief \~chinese 无效的缓冲区句柄 \~english Invalid buffer handle
///@}

// level first so that it be visible instantly
#define ITKSTATUS_BUILD(level, id, module)           ((id) | (((level)&ITKSTATUS_LEVEL_BIT_MASK) << ITKSTATUS_LEVEL_BIT_OFFSET) | (((module)&ITKSTATUS_MODULE_BIT_MASK) << ITKSTATUS_MODULE_BIT_OFFSET))

/* Macros for adding single fields to status */
#define ITKSTATUS_ADD_ID(status, id)       ((status) | ((id)&ITKSTATUS_STATUS_BIT_MASK))
#define ITKSTATUS_ADD_LEVEL(status, level) ((status) | (((level)&ITKSTATUS_LEVEL_BIT_MASK) << ITKSTATUS_LEVEL_BIT_OFFSET))
#define ITKSTATUS_ADD_MODULE(status, module)         ((status) | (((module)&ITKSTATUS_MODULE_BIT_MASK) << ITKSTATUS_MODULE_BIT_OFFSET))

/* Macros for adding module to status */
#define ITKSTATUS_DEVICE(status)    ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_DEVICE)
#define ITKSTATUS_BUFFER(status)    ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_BUFFER)
#define ITKSTATUS_LOG(status)       ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_LOG)
#define ITKSTATUS_MANAGER(status)   ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_MANAGER)
#define ITKSTATUS_STREAM(status)    ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_STREAM)
#define ITKSTATUS_PARAM(status)     ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_PARAM)
#define ITKSTATUS_SERIAL(status)    ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_SERIAL)
#define ITKSTATUS_EVENTINFO(status) ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_EVENTINFO)
#define ITKSTATUS_FEATURE(status)   ITKSTATUS_ADD_MODULE(status, ITKSTATUS_MODULE_FEATURE)

#endif
// clang-format on
