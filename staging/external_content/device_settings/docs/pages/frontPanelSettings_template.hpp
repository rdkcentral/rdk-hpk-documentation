/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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
 * the FPD subsystem. The FPD subsystem manages system-specific HAL operations.
 *  @{
 */

/**
 * @addtogroup dsFPD_HAL DS Front Panel Display HAL
 *  @{
 * @par Application API Specification
 * dsFPD HAL provides an interface for managing Front Panel Display settings
 */

/**
 * @defgroup dsFPD_Settings_H DS Front Panel Display Settings Header
 *  @{
 * @par Application API Specification
 * dsFPD provides an template for various Front Panel Indicator configurations
 */

#ifndef __DS_FPD_SETTINGS_H__
#define __DS_FPD_SETTINGS_H__

#warning "CONFIGURING PLATFORM FRONT PANEL SETTINGS"

#include "include/dsFPDTypes.h"

#ifdef __cplusplus
extern "C" {
namespace {
#endif

#define _MAX_BRIGHTNESS 100         /**< Maximum brightness value of FPD LEDs */
#define _MIN_BRIGHTNESS 0           /**< Minimum brightness value of FPD LEDs */
#define _DEFAULT_LEVELS 10          /**< Default Brightness level for the FP text display */
#define _MAX_CYCLERATE  2           /**< Maximum Rate at which LED is rotating during scrolling */
#define _MAX_HORZ_COLS  0           /**< Maximum Horizontal Rows of 7-segement display */
#define _MAX_VERT_ROWS  0           /**< Maximum Vertical Rows of 7-segment display */
#define _MAX_HORZ_ITER  0           /**< Maximum horizontal iteration value of 7-segment display */
#define _MAX_VERT_ITER  0           /**< Maximum vertical iteration value of 7-segment display */
#define _DEFAULT_COLOR_MODE 0       /**< Color Mode of LED (Single(0) or Multicolored(1)) */
#define dsFPD_TEXTDISP_MAX  1	     /**< Maximum Front Panel Text Displays */

/**
 * @brief Array that holds various Front Panel Indicator Colors
 */
dsFPDColorConfig_t  kIndicatorColors_1[] = {
	{
		0,			/**< Id */
		dsFPD_COLOR_BLUE,	/**< color */
	},
	{
		1,			/**< Id */
		dsFPD_COLOR_GREEN,	/**< color */
	},
	{
		2,			/**< Id */
		dsFPD_COLOR_RED,	/**< color */
	},
};

/**
 * @brief Array that holds various Front Panel Indicator Colors
 */
dsFPDColorConfig_t  kIndicatorColors_2[] = {
	{
		0,			/**< Id */
		dsFPD_COLOR_BLUE,	/**< color */
	},
	{
		1,			/**< Id */
		dsFPD_COLOR_GREEN,	/**< color */
	},
	{
		2,			/**< Id */
		dsFPD_COLOR_RED,	/**< color */
	},
};

/**
 * @brief Array that holds various Front Panel Indicator Colors
 */
dsFPDColorConfig_t  kIndicatorColors_3[] = {
	{
		0,			/**< Id */
		dsFPD_COLOR_BLUE,	/**< color */
	},
	{
		1,			/**< Id */
		dsFPD_COLOR_GREEN,	/**< color */
	},
	{
		2,			/**< Id */
		dsFPD_COLOR_RED,	/**< color */
	},
};

/**
 * @brief Array that holds Front Panel Indicator Configurations
 */
const dsFPDIndicatorConfig_t kIndicators[] = {
	{
		dsFPD_INDICATOR_MESSAGE,	/**< id */
		kIndicatorColors_1,		/**< supportedColors */
		_MAX_BRIGHTNESS,		/**< maxBrightness */
		_MAX_CYCLERATE,			/**< maxCycleRate */
		_MIN_BRIGHTNESS,		/**< minBrightness */
		_DEFAULT_LEVELS,		/**< default brightness levels */
		_DEFAULT_COLOR_MODE,		/**< colorMode */
	},
	{
		dsFPD_INDICATOR_POWER,		/**< id */
		kIndicatorColors_2,		/**< supportedColors */
		_MAX_BRIGHTNESS,		/**< maxBrightness */
		_MAX_CYCLERATE,			/**< maxCycleRate */
		_MIN_BRIGHTNESS,		/**< minBrightness */
		_DEFAULT_LEVELS,		/**< default brightness levels */
		_DEFAULT_COLOR_MODE,		/**< colorMode */
	},
	{
		dsFPD_INDICATOR_RECORD,		/**< id */
		kIndicatorColors_3,		/**< supportedColors */
		_MAX_BRIGHTNESS,		/**< maxBrightness */
		_MAX_CYCLERATE,			/**< maxCycleRate  */
		_MIN_BRIGHTNESS,		/**< minBrightness */
		_DEFAULT_LEVELS,		/**< default brightness levels */
		_DEFAULT_COLOR_MODE,		/**< colorMode */
	},
};

/**
 * @brief Array that holds Front Panel Text Display Configurations
 */
const dsFPDTextDisplayConfig_t  kTextDisplays[] = {
	{
		dsFPD_TEXTDISP_TEXT,		/**< id	*/
		"Text",				/**< name */
		kIndicatorColors,		/**< supportedColors */
		_MAX_BRIGHTNESS,		/**< maxBrightness */
		_MAX_CYCLERATE,			/**< maxCycleRate */
		"ABCDEFG",			/**< supportedCharacters (all alpha-numric characters except special characters)*/
		_MAX_HORZ_COLS,			/**< columns */
		_MAX_VERT_ROWS,			/**< rows */
		_MAX_HORZ_ITER,			/**< horizontal iterations */
		_MAX_VERT_ITER,			/**< vertical iterations */
		_DEFAULT_LEVELS,		/**< default brightness levels */
		_DEFAULT_COLOR_MODE		/**< colorMode */
	},
};

#ifdef __cplusplus
}
}
#endif
#endif /* __DS_FPD_SETTINGS_H__ */

/** @} */ // End of DS FP Settings Header
/** @} */ // End of DS FPD HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
