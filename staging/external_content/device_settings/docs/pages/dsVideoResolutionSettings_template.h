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
 * @defgroup DSHAL_VIDEO_RESOLUTION_SETTINGS_H DS Video Resolution Settings Header
 * @{
 * @par Application API Specification
 */

/** 
 * @file dsVideoResolutionSettings_template.h
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

#ifndef __DS_VIDEO_RESOLUTION_SETTINGS_H__
#define __DS_VIDEO_RESOLUTION_SETTINGS_H__

#include "dsAVDTypes.h"

#ifdef __cplusplus
extern "C" {
namespace {
#endif

#define  _INTERLACED true
#define _PROGRESSIVE false

#define dsVideoPortRESOLUTION_NUMMAX 32

/* List all supported resolutions here */

static dsVideoPortResolution_t kResolutions[] = {
		{   /*480i*/
			/*.name = */				"480i",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_720x480,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_4x3,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_29dot97,
			/*.interlaced = */			_INTERLACED,
		},
		{   /*480p*/
			/*.name = */				"480p",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_720x480,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_4x3,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_59dot94,
			/*.interlaced = */			_PROGRESSIVE,
		},
		{   /*720i*/
			/*.name = */				"576p50",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_720x576,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_4x3,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_50,
			/*.interlaced = */			_PROGRESSIVE,
		},
		{   /*720p - Default - AutoSelect */
			/*.name = */				"720p",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_1280x720,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_16x9,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_59dot94,
			/*.interlaced = */			_PROGRESSIVE,
		},
		{   /*720p - Default - AutoSelect */
			/*.name = */				"720p50",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_1280x720,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_16x9,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_50,
			/*.interlaced = */			_PROGRESSIVE,
		},
		{   /*1080p24*/
			/*.name = */				"1080p24",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_1920x1080,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_16x9,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_50,
			/*.interlaced = */			_PROGRESSIVE,
		},
		{   /*1080p60*/
			/*.name = */				"1080p60",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_1920x1080,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_16x9,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_59dot94,
			/*.interlaced = */			_PROGRESSIVE,
		},
		{   /*1080i*/
			/*.name = */				"1080i50",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_1920x1080,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_16x9,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_50,
			/*.interlaced = */			_INTERLACED,
		},
		{   /*1080p30*/
			/*.name = */				"1080i",
			/*.pixelResolution = */			dsVIDEO_PIXELRES_1920x1080,
			/*.aspectRatio = */			dsVIDEO_ASPECT_RATIO_16x9,
			/*.stereoscopicMode = */		dsVIDEO_SSMODE_2D,
			/*.frameRate = */			dsVIDEO_FRAMERATE_59dot94,
			/*.interlaced = */			_INTERLACED,
		},
};

static const int kDefaultResIndex = 3; //Pick one resolution from kResolutions[] as default

#ifdef __cplusplus
}
}
#endif
#endif /* __VIDEO_RESOLUTION_SETTINGS_H__ */

/** @} */ // End of DS Video Resolution Settings Header
/** @} */ // End of DS Video Port HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
