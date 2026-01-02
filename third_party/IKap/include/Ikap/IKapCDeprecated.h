// clang-format off
/**
\~chinese
 * @file IKapCDeprecated.h
 * @brief IKapC库头文件(已抛弃接口)
*/
/**
\~english
 * @file IKapCDeprecated.h
 * @brief IKapC library header(Deprecated API)
 */

#ifndef _IKAPCDEPRECATED_H
#define _IKAPCDEPRECATED_H

#include "IKapCDef.h"

#ifdef __cplusplus
 // avoid namespace ambiguities between std::_Bool (from yvals.h) and ::_Bool (from IKapCTypes.h)
#    if defined(_MSC_VER)
#        define _Bool ::_Bool	///< @brief \~chinese 布尔类型 \~english Boolean type
#        define IKAPC_BOOL_DEFINED
#    endif

/*Deprecated*/


/**
\~chinese
 * @brief 缓冲区图像深度【RO】
 * @note 【弃用】可能在后续版本中移除，请使用ITKBUFFER_PRM_PIXEL_BITS代替。
 * 1. 本参数无法设置，数值由缓冲区像素格式 ITKBUFFER_PRM_FORMAT 决定。
*/
/**
\~english
 * @brief Buffer image depth[RO]
 * @note [Deprecated] May be removed in future versions, please use ITKBUFFER_PRM_IMAGE_BITS instead.
 * 1. This parameter cannot be set, the value is determined by buffer pixel format ITKBUFFER_PRM_FORMAT .
 */
#define ITKBUFFER_PRM_DATA_BIT ITKBUFFER_PRM(1, 4)

 /**
 \~chinese
  * @brief 缓冲区通道深度【RO】
  * @note 【弃用】可能在后续版本中移除，请使用ITKBUFFER_PRM_CHANNEL_BITS代替。
  * 1. 本参数无法设置，数值由缓冲区像素格式 ITKBUFFER_PRM_FORMAT 决定。
 */
 /**
\~english
 * @brief Buffer channel depth[RO]
 * @note [Deprecated] May be removed in future versions, please use ITKBUFFER_PRM_CHANNEL_BITS instead.
 * 1. This parameter cannot be set, the value is determined by buffer pixel format ITKBUFFER_PRM_FORMAT .
 */
#define ITKBUFFER_PRM_PIXEL_DEPTH ITKBUFFER_PRM(2, 4)

#define ITKBUFFER_FORMATL_DATA_BIT(format) (((format) >> 8) & 0xFF)

 /**
 \~chinese
 * @brief 单个像素深度
 * @deprecated 【弃用】可能在后续版本中移除，请使用ITKBUFFER_FORMAT_PIXEL_BITS代替
 * @see ITKBUFFER_FORMAT
 */
 /**
 \~english
 * @brief Single pixel size
 * @deprecated [Deprecated] May be removed in future versions, please use ITKBUFFER_FORMAT_PIXEL_BITS instead.
 * @see ITKBUFFER_FORMAT
 */
#define ITKBUFFER_FORMAT_DATA_BIT(format) (((format) >> 8) & 0xFF)

 /**
 \~chinese
 * @brief 单个通道深度
 * @deprecated 【弃用】可能在后续版本中移除，请使用ITKBUFFER_FORMAT_CHANNEL_BITS代替
 * @see ITKBUFFER_FORMAT (nDepth, nBits, index)
 */
 /**
 \~english
 * @brief Single channel size
 * @deprecated [Deprecated] May be removed in future versions, please use ITKBUFFER_FORMAT_CHANNEL_BITS instead.
 * @see ITKBUFFER_FORMAT (nDepth, nBits, index)
 */
#define ITKBUFFER_FORMAT_PIXEL_DEPTH(format) (((format) >> 16) & 0xFF)

 /**
 \~chinese
  * @brief 当前缓冲区句柄
  * @note 本参数表示当前缓冲区句柄【RO】。
 */
 /**
 \~english
  * @brief Handle of current buffer
  * @note This parameter indicates handle of current buffer [RO].
  */
