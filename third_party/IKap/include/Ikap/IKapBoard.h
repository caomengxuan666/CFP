// clang-format off
/**
\~chinese
 * @file IKapBoard.h
 * @brief IKapBoard库头文件
 * @note 本文件介绍创建采集卡C/C++应用程序所需要的接口定义。
*/
 /**
\~english
 * @file IKapBoard.h
 * @brief IKapBoard library header file
 * @note This document introduces the definitions of interfaces required to create C/C++ application for frame grabber.
 */

#ifndef _IKAP_BOARD_H
#define _IKAP_BOARD_H

#if defined(_MSC_VER) || defined(__BORLANDC__)
#    ifdef IKAPBOARD_EXPORTS
#        define IKAPBOARDDLL __declspec(dllexport)
#    else
#        define IKAPBOARDDLL __declspec(dllimport)
#    endif
#    define IKAPBOARD_CC      __stdcall
#    define IKAPBOARD_PACKING 8
#elif defined(__GNUC__) || defined(SWIG)
#    define IKAPBOARDDLL
#    define IKAPBOARD_CC
#    define IKAPBOARD_PACKING 8
#else
#    error Unsupported platform
#endif

#if defined(_WIN32)
#    if defined(_MSC_VER) && _MSC_VER >= 1600
#        include <stdint.h>
#    elif !defined _STDINT_H && !defined _STDINT

typedef unsigned char    uint8_t;
typedef __int32          int32_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#    endif
#else
#    include <stdint.h>
#endif

#include "IKapBoardInfoType.h"

/**
\~chinese
 * @brief 采集卡设备句柄
*/
 /**
\~english
 * @brief Frame grabber device handle
 */
typedef void* HANDLE;

#ifdef __cplusplus
#    define ENUM_TYPE :unsigned
#else
#    define ENUM_TYPE	///< @brief \~chinese 枚举类型 \~english Enumeration type
#endif  // __cplusplus

///@{
/**
\~chinese
 * @name 回调事件类型
*/
 /**
\~english
 * @name Callback event type
 */
/**
\~chinese
 * @brief 采集开始回调事件
 * @note 图像采集开始时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Grab start callback event
 * @note This callback event is triggered when image acquisition starts.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_GrabStart (0x00000000)

/**
\~chinese
 * @brief 采集帧完成回调事件
 * @note 在图像采集过程中，缓冲区有一帧或超过一帧的完整图像被采集完毕时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Grab frame ready callback event
 * @note This callback event is triggered when one or more frames of complete images are grabbed in buffer during acquisition process.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_FrameReady (0x00000001)

/**
\~chinese
 * @brief 采集停止回调事件
 * @note 图像采集停止时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Grab stop callback event
 * @note This callback event is triggered when image acquisition stops.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_GrabStop (0x00000002)

/**
\~chinese
 * @brief 帧丢失回调事件
 * @note 图像缓冲区溢出时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Frame loss callback event
 * @note This callback event is triggered when image buffer overflows.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_FrameLost (0x00000003)

/**
\~chinese
 * @brief 采集超时回调事件
 * @note 图像采集超时时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Grab timeout callback event
 * @note This callback event is triggered when image acquisition timeout.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_TimeOut (0x00000004)

/**
\~chinese
 * @brief 像素时钟有效回调事件
 * @note 相机时钟信号有效时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Pixel clock valid callback event
 * @note This callback event is triggered when camera clock signal is valid.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_PixelClock (0x00000005)

/**
\~chinese
 * @brief 像素时钟无效回调事件
 * @note 相机时钟信号无效时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Pixel clock invalid callback event
 * @note This callback event is triggered when camera clock signal is invalid.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_No_PixelClock (0x00000006)

/**
\~chinese
 * @brief 触发信号忽略回调事件
 * @note 采集卡工作在外触发模式，外触发信号被采集卡丢失时触发该回调事件。该回调事件可能在外触发信号频率高于采集频率的情况下发生。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Trigger signal ignored callback event
 * @note When frame grabber works in external trigger mode, callback event is triggered when external trigger signal is
ignored by frame grabber. This callback event may occur when external trigger frequency is higher than frame rate.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_External_Trigger_Ignored (0x00000007)

/**
\~chinese
 * @brief CXP采集卡过流保护回调事件
 * @note CXP采集卡发生过流保护时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief CXP frame grabber overcurrent protection callback event
 * @note This callback event is triggered when overcurrent protection occurs on CXP frame grabber.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_CXP_Over_Current_Protection (0x00000008)

/**
\~chinese
 * @brief CXP采集卡CRC错误回调事件
 * @note CXP采集卡图像数据发生CRC错误时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief CXP frame grabber CRC error callback event
 * @note This callback event is triggered when CRC error occurs on CXP frame grabber.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_CXP_CRC_Check_Error (0x00000009)

/**
\~chinese
 * @brief CXP采集卡传输错误回调事件
 * @note CXP采集卡发生传输错误时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief CXP frame grabber transmission error callback event
 * @note This callback event is triggered when transmission error occurs on CXP frame grabber.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_CXP_Transfer_Error (0x0000000A)

/**
\~chinese
 * @brief 行间超时回调事件
 * @note 采集卡发生行间超时时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Line timeout callback event
 * @note This callback event is triggered when line timeout occurs on frame grabber.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_Line_Timeout (0x0000000B)

/**
\~chinese
 * @brief CXP采集卡未完成帧回调事件
 * @note CXP采集卡收到未完成帧时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief CXP frame grabber incomplete frame callback event
 * @note This callback event is triggered when CXP frame grabber receives an incomplete frame.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_CXP_Incomplete_Frame (0x0000000C)

/**
\~chinese
 * @brief GVB采集卡未完成帧回调事件
 * @note GVB采集卡收到未完成帧时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief GVB frame grabber incomplete frame callback event
 * @note This callback event is triggered when GVB frame grabber receives an incomplete frame.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_GVB_Incomplete_Frame (0x0000000D)

/**
\~chinese
 * @brief GVB采集卡CRC错误回调事件
 * @note GVB采集卡图像数据发生CRC错误时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief GVB frame grabber CRC error callback event
 * @note This callback event is triggered when CRC error occurs on GVB frame grabber.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_GVB_GVSP_CRC_Check_Error (0x0000000E)

/**
\~chinese
 * @brief 缩略图传入回调事件
 * @note 缩略图从采集卡传入内存时触发该回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Thumbnail input callback event
 * @note This callback event is triggered when thumbnail passes into memory from frame grabber.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_Thumbnail_End (0x0000000F)

/**
\~chinese
 * @brief 中心线传入回调事件
 * @note 当第n行中心线数据从采集卡传入内存时触发该回调事件，仅适用于支持中心线功能的SFP线扫描相机。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Central line input callback event
 * @note This callback event is triggered when the N-th central line data passes into memory from frame grabber. It is only
applicable to SFP line scan cameras that support central line function.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_SFP_Central_Line_End (0x00001000)

/**
\~chinese
 * @brief 行数据开始回调事件
 * @note
当第n行数据从相机传入采集卡中时触发该回调事件，此时缓冲区内尚未存在有效的图像数据。仅适用于线扫描相机【该功能已弃用】。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Line data start callback event
 * @note This callback event is triggered when the N-th line data passes into frame grabber from camera, and there is no
valid image data in the buffer at this time. It is only applicable to line scan cameras [abandoned].
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_GrabLine (0x00100000)

/**
\~chinese
 * @brief 行数据结束回调事件
 * @note 当第n行数据从采集卡传入内存时触发该回调事件，仅适用于线扫描相机。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Line data end callback event
 * @note This callback event is triggered when the N-th line data passes into memory from frame grabber. It is only
applicable to line scan cameras.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_GrabLineEnd (0x00200000)

/**
\~chinese
 * @brief 触发信号下降沿回调事件
 * @note 采集卡检测到外触发信号n的下降沿时触发该回调事件，n的取值参考 IKapRegisterCallback 函数说明。 IKEvent_INPUT_FALLING_EDGE 和 IKEVENT_INPUT_RISING_EDGE 不能同时注册。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Trigger signal falling edge callback event
 * @note This callback event is triggered when frame grabber detects the falling edge of external trigger signal n, and the
value of n is described in IKapRegisterCallback function. IKEvent_INPUT_FALLING_EDGE and IKEVENT_INPUT_RISING_EDGE cannot be
registered simultaneously.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_INPUT_FALLING_EDGE (0x00400000)

/**
\~chinese
 * @brief 触发信号上升沿回调事件
 * @note 采集卡检测到外触发信号n的上升沿时触发该回调事件，n的取值参考 IKapRegisterCallback 函数说明。 IKEvent_INPUT_FALLING_EDGE 和 IKEVENT_INPUT_RISING_EDGE 不能同时注册。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Trigger signal rising edge callback event
 * @note This callback event is triggered when frame grabber detects the rising edge of external trigger signal n, and the
value of n is described in IKapRegisterCallback function. IKEvent_INPUT_FALLING_EDGE and IKEVENT_INPUT_RISING_EDGE cannot be
registered simultaneously.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEVENT_INPUT_RISING_EDGE (0x00800000)

/**
\~chinese
 * @brief 帧开始回调事件
 * @note 当采集卡开始一帧图像数据时触发该回调事件。该回调事件为相机数据传输到采集卡的开始结点,
获取图像数据需要通过 IKEvent_FrameReady 回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Frame start callback event
 * @note This callback event is triggered when frame grabber starts a frame of image data. This callback event is the
starting node for transmitting camera data to frame grabber, and getting image data requires IKEvent_FrameReady callback
event.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_Start_Of_Frame (0x01000000)

/**
\~chinese
 * @brief 帧结束回调事件
 * @note 当一帧数据已经在采集卡中完成，但是还未传到上位机时触发该回调事件。该回调事件为相机数据传输到采集卡的结束结点,
获取图像数据需要通过 IKEvent_FrameReady 回调事件。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Frame end callback event
 * @note This callback event is triggered when a frame of data has been completed in frame grabber but has not been
transmitted to upper computer. This callback event is the end node of camera data transmission to frame grabber, and getting
image data requires IKEvent_FrameReady callback event.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_End_Of_Frame (0x02000000)

/**
\~chinese
 * @brief 传输完成回调事件
 * @note 当第n个缓冲区数据传输完成时触发该回调事件，缓冲区计数从0开始。
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @brief Transmission completion callback event
 * @note This callback event is triggered when data transmission of N-th buffer is completed, and buffer index starts
counting from 0.
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
#define IKEvent_End_Of_Transfer (0x04000000)
#define IKEvent_SubFrameReady   (0x08000000)
///@}

///@{
/**
\~chinese
 * @name 用户自定义回调函数
 * @see IKapRegisterCallback IKapUnRegisterCallback
*/
 /**
\~english
 * @name User defined callback function
 * @see IKapRegisterCallback IKapUnRegisterCallback
 */
