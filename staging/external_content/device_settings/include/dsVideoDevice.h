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
 * @defgroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @defgroup Device_Settings_HAL Device Settings HAL
 * Describe the details about Device Settings HAL function specifications.
 *
 * <b> Following abbreviations present in HAL function </b>
 *
 * @par Abbreviations
 * - cb:      Callback function (suffix).
 * - DS:      Device Settings.
 * - HAL:     Hardware Abstraction Layer.
 * - FPS:     Frames Per Second.
 * - FRF:     Frame Rate Frequency
 * - HDR:     High Dynamic Range
 * 
 *  @{
 */

/**
 * @defgroup dsVIDEODEVICE_HAL DS Video Device HAL
 *  @{
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the Video Device subsystem. The Video Device subsystem manages system-specific HAL operations.
 *
 */

/**
 * @defgroup DS_VIDEODEVICE_API DS HAL Video Device Public APIs
 * @par Application API Specification
 * dsVideoDevice HAL provides an interface for managing the VideoDevice settings for the device settings module
 *
 *  @{
 */


/**
 * @file dsVideoDevice.h
 */


#ifndef __DS_VIDEO_DEVICE_H__
#define __DS_VIDEO_DEVICE_H__

#include <stdbool.h>
#include "dsError.h"
#include "dsVideoDeviceTypes.h"
#include "dsAVDTypes.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes all the video devices in the system
 *
 * @return dsError_t                   - Status
 * @retval dsERR_NONE                   - Success
 * @retval dsERR_ALREADY_INITIALIZED    - Function is already initialized.
 * @retval dsERR_GENERAL                - Underlying undefined platform error
 *
 * @warning  This function is Not thread safe.
 *
 * @post dsVideoDeviceTerm() must be called to release resources.
 *
 * @see dsVideoDeviceTerm()
 * 
 * 
 */
dsError_t  dsVideoDeviceInit();


/**
 * @brief Gets the handle for the video device requested
 * 
 * @note Index is always 0, due to devices only having a single video device
 * 
 * @param[in]  index    - Index of video device. Max number is device specific. Min of 0
 * @param[out] handle   - The handle used by the Caller to uniquely identify the HAL instance
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() must be called before calling this function
 * 
 * @warning  This function is Not thread safe.
* 
 * 
 */
dsError_t  dsGetVideoDevice(int index, intptr_t *handle);

/**
 * @brief Sets the screen zoom mode (decoder format conversion)
 *
 * For source devices, this function establishes the zoom mode settings according to the available zoom options for specified video device.
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] handle    - The handle returned from the dsGetVideoDevice() function
 * @param[in] dfc       - Type of zoom mode to be used.  Please refer ::dsVideoZoom_t
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsGetDFC()
 * 
 */
dsError_t  dsSetDFC(intptr_t handle, dsVideoZoom_t dfc);


/**
 * @brief Gets the screen zoom mode (decoder format conversion)
 *
 * For source devices, this function gets the zoom mode settings for specified video device.
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] handle    - The handle returned from the dsGetVideoDevice() function
 * @param[out] dfc      - Type of zoom mode being used.  Please refer ::dsVideoZoom_t
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsSetDFC()
 * 
 */
dsError_t  dsGetDFC(intptr_t handle, dsVideoZoom_t *dfc);


/**
 * @brief De-initializes all the video devices in the system
 *
 * This function reset any data structures used within this module and 
 * release any handles specific to the video devices.
 *
 * @return dsError_t             - Status
 * @retval dsERR_NONE            - Success
 * @retval dsERR_NOT_INITIALIZED - Module is not initialized
 * @retval dsERR_GENERAL         - General failure
 * 
 * @pre dsVideoDeviceInit() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsVideoDeviceInit()
 * 
 */
dsError_t  dsVideoDeviceTerm();
 

/**
 * @brief Gets the HDR capabilities
 *
 * For sink devices, this function returns the HDR Capabilities of TV
 * For source devices, this function returns the HDR Capabilities of STB
 *
 * @param[in]  handle       - The handle returned from the dsGetVideoDevice() function
 * @param[out] capabilities - OR-ed values of all supported HDR standards.  Please refer ::dsHDRStandard_t,
 *                                  dsHDRStandard_t is currently in the audioVisual combined file.
 *
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 */
dsError_t dsGetHDRCapabilities(intptr_t handle, int *capabilities);


/**
 * @brief Gets the video formats supported
 *
 * @param[in]   handle              - The handle returned from the dsGetVideoDevice() function
 * @param[out]  supported_formats   - OR-ed values of all the supported video codec formats. 
 *                                           Please refer ::dsVideoCodingFormat_t
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 */
dsError_t dsGetSupportedVideoCodingFormats(intptr_t handle, unsigned int * supported_formats);


/**
 * @brief Gets the video codec information of the provided coding format.
 *
 * For source devices, this function returns the supported codec profile and levels for the input coding format.
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in]  handle   - The handle returned from the dsGetVideoDevice() function
 * @param[in]  codec    - OR-ed value of supported video codec formats.  Please refer ::dsVideoCodingFormat_t.
 * @param[out] info     - Structure containing Video codec information.  Please refer ::dsVideoCodecInfo_t
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 */
dsError_t dsGetVideoCodecInfo(intptr_t handle, dsVideoCodingFormat_t codec, dsVideoCodecInfo_t * info); 


/**
 * @brief Disables, forcefully the HDR support of the device
 *
 * This function returns dsERR_OPERATION_NOT_SUPPORTED for both source and sink devices.
 *
 * @param[in] handle    - The handle returned from the dsGetVideoDevice() function
 * @param[in] disable   - Boolean value to force disable HDR or not.
 *                              True to force disable, false to remove force disable
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 */
dsError_t dsForceDisableHDRSupport(intptr_t handle, bool disable);

/**
 * @brief Sets the FRF mode of the device
 *
 * For sink devices, this function sets the Frame Rate Mode for the video device.
 * For source devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] handle    - The handle returned from the dsGetVideoDevice() function
 * @param[in] frfmode   - integer value to enable/disable Auto Framerate mode.
 *                               1 to enable auto framerate mode, 0 to disable auto framerate mode
 *
 * @return dsError_t                       - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsGetFRFMode()
 * 
 */
dsError_t dsSetFRFMode(intptr_t handle, int frfmode);


/**
 * @brief Gets the FRF mode of the device
 *
 * For sink devices, this function returns the Frame Rate Mode of the video device.
 * For source devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in]  handle   - The handle returned from the dsGetVideoDevice() function
 * @param[out] frfmode  - integer value corresponding to Auto Framerate mode of the device.
 *                             1 if auto framerate mode is enabled, 0 if auto framerate mode is disabled
 *
 * @return dsError_t                       - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsSetFRFMode()
 * 
 */
dsError_t dsGetFRFMode(intptr_t handle, int *frfmode);

/**
 * @brief Gets the current panel refresh rate of the device
 *
 * For sink devices, this function returns the current panel refresh rate of the device.
 * For source devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in]  handle       - The handle returned from the dsGetVideoDevice() function
 * @param[out] framerate    - Current Panel Refresh Rate will be represented.
 *                            Returns the value as a string(eg:"3840x2160px48")
 *
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized 
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsSetDisplayframerate()
 * 
 */
dsError_t dsGetCurrentDisplayframerate(intptr_t handle, char *framerate);
 

/**
 * @brief Sets the panel refresh rate for the device
 *
 * For sink devices, this function sets the panel refresh rate for the device.
 * For source devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] handle    - The handle returned from the dsGetVideoDevice() function
 * @param[in] framerate - Panel Refresh Rate value to be set which is platform-specific.
 *                        Expects the value as a string(eg:"3840x2160px48").
 *                        aaaaxbbbbxyy (aaaaxbbbb - Panel Resolution, yy - Panel Refresh Rate)
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialized
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() and dsGetVideoDevice() must be called before calling this function
 * 
 * @warning  This function is Not thread safe.
 * 
 * @see dsGetCurrentDisplayframerate()
 * 
 */
dsError_t dsSetDisplayframerate(intptr_t handle, char *framerate);

/**
 * @brief Call back function to receive the framerate pre change event from the HAL side.
 * @param [in] tSecond - time(in Seconds) during which the framerate will change.
 * 
 * @pre dsVideoDeviceInit(), dsRegisterFrameratePreChangeCB() must be called before calling this function.
 *
 */
typedef void (*dsRegisterFrameratePreChangeCB_t)(unsigned int tSecond);

/**
 * @brief This function is used to register the callback function for the Display framerate pre change event.
 *
 * For sink devices, this function registers a callback for Display framerate pre change event.
 * For source devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] CBFunc    - Function callback to register for the event. 
 *                              See dsRegisterFrameratePreChangeCB_t.
 *
 * @return dsError_t                       - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() must be called before calling this function.
 * @post dsRegisterFrameratePreChangeCB_t callback must be called after calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 */
dsError_t dsRegisterFrameratePreChangeCB(dsRegisterFrameratePreChangeCB_t CBFunc);

/**
 * @brief Call back function to receive the framerate post change event.
 * @param [in] tSecond - time elapsed(in Seconds) after framerate change.
 * 
 * @pre dsVideoDeviceInit(), dsRegisterFrameratePostChangeCB() must be called before calling this function.
 * 
 *
 */
typedef void (*dsRegisterFrameratePostChangeCB_t)(unsigned int tSecond);

/**
 * @brief This function is used to register a callback function for the Display framerate 
 *                      post change event from the HAL side.
 *
 * For sink devices, this function registers a callback for Display framerate post change event.
 * For source devices, this function returns dsERR_OPERATION_NOT_SUPPORTED always.
 *
 * @param[in] CBFunc    - Function to register for the event. 
 *                                  See dsRegisterFrameratePostChangeCB_t.
 *
 * @return dsError_t                       - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre dsVideoDeviceInit() must be called before calling this function.
 * @post dsRegisterFrameratePostChangeCB_t callback must be called after calling this function.
 * 
 * @warning  This function is Not thread safe.
 * 
 */
dsError_t dsRegisterFrameratePostChangeCB(dsRegisterFrameratePostChangeCB_t CBFunc);


#ifdef __cplusplus
}
#endif
#endif /* __DS_VIDEO_DEVICE_H__ */

/** @} */ // End of DSHAL_VIDEODEVICE_APIs
/** @} */ // End of DS Video Device HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