#define ITKSTREAM_PRM_CURRENT_BUFFER_HANDLE ITKSTREAM_PRM(16, 8)
extern "C" {
#endif  // __cplusplus

    /**
\~chinese
 * @brief 获取当前连接到PC上采集卡设备的数量【已弃用】
 * @param[out] pCount 当前连接到PC上的采集卡的数量
 * @return
 * - ITKSTATUS_OK
*/
/**
\~english
 * @brief Get the current number of frame grabber devices connected to PC [abandoned]
 * @param[out] pCount The current number of frame grabber connected to PC
 * @return
 * - ITKSTATUS_OK
 */
    IKAPC_API ITKSTATUS IKAPC_CC ItkManGetBoardCount(uint32_t* pCount);

    /**
    \~chinese
     * @brief 获取采集卡设备信息【已弃用】
     * @param[in] index 采集卡索引
     * @param[out] pBI 采集卡信息
     * @return
     * - ITKSTATUS_OK
     * @note 在调用本函数前必须先调用 ItkManGetBoardCount 。
     * @see ItkManGetBoardCount
    */
    /**
   \~english
    * @brief Get frame grabber device information [abandoned]
    * @param[in] index Frame grabber index
    * @param[out] pBI Frame grabber information
    * @return
    * - ITKSTATUS_OK
    * @note Before calling this function, ItkManGetBoardCount must be called first.
    * @see ItkManGetBoardCount
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkManGetBoardInfo(uint32_t index, PITKBOARD_INFO pBI);

    /**
    \~chinese
     * @brief 申请配置相机特征需要的资源
     * @param[in] hDev 相机设备句柄
     * @param[in] name 特征名称
     * @param[out] phFeature 相机特征句柄
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征的所有信息都存储在ITKFEATURE句柄中，包括特征名称、特征类型、参数范围等。
     * @see ItkDevGetFeatureName ItkDevFreeFeature
    */
    /**
   \~english
    * @brief Request for resources required to configure camera feature
    * @param[in] hDev Camera device handle
    * @param[in] name Feature name
    * @param[out] phFeature Camera feature handle
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_HANDLE
    * @note All information about the features is stored in ITKFEATURE handle, including feature name, feature type, parameter range, etc.
    * @see ItkDevGetFeatureName ItkDevFreeFeature
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevAllocFeature(ITKDEVICE hDev, const char* name, ITKFEATURE* phFeature);



    /**
    \~chinese
     * @brief 释放配置相机特征资源
     * @param[in] hFeature 相机特征句柄
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_HANDLE
     * @note 释放句柄后，无法再次通过该句柄配置相机参数。
     * @see ItkDevAllocFeature
    */
    /**
   \~english
    * @brief Release configuration camera feature resources
    * @param[in] hFeature Camera feature handle
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_HANDLE
    * @note After releasing the handle, camera parameters cannot be configured again through the handle.
    * @see ItkDevAllocFeature
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevFreeFeature(ITKFEATURE hFeature);

    /**
    \~chinese
     * @brief 申请数据流采集过程中需要的资源
     * @param[in] hDev 相机设备句柄
     * @param[in] index 数据流索引，有效范围[0, ItkDevGetStreamCount -1]
     * @param[in] hBuffer 缓冲区句柄，默认的数据流目标
     * @param[out] phStream 数据流句柄，控制数据流整个生命周期
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note
     * 1. 对于同一个相机，可能存在多个数据流同时采集数据，通过索引来指明希望采集的数据流。
     * 2. 收集数据流资源的过程中必须指定一个数据缓冲区，用来存放采集过程中的图像数据。
     * @see ItkDevFreeStream
    */
    /**
   \~english
    * @brief Request for resources during the process of applying for data stream acquisition
    * @param[in] hDev Camera device handle
    * @param[in] index Data stream index, valid range is [0, ItkDevGetStreamCount -1]
    * @param[in] hBuffer Buffer handle, default data stream destination
    * @param[out] phStream Data stream handle, controlling the entire life cycle of data stream
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note
    * 1. For the same camera, there may be multiple data streams grab data simultaneously. Use an index to indicate the desired data stream to grab.
    * 2. During the process of grabbing data stream resources, a data buffer must be specified to store the image data grabbed during acquisition process.
    * @see ItkDevFreeStream
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevAllocStream(ITKDEVICE hDev, uint32_t index, ITKBUFFER hBuffer, ITKSTREAM* phStream);

    /**
    \~chinese
     * @brief 从串口读取数据
     * @param[in] hDev 相机设备句柄
     * @param[out] pBuffer 存放数据的缓冲区
     * @param[out] length 缓冲区长度
     * @param[in] timeout 读取超时时间
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 从串口读取的数据将被写入缓冲区，请确保缓冲区大小足够。
     * @see ItkDevSerialPortWrite
    */
    /**
   \~english
    * @brief Read data from serial port
    * @param[in] hDev Camera device handle
    * @param[out] pBuffer Buffer for storing data
    * @param[out] length Register address
    * @param[in] timeout Read timeout time
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The data read from serial port will be written to the buffer, please ensure that the buffer size is sufficient.
    * @see ItkDevSerialPortWrite
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevSerialPortRead(ITKDEVICE hDev, void* pBuffer, uint32_t* length, uint32_t timeout);

    /**
    \~chinese
     * @brief 向串口写入数据
     * @param[in] hDev 相机设备句柄
     * @param[in] pBuffer 存放数据的缓冲区
     * @param[in] length 缓冲区长度
     * @param[in] timeout 读取超时时间
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 从缓冲区读取的数据将被写入串口，请确保缓冲区大小适当。
     * @see ItkDevSerialPortRead
    */
    /**
   \~english
    * @brief Write data to serial port
    * @param[in] hDev Camera device handle
    * @param[in] pBuffer Buffer for storing data
    * @param[in] length Register address
    * @param[in] timeout Read timeout time
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The data read from the buffer will be written to serial port, please ensure that the buffer size is sufficient.
    * @see ItkDevSerialPortRead
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevSerialPortWrite(ITKDEVICE hDev, const void* pBuffer, uint32_t length, uint32_t timeout);

    /**
    \~chinese
     * @brief 获取相机特征访问模式
     * @param[in] hFeature 相机特征句柄
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
    * @param[in] hFeature Camera feature handle
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
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetAccessMode(ITKFEATURE hFeature, uint32_t* pAccessMode);

    /**
    \~chinese
     * @brief 获取相机特征类别
     * @param[in] hFeature 相机特征句柄
     * @param[out] pCategory 相机特征所属类别
     * @param[in,out] pBufferSize 作为输入指明特征类别的最大长度，作为输出指明特征类别的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
    */
    /**
   \~english
    * @brief Get camera feature category
    * @param[in] hFeature Camera feature handle
    * @param[out] pCategory Category of camera feature
    * @param[in,out] pBufferSize As input, it indicates the maximum length of feature category. As output, it indicates the effective length of feature category
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetCategory(ITKFEATURE hFeature, char* pCategory, uint32_t* pBufferSize);

    /**
    \~chinese
     * @brief 获取相机特征类别
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pCategory 相机特征所属类别
     * @param[in,out] pBufferSize 作为输入指明特征类别的最大长度，作为输出指明特征类别的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
    */
    /**
   \~english
    * @brief Get camera feature category
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pCategory Category of camera feature
    * @param[in,out] pBufferSize As input, it indicates the maximum length of feature category. As output, it indicates the effective length of feature category
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    */
    IKAPC_API ITKSTATUS IKAPC_CC
        ItkDevGetCategory(ITKDEVICE hDev, const char* featureName, char* pCategory, uint32_t* pBufferSize);

    /**
    \~chinese
     * @brief 获取相机特征数据类型
     * @param[in] hFeature 相机特征句柄
     * @param[out] pType 相机特征数据类型
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征数据类型可选项如下所示：
     * | 数据类型                      |
     * |-------------------------------|
     * | ITKFEATURE_VAL_TYPE_UNDEFINED |
     * | ITKFEATURE_VAL_TYPE_INT32     |
     * | ITKFEATURE_VAL_TYPE_INT64     |
     * | ITKFEATURE_VAL_TYPE_FLOAT     |
     * | ITKFEATURE_VAL_TYPE_DOUBLE    |
     * | ITKFEATURE_VAL_TYPE_BOOL      |
     * | ITKFEATURE_VAL_TYPE_ENUM      |
     * | ITKFEATURE_VAL_TYPE_STRING    |
     * | ITKFEATURE_VAL_TYPE_COMMAND   |
     * | ITKFEATURE_VAL_TYPE_CATEGORY  |
     * | ITKFEATURE_VAL_TYPE_REGISTER  |
    */
    /**
   \~english
    * @brief Get camera feature data type
    * @param[in] hFeature Camera feature handle
    * @param[out] pType Camera feature data type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The available options for feature data types are as follows:
    * | Data type                     |
    * |-------------------------------|
    * | ITKFEATURE_VAL_TYPE_UNDEFINED |
    * | ITKFEATURE_VAL_TYPE_INT32     |
    * | ITKFEATURE_VAL_TYPE_INT64     |
    * | ITKFEATURE_VAL_TYPE_FLOAT     |
    * | ITKFEATURE_VAL_TYPE_DOUBLE    |
    * | ITKFEATURE_VAL_TYPE_BOOL      |
    * | ITKFEATURE_VAL_TYPE_ENUM      |
    * | ITKFEATURE_VAL_TYPE_STRING    |
    * | ITKFEATURE_VAL_TYPE_COMMAND   |
    * | ITKFEATURE_VAL_TYPE_CATEGORY  |
    * | ITKFEATURE_VAL_TYPE_REGISTER  |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetType(ITKFEATURE hFeature, uint32_t* pType);

    /**
    \~chinese
     * @brief 获取相机特征数据类型
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pType 相机特征数据类型
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征数据类型可选项如下所示：
     * | 数据类型                      |
     * |-------------------------------|
     * | ITKFEATURE_VAL_TYPE_UNDEFINED |
     * | ITKFEATURE_VAL_TYPE_INT32     |
     * | ITKFEATURE_VAL_TYPE_INT64     |
     * | ITKFEATURE_VAL_TYPE_FLOAT     |
     * | ITKFEATURE_VAL_TYPE_DOUBLE    |
     * | ITKFEATURE_VAL_TYPE_BOOL      |
     * | ITKFEATURE_VAL_TYPE_ENUM      |
     * | ITKFEATURE_VAL_TYPE_STRING    |
     * | ITKFEATURE_VAL_TYPE_COMMAND   |
     * | ITKFEATURE_VAL_TYPE_CATEGORY  |
     * | ITKFEATURE_VAL_TYPE_REGISTER  |
    */
    /**
   \~english
    * @brief Get camera feature data type
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pType Camera feature data type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The available options for feature data types are as follows:
    * | Data type                     |
    * |-------------------------------|
    * | ITKFEATURE_VAL_TYPE_UNDEFINED |
    * | ITKFEATURE_VAL_TYPE_INT32     |
    * | ITKFEATURE_VAL_TYPE_INT64     |
    * | ITKFEATURE_VAL_TYPE_FLOAT     |
    * | ITKFEATURE_VAL_TYPE_DOUBLE    |
    * | ITKFEATURE_VAL_TYPE_BOOL      |
    * | ITKFEATURE_VAL_TYPE_ENUM      |
    * | ITKFEATURE_VAL_TYPE_STRING    |
    * | ITKFEATURE_VAL_TYPE_COMMAND   |
    * | ITKFEATURE_VAL_TYPE_CATEGORY  |
    * | ITKFEATURE_VAL_TYPE_REGISTER  |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetType(ITKDEVICE hDev, const char* featureName, uint32_t* pType);

    /**
    \~chinese
     * @brief 获取相机特征显示名称
     * @param[in] hFeature 相机特征句柄
     * @param[out] featureName 相机特征显示名称
     * @param[in,out] nameSize 作为输入指明显示名称的最大长度，作为输出指明显示名称的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 相机特征显示名称和名称有所不同，前者用于在GUI界面中显示特征，后者则唯一标示了相机特征。
    */
    /**
   \~english
    * @brief Get camera feature display name
    * @param[in] hFeature Camera feature handle
    * @param[out] featureName Camera feature display name
    * @param[in,out] nameSize As input, it indicates the maximum length of display name. As output, it indicates the effective length of the display name
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The camera feature display name and name are different. The former is used to display feature in GUI interface, while the latter uniquely identifies camera feature.
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDisplayName(ITKFEATURE hFeature, char* featureName, uint32_t* pNameSize);

    /**
    \~chinese
     * @brief 获取相机特征显示名称
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] featureDisplayName 相机特征显示名称
     * @param[in,out] nameSize 作为输入指明显示名称的最大长度，作为输出指明显示名称的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 相机特征显示名称和名称有所不同，前者用于在GUI界面中显示特征，后者则唯一标示了相机特征。
    */
    /**
   \~english
    * @brief Get camera feature display name
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] featureDisplayName Camera feature display name
    * @param[in,out] nameSize As input, it indicates the maximum length of display name. As output, it indicates the effective length of the display name
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The camera feature display name and name are different. The former is used to display feature in GUI interface, while the latter uniquely identifies camera feature.
    */
    IKAPC_API ITKSTATUS IKAPC_CC
        ItkDevGetDisplayName(ITKDEVICE hDev, const char* featureName, char* featureDisplayName, uint32_t* pNameSize);

    /**
    \~chinese
     * @brief 获取相机特征提示信息
     * @param[in] hFeature 相机特征句柄
     * @param[out] pBuff 相机特征提示信息
     * @param[in,out] pBufLen 作为输入指明提示信息的最大长度，作为输出指明提示信息的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征提示信息是一个简要描述节点及其目的的文本字符串，传递给此函数的缓冲区必须足够容纳整个字符串。
     如果调用该函数时，将pBuff设置为NULL，则pBufLen指向的变量将设置为所需的缓冲区大小。
    */
    /**
   \~english
    * @brief Get camera feature tooltip information
    * @param[in] hFeature Camera feature handle
    * @param[out] pBuff Camera feature tooltip information
    * @param[in,out] pBufLen As input, it indicates the maximum length of tooltip information. As output, it indicates the effective length of tooltip information
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The feature tooltip information is a text string that briefly describes the node and its purpose,
    and the buffer passed to this function must be sufficient to accommodate the entire string.
    If pBuff is set to NULL when calling this function, the variable pointed to by pBufLen will be set to the desired buffer size.
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetTooltip(ITKFEATURE hFeature, char* pBuff, uint32_t* pBufLen);

    /**
    \~chinese
     * @brief 获取相机特征提示信息
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pBuff 相机特征提示信息
     * @param[in,out] pBufLen 作为输入指明提示信息的最大长度，作为输出指明提示信息的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征提示信息是一个简要描述节点及其目的的文本字符串，传递给此函数的缓冲区必须足够容纳整个字符串。
     如果调用该函数时，将pBuff设置为NULL，则pBufLen指向的变量将设置为所需的缓冲区大小。
    */
    /**
   \~english
    * @brief Get camera feature tooltip information
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pBuff Camera feature tooltip information
    * @param[in,out] pBufLen As input, it indicates the maximum length of tooltip information. As output, it indicates the effective length of tooltip information
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The feature tooltip information is a text string that briefly describes the node and its purpose,
    and the buffer passed to this function must be sufficient to accommodate the entire string.
    If pBuff is set to NULL when calling this function, the variable pointed to by pBufLen will be set to the desired buffer size.
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetTooltip(ITKDEVICE hDev, const char* featureName, char* pBuff, uint32_t* pBufLen);

    /**
    \~chinese
     * @brief 获取相机特征具体描述信息
     * @param[in] hFeature 相机特征句柄
     * @param[out] pBuff 相机特征详细说明信息
     * @param[in,out] pBufLen 作为输入指明详细说明信息的最大长度，作为输出指明详细说明信息的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征具体描述信息是一个描述节点及其目的的文本字符串，比ItkDevGetTooltip获取的信息更详细，传递给此函数的缓冲区必须足够容纳整个字符串。
     如果调用该函数时，pBuff将设置为NULL，则pBufLen指向的变量将设置为所需的缓冲区大小。
    */
    /**
   \~english
    * @brief Get specific description information of camera feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pBuff Detailed description of camera feature
    * @param[in,out] pBufLen As input, it indicates the maximum length of detailed description information. As output, it indicates the effective length of detailed description information
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The specific feature description information is a text string that describes the node and its purpose,
    which is more detailed than the information obtained by ItkDevGetTooltip. The buffer passed to this function must be sufficient to accommodate the entire string.
    If pBuff is set to NULL when calling this function, the variable pointed to by pBufLen will be set to the desired buffer size.
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDescription(ITKFEATURE hFeature, char* pBuff, uint32_t* pBufLen);

    /**
    \~chinese
     * @brief 获取相机特征具体描述信息
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pBuff 相机特征详细说明信息
     * @param[in,out] pBufLen 作为输入指明详细说明信息的最大长度，作为输出指明详细说明信息的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征具体描述信息是一个描述节点及其目的的文本字符串，比ItkDevGetTooltip获取的信息更详细，
     传递给此函数的缓冲区必须足够容纳整个字符串。如果调用该函数时，pBuff将设置为NULL，则pBufLen指向的变量将设置为所需的缓冲区大小。
    */
    /**
   \~english
    * @brief Get specific description information of camera feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pBuff Detailed description of camera feature
    * @param[in,out] pBufLen As input, it indicates the maximum length of detailed description information. As output, it indicates the effective length of detailed description information
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The specific feature description information is a text string that describes the node and its purpose,
    which is more detailed than the information obtained by ItkDevGetTooltip. The buffer passed to this function must be sufficient to accommodate the entire string.
    If pBuff is set to NULL when calling this function, the variable pointed to by pBufLen will be set to the desired buffer size.
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDescription(ITKDEVICE hDev, const char* featureName, char* pBuff, uint32_t* pBufLen);

    /**
    \~chinese
     * @brief 获取相机特征命名空间
     * @param[in] hFeature 相机特征句柄
     * @param[out] pNameSpace 相机特征命名空间
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征命名空间可选项如下所示：
     * | 命名空间                            |
     * |-------------------------------------|
     * | ITKFEATURE_VAL_NAME_SPACE_UNDEFINED |
     * | ITKFEATURE_VAL_NAME_SPACE_CUSTOM    |
     * | ITKFEATURE_VAL_NAME_SPACE_STANDARD  |
    */
    /**
   \~english
    * @brief Get camera feature name space
    * @param[in] hFeature Camera feature handle
    * @param[out] pNameSpace Camera feature name space
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The feature name space options are as follows:
    * | Name space                          |
    * |-------------------------------------|
    * | ITKFEATURE_VAL_NAME_SPACE_UNDEFINED |
    * | ITKFEATURE_VAL_NAME_SPACE_CUSTOM    |
    * | ITKFEATURE_VAL_NAME_SPACE_STANDARD  |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetNameSpace(ITKFEATURE hFeature, uint32_t* pNameSpace);

    /**
    \~chinese
     * @brief 获取相机特征命名空间
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pNameSpace 相机特征命名空间
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征命名空间可选项如下所示：
     * | 命名空间                            |
     * |-------------------------------------|
     * | ITKFEATURE_VAL_NAME_SPACE_UNDEFINED |
     * | ITKFEATURE_VAL_NAME_SPACE_CUSTOM    |
     * | ITKFEATURE_VAL_NAME_SPACE_STANDARD  |
    */
    /**
   \~english
    * @brief Get camera feature name space
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pNameSpace Camera feature name space
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The feature name space options are as follows:
    * | Name space                          |
    * |-------------------------------------|
    * | ITKFEATURE_VAL_NAME_SPACE_UNDEFINED |
    * | ITKFEATURE_VAL_NAME_SPACE_CUSTOM    |
    * | ITKFEATURE_VAL_NAME_SPACE_STANDARD  |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetNameSpace(ITKDEVICE hDev, const char* featureName, uint32_t* pNameSpace);

    /**
    \~chinese
     * @brief 获取相机特征可视权限
     * @param[in] hFeature 相机特征句柄
     * @param[out] pVis 相机特征可视性
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征可视权限可选项如下所示：
     * | 可视权限                            |
     * |-------------------------------------|
     * | ITKFEATURE_VAL_VISIBILITY_UNDEFINED |
     * | ITKFEATURE_VAL_VISIBILITY_BEGINNER  |
     * | ITKFEATURE_VAL_VISIBILITY_EXPERT    |
     * | ITKFEATURE_VAL_VISIBILITY_GURU      |
     * | ITKFEATURE_VAL_VISIBILITY_INVISIBLE |
    */
    /**
   \~english
    * @brief Get camera feature visibility
    * @param[in] hFeature Camera feature handle
    * @param[out] pVis Camera feature visibility
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The options for feature visibility are as follows:
    * | Visibility                          |
    * |-------------------------------------|
    * | ITKFEATURE_VAL_VISIBILITY_UNDEFINED |
    * | ITKFEATURE_VAL_VISIBILITY_BEGINNER  |
    * | ITKFEATURE_VAL_VISIBILITY_EXPERT    |
    * | ITKFEATURE_VAL_VISIBILITY_GURU      |
    * | ITKFEATURE_VAL_VISIBILITY_INVISIBLE |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetVisibility(ITKFEATURE hFeature, uint32_t* pVis);

    /**
    \~chinese
     * @brief 获取相机特征可视权限
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pVis 相机特征可视性
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 特征可视权限可选项如下所示：
     * | 可视权限                            |
     * |-------------------------------------|
     * | ITKFEATURE_VAL_VISIBILITY_UNDEFINED |
     * | ITKFEATURE_VAL_VISIBILITY_BEGINNER  |
     * | ITKFEATURE_VAL_VISIBILITY_EXPERT    |
     * | ITKFEATURE_VAL_VISIBILITY_GURU      |
     * | ITKFEATURE_VAL_VISIBILITY_INVISIBLE |
    */
    /**
   \~english
    * @brief Get camera feature visibility
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pVis Camera feature visibility
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note The options for feature visibility are as follows:
    * | Visibility                          |
    * |-------------------------------------|
    * | ITKFEATURE_VAL_VISIBILITY_UNDEFINED |
    * | ITKFEATURE_VAL_VISIBILITY_BEGINNER  |
    * | ITKFEATURE_VAL_VISIBILITY_EXPERT    |
    * | ITKFEATURE_VAL_VISIBILITY_GURU      |
    * | ITKFEATURE_VAL_VISIBILITY_INVISIBLE |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetVisibility(ITKDEVICE hDev, const char* featureName, uint32_t* pVis);

    /**
    \~chinese
     * @brief 获取相机特征表示方式
     * @param[in] hFeature 相机特征句柄
     * @param[out] pRep 相机特征表示方式
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note
     * 1. 该函数只对整数类型特征有效，对于其它类型特征，表示方式取值为 ITKFEATURE_VAL_REPRESENTATION_UNDEFINED 。
     * 2. 特征表示方式可选项如下所示：
     * | 表示方式                                  |
     * |-------------------------------------------|
     * | ITKFEATURE_VAL_REPRESENTATION_UNDEFINED   |
     * | ITKFEATURE_VAL_REPRESENTATION_LINEAR      |
     * | ITKFEATURE_VAL_REPRESENTATION_LOGARITHMIC |
     * | ITKFEATURE_VAL_REPRESENTATION_BOOLEAN     |
     * | ITKFEATURE_VAL_REPRESENTATION_PURENUMBER  |
     * | ITKFEATURE_VAL_REPRESENTATION_HEXNUMBER   |
     * | ITKFEATURE_VAL_REPRESENTATION_IPV4ADDRESS |
     * | ITKFEATURE_VAL_REPRESENTATION_MACADDRESS  |
    */
    /**
   \~english
    * @brief Get camera feature representation method
    * @param[in] hFeature Camera feature handle
    * @param[out] pRep Camera feature representation method
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note
    * 1. This function is only valid for integer type features, and for other type features, the representation takes the value ITKFEATURE_VAL_REPRESENTATION_UNDEFINED .
    * 2. The options for feature representation are as follows:
    * | Representation method                     |
    * |-------------------------------------------|
    * | ITKFEATURE_VAL_REPRESENTATION_UNDEFINED   |
    * | ITKFEATURE_VAL_REPRESENTATION_LINEAR      |
    * | ITKFEATURE_VAL_REPRESENTATION_LOGARITHMIC |
    * | ITKFEATURE_VAL_REPRESENTATION_BOOLEAN     |
    * | ITKFEATURE_VAL_REPRESENTATION_PURENUMBER  |
    * | ITKFEATURE_VAL_REPRESENTATION_HEXNUMBER   |
    * | ITKFEATURE_VAL_REPRESENTATION_IPV4ADDRESS |
    * | ITKFEATURE_VAL_REPRESENTATION_MACADDRESS  |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetRepresentation(ITKFEATURE hFeature, uint32_t* pRep);

    /**
    \~chinese
     * @brief 获取相机特征表示方式
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pRep 相机特征表示方式
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note
     * 1. 该函数只对整数类型特征有效，对于其它类型特征，表示方式取值为 ITKFEATURE_VAL_REPRESENTATION_UNDEFINED 。
     * 2. 特征表示方式可选项如下所示：
     * | 表示方式                                  |
     * |-------------------------------------------|
     * | ITKFEATURE_VAL_REPRESENTATION_UNDEFINED   |
     * | ITKFEATURE_VAL_REPRESENTATION_LINEAR      |
     * | ITKFEATURE_VAL_REPRESENTATION_LOGARITHMIC |
     * | ITKFEATURE_VAL_REPRESENTATION_BOOLEAN     |
     * | ITKFEATURE_VAL_REPRESENTATION_PURENUMBER  |
     * | ITKFEATURE_VAL_REPRESENTATION_HEXNUMBER   |
     * | ITKFEATURE_VAL_REPRESENTATION_IPV4ADDRESS |
     * | ITKFEATURE_VAL_REPRESENTATION_MACADDRESS  |
    */
    /**
   \~english
    * @brief Get camera feature representation method
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pRep Camera feature representation method
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note
    * 1. This function is only valid for integer type features, and for other type features, the representation takes the value ITKFEATURE_VAL_REPRESENTATION_UNDEFINED .
    * 2. The options for feature representation are as follows:
    * | Representation method                     |
    * |-------------------------------------------|
    * | ITKFEATURE_VAL_REPRESENTATION_UNDEFINED   |
    * | ITKFEATURE_VAL_REPRESENTATION_LINEAR      |
    * | ITKFEATURE_VAL_REPRESENTATION_LOGARITHMIC |
    * | ITKFEATURE_VAL_REPRESENTATION_BOOLEAN     |
    * | ITKFEATURE_VAL_REPRESENTATION_PURENUMBER  |
    * | ITKFEATURE_VAL_REPRESENTATION_HEXNUMBER   |
    * | ITKFEATURE_VAL_REPRESENTATION_IPV4ADDRESS |
    * | ITKFEATURE_VAL_REPRESENTATION_MACADDRESS  |
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetRepresentation(ITKDEVICE hDev, const char* featureName, uint32_t* pRep);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的最小值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 32位整数类型的最小值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the minimum value of 32bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Minimum value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt32Min(ITKFEATURE hFeature, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的最小值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 32位整数类型的最小值
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
    * @brief Get the minimum value of 32bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Minimum value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt32Min(ITKDEVICE hDev, const char* featureName, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的最大值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 32位整数类型的最大值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the maximum value of 32bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Maximum value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt32Max(ITKFEATURE hFeature, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的最大值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 32位整数类型的最大值
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
    * @brief Get the maximum value of 32bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Maximum value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt32Max(ITKDEVICE hDev, const char* featureName, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的步长
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 32位整数类型的单位增量
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the increment of 32bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Increment of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt32Inc(ITKFEATURE hFeature, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的步长
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 32位整数类型的单位增量
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
    * @brief Get the increment of 32bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Increment of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt32Inc(ITKDEVICE hDev, const char* featureName, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 32位整数类型相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the value of 32bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt32(ITKFEATURE hFeature, int32_t* pValue);

    /**
    \~chinese
     * @brief 获取32位整数类型特征的值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 32位整数类型相机特征值
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
    * @brief Get the value of 32bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt32(ITKDEVICE hDev, const char* featureName, int32_t* pValue);

    /**
    \~chinese
     * @brief 设置32位整数类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[in] value 32位整数类型相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 设定值的取值范围一定是[ ItkFeatureGetInt32Min , ItkFeatureGetInt32Max ]，
     取值同时也应满足value = ItkFeatureGetInt32Min + N* ItkFeatureGetInt32Inc ，N=0,1,2,…
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Set the value of 32bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[in] value Value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @note The range of values for the set value must be [ ItkFeatureGetInt32Min , ItkFeatureGetInt32Max ],
    value should also satisfy the condition of value = ItkFeatureGetInt32Min + N* ItkFeatureGetInt32Inc , N=0,1,2,…
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureSetInt32(ITKFEATURE hFeature, int32_t value);

    /**
    \~chinese
     * @brief 设置32位整数类型特征的值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[in] value 32位整数类型相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 设定值的取值范围一定是[ ItkDevGetInt32Min , ItkDevGetInt32Max ]，
     取值同时也应满足value = ItkDevGetInt32Min + N* ItkDevGetInt32Inc ，N=0,1,2,…
     * @see ItkDevGetAccessMode ItkDevGetType
    */
    /**
   \~english
    * @brief Set the value of 32bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[in] value Value of 32bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @note The range of values for the set value must be [ ItkDevGetInt32Min , ItkDevGetInt32Max ],
    value should also satisfy the condition of value = ItkDevGetInt32Min + N* ItkDevGetInt32Inc , N=0,1,2,…
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevSetInt32(ITKDEVICE hDev, const char* featureName, int32_t value);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的最小值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位整数类型的最小值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the minimum value of 64bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Minimum value of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt64Min(ITKFEATURE hFeature, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的最小值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 64位整数类型的最小值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkDevGetAccessMode tkDevGetType
    */
    /**
   \~english
    * @brief Get the minimum value of 64bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Minimum value of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt64Min(ITKDEVICE hDev, const char* featureName, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的最大值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位整数类型的最大值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the maximum value of 64bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Maximum value of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt64Max(ITKFEATURE hFeature, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的最大值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 64位整数类型的最大值
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
    * @brief Get the maximum value of 64bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Maximum value of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt64Max(ITKDEVICE hDev, const char* featureName, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的步长
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位整数类型的单位增量
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the increment of 64bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Increment of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt64Inc(ITKFEATURE hFeature, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的步长
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 64位整数类型的单位增量
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
    * @brief Get the increment of 64bit integer type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Increment of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetInt64Inc(ITKDEVICE hDev, const char* featureName, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取64位整数类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位整数类型相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the value of 64bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Value of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetInt64(ITKFEATURE hFeature, int64_t* pValue);

    /**
    \~chinese
     * @brief 设置64位整数类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[in] value 64位整数类型相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 设定值的取值范围一定是[ ItkFeatureGetInt64Min , ItkFeatureGetInt64Max ]，
     取值同时也应满足value = ItkFeatureGetInt64Min + N* ItkFeatureGetInt64Inc ，N=0,1,2,…
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Set the value of 64bit integer type feature
    * @param[in] hFeature Camera feature handle
    * @param[in] value Value of 64bit integer type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @note The range of values for the set value must be [ ItkFeatureGetInt64Min , ItkFeatureGetInt64Max ],
    value should also satisfy the condition of value = ItkFeatureGetInt64Min + N* ItkFeatureGetInt64Inc , N=0,1,2,…
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureSetInt64(ITKFEATURE hFeature, int64_t value);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的最小值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位浮点数类型的最小值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the minimum value of double type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Minimum value of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDoubleMin(ITKFEATURE hFeature, double* pValue);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的最小值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 64位浮点数类型的最小值
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
    * @brief Get the minimum value of double type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Minimum value of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDoubleMin(ITKDEVICE hDev, const char* featureName, double* pValue);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的最大值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位浮点数类型的最大值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the maximum value of double type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Maximum value of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDoubleMax(ITKFEATURE hFeature, double* pValue);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的最大值
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 64位浮点数类型的最大值
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
    * @brief Get the maximum value of double type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Maximum value of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDoubleMax(ITKDEVICE hDev, const char* featureName, double* pValue);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的步长
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位浮点数类型的增量值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the increment value of double type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Increment of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDoubleInc(ITKFEATURE hFeature, double* pValue);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的步长
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 64位浮点数类型的增量值
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
    * @brief Get the increment value of double type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Increment of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDoubleInc(ITKDEVICE hDev, const char* featureName, double* pValue);

    /**
    \~chinese
     * @brief 获取浮点数类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 64位浮点数类型的相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get value of double type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Value of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDouble(ITKFEATURE hFeature, double* pValue);

    /**
    \~chinese
     * @brief 设置浮点数类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[in] value 64位浮点数类型的相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 设定值的取值范围一定是[ ItkFeatureGetDoubleMin , ItkFeatureGetDoubleMax ]。
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Set value of double type feature
    * @param[in] hFeature Camera feature handle
    * @param[in] value Value of 64bit double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @note The range of values for the set value must be [ ItkFeatureGetDoubleMin , ItkFeatureGetDoubleMax ].
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureSetDouble(ITKFEATURE hFeature, double value);

    /**
    \~chinese
     * @brief 获取浮点数的显示精度
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 浮点数的显示精度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the precision of double type
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Precision of double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetDoublePrecision(ITKFEATURE hFeature, int64_t* pValue);

    /**
    \~chinese
     * @brief 获取浮点数的显示精度
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pValue 浮点数的显示精度
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
    * @brief Get the precision of double type
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pValue Precision of double type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetAccessMode ItkDevGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetDoublePrecision(ITKDEVICE hDev, const char* featureName, int64_t* pValue);

    /**
    \~chinese
     * @brief 设置布尔数据类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[in] pValue 布尔类型的相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Set the value of boolean type feature
    * @param[in] hFeature Camera feature handle
    * @param[in] pValue Boolean type value
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureSetBool(ITKFEATURE hFeature, _Bool value);

    /**
    \~chinese
     * @brief 获取布尔数据类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 布尔类型的相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the value of boolean type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue Boolean type value
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetBool(ITKFEATURE hFeature, _Bool* pValue);

    IKAPC_API ITKSTATUS IKAPC_CC ItkGigEDevWriteFirmware(ITKDEVICE hDev, uint8_t* firmware, uint16_t id);

    IKAPC_API ITKSTATUS IKAPC_CC ItkU3VDevWriteFirmware(ITKDEVICE hDev, uint8_t* firmware, uint16_t id);


    /**
    \~chinese
     * @brief 获取字符串或枚举类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[out] pBuff 字符串或枚举类型的相机特征值
     * @param[in,out] pBuffLen 作为输入指明特征值的最大长度，作为输出指明特征值的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 传递给此函数的缓冲区必须足够容纳整个字符串。如果调用该函数时，将pBuff设置为NULL，则pBufLen指向的变量将设置为所需的缓冲区大小。
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Get the value of string or enumeration type
    * @param[in] hFeature Camera feature handle
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
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureToString(ITKFEATURE hFeature, char* pBuff, uint32_t* pBuffLen);

    /**
    \~chinese
     * @brief 设置字符串或枚举类型特征的值
     * @param[in] hFeature 相机特征句柄
     * @param[in] pValue 字符串或枚举类型的相机特征值
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Set the value of string or enumeration type
    * @param[in] hFeature Camera feature handle
    * @param[in] pValue Value of string or enumeration type
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureFromString(ITKFEATURE hFeature, const char* pValue);

    /**
    \~chinese
     * @brief 获取枚举类型特征的枚举数量
     * @param[in] hFeature 相机特征句柄
     * @param[out] count 相机特征的枚举项数量
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetEnumString
    */
    /**
   \~english
    * @brief Get the number of enumerations with enumeration type feature
    * @param[in] hFeature Camera feature handle
    * @param[out] count The number of enumeration items for camera feature
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetEnumString
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetEnumCount(ITKFEATURE hFeature, uint32_t* pValue);

    /**
    \~chinese 
     * @brief 获取枚举类型特征的枚举数量
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] count 相机特征的枚举项数量
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkDevGetEnumString
    */
     /**
    \~english 
     * @brief Get the number of enumerations with enumeration type feature
     * @param[in] hDev Camera device handle
     * @param[in] featureName Camera feature name
     * @param[out] count The number of enumeration items for camera feature
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkDevGetEnumString
     */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetEnumCount(ITKDEVICE hDev, const char* featureName, uint32_t* pValue);

    /**
    \~chinese
     * @brief 获取枚举类型特征的枚举名
     * @param[in] hFeature 相机特征句柄
     * @param[in] index 枚举项索引，有效范围[0,ItkFeatureGetEnumCount-1]
     * @param[out] enumString 枚举项名称
     * @param[in,out] stringSize 作为输入指明枚举项名称的最大长度，作为输出指明枚举项名称的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetEnumCount
    */
    /**
   \~english
    * @brief Get the name of enumerations with enumeration type feature
    * @param[in] hFeature Camera feature handle
    * @param[in] index Enumeration item index, valid range is [0,ItkFeatureGetEnumCount-1]
    * @param[out] enumString Enumeration item name
    * @param[in,out] stringSize As input, it indicates the maximum length of enumeration item name. As output, it indicates the valid length of enumeration item name
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetEnumCount
    */
    IKAPC_API ITKSTATUS IKAPC_CC
        ItkFeatureGetEnumString(ITKFEATURE hFeature, uint32_t index, char* enumString, uint32_t* pStringSize);

    /**
    \~chinese
     * @brief 获取枚举类型特征的枚举名
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[in] index 枚举项索引，有效范围[0,ItkDevGetEnumCount-1]
     * @param[out] enumString 枚举项名称
     * @param[in,out] stringSize 作为输入指明枚举项名称的最大长度，作为输出指明枚举项名称的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkDevGetEnumCount
    */
    /**
   \~english
    * @brief Get the name of enumerations with enumeration type feature
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[in] index Enumeration item index, valid range is [0,ItkDevGetEnumCount-1]
    * @param[out] enumString Enumeration item name
    * @param[in,out] stringSize As input, it indicates the maximum length of enumeration item name. As output, it indicates the valid length of enumeration item name
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetEnumCount
    */
    IKAPC_API ITKSTATUS IKAPC_CC
        ItkDevGetEnumString(ITKDEVICE hDev, const char* featureName, uint32_t index, char* enumString, uint32_t* pStringSize);

    /**
    \~chinese
     * @brief 判断枚举项是否存在
     * @param[in] hFeature 相机特征句柄
     * @param[in] enumString 枚举项名称
     * @param[out] bAvailable true表示相机支持该枚举项，false表示相机不支持该枚举项
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetEnumString
    */
    /**
   \~english
    * @brief Determine whether the enumeration item exists
    * @param[in] hFeature Camera feature handle
    * @param[in] enumString Enumeration item name
    * @param[out] bAvailable true indicates that the camera supports the enumeration item, while false indicates that the camera does not support the enumeration item
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetEnumString
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetEnumIsAvailable(ITKFEATURE hFeature, const char* enumString, _Bool* bAvailable);

    /**
    \~chinese
     * @brief 判断枚举项是否存在
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[in] enumString 枚举项名称
     * @param[out] bAvailable true表示相机支持该枚举项，false表示相机不支持该枚举项
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkDevGetEnumString
    */
    /**
   \~english
    * @brief Determine whether the enumeration item exists
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[in] enumString Enumeration item name
    * @param[out] bAvailable true indicates that the camera supports the enumeration item, while false indicates that the camera does not support the enumeration item
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkDevGetEnumString
    */
    IKAPC_API ITKSTATUS IKAPC_CC
        ItkDevGetEnumIsAvailable(ITKDEVICE hDev, const char* featureName, const char* enumString, _Bool* bAvailable);

    /**
    \~chinese
     * @brief 根据相机特征执行相应命令
     * @param[in] hFeature 相机特征句柄
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Execute corresponding commands based on camera feature
    * @param[in] hFeature Camera feature handle
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureExecuteCommand(ITKFEATURE hFeature);

    /**
    \~chinese
     * @brief 判断命令类型特征是否执行完成
     * @param[in] hFeature 相机特征句柄
     * @param[out] isDone 是否执行完成
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    /**
   \~english
    * @brief Determine whether the command type feature has been executed successfully
    * @param[in] hFeature Camera feature handle
    * @param[out] isDone Is the execution completed
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureGetAccessMode ItkFeatureGetType
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureCommandIsDone(ITKFEATURE hFeature, _Bool* isDone);

    /**
    \~chinese
     * @brief 判断特征是否为选择器
     * @param[in] hFeature 相机特征句柄
     * @param[out] bSelector 是否为选择器
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 如果相机特征是选择器，在当该特征值改变时应该更新所有被选择特征的值。
     * @see ItkFeatureGetSelectedFeatureCounts
    */
    /**
   \~english
    * @brief Determine whether the feature is selector
    * @param[in] hFeature Camera feature handle
    * @param[out] bSelector Whether it is selector
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @note If the camera feature is selector, the values of all selected features should be updated when the feature value changes.
    * @see ItkFeatureGetSelectedFeatureCounts
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureIsSelector(ITKFEATURE hFeature, _Bool* bSelector);

    /**
    \~chinese
     * @brief 判断特征是否为选择器
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] bSelector 是否为选择器
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @note 如果相机特征是选择器，在当该特征值改变时应该更新所有被选择特征的值。
     * @see ItkDevGetSelectedFeatureCounts
    */
    /**
   \~english
    * @brief Determine whether the feature is selector
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] bSelector Whether it is selector
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @note If the camera feature is selector, the values of all selected features should be updated when the feature value changes.
    * @see ItkDevGetSelectedFeatureCounts
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevIsSelector(ITKDEVICE hDev, const char* featureName, _Bool* bSelector);

    /**
    \~chinese
     * @brief 获取被选择特征的数量
     * @param[in] hFeature 相机特征句柄
     * @param[out] pValue 被选择特征的数量
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureIsSelector
    */
    /**
   \~english
    * @brief Get the number of selected features
    * @param[in] hFeature Camera feature handle
    * @param[out] pValue The number of selected features
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureIsSelector
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetSelectedFeatureCounts(ITKFEATURE hFeature, uint32_t* pValue);

    /**
    \~chinese
     * @brief 获取被选择特征的名称
     * @param[in] hFeature 相机特征句柄
     * @param[in] index 特征索引，有效范围[0, ItkFeatureGetSelectedFeatureCounts -1]
     * @param[out] pBuf 被选择特征的名称
     * @param[out] pBufLen 作为输入指明特征名称的最大长度，作为输出指明特征名称的有效长度
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * - ITKSTATUS_NOT_IMPLEMENT
     * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
     * @see ItkFeatureIsSelector
    */
    /**
   \~english
    * @brief Get the name of the selected feature
    * @param[in] hFeature Camera feature handle
    * @param[in] index Feature index, valid range is [0, ItkFeatureGetSelectedFeatureCounts -1]
    * @param[out] pBuf The name of the selected feature
    * @param[out] pBufLen As input, it indicates the maximum length of feature name. As output, it indicates the effective length of feature name
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * - ITKSTATUS_NOT_IMPLEMENT
    * - ITKSTATUS_INCOMPATIBLE_FEATURE_TYPE
    * @see ItkFeatureIsSelector
    */
    IKAPC_API ITKSTATUS IKAPC_CC
        ItkFeatureGetSelectedFeatureName(ITKFEATURE hFeature, uint32_t index, char* pBuf, uint32_t* pBufLen);

    /**
    \~chinese
     * @brief 获取相机特征的轮询周期
     * @param[in] hFeature 相机特征句柄
     * @param[out] pPollingTime 相机特征查询的轮询时间
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 对于只读型特征例如相机温度，可能需要周期性的更新该特征值来实时监控相机状态，轮询时间指明更新周期的最小值。
     * @see ItkDevAllocFeature
    */
    /**
   \~english
    * @brief Polling period for getting camera features
    * @param[in] hFeature Camera feature handle
    * @param[out] pPollingTime Polling time for camera feature query
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note For read-only features such as camera temperature, it may be necessary to periodically update the feature value to monitor the camera status in real time,
    and the polling time indicates the minimum value of the update period.
    * @see ItkDevAllocFeature
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkFeatureGetPollingTime(ITKFEATURE hFeature, uint32_t* pPollingTime);

    /**
    \~chinese
     * @brief 获取相机特征的轮询周期
     * @param[in] hDev 相机设备句柄
     * @param[in] featureName 相机特征名称
     * @param[out] pPollingTime 相机特征查询的轮询时间
     * @return
     * - ITKSTATUS_OK
     * - ITKSTATUS_INVALID_ARG
     * - ITKSTATUS_INVALID_HANDLE
     * @note 对于只读型特征例如相机温度，可能需要周期性的更新该特征值来实时监控相机状态，轮询时间指明更新周期的最小值。
     * @see ItkDevAllocFeature
    */
    /**
   \~english
    * @brief Polling period for getting camera features
    * @param[in] hDev Camera device handle
    * @param[in] featureName Camera feature name
    * @param[out] pPollingTime Polling time for camera feature query
    * @return
    * - ITKSTATUS_OK
    * - ITKSTATUS_INVALID_ARG
    * - ITKSTATUS_INVALID_HANDLE
    * @note For read-only features such as camera temperature, it may be necessary to periodically update the feature value to monitor the camera status in real time,
    and the polling time indicates the minimum value of the update period.
    * @see ItkDevAllocFeature
    */
    IKAPC_API ITKSTATUS IKAPC_CC ItkDevGetPollingTime(ITKDEVICE hDev, const char* featureName, uint32_t* pPollingTime);

/**
\~chinese 
 * @brief 获取缓冲区参数
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] prm 参数索引
 * @param[out] pValue 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferSetPrm ITKBUFFER_PRM (n, size)
*/
 /**
\~english 
 * @brief Get buffer parameter
 * @param[in] hBuffer Buffer handle
 * @param[in] prm Parameter index
 * @param[out] pValue Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferSetPrm ITKBUFFER_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferGetPrm(ITKBUFFER hBuffer, uint32_t prm, void* pValue);

/**
\~chinese 
 * @brief 设置缓冲区参数
 * @param[in] hBuffer 缓冲区句柄
 * @param[in] prm 参数索引
 * @param[in] value 参数值
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferGetPrm ITKBUFFER_PRM (n, size)
*/
 /**
\~english 
 * @brief Set buffer parameter
 * @param[in] hBuffer Buffer handle
 * @param[in] prm Parameter index
 * @param[in] value Parameter value
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @see ItkBufferGetPrm ITKBUFFER_PRM (n, size)
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferSetPrm(ITKBUFFER hBuffer, uint32_t prm, const void* value);

/**
\~chinese 
 * @brief RGB通道重排列
 * @param[in] hSrc 源缓冲区句柄，像素格式必须是彩色格式
 * @param[in] hDst 目的缓冲区句柄。目的缓冲区图像高度必须是源缓冲区图像高度的3倍，像素格式大小必须是源缓冲区的1/3
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note 重排后的像素如下
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
 * @see ItkBufferNew
*/
 /**
\~english 
 * @brief RGB channel rearrangement
 * @param[in] hSrc Source buffer handle, pixel format must be in color format
 * @param[in] hDst Destination buffer handle. The height of the destination buffer image must be three times that of the source buffer image, 
 and the pixel format size must be one-third that of the source buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * - ITKSTATUS_INVALID_HANDLE
 * @note Pixel after rearrangement
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
 * @see ItkBufferNew
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkBufferRGBSplit(ITKBUFFER hSrc, ITKBUFFER hDst);

/**
\~chinese
 * @brief 黑白压缩8bit
 * @note 灰度8bit压缩图像数据。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief Monochrome packed 8bit
 * @note Grayscale 8bit packed image data.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_MONO8PACKED (ITKBUFFER_FORMAT(8, 8, 0x28) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief RGB彩色压缩8bit
 * @note 彩色8bit压缩图像，像素值按照RGB顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief RGB color packed 8bit
 * @note Color 8bit packed image data, pixel values are arranged in memory in RGB order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_RGB888PACKED                                                                                   \
    (ITKBUFFER_FORMAT(8, 24, 0x29) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )

/**
\~chinese
 * @brief BGR彩色压缩8bit
 * @note 彩色8bit压缩图像，像素值按照BGR顺序在内存中排列。
 * @see ITKBUFFER_PRM_FORMAT
*/
 /**
\~english
 * @brief BGR color packed 8bit
 * @note Color 8bit packed image data, pixel values are arranged in memory in BGR order.
 * @see ITKBUFFER_PRM_FORMAT
 */
#define ITKBUFFER_VAL_FORMAT_BGR888PACKED                                                                                   \
    (ITKBUFFER_FORMAT(8, 24, 0x2A) | ITKBUFFER_FORMAT_COLOR | ITKBUFFER_FORMAT_UNSIGNED )


#define ITKBUFFER_VAL_BAYER_METHOD_ITEK_V4                                                                                  \
    0x00000030u	///< @brief \~chinese Bayer转换itek_v4方法 \~english Bayer conversion itek_v4 method

/**
\~chinese
 * @brief 帧触发信号忽略回调事件
 * @note 本事件在帧触发信号忽略时触发,通过"FrameTriggerIgnored"注册。
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english
 * @brief Frame trigger signal ignores callback event
 * @note This event is triggered when the frame trigger signal is ignored, registered by "FrameTriggerIgnored".
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
#define ITKDEV_VAL_EVENT_TYPE_FRAME_TRIGGER_IGNORE 0x00040000u

/**
\~chinese
 * @brief 行触发信号忽略回调事件
 * @note 本事件在行触发信号忽略时触发,通过"LineTriggerIgnored"注册。
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english
 * @brief Line trigger signal ignores callback event
 * @note This event is triggered when the line trigger signal is ignored, registered by "LineTriggerIgnored".
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
#define ITKDEV_VAL_EVENT_TYPE_LINE_TRIGGER_IGNORE 0x00080000u

/**
\~chinese
 * @brief 帧触发采集回调事件
 * @note 本事件在帧触发采集时触发,通过"FrameTrigger"注册。
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
*/
 /**
\~english
 * @brief Frame trigger acquisition callback event
 * @note This event is triggered during frame trigger acquisition, registered by "FrameTrigger".
 * @see ItkDevRegisterCallback ItkDevUnregisterCallback
 */
#define ITKDEV_VAL_EVENT_TYPE_FRAME_TRIGGER 0x00100000u

///@{
/**
\~chinese
 * @name 事件信息参数
 * @see ItkEventInfoGetPrm
 * @note 推荐使用ItkEventGetInfo获取event信息。
*/
 /**
\~english
 * @name Event information parameter
 * @see ItkEventInfoGetPrm
 * @note It is recommended to use ItkEventGetInfo to obtain event information.
 */
#define ITKEVENTINFO_PRM_FIRST    0u
#define ITKEVENTINFO_PRM(n, size) ((((n) + ITKDEV_PRM_FIRST) << 16) | (size))
#define ITKEVENTINFO_PRM_TYPE            ITKEVENTINFO_PRM(0, 4)	///< @brief \~chinese 事件类型 \~english Event type
#define ITKEVENTINFO_PRM_FEATURE_NAME    ITKEVENTINFO_PRM(1, 128)	///< @brief \~chinese 特征名称 \~english Feature Name
#define ITKEVENTINFO_PRM_HOST_TIME_STAMP ITKEVENTINFO_PRM(2, 8)	///< @brief \~chinese 主机时间戳 \~english Host timestamp
#define ITKEVENTINFO_PRM_BLOCK_ID        ITKEVENTINFO_PRM(3, 8)	///< @brief \~chinese块索引  \~english Block ID
#define ITKEVENTINFO_PRM_TIMESTAMP       ITKEVENTINFO_PRM(4, 8)	///< @brief \~chinese 时间戳 \~english Timestamp of event info
#define ITKEVENTINFO_PRM_DATA            ITKEVENTINFO_PRM(5, 256)	///< @brief \~chinese 事件数据 \~english Data of event info
///@}

/**
\~chinese 
 * @brief 获取事件参数信息
 * @param[in] hEventInfo 事件信息句柄
 * @param[in] prm 事件信息索引
 * @param[out] value 事件信息输出缓冲区
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * @note 推荐使用ItkEventGetInfo获取event信息
*/
 /**
\~english 
 * @brief Get event parameter information
 * @param[in] hEventInfo Event information handle
 * @param[in] prm Event Information Index
 * @param[out] value Event information output buffer
 * @return
 * - ITKSTATUS_OK
 * - ITKSTATUS_INVALID_ARG
 * @note It is recommended to use ItkEventGetInfo to obtain event information.
 */
IKAPC_API ITKSTATUS IKAPC_CC ItkEventInfoGetPrm(ITKEVENTINFO hEventInfo, uint32_t prm, void* value);

///@{
/**
\~chinese
 * @brief 负载类型
 * @see ItkBufferSizedNew ItkBufferNewWithPtr
 * @note 【弃用】，推荐使用PAYLOAD_TYPE
*/
 /**
\~english
 * @brief paylaod type
 * @see ItkBufferSizedNew ItkBufferNewWithPtr
 * @note [Deprecated]，PAYLOAD_TYPE recommended
 */
#define ITKBUFFER_VAL_IMAGE_PAYLOAD_TYPE                    0u	///< @brief \~chinese 相机图像模式 \~english Camera image mode
#define ITKBUFFER_VAL_U3V_IMAGE_EXTENDED_CHUNK_PAYLOAD_TYPE 1u	///< @brief \~chinese U3V相机Chunk模式1 \~english U3V camera chunk mode
#define ITKBUFFER_VAL_U3V_CHUNK_PAYLOAD_TYPE                2u	///< @brief \~chinese U3V相机Chunk模式2 \~english GV camera chunk mode
#define ITKBUFFER_VAL_GV_CHUNK_DATA_PAYLOAD_TYPE            3u	///< @brief \~chinese GV相机Chunk模式 \~english GV camera chunk mode
///@}

#ifdef __cplusplus
}
#    ifdef IKAPC_BOOL_DEFINED
#        undef _Bool
#        undef IKAPC_BOOL_DEFINED
#    endif
#endif 

#endif
// clang-format on