typedef void(IKAPBOARD_CC* HookFnPtr)(void* pContext);
///@}

///@{
/**
\~chinese
 * @name 采集卡资源类型
 * @see IKapGetBoardCount
*/
 /**
\~english
 * @name Frame grabber resource type
 * @see IKapGetBoardCount
 */

#define IKBoardUnknown 0xFFFFFFFF	///< @brief \~chinese 未知类型采集卡 \~english Unknown type frame grabber
#define IKBoardALL     0x00000000	///< @brief \~chinese 所有类型采集卡 \~english All types of frame grabber
#define IKBoardUSB30   0x00000001	///< @brief \~chinese USB3.0采集卡 \~english USB3.0 frame grabber
#define IKBoardPCIE    0x00000002	///< @brief \~chinese PCIe采集卡 \~english PCIe frame grabber
///@}

///@{
/**
\~chinese
 * @name 错误码类型
 * @see IKapGetLastError
*/
 /**
\~english
 * @name Error code type
 * @see IKapGetLastError
 */

#define IK_RTN_OK    (1)	///< @brief \~chinese 正常 \~english Success
#define IK_RTN_ERROR (0)	///< @brief \~chinese 异常 \~english Error
#define IKStatus_Success 				0x00000001	///< @brief \~chinese 操作成功 \~english Successful operation
#define IKStatus_BoardNotFound          0x00000002	///< @brief \~chinese 设备未发现 \~english Cannot find board
#define IKStatus_AllocMemoryFail        0x00000003	///< @brief \~chinese 申请内存失败 \~english Fail to allocate memory
#define IKStatus_InvalidParameter       0x00000004	///< @brief \~chinese 无效的参数 \~english Invalid parameter
#define IKStatus_OpenBoardFail          0x00000005	///< @brief \~chinese 打开设备失败 \~english Fail to open board
#define IKStatus_TimeOut                0x00000006	///< @brief \~chinese 操作超时 \~english Operation timeout
#define IKStatus_WinError               0x00000007	///< @brief \~chinese 系统错误 \~english System error
#define IKStatus_BoardNotOpen           0x00000008	///< @brief \~chinese 设备未打开 \~english Board is not open
#define IKStatus_ConfigFilePathInvalid  0x00000009	///< @brief \~chinese 配置文件路径无效 \~english Invalid configuration file path
#define IKStatus_ConfigParameterInvalid 0x0000000a	///< @brief \~chinese 配置参数无效 \~english Invalid configuration parameter
#define IKStatus_ZLP                    0x0000000b	///< @brief \~chinese USB3.0零长度包 \~english USB3.0 zero length packet
#define IKStatus_ThreadUnsetup          0x0000000c	///< @brief \~chinese 线程无法建立 \~english Fail to create the thread
#define IKStatus_ThreadExist            0x0000000d	///< @brief \~chinese 当前线程已存在 \~english The current thread already exists
#define IKStatus_CameraUnsupport        0x0000000e	///< @brief \~chinese 相机类型不支持 \~english The camera type is not supported
#define IKStatus_XMLFileLoadFail        0x0000000f	///< @brief \~chinese 载入XML文件失败 \~english Fail to load XML file
#define IKStatus_NodeNotExist           0x00000010	///< @brief \~chinese XML文件节点不存在 \~english XML file node does not exist
#define IKStatus_WriteSerialFail        0x00000011	///< @brief \~chinese 写串口失败 \~english Fail for serial writing
#define IKStatus_CommandNotExist        0x00000012	///< @brief \~chinese 串口指令不存在 \~english The serial command does not exist
#define IKStatus_CommandCollision       0x00000013	///< @brief \~chinese 串口指令冲突 \~english Serial command conflict
#define IKStatus_CommandMissRequirement                                                                                     \
    0x00000014	///< @brief \~chinese 串口指令不符合要求 \~english The serial port command does not meet the requirements
#define IKStatus_CommandNotAllow                                                                                            \
    0x00000015	///< @brief \~chinese 当前不允许串口指令 \~english The serial port command is not allowed
#define IKStatus_CommandSyntaxError 0x00000016	///< @brief \~chinese 串口指令格式错误 \~english Serial port command format error
#define IKStatus_NodeTypeDismatch   0x00000017	///< @brief \~chinese XML节点不匹配 \~english XML node does not match
#define IKStatus_FeatureUnSupport   0x00000018	///< @brief \~chinese 不支持该相机特征 \~english The camera feature is not supported
#define IKStatus_CommandResultNoExit                                                                                        \
    0x00000019	///< @brief \~chinese 相机执行结果无意义 \~english Camera execution result is meaningless
#define IKStatus_CLRegPathNotFound   0x0000001a	///< @brief \~chinese CamaraLinkPath不存在 \~english Cannot find CamaraLinkPath
#define IKStatus_CLDLLNotFound       0x0000001b	///< @brief \~chinese CLallserial.dll不存在 \~english Cannot find CLallserial.dll
#define IKStatus_CameraNotFound      0x0000001c	///< @brief \~chinese 相机未发现 \~english Cannot find camera
#define IKStatus_BufferTooSmall      0x0000001d	///< @brief \~chinese 传入缓冲区域太小 \~english Input buffer is too small
#define IKStatus_BaudrateNotSupport  0x0000001e	///< @brief \~chinese 不支持该波特率 \~english The baud rate is not supported
#define IKStatus_CameraInUse         0x0000001f	///< @brief \~chinese 相机正在使用中 \~english Camera is in use
#define IKStatus_FPGA_EraseFlashFail 0x00000020	///< @brief \~chinese 擦除FPGA失败 \~english Fail to erase FPGA
#define IKStatus_FPGA_CheckFail      0x00000021	///< @brief \~chinese FPGA下载过程校验失败 \~english Fail to check FPGA firmware
#define IKStatus_BoardNotBindingCOM  0x00000022	///< @brief \~chinese 当前设备未绑定串口 \~english Board is not bound to COM
#define IKStatus_ReadRegFail         0x00000023	///< @brief \~chinese 读取相机状体寄存器失败 \~english Fail to read camera status register
#define IKStatus_Invalid_Mutex       0x00000024	///< @brief \~chinese 无效的设备锁句柄 \~english Invalid board lock handle
#define IKStatus_Mutex_Locked        0x00000025	///< @brief \~chinese 设备已经被占用 \~english Board is occupied
#define IKStatus_Invalid_Handle      0x00000026	///< @brief \~chinese 无效的设备句柄 \~english Invalid device handle
#define IKStatus_Set_Info_Error                                                                                             \
    0x00000027	///< @brief \~chinese 设置参数值无效或者错误 \~english Invalid or wrong setting parameter value
#define IKStatus_Grab_Pending                                                                                               \
    0x00000028	///< @brief \~chinese 当前有正在执行的图像采集过程 \~english Image acquisition process is in progress
#define IKStatus_Insufficient_Resource                                                                                      \
    0x00000029	///< @brief \~chinese 系统没有足够的资源 \~english System does not have enough resources
#define IKStatus_Grab_Abort 0x0000002a	///< @brief \~chinese 放弃当前采集 \~english Abandon the current acquisition
#define IKStatus_Need_Reboot                                                                                                \
    0x0000002b	///< @brief \~chinese 系统出现致命的错误，必须重启计算机才能恢复正常 \~english A fatal error occurred in the system, and the computer must be restarted to return to normal
#define IKStatus_Need_Restart                                                                                               \
    0x0000002c	///< @brief \~chinese 采集卡出现错误，需要重新打开采集卡设备才能恢复正常 \~english An error occurred in the frame grabber, and the frame grabber needs to be reopened to recover to normal
