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
 * @file dsHdmiInTypes.h
 *
 * @brief Device Setting HDMI Input HAL types.
 *
 * This file defines common types used by the above mentioned DS HDMI Input HAL Submodule.
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
 * - HDMI:    High-Definition Multimedia Interface
 * - ARC:     Audio Return Channel.
 * - SPD:     Source Product Description
 * - rsd:     Repetition of static data
 * - pkttype: Package type
 * - ALLM:    Auto Low Latency Mode
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

/**
 * @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the HdmiIn subsystem. The HdmiIn subsystem manages system-specific HAL operations.
 *  @{
 */

/**
 * @addtogroup dsHdmiIn_HAL DS HdmiIn HAL
 *  @{
 * @par Application API Specification
 * dsHdmiIn HAL provides an interface for managing the HdmiIn settings for the device settings module
 */

/**
 * @defgroup dsHdmiIn_HAL_Type_H DS HAL Hdmi Input Type Definitions
 *  @{
 */

#ifndef __DS_HDMI_IN_TYPES_H__
#define __DS_HDMI_IN_TYPES_H__

#include <stdint.h>

#pragma once //Required for compilation, until header structure is properly updated.

/**
 * @brief Max buffer length for the feature list
 * 
 */
#define MAX_FEATURE_LIST_BUFFER_LEN 1024

/**
 * @brief This enumeration defines the type of HDMI ports.
 */
typedef enum _dsHdmiInPort_t
{
    dsHDMI_IN_PORT_NONE = -1,  /*!< HDMI input port index for NONE */
    dsHDMI_IN_PORT_0,          /*!< HDMI input port index for PORT 0 */
    dsHDMI_IN_PORT_1,          /*!< HDMI input port index for PORT 1 */
    dsHDMI_IN_PORT_2,          /*!< HDMI input port index for PORT 2 */
    dsHDMI_IN_PORT_3,          /*!< HDMI input port index for PORT 3 */
    dsHDMI_IN_PORT_4,          /*!< HDMI input port index for PORT 4 */
    dsHDMI_IN_PORT_MAX         /*!< Out of range */
} dsHdmiInPort_t;

/**
 * @brief This enumeration defines the type of HDMI signal status.
 */
typedef enum _dsHdmiInSignalStatus_t
{
    dsHDMI_IN_SIGNAL_STATUS_NONE = -1,    /*!< HDMI input signal status NONE. Default state upon start up */
    dsHDMI_IN_SIGNAL_STATUS_NOSIGNAL,     /*!< HDMI input No signal signal status. No device connected */
    dsHDMI_IN_SIGNAL_STATUS_UNSTABLE,     /*!< HDMI input unstable signal status. This is normally a transitional state,  */
                                          /*!< but can remain here due to some faults on HDMI Source / Cable */
    dsHDMI_IN_SIGNAL_STATUS_NOTSUPPORTED, /*!< HDMI input not supported signal status */
                                          /*!< e.g. The sink device does not support the incoming HDMI In signal from source */
    dsHDMI_IN_SIGNAL_STATUS_STABLE,       /*!< HDMI input Stable signal status are presented on plane */
    dsHDMI_IN_SIGNAL_STATUS_MAX           /*!< Out of range  */
} dsHdmiInSignalStatus_t;

/**
 * @brief Structure type for HDMI input status.
 * 
 */
typedef struct _dsHdmiInStatus_t
{
    bool             isPresented;                           /*!< Boolean flag indicating HDMI input is presenting for the activePort */
    bool             isPortConnected[dsHDMI_IN_PORT_MAX];   /*!< Boolean flag indicating HDMI source connected to this HDMI input port  */
    dsHdmiInPort_t   activePort;                            /*!< HDMI input Port selected as the currently active port (to the set-top) */
                                                            /** @note that only one HDMI input port can be active at a time  */
} dsHdmiInStatus_t;

/**
 * @brief Structure type for HDMI input ARC Capability.
 */
typedef struct _dsHdmiInCap_t
{
    bool    isPortArcCapable[dsHDMI_IN_PORT_MAX]; /*!< Boolean flag indicating HDMI source is ARC capable  */
} dsHdmiInCap_t;

/**
 * @brief This enumeration defines the AVI Content Types
 */
typedef enum dsAviContentType {
  dsAVICONTENT_TYPE_GRAPHICS,   /*!< Content type Graphics. */
  dsAVICONTENT_TYPE_PHOTO,      /*!< Content type Photo */
  dsAVICONTENT_TYPE_CINEMA,     /*!< Content type Cinema */
  dsAVICONTENT_TYPE_GAME,       /*!< Content type Game */
  dsAVICONTENT_TYPE_INVALID,    /*!< Content type Invalid */
  dsAVICONTENT_TYPE_MAX,        /*!< Out of range */
}dsAviContentType_t;

/**
 * @brief Structure that captures Supported Game Features list
 */
typedef struct _dsSupportedGameFeatureList_t {
    char gameFeatureList[MAX_FEATURE_LIST_BUFFER_LEN]; /*!< buffer containing the list of comma separated supported game features (e.g: "allm") */
    int gameFeatureCount;                              /*!< Total number of supported game features */
} dsSupportedGameFeatureList_t;

/**
 * @brief Structure type for spd info frame.
 */
struct dsSpd_infoframe_st {
    uint8_t pkttype;            /*!< Package type */
    uint8_t version;            /*!< Version */
    uint8_t length;             /*!< max length 25, min length 0 */
    uint8_t rsd;                /*!< Repetition of static data */
    uint8_t checksum;           /*!< Checksum for spd info frame */
    uint8_t vendor_name[8];     /*!< Vendor Name string. Min length 0 */
    uint8_t product_des[16];    /*!< Product Description string. Min length 0 */
    uint8_t source_info;        /*!< Source info of the frame  */
} ;

/**
 * @brief Enum for EDID version.
 */
typedef enum tv_hdmi_edid_version_e {
    HDMI_EDID_VER_14 = 0,   /*!< EDID version 1.4 */
    HDMI_EDID_VER_20,       /*!< EDID version 2.0 */
    HDMI_EDID_VER_MAX,      /*!< Out of bounds */
} tv_hdmi_edid_version_t;

/**
 * @brief Enum for Video Plant Types
 */
typedef enum dsVideoPlaneType {
    dsVideoPlane_PRIMARY,       /*!< Primary Video Plane*/
    dsVideoPlane_SECONDARY,     /*!< Secondary Video Plane*/
    dsVideoPlane_MAX            /*!< Out of bounds*/
}dsVideoPlaneType_t;

/**
* @brief Enum for Hdmi Max Compatibility version
*/
typedef enum dsHdmiMaxCapabilityVersion{
    HDMI_COMPATIBILITY_VERSION_14 = 0, /*!< Hdmi Compatibility Version 1.4 */
    HDMI_COMPATIBILITY_VERSION_20,     /*!< Hdmi Compatibility Version 2.0 */
    HDMI_COMPATIBILITY_VERSION_21,     /*!< Hdmi Compatibility Version 2.1 */
    HDMI_COMPATIBILITY_VERSION_MAX     /*!< Out of bounds */
}dsHdmiMaxCapabilityVersion_t;

#endif // End of __DS_HDMI_IN_TYPES_H__

/** @} */ // End of dsHdmiIn_HAL_Type_H
/** @} */ // End of DS HdmiIn HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
