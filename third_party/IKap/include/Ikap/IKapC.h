// clang-format off
/**
\~chinese
 * @file IKapC.h
 * @brief IKapC库头文件
 * @note 本文件介绍创建相机C/C++应用程序所需要的接口定义。
*/
 /**
\~english
 * @file IKapC.h
 * @brief IKapC library header
 * @note This document introduces the definitions of interfaces required to create C/C++ application for camera.
 */

#ifndef _IKAPC_H
#define _IKAPC_H

#include "IKapCDef.h"
#include "IKapCDeprecated.h"

#ifdef __cplusplus
// avoid namespace ambiguities between std::_Bool (from yvals.h) and ::_Bool (from IKapCTypes.h)
#    if defined(_MSC_VER)
#        define _Bool ::_Bool	///< @brief \~chinese 布尔类型 \~english Boolean type
#        define IKAPC_BOOL_DEFINED
#    endif

extern "C" {
#endif  // __cplusplus

/**
\~chinese 
 * @brief 初始化IKapC开发环境
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note 必须在调用其他IKapC函数之前调用该函数。
 * @see ItkManTerminate
*/
 /**
\~english 
 * @brief Initialize IKapC development environment
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note This function must be called before calling other IKapC functions.
 * @see ItkManTerminate
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManInitialize(void);

/**
\~chinese 
 * @brief 释放IKapC初始化过程中申请的资源
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note 必须在其他IKapC函数调用完成之后调用该函数。
 * @see ItkManInitialize
*/
 /**
\~english 
 * @brief Release resources requested during initialization process of IKapC
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note This function must be called after other IKapC function calls are completed.
 * @see ItkManInitialize
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManTerminate(void);

/**
\~chinese
 * @brief 获取当前库版本
 * @param[out] version 当前库版本
 * @return
 * - ITKSTATUS_OK
 * @note 
 * | 0xff000000 | 0x00ff0000 | 0x0000ff00 | 0x000000ff |
 * |------------|------------|------------|------------|
 * | 主要版本号  | 次要版本号   | 补丁版本号   | 特定版本号   | 
*/
 /**
\~english
 * @brief Get the current library version
 * @param[out] version Current library version
 * @return
 * - ITKSTATUS_OK
 * @note 
 * | 0xff000000   | 0x00ff0000   | 0x0000ff00   | 0x000000ff     |
 * |--------------|--------------|--------------|----------------|
 * | Major version| Minor version| Patch version|Specific version| 
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetVersion(uint32_t *version);

/**
\~chinese 
 * @brief 添加cti库路径，多个路径间以英文;分隔
 * @param[in] fullPath cti路径
 * @return
 * - ITKSTATUS_OK
*/
 /**
\~english 
 * @brief Add cti path, use ';' to split mulit cti path
 * @param[in] fullPath cti path
 * @return
 * - ITKSTATUS_OK
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManAddCTIPath(const char* fullPath);

/**
\~chinese 
 * @brief 获取IKapC日志路径
 * @param[out] buffer 日志路径
 * @param[in/out] bufferSize 输入/写入大小
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @see ItkManTerminate
*/
 /**
\~english 
 * @brief Get IKapC log path
 * @param[out] buffer log path
 * @param[in/out] bufferSize Input/Output size
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note This function must be called before calling other IKapC functions.
 * @see ItkManTerminate
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetLogPath(char* buffer,int* bufferSize);

/**
\~chinese 
 * @brief 获取当前连接到PC上相机设备的数量
 * @param[out] pCount 当前连接到PC上的相机的数量
 * @return
 * - ITKSTATUS_OK
*/
 /**
\~english 
 * @brief Get the current number of camera devices connected to PC
 * @param[out] pCount The current number of camera connected to PC
 * @return
 * - ITKSTATUS_OK
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetDeviceCount(uint32_t* pCount);

/**
\~chinese 
 * @brief 枚举设备
 * @param[in] nDevType 设备类型, 参考MANAGER_TYPE_LIST。当nDevType为-1，该接口将枚举所有类型设备。
 * @param[out] pDevList 设备信息列表,包含枚举的设备数量与设备信息。
 * @return
 * - ITKSTATUS_OK
 * @see ItkManGetDeviceCount MANAGER_TYPE_LIST
*/
 /**
\~english 
 * @brief Enumerate devices
 * @param[in] nDevType Device type, refer to MANAGER_TYPE_LIST. When nDevType equals to -1, the interface will enumerate all type devices.
 * @param[out] pDevList Device information list，including the number of enumerated devices and device information.
 * @return
 * - ITKSTATUS_OK
 * @see ItkManGetDeviceCount MANAGER_TYPE_LIST
  */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetDeviceCountEx(const int32_t nDevType, PITK_DEVICE_INFO_LIST pDevInfoList);

/**
\~chinese 
 * @brief 获取相机设备信息
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[out] pDI 相机描述信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * @note 在调用本函数前必须先调用 ItkManGetDeviceCount 。
 * @see ItkManGetDeviceCount
*/
 /**
\~english 
 * @brief Get camera device information
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[out] pDI Camera description information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * @note Before calling this function, ItkManGetDeviceCount must be called first.
 * @see ItkManGetDeviceCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetDeviceInfo(uint32_t index, PITKDEV_INFO pDI);

/**
\~chinese 
 * @brief 获取网口相机专有设备信息
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[out] pDI 网口相机专有设备信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 在调用本函数前必须先调用 ItkManGetDeviceCount 。
 * @see ItkManGetDeviceCount
*/
 /**
\~english 
 * @brief Get network camera specific device information
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[out] pDI Network camera specific device information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note Before calling this function, ItkManGetDeviceCount must be called first.
 * @see ItkManGetDeviceCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetGigEDeviceInfo(uint32_t index, PITKGIGEDEV_INFO pDI);

/**
\~chinese 
 * @brief 获取Camera Link相机专有信息
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[out] pDI Camera Link相机专有设备信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 在调用本函数前必须先调用 ItkManGetDeviceCount 。
 * @see ItkManGetDeviceCount
*/
 /**
\~english 
 * @brief Get Camera Link camera specific information
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[out] pDI Camera Link camera specific device information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note Before calling this function, ItkManGetDeviceCount must be called first.
 * @see ItkManGetDeviceCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetCLDeviceInfo(uint32_t index, PITK_CL_DEV_INFO pDI);

/**
\~chinese 
 * @brief 获取CoaXPress相机专有信息
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[out] pDI CoaXPress相机专有设备信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 在调用本函数前必须先调用 ItkManGetDeviceCount 。
 * @see ItkManGetDeviceCount
*/
 /**
\~english 
 * @brief Get CoaXPress camera specific information
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[out] pDI CoaXPress camera specific device information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note Before calling this function, ItkManGetDeviceCount must be called first.
 * @see ItkManGetDeviceCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetCXPDeviceInfo(uint32_t index, PITK_CXP_DEV_INFO pDI);

/**
\~chinese 
 * @brief 获取USB3.0相机专有信息
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[out] pDI USB3.0相机专有设备信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 在调用本函数前必须先调用 ItkManGetDeviceCount 。
 * @see ItkManGetDeviceCount
*/
 /**
\~english 
 * @brief Get USB3.0 camera specific information
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[out] pDI USB3.0 camera specific device information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note Before calling this function, ItkManGetDeviceCount must be called first.
 * @see ItkManGetDeviceCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetU3VDeviceInfo(uint32_t index, PITK_U3V_DEV_INFO pDI);

/**
\~chinese 
 * @brief 获取GigEVision Board设备专有信息
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[out] pDI GigEVision Board相机专有设备信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 在调用本函数前必须先调用 ItkManGetDeviceCount 。
 * @see ItkManGetDeviceCount
*/
 /**
\~english 
 * @brief Get GigEVision Board device specific information
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[out] pDI GigEVision Board camera specific device information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_DEVICE_ID_OUTOF_RANGE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note Before calling this function, ItkManGetDeviceCount must be called first.
 * @see ItkManGetDeviceCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetGVBDeviceInfo(uint32_t index, PITK_GVB_DEV_INFO pDI);

/**
\~chinese 
 * @brief 获取状态码信息
 * @param[in] status 状态码
 * @param[out] idBuf 错误类别信息
 * @param[in,out] idBufSize 作为输入指明错误类别信息最大长度，作为输出指明错误类别信息有效长度
 * @param[out] levelBuf 错误级别信息
 * @param[in,out] levelBufSize 作为输入指明错误级别信息最大长度，作为输出指明错误级别信息有效长度
 * @param[out] moduleBuf 错误模块信息
 * @param[in,out] moduleBufSize 作为输入指明错误模块信息最大长度，作为输出指明错误模块信息有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @see ITKSTATUS
*/
 /**
\~english 
  * @brief Get status code information
 * @param[in] status Status code
 * @param[out] idBuf Error ID information
 * @param[in,out] idBufSize As input, it indicates the maximum length of error ID information. As output, it indicates the effective length of error ID information
 * @param[out] levelBuf Error level information
 * @param[in,out] levelBufSize As input, it indicates the maximum length of error level information information. As output, it indicates the effective length of error level information information
 * @param[out] moduleBuf Error module information
 * @param[in,out] moduleBufSize As input, it indicates the maximum length of error module information information. As output, it indicates the effective length of error module information information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @see ITKSTATUS
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetStatusText(
    ITKSTATUS status, char* idBuf, uint32_t* idBufSize, char* levelBuf, uint32_t* levelBufSize, char* moduleBuf,
    uint32_t* moduleBufSize);

/**
\~chinese 
 * @brief 获取全局参数
 * @param[in] prm 全局参数索引
 * @param[out] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkManSetPrm ITKMANAGER_PRM (n, size)
*/
 /**
\~english 
 * @brief Get manager parameter
 * @param[in] prm Manager parameter index
 * @param[out] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkManSetPrm ITKMANAGER_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManGetPrm(uint32_t prm, void* pValue);

/**
\~chinese 
 * @brief 设置全局参数
 * @param[in] prm 全局参数索引
 * @param[in] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkManGetPrm ITKMANAGER_PRM (n, size)
*/
 /**
\~english 
 * @brief Set manager parameter
 * @param[in] prm Manager parameter index
 * @param[in] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkManGetPrm ITKMANAGER_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkManSetPrm(uint32_t prm, const void* value);

/**
\~chinese 
 * @brief 打开相机设备
 * @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 * @param[in] accessMode 相机访问模式
 * @param[out] phDev 相机设备句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @note 
 * 1. 在对设备进行其他操作之前，必须首先打开设备。
 * 2. 打开相机设备前，必须先调用 ItkManGetDeviceCount 枚举当前电脑上可用相机的数量。
 * 3. 相机访问模式可选项如下所示：
 * | 访问模式                         |
 * |----------------------------------|
 * | ITKDEV_VAL_ACCESS_MODE_MONITOR   |
 * | ITKDEV_VAL_ACCESS_MODE_CONTROL   |
 * | ITKDEV_VAL_ACCESS_MODE_STREAM    |
 * | ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE |
 * @see ItkDevOpenWithXmlDescription ItkDevClose
*/
 /**
\~english 
 * @brief Open camera device
 * @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 * @param[in] accessMode Camera access mode
 * @param[out] phDev Camera device handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @note
 * 1. Before performing any other operations on the device, it must be turned on first.
 * 2. Before opening camera device, users must first call ItkManGetDeviceCount to enumerate the number of available camera on the current computer.
 * 3. Camera access mode options are as follows:
 * | Access mode                      |
 * |----------------------------------|
 * | ITKDEV_VAL_ACCESS_MODE_MONITOR   |
 * | ITKDEV_VAL_ACCESS_MODE_CONTROL   |
 * | ITKDEV_VAL_ACCESS_MODE_STREAM    |
 * | ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE |
 * @see ItkDevOpenWithXmlDescription ItkDevClose
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevOpen(uint32_t index, int accessMode, ITKDEVICE* phDev);

/**
\~chinese 
 *  @brief 通过指定xml打开相机设备
 *  @param[in] index 相机索引，有效范围[0, ItkManGetDeviceCount -1]
 *  @param[in] accessMode 相机访问模式，用户可以根据需要使用任意值或者它们的组合（通过“or”标志连接）
 *  @param[out] phDev 相机设备句柄
 *  @return
 *  - ITKSTATUS_OK
 *  - ITKSTATUS_ARG_OUT_OF_RANGE
 *  @note 
 * 1. 在对设备进行其他操作之前，必须首先打开设备。
 * 2. 打开相机设备前，必须先调用 ItkManGetDeviceCount 枚举当前电脑上可用相机的数量。
 * 3. 相机xml文件地址格式为“ local:[///]filename.extension;address;length[?SchemaVersion=x.x.x] ”的URL表示XML描述文件位于模块的虚拟寄存器映射中。
 一个完整的本地URL应该是从设备读取的“ local:itk_dev.zip;F0F00000;3BF?SchemaVersion=1.0.0 ”或
 从文件读取的“ file:///C:program%20files/genicam/xml/genapi/tlguru/tlguru_system_rev1.xml?SchemaVersion=1.0.0 ”。
 * 4. 相机访问模式可选项如下所示：
 * | 访问模式                         |
 * |----------------------------------|
 * | ITKDEV_VAL_ACCESS_MODE_MONITOR   |
 * | ITKDEV_VAL_ACCESS_MODE_CONTROL   |
 * | ITKDEV_VAL_ACCESS_MODE_STREAM    |
 * | ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE |
 * @see ItkDevOpen ItkDevClose
*/
 /**
\~english 
 *  @brief Open camera device by specifying xml
 *  @param[in] index Camera index, valid range is [0, ItkManGetDeviceCount -1]
 *  @param[in] accessMode Camera access mode, users can use any value or combination of them as needed (connected by the "or" flag)
 *  @param[out] phDev Camera device handle
 *  @return
 *  - ITKSTATUS_OK
 *  - ITKSTATUS_ARG_OUT_OF_RANGE
 *  @note 
 * 1. Before performing any other operations on the device, it must be turned on first.
 * 2. Before opening camera device, users must first call ItkManGetDeviceCount to enumerate the number of available camera on the current computer.
 * 3. The camera xml file address format is “ local:[///]filename.extension;address;length[?SchemaVersion=x.x.x] ”. 
 The URL represents that the xml description file is located in the virtual register mapping of the module. 
 A complete local URL should be read from the device as “ local:itk_dev.zip;F0F00000;3BF?SchemaVersion=1.0.0 ” or 
 read from file as “ file:///C:program%20files/genicam/xml/genapi/tlguru/tlguru_system_rev1.xml?SchemaVersion=1.0.0 ”.
 * 4. Camera access mode options are as follows:
 * | Access mode                      |
 * |----------------------------------|
 * | ITKDEV_VAL_ACCESS_MODE_MONITOR   |
 * | ITKDEV_VAL_ACCESS_MODE_CONTROL   |
 * | ITKDEV_VAL_ACCESS_MODE_STREAM    |
 * | ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE |
 * @see ItkDevOpen ItkDevClose
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkDevOpenWithXmlDescription(uint32_t index, int accessMode, ITKDEVICE* phDev, const char* xmlUrl);

/**
\~chinese 
 * @brief 检测相机是否连接
 * @param[in] hDev 相机设备句柄
 * @param[out] isConnected 连接状态
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkDevOpen
*/
 /**
\~english 
 * @brief Check if the camera is already connected 
 * @param[in] hDev Camera device handle
 * @param[out] isConnected connection status
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkDevOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevIsConnected(ITKDEVICE hDev, bool* isConnected);

/**
\~chinese 
 * @brief 关闭相机设备
 * @param[in] hDev 相机设备句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当设备关闭后除非再次打开，否则无法访问。
 * @see ItkDevOpen
*/
 /**
\~english 
 * @brief Close camera device
 * @param[in] hDev Camera device handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note After the device is closed, it cannot be accessed unless it is opened again.
 * @see ItkDevOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevClose(ITKDEVICE hDev);

/**
\~chinese 
 * @brief 读取相机设备xml
 * @param[in] hDev 相机设备句柄
 * @param[out] xml 读取的相机xml文件
 * @param[in,out] pXmlSize 作为输入指明相机xml文件最大长度，作为输出指明相机xml文件有效长度
 * @param[in] option 预留选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkManGetDeviceInfo ItkDevOpen
*/
 /**
\~english 
 * @brief Read camera device xml
 * @param[in] hDev Camera device handle
 * @param[out] xml Camera xml file
 * @param[in,out] pXmlSize As input, it indicates the maximum length of camera XML file. As output, it indicates the effective length of camera XML file
 * @param[in] option Reserved option
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkManGetDeviceInfo ItkDevOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevReadXmlDescription(ITKDEVICE hDev, char* xml, uint32_t* pXmlSize, uint32_t option);

/**
\~chinese 
 * @brief 加载相机设备配置文件
 * @param[in] hDev 相机设备句柄
 * @param[in] lpFileName 相机设备文件名称，以.ccf为后缀
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 本函数必须要指明配置文件的全部路径。
 * @see ItkDevSaveConfigurationToFile
*/
 /**
\~english 
 * @brief Load camera device configuration file
 * @param[in] hDev Camera device handle
 * @param[in] lpFileName Camera device file name with .ccf suffix
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note This function must indicate the full path of the configuration file.
 * @see ItkDevSaveConfigurationToFile
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevLoadConfigurationFromFile(ITKDEVICE hDev, char* lpFileName);

/**
\~chinese 
 * @brief 保存相机设备配置文件
 * @param[in] hDev 相机设备句柄
 * @param[in] lpFileName 相机设备文件名称，以.ccf为后缀
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 本函数必须要指明配置文件的全部路径。
 * @see ItkDevLoadConfigurationFromFile
*/
 /**
\~english 
 * @brief Save camera device configuration file
 * @param[in] hDev Camera device handle
 * @param[in] lpFileName Camera device file name with .ccf suffix
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note This function must indicate the full path of the configuration file.
 * @see ItkDevLoadConfigurationFromFile
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevSaveConfigurationToFile(ITKDEVICE hDev, char* lpFileName);

/**
\~chinese 
 * @brief 相机固件升级,函数成功返回后需要等待相机状态灯变为绿色时，断上电以使升级生效
 * @param[in] hDev 相机设备句柄
 * @param[in] strFileName 本地固件文件
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 本函数必须要指明文件的全部路径。
 * @see ItkDevLoadConfigurationFromFile
*/
 /**
\~english 
 * @brief Upgrade device's firmware. When the function returns successfully, the camera should be repowered when status light turns green
 * @param[in] hDev Camera device handle
 * @param[in] strFileName local firmware file
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note This function must indicate the full path of the file.
 * @see ItkDevLoadConfigurationFromFile
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevUpgrade(ITKDEVICE hDev, const char* strFileName);

/**
\~chinese 
 * @brief 获取相机升级进度
 * @param[in] hDev 相机设备句柄
 * @param[out] pnProcess 相机升级进度：0-100
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetFeatureInfo
*/
 /**
\~english 
 * @brief Get the process of device upgrade
 * @param[in] hDev Camera device handle
 * @param[out] pnProcess the process of device upgrade: 0-100
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetFeatureInfo
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetUpgradeProcess(ITKDEVICE hDev, unsigned int* pnProcess);

/**
\~chinese 
 * @brief 获取相机支持配置特征的数量
 * @param[in] hDev 相机设备句柄
 * @param[out] pCount 相机支持的特征数量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetFeatureInfo
*/
 /**
\~english 
 * @brief Get the number of camera supported configuration features
 * @param[in] hDev Camera device handle
 * @param[out] pCount The number of features supported by camera
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetFeatureInfo
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetFeatureCount(ITKDEVICE hDev, uint32_t* pCount);

/**
\~chinese 
 * @brief 获取相机特征名称
 * @param[in] hDev 相机设备句柄
 * @param[in] index 特征索引，有效范围[0, ItkDevGetFeatureCount -1]
 * @param[out] name 特征名称
 * @param[in,out] nameSize 作为输入指明特征名称最大长度，作为输出指明特征名称有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @see ItkDevGetFeatureCount
*/
 /**
\~english 
 * @brief Get camera feature name
 * @param[in] hDev Camera device handle
 * @param[in] index Feature index, valid range is [0, ItkDevGetFeatureCount -1]
 * @param[out] name Feature name
 * @param[in,out] nameSize As input, it indicates the maximum length of feature name. As output, it indicates the effective length of feature name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @see ItkDevGetFeatureCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetFeatureName(ITKDEVICE hDev, uint32_t index, char* name, uint32_t* pNameSize);

/**
\~chinese 
 * @brief 获取相机的寄存器地址数量
 * @param[in] hDev 相机设备句柄
 * @param[out] pCount 寄存器地址数量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetFeatureInfo
*/
 /**
\~english 
 * @brief Get the number of camera register addresses
 * @param[in] hDev Camera device handle
 * @param[out] pCount The number of camera register addresses
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetFeatureInfo
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetRegisterCount(ITKDEVICE hDev, uint32_t* pCount);

/**
\~chinese 
 * @brief 获取相机寄存器信息
 * @param[in] hDev 相机设备句柄
 * @param[in] index 寄存器索引，有效范围[0, ItkDevGetRegisterCount -1]
 * @param[out] pAddress 寄存器地址
 * @param[out] pLength 寄存器长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @see ItkDevGetFeatureCount
*/
 /**
\~english 
 * @brief Get camera register info
 * @param[in] hDev Camera device handle
 * @param[in] index Register index, valid range is [0, ItkDevGetRegisterCount -1]
 * @param[out] pAddress Register address
 * @param[out] pLength Register length
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @see ItkDevGetFeatureCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetRegisterInfo(ITKDEVICE hDev, uint32_t index, int64_t* pAddress, int64_t* pLength, uint32_t* pAccessMode);

/**
\~chinese 
 * @brief 获取相机数据流的数量
 * @param[in] hDev 相机设备句柄
 * @param[out] pCount 相机可用数据流的数量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 对于网口相机，至少有一个数据流；对于Camera Link相机，不存在数据流通道。
 * @see ItkDevAllocStreamEx
*/
 /**
\~english 
 * @brief Get the number of camera data streams
 * @param[in] hDev Camera feature handle
 * @param[out] pCount The number of available data streams for the camera
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note For Ethernet camera, there should be at least one data stream. For Camera Link camera, there is no data stream channel.
 * @see ItkDevAllocStreamEx
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetStreamCount(ITKDEVICE hDev, uint32_t* pCount);

/**
\~chinese 
 *  @brief  申请数据流采集过程中需要的资源, 同时会为该数据流分配合适属性的缓冲区
 *  @param[in]  hDev  相机的句柄
 *  @param[in]  index  数据流索引，有效范围[0, ItkDevGetStreamCount -1]
 *  @param[in]  bufferNum  当前流分配的缓冲区数量
 *  @param[out] phStream  数据流句柄，控制数据流整个生命周期
 *  @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 *  @note   
 * 1. 对于同一个相机，可能存在多个数据流同时采集数据，通过索引来指明希望采集的数据流。
 * 2. 收集数据流资源的过程中必须指定至少一个数据缓冲区，用来存放采集过程中的图像数据。
 * @see ItkDevFreeStream
*/
 /**
\~english 
 *  @brief  Creates an stream object with the specified number of buffers. The width, height, format of buffers are same as stream's
 *  @param[in]  hDev  Device handle
 *  @param[in]  index Data stream index, valid range is [0, ItkDevGetStreamCount -1]
 *  @param[in]  bufferNum  The Number of buffers created for the phStream
 *  @param[out] phStream  Stream handle, controlling the entire life cycle of data stream
 *  @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 *  @note   
 * 1. For the same camera, there may be multiple data streams grab data simultaneously. Use an index to indicate the desired data stream to grab.
 * 2. During the process of grabbing data stream resources, a data buffer must be specified to store the image data grabbed during acquisition process.
 * @see ItkDevFreeStream
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevAllocStreamEx(ITKDEVICE hDev, uint32_t index, uint32_t bufferNum, ITKSTREAM* phStream);

/**
\~chinese 
 * @brief 释放数据流采集过程中申请的资源
 * @param[in] hStream 数据流句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 如果存在正在进行的数据流采集过程，本函数将停止数据采集并释放资源。
 * 2. 本函数不会释放数据传输过程中的缓冲区资源，用户需要调用 ItkBufferFree 释放缓冲区。
 * @see ItkDevAllocStreamEx
*/
 /**
\~english 
 * @brief Release the resources requested during data stream acquisition process
 * @param[in] hStream Data stream handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. If there is an ongoing data stream acquisition process, this function will stop data acquisition and release resources.
 * 2. This function will not release buffer resources during data transmission. Users need to call ItkBufferFree to release buffer.
 * @see ItkDevAllocStreamEx
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevFreeStream(ITKSTREAM hStream);

/**
\~chinese 
 * @brief 获取相机支持触发事件的数量
 * @param[in] hDev 相机设备句柄
 * @param[out] pCount 相机支持触发事件的数量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetEventName
*/
 /**
\~english 
 * @brief Get the number of triggering events supported by the camera
 * @param[in] hDev Camera device handle
 * @param[out] pCount The number of triggering events supported by the camera
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetEventName
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetEventCount(ITKDEVICE hDev, uint32_t* pCount);

/**
\~chinese 
 * @brief 获取相机支持触发事件的名称
 * @param[in] hDev 相机设备句柄
 * @param[in] index 事件索引，有效范围[0, ItkDevGetEventCount -1]
 * @param[out] pEventName 相机触发事件的名称
 * @param[in,out] pNameSize 作为输入指明触发事件名称的最大长度，作为输出指明触发事件名称的有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @note 用户可以为其感兴趣的触发事件注册回调函数进行进一步的处理。
 * @see ItkDevGetEventCount
*/
 /**
\~english 
 * @brief Get the name of triggering events supported by the camera
 * @param[in] hDev Camera device handle
 * @param[in] index Event index, valid range is [0, ItkDevGetEventCount -1]
 * @param[out] pEventName The name of triggering events supported by the camera
 * @param[in,out] pNameSize As input, it indicates the maximum length of event name. As output, it indicates the effective length of event name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * @note Users can register callback functions for their interested triggering events for further processing.
 * @see ItkDevGetEventCount
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetEventName(ITKDEVICE hDev, uint32_t index, char* pEventName, uint32_t* pNameSize);

/**
\~chinese 
 * @brief 判断相机是否支持该触发事件
 * @param[in] hDev 相机设备句柄
 * @param[int] pEventName 相机触发事件的名称
 * @param[out] bAvailable true表示相机支持该触发事件，false表示相机不支持该触发事件
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetEventCount ItkDevGetEventName
*/
 /**
\~english 
 * @brief Determine whether the camera supports the triggering event
 * @param[in] hDev Camera device handle
 * @param[int] pEventName The name of triggering events supported by the camera
 * @param[out] bAvailable true indicates that the camera supports the triggering event, while false indicates that the camera does not support the triggering event
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetEventCount ItkDevGetEventName
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevIsEventAvailable(ITKDEVICE hDev, const char* pEventName, _Bool* bAvailable);

/**
\~chinese 
 * @brief 为相机触发事件注册回调函数
 * @param[in] hDev 相机设备句柄
 * @param[in] pEventName 相机触发事件的名称
 * @param[in] callback 用户回调函数地址
 * @param[in] pContext 用户回调函数参数（结构体、缓冲区等），可以为NULL
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note pEventName输入项:{"DeviceRemove","FeatureValueChanged","MessageChannel"}。多次为相同事件注册回调函数，仅会调用最近一次注册的回调函数。
 * @see ItkDevUnregisterCallback
*/
 /**
\~english 
 * @brief Register callback functions for camera triggering event
 * @param[in] hDev Camera device handle
 * @param[in] pEventName The name of triggering events supported by the camera.
 * @param[in] callback User callback function address
 * @param[in] pContext User callback function parameters (structure, buffer, etc.), which can be NULL
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note pEventName options:{"DeviceRemove","FeatureValueChanged","MessageChannel"}. Multiple registrations of callback functions for the same event will only call the most recently registered callback function.
 * @see ItkDevUnregisterCallback
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevRegisterCallback(ITKDEVICE hDev, const char* pEventName, PITKEVENTINFOCALLBACK callback, void* pContext);

/**
\~chinese 
 * @brief 清除注册的回调函数
 * @param[in] hDev 相机设备句柄
 * @param[in] pEventName 相机触发事件的名称
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevRegisterCallback
*/
 /**
\~english 
 * @brief Clear the registered callback function
 * @param[in] hDev Camera device handle
 * @param[in] pEventName The name of triggering events supported by the camera
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevRegisterCallback
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevUnregisterCallback(ITKDEVICE hDev, const char* pEventName);

/**
\~chinese 
 * @brief 获取相机参数
 * @param[in] hDev 相机设备句柄
 * @param[in] prm 设备参数索引
 * @param[out] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevSetPrm ITKDEV_PRM (n, size)
*/
 /**
\~english 
 * @brief Get camera parameter
 * @param[in] hDev Camera device handle
 * @param[in] prm Device parameter index
 * @param[out] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevSetPrm ITKDEV_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetPrm(ITKDEVICE hDev, uint32_t prm, void* pValue);

/**
\~chinese 
 * @brief 获取相机像素格式对应参数值
 * @param[in] hDev 相机设备句柄
 * @param[in] pixelFormatStr 像素格式字符串
 * @param[out] pixelFromatValue 像素格式参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferNew
*/
 /**
\~english 
 * @brief Get the parameter values corresponding to the camera pixel format
 * @param[in] hDev Camera device handle
 * @param[in] pixelFormatStr Pixel format string
 * @param[out] pixelFromatValue Pixel format parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferNew
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkDevGetPixelFormatVal(ITKDEVICE hDev, const char* pixelFormatStr, uint32_t* pixelFromatValuePtr);

/**
\~chinese 
 * @brief 设置相机参数
 * @param[in] hDev 相机设备句柄
 * @param[in] prm 设备参数索引
 * @param[in] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetPrm ITKDEV_PRM (n, size)
*/
 /**
\~english 
 * @brief Set camera parameter
 * @param[in] hDev Camera device handle
 * @param[in] prm Device parameter index
 * @param[in] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkDevGetPrm ITKDEV_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevSetPrm(ITKDEVICE hDev, uint32_t prm, const void* value);

/**
\~chinese 
 * @brief 从Device端口读取原始数据
 * @param[in] hDev 相机设备句柄
 * @param[out] pBuffer 存放数据的缓冲区
 * @param[in] address 寄存器地址
 * @param[in] length 缓冲区长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 从Device端口读取的原始数据将被写入缓冲区，请确保缓冲区大小足够。
 * @see ItkDevPortWrite
*/
 /**
\~english 
 * @brief Read raw data from Device port
 * @param[in] hDev Camera device handle
 * @param[out] pBuffer Buffer for storing data
 * @param[in] address Register address
 * @param[in] length Buffer length
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The raw data read from Device port will be written to the buffer, please ensure that the buffer size is sufficient.
 * @see ItkDevPortWrite
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevPortRead(ITKDEVICE hDev, void* pBuffer, uint64_t address, uint32_t length);

/**
\~chinese 
 * @brief 向Device端口写入原始数据
 * @param[in] hDev 相机设备句柄
 * @param[in] pBuffer 存放数据的缓冲区
 * @param[in] address 寄存器地址
 * @param[in] length 缓冲区长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 从缓冲区读取的原始数据将被写入Device端口，请确保缓冲区大小适当。
 * @see ItkDevPortRead
*/
 /**
\~english 
 * @brief Write raw data to Device port
 * @param[in] hDev Camera device handle
 * @param[in] pBuffer Buffer for storing data
 * @param[in] address Register address
 * @param[in] length Buffer length
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The raw data read from the buffer will be written to Device port, please ensure that the buffer size is sufficient.
 * @see ItkDevPortRead
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevPortWrite(ITKDEVICE hDev, const void* pBuffer, uint64_t address, uint32_t length);

/**
\~chinese 
 * @brief 设置千兆网相机临时IP地址
 * @param[in] pMacAddress 设备MAC地址，例如00-30-53-06-1a-58
 * @param[in] pIpAddress 待设置IP地址，例如192.168.1.2
 * @param[in] pSubnetMask 待设置子网掩码，例如255.255.255.0
 * @param[in] pDefaultGateway 待设置默认网关，例如192.168.1.1
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * @note 调用此函数时，必须关闭相机进行配置。
 * @see ItkDevGetPrm
*/
 /**
\~english 
 * @brief Set force IP address for gigabit network camera
 * @param[in] pMacAddress Device MAC address, such as 00-30-53-06-1a-58
 * @param[in] pIpAddress IP address to be set, such as 192.168.1.2
 * @param[in] pSubnetMask Subnet mask to be set, such as 255.255.255.0
 * @param[in] pDefaultGateway Default gateway to be set, such as 192.168.1.1
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * @note When calling this function, the camera must be turned off for configuration.
 * @see ItkDevGetPrm
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkGigEDevForceIp(const char* pMacAddress, const char* pIpAddress, const char* pSubnetMask, const char* pDefaultGateway);


/**
\~chinese 
 * @brief 发出动作命令
 * @param[in]  pActionCmdInfo 动作命令信息
 * @param[out] pActionCmdResults 动作命令返回信息列表
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_NO_MEMORY
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * - ITKSTATUS_SYSTEM_ERROR
 * @note 仅GigEVision设备支持。
 * @note 调用此API之前，必须先调用 ItkManGetDeviceCount 枚举当前电脑上可用相机的数量。
 * pActionCmdResults仅对当次调用生效。若多次调用该函数，则之前返回的pActionCmdResults失效。
*/
 /**
\~english 
 * @brief Set force IP address for gigabit network camera
 * @param[in]  pActionCmdInfo Action Command
 * @param[out] pActionCmdResults Action Command Result List
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_NO_MEMORY
 * - ITKSTATUS_BUFFER_TOO_SMALL
 * - ITKSTATUS_SYSTEM_ERROR
 * @note This API is supported only by GigEVision camera.
 * @note Before calling this API, ItkManGetDeviceCount must be called first.
 * pActionCmdResults is only valid for current call. When this api is called multiple times, pActionCmdResults returned before become invalid.
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkGigEDevIssueActionCommand(PITK_ACTION_CMD_INFO pActionCmdInfo, PITK_ACTION_CMD_RESULT_LIST pActionCmdResults);

/**
\~chinese
 * @brief 向GigEDevice端口写入原始UDP payload数据
 * @param[in] hDev：相机设备句柄
 * @param[in] pBuffer：存放数据的缓冲区
 * @param[in] length：缓冲区长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 从缓冲区读取的原始数据将被写入Device 的GVCP端口，请确保缓冲区大小适当，确保整个IP包大小不超过MTU(一般为1500)。
 * @see ItkDevPortRead
 */
/**
\~english
 * @brief Write the raw UDP payload data to the GigEDevice port
 * @param[in] hDev：Camera device handle
 * @param[in] pBuffer：Buffer for storing data
 * @param[in] length：Buffer length
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The raw data read from the buffer will be written to the Device GVCP port, 
 * make sure that the buffer is appropriate in size and that the entire IP package size does not exceed the MTU (generally 1500).
 * @see ItkDevPortRead
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkGigEDevPortWriteRaw(ITKDEVICE hDev,const uint8_t* buffer, uint32_t length);

/**
\~chinese 
 * @brief 获取千兆网相机静态IP地址
 * @param[in] hDev 相机设备句柄
 * @param[in] pIpAddress 设备静态IP地址，例如192.168.1.2
 * @param[in,out] pIpAddressLen 静态IP地址长度
 * @param[in] pSubnetMask 设备静态子网掩码，例如255.255.255.0
 * @param[in,out] pSubnetMaskLen 静态子网掩码长度
 * @param[in] pDefaultGateway 设备默认网关，例如192.168.1.1
 * @param[in,out] pDefaultGatewayLen 默认网关长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkGigEDevSetPersistentIpAddress
*/
 /**
\~english 
 * @brief Get persistent IP address of gigabit network camera
 * @param[in] hDev Camera device handle
 * @param[in] pIpAddress Device persistent IP address, such as 192.168.1.2
 * @param[in,out] pIpAddressLen Persistent IP address length
 * @param[in] pSubnetMask Device persistent subnet mask, such as 255.255.255.0
 * @param[in,out] pSubnetMaskLen Persistent subnet mask length
 * @param[in] pDefaultGateway Device default gateway, such as 192.168.1.1
 * @param[in,out] pDefaultGatewayLen Default gateway length
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkGigEDevSetPersistentIpAddress
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkGigEDevGetPersistentIpAddress(
    ITKDEVICE hDev, char* pIpAddress, uint32_t* pIpAddressLen, char* pSubnetMask, uint32_t* pSubnetMaskLen,
    char* pDefaultGateway, uint32_t* pDefaultGatewayLen);

/**
\~chinese 
 * @brief 设置千兆网相机静态IP地址
 * @param[in] hDev 相机设备句柄
 * @param[in] pIpAddress 设备静态IP地址，例如192.168.1.2
 * @param[in] pSubnetMask 设备静态子网掩码，例如255.255.255.0
 * @param[in] pDefaultGateway 设备默认网关，例如192.168.1.1
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkGigEDevGetPersistentIpAddress
*/
 /**
\~english 
 * @brief Set persistent IP address of gigabit network camera
 * @param[in] hDev Camera device handle
 * @param[in] pIpAddress Device persistent IP address, such as 192.168.1.2
 * @param[in] pSubnetMask Device persistent subnet mask, such as 255.255.255.0
 * @param[in] pDefaultGateway Device default gateway, such as 192.168.1.1
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkGigEDevGetPersistentIpAddress
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkGigEDevSetPersistentIpAddress(
    ITKDEVICE hDev, const char* pIpAddress, const char* pSubnetMask, const char* pDefaultGateway);

/**
\~chinese 
 * @brief 获取事件参数信息结构体
 * @param[in] hEventInfo 事件信息句柄
 * @param[out] pInfo 事件信息结构体
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * @see ItkDevGetPrm
*/
 /**
\~english 
 * @brief Get event parameter information struct
 * @param[in] hEventInfo Event information handle
 * @param[out] value Event information struct output buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * @see ItkDevGetPrm
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkEventGetInfo(ITKEVENTINFO hEventInfo, ITK_DEVEVENT_INFO* pInfo);

/**
\~chinese 
 * @brief 获取相机特征访问模式
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pAccessMode 相机特征访问模式
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 特征访问模式可选项如下所示：
 * | 访问模式                             |
 * |--------------------------------------|
 * | ITKFEATURE_VAL_ACCESS_MODE_UNDEFINED |
 * | ITKFEATURE_VAL_ACCESS_MODE_RW        |
 * | ITKFEATURE_VAL_ACCESS_MODE_RO        |
 * | ITKFEATURE_VAL_ACCESS_MODE_WO        |
 * | ITKFEATURE_VAL_ACCESS_MODE_NI        |
 * | ITKFEATURE_VAL_ACCESS_MODE_NA        |
*/
 /**
\~english 
 * @brief Get camera feature access mode
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pAccessMode Camera feature access mode
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The available options for feature access mode are as follows:
 * | Access mode                          |
 * |--------------------------------------|
 * | ITKFEATURE_VAL_ACCESS_MODE_UNDEFINED |
 * | ITKFEATURE_VAL_ACCESS_MODE_RW        |
 * | ITKFEATURE_VAL_ACCESS_MODE_RO        |
 * | ITKFEATURE_VAL_ACCESS_MODE_WO        |
 * | ITKFEATURE_VAL_ACCESS_MODE_NI        |
 * | ITKFEATURE_VAL_ACCESS_MODE_NA        |
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetAccessMode(ITKDEVICE hDev, const char* featureName, uint32_t* pAccessMode);

/**
\~chinese 
 * @brief 获取相机特征基本信息
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pInfo 相机特征基本信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get camera feature basic information
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pInfo Camera feature basic information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetFeatureInfo(ITKDEVICE hDev, const char* featureName, ITK_FEATURE_BASE_INFO* pInfo);

/**
\~chinese 
 * @brief 无效化设备所有特征的缓存
 * @param[in] hDev 相机设备句柄
 * @note 使用此函数可以刷新设备所有特征值. 调用时会触发所有特征的ITKDEV_VAL_EVENT_TYPE_FEATURE_VALUE_CHANGED事件.
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Invalidate all features cache in the device.
 * @param[in] hDev Camera device handle
 * @note Use this function to refresh all features' value.  
 *       Trigger ITKDEV_VAL_EVENT_TYPE_FEATURE_VALUE_CHANGED event when executing.
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevInvalidateFeatures(ITKDEVICE hDev);

/**
\~chinese 
 * @brief 获取64位整数类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 64位整数类型相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of 64bit integer type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Value of 64bit integer type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt64(ITKDEVICE hDev, const char* featureName, int64_t* pValue);

/**
\~chinese 
 * @brief 设置64位整数类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[in] value 64位整数类型相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @note 设定值的取值范围一定是[ ItkDevGetInt64Min , ItkDevGetInt64Max ]，
 取值同时也应满足value = ItkDevGetInt64Min + N* ItkDevGetInt64Inc ，N=0,1,2,…
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Set the value of 64bit integer type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[in] value Value of 64bit integer type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @note The range of values for the set value must be [ ItkDevGetInt64Min , ItkDevGetInt64Max ], 
 value should also satisfy the condition of value = ItkDevGetInt64Min + N* ItkDevGetInt64Inc , N=0,1,2,…
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevSetInt64(ITKDEVICE hDev, const char* featureName, int64_t value);

/**
\~chinese 
 * @brief 获取64位整数类型特征基本信息
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pInfo 相机特征基本信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get 64bit integer type feature basic information
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pInfo Camera feature basic information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt64FeatureInfo(ITKDEVICE hDev, const char* featureName, ITK_FEATURE_INT64_INFO* pInfo);

/**
\~chinese 
 * @brief 获取浮点数类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 64位浮点数类型的相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get value of double type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Value of 64bit double type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDouble(ITKDEVICE hDev, const char* featureName, double* pValue);

/**
\~chinese 
 * @brief 设置浮点数类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[in] value 64位浮点数类型的相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @note 设定值的取值范围一定是[ ItkDevGetDoubleMin , ItkDevGetDoubleMax ]。
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Set value of double type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[in] value Value of 64bit double type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @note The range of values for the set value must be [ ItkDevGetDoubleMin , ItkDevGetDoubleMax ].
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevSetDouble(ITKDEVICE hDev, const char* featureName, double value);

/**
\~chinese 
 * @brief 获取浮点数类型特征基本信息
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pInfo 相机特征基本信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get double type feature basic information
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pInfo Camera feature basic information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDoubleFeatureInfo(ITKDEVICE hDev, const char* featureName, ITK_FEATURE_DOUBLE_INFO* pInfo);
/**
\~chinese 
 * @brief 获取布尔数据类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 布尔类型的相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of boolean type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Boolean type value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetBool(ITKDEVICE hDev, const char* featureName, _Bool* pValue);

/**
\~chinese 
 * @brief 设置布尔数据类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[in] pValue 布尔类型的相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Set the value of boolean type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[in] pValue Boolean type value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevSetBool(ITKDEVICE hDev, const char* featureName, _Bool value);

/**
\~chinese 
 * @brief 获取字符串或枚举类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pBuff 字符串或枚举类型的相机特征值
 * @param[in,out] pBuffLen 作为输入指明特征值的最大长度，作为输出指明特征值的有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @note 传递给此函数的缓冲区必须足够容纳整个字符串。如果调用该函数时，将pBuff设置为NULL，则pBufLen指向的变量将设置为所需的缓冲区大小。
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of string or enumeration type
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pBuff Value of string or enumeration type
 * @param[in,out] pBuffLen As input, it indicates the maximum length of feature value. As output, it indicates the effective length of feature value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @note The buffer passed to this function must be sufficient to hold the entire string. 
 If pBuff is set to NULL when calling this function, the variable pointed to by pBufLen will be set to the desired buffer size.
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevToString(ITKDEVICE hDev, const char* featureName, char* pBuff, uint32_t* pBuffLen);

/**
\~chinese 
 * @brief 设置字符串或枚举类型特征的值
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 字符串或枚举类型的相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Set the value of string or enumeration type
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Value of string or enumeration type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevFromString(ITKDEVICE hDev, const char* featureName, const char* pValue);

/**
\~chinese 
 * @brief 获取枚举类型特征枚举项基本信息
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[in] enumIndex 枚举项索引
 * @param[out] pInfo 枚举项基本信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get basic information of entry of enumeration type feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[in] enumIndex Entry index
 * @param[out] pInfo Entry basic information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetEnumEntryFeatureInfo(ITKDEVICE hDev, const char* featureName, uint64_t enumIndex, ITK_FEATURE_ENUM_ENTRY_INFO* pInfo);
/**
\~chinese 
 * @brief 根据相机特征执行相应命令
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Execute corresponding commands based on camera feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevExecuteCommand(ITKDEVICE hDev, const char* featureName);

/**
\~chinese 
 * @brief 判断命令类型特征是否执行完成
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] isDone 是否执行完成
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Determine whether the command type feature has been executed successfully
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] isDone Is the execution completed
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevCommandIsDone(ITKDEVICE hDev, const char* featureName, _Bool* isDone);

/**
\~chinese 
 * @brief 获取被选择特征的数量
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 被选择特征的数量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevIsSelector
*/
 /**
\~english 
 * @brief Get the number of selected features
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue The number of selected features
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevIsSelector
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetSelectedFeatureCounts(ITKDEVICE hDev, const char* featureName, uint32_t* pValue);

/**
\~chinese 
 * @brief 获取被选择特征的名称
 * @param[in] hDev 相机设备句柄
 * @param[in] featureName 相机特征名称
 * @param[in] index 特征索引，有效范围[0, ItkDevGetSelectedFeatureCounts -1]
 * @param[out] pBuf 被选择特征的名称
 * @param[out] pBufLen 作为输入指明特征名称的最大长度，作为输出指明特征名称的有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevIsSelector
*/
 /**
\~english 
 * @brief Get the name of the selected feature
 * @param[in] hDev Camera device handle
 * @param[in] featureName Camera feature name
 * @param[in] index Feature index, valid range is [0, ItkDevGetSelectedFeatureCounts -1]
 * @param[out] pBuf The name of the selected feature
 * @param[out] pBufLen As input, it indicates the maximum length of feature name. As output, it indicates the effective length of feature name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevIsSelector
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkDevGetSelectedFeatureName(ITKDEVICE hDev, const char* featureName, uint32_t index, char* pBuf, uint32_t* pBufLen);

/**
\~chinese 
 * @brief 获取U3V的传输包大小
 * @param[in] hDev 相机设备句柄
 * @param[in] nTransferSize 传输的包大小指针, Byte
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 接口仅U3V协议的USB相机支持。
*/
 /**
\~english 
 * @brief Get transfer size of U3V device
 * @param[in] hDev Camera device handle
 * @param[in] nTransferSize Transfer size，Byte
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note The interface is only supported by the USB camera of the U3V protocol.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetTransferSize(ITKDEVICE hDev, uint32_t* nTransferSize);

/**
\~chinese 
 * @brief 设置U3V的传输包大小
 * @param[in] hDev 相机设备句柄
 * @param[in] nTransferSize 传输的包大小, Byte，默认为4M，rang：>=0x400，建议最大值：[windows] rang <= 0x400000；[Linux] rang <= 0x200000
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note 接口仅U3V协议的USB相机支持。增加传输包大小可以适当降低取流时的CPU占用率。但不同的PC和不同USB扩展卡存在不同的兼容性，如果该参数设置过大可能会出现取不到图像的风险。
*/
 /**
\~english 
 * @brief Set transfer size of U3V device
 * @param[in] hDev Camera device handle
 * @param[in] nTransferSize Transfer size，Byte，default：4M，rang：>=0x400，Recommended maximum: [windows] rang <= 0x400000; [Linux] rang <= 0x200000
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * @note The interface is only supported by the USB camera of the U3V protocol. Increasing the transmission package size can appropriately reduce the CPU occupancy during flow extraction. However, different PC and different USB expansion cards have different compatibility, and if this parameter is set too large, there may be the risk of not taking the image.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDevSetTransferSize(ITKDEVICE hDev, uint32_t nTransferSize);

/**
\~chinese 
 * @brief 异步结束数据流采集
 * @param[in] hStream 数据流句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当本函数执行完成后，采集过程将会被终止，但是无法保证最后一帧图像的完整性。
 * @see ItkStreamStop
*/
 /**
\~english 
 * @brief Asynchronously stop data stream acquisition
 * @param[in] hStream Data stream handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note After the execution of this function is completed, the acquisition process will be stopped, but the integrity of the last frame image cannot be guaranteed.
 * @see ItkStreamStop
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamAbort(ITKSTREAM hStream);

/**
\~chinese 
 * @brief 设置CPU亲和性
 * @param[in] hStream 数据流句柄
 * @param[out] isSetAffinity 是否设置CPU亲和性
 * @param[out] affinity CPU核数
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 通常情况下，程序运行时，资源会根据PC策略自动在CPU上进行分配。该函数用于设置程序运行时资源分配所占用的CPU个数。
*/
 /**
\~english 
 * @brief Set CPU affinity
 * @param[in] hStream Data stream handle
 * @param[out] isSetAffinity Whether to set CPU affinity
 * @param[out] affinity Number of CPU cores
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note Normally, during program execution, resources are automatically allocated on CPU according to PC policies. 
 This function is used to set the number of CPUs occupied by resource allocation during program runtime.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamGetAffinity(ITKSTREAM hStream, _Bool* isSetAffinity, uint32_t* affinity);

/**
\~chinese 
 * @brief 向数据流添加缓冲区
 * @param[in] hStream 数据流句柄
 * @param[in] hBuffer 数据缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 每个数据流都会维护独立的缓冲区链表，采集得到的图像数据将会顺序填入缓冲区中。本函数将会添加一个缓冲区到缓冲区链表的尾部。
 * 2. 数据流不会多次添加同一个数据缓冲区。
 * @see ItkStreamRemoveBuffer
*/
 /**
\~english 
 * @brief Add buffer to data stream
 * @param[in] hStream Data stream handle
 * @param[in] hBuffer Data buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. Each data stream will maintain an independent buffer linked list, and the grabbed image data will be sequentially filled into the buffer. 
 This function will add buffer to the end of the buffer linked list.
 * 2. The data stream will not add the same data buffer multiple times.
 * @see ItkStreamRemoveBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamAddBuffer(ITKSTREAM hStream, ITKBUFFER hBuffer);

/**
\~chinese
 *  @brief  获取Stream中的缓冲区。
 *  @param[in]	hStream 数据流句柄
 *  @param[in]	index 需要获取句柄的序号
 *  @param[out] phBuffer 获取到的缓冲区句柄
 *  @return
 *  - ITKSTATUS_OK
 *  @note 从数据流缓冲区列表中获取指定索引位置的缓冲区句柄。
*/
 /**
\~english
 *  @brief  Get buffer handle from stream.
 *  @param[in]	hStream Stream handle
 *  @param[in]	index The index of buffer in buffer_list of stream
 *  @param[out] phBuffer Buffer handle to get
 *  @return
 *  - ITKSTATUS_OK
 *  @note Get the buffer handle from stream's buffer list with specific index.
 */

IKAPC_API ITKSTATUS IKAPC_CC ItkStreamGetBuffer(ITKSTREAM hStream, uint32_t index, ITKBUFFER* phBuffer);

/**
\~chinese 
 * @brief 向数据流添加批量缓冲区
 * @param[in] hStream 数据流句柄
 * @param[in] hBuffer 数据缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 该函数接口用于光谱共聚焦传感器。用户在添加常规缓冲区后，还需要添加批量缓冲区。批量缓冲区不能注册帧完成回调，
 需要注册批量获取回调。在批量获取回调中，用户可以根据当前批量缓冲区序号批量获取常规缓冲区的指针和常规缓冲区状态的指针，
 再根据图像的大小和常规缓冲区状态结构体的大小遍历所有批量获取到的图像。
 * @see ItkStreamRemoveBatchBuffer
*/
 /**
\~english 
 * @brief Add batch buffers to data stream
 * @param[in] hStream Data stream handle
 * @param[in] hBuffer Data buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note This function interface is used for spectral confocal sensor. After adding a regular buffer, users also need to add batch buffer. 
 Batch buffer cannot register frame completion callback, batch retrieval callback need to be registered. 
 In batch retrieval callback, users can batch retrieve pointers to regular buffers and regular buffer states based on the current batch buffer sequence number, 
 and then traverse all batch retrieved images based on the size of the image and the size of the regular buffer state structure.
 * @see ItkStreamRemoveBatchBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamAddBatchBuffer(ITKSTREAM hStream, ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 从数据流移除缓冲区
 * @param[in] hStream 数据流句柄
 * @param[in] hBuffer 数据缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 每个数据流都会维护独立的缓冲区链表，采集得到的图像数据将会顺序填入缓冲区中。本函数将会从数据流缓冲区链表中删除一个指定的缓冲区。
 * 2. 为了确保数据采集过程的正常运行，数据流的缓冲区链表中至少要一个可用缓冲区。
 * @see ItkStreamAddBuffer
*/
 /**
\~english 
 * @brief Remove buffer from data stream
 * @param[in] hStream Data stream handle
 * @param[in] hBuffer Data buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. Each data stream will maintain an independent buffer linked list, and the grabbed image data will be sequentially filled into the buffer. 
 This function will delete a specified buffer from the data stream buffer linked list.
 * 2. To ensure the normal operation of the data acquisition process, there must be at least one available buffer in the buffer linked list of the data stream.
 * @see ItkStreamAddBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamRemoveBuffer(ITKSTREAM hStream, ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 从数据流移除批量缓冲区
 * @param[in] hStream 数据流句柄
 * @param[in] hBuffer 数据缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 该函数接口用于光谱共聚焦传感器。
 * @see ItkStreamAddBatchBuffer
*/
 /**
\~english 
 * @brief Remove batch buffer from data stream
 * @param[in] hStream Data stream handle
 * @param[in] hBuffer Data buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note This function interface is used for spectral confocal sensor.
 * @see ItkStreamAddBatchBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamRemoveBatchBuffer(ITKSTREAM hStream, ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 从数据流保存批量缓冲区
 * @param[in] hStream 数据流句柄
 * @param[in] hBuffer 批量缓冲区句柄
 * @param[in] filename 保存的文件名
 * @param[in] options 保存配置选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 该函数接口用于光谱共聚焦传感器，保存配置选项描述如下 
 * | 位数    | 描述                                   |
 * |---------|----------------------------------------|
 * | [0:7]   | 图像保存为 ITKBUFFER_VAL_PCD 类型      |
 * | [8:23]  | 仅用于设置Jpeg图像质量系数，范围1~100  |
 * | [24]    | 仅用于pcd图像，0表示ASCII，1表示二进制 |
 * | [25:31] | 预留                                   |
 * @see ItkBufferSave
*/
 /**
\~english 
 * @brief Save batch buffer from data stream
 * @param[in] hStream Data stream handle
 * @param[in] hBuffer Batch buffer handle
 * @param[in] filename File name
 * @param[in] options Configuration options
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note This function interface is used for spectral confocal sensor. The description of saving configuration options is as follows:
 * | Bit     | Description                                                            |
 * |---------|------------------------------------------------------------------------|
 * | [0:7]   | Save the image as ITKBUFFER_VAL_PCD type                               |
 * | [8:23]  | Only used to set Jpeg image quality coefficient, ranging from 1 to 100 |
 * | [24]    | Only used for pcd images, 0 represents ASCII, 1 represents binary      |
 * | [25:31] | Reserved                                                               |
 * @see ItkBufferSave
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkStreamSaveBatchBuffer(ITKSTREAM hStream, ITKBUFFER hBuffer, const char* filename, uint32_t options);

/**
\~chinese 
 * @brief 清空数据流缓冲区
 * @param[in] hStream 数据流句柄
 * @param[in] hBuffer 数据缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 清除数据流缓冲区的状态，适用于流自动清空机制禁用的情况。
 * @see ItkStreamAddBuffer
*/
 /**
\~english 
 * @brief Clear data stream buffer
 * @param[in] hStream Data stream handle
 * @param[in] hBuffer Data buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note Clear the state of data stream buffer, suitable for situations where the automatic stream clearing mechanism is disabled.
 * @see ItkStreamAddBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamClearBuffer(ITKSTREAM hStream, ITKBUFFER hBuffer);

/**
\~chinese
 * @brief 获取当前缓冲区列表的buffer个数
 * @param[in] hStream 数据流句柄
 * @param[out] pnBufferNum 当前缓冲区有效buffer个数的指针
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamAddBuffer
*/
 /**
\~english
 * @brief Gets the buffer number of the current bufferList
 * @param[in] hStream Data stream handle
 * @param[out] pnBufferNum Pointer to the buffer number of the current bufferList
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamAddBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamGetBufferNum(ITKSTREAM hStream, uint32_t* pnBufferNum);

/**
\~chinese
 * @brief 获取当前缓冲区列表的有效buffer个数
 * @param[in] hStream 数据流句柄
 * @param[out] pnValidBufferNum 当前缓冲区有效buffer个数的指针
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 缓冲区有效指缓冲区状态为：ITKBUFFER_VAL_STATE_FULL、ITKBUFFER_VAL_STATE_OVERFLOW、ITKBUFFER_VAL_STATE_UNCOMPLETED
 * @see ItkStreamAddBuffer
*/
 /**
\~english
 * @brief Gets the valid buffer number of the current bufferList
 * @param[in] hStream Data stream handle
 * @param[out] pnValidBufferNum Pointer to the valid buffer number of the current bufferList
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The buffer is valid means that the buffer status is: ITKBUFFER_VAL_STATE_FULL、ITKBUFFER_VAL_STATE_OVERFLOW、ITKBUFFER_VAL_STATE_UNCOMPLETED 
 * @see ItkStreamAddBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamGetValidBufferNum(ITKSTREAM hStream, uint32_t* pnValidBufferNum);

/**
\~chinese 
 * @brief 获取数据流参数
 * @param[in] hStream 数据流句柄
 * @param[in] prm 参数ID
 * @param[out] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamSetPrm ITKSTREAM_PRM (n, size)
*/
 /**
\~english 
 * @brief Get data stream parameter
 * @param[in] hStream Data stream handle
 * @param[in] prm Parameter ID
 * @param[out] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamSetPrm ITKSTREAM_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamGetPrm(ITKSTREAM hStream, uint32_t prm, void* pValue);

/**
\~chinese 
 * @brief 设置数据流参数
 * @param[in] hStream 数据流句柄
 * @param[in] prm 参数ID
 * @param[in] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamGetPrm ITKSTREAM_PRM (n, size)
*/
 /**
\~english 
 * @brief Set data stream parameter
 * @param[in] hStream Data stream handle
 * @param[in] prm Parameter ID
 * @param[in] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamGetPrm ITKSTREAM_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamSetPrm(ITKSTREAM hStream, uint32_t prm, const void* value);

/**
\~chinese 
 * @brief 获取当前帧完成回调中指向当前完成帧的缓冲区句柄
 * @param[in] hStream 数据流句柄
 * @param[out] phBuffer 当前完成帧的缓冲区句柄
 * @note  获取当前帧完成回调中指向当前完成帧的缓冲区句柄, 需要注意该函数只有在帧完成回调中使用时才可以获取到正确的值.
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get buffer that indacate current frame in end-of-frame callback.
 * @param[in] hStream Data stream handle
 * @param[out] phBuffer Buffer that indacate current frame
 * @note  Get buffer that indacate current frame in end-of-frame callback. 
 *        Use this function to get correct value only in end-of-frame callback.
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamGetCurrentBuffer(ITKSTREAM hStream, ITKBUFFER* phBuffer);

/**
\~chinese 
 * @brief 开始数据流采集工作
 * @param[in] hStream 数据流句柄
 * @param[in] count 采集图像数量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 *  @note 
 * 1. 采集图像数量对应关系如下所示：
 * | 数值                 | 图像帧数 |
 * |----------------------|----------|
 * | 0                    | 连续采集 |
 * | ITKSTREAM_CONTINUOUS | 连续采集 |
 * | 1                    | 单帧采集 |
 * | 2~UINT32_MAX-1       | 多帧采集 |
 * 2. 如果数据流正在进行采集，则本函数不会执行并返回 ITKSTATUS_STREAM_IN_PROCESS 。
 * 3. 如果参数 ITKSTREAM_PRM_START_MODE 被设置为 ITKSTREAM_VAL_START_MODE_NON_BLOCK ，则本函数不等待采集过程结束而直接返回。
 * 4. 如果参数 ITKSTREAM_PRM_START_MODE 被设置为 ITKSTREAM_VAL_START_MODE_BLOCK ，则本函数会等待所有图像数据采集完毕或者采集图像超时才会返回。
 * 5. 每个采集数据流维护独立的缓冲区链表，数据采集流将会顺序写入缓冲区链表中。假设缓冲区链表长度为4，则采集得到的图像将按照如下顺序排列：
 * * 单帧采集时(count=1)：1→1→1→1→…
 * * 多帧采集时(count=2)：(1→2)→(1→2)→…
 * * 多帧采集时(count=6)：(1→2→3→4→1→2)→(1→2→3→4→1→2)→…
 * * 连续采集时(count= ITKSTREAM_CONTINUOUS )：(1→2→3→4)→(1→2→3→4)→…
 *  @see ItkStreamStop
*/
 /**
\~english 
 * @brief Start data stream acquisition
 * @param[in] hStream Data stream handle
 * @param[in] count Number of grabbed images
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 *  @note 
 * 1. The corresponding relationship between the number of acquisition images is as follows:
 * | Value                | Image Count              |
 * |----------------------|--------------------------|
 * | 0                    | Continuous acquisition   |
 * | ITKSTREAM_CONTINUOUS | Continuous acquisition   |
 * | 1                    | Single frame acquisition |
 * | 2~UINT32_MAX-1       | Multi frame acquisition  |
 * 2. If the data stream is in process of acquisition, this function will not execute and return ITKSTATUS_STREAM_IN_PROCESS .
 * 3. If the parameter ITKSTREAM_PRM_START_MODE is set to ITKSTREAM_VAL_START_MODE_NON_BLOCK , 
 this function returns directly without waiting for acquisition process to end.
 * 4. If the parameter ITKSTREAM_PRM_START_MODE is set to ITKSTREAM_VAL_START_MODE_BLOCK , 
 this function will wait for all image data to be grabbed or for the grabbed images to timeout before returning.
 * 5. Each acquisition data stream maintains an independent buffer linked list, 
 and data streams will be sequentially written into buffer linked list. Assuming the length of the buffer linked list is 4, 
 the grabbed images will be arranged in the following order:
 * * During single frame acquisition(count=1)：1→1→1→1→…
 * * During multi frame acquisition(count=2)：(1→2)→(1→2)→…
 * * During multi frame acquisition(count=6)：(1→2→3→4→1→2)→(1→2→3→4→1→2)→…
 * * During continuous acquisition(count= ITKSTREAM_CONTINUOUS )：(1→2→3→4)→(1→2→3→4)→…
 *  @see ItkStreamStop
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamStart(ITKSTREAM hStream, uint32_t count);

/**
\~chinese 
 * @brief 同步结束数据流采集工作
 * @param[in] hStream 数据流句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当本函数执行后，直到当前正在采集的图像采集完毕才会结束采集过程，因此可以保证最后一帧图像的完整性。
 * @see ItkStreamAbort
*/
 /**
\~english 
 * @brief Synchronously stop data stream acquisition
 * @param[in] hStream Data stream handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note After the execution of this function, acquisition process will not end until the current image being grabbed is completed, thus ensuring the integrity of the last frame of image.
 * @see ItkStreamAbort
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamStop(ITKSTREAM hStream);

/**
\~chinese 
 * @brief 等待数据流采集结束
 * @param[in] hStream 数据流句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当采集单帧或者多帧图像数据时，可以调用该函数等待图像采集操作完成。在连续采集时将直接返回ITKSTATUS_OK.
 * @see ItkStreamStart
*/
 /**
\~english 
 * @brief Waiting for the end of data stream acquisition
 * @param[in] hStream Data stream handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note When grabbing single or multiple frames of image data, this function can be called to wait for the image acquisition operation completion. 
 * In continuous acquisition, it will return ITKSTATUS_OK immediately.
 * @see ItkStreamStart
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamWait(ITKSTREAM hStream);

/**
\~chinese
 * @brief 等待一帧图像完成并获取这帧图像的句柄
 * @param[in] hStream 数据流句柄
 * @param[out] hBuffer 图像句柄
 * @param[in] timeout_ms 超时时间(ms)，如果输入值为小于0，则表示超时时间为无限
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG      hBuffer为空
 * - ITKSTATUS_INVALID_HANDLE   hStream句柄无效
 * - ITKSTATUS_STREAM_PENDING   采集尚未开始
 * - ITKSTATUS_STREAM_TIMEOUT   采集超时
 * - ITKSTATUS_STREAM_ABORTED   采集被终止
 * @note 
 * 1. 在获取图像并处理完成后，需要调用 ItkStreamClearBuffer 将缓冲区释放。
 * 2. 当内部缓冲区中已经有已完成的图像时，该接口会立即返回。
 * 3. 默认情况下该接口会优先返回最早到来的图像，直到内部缓冲区没有已经完成的图像。
 * 4. 如果客户处理图像并释放缓冲区速度慢于采图速度会发生缓冲区溢出现象进而造成丢帧。
 * @see ItkStreamStart
*/
 /**
\~english
 * @brief Wait one frame ready and get the handle of it
 * @param[in] hStream Data stream handle
 * @param[out] hBuffer Image buffer handle
 * @param[in] timeout_ms timeout(ms). If it is less than 0, timeout is infinite
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG      hBuffer is NULL
 * - ITKSTATUS_INVALID_HANDLE   hStream is invalid
 * - ITKSTATUS_STREAM_PENDING   grab is not started
 * - ITKSTATUS_STREAM_TIMEOUT   wait one frame timeout
 * - ITKSTATUS_STREAM_ABORTED   wait abort because grab is stop
 * @note 
 * 1. After ItkStreamWaitOneFrameReady and processing image, need use ItkStreamClearBuffer to release buffer.
 * 2. This api return instantly if there are one or more ready frames in internal buffers.
 * 3. This api return the earliest ready frame on default mode.
 * 4. If the image process and the buffer release slower than the acquisition speed, the buffer overflow will cause frame loss.
 * @see ItkStreamStart
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamWaitOneFrameReady(ITKSTREAM hStream, ITKBUFFER* hBuffer, long long timeout_ms);

/**
\~chinese 
 * @brief 注册数据流回调函数
 * @param[in] hStream 数据流句柄
 * @param[in] eventType 回调事件类型，事件触发后回调函数会被执行
 * @param[in] callback 用户回调函数
 * @param[in] context 用户上下文指针，在回调函数调用时传入
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 如果在回调函数中进行过于费时的操作或事件触发频率过快，可能会导致触发事件丢失。
 * 2. 为相同事件注册多次回调函数，仅保证最近一次注册的回调函数会被调用。
 * 3. 当为 ITKSTREAM_VAL_EVENT_TYPE_END_OF_LINE 注册回调函数时，必须指明监听的行数n，
      其回调事件类型为 ITKSTREAM_VAL_EVENT_TYPE_END_OF_LINE +n。该类型仅适用于线扫描相机。
 * @see ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english 
 * @brief Register data stream callback function
 * @param[in] hStream Data stream handle
 * @param[in] eventType Callback event type, the callback function will be executed after the event is triggered
 * @param[in] callback User callback function
 * @param[in] context User context pointer, passed in when calling callback function
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. If time-consuming operations or event triggering frequency are too fast in callback function, it may result in the loss of triggering events.
 * 2. Register multiple callback functions for the same event, only the most recently registered callback function will be called.
 * 3. When registering a callback function for ITKSTREAM_VAL_EVENT_TYPE_END_OF_LINE , it is necessary to specify the number of listening lines n, 
      and the callback event type is ITKSTREAM_VAL_EVENT_TYPE_END_OF_LINE +n. This type is only applicable to line scan camera.
 * @see ItkStreamUnregisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkStreamRegisterCallback(ITKSTREAM hStream, uint32_t eventType, PITKSTREAMCALLBACK callback, void* context);

/**
\~chinese 
 * @brief 清空数据流回调函数
 * @param[in] hStream 数据流句柄
 * @param[in] eventType 回调事件类型
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamRegisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
*/
 /**
\~english 
 * @brief Clear data stream callback function
 * @param[in] hStream Data stream handle
 * @param[in] eventType Callback event type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamRegisterCallback ITKSTREAM_PRM_SUPPORT_EVENT_TYPE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamUnregisterCallback(ITKSTREAM hStream, uint32_t eventType);

/**
\~chinese 
 * @brief 开始录像
 * @param[in] hStream 数据流句柄
 * @param[in] pRecordParam 录像参数结构体 
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 图像像素支持通道深度为8位的灰白图像和通道深度为8位的彩色图像。
 * 2. 视频录制功能需链接ffmpeg动态库。
 * @see ItkStreamInputOneFrame ItkStreamStopRecord
*/
 /**
\~english 
 * @brief Start record
 * @param[in] hStream Stream handle
 * @param[in] pRecordParam Record param structure
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. The image pixelformat data only supports gray images with channel depth of 8 bits and color images with channel depth of 8 bits.
 * 2. The video record functions need link ffmpeg dll. 
 * @see ItkStreamInputOneFrame ItkStreamStopRecord
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamStartRecord(ITKSTREAM hStream, ITK_RECORD_PARAM* pRecordParam);

/**
\~chinese 
 * @brief  输入一帧录像数据
 * @param[in] hStream 数据流句柄
 * @param[in] pSrcDataAddress 数据地址 
 * @param[in] nDataLen 数据长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 输入图像数据支持通道深度为8位的灰白图像和通道深度为8位的彩色图像。
 * @see ItkStreamStartRecord ItkStreamStopRecord
*/
 /**
\~english 
 * @brief Input one frame of RAW data
 * @param[in] hStream Stream handle
 * @param[in] pSrcDataAddress The Address of data
 * @param[in] nDataLen The Length of data
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The input image data supports gray images with channel depth of 8 bits and color images with channel depth of 8 bits.
 * @see ItkStreamStartRecord ItkStreamStopRecord
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamInputOneFrame(ITKSTREAM hStream, void* pSrcDataAddress, uint32_t nDataLen);

/**
\~chinese 
 * @brief 结束录像
 * @param[in] hStream 数据流句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamStartRecord ItkStreamInputOneFrame
*/
 /**
\~english 
 * @brief Stop record
 * @param[in] hStream Stream handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkStreamStartRecord ItkStreamInputOneFrame
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkStreamStopRecord(ITKSTREAM hStream);

/**
\~chinese 
 * @brief 新建缓冲区
 * @param[in] width 缓冲区宽度，单位像素
 * @param[in] height 缓冲区高度，单位像素
 * @param[in] format 缓冲区像素格式
 * @param[out] phBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
*/
 /**
\~english 
 * @brief Create new buffer
 * @param[in] width Buffer width, unit pixel
 * @param[in] height Buffer height, unit pixel
 * @param[in] format Buffer pixel format
 * @param[out] phBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferNew(int64_t width, int64_t height, uint32_t format, ITKBUFFER* phBuffer);

/**
\~chinese 
 * @brief 新建缓冲区，适用于缓冲区不仅包含图像数据，还包含Chunk数据
 * @param[in] bufferSize 缓冲区大小
 * @param[in] width 缓冲区宽度，单位像素
 * @param[in] height 缓冲区高度，单位像素
 * @param[in] format 缓冲区像素格式
 * @param[in] payloadType Chunk数据类型，见enum PAYLOAD_TYPE
 * @param[out] phBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
*/
 /**
\~english 
 * @brief Create new buffer，applicable to buffers that contain Chunk data as well as image data 
 * @param[in] bufferSize Buffer size
 * @param[in] width Buffer width, unit pixel
 * @param[in] height Buffer height, unit pixel
 * @param[in] format Buffer pixel format
 * @param[in] payloadType Chunk data type, see enum PAYLOAD_TYPE
 * @param[out] phBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
 */
 IKAPC_API ITKSTATUS IKAPC_CC ItkBufferSizedNew(int64_t bufferSize, int64_t width, int64_t height, uint32_t format, uint32_t payloadType, ITKBUFFER* phBuffer);

/**
\~chinese 
 * @brief 使用SDK外部内存新建缓冲区
 * @param[in] width 缓冲区宽度，单位像素
 * @param[in] height 缓冲区高度，单位像素
 * @param[in] format 缓冲区像素格式
 * @param[in] address 缓冲区地址，由用户申请
 * @param[out] phBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
*/
 /**
\~english 
 * @brief Create a new buffer using the SDK external memory
 * @param[in] width Buffer width, unit pixel
 * @param[in] height Buffer height, unit pixel
 * @param[in] format Buffer pixel format
 * @param[in] address Buffer address, requested by users
 * @param[out] phBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkBufferNewWithPtr(int64_t width, int64_t height, uint32_t format, void* address, ITKBUFFER* phBuffer);

/**
\~chinese 
 * @brief 使用SDK外部内存新建缓冲区,适用于缓冲区不仅包含图像数据，还包含Chunk数据
 * @param[in] bufferSize 缓冲区大小
 * @param[in] width 缓冲区宽度，单位像素
 * @param[in] height 缓冲区高度，单位像素
 * @param[in] format 缓冲区像素格式
 * @param[in] payloadType Chunk数据类型，见enum PAYLOAD_TYPE 
 * @param[in] address 缓冲区地址，由用户申请
 * @param[out] phBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
*/
 /**
\~english 
 * @brief Create a new buffer using the SDK external memory, applicable to buffers that contain Chunk data as well as image data
 * @param[in] bufferSize Buffer size
 * @param[in] width Buffer width, unit pixel
 * @param[in] height Buffer height, unit pixel
 * @param[in] format Buffer pixel format
 * @param[in] payloadType Chunk data type, see enum PAYLOAD_TYPE
 * @param[in] address Buffer address, requested by users
 * @param[out] phBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferFree ITKBUFFER_PRM_FORMAT
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferSizedNewWithPtr(int64_t bufferSize, int64_t width, int64_t height, uint32_t format, uint32_t payloadType, void* address, ITKBUFFER* phBuffer);

/**
\~chinese 
 * @brief 判断缓冲区内存是否为用户创建
 * @param[in] hBuffer 缓冲区句柄
 * @param[out] bExternal 标记内存是否为用户创建
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 *  @see ItkBufferNewWithPtr
*/
 /**
\~english 
 * @brief Determine whether the buffer memory is created by the user
 * @param[in] hBuffer Buffer handle
 * @param[out] bExternal Mark whether the memory is created by the user
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 *  @see ItkBufferNewWithPtr
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkBufferIsExternal(ITKBUFFER hBuffer, bool* bExternal);

/**
\~chinese 
 * @brief 销毁缓冲区
 * @param[in] hBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 *  @see ItkBufferNew
*/
 /**
\~english 
 * @brief Destroy buffer
 * @param[in] hBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 *  @see ItkBufferNew
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferFree(ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 获取缓冲区信息, 相比较ItkBufferGetPrm的简单实现.
 * @param[in] hBuffer 缓冲区句柄
 * @param[out] pInfo 缓冲区描述信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get buffer info.
 * @param[in] hBuffer Buffer handle
 * @param[out] pInfo Info of buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGetInfo(ITKBUFFER hBuffer, ITK_BUFFER_INFO *pInfo);

/**
\~chinese 
 * @brief 获取缓冲区PART信息, 仅在缓冲区负载为MULTI_PART_PAYLOAD_TYPE时使用
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] index  缓冲区part序号, 应小于缓冲区的PartNumber.
 * @param[out] pInfo 缓冲区Part描述信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
*/
 /**
\~english 
 * @brief Get buffer part info,  only used when the buffer payload type is MULTI_PART_PAYLOAD_TYPE.
 * @param[in] hBuffer Buffer handle
 * @param[in] index Buffer part index 
 * @param[out] pInfo Part info of buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGetPartInfo(ITKBUFFER hBuffer, uint64_t index, ITK_BUFFER_PART_INFO *pInfo);

/**
\~chinese 
 * @brief 设置缓冲区地址.
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] address 指向待设置的缓冲区的地址
 * @param[in] size 待设置的缓冲区大小
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferNewWithPtr ItkStreamGetBuffer
*/
 /**
\~english 
 * @brief Set buffer address.
 * @param[in] hBuffer Buffer handle
 * @param[in] address Pointer to buffer memory to set
 * @param[in] size Size of the buffer in bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferNewWithPtr ItkStreamGetBuffer
*/
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferSetAddress(ITKBUFFER hBuffer, void *address, uint64_t size);
/**
\~chinese 
 * @brief 从缓冲区中读取数据
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] offset 缓冲区偏移量，单位字节
 * @param[out] data 用户缓冲区
 * @param[in,out] size 作为输入指明用户缓冲区最大长度，作为输出指明用户缓冲区有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 缓冲区偏移量和用户缓冲区大小之和不能超过数据缓冲区的大小，即offset + size <= ITKBUFFER_PRM_SIZE
 * @see ItkBufferWrite
*/
 /**
\~english 
 * @brief Read data from buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] offset Buffer offset in bytes
 * @param[out] data User buffer
 * @param[in,out] size As input, it indicates the maximum length of user buffer. As output, it indicates the effective length of user buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note The sum of buffer offset and user buffer size cannot exceed the size of data buffer, that is offset + size <= ITKBUFFER_PRM_SIZE
 * @see ItkBufferWrite
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferRead(ITKBUFFER hBuffer, uint32_t offset, void* data, uint32_t size);

/**
\~chinese 
 * @brief 读取缓冲区中的一个像素值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] posX 像素水平位置
 * @param[in] posY 像素垂直位置
 * @param[in] element 像素值
 * @param[in] size 读取数据的大小，单位字节
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferWriteElement
*/
 /**
\~english 
 * @brief Read single pixel value from buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] posX Pixel horizontal position
 * @param[in] posY Pixel vertical position
 * @param[in] element Pixel value
 * @param[in] size Read the size of data in bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferWriteElement
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkBufferReadElement(ITKBUFFER hBuffer, uint32_t posX, uint32_t posY, void* element, uint32_t size);

/**
\~chinese 
 * @brief 读取缓冲区中直线上的像素值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] startX 缓冲区起点的水平位置
 * @param[in] startY 缓冲区起点的垂直位置
 * @param[in] endX 缓冲区终点的水平位置
 * @param[in] endY 缓冲区终点的垂直位置
 * @param[in] array 存放读取像素的数组
 * @param[in] uElements 读取像素的个数
 * @param[in] size 数组的大小，单位字节
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @note 使用Bresenham算法确定(startX, startY)和（endX, endY）两点间的理论直线。
 * @see ItkBufferWriteLine
*/
 /**
\~english 
 * @brief Read pixel values of a line in buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] startX The horizontal position of the starting point of buffer
 * @param[in] startY The vertical position of the starting point of buffer
 * @param[in] endX The horizontal position of the end point of buffer
 * @param[in] endY The vertical position of the end point of buffer
 * @param[in] array Store an array of read pixels
 * @param[in] uElements The number of read pixels
 * @param[in] size The size of array, unit bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @note Use the Bresenham algorithm to determine the theoretical straight line between two points (startX, startY) and (endX, endY).
 * @see ItkBufferWriteLine
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferReadLine(
    ITKBUFFER hBuffer, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY, uint32_t* uElements, void* data,
    uint32_t size);

/**
\~chinese 
 * @brief 读取缓冲区中矩形区域的像素值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] offsetX 缓冲区的水平起始位置
 * @param[in] offsetY 缓冲区的垂直起始位置
 * @param[in] width 缓冲区矩形区域的宽度
 * @param[in] height 缓冲区矩形区域的高度
 * @param[in] data 存放读取数据的数组
 * @param[in] size 数组的大小，单位字节
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferWriteRect
*/
 /**
\~english 
 * @brief Read pixel values of the rectangular area in buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] offsetX Horizontal offset of the rectangular area of buffer
 * @param[in] offsetY Vertical offset of the rectangular area of buffer
 * @param[in] width Width of the rectangular area of buffer
 * @param[in] height Height of the rectangular area of buffer
 * @param[in] data An array for storing read data
 * @param[in] size The size of array, unit bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferWriteRect
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferReadRect(
    ITKBUFFER hBuffer, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height, void* data, uint32_t size);

/**
\~chinese 
 * @brief 向缓冲区写入数据
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] offset 数据缓冲区的偏移量，单位字节
 * @param[in] data 用户缓冲区的大小，单位字节
 * @param[in,out] size 作为输入指明用户缓冲区最大长度，作为输出指明用户缓冲区有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * @note 缓冲区偏移量和用户缓冲区大小之和不能超过数据缓冲区的大小，即offset + size <= ITKBUFFER_PRM_SIZE
 * @see ItkBufferRead
*/
 /**
\~english 
 * @brief Write data to buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] offset Offset of data buffer, in bytes
 * @param[in] data The size of user buffer, in bytes
 * @param[in,out] size As input, it indicates the maximum length of user buffer. As output, it indicates the effective length of user buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * @note The sum of buffer offset and user buffer size cannot exceed the size of data buffer, that is offset + size <= ITKBUFFER_PRM_SIZE
 * @see ItkBufferRead
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferWrite(ITKBUFFER hBuffer, uint32_t offset, const void* data, uint32_t size);

/**
\~chinese 
 * @brief 向缓冲区写入单个像素数据
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] posX 像素的水平位置
 * @param[in] posY 像素的垂直位置
 * @param[in] element 像素值
 * @param[in] size 数据大小，单位字节
 * @return
 * -ITKSTATUS_OK
 * -ITKSTATUS_INVALD_ARG
 * -ITKSTATUS_IVNALID_HANDLE
 * -ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferReadElement
*/
 /**
\~english 
 * @brief Write single pixel data to buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] posX Pixel horizontal position
 * @param[in] posY Pixel vertical position
 * @param[in] element Pixel value
 * @param[in] size The size of data in bytes
 * @return
 * -ITKSTATUS_OK
 * -ITKSTATUS_INVALD_ARG
 * -ITKSTATUS_IVNALID_HANDLE
 * -ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferReadElement
 */
IKAPC_API ITKSTATUS IKAPC_CC
    ItkBufferWriteElement(ITKBUFFER hBuffer, uint32_t posX, uint32_t posY, const void* element, uint32_t size);

/**
\~chinese 
 * @brief 向缓冲区写入一行像素数据
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] startX 缓冲区起点的水平位置
 * @param[in] startY 缓冲区起点的垂直位置
 * @param[in] endX 缓冲区终点的水平位置
 * @param[in] endY 缓冲区终点的垂直位置
 * @param[in] array 存放写入像素的数组
 * @param[in] uElements 写入像素的个数
 * @param[in] size 写入像素数组的大小，单位字节
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 *  @note 使用Bresenham算法确定(startX, startY)和（endX, endY）两点间的理论直线。
 *  @see ItkBufferReadLine
*/
 /**
\~english 
 * @brief Write a line of pixel data to the buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] startX The horizontal position of the starting point of buffer
 * @param[in] startY The vertical position of the starting point of buffer
 * @param[in] endX The horizontal position of the end point of buffer
 * @param[in] endY The vertical position of the end point of buffer
 * @param[in] array Store an array of written pixels
 * @param[in] uElements Number of written pixels
 * @param[in] size The size of written pixel array in bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 *  @note Use the Bresenham algorithm to determine the theoretical straight line between two points (startX, startY) and (endX, endY).
 *  @see ItkBufferReadLine
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferWriteLine(
    ITKBUFFER hBuffer, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY, uint32_t* uElements, const void* data,
    uint32_t size);

/**
\~chinese 
 * @brief 向缓冲区的矩形区域写入数据
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] offsetX 缓冲区的水平起始位置
 * @param[in] offsetY 缓冲区的垂直起始位置
 * @param[in] width 缓冲区矩形区域的宽度
 * @param[in] height 缓冲区矩形区域的高度
 * @param[in] data 包含写入像素值的数组
 * @param[in] size 写入的数据的大小，单位字节
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferReadRect
*/
 /**
\~english 
 * @brief Write data to rectangular area of buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] offsetX Horizontal offset of the rectangular area of buffer
 * @param[in] offsetY Vertical offset of the rectangular area of buffer
 * @param[in] width Width of the rectangular area of buffer
 * @param[in] height Height of the rectangular area of buffer
 * @param[in] data An array containing written pixel values
 * @param[in] size The size of the written data, in bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALD_ARG
 * - ITKSTATUS_IVNALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferReadRect
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferWriteRect(
    ITKBUFFER hBuffer, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height, const void* data, uint32_t size);

/**
\~chinese 
 * @brief 将Bayer图像转换成彩色图像
 * @param[in] hSrc Bayer编码的源图像
 * @param[in] hDst 转换后的彩色图像
 * @param[in] options Bayer图像的编码方式
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 在Bayer格式的图像中，每个像素的灰度值都对应了彩色图像的某个像素通道值，通过最邻近差值方法可以从Bayer图像中重构出彩色图像。
 * 2. 源图像和转换后的图像必须具有相同的宽度、高度和通道深度，否则无法进行Bayer转换。
 * 3. Bayer编码的源图像数据格式必须是以下取值之一：
 * | 源图像数据格式              |
 * |---------------------------------|
 * | ITKBUFFER_VAL_FORMAT_MONO8      |
 * | ITKBUFFER_VAL_FORMAT_MONO10     |
 * | ……                              |
 * | ITKBUFFER_VAL_FORMAT_BAYER_RG8  |
 * | ITKBUFFER_VAL_FORMAT_BAYER_RG10 |
 * | ……                              |
 * 4. 转换后的彩色图像数据格式必须是以下取值之一：
 * | 转换图像数据格式               |
 * |--------------------------------|
 * | ITKBUFFER_VAL_FORMAT_RGB888    |
 * | ITKBUFFER_VAL_FORMAT_RGB101010 |
 * | ITKBUFFER_VAL_FORMAT_RGB121212 |
 * | ITKBUFFER_VAL_FORMAT_RGB141414 |
 * | ITKBUFFER_VAL_FORMAT_RGB161616 |
 * | ITKBUFFER_VAL_FORMAT_BGR888    |
 * | ITKBUFFER_VAL_FORMAT_BGR101010 |
 * | ITKBUFFER_VAL_FORMAT_BGR121212 |
 * | ITKBUFFER_VAL_FORMAT_BGR141414 |
 * | ITKBUFFER_VAL_FORMAT_BGR161616 |
 * 5. Bayer图像的编码方式必须是以下取值之一：
 * | Bayer图像编码方式         |
 * |---------------------------|
 * | ITKBUFFER_VAL_BAYER_BGGR  |
 * | ITKBUFFER_VAL_BAYER_RGGB  |
 * | ITKBUFFER_VAL_BAYER_GBRG  |
 * | ITKBUFFER_VAL_BAYER_GRBG  |
 * 6. Bayer图像的转换方法必须是以下取值之一：
 * | Bayer图像转换方法                   |
 * |-------------------------------------|
 * | ITKBUFFER_VAL_BAYER_METHOD_DEFAULT  |
 * | ITKBUFFER_VAL_BAYER_METHOD_BILINEAR |
 * | ITKBUFFER_VAL_BAYER_METHOD_VNG      |
 * | ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4  |
 * | ITKBUFFER_VAL_BAYER_METHOD_ITEK_OLD |
 * | ITKBUFFER_VAL_BAYER_METHOD_DFCD2    |
 * 7. 编码方式和转换方法通过或(|)连接传入options. 注意当使用16bit的图像时, 只能使用 ITKBUFFER_VAL_BAYER_METHOD_BILINEAR 方法.
 * @see ItkBufferNew
*/
 /**
\~english 
 * @brief Convert Bayer image to color image
 * @param[in] hSrc Bayer encoded source image
 * @param[in] hDst Converted color image
 * @param[in] options The encoding method of Bayer image
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. In Bayer format images, the grayscale value of each pixel corresponds to a certain pixel channel value of the color image, 
 and the color image can be reconstructed from the Bayer image using the nearest neighbor difference method.
 * 2. The source image and the converted image must have the same width, height, and channel depth, otherwise Bayer conversion cannot be performed.
 * 3. The source image data format encoded by Bayer must take one of the following values:
 * | Source image data format        |
 * |---------------------------------|
 * | ITKBUFFER_VAL_FORMAT_MONO8      |
 * | ITKBUFFER_VAL_FORMAT_MONO10     |
 * | ……                              |
 * | ITKBUFFER_VAL_FORMAT_BAYER_RG8  |
 * | ITKBUFFER_VAL_FORMAT_BAYER_RG10 |
 * | ……                              |
 * 4. The converted color image data format must be one of the following values:
 * | Converted image data format    |
 * |--------------------------------|
 * | ITKBUFFER_VAL_FORMAT_RGB888    |
 * | ITKBUFFER_VAL_FORMAT_RGB101010 |
 * | ITKBUFFER_VAL_FORMAT_RGB121212 |
 * | ITKBUFFER_VAL_FORMAT_RGB141414 |
 * | ITKBUFFER_VAL_FORMAT_RGB161616 |
 * | ITKBUFFER_VAL_FORMAT_BGR888    |
 * | ITKBUFFER_VAL_FORMAT_BGR101010 |
 * | ITKBUFFER_VAL_FORMAT_BGR121212 |
 * | ITKBUFFER_VAL_FORMAT_BGR141414 |
 * | ITKBUFFER_VAL_FORMAT_BGR161616 |
 * 5. The encoding method of Bayer image must be one of the following values:
 * | Bayer image encoding method |
 * |-----------------------------|
 * | ITKBUFFER_VAL_BAYER_BGGR    |
 * | ITKBUFFER_VAL_BAYER_RGGB    |
 * | ITKBUFFER_VAL_BAYER_GBRG    |
 * | ITKBUFFER_VAL_BAYER_GRBG    |
 * 6. The bayer convert method of Bayer image must be one of the following values:
 * | Bayer image convert method          |
 * |-------------------------------------|
 * | ITKBUFFER_VAL_BAYER_METHOD_DEFAULT  |
 * | ITKBUFFER_VAL_BAYER_METHOD_BILINEAR |
 * | ITKBUFFER_VAL_BAYER_METHOD_VNG      |
 * | ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4  |
 * | ITKBUFFER_VAL_BAYER_METHOD_ITEK_OLD |
 * | ITKBUFFER_VAL_BAYER_METHOD_DFCD2    |
 * 7. Encoding methods and conversion methods passed to the incoming options through or(|) connection.
 *  Note that when working with 16-bit images, only the ITKBUFFER_VAL_BAYER_METHOD_BILINEAR method can be used.
 * @see ItkBufferNew
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferBayerConvert(ITKBUFFER hSrc, ITKBUFFER hDst, uint32_t options);

/**
\~chinese
 * @brief RGB通道重排列
 * @param[in] hSrc：源缓冲区句柄，像素格式必须是彩色格式
 * @param[in] hDst：目的缓冲区句柄。目的缓冲区图像高度必须是源缓冲区图像高度的3倍，像素格式大小必须是源缓冲区的1/3
 * @param[in] option：重排选项:{ITKBUFFER_VAL_RGB_SPLIT_INTO_ROW,ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK}
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK 选项对应的重排如下
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
 * @see ItkBufferNew ItkBufferRGBSplit ITKBUFFER_VAL_RGB_SPLIT_INTO_ROW ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK
 */
 /**
\~english
 * @brief RGB channel rearrangement
 * @param[in] hSrc: Source buffer handle, pixel format must be in color format
 * @param[in] hDst: Destination buffer handle. The height of the destination buffer image must be three times that of the source buffer image,
 and the pixel format size must be one-third that of the source buffer
 * @param[in] option: option to rearrange:{ITKBUFFER_VAL_RGB_SPLIT_INTO_ROW,ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK}
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note Pixel after rearrangement with ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK
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
 * @see ItkBufferNew ItkBufferRGBSplit ITKBUFFER_VAL_RGB_SPLIT_INTO_ROW ITKBUFFER_VAL_RGB_SPLIT_INTO_BLOCK
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferRGBSplitEx(ITKBUFFER hSrc, ITKBUFFER hDst, int option);

/**
\~chinese
 * @brief 3D像素通道重排列
 * @param[in] hSrc：源缓冲区句柄，像素格式必须是3D格式
 * @param[in] hDst：目的缓冲区句柄。对于C16、AC16和ACR16目的缓冲区图像高度必须是源缓冲区图像高度的N倍，输出像素大小必须为输入的单通道大小，N为源缓冲区的通道个数，对于C32和AC32目的缓冲区图像高度必须是源缓冲区图像高度，输出像素大小必须为(输入的像素通道数乘以16bit)加8bit
 * @param[in] options1：重排参数
 * @param[in] options2：当option1为ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED时表示batchCount，其他情况未启用
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK 选项,对应的重排如下
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
 * ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED 选项,对应的重排如下
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
 * @see ItkBufferNew ItkBufferRGBSplit ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED
 */
 /**
\~english
 * @brief 3D channel rearrangement
 * @param[in] hSrc: Source buffer handle, pixel format must be in 3D format
 * @param[in] hDst: Destination buffer handle. For C16、AC16 and ACR16 the target buffer image height must be N times the source buffer image height, the output pixel size must be the single channel size of the input, and N is the number of channels in the source buffer. For C32 and AC32 the target buffer image height must be same as the source buffer image height, the output pixel size must be (the pixel channel count of the input multiple 16bit) plus 8 bit
 * @param[in] options1: option to rearrange
 * @param[in] options2: batchCount is indicated when option1 is ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED, otherwise  which is not enabled
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note Pixel after rearrangement with ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK:
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
 * Pixel after rearrangement with ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED:
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
 * @see ItkBufferNew ItkBufferRGBSplit ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK ITKBUFFER_VAL_3D_SPLIT_INTO_BLOCK_AND_STRATIFIED
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBuffer3DSplit(ITKBUFFER hSrc, ITKBUFFER hDst, uint32_t options1,uint32_t options2);

/**
\~chinese
 * @brief 图像缩放
 * @param[in] hSrc：源缓冲区句柄
 * @param[in] hDst：目的缓冲区句柄。目的缓冲区除图像宽高与源缓冲区不一致外，其余参数均需保持一致
 * @param[in] option：缩放选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 缓冲区数据格式仅支持非packed的Mono和RGB/BGR。
 * @see ItkBufferNew ITKBUFFER_VAL_RESIZE_NEAREST ITKBUFFER_VAL_RESIZE_LINEAR ITKBUFFER_VAL_RESIZE_CUBIC ITKBUFFER_VAL_RESIZE_AREA
 */
 /**
\~english
 * @brief resize image
 * @param[in] hSrc: Source buffer handle
 * @param[in] hDst: Destination buffer handle. All the target image parameters should be consistent except that
 the width and height are inconsistent with the original image
 * @param[in] option: option to resize
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note Buffer data format only supports non-packed Mono and RGB/BGR.
 * @see ItkBufferNew ITKBUFFER_VAL_RESIZE_NEAREST ITKBUFFER_VAL_RESIZE_LINEAR ITKBUFFER_VAL_RESIZE_CUBIC ITKBUFFER_VAL_RESIZE_AREA
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferResize(ITKBUFFER hSrc, ITKBUFFER hDst, int option);

/**
\~chinese 
 * @brief 将3D图像转换成pcd图像
 * @param[in] hStream 数据流句柄
 * @param[in] hSrc 源图像数据格式
 * @param[in] hDst 转换后的图像数据格式
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 源图像数据格式必须是以下取值之一：
 * | 源图像数据格式                     |
 * |------------------------------------|
 * | ITKBUFFER_VAL_FORMAT_COORD3D_C16   |
 * | ITKBUFFER_VAL_FORMAT_COORD3D_AC16  |
 * | ITKBUFFER_VAL_FORMAT_COORD3D_ACR16 |
 * | ITKBUFFER_VAL_FORMAT_CALIBRATED_COORD3D_AC32  |
 * | ITKBUFFER_VAL_FORMAT_CALIBRATED_COORD3D_C32 |
 * 2. 转换后的图像数据格式必须是以下取值之一：
 * | 转换图像数据格式         |
 * |--------------------------|
 * | ITKBUFFER_VAL_FORMAT_PCD |
 * @see ItkBufferNew
*/
 /**
\~english 
 * @brief Convert 3D images to pcd images
 * @param[in] hStream Data stream handle
 * @param[in] hSrc Source image data format
 * @param[in] hDst Converted image data format
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. The format of the source image data must be one of the following values:
 * | Source image data format           |
 * |------------------------------------|
 * | ITKBUFFER_VAL_FORMAT_COORD3D_C16   |
 * | ITKBUFFER_VAL_FORMAT_COORD3D_AC16  |
 * | ITKBUFFER_VAL_FORMAT_COORD3D_ACR16 |
 * | ITKBUFFER_VAL_FORMAT_CALIBRATED_COORD3D_AC32  |
 * | ITKBUFFER_VAL_FORMAT_CALIBRATED_COORD3D_C32 |
 * 2. The converted image data format must be one of the following values:
 * | Convert image data format |
 * |---------------------------|
 * | ITKBUFFER_VAL_FORMAT_PCD  |
 * @see ItkBufferNew
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBuffer3DConvertToPCD(ITKSTREAM hStream, ITKBUFFER hSrc, ITKBUFFER hDst);

/**
\~chinese 
 * @brief 清除缓冲区中的数据
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] pValue 缓冲区中像素的设定值
 * @param[in] size 单个缓冲区元素的数据位
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferClearBlack
*/
 /**
\~english 
 * @brief Clear data from buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] pValue The set value of pixels in buffer
 * @param[in] size The data bits of a single buffer element
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferClearBlack
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferClear(ITKBUFFER hBuffer, const void* pValue, uint32_t size);

/**
\~chinese 
 * @brief 将缓冲区中所有像素设为黑色
 * @param[in] hBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferClear
*/
 /**
\~english 
 * @brief Set all pixels in the buffer to black
 * @param[in] hBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferClear
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferClearBlack(ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 拷贝源缓冲区中的数据到目的缓冲区中
 * @param[in] hSrc 源缓冲区句柄
 * @param[in] dstOffsetX 目的缓冲区水平位置
 * @param[in] dstOffsetY 目的缓冲区垂直位置
 * @param[in] hDst 目的缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @note 
 * 1. 若源缓冲区大于目的缓冲区，则只有在目的缓冲区有效范围内的数据才被拷贝。
 * 2. 若源缓冲区为三通道图像数据，目的缓冲区为单通道图像数据，则拷贝第一个通道的数据到目的缓冲区中，例如RGB图像拷贝R通道数值。
 * 3. 若源缓冲区为单通道图像数据，目的缓冲区为三通道图像数据，则将数据拷贝到第一个通道中。
 * 4. 若源缓冲区图像数据深度大于目的缓冲区，图像的高位信息将会丢失。
 * @see ItkBufferCopyRect
*/
 /**
\~english 
 * @brief Copy data from the source buffer to the destination buffer
 * @param[in] hSrc Source buffer handle
 * @param[in] dstOffsetX Horizontal position of the destination buffer
 * @param[in] dstOffsetY Vertical position of the destination buffer
 * @param[in] hDst Destination buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @note 
 * 1. If the source buffer is larger than the destination buffer, only data within the valid range of the destination buffer will be copied.
 * 2. If the source buffer is three channel image data and the destination buffer is single channel image data, 
 then copy the data of the first channel to the destination buffer, such as copying the R channel values for RGB images.
 * 3. If the source buffer is single channel image data and the destination buffer is three channel image data, then copy the data to the first channel.
 * 4. If the depth of the source buffer image data is greater than that of the destination buffer, the high-order information of the image will be lost.
 * @see ItkBufferCopyRect
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferCopy(ITKBUFFER hSrc, uint32_t dstOffsetX, uint32_t dstOffsetY, ITKBUFFER hDst);

/**
\~chinese 
 * @brief 拷贝源缓冲区矩形区域的数据到目的缓冲区中
 * @param[in] hSrc 源缓冲区句柄
 * @param[in] srcOffsetX 源缓冲区矩形区域水平起始位置
 * @param[in] srcOffsetY 源缓冲区矩形区域垂直起始位置
 * @param[in] width 源缓冲区矩形区域的宽度
 * @param[in] height 源缓冲区矩形区域的高度
 * @param[in] hDst 目的缓冲区句柄
 * @param[in] dstOffsetX 目的缓冲区水平起始位置
 * @param[in] dstOffsetY 目的缓冲区垂直起始位置
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferCopy
*/
 /**
\~english 
 * @brief Copy the data from the rectangular area of the source buffer to the destination buffer
 * @param[in] hSrc Source buffer handle
 * @param[in] srcOffsetX Horizontal starting position of rectangular area in the source buffer
 * @param[in] srcOffsetY Vertical starting position of rectangular area in the source buffer
 * @param[in] width Width of the rectangular area of the source buffer
 * @param[in] height Height of the rectangular area of the source buffer
 * @param[in] hDst Destination buffer handle
 * @param[in] dstOffsetX Horizontal starting position of rectangular area in the destination buffer
 * @param[in] dstOffsetY Vertical starting position of rectangular area in the destination buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_ARG_OUT_OF_RANGE
 * @see ItkBufferCopy
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferCopyRect(
    ITKBUFFER hSrc, uint32_t srcOffsetX, uint32_t srcOffsetY, uint32_t width, uint32_t height, ITKBUFFER hDst,
    uint32_t dstOffsetX, uint32_t dstOffsetY);

/**
\~chinese 
 * @brief 缓冲区图像原地顺时针旋转
 * @param[in] hSrc 源缓冲区句柄
 * @param[in] degree 旋转角度，可设置为90°、180°或270°
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 
 * 1. 若degree是90或270，图像的宽高属性会调换，再次使用该buffer时请注意这一点。
 * 2. 缓冲区数据格式仅支持非packed的Mono和RGB/BGR。
 * @see ItkBufferCopy
*/
 /**
\~english 
 * @brief Buffer image rotate clockwise in situ
 * @param[in] hSrc Source buffer handle
 * @param[in] degree Rotation angle, can be set to 90°, 180° or 270°
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 
 * 1. If the degree is 90 or 270, the width and height of the image are change, so note when using the buffer again.
 * 2. Buffer data format only supports non-packed Mono and RGB/BGR.
 * @see ItkBufferCopy
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferRotate(ITKBUFFER hSrc, uint32_t degree);

/**
\~chinese
 * @brief 缓冲区图像翻转
 * @param[in] hSrc 源缓冲区句柄
 * @param[out] hDst 目的缓冲区句柄
 * @param[in] flipCode 翻转方式标识，0表示沿X轴翻转，正值（例如1）表示沿Y轴翻转，负值（例如-1）表示沿X、Y轴翻转
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note
 * 1. 支持原地翻转。
 * 2. 缓冲区数据格式仅支持非packed的Mono和RGB/BGR。
 * @see ItkBufferRotate
*/
 /**
\~english
 * @brief Buffer image flip
 * @param[in] hSrc Source buffer handle
 * @param[out] hDst Destination buffer handle
 * @param[in] flipCode a flag to specify how to flip the buffer; 0 means flipping around the x-axis and positive value (for example, 1) means
flipping around y-axis. Negative value (for example, -1) means flipping around both axes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note
 * 1. hDst can be same with hSrc.
 * 2. Buffer data format only supports non-packed Mono and RGB/BGR.
 * @see ItkBufferRotate
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferFlip(ITKBUFFER hSrc, ITKBUFFER hDst, int32_t flipCode);

/**
\~chinese 
 * @brief 将缓冲区中的数据保存到文件
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] filename 保存的文件名
 * @param[in] options 保存的图像格式
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @see ItkBufferLoad
*/
 /**
\~english 
 * @brief Save the data in the buffer to a file
 * @param[in] hBuffer Buffer handle
 * @param[in] filename Saved file name
 * @param[in] options Saved image format
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @see ItkBufferLoad
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferSave(ITKBUFFER hBuffer, const char* filename, uint32_t options);

/**
\~chinese 
 * @brief 从文件中载入数据到缓冲区
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] filename 载入的文件名
 * @param[in] options 载入的图像格式
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 *  @note 缓冲区的数据位数和文件的数据位数必须一致。
 *  @see ItkBufferSave
*/
 /**
\~english 
 * @brief Load data from a file to buffer
 * @param[in] hBuffer Buffer handle
 * @param[in] filename Loaded file name
 * @param[in] options Loaded image format
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 *  @note The number of data bits in the buffer must be consistent with the number of data bits in the file.
 *  @see ItkBufferSave
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferLoad(ITKBUFFER hBuffer, const char* filename, uint32_t options);

/**
\~chinese 
 * @brief 缓冲区图像格式转换
 * @param[in] hSrc 源缓冲区句柄
 * @param[out] hDst 目的缓冲区句柄
 * @param[in] dstFormat 目的缓冲区图像格式
 * @param[in] option 转换选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 转换后的目的缓冲区大小需要符合实际情况。
 * @see ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT ITKBUFFER_VAL_CONVERT_OPTION_STRETCH ItkBufferNeedAutoConvert
*/
 /**
\~english 
 * @brief Buffer image format conversion
 * @param[in] hSrc Source buffer handle
 * @param[out] hDst Destination buffer handle
 * @param[in] dstFormat Destination buffer image format
 * @param[in] option Convert option
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note The size of the destination buffer after conversion needs to match the actual situation.
 * @see ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT ITKBUFFER_VAL_CONVERT_OPTION_STRETCH ItkBufferNeedAutoConvert
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferConvert(ITKBUFFER hSrc, ITKBUFFER hDst, uint32_t dstFormat, int option);

/**
\~chinese 
 * @brief 指示buffer的当前格式是否需要使用自动转换
 * @param[in] hBuf 源缓冲区句柄
 * @param[out] need_auto_convert 返回buf的格式是否需要使用自动转换
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 如果buffer的格式是可以通过 ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT 返回其他格式的, 则会返回true.
 * @see ItkBufferConvert ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT ITKBUFFER_VAL_CONVERT_OPTION_STRETCH 
*/
 /**
\~english 
 * @brief Indicate buffer format if need auto convert
 * @param[in] hBuf Source buffer handle
 * @param[out] need_auto_convert Indicate buffer format if need auto convert
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note If buffer can be converted by ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT, return true.
 * @see ItkBufferConvert ITKBUFFER_VAL_CONVERT_OPTION_AUTO_FORMAT ITKBUFFER_VAL_CONVERT_OPTION_STRETCH 
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferNeedAutoConvert(ITKBUFFER hBuf, uint8_t * need_auto_convert);

/**
\~chinese 
 * @brief 获取64位整数类型块数据特征的值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 64位整数类型相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of chunk data feature of 64bit integer type
 * @param[in] hBuffer Buffer handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Value of 64bit integer type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGetChunkInt64(ITKBUFFER hBuffer, const char* featureName, int64_t* pValue);


/**
\~chinese 
 * @brief 获取布尔类型块数据特征的值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 布尔类型相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of chunk data feature of boolean type
 * @param[in] hBuffer Buffer handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Value of boolean type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGetChunkBool(ITKBUFFER hBuffer, const char* featureName, _Bool* pValue);

/**
\~chinese 
 * @brief 获取浮点数类型块数据特征的值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 浮点数类型相机特征值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of chunk data feature of double type
 * @param[in] hBuffer Buffer handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue Value of double type
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGetChunkDouble(ITKBUFFER hBuffer, const char* featureName, double* pValue);

/**
\~chinese 
 * @brief 获取字符串或枚举类型块数据特征的值
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] featureName 相机特征名称
 * @param[out] pValue 字符串或枚举类型相机特征值
 * @param[out] pLen 作为输入指明特征值的最大长度，作为输出指明特征值的有效长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
*/
 /**
\~english 
 * @brief Get the value of chunk data feature of string or enumeration type
 * @param[in] hBuffer Buffer handle
 * @param[in] featureName Camera feature name
 * @param[out] pValue String or enumeration type camera feature value
 * @param[out] pLen As input, it indicates the maximum length of feature value. As output, it indicates the effective length of feature value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_NOT_IMPLEMENT
 * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
 * @see ItkDevGetAccessMode ItkDevGetType
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferToChunkString(ITKBUFFER hBuffer, const char* featureName, char* pValue,uint32_t* pLen);

/**
\~chinese
 *  @brief  自适应高斯降噪算法
 *  @param[in]  hSrc        源缓冲区句柄
 *  @param[out] hDst        目的缓冲区句柄,不可与源缓冲区句柄相同
 *  @param[in]  isBayerImg  源图像为Bayer图像时为真
 *  @param[in]  sigma       高斯sigma值,推荐范围 0~10；
 *  @param[in]  threshold   平坦阈值 范围[0,INT_MAX) 推荐15~25
 *  @param[in]  option      预留选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 缓冲区数据格式仅支持Mono8、Mono10、Mono12;Bayer8、Bayer10、Bayer12。
*/
/**
\~english
 *  @brief  Adaptive Gaussian filtering 
 *  @param[in]  hSrc        Source buffer to denoise
 *  @param[out] hDst        Destination buffer, dst could not be the same as src
 *  @param[in]  isBayerImg  It is true when the source image is a Bayer image
 *  @param[in]  sigma       Gaussian sigma value
 *  @param[in]  threshold   Flat area threshold [0,INT_MAX),15~25 is recommended when hSrc is 8 bit image
 *  @param[in]  option      Leave a reservation
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note Buffer data format only supports Mono8、Mono10、Mono12,and Bayer8、Bayer10、Bayer12.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGaussianDenoise(ITKBUFFER pSrc, ITKBUFFER pDst, 
                                bool is_Bayer, float sigma, int threshold,int option);

/**
\~chinese 
 *  @brief  图像降噪算法
 *  @param[in]  hSrc  源缓冲区句柄
 *  @param[out] hDst  目的缓冲区句柄，可与源缓冲区句柄相同
 *  @param[in]  threshold  阈值1~64
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 缓冲区数据格式仅支持非packed的Mono和RGB/BGR。
*/
 /**
\~english 
 *  @brief  Denoise a image by GIMP algothrim
 *  @param[in]  hSrc  Source buffer to denoise
 *  @param[out] hDst  Destination buffer, dst could be the same as src
 *  @param[in]  threshold  a valid threshold from 1 to 64
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note Buffer data format only supports non-packed Mono and RGB/BGR.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferDenoise(ITKBUFFER hSrc, ITKBUFFER hDst, int threshold);

/**
\~chinese
 *  @brief  图像双阈值中值滤波算法
 *  @param[in]  hSrc  源缓冲区句柄
 *  @param[out] hDst  目的缓冲区句柄，不可与源缓冲区句柄相同
 *  @param[in]  size  窗口大小，取值：{3,5}，视情况而定
 *  @param[in]  threshold  阈值（低）[0,INT_MAX - 8)，视情况而定，一般推荐10~35
 *  @param[in]  isBayerImg  源图像为Bayer图像时为真
 *  @param[in]  option  预留选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 缓冲区数据格式仅支持Mono8、Mono10、Mono12;Bayer8、Bayer10、Bayer12。
*/
/**
\~english
 *  @brief  double threshold median blur
 *  @param[in]  hSrc  Source buffer to denoise
 *  @param[out] hDst  Destination buffer, dst could not be the same as src
 *  @param[in]  size  Window size，must be 3 or 5
 *  @param[in]  threshold   A low threshold , value threshold [0,INT_MAX - 8),10~35 is recommended when hSrc is 8bit image
 *  @param[in]  isBayerImg  It is true when the source image is a Bayer image
 *  @param[in]  option      Leave a reservation
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note Buffer data format only supports Mono8、Mono10、Mono12,and Bayer8、Bayer10、Bayer12.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferThreshMedianFilter(ITKBUFFER pSrc, ITKBUFFER pDst, int size, int threshold, bool isBayerImg, int option);

/**
\~chinese 
 * @brief 缓冲区锐化
 * @param[in] hSrc 源缓冲区句柄
 * @param[out] hDst 目的缓冲区句柄
 * @param[in] degree 锐化系数，参数范围0~100
 * @param[in] size   滤波窗口大小，奇数，最大到31.
 * @param[in] option 预留选项
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note 缓冲区数据格式仅支持非packed的Mono和RGB/BGR。
*/
 /**
\~english 
 * @brief Buffer sharpening
 * @param[in]  hSrc   Source buffer handle
 * @param[out] hDst   Destination buffer handle
 * @param[in]  degree Sharpening coefficient, parameter range 0~100
 * @param[in]  size   laplacian kernel size，It must be an odd number, with a maximum value of 31
 * @param[in]  option Reserved
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALID_ARG
 * @note Buffer data format only supports non-packed Mono and RGB/BGR.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferSharpen(ITKBUFFER hSrc, ITKBUFFER hDst, int degree, int option);

/**
\~chinese
*  @brief  合成像素位移图像
*  @param[in]  hBufferArray  输入的像素位移图像数组
*  @param[in]  hImageArrayCount  像素位移图像数量, 必须是4、9或16
*  @param[out] hDst  输出的高分辨率图像
*  @param[in]  option   保留
*  @return
*  - ITKSTATUS_OK
*  - ITKSTATUS_INVALID_HANDLE
*  - ITKSTATUS_INVALID_ARG
*  @note 将4/9/16个像素位移图像合成一张更高分辨率的图像, 输入图像的格式必须是Mono或者BayerRG。支持细节如下：
* 1. 输入4张像素位移图像情况下，输入格式支持Mono、BayerRG格式。
*           如果输入格式为Mono，输出格式为Mono，输出宽、高分别为输入的两倍。
*           如果输入格式为BayerRG，输出格式为BGR，输出宽、高和输入同样。
* 2. 输入9张像素位移图像情况下，输入格式只支持Mono格式，输出宽、高分别为输入的三倍。
* 3. 输入16张像素位移图像情况下，输入格式只支持BayerRG格式，输出格式为BGR，输出宽、高分别为输入的两倍。
* 4. 在生成BGR图像的情况下,  图像的第一行和第一列由于原始图像没有对应数据, 所以是缺失的, 为正常现象.  
*/
 /**
\~english
*  @brief  Combine pixel shift images to a higer resolution image
*  @param[in]  hBufferArray pixel shift images array
*  @param[in]  hImageArrayCount pixel shift images array, it must be 4，9 or 16
*  @param[out] hDst the image combined by input images
*  @param[in]  option reserved
*  @return
*  - ITKSTATUS_OK
*  - ITKSTATUS_INVALID_HANDLE
*  - ITKSTATUS_INVALID_ARG
*  @note Combine four pixel-shifted images from 4/9/16 to create a higher resolution image. The input image format must be Mono or BayerRG. Details supported are as follows: 
*   1. When inputting four pixel-shifted images, the input format supports Mono and BayerRG formats.
*       If the input format is Mono, the output format is also Mono, with the output width and height being twice that of the input. 
*       If the input format is BayerRG, the output format is BGR, with the same width and height as the input.
*   2. When inputting nine pixel-shifted images, the input format only supports Mono, with the output width and height being three times that of the input.
*   3. When inputting sixteen pixel-shifted images, the input format only supports BayerRG, with the output format being BGR, and the output width and height being twice that of the input.
*   4. When generating a BGR image, the first row and the first column are missing due to the lack of corresponding data in the original image, which is normal and not a problem.
*/
  IKAPC_API ITKSTATUS IKAPC_CC
    ItkBufferPixelShiftCombine(const ITKBUFFER* hBufferArray, uint32_t hImageArrayCount, ITKBUFFER hDst, uint32_t options);

/**
\~chinese 
 * @brief 新建视图窗口
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] hParentWnd 父窗口句柄
 * @param[out] hView 视图窗口句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note hParentWnd指向父窗口句柄，当父窗口句柄不为空时，新建窗口共享父窗口消息循环队列；当父窗口句柄为空时，新建窗口会新建消息循环队列。
 * @see ItkViewFree
*/
 /**
\~english 
 * @brief Create view window
 * @param[in] hBuffer Buffer handle
 * @param[in] hParentWnd Parent window handle
 * @param[out] hView View window handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note hParentWnd points to the handle of the parent window. When the handle of the parent window is not empty, 
 a new window shares the message loop queue of the parent window. When the handle of the parent window is empty, creating a new window will create a new message loop queue.
 * @see ItkViewFree
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewNew(ITKBUFFER hBuffer, const void* hParentWnd, ITKVIEW* hView);

/**
\~chinese 
 * @brief 释放视图窗口
 * @param[in] hView 视图窗口句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当用户点击关闭按钮关闭视图窗口后，仍然需要调用本函数释放在建立图像窗口中申请的资源。
 * @see ItkViewNew
*/
 /**
\~english 
 * @brief Release view window
 * @param[in] hView View window handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note When users click the close button to close the view window, calling this function to release the resources requested in the image window creation.
 * @see ItkViewNew
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewFree(ITKVIEW hView);

/**
\~chinese 
 * @brief 视图窗口添加缓冲区
 * @param[in] hView 视图窗口句柄
 * @param[in] hBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 添加的缓冲区宽高和图像格式必须和视图窗口宽高和图像格式保持一致。
 * 2. 每个视图窗口中都会维护独立的缓冲区链表，每次调用 ItkViewShow 或 ItkViewShowNext 时，视图窗口都会从缓冲区链表取出一个或者多个缓冲区进行显示。
 * 3. 视图窗口不会多次添加同一个数据缓冲区。
 * @see ItkViewRemoveBuffer
*/
 /**
\~english 
 * @brief Add buffer to view window
 * @param[in] hView View window handle
 * @param[in] hBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. The width, height, and image format of the added buffer must be consistent with the width, height, and image format of the view window.
 * 2. Each view window maintains an independent buffer list, and each time ItkViewShow or ItkViewShowNext is called, 
 the view window retrieves one or more buffers from the buffer list for display.
 * 3. The view window will not add the same data buffer multiple times.
 * @see ItkViewRemoveBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewAddBuffer(ITKVIEW hView, ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 视图窗口移除缓冲区
 * @param[in] hView 视图窗口句柄
 * @param[in] hBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 视图窗口应该至少保留一个缓冲区，否则无法显示任何图像数据。
 * @see ItkViewAddBuffer
*/
 /**
\~english 
 * @brief Remove buffer from view window
 * @param[in] hView View window handle
 * @param[in] hBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note The view window should reserve at least one buffer, otherwise no image data can be displayed.
 * @see ItkViewAddBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewRemoveBuffer(ITKVIEW hView, ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 视图窗口移除所有缓冲区
 * @param[in] hView 视图窗口句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 视图窗口应该至少保留一个缓冲区，否则无法显示任何图像数据。
 * @see ItkViewAddBuffer
*/
 /**
\~english 
 * @brief Remove all buffers from view window
 * @param[in] hView View window handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note The view window should reserve at least one buffer, otherwise no image data can be displayed.
 * @see ItkViewAddBuffer
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewRemoveAllBuffer(ITKVIEW hView);

/**
\~chinese 
 * @brief 设置视图窗口参数
 * @param[in] hView 视图窗口句柄
 * @param[in] prm 参数ID
 * @param[in] value 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkViewGetPrm
*/
 /**
\~english 
 * @brief Set view window parameter
 * @param[in] hView View window handle
 * @param[in] prm Parameter ID
 * @param[in] value Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkViewGetPrm
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewSetPrm(ITKVIEW hView, uint32_t prm, const void* value);

/**
\~chinese 
 * @brief 获取视图窗口参数
 * @param[in] hView 视图窗口句柄
 * @param[in] prm 参数ID
 * @param[out] value 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkViewSetPrm
*/
 /**
\~english 
 * @brief Get view window parameter
 * @param[in] hView View window handle
 * @param[in] prm Parameter ID
 * @param[out] value Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkViewSetPrm
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewGetPrm(ITKVIEW hView, uint32_t prm, void* value);

/**
\~chinese 
 * @brief 隐藏视图窗口
 * @param[in] hView 视图窗口句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkViewShow
*/
 /**
\~english 
 * @brief Hide view window
 * @param[in] hView View window handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkViewShow
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewHide(ITKVIEW hView);

/**
\~chinese 
 * @brief 显示所有采集完毕的缓冲区数据
 * @param[in] hView 视图窗口句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. 本函数在不影响图像数据传输的情况下显示尽可能多的图像数据，如果显示图像的操作过于耗时，可能会跳过部分数据图像的显示过程。
 * 2. 当缓冲区为满时才会显示数据。
 * @see ItkViewHide
*/
 /**
\~english 
 * @brief Display all grabbed buffer data
 * @param[in] hView View window handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 
 * 1. This function displays as much image data as possible without affecting the transmission of image data. 
 If the operation of displaying images is too time-consuming, it may skip the display process of some data images.
 * 2. Data will only be displayed when the buffer is full.
 * @see ItkViewHide
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewShow(ITKVIEW hView);

/**
\~chinese 
 * @brief 显示下一帧缓冲区数据
 * @param[in] hView 视图窗口句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkViewHide
*/
 /**
\~english 
 * @brief Display the next frame of buffer data
 * @param[in] hView View window handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkViewHide
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewShowNext(ITKVIEW hView);

/**
\~chinese 
 * @brief 显示某个缓冲区数据
 * @param[in] hView 视图窗口句柄
 * @param[in] hBuffer 缓冲区句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkViewHide
*/
 /**
\~english 
 * @brief Display the image data of some buffer
 * @param[in] hView View window handle
 * @param[in] hBuffer Buffer handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkViewHide
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkViewShowBuffer(ITKVIEW hView, ITKBUFFER hBuffer);

/**
\~chinese 
 * @brief 打开相机中的文件
 * @param[in] hDev 相机设备句柄
 * @param[in] path 打开或创建的文件名
 * @param[in] mode 设备文件打开模式
 * @param[out] fp 文件句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note 
 * 1. 打开的文件应该在操作结束后使用 ItkFileClose 关闭。
 * 2. 如果在断电前对文件进行了更改而没有关闭，则可以删除或重新安装文件。
 * 3. 禁止重复打开具有任何写模式标志的文件。
 * @see ItkFileClose
*/
 /**
\~english 
 * @brief Open files in the camera
 * @param[in] hDev Camera device handle
 * @param[in] path File name opened or created
 * @param[in] mode Device file opening mode
 * @param[out] fp File handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note 
 * 1. The opened file should be closed using ItkFileClose after the operation is completed.
 * 2. If changes are made to the file before the power outage and it is not closed, the file can be deleted or reinstalled.
 * 3. Do not repeatedly open files with any write mode flags.
 * @see ItkFileClose
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileOpen(ITKDEVICE hDev, const char* path, char mode, ITKFILE* fp);

/**
\~chinese 
 * @brief 关闭相机中的文件
 * @param[in] fp 文件句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 如果文件已更改，则将文件的缓存信息写回。
 * @see ItkFileOpen
*/
 /**
\~english 
 * @brief Close files in the camera
 * @param[in] fp File handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note If the file has been changed, write back the cached information of the file.
 * @see ItkFileOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileClose(ITKFILE fp);

/**
\~chinese 
 * @brief 读取设备文件数据
 * @param[in] fp 文件句柄
 * @param[out] buf 存储读取数据的用户缓冲区
 * @param[in] btr 缓冲区长度
 * @param[out] br 实际读取的字节数
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. 本函数从相机文件指针位置读取数据到用户缓冲区。读取成功后，文件指针会向前移动*br字节。
 * 2. 每次打开文件后，文件指针都会复位到0。
 * 3. 每次读取完成后，都应检测*br参数以验证是否到达文件末尾。若*br < btr，则表示当前文件指针已经到达文件末尾。
 * @see ItkFileWrite
*/
 /**
\~english 
 * @brief Read device file data
 * @param[in] fp File handle
 * @param[out] buf User buffer for storing read data
 * @param[in] btr Buffer length
 * @param[out] br Actual number of bytes read
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. This function reads data from the camera file pointer position to the user buffer. After successful reading, the file pointer will move forward by *br bytes.
 * 2. After each file is opened, the file pointer is reset to 0.
 * 3. After each reading is completed, the *br parameter should be checked to verify if it has reached the end of the file. 
 If *br<btr, it means that the current file pointer has reached the end of the file.
 * @see ItkFileWrite
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileRead(ITKFILE fp, void* buf, unsigned int btr, unsigned int* br);

/**
\~chinese 
 * @brief 写入设备文件数据
 * @param[in] fp 文件句柄
 * @param[in] buf 存储写入数据的用户缓冲区
 * @param[in] btw 缓冲区长度
 * @param[out] bw 实际写入的字节数
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. 本函数向相机文件指针位置写入用户缓冲区数据。写入成功后，文件指针会向前移动*bw字节。
 * 2. 每次打开文件后，文件指针都会复位到0。
 * 3. 每次写入完成后，都应检测*bw参数以验证文件是否已经耗尽所有剩余存储空间。若*bw < btw，则表示当前文件系统已无更多空间来存储数据。
 * @see ItkFileRead
*/
 /**
\~english 
 * @brief Write device file data
 * @param[in] fp File handle
 * @param[in] buf User buffer for storing written data
 * @param[in] btw Buffer length
 * @param[out] bw Actual number of bytes written
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. This function writes user buffer data to the camera file pointer position. After successful writing, the file pointer will move forward by *bw bytes.
 * 2. After each file is opened, the file pointer is reset to 0.
 * 3. After each write is completed, the *bw parameter should be checked to verify if the file has exhausted all remaining storage space. 
 If *bw<btw, it means that the current file system has no more space to store data.
 * @see ItkFileRead
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileWrite(ITKFILE fp, const void* buf, unsigned int btw, unsigned int* bw);

/**
\~chinese 
 * @brief 获取文件存取的进度
 * @param[in] fp 文件句柄
 * @param[out] nCompleted 已完成的长度
 * @param[out] nTotal 总长度
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkFileRead ItkFileWrite
*/
 /**
\~english 
 * @brief Get the progress value of file read/write operation
 * @param[in] fp File handle
 * @param[out] nCompleted Completed Length
 * @param[out] nTotal Total Length
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkFileRead ItkFileWrite
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileGetFileAccessProgress(ITKFILE fp, int64_t* nCompleted, int64_t* nTotal);

/**
\~chinese 
 * @brief 设置设备文件偏移量
 * @param[in] fp 文件句柄
 * @param[in] ofs 偏移量
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 文件指针位置会作为写入文件和读取文件的起始地址，每次成功读取或写入文件都会移动指针。使用 ItkFileLSeek 可以移动指针位置。
*/
 /**
\~english 
 * @brief Set device file offset
 * @param[in] fp File handle
 * @param[in] ofs Offset
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note The file pointer position will serve as the starting address for writing and reading files, 
 and the pointer will be moved every time a file is successfully read or written. ItkFileLSeek allows you to move the pointer position.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileLSeek(ITKFILE fp, unsigned ofs);

/**
\~chinese 
 * @brief 获取设备文件大小
 * @param[in] fp 文件句柄
 * @param[out] size 文件尺寸
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
*/
 /**
\~english 
 * @brief Get device file size
 * @param[in] fp File handle
 * @param[out] size File size
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileSize(ITKFILE fp, unsigned* size);

/**
\~chinese 
 * @brief 刷新写入设备文件的缓冲信息
 * @param[in] fp 文件句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当需要长时间打开文件并进行写入操作时，建议周期性调用本函数来刷新文件缓存信息，避免由于突然断电导致数据丢失。
*/
 /**
\~english 
 * @brief Refresh the cache information for writing device files
 * @param[in] fp File handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note When it is necessary to open a file for a long time and perform write operations, 
 it is recommended to periodically call this function to refresh the file cache information to avoid data loss due to sudden power outages.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileSync(ITKFILE fp);

/**
\~chinese 
 * @brief 获取设备文件位置相对于文件首的偏移字节数
 * @param[in] fp 文件句柄
 * @param[out] ofs 偏移字节数
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkFileLSeek
*/
 /**
\~english 
 * @brief Get the offset byte count of the device file location relative to the file header
 * @param[in] fp File handle
 * @param[out] ofs Offset byte count
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkFileLSeek
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileTell(ITKFILE fp, unsigned* ofs);

/**
\~chinese 
 * @brief 检测设备文件结束符
 * @param[in] fp 文件句柄
 * @param[out] eof 结束符
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 若当前文件指针已经到达末尾，则ofs值为1，否则ofs值为0。
 * @see ItkFileLSeek
*/
 /**
\~english 
 * @brief Detecting device file terminator
 * @param[in] fp File handle
 * @param[out] eof End symbol
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note If the current file pointer has reached the end, the ofs value is 1, otherwise the ofs value is 0.
 * @see ItkFileLSeek
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileEof(ITKFILE fp, unsigned* eof);

/**
\~chinese 
 * @brief 打开设备文件夹
 * @param[in] hDev 相机设备句柄
 * @param[in] path 文件夹名称
 * @param[out] dp 指向文件夹对象的句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note 用户可以在文件夹中存储文件以形成树形文件结构。
 * @see ItkDirClose
*/
 /**
\~english 
 * @brief Open device folder
 * @param[in] hDev Camera device handle
 * @param[in] path Folder name
 * @param[out] dp Handle to folder object
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INSUFFICIENT_RESOURCES
 * @note Users can store files in folders to form a tree like file structure.
 * @see ItkDirClose
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDirOpen(ITKDEVICE hDev, const char* path, ITKDIR* dp);

/**
\~chinese 
 * @brief 关闭设备文件夹
 * @param[in] dp 文件夹句柄
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note 当文件夹句柄关闭后，后续所有使用该句柄的API都不再有效。
 * @see ItkDirOpen
*/
 /**
\~english 
 * @brief Close the device folder
 * @param[in] dp Folder handle
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @note After the folder handle is closed, all subsequent API that use the handle are no longer valid.
 * @see ItkDirOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDirClose(ITKDIR dp);

/**
\~chinese 
 * @brief 读取设备文件夹中的项
 * @param[in] dp 文件夹句柄
 * @param[out] fno 指向文件信息结构的指针，用于存储有关已读项的信息 
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note
 * 1. 点条目（“”和“..”)在子目录中会被过滤掉，不会出现在读取项中。
 * 2. 当所有文件都已被读取后，将存储一个空字符串到fno->fname[]中。
 * @see ItkDirOpen
*/
 /**
\~english 
 * @brief Read items from the device folder
 * @param[in] dp Folder handle
 * @param[out] fno Pointer to the file information structure, used to store information about read items
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note
 * 1. Point entries ("" and "..") will be filtered out in subdirectories and will not appear in read entries.
 * 2. After all files have been read, an empty string will be stored in fno->fname[].
 * @see ItkDirOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkDirRead(ITKDIR dp, ITKFILE_INFO* fno);

/**
\~chinese 
 * @brief 检测文件或目录是否存在
 * @param[in] hDev 相机设备句柄
 * @param[in] path 文件名称
 * @param[out] fno 指向文件信息结构的指针，存储文件信息
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. 如果文件不存在，则返回 FR_NO_FILE 。
 * 2. 如果文件存在，则返回FR_OK和存储到文件信息结构中的文件信息。
 * @see ItkFileOpen
*/
 /**
\~english 
 * @brief Check if the file or directory exists
 * @param[in] hDev Camera device handle
 * @param[in] path File name
 * @param[out] fno Pointer to the file information structure, storing file information
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. If the file does not exist, return FR_NO_FILE .
 * 2. If the file exists, return the FR_OK and the file information stored in the file information structure.
 * @see ItkFileOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileStat(ITKDEVICE hDev, const char* path, ITKFILE_INFO* fno);

/**
\~chinese 
 * @brief 删除文件或目录
 * @param[in] hDev 相机设备句柄
 * @param[in] path 文件名称
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. 如果文件或目录的访问模式为只读模式，则无法被删除。
 * 2. 子目录必须为空，且不能是当前目录。
 * 3. 文件或目录必须未被打开。
 * @see ItkFileOpen
*/
 /**
\~english 
 * @brief Delete files or directories
 * @param[in] hDev Camera device handle
 * @param[in] path File name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 
 * 1. If the access mode of a file or directory is read-only, it cannot be deleted.
 * 2. The subdirectories must be empty and cannot be the current directory.
 * 3. The file or directory must not be opened.
 * @see ItkFileOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileUnlink(ITKDEVICE hDev, const char* path);

/**
\~chinese 
 * @brief 文件或子目录重命名
 * @param[in] hDev 相机设备句柄
 * @param[in] old_name 文件或目录原名称
 * @param[in] new_name 文件或目录重命名名称
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note 要重命名的文件或目录必须未被打开。
 * @see ItkFileOpen
*/
 /**
\~english 
 * @brief Rename files or subdirectories
 * @param[in] hDev Camera device handle
 * @param[in] old_name Original name of file or directory
 * @param[in] new_name Rename file or directory name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @note The file or directory to be renamed must not be opened.
 * @see ItkFileOpen
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileRename(ITKDEVICE hDev, const char* old_name, const char* new_name);

/**
\~chinese 
 * @brief 创建新目录
 * @param[in] hDev 相机设备句柄
 * @param[in] path 创建目录名
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkFileUnlink
*/
 /**
\~english 
 * @brief Create a new directory
 * @param[in] hDev Camera device handle
 * @param[in] path Created directory name
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * - ITKSTATUS_INVALD_ARG
 * @see ItkFileUnlink
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileMkdir(ITKDEVICE hDev, const char* path);

/**
\~chinese 
 * @brief 获取设备的空余空间
 * @param[in] hDev 相机设备句柄
 * @param[out] nbfree 空余空间，单位字节
 * @param[out] nbtotal 设备总空间，单位字节
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkFileUnlink
*/
 /**
\~english 
 * @brief Get free space on the device
 * @param[in] hDev Camera device handle
 * @param[out] nbfree Free space, unit byte
 * @param[out] nbtotal Total device space, in bytes
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkFileUnlink
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkFileGetFree(ITKDEVICE hDev, unsigned int* nbfree, unsigned int* nbtotal);

#ifdef __cplusplus
}
#    ifdef IKAPC_BOOL_DEFINED
#        undef _Bool
#        undef IKAPC_BOOL_DEFINED
#    endif
#endif 

#endif
// clang-format on