#define IKStatus_Not_Implement 0x0000002d	///< @brief \~chinese 功能未实现 \~english Function not implemented
#define IKStatus_Not_In_Callback                                                                                            \
    0x0000002e	///< @brief \~chinese 该功能应在回调函数中运行，但实际不在 \~english This function should be run in callback function, but actually not
#define IKStatus_Not_Match_Driver_Version 0x0000002f	///< @brief \~chinese 驱动版本不匹配 \~english Driver version mismatch
#define IKStatus_Prm_Not_Available                                                                                          \
    0x00000030	///< @brief \~chinese 参数当前不可读不可写 \~english The parameter is currently unreadable and not writable
#define IKStatus_Prm_Locked                                                                                                 \
    0x00000031	///< @brief \~chinese 采集卡参数锁定，无法修改 \~english The parameters of frame grabber are locked and cannot be modified
#define IKStatus_Logic_Error            0x00000032	///< @brief \~chinese 逻辑错误 \~english Logic error
#define IKStatus_Out_Of_Bounds          0x00000033	///< @brief \~chinese 参数越界 \~english Out-of-bounds
#define IKStatus_Dvp_Error              0x00000034	///< @brief \~chinese Dvp 错误 \~english Dvp error
#define IKStatus_Prm_Write_Only         0x00000035	///< @brief \~chinese 采集卡参数只写 \~english The parameter is currently unreadable
///@}

///@{
/**
\~chinese
 * @brief 缓冲区状态信息
 * @see IKapGetBufferStatus
*/
 /**
\~english
 * @brief Buffer status information
 * @see IKapGetBufferStatus
 */
typedef struct _ikap_buffer_status {
    unsigned uFull;      ///< @brief \~chinese 缓冲区是否为满 \~english Whether buffer is full
    unsigned uEmpty;     ///< @brief \~chinese 缓冲区是否为空 \~english Whether buffer is empty
    unsigned uTransfer;  ///< @brief \~chinese 缓冲区是否正在传输 \~english Whether buffer is transferring
    unsigned uOverflow;  ///< @brief \~chinese 缓冲区是否溢出 \~english Whether buffer is overflow

/**
\~chinese
 * @brief 有效行数
 * @note 对于线扫描相机，本参数指明当前传输的图像有效行数；对于面扫描相机，本参数未定义。
 */
 /**
\~english
 * @brief Valid line number
 * @note For line scan camera, it indicates the number of valid image lines currently transferred. For line scan camera,
this parameter is not defined.
 */
    unsigned uLineNum;
} IKAPBUFFERSTATUS, *PIKAPBUFFERSTATUS;
///@}

///@{
/**
\~chinese
 * @brief 缓冲区状态信息
 * @see IKapGetBufferStatusEx
*/
 /**
\~english
 * @brief Buffer status information
 * @see IKapGetBufferStatusEx
 */
typedef struct _ikap_buffer_status_ex {
    unsigned uFull;      ///< @brief \~chinese 缓冲区是否为满 \~english Whether buffer is full
    unsigned uEmpty;     ///< @brief \~chinese 缓冲区是否为空 \~english Whether buffer is empty
    unsigned uTransfer;  ///< @brief \~chinese 缓冲区是否正在传输 \~english Whether buffer is transferring
    unsigned uOverflow;  ///< @brief \~chinese 缓冲区是否溢出 \~english Whether buffer is overflow

/**
\~chinese
 * @brief 有效行数
 * @note 对于线扫描相机，本参数指明当前传输的图像有效行数；对于面扫描相机，本参数未定义。
 */
 /**
\~english
 * @brief Valid line number
 * @note For line scan camera, it indicates the number of valid image lines currently transferred. For line scan camera,
this parameter is not defined.
 */
    unsigned uLineNum;

/**
\~chinese
 * @brief 压缩图像有效长度
 * @note 对于压缩图像，本参数指明当前缓冲区中有效长度。
 */
 /**
\~english
 * @brief Compressed image effective length
 * @note For compression image, it stores valid length in the current buffer.
 */
    unsigned long long uCompressSize;

/**
\~chinese
 * @brief 时间戳（us）
 * @note 本参数指明缓冲区完成的时间相对于开始采集的时间戳。
 */
 /**
\~english
 * @brief Timestamp (us)
 * @note This parameter indicates the completion time of buffer relative to timestamp of the start of acquisition.
 */
    unsigned long long uTimestamp;

/**
\~chinese
 * @brief 计算帧率
 * @note 本参数指明计算出的当前缓冲区帧率, 大小等于1/(距离上一帧完成的时间),与从采集卡读取的瞬时帧率不同。
 */
 /**
\~english
 * @brief Calculate frame rate
 * @note This parameter indicates the calculated current buffer frame rate, which is equal to 1/(the time from completion
of the previous frame) and is different from the instantaneous frame rate read from frame grabber.
 */
    double dFrameRate;

/**
\~chinese
 * @brief IntegrationSignal1计数
 * @note 本参数指明帧完成时的IntegrationSignal1计数。
 */
 /**
\~english
 * @brief IntegrationSignal1 counter
 * @note This parameter indicates the IntegrationSignal1 count at frame completion.
 */
    unsigned uIntegrationSignal1Count;

/**
\~chinese
 * @brief 缩略图大小
 * @note 本参数指明位于有效图像数据之前的缩略图大小。
 */
 /**
\~english
 * @brief Thumbnail size
 * @note This parameter indicates thumbnail size before the valid image data.
 */
    unsigned long long uThumbnailSize;

/**
\~chinese
 * @brief 纪元时间戳（us）
 * @note 本参数指明缓冲区完成的纪元时间戳。
 */
 /**
\~english
 * @brief Epoch timestamp (us)
 * @note This parameter indicates the epoch timestamp of the buffer completion.
 */
    unsigned long long uTimestampSinceEpoch;

/**
\~chinese
 * @brief 不定长采图结束标志
 * @note 
 * 1. 当前帧为不定长采图最后一帧且图像不完整时，( uVaryFrameEnd &0xffff)表示此次不定长采图的缓冲区个数。
 * 2. 当前帧为不定长采图最后一帧且图像完整时，此时此帧的 uVaryFrameEnd 等于0，下一帧的( uVaryFrameEnd >>16)表示此次不定长采图的缓冲区个数。
 */
 /**
\~english
 * @brief Frame active end flag
 * @note 
 * 1. When the current frame is the last frame of frame active acquisition and the image is incompleted,
( uVaryFrameEnd &0xffff) represents the number of buffers used for this frame active acquisition.
 * 2. When the current frame is the last frame of frame active acquisition and the image is completed,
uVaryFrameEnd of this frame is equal to 0, and the ( uVaryFrameEnd >>16) of the next frame represents the number of buffers for
this frame active acquisition.
 */
    unsigned uVaryFrameEnd;

/**
\~chinese
 * @brief all_levels模式高低电平
 * @note 0表示低电平，1表示高电平。
 */
 /**
\~english
 * @brief all_levels mode high and low levels
 * @note 0 represents low level, 1 represents high level.
 */
    unsigned uLevel;

/**
\~chinese
 * @brief 帧开始时间戳
 * @note 仅FXGV-2/FXGV-4采集卡支持该功能. 该时间戳的数值是从相机端获取的.
 */
 /**
\~english
 * @brief Timestamp on frame start
 * @note Only FXGV-2/FXGV-4 support it. The value of timestamp is taken from camera. 
 */
    unsigned long long uFrameStartTimestamp;

/**
\~chinese
 * @brief 采集卡帧触发ID
 * @note 用于表示当前帧属于的采集卡帧触发ID. 仅用于frame active模式.
 */
 /**
\~english
 * @brief Frame trigger ID of the frame grabber
 * @note Indicates the the frame grabber frame trigger ID to which the current frame belongs. 
 Only used in frame active mode.
 */
    unsigned uFrameTriggerId;
    unsigned Reserved[48];  ///< @brief \~chinese 预留字段 \~english Reserved field
} IKAPBUFFERSTATUSEX, *PIKAPBUFFERSTATUSEX;
///@}

///@{
/**
\~chinese
 * @brief 错误码信息
 * @see IKapGetLastError
*/
 /**
\~english
 * @brief Error code information
 * @see IKapGetLastError
 */
typedef struct _ikap_error_info {
    unsigned uBoardType;   ///< @brief \~chinese 采集卡类型 \~english Frame grabber type
    unsigned uBoardIndex;  ///< @brief \~chinese 采集卡索引 \~english Frame grabber index
    unsigned uErrorCode;   ///< @brief \~chinese 错误代码 \~english Error code
} IKAPERRORINFO, *PIKAPERRORINFO;
///@}

///@{
/**
\~chinese
 * @brief CXP采集卡专有信息
 * @deprecated 使用 IKapOpenWithSpecificInfo
 * @see IKapOpenCXP
*/
 /**
\~english
 * @brief CXP frame grabber proprietary information
 * @deprecated Use IKapOpenWithSpecificInfo
 * @see IKapOpenCXP
 */
