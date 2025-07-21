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
 * @file dsFPDTypes.h
 *
 * @brief Device Setting Front Panel Display HAL types.
 *
 * This file defines common types used by the above mentioned DS Front Panel Display HAL Submodule.
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
 * - FPD  :   Front-Panel Display
 * - HAL  :   Hardware Abstraction Layer
 * - RF   :   Radio Frequency
 * - RGB  :   Red Green Blue
 * - LED  :   Light-Emitting Diode
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
 * @defgroup DSHAL_FPD_TYPES DS HAL Front Panel Display (FPD) Type Definitions
 *  @ingroup dsFPD_HAL
 *  @{
 */


#ifndef __DS_FPD_TYPES_H__
#define __DS_FPD_TYPES_H__

#include <stdint.h>

#pragma once

/**
 * @brief dsFPDColor_t is an 4-byte integer that is composed of RGB32 value in the
 * following pattern:
 * 
 * Combines the RGB Color values into a single Hex value.
 * 
 * FPD_COLOR = (((R8)<<16) | ((G8)<< 8) | ((B8) << 0))
 *
 * The Most Significant 8 bit is reserved.
 * 
 * @see dsFPDColor_isValid
 */
typedef uint32_t dsFPDColor_t;

/**
 * @brief Defines the color values in RGB format.
*/
#define dsFPDColor_Make(R8,G8,B8)  (((R8)<<16) | ((G8)<< 8) | ((B8) )) ///< combine Red Green Blue value to a single Hex value
#define dsFPDColor_R(RGB32)    (((RGB32) >> 16) & 0xFF)                ///< Extract Red value form RGB value
#define dsFPDColor_G(RGB32)    (((RGB32) >>  8) & 0xFF)                ///< Extract Green value form RGB value
#define dsFPDColor_B(RGB32)    (((RGB32)      ) & 0xFF)                ///< Extract Blue value form RGB value

/**
 * @brief Define a set of common colors, for backward compatibility .
 */
#define dsFPD_COLOR_BLUE   dsFPDColor_Make(0, 0, 0xFF)          ///< Blue color LED                 
#define dsFPD_COLOR_GREEN  dsFPDColor_Make(0, 0xFF, 0)          ///< Green color LED                
#define dsFPD_COLOR_RED    dsFPDColor_Make(0xFF, 0, 0x0)        ///< Red color LED                 
#define dsFPD_COLOR_YELLOW dsFPDColor_Make(0xFF, 0xFF, 0xE0)    ///< Yellow color LED               
#define dsFPD_COLOR_ORANGE dsFPDColor_Make(0xFF, 0x8C, 0x00)    ///< Orange color LED               
#define dsFPD_COLOR_WHITE  dsFPDColor_Make(0xFF, 0xFF, 0xFF)    ///< White color LED               
#define dsFPD_COLOR_MAX    6                                    ///< Out of range 

/**
 * @brief Front panel LED colors validation check.
 * @see dsFPDColor_t
 */
#define dsFPDColor_isValid(t)  (((t) & 0xFF000000) == 0)

/**
 * @brief This enumeration pre-defines common front panel indicators.
 * Implementation may not have to use these enumerators.
 */

typedef enum __dsFPDIndicator_t
{
    dsFPD_INDICATOR_MESSAGE,     ///< Message/Mail LED
    dsFPD_INDICATOR_POWER,       ///< Power LED
    dsFPD_INDICATOR_RECORD,      ///< Record LED
    dsFPD_INDICATOR_REMOTE,      ///< Remote LED
    dsFPD_INDICATOR_RFBYPASS,    ///< RF Bypass LED
    dsFPD_INDICATOR_MAX          ///< Out of range
}dsFPDIndicator_t;


/**
 * @brief Front panel LED indicator type validation check.
 * @see dsFPDIndicator_t
 */
#define dsFPDIndicator_isValid(t)  (((t)  >= dsFPD_INDICATOR_MESSAGE ) && ((t) < dsFPD_INDICATOR_MAX))


/**
 * @brief This enumeration defines the FPD text display types
 * @see dsFPDTextDisplay_isValid
 */
typedef enum __dsFPDTextDisplay_t
{
    dsFPD_TEXTDISP_TEXT,      ///< 7-segment LED display                 
    dsFPD_TEXTDISP_MAX       ///< Out of range 
}dsFPDTextDisplay_t;

/**
 * @brief FPD text display type validation check.
 * @see dsFPDTextDisplay_t
 */
#define dsFPDTextDisplay_isValid(t)  (((t)  >= dsFPD_TEXTDISP_TEXT ) && ((t) < dsFPD_TEXTDISP_MAX))

/**
 * @brief This enumeration defines the front panel display state
 */
typedef enum __dsFPDState_t
{
    dsFPD_STATE_OFF=0,  ///< FPD State Disable
    dsFPD_STATE_ON,     ///< FPD State Enable
    dsFPD_STATE_MAX,    ///< Out of range 
}dsFPDState_t;

/**
 * @brief This enumeration defines the front panel display state
 * 
 * The HAL shall define the responsibility of LED behavior during each of the below states
 * 
 */
typedef enum __dsFPDLedState_t
{
    dsFPD_LED_DEVICE_NONE,                      ///< Default state
    dsFPD_LED_DEVICE_ACTIVE,                    ///< Device is active
    dsFPD_LED_DEVICE_STANDBY,                   ///< Device is in standby mode
    dsFPD_LED_DEVICE_WPS_CONNECTING,            ///< Device connecting to WPS
    dsFPD_LED_DEVICE_WPS_CONNECTED,             ///< Device connected to WPS
    dsFPD_LED_DEVICE_WPS_ERROR,                 ///< Error when trying to connect to WPS
    dsFPD_LED_DEVICE_FACTORY_RESET,             ///< Reset Device to factory base
    dsFPD_LED_DEVICE_USB_UPGRADE,               ///< Updating from USB drive
    dsFPD_LED_DEVICE_SOFTWARE_DOWNLOAD_ERROR,   ///< Error in downloading new software update
    dsFPD_LED_DEVICE_MAX                        ///< Out of range 
}dsFPDLedState_t;

/**
 * @brief This enumeration defines the time formats to be used in the front panel text display.
 */
typedef enum __dsFPDTimeFormat_t
{
    dsFPD_TIME_12_HOUR,  ///< 12 hour time format
    dsFPD_TIME_24_HOUR,  ///< 24 hour time format
    dsFPD_TIME_MAX       ///< Out of range 
}dsFPDTimeFormat_t;

/**
 * @brief Maximum Value of FPD Led Brightness.
 */
#define dsFPD_BRIGHTNESS_MAX    100     ///< Maximum Value of FPD LED brightness

/**
 * @brief Type definition for the brightness setting of a front panel indicator LED.
 */
typedef uint32_t dsFPDBrightness_t;

/**
 * @brief This enumeration defines the front panel display mode.
 */
typedef enum __dsFPDMode_t{
    dsFPD_MODE_ANY = 0, ///< All modes (text/clock)
    dsFPD_MODE_TEXT,    ///< Text mode
    dsFPD_MODE_CLOCK,   ///< Clock mode
    dsFPD_MODE_MAX,     ///< Out of range 
} dsFPDMode_t;

/**	
* @brief Structure that defines the Front Panel Display Color Configurations
 */
typedef struct _dsFPDColorConfig_t {
	int id;              // FPD Color Identifier
	dsFPDColor_t color;  // Color of the FPD LED
}dsFPDColorConfig_t;

/**
 * @brief Structure that defines the Front Panel Display Indicator Configurations
 */
typedef struct _dsFPDIndicatorConfig_t {
	dsFPDIndicator_t id;			// FPD LED Identifier
	dsFPDColorConfig_t * supportedColors;	// Supported Colors of the FPD LEDs 
	int maxBrightness;			// Maximum Brightness of FPD LEDs
	int maxCycleRate;			// Maximum Cycle Rate of FPD LEDs
    	int minBrightness;			// Minimum Brightness of FPD LEDs
    	int levels;				// Default brightness level of FP LEDs 
    	int colorMode;				// Color Mode(Single(0) or Multi-colored(1)) of FPD LEDs
} dsFPDIndicatorConfig_t;

/**
 * @brief Structure that defines the Front Panel Text Display Configurations
 */
typedef struct _dsFPDTextDisplayConfig_t {
	dsFPDTextDisplay_t id;                 	// FP Text Display Identifier
	const char * name;                     	// Name of the FP Text Display
	dsFPDColorConfig_t * supportedColors;	// Supported Colors of 7-Segement LED Display
	int	maxBrightness;                 	// Maximum Brightness of 7-Segement LED Display
	int maxCycleRate;                      	// Maximum Cycle Rate of 7-Segement LED Display
	const char * supportedCharacters;      	// Supported Characters (alpha-numeric characters except special characters) of 7-Segement LED Display
	int columns;                           	// Maximum  Horizontal Columns of 7-Segement LED Display
	int rows;                              	// Maximum Vertical Rows of 7-Segement LED Display
	int	maxHorizontalIterations;      	// Maximum Horizontal Iterations of 7-Segement LED Display
	int maxVerticalIterations;             	// Maximum Vertical Iterations of 7-Segement LED Display
	int levels;                            	// Default brightness level of FP Text Display
	int colorMode;                         	// Color Mode (Single(0) or Multi-colored(1)) of 7-Segement LED Display
} dsFPDTextDisplayConfig_t;


#endif /* __DS_FPD_TYPES_H__ */

/** @} */ // End of DSHAL_FPD_TYPES
/** @} */ // End of DS FPD HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
