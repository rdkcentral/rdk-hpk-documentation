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
 *  @{
 */

/**
*   @defgroup DSHAL_TYPES_H DS Types Header
 * @{
 *
 * Main configuration options provided by the device settings modules are:
 *  - Audio output ports (volume, mute, MS12 settings, Audio Delay, Audio Mixing,
 *           Fader Control, Primary Language),
 *  - Video output ports (resolutions, aspect ratio, Active Source, color),
 *  - Video device (zoom settings),
 *  - Display (aspect ratio, EDID data),
 *  - Front-panel indicators,
 *  - General platform (host) configuration,
 *
 * @note Each module in the must have an initialization function and
 * a termination function. Clients of the HAL API are required to call the initialization
 * function first before using any other APIs from the module. If the initialization
 * function is not yet called, all HAL API invocations must return ::dsERR_NOT_INITIALIZED.
 * If an API is not supported on a platform, the implementation must return
 * ::dsERR_OPERATION_NOT_SUPPORTED. Otherwise, the API must return ::dsERR_NONE upon
 * success, or return the most suitable error code. Vendors implementing HAL APIs are
 * allowed to add to the dsError_t enumerations, but should only do so with discretion.
 *
 */

/**
 * @file dsTypes.h
 *
 * @brief HAL types.
 *
 * @brief This API defines common types used by the HAL.
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
 *
 * @par Implementation Notes
 * -# None
 *
 */
#ifndef _DS_TYPES_H_
#define _DS_TYPES_H_

#include <sys/types.h>
#include <stdint.h>    
#include <stdbool.h>
#include "dsAVDTypes.h"
#include "dsFPDTypes.h"
#include "dsHdmiInTypes.h"
#include "dsVideoDeviceTypes.h"
#include "dsCompositeInTypes.h"

 /** @addtogroup DSHAL_VERSION_TYPES HAL VERSION Definitions
 *  @{
 */

/**
 * @brief HAL version number.
*/

#define dsHAL_APIVER(major, minor) (uint32_t)((major << 16) | (minor & 0xFFFF)) 
    ///< Return 4 Bytes version value

/**
 * @brief HAL Major version number.
*/

#define dsHAL_APIVER_MAJOR(x)   ((x) >> 16) & (0x7FFF) 
    ///< Return the two Most Significant Byte value

/**
 * @brief HAL Minor version number.
*/

#define dsHAL_APIVER_MINOR(x)   (x & 0xFFFF) ///< Return the two Least Significant Byte value

/**
 * @} // End of DSHAL_VERSION_TYPES
 */
#endif

/** @} */ // End of DSHAL_TYPES_H
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