typedef struct _ikap_cxp_board_info {
    unsigned BoardIndex;    ///< @brief \~chinese 板卡索引 \~english Board Index
    unsigned MasterPort;    ///< @brief \~chinese 主端口号 \~english Main port
    unsigned SlaveCount;    ///< @brief \~chinese 从端口数量 \~english Number of slave ports
    unsigned SlavePort[7];  ///< @brief \~chinese 从端口号 \~english Slave port
    unsigned CameraIndex;   ///< @brief \~chinese 相机索引 \~english Camera index
    unsigned Topology;      ///< @brief \~chinese 相机和采集卡的拓扑连接 \~english Topological connection of camera and frame grabber
    char Reserved[248];     ///< @brief \~chinese 预留字段 \~english Reserved field
} IKAP_CXP_BOARD_INFO, *PIKAP_CXP_BOARD_INFO;
///@}

///@{
/**
\~chinese
 * @brief GVB采集卡专有信息
 * @deprecated 使用 IKapOpenWithSpecificInfo
 * @see IKapOpenGVB
*/
 /**
\~english
 * @brief GVB frame grabber proprietary information
 * @deprecated Use IKapOpenWithSpecificInfo
 * @see IKapOpenGVB
 */
typedef struct _ikap_gvb_board_info {
    unsigned BoardIndex;      ///< @brief \~chinese 板卡索引 \~english Board index
    unsigned MasterPort;      ///< @brief \~chinese 主端口号 \~english Main port
    char     MAC[32];         ///< @brief \~chinese MAC地址 \~english MAC address
    char     Ip[32];          ///< @brief \~chinese IP地址 \~english IP address
    char     SubNetMask[32];  ///< @brief \~chinese 子网掩码 \~english Subnet mask
    char     GateWay[32];     ///< @brief \~chinese 默认网关 \~english Default gateway
    unsigned SlaveCount;      ///< @brief \~chinese 从端口数量 \~english Number of slave ports
    unsigned SlavePort[3];    ///< @brief \~chinese 从端口号 \~english Slave port
    unsigned EffectPixel;     ///< @brief \~chinese 有效像素 \~english Effective pixel
    char     Reserved2[140];  ///< @brief \~chinese 预留字段 \~english Reserved field
} IKAP_GVB_BOARD_INFO, *PIKAP_GVB_BOARD_INFO;
///@}

///@{
/**
\~chinese
 * @brief 总线信息
 * @see IKapGetBufferBusAddress IKapSetBufferBusAddress
*/
 /**
\~english
 * @brief Bus information
 * @see IKapGetBufferBusAddress IKapSetBufferBusAddress
 */
typedef struct _ikap_bus_memory {
    unsigned long long BusAddress;   ///< @brief \~chinese PCIe总线物理地址 \~english PCIe bus physical address
    unsigned           Size;         ///< @brief \~chinese 内存大小 \~english Memory size
    void*              UserPointer;  ///< @brief \~chinese 用户自定义指针 \~english User-defined pointer
} IKAP_BUS_MEMORY, *PIKAP_BUS_MEMORY;
///@}

///@{
/**
\~chinese
 * @brief 中心线信息
 * @see IKapGetCentralLine
*/
 /**
\~english
 * @brief Central line information
 * @see IKapGetCentralLine
 */
typedef struct _ikap_central_info {
    unsigned char  strength;  ///< @brief \~chinese 中心线强度 \~english Central line strength
    unsigned char  block_id;  ///< @brief \~chinese 中心线区块索引 \~english Central line block index
    unsigned short pos_x;     ///< @brief \~chinese 中心线横坐标 \~english Horizontal axis of central line
    unsigned       pos_y;     ///< @brief \~chinese 中心线纵坐标 \~english Vertical axis of central line
    // int reserved[4];
} IKAP_CENTRAL_INFO, *PIKAP_CENTRAL_INFO;
///@}

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
\~chinese
 * @brief 获取当前库版本
 * @param[out] version 当前库版本
 * @return
 * - IK_RTN_OK
 * - IKStatus_InvalidParameter
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
 * - IK_RTN_OK
 * - IKStatus_InvalidParameter
 * @note 
 * | 0xff000000   | 0x00ff0000   | 0x0000ff00   | 0x000000ff     |
 * |--------------|--------------|--------------|----------------|
 * | Major version| Minor version| Patch version|Specific version| 
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetVersion(unsigned *version);

/**
\~chinese
 * @brief 获取连接到计算机上的采集卡数量
 * @param[in] resourceType 采集卡资源类型
 * @param[out] resourceCount 采集卡数量
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_InvalidParameter
 * @note 
 * 1. 对于同一类型的采集卡，其序号从0开始，并且依次递增。
 * 2. 如果没有查询类型的采集卡连接到计算机上，则返回0。
 * @see IKapGetBoardName
*/
 /**
\~english
 * @brief Get the count of frame grabbers connected to computer
 * @param[in] resourceType Type of frame grabbers
 * @param[out] resourceCount Number of frame grabbers
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_InvalidParameter
 * @note 
 * 1. For the same type of frame grabber, its index starts from 0 and increases sequentially.
 * 2. If no frame grabber of query type is connected to the computer, return 0.
 * @see IKapGetBoardName
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetBoardCount(unsigned resourceType, unsigned* resourceCount);

/**
\~chinese
 *  @brief 获取采集卡名称
 *  @param[in] resourceType 采集卡资源类型
 *  @param[in] resourceIndex 采集卡索引
 *  @param[out] resourceName 采集卡名称
 *  @param[in,out] resoureceNameSize resourceName缓冲区长度
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotFound
 * - IKStatus_BufferTooSmall
 * - IKStatus_InvalidParameter
 *  @note 
 * 1. 采集卡索引的有效范围为[0,n-1]，其中n是通过 IKapGetBoardCount 获得的resourceCount值。
 * 2. 当resourceName为空时，函数调用会失败，并且返回 IKStatus_InvalidParameter 。
 * 3. resoureceNameSize作为输入时，指明的是输入缓冲区resourceName的最大长度；作为输出时，指明的是实际获取的名字的有效长度。
 当输入缓冲区过小时，函数调用将会失败，此时会返回 IKStatus_BufferTooSmall 错误代码并在resourceNameSize中存放采集卡名称的缓冲区大小。
 *  @see IKapGetBoardCount
*/
 /**
\~english
 *  @brief Get the name of frame grabber
 *  @param[in] resourceType Type of frame grabbers
 *  @param[in] resourceIndex Frame grabber index
 *  @param[out] resourceName Name of frame grabber
 *  @param[in,out] resoureceNameSize resourceName buffer length
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotFound
 * - IKStatus_BufferTooSmall
 * - IKStatus_InvalidParameter
 *  @note 
 * 1. The valid range for frame grabber index is [0, n-1], where n is the resourceCount value obtained through
IKapGetBoardCount .
 * 2. When resourceName is empty, the function will fail and return IKStatus_InvalidParameter .
 * 3. When resoureceNameSize is used as input, it indicates the maximum length of input buffer resourceName. When
used as output, it indicates effective length of name actually get. When input buffer is too small, function will fail,
returning IKStatus_BufferTooSmall error code and storing buffer size of frame grabber name in resourceNameSize.
 *  @see IKapGetBoardCount
 */
IKAPBOARDDLL int IKAPBOARD_CC
    IKapGetBoardName(unsigned resourceType, unsigned resourceIndex, char* resourceName, unsigned* resourceNameSize);

/**
\~chinese
 * @brief 打开采集卡
 * @param[in] resourceType 采集卡资源类型
 * @param[in] resourceIndex 采集卡索引
 * @return
 * - Handle: 采集卡设备句柄
 * - INVALID_HANDLE_VALUE (-1): 调用失败
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note 本函数获取指定类型和序号的采集卡设备句柄，同时初始化采集卡。如果多次打开同一采集卡，只有第一次调用会被执行，后续调用会返回打开失败错误。
该接口仅使用于CL采集卡.
 * @see IKapClose
*/
 /**
\~english
 * @brief Open frame grabber
 * @param[in] resourceType Frame grabber resource type
 * @param[in] resourceIndex Frame grabber index
 * @return
 * - Handle: Frame grabber device handle
 * - INVALID_HANDLE_VALUE (-1): Call failed
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note This function gets the handle of frame grabber device of the specified type and, index, and initializes frame
grabber at the same time. If the same frame grabber is opened multiple times, only the first call will be executed, and
subsequent calls will return an open failure error.
Only applicable to CL frame grabbers. 
 * @see IKapClose
 */
IKAPBOARDDLL HANDLE IKAPBOARD_CC IKapOpen(unsigned resourceType, unsigned resourceIndex);

/**
\~chinese
 * @brief 打开CXP采集卡
 * @param[in] resourceType 采集卡资源类型
 * @param[in] resourceIndex 采集卡索引
 * @param[in] info CoaXPress采集卡信息
 * @return
 * - Handle: 采集卡设备句柄
 * - INVALID_HANDLE_VALUE (-1): 调用失败
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note 本函数获取指定类型和序号的采集卡设备句柄，同时初始化采集卡。如果多次打开同一采集卡，只有第一次调用会被执行，后续调用会返回打开失败错误。
 * @deprecated 使用 IKapOpenWithSpecificInfo
 * @see IKapClose IKAP_CXP_BOARD_INFO
*/
 /**
\~english
 * @brief Open CXP frame grabber
 * @param[in] resourceType Frame grabber resource type
 * @param[in] resourceIndex Frame grabber index
 * @param[in] info CoaXPress frame grabber information
 * @return
 * - Handle: Frame grabber device handle
 * - INVALID_HANDLE_VALUE (-1): Call failed
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note This function gets the handle of frame grabber device of the specified type and, index, and initializes frame
grabber at the same time. If the same frame grabber is opened multiple times, only the first call will be executed, and
subsequent calls will return an open failure error.
 * @deprecated Use IKapOpenWithSpecificInfo
 * @see IKapClose IKAP_CXP_BOARD_INFO
 */
