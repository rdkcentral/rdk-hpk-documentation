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
 * the Video Device subsystem. The Video Device subsystem manages system-specific HAL operations.
 * @{
 */

/**
 * @addtogroup dsVIDEODEVICE_HAL DS Video Device HAL
 * @{
 * @par Application API Specification
 * dsVideoDevice HAL provides an interface for managing the VideoDevice settings for the device settings module
 */

/**
 * @addtogroup DSHAL_VIDEODEVICE_SETTINGS_H DS Video Device Settings Header
 * @{
 * @par Application API Specification
 */

/** 
 * @file
 * 
 * <b> Following abbreviations present in HAL API </b>
 *
 * @par Abbreviations
 * - cb:      Callback function (suffix).
 * - DS:      Device Settings.
 * - HAL:     Hardware Abstraction Layer.
 * - HDMI:    High-Definition Multimedia Interface
 * - DFC:     Decoder format conversion
 *
 */


#ifndef __DS_VIDEO_DEVICE_SETTINGS_H__
#define __DS_VIDEO_DEVICE_SETTINGS_H__

#include "dsUtl.h"
#include "dsVideoDeviceTypes.h"


#ifdef __cplusplus
extern "C" {
namespace  {
#endif



/**
 * @brief A constant array of supported ZOOM settings.
 *
 * The array is accessible only within the current translation unit and the
 * values cannot be modified at runtime.
 *
 * @note The size of the array is fixed and cannot be changed.
 */
const dsVideoZoom_t kSupportedDFCs[] = {
    dsVIDEO_ZOOM_NONE,      /*!< Decoder format conversion is inactive, */
    dsVIDEO_ZOOM_FULL,      /*!< Full screen (16:9 video is zoomed to fit 4:3 frame) */
    dsVIDEO_ZOOM_PLATFORM}; /*!< Control over the decoder format conversions is managed by the platform. */

/**
 * @brief Default ZOOM Settings value
 *
 * The variable is accessible only within the current translation unit and the
 * values cannot be modified at runtime.
 *
 */
const dsVideoZoom_t kDefaultDFC  = dsVIDEO_ZOOM_FULL; /*!< 16:9 Zoom (4:3 video is zoomed to fill 16:9 frame) */

/**
 * @brief Number of Video Devices supported
 *
 * The variable is accessible only within the current translation unit and the
 * values cannot be modified at runtime.
 *
 */
const int kNumVideoDevices = 1;

/**
 * @brief A constant array of the device configuration
 *
 * The variable is accessible only within the current translation unit and the
 * values cannot be modified at runtime.
 *
 * @note The size of the array is fixed and cannot be changed.
 */
const dsVideoConfig_t kConfigs[]= {
    {
    /*.numSupportedDFCs = */    dsUTL_DIM(kSupportedDFCs),  /*!< Number of supported DFCs. 0 means "Info available at runtime" */
    /*.supportedDFCs = */       kSupportedDFCs,             /*!< Supported DFCs. */
    /*.defaultDFC = */          dsVIDEO_ZOOM_FULL,          /*!< 16:9 Zoom (4:3 video is zoomed to fill 16:9 frame). */
    },
};

/**
 * @brief Checks if number of elements in ::kConfigs is the same as in ::kNumVideoDevices
 *
 */
typedef int _SafetyCheck[(dsUTL_DIM(kConfigs) == kNumVideoDevices) ? 1 : -1];




#ifdef __cplusplus
}
}
#endif

#endif /* __DS_VIDEO_DEVICE_SETTINGS_H__ */

/** @} */ // End of DS Video Device Settings Header
/** @} */ // End of DS Video Device HAK
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK

