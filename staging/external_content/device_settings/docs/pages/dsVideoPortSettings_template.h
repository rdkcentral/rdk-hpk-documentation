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
 * @{
 */

/**
 * @addtogroup dsVideoPort_HAL DS Video Port HAL
 * @{
 * @par Application API Specification
 * dsVideoPort HAL provides an interface for managing the VideoPort settings for the device settings module
 */

/**
 * @defgroup DSHAL_VIDEOPORT_SETTINGS_H DS Video Port Settings Header
 * @{
 * @par Application API Specification
 */

/** 
 * @file dsVideoPortSettings_template.h
 * 
 * <b> Following abbreviations present in HAL API </b>
 *
 * @par Abbreviations
 * - cb:      Callback function (suffix).
 * - DS:      Device Settings.
 * - HAL:     Hardware Abstraction Layer.
 * - HDMI:    High-Definition Multimedia Interface
 *
 */

#ifndef __DS_VIDEO_PORT_SETTINGS_H__
#define __DS_VIDEO_PORT_SETTINGS_H__

#include "dsAVDTypes.h"
#include "dsUtl.h"
#include "dsVideoResolutionSettings_template.h"

#ifdef __cplusplus
extern "C" {
namespace {
#endif

/*
 * Enumerate the supported video port configurations here.
 */
static const dsVideoPortType_t kSupportedPortTypes[] = { dsVIDEOPORT_TYPE_INTERNAL };

/* 
 * Enumerate all configurations for each port type. 
 */
static const dsVideoPortTypeConfig_t kConfigs[]= {
		{
		/*.typeId = */				dsVIDEOPORT_TYPE_INTERNAL,
		/*.name = */ 				"INTERNAL",
		/*.dtcpSupported = */			false,
		/*.hdcpSupported = */			true,
		/*.restrictedResollution = */		-1,
		/*.numSupportedResolutions = */ 	dsUTL_DIM(kResolutions), // 0 means "Info available at runtime"
		/*.supportedResolutons = */     	kResolutions,
		},
};

/*
 *  Enumerate all enabled Video Output Ports here.
 */
static const dsVideoPortPortConfig_t kPorts[] = {
		{
		/*.typeId = */ 			{dsVIDEOPORT_TYPE_INTERNAL, 0},
		/*.connectedAOP */              {dsAUDIOPORT_TYPE_HDMI_ARC, 0},
		/*.defaultResolution = */	"1080i50"
		},
		{
                /*.typeId = */                  {dsVIDEOPORT_TYPE_INTERNAL, 0},
                /*.connectedAOP */              {dsAUDIOPORT_TYPE_SPDIF, 0},
                /*.defaultResolution = */       "1080i50"
        },
};



#ifdef __cplusplus
}
}
#endif
#endif /* __DS_VIDEO_PORT_SETTINGS_H__ */

/** @} */ // End of DS Video Port Settings Header
/** @} */ // End of DS Video Port HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