IKAPBOARDDLL HANDLE IKAPBOARD_CC IKapOpenCXP(unsigned resourceType, unsigned resourceIndex, IKAP_CXP_BOARD_INFO info);

/**
\~chinese
 * @brief 打开GVB采集卡
 * @param[in] resourceType 采集卡资源类型
 * @param[in] resourceIndex 采集卡索引
 * @param[in] info GVB采集卡信息
 * @return
 * - Handle: 采集卡设备句柄
 * - INVALID_HANDLE_VALUE (-1): 调用失败
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note 本函数获取指定类型和序号的采集卡设备句柄，同时初始化采集卡。如果多次打开同一采集卡，只有第一次调用会被执行，后续调用会返回打开失败错误。
 * @deprecated 使用 IKapOpenWithSpecificInfo
 * @see IKapClose IKAP_GVB_BOARD_INFO
*/
 /**
\~english
 * @brief Open GVB frame grabber
 * @param[in] resourceType Frame grabber resource type
 * @param[in] resourceIndex Frame grabber index
 * @param[in] info GVB frame grabber information
 * @return
 * - Handle: Frame grabber device handle
 * - INVALID_HANDLE_VALUE (-1): Call failed
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note This function gets the handle of frame grabber device of the specified type and, index, and initializes frame
grabber at the same time. If the same frame grabber is opened multiple times, only the first call will be executed, and
subsequent calls will return an open failure error.
 * @deprecated Use IKapOpenWithSpecificInfo
 * @see IKapClose IKAP_GVB_BOARD_INFO
 */
IKAPBOARDDLL HANDLE IKAPBOARD_CC IKapOpenGVB(unsigned resourceType, unsigned resourceIndex, IKAP_GVB_BOARD_INFO info);

/**
\~chinese
 * @brief 使用IKapC获取的相机类型特定信息打开采集卡
 * @param[in] info IKapC获取的相机类型特定信息
 * @return
 * - Handle: 采集卡设备句柄
 * - INVALID_HANDLE_VALUE (-1): 调用失败
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note 本函数使用IKapC获取的相机类型特定信息打开与此相机连接的采集卡，同时初始化采集卡。如果多次打开同一采集卡，只有第一次调用会被执行，后续调用会返回打开失败错误。
 可以用来获取相机特定信息的函数有: ItkManGetCLDeviceInfo, ItkManGetCXPDeviceInfo, ItkManGetGVBDeviceInfo
 * @see IKapClose
*/
 /**
\~english
 * @brief Open a frame grabber with IKapC camera specific info.
 *  @param[in] info The camera specific info get from IKapC
 * @return
 * - Handle: Frame grabber device handle
 * - INVALID_HANDLE_VALUE (-1): Call failed
 * - IKStatus_BoardNotFound
 * - IKStatus_AllocMemoryFail
 * - IKStatus_Mutex_Locked
 * - IKStatus_Invalid_Mutex
 * - IKStatus_WinError
 * - IKStatus_InvalidParameter
 * @note This function gets the handle of the frame grabber device by camera's specific info, and initializes frame grabber
at the same time. If the same frame grabber is opened multiple times, only the first call will be executed, and subsequent
calls will return an open failure error.
 * @see IKapClose
 */
IKAPBOARDDLL HANDLE IKAPBOARD_CC IKapOpenWithSpecificInfo(void* info);
/**
\~chinese
 * @brief 关闭采集卡
 * @param[in] hDev 采集卡句柄
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotFound
 * - IKStatus_WinError
 * - IKStatus_Invalid_Handle
 * @note 如果采集卡设备已经关闭或者没有打开，该函数不会重复关闭设备。
 * @see IKapOpen
*/
 /**
\~english
 * @brief Close frame grabber
 * @param[in] hDev Frame grabber handle
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotFound
 * - IKStatus_WinError
 * - IKStatus_Invalid_Handle
 * @note If the frame grabber device is already turned off or not turned on, this function will not repeatedly turn off the
device.
 * @see IKapOpen
 */
IKAPBOARDDLL int IKAPBOARD_CC    IKapClose(HANDLE hDev);

/**
\~chinese
 * @brief 导入采集卡配置文件
 * @param[in] hDev 采集卡句柄
 * @param[in] lpFileName 配置文件路径，以.vlcf为后缀
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_ConfigFilePathInvalid
 * - IKStatus_ConfigParameterInvalid
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 本函数读取由lpFileName指定路径的文件，该文件是一个以.vlcf为后缀的采集卡配置文件，可以通过IKapCViewer软件中的保存功能生成，也可以由用户通过任意文档编辑工具手动修改生成。
 * 2. 本函数必须指明配置文件的全部路径。
 * 3. 本函数同时会校验导入的配置文件参数，如果设置参数错误则函数返回 IK_RTN_ERROR 。
 * @see IKapSaveConfigurationToFile
*/
 /**
\~english
 * @brief Load frame grabber configuration file
 * @param[in] hDev Frame grabber handle
 * @param[in] lpFileName Configuration file path with .vlcf suffix
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_ConfigFilePathInvalid
 * - IKStatus_ConfigParameterInvalid
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. This function reads a file from the path specified by lpFileName, which is a frame grabber configuration file
with .vlcf suffix. It can be generated through the save function in IKapCViewer software or manually modified by users with
any document editing tool.
 * 2. This function must indicate the full path of configuration file.
 * 3. This function will also verify the parameters of the loaded configuration file. If parameters are set
incorrectly, the function will return IK_RTN_ERROR .
 * @see IKapSaveConfigurationToFile
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapLoadConfigurationFromFile(HANDLE hDev, char* lpFileName);

/**
\~chinese
 * @brief 保存采集卡配置文件
 * @param[in] hDev 采集卡句柄
 * @param[in] lpFileName 配置文件路径，以.vlcf为后缀
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_ConfigFilePathInvalid
 * - IKStatus_Invalid_Handle
 * @note 本函数必须指明配置文件的全部路径。
 * @see IKapLoadConfigurationFromFile
*/
 /**
\~english
 * @brief Save frame grabber configuration file
 * @param[in] hDev Frame grabber handle
 * @param[in] lpFileName Configuration file path with .vlcf suffix
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_ConfigFilePathInvalid
 * - IKStatus_Invalid_Handle
 * @note This function must indicate the full path of configuration file.
 * @see IKapLoadConfigurationFromFile
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapSaveConfigurationToFile(HANDLE hDev, char* lpFileName);

/**
\~chinese
 * @brief 获取采集卡参数
 * @param[in] hDev 采集卡句柄
 * @param[in] uType 参数类型
 * @param[out] npValue 参数值
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @deprecated 使用 IKapGetInfo64
 * @see IKapSetInfo
*/
 /**
\~english
 * @brief Get frame grabber parameters
 * @param[in] hDev Frame grabber handle
 * @param[in] uType Parameter type
 * @param[out] npValue Parameter value
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @deprecated Use IKapGetInfo64
 * @see IKapSetInfo
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetInfo(HANDLE hDev, unsigned int uType, int* npValue);

/**
\~chinese
 * @brief 获取采集卡的参数
 * @param[in] hDev: 采集卡句柄
 * @param[in] uType: 参数类型
 * @param[out] npValue: 参数值
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @see IKapSetInfo IKapSetInfo64 IKapGetInfo
 */
/**
\~english
 * @brief Get frame grabber parameters
 * @param[in] hDev: Frame grabber handle
 * @param[in] uType: Parameter type
 * @param[out] npValue: Parameter value
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @see IKapSetInfo IKapSetInfo64 IKapGetInfo
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetInfo64(HANDLE hDev, unsigned int uType, int64_t* npValue);

/**
\~chinese
 * @brief 设置采集卡参数
 * @param[in] hDev 采集卡句柄
 * @param[in] uType 参数类型
 * @param[in] nValue 参数值
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note 不要在图像传输过程中调用该函数，否则可能导致系统错误。
 * @deprecated 使用 IKapSetInfo64
 * @see IKapGetInfo
*/
 /**
\~english
 * @brief Set frame grabber parameters
 * @param[in] hDev Frame grabber handle
 * @param[in] uType Parameter type
 * @param[in] nValue Parameter value
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note Do not call this function during image transmission, otherwise system errors may result.
 * @deprecated Use IKapSetInfo64
 * @see IKapGetInfo
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapSetInfo(HANDLE hDev, unsigned int uType, int nValue);

/**
\~chinese
 * @brief 设置采集卡参数
 * @param[in] hDev: 采集卡句柄
 * @param[in] uType: 参数类型
 * @param[in] nValue: 参数值
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note 不要在图像传输过程中调用该函数，否则可能导致系统错误。
 * @see IKapGetInfo IKapGetInfo64 IKapSetInfo
 */
