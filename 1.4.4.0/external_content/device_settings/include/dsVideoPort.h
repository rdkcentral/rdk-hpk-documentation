/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the Video Port subsystem. The Video Port subsystem manages system-specific HAL operations.
 *  @{
 */

/**
*   @defgroup dsVideoPort_HAL DS Video Port HAL
 *  @{
 * @par Application API Specification
 * dsVideoPort HAL provides an interface for managing the Video Port settings for the device settings module
 */

/**
 * @defgroup DSHAL_VIDEO_PORT_API DS HAL Video Port Public APIs
 *  @{
 */

/**
 * @file dsVideoPort.h
 *
 * @brief Device Settings HAL Video Port Public API.
 * This API defines the HAL for the Device Settings Video Port interface.
 *
 * @par Document
 * Document reference.
 *
 * @par Open Issues (in no particular order)
 * -# None
 *
 * @par Assumptions
 * -# None
 *
 * @par Abbreviations
 * - HAL:   Hardware Abstraction Layer
 * - API:   Caller Programming Interface
 * - Caller:  Any user of the interface via the APIs
 * - CPU:   Central Processing Unit
 * - DS:    Device Settings
 * - SoC:   System on Chip
 * - HDMI:  High-Definition Multimedia Interface
 * - DTCP:  Digital Transmission Content Protection
 * - HDCP:  High-bandwidth Digital Content Protection
 * - HDR:   High Dynamic Range
 * - SDR:   Standard Dynamic Range
 * - SCART: Syndicat des Constructeurs d'Appareils Radiorécepteurs et Téléviseurs - Radio and Television Receiver Manufacturers' Association
 * - EDID:  Extended Display Identification Data
 *
 * @par Implementation Notes
 * -# None
 *
 */

#ifndef __DS_VIDEO_PORT_H__
#define __DS_VIDEO_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "dsError.h"
#include <sys/types.h>
#include "dsAVDTypes.h"

/**
 * @brief Initializes the underlying Video Port sub-system.
 * 
 * This function must initialize all the video specific output ports and any associated resources.
 *
 * @return dsError_t                    - Status
 * @retval dsERR_NONE                   - Success
 * @retval dsERR_ALREADY_INITIALIZED    - Function is already initialized
 * @retval dsERR_RESOURCE_NOT_AVAILABLE - Resources have failed to allocate
 * @retval dsERR_GENERAL                - Underlying undefined platform error
 * 
 * @warning  This API is Not thread safe.
 *
 * @post dsVideoPortTerm() must be called to release resources.
 * 
 * @see dsVideoPortTerm()
 */
dsError_t dsVideoPortInit();

 /**
 * @brief Terminates the underlying Video Port sub-system.
 *
 * This function must terminate all the video output ports and any associated resources.
 *
 * @return dsError_t                - Status
 * @retval dsERR_NONE               - Success
 * @retval dsERR_NOT_INITIALIZED    - Module is not initialised
 * @retval dsERR_GENERAL            - Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsVideoPortInit()
 */
dsError_t dsVideoPortTerm();

/**
 * @brief Gets the handle for video port requested 
 * 
 * This function is used to get the handle of the video port corresponding to specified port type. It must return
 * dsERR_OPERATION_NOT_SUPPORTED if the requested video port is unavailable.
 *
 * @param[in]  type     - Type of video port (e.g. HDMI).  Please refer ::dsVideoPortType_t
 * @param[in]  index    - Index of the video device (0, 1, ...)  (Index of the port must be 0 if not specified)
 *                          Max index is platform specific. Min value is 0.   Please refer ::kSupportedPortTypes
 * @param[out] handle   - The handle used by the Caller to uniquely identify the HAL instance
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetVideoPort(dsVideoPortType_t type, int index, intptr_t *handle);

/**
 * @brief Checks whether a video port is enabled or not.
 * 
 * This function indicates whether the specified video port is enabled or not.
 *
 * @param[in]  handle   - Handle of the video port returned from dsGetVideoPort()
 * @param[out] enabled  - Flag to hold the enabled status of Video Port. 
 *                          ( @a true when video port is enabled or @a false otherwise)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableVideoPort()
 */
dsError_t  dsIsVideoPortEnabled(intptr_t handle, bool *enabled);

/**
 * @brief Checks whether the specific video port is connected to display.
 *
 * For sink devices, where the display is always connected (dsVIDEOPORT_TYPE_INTERNAL), this API will consistently return true
 * for the display connection status.
 * For source devices, this function is used to check whether video port is connected to a display or not.
 *
 * @param[in]  handle       - Handle of the video port returned from dsGetVideoPort()
 * @param[out] connected    - Flag to hold the connection status of display
 *                              ( @a true if display is connected or @a false otherwise)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsIsDisplayConnected(intptr_t handle, bool *connected);

/**
 * @brief Checks if the connected display supports the audio surround.
 * 
 * This function is used to check if the display connected to video port supports the audio surround.
 *
 * @param[in]  handle   - Handle of the video port returned from dsGetVideoPort()
 * @param[out] surround - Audio surround support  ( @a true if display supports surround sound or @a false otherwise)
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsIsDisplaySurround(intptr_t handle, bool *surround);

/**
 * @brief Gets the surround mode of video port
 * 
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 * For source devices, this function is used to get the surround mode of the specified video port.
 *
 * @param[in]  handle   - Handle of the video port returned from dsGetVideoPort()
 * @param[out] surround - Surround mode .Please refer :: dsSURROUNDMode_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsGetSurroundMode(intptr_t handle, int *surround);

/**
 * @brief Callback function to notify the Video Format change event to the clients.
 *
 * HAL Implementation must call this method to update the Video Format info event
 * to the application.
 *
 * @param[in] videoFormat Current video format.  Please refer ::dsHDRStandard_t
 *
 * @pre dsVideoFormatUpdateRegisterCB()
 */
typedef void (*dsVideoFormatUpdateCB_t)(dsHDRStandard_t videoFormat);

/**
 * @brief Callback Registration for the Video Format update event.
 *
 * This function registers a callback function to receive the Video Format update events.
 *
 * @param[in] cb        - Callback function
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
 dsError_t dsVideoFormatUpdateRegisterCB (dsVideoFormatUpdateCB_t cb);

/**
 * @brief Checks whether a video port is active or not.
 * 
 * For Source devices, this function is used to indicate whether a video port is active or not. A HDMI output port is active if it is connected
 * to the active port of sink device.
 * For Sink devices, where the display is always connected (dsVIDEOPORT_TYPE_INTERNAL), this API will return true for the Video port status.
 *
 * @param[in]  handle   - Handle of the video port returned from dsGetVideoPort()
 * @param[out] active   - Connection state of the video port 
 *                          ( @a true if connected, @a false otherwise)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t  dsIsVideoPortActive(intptr_t handle, bool *active);

/**
 * @brief Enables/Disables the HDCP of a video port.
 * 
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 * For source devices, this function is used to enable/disable the HDCP (High-bandwidth Digital Content Protection)
 * for the specified video port. It must return dsERR_OPERATION_NOT_SUPPORTED if connected 
 * video port does not support HDCP.
 *
 * @param[in] handle            - Handle of the video port returned from dsGetVideoPort()
 * @param[in] contentProtect    - Flag to enable/disable HDCP content protection
 *                                  ( @a true to enable, @a false to disable)
 * @param[in] hdcpKey           - HDCP key
 * @param[in] keySize           - HDCP key size.  Please refer ::HDCP_KEY_MAX_SIZE
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetHDCPStatus(), dsIsHDCPEnabled()
 */
dsError_t  dsEnableHDCP(intptr_t handle, bool contentProtect, char *hdcpKey, size_t keySize);

 /**
 * @brief Indicates whether a video port is HDCP protected.
 * 
 * For sink devices, this function returns dsERR_NONE and the content protection status is true always.
 * For source devices,this function indicates whether the specified video port is configured for HDCP
 * content protection. It must return dsERR_OPERATION_NOT_SUPPORTED if HDCP
 * is not supported.
 *
 * @param[in]  handle               - Handle of the video port returned from dsGetVideoPort()
 * @param [out] pContentProtected   - Current HDCP content protection status
 *                                      ( @a true when enabled, @a false otherwise)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableHDCP()
 */
dsError_t  dsIsHDCPEnabled (intptr_t handle, bool* pContentProtected);

 /**
 * @brief Enables/Disables a video port.
 *
 * This function enables or disables the specified video port.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] enabled   - Flag to enable/disable the video port 
 *                         ( @a true to enable, @a false to disable)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsIsVideoPortEnabled()
 */
dsError_t  dsEnableVideoPort(intptr_t handle, bool enabled);

/**
 * @brief Sets the display resolution of specified video port.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 * For source devices, this function sets the resolution of the specified video port.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param[in] resolution    - Video resolution. Please refer ::dsVideoPortResolution_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetResolution()
 */
dsError_t  dsSetResolution(intptr_t handle, dsVideoPortResolution_t *resolution);

/**
 * @brief Gets the display resolution of specified video port.
 *
 * This function gets the current display resolution of the specified video port.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param [out] resolution  - Current resolution of the video port.  Please refer ::dsVideoPortResolution_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetResolution()
 */
dsError_t  dsGetResolution(intptr_t handle, dsVideoPortResolution_t *resolution);

/**
 * @brief Callback function to notify the HDCP status change.
 * 
 * HAL Implementation must invoke this callback to notify the HDCP status change event
 * to the caller(e.g. Authentication , Failure etc.).
 * 
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] status    - HDCP status change. Please refer :: dsHdcpStatus_t
 *
 *@pre dsRegisterHdcpStatusCallback()
 */
typedef void (*dsHDCPStatusCallback_t)(intptr_t handle, dsHdcpStatus_t status);

/**
 * @brief Callback Registration for the HDCP status change event.
 *
 * For sink devices, this function returns dsERR_NONE and callback is triggered once during bootup 
 * with HDCP status as authenticated.
 * For source devices, this function registers a callback function to receive the HDCP status change event from
 * the specific HDMI Port.
 * 
 * @note Application must install at most one callback function per handle.
 * Multiple listeners are supported at application layer and thus not
 * required in HAL implementation.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] cb        - Callback function
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
 dsError_t dsRegisterHdcpStatusCallback (intptr_t handle, dsHDCPStatusCallback_t cb);

 /**
 * @brief Gets the current HDCP status of the specified video port.
 *
 * For sink devices, this function returns the authentication status as dsHDCP_STATUS_AUTHENTICATED and returns dsERR_NONE always.
 * For source device, this function gives current HDCP status of the specified video port. It must return dsERR_OPERATION_NOT_SUPPORTED if connected  video port does not support HDCP.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[out] status   - HDCP status of the video port.  Please refer ::dsHdcpStatus_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableHDCP()
 */
dsError_t dsGetHDCPStatus (intptr_t handle, dsHdcpStatus_t *status);

/**
 * @brief Gets the HDCP protocol version of the device.
 *
 * @param[in] handle                - Handle of the video port returned from dsGetVideoPort()
 * @param [out] protocolVersion     - HDCP protocol version.  Please refer ::dsHdcpProtocolVersion_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableHDCP()
 */
dsError_t dsGetHDCPProtocol (intptr_t handle, dsHdcpProtocolVersion_t *protocolVersion);

/**
 * @brief Gets the HDCP protocol version of the connected sink device.
 * 
 * This function is only for source devices.
 *
 * @param[in] handle                - Handle of the video port returned from dsGetVideoPort()
 * @param [out] protocolVersion     - HDCP protocol version.  Please refer ::dsHdcpProtocolVersion_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/When a sink device invokes this API
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableHDCP()
 */
dsError_t dsGetHDCPReceiverProtocol (intptr_t handle, dsHdcpProtocolVersion_t *protocolVersion);

/**
 * @brief Gets the current negotiated HDCP protocol version.
 *
 * @param[in] handle                - Handle of the video port returned from dsGetVideoPort()
 * @param [out] protocolVersion     - HDCP protocol version.  Please refer ::dsHdcpProtocolVersion_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsEnableHDCP()
 */
dsError_t dsGetHDCPCurrentProtocol (intptr_t handle, dsHdcpProtocolVersion_t *protocolVersion);


/**
 * @brief Gets the HDR capabilities of the TV/display device
 *
 * This function is used to get the HDR capabilities of the TV/display device.
 *
 * @param[in] handle            - Handle of the video port(TV) returned from dsGetVideoPort()
 * @param [out] capabilities    - Bitwise OR-ed value of supported HDR standards.  Please refer ::dsHDRStandard_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @see dsIsOutputHDR()
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetTVHDRCapabilities(intptr_t handle, int *capabilities);

/**
 * @brief Gets the supported resolutions of TV.
 *
 * This function is used to get TV supported resolutions of TV/display device.
 *
 * @param[in] handle            - Handle of the video port(TV) returned from dsGetVideoPort()
 * @param [out] resolutions     - OR-ed value supported resolutions.  Please refer ::dsTVResolution_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsSupportedTvResolutions(intptr_t handle, int *resolutions);

/**
 * @brief Sets ForceDisable 4K support variable.
 *
 * This function returns dsERR_OPERATION_NOT_SUPPORTED for both source and sink devices.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param [out] disable     - bool value to disable 4K support
 *                              ( @a true to forcefully disable 4K support, @a false otherwise) 
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetForceDisable4KSupport()
 */
dsError_t dsSetForceDisable4KSupport(intptr_t handle, bool disable);

/**
 * @brief Gets ForceDisable 4K support variable.
 *
 * This function returns dsERR_OPERATION_NOT_SUPPORTED for both source and sink devices.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param [out] disable     - bool value which indicates whether 4K Support is forcefully disabled or not
 *                              ( @a true if disabled, @a false otherwise)
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetForceDisable4KSupport()
 */
dsError_t dsGetForceDisable4KSupport(intptr_t handle, bool *disable);

/**
 * @brief Enables/Disables ALLM mode for HDMI output video port.
 *
 * For source devices, this function enables or disables the ALLM mode for specified HDMI output video port.
 * Source ALLM mode(on HF-VSIF) is enabled only if Sink ALLM bit is set (on HF-VSDB) as per HDMI 2.1 Specification
 * ALLM mode set remains until the hotplug or device reboot
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] enabled   - Flag to enable/disable the ALLM mode for the HDMI output port
 *                         ( @a true to enable, @a false to disable)
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 *
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 *
 * @warning  This API is Not thread safe.
 *
 * @see dsGetAllmEnabled()
 */

dsError_t dsSetAllmEnabled (intptr_t  handle, bool enabled);

/**
 * @brief Checks whether ALLM mode of HDMI output video port is enabled or not.
 *
 * For Source devices, this function indicates whether ALLM mode for specified HDMI output video port is enabled or not.
 * By default, ALLM mode is disabled on bootup and after hotplug.
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in]  handle   - Handle of the video port returned from dsGetVideoPort()
 * @param[out] enabled  - Pointer to hold the enabled status of ALLM mode for given HDMI output video port.
 *                          ( @a true when ALLM mode is enabled or @a false otherwise)
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 *
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 *
 * @warning  This API is Not thread safe.
 *
 * @see dsSetAllmEnabled()
 */
dsError_t dsGetAllmEnabled (intptr_t  handle, bool *enabled);

/**
 * @brief Gets the current video Electro-Optical Transfer Function (EOT) value.
 * 
 * This function is used to get the current HDR format on a specified video port.
 *
 * @param[in]  handle       - Handle of the video port returned from dsGetVideoPort()
 * @param[out] video_eotf   - EOTF value.  Please refer ::dsHDRStandard_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetVideoEOTF(intptr_t handle, dsHDRStandard_t *video_eotf);

/**
 * @brief Gets the current matrix coefficients value.
 * 
 * This function is used to get the current matrix coefficient value of the specified video port.
 * For source devices, this function would return dsDISPLAY_MATRIXCOEFFICIENT_UNKNOWN  when TV is not connected.
 *
 * @param[in]  handle               - Handle of the video port returned from dsGetVideoPort()
 * @param[out] matrix_coefficients  - pointer to matrix coefficients value.  Please refer ::dsDisplayMatrixCoefficients_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetMatrixCoefficients(intptr_t handle, dsDisplayMatrixCoefficients_t *matrix_coefficients);

/**
 * @brief Gets the color depth value of specified video port.
 * 
 * For sink devices, this function returns the default color depth, which is platform dependent.
 *
 * For source devices, this function is used to get the current color depth value of specified video port.
 * Typically for UHD resolution, the color depth is 10/12-bit, while for non-UHD resolutions, it is 8-bit
 *
 * @param[in]  handle       - Handle of the video port returned from dsGetVideoPort()
 * @param[out] color_depth  - pointer to color depth values.Please refer :: dsDisplayColorDepth_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetColorDepth(intptr_t handle, unsigned int* color_depth);

/**
 * @brief Gets the color space setting of specified video port.
 * 
 * For sink devices, this function returns the default color space setting, which is platform dependent.
 *
 * For source devices, this function is used to get the current color space setting of specified video port.
 * The color space is typically YCbCr.
 *
 * @param[in]  handle       - Handle of the video port returned from dsGetVideoPort()
 * @param[out] color_space  - pointer to color space value. Please refer ::dsDisplayColorSpace_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetColorSpace(intptr_t handle, dsDisplayColorSpace_t* color_space);
 

/**
 * @brief Gets the quantization range of specified video port.
 * 
 * This function is used to get the quantization range of the specified video port.
 * For source devices, this function would return dsDISPLAY_MATRIXCOEFFICIENT_UNKNOWN when TV is not connected.
 *
 * @param[in]  handle               - Handle of the video port returned from dsGetVideoPort()
 * @param[out] quantization_range   - pointer to quantization range.  Please refer ::dsDisplayQuantizationRange_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetQuantizationRange(intptr_t handle, dsDisplayQuantizationRange_t* quantization_range);

/**
 * @brief Gets current color space setting, color depth, matrix coefficients, video Electro-Optical Transfer Function (EOT)
 *        and  quantization range in one call of the specified video port
 *
 * @param[in]  handle               - Handle of the video port returned from dsGetVideoPort()
 * @param[out] video_eotf           - pointer to EOTF value.  Please refer ::dsHDRStandard_t
 * @param[out] matrix_coefficients  - pointer to matrix coefficients value.  Please refer ::dsDisplayMatrixCoefficients_t
 * @param[out] color_space          - pointer to color space value.  Please refer ::dsDisplayColorSpace_t
 * @param[out] color_depth          - pointer to color depths value.  Please refer ::dsDisplayColorDepth_t
 * @param[out] quantization_range   - pointer to quantization range value.  Please refer ::dsDisplayQuantizationRange_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetCurrentOutputSettings(intptr_t handle, dsHDRStandard_t* video_eotf, dsDisplayMatrixCoefficients_t* matrix_coefficients, dsDisplayColorSpace_t* color_space, unsigned int* color_depth, dsDisplayQuantizationRange_t* quantization_range);

/**
 * @brief Checks if video output is HDR or not.
 *
 * This function checks if the video output is HDR or not.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param [out] hdr     - pointer to HDR support status.( @a true if output is HDR, @a false otherwise )
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @see dsGetTVHDRCapabilities()
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsIsOutputHDR(intptr_t handle, bool* hdr);

/**
 * @brief Resets Video Output to SDR.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * For source devices, this function resets the video output to SDR.
 * It forces and locks the HDMI output to SDR mode regardless of the source content format
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsResetOutputToSDR();

/**
 * @brief Sets the preferred HDMI Protocol of the specified video port.
 *
 * This function sets the preferred HDMI Protocol of the specified video port.
 *
 * @param[in] handle                    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] hdcpCurrentProtocol       - HDCP protocol to be set.  Please refer ::dsHdcpProtocolVersion_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetHdmiPreference()
 */
dsError_t dsSetHdmiPreference(intptr_t handle, dsHdcpProtocolVersion_t *hdcpCurrentProtocol);

/**
 * @brief Gets the preferred HDMI Protocol version of specified video port.
 * 
 * This function is used to get the preferred HDMI protocol version of the specified video port.
 *
 * @param[in] handle                    - Handle of the video port returned from dsGetVideoPort()
 * @param [out] hdcpCurrentProtocol     - Preferred HDMI Protocol.  Please refer ::dsHdcpProtocolVersion_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetHdmiPreference()
 */
dsError_t dsGetHdmiPreference(intptr_t handle, dsHdcpProtocolVersion_t *hdcpCurrentProtocol);

/**
 * @brief Gets the IgnoreEDID status variable set in the device.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 * For source devices, this function is used to retrieve the status variable in order to determine whether to ignore the EDID data. 
 * Used by caller to decide whether it should handle the hdmi resolution settings or not after hdcp Authentication. 
 * If platform doesn't want to set the status, then returns dsERR_OPERATION_NOT_SUPPORTED.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param [out] status      - Status of IgnoreEDID variable, ( @a true if EDID data ccan be ignored, @a false otherwise )
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsGetIgnoreEDIDStatus(intptr_t handle, bool* status);

/**
 * @brief Sets the background color of the specified video port.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * For source devices, this function sets the background color of the specified video port.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] color     - Background color to be set.  Please refer ::dsVideoBackgroundColor_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsSetBackgroundColor(intptr_t handle, dsVideoBackgroundColor_t color);

/**
 * @brief Sets/Resets the force HDR mode.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * For source devices, this function is used to set/reset force HDR mode for the specified video port.
 * It forces and locks the HDMI output to a specified HDR mode regardless of the source content format,
 * if the mode dsHDRSTANDARD_NONE is set, then the HDMI output to follow source contnet format.
 *
 * @param[in] handle    - Handle of the video port returned from dsGetVideoPort()
 * @param[in] mode      - HDR mode to be forced.  Please refer ::dsHDRStandard_t
 *
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsSetForceHDRMode(intptr_t handle, dsHDRStandard_t mode);

/**
 * @brief Gets the color depth capabilities of the specified video port
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 * For source devices, this function is used to get the color depth capabilities of the specified video port.
 *
 * @param[in] handle                    - Handle of the video port returned from dsGetVideoPort()
 * @param [out] colorDepthCapability    - OR-ed value of supported color depth standards.  Please refer ::dsDisplayColorDepth_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 */
dsError_t dsColorDepthCapabilities(intptr_t handle, unsigned int *colorDepthCapability );

/**
 * @brief Gets the preferred color depth values.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 * For source devices, this function is used to get the preferred color depth of the specified video port.
 * Typically for UHD resolution, the color depth is 10/12-bit, while for non-UHD resolutions, it is 8-bit.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param [out] colorDepth  - color depth value.  Please refer ::dsDisplayColorDepth_t
 *   
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetPreferredColorDepth()
 */
dsError_t dsGetPreferredColorDepth(intptr_t handle, dsDisplayColorDepth_t *colorDepth);

/**
 * @brief Sets the preferred color depth for the specified video port.
 *
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always
 * For source devices, this function is used to set the preferred color depth for the specified video port.
 *
 * @param[in] handle        - Handle of the video port returned from dsGetVideoPort()
 * @param[in] colorDepth    - color depth value.Please refer :: dsDisplayColorDepth_t
 * 
 * @return dsError_t                      -  Status 
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsVideoPortInit() and dsGetVideoPort() must be called before calling this API.
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsGetPreferredColorDepth()
 */
dsError_t dsSetPreferredColorDepth(intptr_t handle,dsDisplayColorDepth_t colorDepth);

#ifdef __cplusplus
}
#endif
#endif /* __DS_VIDEO_PORT_H__ */

/** @} */ // End of DS HAL VIDEO PORT Public APIs
/** @} */ // End of DS Video Port HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
