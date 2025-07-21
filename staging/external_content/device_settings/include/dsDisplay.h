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
 * @file dsDisplay.h
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
 * - cb:      Callback function (suffix).
 * - DS:      Device Settings.
 * - HAL:     Hardware Abstraction Layer.
 * - EDID:    Extended Display Identification Data.
 * - HDMI:    High-Definition Multimedia Interface
 * - DVI:     Digital Visual Interface
 * - EDID:    Extended Display Identification Data
 * - HDCP:    High-bandwidth Digital Content Protection
 * - SoC:     System on chip
 *
 * @par Implementation Notes
 * -# None
 *
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

/** @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**@addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the Display subsystem. The Display subsystem manages display-specific HAL operations.
 *  @{
 */

/** @defgroup dsDisplay_HAL DS Display HAL
 *  @{
 * @par Application API Specification
 * dsDisplay HAL provides an interface for managing the display related settings of device settings module
 */

/** @defgroup DSHAL_DISPLAY_API DS Display HAL Public APIs
 * 
 *  @{
 */
 
#ifndef __DS_VIDEO_DISPLAY_H__
#define __DS_VIDEO_DISPLAY_H__

#include <stdint.h>
#include "dsError.h"
#include "dsAVDTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum _dsDisplayEvent_t
 * @brief This enumeration defines standard display event types. 
 * @note  Appropriate events can be added based on requirement.
 */
typedef enum _dsDisplayEvent_t {
    dsDISPLAY_EVENT_CONNECTED = 0,  ///< Display connected event   
    dsDISPLAY_EVENT_DISCONNECTED,   ///< Display disconnected event       
    dsDISPLAY_RXSENSE_ON,           ///< Rx Sense ON event
    dsDISPLAY_RXSENSE_OFF,          ///< Rx Sense OFF event
    dsDISPLAY_HDCPPROTOCOL_CHANGE,  ///< HDCP Protocol Version Change event
    dsDISPLAY_EVENT_MAX             ///< Display max event
} dsDisplayEvent_t;

/**
 * @brief Callback function used to notify display related events to the caller.
 * 
 * For source devices, this callback is triggered whenever there is a display related change and it is notified to the caller.
 * For sink devices, this callback fires display connected once on bootup. Display is always connected for a sink device.
 *
 * @param[in]  handle   - Handle of the display device
 * @param[in]  event    - Display event. Please refer ::dsDisplayEvent_t
 * @param[in] eventData - Data associated with the event(optional parameter)
 *                         eventData contains additional information such as HPD status when a display is connected/disconnected
 *
 * @return None
 *
 * @pre dsRegisterDisplayEventCallback()
 */
typedef void (*dsDisplayEventCallback_t)(int handle, dsDisplayEvent_t event,
                                             void* eventData/*Optional*/);

/**
 * @brief Initializes the DS Display sub-system.
 *
 * This function initializes all required resources for Display sub-system and
 * is required to be called before the other APIs in this module. Also this function
 * needs to initialize all the required device handles for the different display ports and 
 * the number of connected devices for each display port. 
 * 
 * @return dsError_t                    - Status
 * @retval dsERR_NONE                   - Success
 * @retval dsERR_ALREADY_INITIALIZED    - Function is already initialized
 * @retval dsERR_GENERAL                - Underlying undefined platform error
 * 
 * @warning  This API is Not thread safe
 *
 * @see dsDisplayTerm()
 * 
 * @post dsDisplayTerm() must be called to release resources.
 */
dsError_t dsDisplayInit();

/**
 * @brief Gets the handle of connected display device.
 * 
 * This function is used to get the handle(as created in dsDisplayInit()) for the connected display device
 * corresponding to the specified video port.
 *
 * @param[in]  vType    - Type of video port. Please refer ::dsVideoPortType_t
 * @param[in]  index    - Index of the video port. (Index of the port must be 0 if not specified)
 *                          Max index is platform specific. Min value is 0.
 * @param[out] handle   - Pointer to hold the handle of display device corresponding 
 *                          to specified video port
 * 
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre  dsDisplayInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */

 dsError_t dsGetDisplay(dsVideoPortType_t vType, int index, intptr_t *handle);
  
/** 
 * @brief Gets the EDID information from the specified display device.
 * 
 * This function gets the EDID information from the HDMI/DVI display corresponding to
 * the specified display device handle.
 * 
 * @param[in]  handle   - Handle of the display device
 * @param[out] edid     - EDID info of the specified display device. Please refer ::dsDisplayEDID_t
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre  dsDisplayInit() and dsGetDisplay() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */

dsError_t dsGetEDID(intptr_t handle, dsDisplayEDID_t *edid);

/**
 * @brief Gets the EDID buffer and EDID length of connected display device. 
 * 
 * This function is used to get the EDID buffer and EDID size of the connected display corresponding to
 * the specified display device handle.
 *
 * @param[in] handle    - Handle of the display device
 * @param[out] edid     - Pointer to raw EDID buffer
 * @param[out] length   - length of the EDID buffer data. Min value is 0
 *
 * @note Caller is responsible for allocating memory for edid( please refer ::MAX_EDID_BYTES_LEN ) and freeing the EDID buffer
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre  dsDisplayInit() and dsGetDisplay() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */

dsError_t dsGetEDIDBytes(intptr_t handle, unsigned char *edid, int *length);

/**
 * @brief Gets the aspect ratio of connected display device.
 * 
 * For source devices, this function returns the aspect ratio of the display corresponding to the
 * specified display device handle. When no TV connected, this API would return aspect ratio 16:9.
 * For sink devices, this function returns dsERR_OPERATION_NOT_SUPPORTED as it is handled in TV Settings module.
 *
 * @param[in]  handle       - Handle of the display device
 * @param[out] aspectRatio  - Current aspect ratio of the specified display device
 *                              Please refer ::dsVideoAspectRatio_t
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre  dsDisplayInit() and dsGetDisplay() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */

dsError_t dsGetDisplayAspectRatio(intptr_t handle, dsVideoAspectRatio_t *aspectRatio);

/**
 * @brief Terminates the display sub-system.
 * 
 * This function resets any data structures used within Display sub-system,
 * and releases all the resources allocated during the init function.
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre  dsDisplayInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsDisplayInit()
 * 
 */
dsError_t dsDisplayTerm();

/**
 * @brief Callback registration for display related events.
 * 
 * This function registers a callback for display events corresponding to
 * the specified display device handle.
 * 
 * @note Caller should install at most one callback function per handle.
 * Multiple listeners are supported at Caller layer and thus not
 * required in HAL implementation.
 *
 * @param[in] handle    - Handle of the display device
 * @param[in] cb        - Display Event callback function. Please refer ::dsDisplayEventCallback_t
 *
 * @return dsError_t                        - Status
 * @retval dsERR_NONE                       - Success
 * @retval dsERR_NOT_INITIALIZED            - Module is not initialised
 * @retval dsERR_INVALID_PARAM              - Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED    - The attempted operation is not supported
 * @retval dsERR_GENERAL                    - Underlying undefined platform error
 * 
 * @pre  dsDisplayInit() and dsGetDisplay() must be called before calling this API
 *
 * @warning  This API is Not thread safe
 * 
 * @see dsDisplayEventCallback_t()
 * 
 */
dsError_t dsRegisterDisplayEventCallback(intptr_t handle, dsDisplayEventCallback_t cb);

#ifdef __cplusplus
}
#endif
#endif /* __DS_VIDEO_DISPLAY_H__ */

/** @} */ // End of DSHAL DISPLAY API
/** @} */ // End of DS Display HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