/**
\~english
 * @brief Set frame grabber parameters
 * @param[in] hDev: Frame grabber handle
 * @param[in] uType: Parameter type
 * @param[in] nValue: Parameter value
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note Do not call this function during image transmission, otherwise system errors may result.
 * @see IKapGetInfo IKapGetInfo64 IKapSetInfo
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapSetInfo64(HANDLE hDev, unsigned int uType, int64_t nValue);

/**
\~chinese
 * @brief 注册回调事件
 * @param[in] hDev 采集卡句柄
 * @param[in] uEventType 回调事件类型
 * @param[in] fEventFunc 注册的回调事件指针
 * @param[in] pContext 通用参数
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 用户可以自定义在回调事件中的功能，比如保存图像数据，但是不要在回调事件中进行过于耗时的操作，否则可能导致缓冲区图像数据被新采集的数据覆盖。
 * 2. 如果多次设置回调事件，仅保证最新设置的回调事件会被调用。
 * 3. pContext是用户申请内存地址的指针，如果在已经释放该内存的情况下，在回调事件中继续使用该函数，可能导致内存越界错误。
 * 4. 采集图像结束后请调用 IKapUnRegisterCallback 清除回调事件。
 * 5. 支持边沿检测的触发信号如下所示：
 * | 信号索引 | 信号类型      |
 * |----------|---------------|
 * | 0x01     | 内部信号      |
 * | 0x02     | 通用输入信号1 |
 * | 0x04     | 通用输入信号2 |
 * | 0x08     | 编码器信号A相 |
 * | 0x10     | 编码器信号B相 |
 * | 0x20     | 板间同步信号1 |
 * | 0x40     | 积分信号1     |
 * | 0x80     | 积分信号2     |
 * @see IKapUnRegisterCallback
*/
/**
\~english
 * @brief Register callback event
 * @param[in] hDev Frame grabber handle
 * @param[in] uEventType Callback event type
 * @param[in] fEventFunc Registered callback event pointer
 * @param[in] pContext General parameters
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. Users can customize functions in callback events, such as saving image data, but do not perform overly
time-consuming operations in callback events, otherwise it may cause buffer image data to be overwritten by newly collected data.
 * 2. If multiple callback events are set, only the latest set callback event will be called.
 * 3. pContext is a pointer to memory address requested by users. Continuing to use this function in callback
events after memory has already been released may result in a memory out of bounds error.
 * 4. When image acquisition is completed, please call IKapUnRegisterCallback to clear callback event.
 * 5. The trigger signals that support edge detection are as follows:
 * | Signal index | Signal type               |
 * |--------------|---------------------------|
 * | 0x01         | Intern signal             |
 * | 0x02         | General input signal1     |
 * | 0x04         | General input signal2     |
 * | 0x08         | Encoder signal A phase    |
 * | 0x10         | Encoder signal B phase    |
 * | 0x20         | Board synchronous signal1 |
 * | 0x40         | Integration signal1       |
 * | 0x80         | Integration signal2       |
 * @see IKapUnRegisterCallback
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapRegisterCallback(HANDLE hDev, unsigned uEventType, HookFnPtr fEventFunc, void* pContext);

/**
\~chinese
 * @brief 清除回调事件
 * @param[in] hDev 采集卡句柄
 * @param[in] uEventType 回调事件类型
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 请在停止采集图像后清除所有回调事件。
 * @see IKapRegisterCallback
*/
 /**
\~english
 * @brief Clear callback event
 * @param[in] hDev Frame grabber handle
 * @param[in] uEventType Callback event type
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note Please clear all callback events after stopping image acquisition.
 * @see IKapRegisterCallback
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapUnRegisterCallback(HANDLE hDev, unsigned int uEventType);

/**
\~chinese
 * @brief 开始采集图像
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameCount 采集图像数量
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 调用该函数，采集卡将根据当前导入的配置文件信息采集图像序列或者连续采集图像。采集图像数量对应关系如下所示：
 * | 数值         | 图像帧数 |
 * |--------------|----------|
 * | 0            | 连续采集 |
 * | 1            | 单帧采集 |
 * | 2~2147483647 | 多帧采集 |
 * 2. 请确保在采集图像前，采集卡设备已经打开，配置参数已经导入。
 * 3. 对于较大尺寸的图像，可能需要较长时间完成一次图像采集。可以通过增长采集时间保证一帧完整图像的采集。
 * 4. 配置参数不合理、相机状态异常都会导致采集失败。
 * 5. 每次调用该函数都会清空原有的图像缓冲数据。
 * 6. 如果单帧采集图像，本函数仅会采集一帧图像，如果 IKapStopGrab 在尚未采集完成前调用，则会终止当前的采集。
 * 7. 如果多帧采集图像，本函数将会采集由nFrameCount指定的图像序列，如果 IKapStopGrab 在尚未采集完成前调用，则会终止当前的采集。
 * 8. 如果连续采集图像，本函数将会连续采集图像直到用户调用 IKapStopGrab 。
 * 9. 采集卡相关函数不会改变相机的采图状态. 对于CXP/SFP/GV/XGV相机, 建议在调用此函数前向相机发送AcquisitionStop命令以停止相机的采集,
在调用此函数后, 向相机发送AcquisitionStart以重新开始相机的采集. 否则需要确保在此函数执行期间, 相机不会向采集卡发送图像数据. 
 * 10. 当调用该函数时，IKapBoard始终从缓冲区的第一帧开始写入区域并清空以前写入的数据。例如设置缓冲区个数为4，采集到的图像将按照如下顺序排列：
 * * 单帧采集时(nFrameCount=1)：1→1→1→1→…
 * * 多帧采集时(nFrameCount=2)：(1→2)→(1→2)→…
 * * 多帧采集时(nFrameCount=6)：(1→2→3→4→1→2)→(1→2→3→4→1→2)→…
 * * 连续采集时(nFrameCount=0)：(1→2→3→4)→(1→2→3→4)→…
 * @see IKapStopGrab
*/
 /**
\~english
 * @brief Start grabbing images
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameCount Image number
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. Call this function, and frame grabber will grab image sequences or continuously based on the current loaded
configuration file information. The corresponding relationship between the number of grabbed images is as follows:
 * | Value        | Image number             |
 * |--------------|--------------------------|
 * | 0            | Continuous acquisition   |
 * | 1            | Single frame acquisition |
 * | 2~2147483647 | Multi frame acquisition  |
 * 2. Please ensure that frame grabber device is turned on and the configuration parameters have been loaded
before acquisition.
 * 3. For large sized images, it may take a longer time to complete an image acquisition. It is possible to
ensure the acquisition of a complete image frame by increasing acquisition time.
 * 4. Unreasonable configuration parameters and abnormal camera status can lead to acquisition failure.
 * 5. Every time this function is called, the existing image buffer data will be cleared.
 * 6. If an image is grabbed in a single frame, this function will only grab one frame of image. If IKapStopGrab
is called before the acquisition is completed, the current acquisition will be terminated.
 * 7. If multiple frames are grabbed, this function will grab the image sequence specified by nFrameCount. If
IKapStopGrab is called before the acquisition is completed, the current acquisition will be terminated.
 * 8. If images are grabbed continuously, this function will continue to grab images until users call
IKapStopGrab .
 * 9. The frame grabber-related functions do not modify the camera's acquisition state. For CXP/SFP/GV/XGV cameras, 
it is recommended to send an AcquisitionStop command to the camera to stop image acquisition before calling this function, 
and then send an AcquisitionStart command after calling the function to restart image acquisition. Otherwise, 
ensure that the camera does not send image data to the frame grabber during the execution of this function.
 * 10. When calling this function, IKapBoard always writes to the area from the first frame of buffer and
clears previously written data. For example, if the number of buffer zones is set to 4, the collected images will be arranged
in the following order:
 * * Single frame acquisition(nFrameCount=1)：1→1→1→1→…
 * * Multi frame acquisition(nFrameCount=2)：(1→2)→(1→2)→…
 * * Multi frame acquisition(nFrameCount=6)：(1→2→3→4→1→2)→(1→2→3→4→1→2)→…
 * * Continuous acquisition(nFrameCount=0)：(1→2→3→4)→(1→2→3→4)→…
 * @see IKapStopGrab
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapStartGrab(HANDLE hDev, int nFrameCount);

/**
\~chinese
 * @brief 停止采集图像
 * @param[in] hDev 采集卡句柄
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 如果当前没有进行图像采集，则该函数不会产生作用。
 * @see IKapStartGrab
*/
 /**
\~english
 * @brief Stop grabbing images
 * @param[in] hDev Frame grabber handle
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note If no image acquisition is currently being performed, this function will not take effect.
 * @see IKapStartGrab
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapStopGrab(HANDLE hDev);

/**
\~chinese
 * @brief 等待采集结束或采集图像超时
 * @param[in] hDev 采集卡句柄
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 用户以非阻塞模式采集图像时，调用该函数会等待异步采集操作结束或者采集超时。 只有在序列采集的情况下才会等待, 在连续采集时将直接返回成功.
 * @see IKapStartGrab
*/
 /**
\~english
 * @brief Waiting for acquisition to end or image acquisition timeout
 * @param[in] hDev Frame grabber handle
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note When users grab images in non blocking mode, calling this function will wait for the asynchronous acquisition
operation to end or for the acquisition to timeout. It Wait only during sequential acquisition; in continuous acquisition, 
it will return IK_RTN_OK immediately.
 * @see IKapStartGrab
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapWaitGrab(HANDLE hDev);

/**
\~chinese
 * @brief 清空当前缓冲区和采集状态
 * @param[in] hDev 采集卡句柄
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 该函数不会停止正在进行的采集操作，仅清空当前采集的缓冲区并复位采集状态。
采集卡相关函数不会改变相机的采图状态, 对于CXP/SFP/GV/XGV相机, 建议在调用此函数前向相机发送AcquisitionStop命令以停止相机的采集,
在调用此函数后, 向相机发送AcquisitionStart以重新开始相机的采集. 否则需要确保在此函数执行期间, 相机不会向采集卡发送图像数据. 
 * @see IKapStartGrab
*/
 /**
\~english
 * @brief Clear the current buffer and acquisition status
 * @param[in] hDev Frame grabber handle
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note This function does not stop the ongoing acquisition operation, only clears the current acquisition buffer and resets
the acquisition state. 
The frame grabber-related functions do not modify the camera's acquisition state. For CXP/SFP/GV/XGV cameras, 
it is recommended to send an AcquisitionStop command to the camera to stop image acquisition before calling this function, 
and then send an AcquisitionStart command after calling the function to restart image acquisition. Otherwise, 
ensure that the camera does not send image data to the frame grabber during the execution of this function.
 * @see IKapStartGrab
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapClearGrab(HANDLE hDev);

/**
\~chinese
 * @brief 等待一帧图像完成并获取这帧图像在采集卡缓冲区的序号
 * @param[in] hDev 采集卡句柄
 * @param[out] pFrameIndex 完成的图像在缓冲区中的序号
 * @param[in] timeout_ms 超时时间(ms), 如果超时时间(ms) < 0,则表示超时时间为无限
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_Grab_Pending  采集尚未开始
 * - IKStatus_TimeOut       等待一帧图像完成超时
 * - IKStatus_Grab_Abort    等待时采集被终止
 * @note 
 * 1. 在获取图像并处理完成后, 需要调用IKapReleaseBuffer将缓冲区释放。
 * 2. 当内部缓冲区中已经有已完成的图像时, 该接口会立即返回。
 * 3. 默认情况下该接口会优先返回最早到来的图像, 直到内部缓冲区没有已经完成的图像。
 * @see IKapStartGrab IKapSetInfo
*/
 /**
\~english
 * @brief Wait one frame ready and get the index of it
 * @param[in] hDev Frame grabber handle
 * @param[out] pFrameIndex The index of the ready frame
 * @param[in] timeout_ms timeout(ms). If it timeout(ms) < 0, timeout is infinite.
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_Grab_Pending  grab is not start
 * - IKStatus_TimeOut       wait one ready frame timeout
 * - IKStatus_Grab_Abort    wait abort because grab is stop
 * @note 
 * 1. After IKapWaitOneFrameReady and processing image, need use IKapReleaseBuffer to release buffer.
 * 2. This api return instantly if there are one or more ready frames in board internal buffers.
 * 3. This api return the earleist ready frame on default mode.
 * @see IKapStartGrab IKapSetInfo
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapWaitOneFrameReady(HANDLE hDev, int* pFrameIndex, long long timeout_ms);

/**
\~chinese
 * @brief 获取缓冲区地址
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[out] pAddress 缓冲区地址
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 当图像数据到缓冲区后，如果需要访问采集的内容，则调用该函数获取缓冲区首地址。注意访问地址的界限不能超过(pAddress+ IKP_FRAME_SIZE )。
 * 2. 像素排列方式从左上角到右下角依次递增。
 * 3. 对于数据格式是8bit的灰度或Bayer图像，每个像素的每个通道仅需要一个字节的缓存区域保存。对于数据格式大于8bit的灰度或者Bayer图像，每个像素的每个通道需要两个字节的缓存区域保存。
 * 4. 对于彩色图像，各通道数值按照如下形式从左到右、从上到下进行存储：
 * |Column1  | ... |ColumnN  |
 * |---------|-----|---------|
 * | BGR(C)  | ... |BGR(C)   |
 * | ...     | ... |...      |
 * | BGR(C)  | ... |BGR(C)   |
 * 5. 像素的有效数据采用little-endian方式排列。
 * @see IKapGetBufferAddress
*/
 /**
\~english
 * @brief Get buffer address
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[out] pAddress Buffer address
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. When the image data reaches the buffer, if it is necessary to access the collected content, call this function to
get the first address of the buffer. Note that the boundary of the access address cannot exceed (pAddress+ IKP_FRAME_SIZE )。
 * 2. The pixel arrangement gradually increases from the top left corner to the bottom right corner.
 * 3. For 8-bit grayscale or Bayer images, each pixel and channel only require one byte of cache area for
storage. For grayscale or Bayer images with data formats greater than 8 bits, each pixel and channel require a buffer area of
two bytes for storage.
 * 4. For color images, the values of each channel are stored in the following format from left to right and top
to bottom:
 * |Column1  | ... |ColumnN  |
 * |---------|-----|---------|
 * | BGR(C)  | ... |BGR(C)   |
 * | ...     | ... |...      |
 * | BGR(C)  | ... |BGR(C)   |
 * 5. The effective data of pixels is arranged in a small endian manner.
 * @see IKapGetBufferAddress
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetBufferAddress(HANDLE hDev, int nFrameNum, void** pAddress);

/**
\~chinese
 * @brief 设置缓冲区地址
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[in] pAddress 缓冲区地址
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note 设置图像数据写入的首地址，当采集卡采集到图像数据后将会直接写入该用户缓冲区内。该函数要在设置缓冲区个数后调用.
 * @see IKapGetBufferAddress
*/
 /**
\~english
 * @brief Set buffer address
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[in] pAddress Buffer address
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note Set the first address for writing image data, and when frame grabber grabs image data, it will be directly written
into users’ buffer. This function must be called after setting the buffer count.
 * @see IKapGetBufferAddress
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapSetBufferAddress(HANDLE hDev, int nFrameNum, void* pAddress);

/**
\~chinese
 * @brief 设置GPU-Direct-For-Video的目标缓冲区地址 
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[in] pAddress GPU-Direct-For-Video的目标缓冲区地址
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note 设置图像数据写入的首地址，当采集卡采集到图像数据后将会通过GPU-Direct-For-Video写入该GPU缓冲区内。
 * @see IKapGetBufferAddress
*/
 /**
\~english
 * @brief Set GPU-Direct-For-Video buffer address
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[in] pAddress  GPU-Direct-For-Video buffer address
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_InvalidParameter
 * - IKStatus_Invalid_Handle
 * @note Set the first address for writing image data, and when frame grabber grabs image data, it will be directly written
into this buffer by GPU-Direct-For-Video.
 * @see IKapGetBufferAddress
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapSetDvpBufferAddress(HANDLE hDev, int nFrameNum, void* pAddress);

/**
\~chinese
 * @brief 获取缓冲区状态
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[out] pIKapBufferStatus 缓冲区状态
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @see IKapGetBufferStatusEx IKapGetBufferAddress IKAPBUFFERSTATUS
*/
 /**
\~english
 * @brief Get buffer status
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[out] pIKapBufferStatus Buffer status
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @see IKapGetBufferStatusEx IKapGetBufferAddress IKAPBUFFERSTATUS
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetBufferStatus(HANDLE hDev, int nFrameNum, PIKAPBUFFERSTATUS pIKapBufferStatus);

/**
\~chinese
 * @brief 获取缓冲区详细状态
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[out] pIKapBufferStatusEx 缓冲区状态
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @see IKapGetBufferAddress IKAPBUFFERSTATUSEX
*/
 /**
\~english
 * @brief Get detailed buffer status
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[out] pIKapBufferStatusEx Buffer status
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @see IKapGetBufferAddress IKAPBUFFERSTATUSEX
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetBufferStatusEx(HANDLE hDev, int nFrameNum, PIKAPBUFFERSTATUSEX pIKapBufferStatusEx);

/**
\~chinese
 * @brief 释放缓冲区
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 调用该函数来释放状态为满的缓冲区，然后该缓冲区可以用来写入新到来的图像数据。本函数在连续采集且关闭自动清理机制时生效。
 * 2. IKapBoard默认自动管理缓冲区状态，因此很少会直接调用该函数。如果希望手动管理缓冲区状态，必须先调用 IKapSetInfo ( HANDLE , 
IKP_FRAME_AUTO_CLEAR , 0)来关闭IKapBoard的缓存自动清空机制。
 * @see IKapGetBufferAddress
*/
 /**
\~english
 * @brief Release buffer
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. Call this function to release buffer with full state, which can then be used to write new image data. This
function takes effect when continuous acquisition is performed and the automatic cleaning mechanism is turned off.
 * 2. IKapBoard automatically manages buffer state by default, so this function is rarely called directly. If you
want to manually manage the buffer state, you must first call IKapSetInfo (HANDLE , IKP_FRAME_AUTO_CLEAR , 0) to turn off the
cache auto clear mechanism of IKapBoard.
 * @see IKapGetBufferAddress
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapReleaseBuffer(HANDLE hDev, int nFrameNum);

/**
\~chinese
 * @brief 获取相机状态
 * @param[in] hDev 采集卡句柄
 * @param[in] npValue 相机状态
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 如果返回0或相机状态npValue为0xFFFFFFFF，则采集卡获取相机信号失败，用户可以在间隔一段时间后（如100ms）重新调用该函数获取相机状态。
 * 2. 相机状态描述如下：
 * * Camera Link相机：
 * | 位数  | 描述                                                 |
 * |-------|------------------------------------------------------|
 * | [7:3] | 预留                                                 |
 * | [2]   | 行有效状态位，1表示检测到行信号，0表示未检测到行信号 |
 * | [1]   | 帧有效状态位，1表示检测到帧信号，0表示未检测到帧信号 |
 * | [0]   | 时钟有效状态位，1表示检测到时钟，0表示未检测到时钟   |
 * * CoaXPress相机：
 * | 位数  | 描述                                                                    |
 * |-------|-------------------------------------------------------------------------|
 * | [4]   | 相机连接状态位，1表示连接到相机，0表示未连接到相机                      |
 * | [3:0] | 链路速度状态位，0表示1.25G，1表示2.5G，2表示3.125G，3表示5G，4表示6.25G |
 * @see IKapReleaseBuffer
*/
 /**
\~english
 * @brief Get camera status
 * @param[in] hDev Frame grabber handle
 * @param[in] npValue Camera status
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. If 0 is returned or camera status npValue is 0xFFFFFFFF, frame grabber fails to obtain camera signal. Users can
call the function again after a certain interval (such as 100ms) to get the camera status.
 * 2. The camera status is described as follows:
 * * Camera Link camera：
 * | Bit   | Description                                                                                          |
 * |-------|------------------------------------------------------------------------------------------------------|
 * | [7:3] | Reserved                                                                                             |
 * | [2]   | Line valid status bit, 1 indicates detection of line signal, 0 indicates no detection of line signal |
 * | [1]   | Frame valid status bit, 1 indicates detected frame signal, 0 indicates not detected frame signal     |
 * | [0]   | Clock valid status bit, 1 indicates clock detected, 0 indicates clock not detected                   |
 * * CoaXPress camera：
 * | Bit   | Description                                                                                                            |
 * |-------|------------------------------------------------------------------------------------------------------------------------|
 * | [4]   | Camera connection status bit, 1 indicates connected to camera, 0 indicates not connected to camera                     |
 * | [3:0] | Link speed status bit, 0 represents 1.25G, 1 represents 2.5G, 2 represents 3.125G, 3 represents 5G, 4 represents 6.25G |
 * @see IKapReleaseBuffer
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetCameraStatus(HANDLE hDev, int* npValue);

/**
\~chinese
 * @brief 获取错误码
 * @param[in] pIKapErrorInfo 指向错误信息结构体的指针
 * @param[in] bErrorReset 是否重置错误信息结构体中的值
 * @note 当函数执行过程中出现错误时，会产生相应的错误信息码，调用该函数可以获取相应详细信息。
 注意如果产生多个错误，则始终保留最近一次产生的错误信息。如果bErrorReset设置为1，则会重置错误信息到初始状态。
*/
 /**
\~english
 * @brief Get error code
 * @param[in] pIKapErrorInfo Pointer to error message structure
 * @param[in] bErrorReset Whether to reset values in error message structure
 * @note When an error occurs during the execution of function, a corresponding error message code will be generated, and
calling the function can get the corresponding detailed information. Note that if multiple errors occur, always keep the most
recent error message. If bError Reset is set to 1, the error message will be reset to its initial state.
 */
