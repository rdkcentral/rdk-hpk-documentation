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
 * the CompositeIn subsystem. The CompositeIn subsystem manages system-specific HAL operations.
 *  @{
 */

/**
*   @addtogroup dsCompositeIn_HAL DS CompositeIn HAL
 *  @{
 * @par Application API Specification
 * dsCompositeIn HAL provides an interface for managing the CompositeIn settings for the device settings module
 */

/**
 * @defgroup DSHAL_COMPOSITE_IN_TYPES DS HAL Composite Input Type Definitions
 *  @{
 */

/**
 * @file dsCompositeInTypes.h
 *
 * @brief Device Setting CompositeIn HAL types.
 *
 * This file defines common types used by the above mentioned DS CompositeIn HAL Submodule.
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
 * - HAL:     Hardware Abstraction Layer.
 *
 * @par Implementation Notes
 * -# None
 *
 */

#ifndef __DS_COMPOSITE_IN_TYPES_H__
#define __DS_COMPOSITE_IN_TYPES_H__
#pragma once
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief This enumeration defines the type composite signal status.
 */
typedef enum _dsCompInSignalStatus_t
{
    dsCOMP_IN_SIGNAL_STATUS_NONE = -1,    ///< Composite input signal status NONE
    dsCOMP_IN_SIGNAL_STATUS_NOSIGNAL,     ///< Composite input No Signal status
    dsCOMP_IN_SIGNAL_STATUS_UNSTABLE,     ///< Composite input Unstable signal status
    dsCOMP_IN_SIGNAL_STATUS_NOTSUPPORTED, ///< Composite input Not supported signal status
    dsCOMP_IN_SIGNAL_STATUS_STABLE,       ///< Composite input Stable signal status
    dsCOMP_IN_SIGNAL_STATUS_MAX           ///< Out of range 
} dsCompInSignalStatus_t;

/**
 * @brief This enumeration defines the type of composite ports.
 */
typedef enum _dsCompositeInPort_t
{
    dsCOMPOSITE_IN_PORT_NONE = -1,  ///< No composite port.
    dsCOMPOSITE_IN_PORT_0,          ///< Composite port index 0.
    dsCOMPOSITE_IN_PORT_1,          ///< Composite port index 1.
    dsCOMPOSITE_IN_PORT_MAX         ///< Out of range 
} dsCompositeInPort_t;

/**
 * @brief Structure type for Composite input status.
 */
typedef struct _dsCompositeInStatus_t
{
    bool    isPresented;                               ///< Boolean flag indicating Composite input is enabled for presentation by client
    bool    isPortConnected[dsCOMPOSITE_IN_PORT_MAX];  ///< Boolean flag indicating Composite source connected to this Composite input port
    dsCompositeInPort_t   activePort;                  ///< Composite input Port selected as the currently active port
                                                       ///<     - note that only one COMPOSITE input port can be active at a time
} dsCompositeInStatus_t;

#endif /* __DS_COMPOSITE_IN_TYPES_H__ */

/** @} */ // End of DS HAL Composite Input Public Types
/** @} */ // End of DS CompositeIn HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