IKAPBOARDDLL void IKAPBOARD_CC IKapGetLastError(PIKAPERRORINFO pIKapErrorInfo, bool bErrorReset);

/**
\~chinese
 * @brief 获取采集卡串口号
 * @param[in] hDev 采集卡句柄
 * @param[in] npPortNumber 串口号
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_BoardNotBindingCOM
 * - IKStatus_Invalid_Handle
 * @note 如果采集卡设备尚未绑定串口，用户可以使用IKComManager.exe工具为采集卡配置串口。
*/
 /**
\~english
 * @brief Get serial port number of frame grabber
 * @param[in] hDev Frame grabber handle
 * @param[in] npPortNumber Serial port number
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_BoardNotBindingCOM
 * - IKStatus_Invalid_Handle
 * @note If the frame grabber device has not been bound to the serial port, users can use IKComManager.exe tool to configure
serial port for frame grabber.
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetSerialPort(HANDLE hDev, int* npPortNumber);

/**
\~chinese
 * @brief 获取采集帧率
 * @param[in] hDev 采集卡句柄
 * @param[in] dpFrameRate 采集帧率
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 单帧采集图像时无法获得相机输出帧率。连续采集过程中，在采集初始阶段(0~10帧内)可能会出现帧率为0的情况。
*/
 /**
\~english
 * @brief Get frame rate of data acquisition
 * @param[in] hDev Frame grabber handle
 * @param[in] dpFrameRate Acquisition frame rate
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note When acquisition images in a single frame, the camera output frame rate cannot be get. During the continuous
acquisition process, there may be a situation where the frame rate is 0 in the initial stage of acquisition (within 0-10
frames).
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapGetFrameRate(HANDLE hDev, double* dpFrameRate);

/**
\~chinese
 * @brief 保存缓冲区到指定位置
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[in] fileName 文件路径
 * @param[in] nFlag 图像标志
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @see IKapLoadBuffer IKP_SAVE_BUFFER_RED_BLUE_SWAP
*/
 /**
\~english
 * @brief Save buffer to the specified location
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[in] fileName File path
 * @param[in] nFlag Image flag
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @see IKapLoadBuffer IKP_SAVE_BUFFER_RED_BLUE_SWAP
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapSaveBuffer(HANDLE hDev, int nFrameNum, const char* fileName, int nFlag);

/**
\~chinese
 * @brief 从指定位置加载缓冲区
 * @param[in] hDev 采集卡句柄
 * @param[in] nFrameNum 缓冲区索引，有效范围[0~ IKP_FRAME_COUNT -1]
 * @param[in] fileName 文件路径
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @see IKapSaveBuffer
*/
 /**
\~english
 * @brief Load buffer from specified location
 * @param[in] hDev Frame grabber handle
 * @param[in] nFrameNum Buffer index, valid range is [0~ IKP_FRAME_COUNT -1]
 * @param[in] fileName File path
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @see IKapSaveBuffer
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapLoadBuffer(HANDLE hDev, int nFrameNum, const char* fileName);

/**
\~chinese
 * @brief 准备采集图像
 * @param[in] hDev 采集卡句柄
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 该函数可能会触发采集相关回调事件，所以最好在注册采集相关回调事件前使用。
 * 2. 该函数用于 IKP_CL_LONG_CABLE_WORKING_MODE 使能的场景，需要在上电后的第一次 IKapStartGrab 之前使用，此时相机需要有非相同图像数据上传。
 * @see IKapStartGrab
*/
 /**
\~english
 * @brief Prepare to grab images
 * @param[in] hDev Frame grabber handle
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. This function may trigger acquisition related callback events, so it is best to use it before registering
acquisition related callback events.
 * 2. This function is used in scenes where IKP_CL_LONG_CABLE_WORKING_MODE is enabled and needs to be used before
the first IKapStartGrab after power on, when the camera needs to upload non identical image data.
 * @see IKapStartGrab
 */
IKAPBOARDDLL int IKAPBOARD_CC IKapPrepareGrab(HANDLE hDev);

/**
\~chinese
 * @brief 支持中心线的相机获取中心线
 * @param[in] hDev 采集卡句柄
 * @param[out] id 帧索引，从0开始计数
 * @param[in] info 线中心数据
 * @param[in,out] cnt info数组长度
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. 线中心为不定长数组，数组中的每个元素都代表一个线中心数据。每帧图像数据都会输出若干线中心，由于图像内容的变化，每帧图像线中心的数量是不固定的，每帧图像最少会输出0个线中心数据。
 * 2. cnt作为输入，表示info数组的最大长度；作为输出，表示当前线中心数据的个数。
 * @see IKapStartGrab IKAP_CENTRAL_INFO
*/
 /**
\~english
 * @brief Camera that supports central line gets central line
 * @param[in] hDev Frame grabber handle
 * @param[out] id Frame index, counting from 0
 * @param[in] info Central line data
 * @param[in,out] cnt info array length
 * @return
 * - IK_RTN_OK
 * - IK_RTN_ERROR
 * - IKStatus_BoardNotOpen
 * - IKStatus_TimeOut
 * - IKStatus_Invalid_Handle
 * @note 
 * 1. The central line is an indefinite length array, where each element represents a central line data. Each frame of
image data will output several central line. Due to changes in image content, the number of central line per frame is not
fixed, and at least 0 line center data will be output per frame of image.
 * 2. cnt as input, representing the maximum length of the info array. As output, it represents the number of data
points at the center of the current line.
 * @see IKapStartGrab IKAP_CENTRAL_INFO
 */
IKAPBOARDDLL int IKapGetCentralLine(HANDLE hDev, unsigned* id, IKAP_CENTRAL_INFO* info, unsigned* cnt);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif

// clang-format on
