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
 * the FPD subsystem. The FPD subsystem manages system-specific HAL operations.
 *  @{
 */

/**
 * @defgroup dsFPD_HAL DS Front Panel Display HAL
 *  @{
 * @par Application API Specification
 * dsFPD HAL provides an interface for managing Front Panel Display settings
 */

/**
 * @defgroup DSHAL_FPD_API DS HAL Front Panel Display Public APIs
 *  @{
 */

/**
 * Describe the details about Device Settings HAL API specifications.
 *
 * <b> Following abbreviations present in Device Settings Front Panel Display API </b>
 *
 * @par Abbreviations
 * - DS  : Device Settings
 * - FPD : Front-Panel Display
 * - HAL : Hardware Abstraction Layer
 * - LED : Light-Emitting Diode
 * - SoC : System-On-Chip
 * - ms  : miliseconds
 *
 */


#ifndef __DS_FPD_H__
#define __DS_FPD_H__

#include "dsError.h"
#include "dsFPDTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Front Panel Display (FPD) sub-module of Device Settings HAL
 *
 * This function allocates required resources for Front Panel and is required to be called before the other APIs in this module.
 * 
 *
 * @return dsError_t                  -  Status
 * @retval dsERR_NONE                 -  Success
 * @retval dsERR_ALREADY_INITIALIZED  -  Function is already initialized
 * @retval dsERR_GENERAL              -  Underlying undefined platform error
 *
 * @post dsFPTerm() must be called to release resources
 *
 * @warning  This API is Not thread safe
 * 
 * @see dsFPTerm()
 * 
 */
dsError_t dsFPInit (void);

/**
 * @brief Sets blink pattern of specified Front Panel Display LED
 * 
 * This function is used to set the individual discrete LED to blink for a specified number of iterations with blink interval.
 * This function must return dsERR_OPERATION_NOT_SUPPORTED if FP State is "OFF".
 * To stop the blink, either dsFPSetLEDState() or dsFPTerm() can be invoked.
 *
 * @param[in] eIndicator        -  FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[in] uBlinkDuration    -  Blink interval. The time in ms the text display will remain ON 
 *                                   during one blink iteration.
 * @param[in] uBlinkIterations  -  The number of iterations per minute data will blink
 * 
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * 
 * @pre dsFPInit() and dsSetFPState() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */
dsError_t dsSetFPBlink (dsFPDIndicator_t eIndicator, unsigned int uBlinkDuration, unsigned int uBlinkIterations);

/**
 * @brief Sets the brightness level of specified Front Panel Display LED
 * 
 * This function will set the brightness of the specified discrete LED on the Front
 * Panel Display to the specified brightness level. This function must return dsERR_OPERATION_NOT_SUPPORTED
 * if the FP State is "OFF". HAL will neither retain the brightness value nor set any default brightness value.
 *
 * @param[in] eIndicator  - FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[in] eBrightness - The brightness value(0 to 100) for the specified indicator.
 *                            Please refer ::dsFPDBrightness_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 c
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() and dsSetFPState() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsGetFPBrightness()
 * 
 */
dsError_t dsSetFPBrightness (dsFPDIndicator_t eIndicator, dsFPDBrightness_t eBrightness);

/**
 * @brief Gets the brightness level of specified Front Panel Display LED
 * 
 * This function returns the brightness level of the specified discrete LED on the Front
 * Panel. This function must return dsERR_OPERATION_NOT_SUPPORTED if FP State is "OFF". 
 *
 * @param[in]  eIndicator  - FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[out] pBrightness - current brightness value(0 to 100) of the specified indicator
 *                             Please refer ::dsFPDBrightness_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() and dsSetFPState() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe.
 * 
 * @see dsSetFPBrightness()
 * 
 */
dsError_t dsGetFPBrightness (dsFPDIndicator_t eIndicator, dsFPDBrightness_t *pBrightness);

/**
 * @brief Sets the indicator state of specified discrete Front Panel Display LED
 * 
 *
 * @param[in] eIndicator - FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[in] state      - Indicates the state of the indicator to be set. Please refer ::dsFPDState_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsGetFPState()
 * 
 */
dsError_t dsSetFPState (dsFPDIndicator_t eIndicator, dsFPDState_t state);

/**
 * @brief Gets the indicator state of specified discrete Front Panel Display LED
 * 
 * 
 * @param[in]  eIndicator - FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[out] state      - current state of the specified indicator. Please refer ::dsFPDState_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsSetFPState()
 * 
 */
dsError_t dsGetFPState (dsFPDIndicator_t eIndicator, dsFPDState_t* state);

/**
 * @brief Sets the color of specified Front Panel Display LED
 * 
 * This function sets the color of the specified Front Panel Indicator LED, if the 
 * indicator supports it (i.e. is multi-colored). It must return
 * dsERR_OPERATION_NOT_SUPPORTED if the indicator is single-colored or if the FP State is "OFF". 
 *
 * @param[in] eIndicator    - FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[in] eColor        - The color index for the specified indicator. Please refer ::dsFPDColor_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() and dsSetFPState() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsGetFPColor()
 * 
 */
dsError_t dsSetFPColor (dsFPDIndicator_t eIndicator, dsFPDColor_t eColor);

/**
 * @brief  Gets the color of specified Front Panel Display LED
 * 
 * This function gets the color of the specified Front Panel Indicator LED, if the
 * indicator supports it (i.e. is multi-colored). It must return
 * dsERR_OPERATION_NOT_SUPPORTED if the indicator is single-colored or if the FP State is "OFF". 
 *
 * @param[in] eIndicator - FPD indicator index. Please refer ::dsFPDIndicator_t
 * @param[out] pColor    - current color value of the specified indicator. Please refer ::dsFPDColor_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() and dsSetFPState() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsSetFPColor()
 * 
 */
dsError_t dsGetFPColor (dsFPDIndicator_t eIndicator, dsFPDColor_t *pColor);

/**
 * @note This API is deprecated.
 *
 * @brief Sets the time on 7-Segment Front Panel Display LEDs
 * 
 * This function sets the 7-segment display LEDs to show the time in specified format.
 * The format (12/24-hour) has to be specified. If there are no 7-Segment display LEDs present on the
 * device or if the FP State is "OFF" then dsERR_OPERATION_NOT_SUPPORTED must be returned.
 * It must return dsERR_INVALID_PARAM if the format and hours values do not agree,
 * or if the hours/minutes are invalid.
 * The FP Display Mode must be dsFPD_MODE_CLOCK/dsFPD_MODE_ANY. Please refer ::dsFPDMode_t
 * 
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 *
 * @param[in] eTimeFormat   - Time format (12 or 24 hrs). Please refer ::dsFPDTimeFormat_t
 * @param[in] uHour         - Hour information
 * @param[in] uMinutes      - Minutes information
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * @warning  This API is Not thread safe
 * 
 * @see dsGetFPTimeFormat()
 * 
 */
dsError_t dsSetFPTime (dsFPDTimeFormat_t eTimeFormat, const unsigned int uHour, const unsigned int uMinutes);

/**
 * @note This API is deprecated.
 *
 * @brief Displays the specified text on 7-segment Front Panel Display LEDs
 * 
 * This function is used to set the 7-segment display LEDs to show the given text.  
 * If there are no 7-Segment display LEDs present on the device or if the FP State is "OFF",
 * then dsERR_OPERATION_NOT_SUPPORTED must be returned. Please refer ::dsFPDState_t.
 * The FP Display Mode must be dsFPD_MODE_TEXT/dsFPD_MODE_ANY. Please refer ::dsFPDMode_t
 * 
 * @param[in] pText - Text to be displayed. Maximum length of Text is 10 characters.
 * 
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsSetFPTextBrightness() 
 * 
 */
dsError_t dsSetFPText(const char* pText);

/**
 * @note This API is deprecated.
 *
 * @brief Sets the brightness level of 7-segment Front Panel Display LEDs
 * 
 * This function will set the brightness of the specified 7-segment display LEDs on the Front
 * Panel Display to the specified brightness level. If there are no 7-Segment display LEDs present
 * on the device or if the FP State is "OFF" then dsERR_OPERATION_NOT_SUPPORTED must be returned.
 * The FP Display Mode must be dsFPD_MODE_TEXT/dsFPD_MODE_ANY. Please refer ::dsFPDMode_t
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 * 
 * @param[in] eIndicator    - FPD Text indicator index. Please refer ::dsFPDTextDisplay_t
 * @param[in] eBrightness   - The brightness value for the specified indicator. Valid range is from 0 to 100
 *                              Please refer ::dsFPDBrightness_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsGetFPTextBrightness(), dsSetFPText()
 * 
 */
dsError_t dsSetFPTextBrightness (dsFPDTextDisplay_t eIndicator, dsFPDBrightness_t eBrightness);

/**
 * @note This API is deprecated.
 *
 * @brief Gets the brightness of 7-segment Front Panel Display LEDs
 * 
 * This function will get the brightness of the specified 7-segment display LEDs on the Front
 * Panel Text Display. If there are no 7-segment display LEDs present or if the FP State is "OFF"
 * then dsERR_OPERATION_NOT_SUPPORTED must be returned. 
 * The FP Display Mode must be dsFPD_MODE_CLOCK/dsFPD_MODE_ANY. Please refer ::dsFPDMode_t
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 * 
 * @param[in] eIndicator    - FPD Text indicator index. Please refer ::dsFPDTextDisplay_t
 * @param[out] eBrightness  - Brightness value. Valid range is from 0 to 100. Please refer ::dsFPDBrightness_t.
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsSetFPTextBrightness()
 * 
 */
dsError_t dsGetFPTextBrightness (dsFPDTextDisplay_t eIndicator, dsFPDBrightness_t *eBrightness);

/**
 * @note This API is deprecated.
 *
 * @brief Enables/Disables the clock display of Front Panel Display LEDs
 * 
 * This function will enable or disable displaying of clock. It will return dsERR_OPERATION_NOT_SUPPORTED
 * if Clock display is not available
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template_template file.
 * 
 * @param[in] enable    - Indicates the clock to be enabled or disabled. 
 *                          1 if enabled, 0 if disabled.
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t / If Clock display is not available
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */
dsError_t dsFPEnableCLockDisplay (int enable);

/**
 * @note This API is deprecated.
 *
 * @brief Enables Text Scrolling on 7-segment Front Panel Display LEDs
 * 
 * This function scrolls the text in the 7-segment display LEDs for the given number of iterations.
 * If there are no 7-segment display LEDs present or if the FP State is "OFF" then
 * dsERR_OPERATION_NOT_SUPPORTED must be returned.
 * Horizontal and Vertical scroll cannot work at the same time. If both values are non-zero values
 * it should return dsERR_OPERATION_NOT_SUPPORTED.
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 * 
 * @param[in] uScrollHoldOnDur       - Duration in ms to hold each char before scrolling to the next position 
 *                                       during one scroll iteration
 * @param[in] uHorzScrollIterations  - Number of iterations to scroll horizontally
 * @param[in] uVertScrollIterations  - Number of iterations to scroll vertically
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */
dsError_t dsSetFPScroll(unsigned int uScrollHoldOnDur, unsigned int uHorzScrollIterations, unsigned int uVertScrollIterations);

/**
 * @brief Terminates the the Front Panel Display sub-module
 * 
 * This function resets any data structures used within Front Panel sub-module,
 * and releases all the resources allocated during the init function.
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning This API is Not thread safe
 * 
 * @see dsFPInit()
 * 
 */
dsError_t dsFPTerm(void);

/**
 * @note This API is deprecated.
 *
 * @brief Sets the current time format on the 7-segment Front Panel Display LEDs
 *
 * This function sets the 7-segment display LEDs to show the
 * specified time in specified format. It must return dsERR_OPERATION_NOT_SUPPORTED 
 * if the underlying hardware does not have support for clock.
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 * 
 * @param[in] eTimeFormat   -  Indicates the time format (12 hour or 24 hour). 
 *                               Please refer ::dsFPDTimeFormat_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @note The time display should also change according to the new format set
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsGetFPTimeFormat()
 * 
 */
dsError_t dsSetFPTimeFormat (dsFPDTimeFormat_t eTimeFormat);

 /**
 * @note This API is deprecated.
 *
 * @brief Gets the current time format on the 7-segment Front Panel Display LEDs
 *
 * This function gets the current time format set on 7-segment display LEDs panel.
 * It must return dsERR_OPERATION_NOT_SUPPORTED if the underlying hardware does not 
 * have support for clock.
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 * 
 * @param[out] pTimeFormat      - Current time format value (12 hour or 24 hour). 
 *                                  Please refer ::dsFPDTimeFormat_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsSetFPTimeFormat()
 * 
 */
dsError_t dsGetFPTimeFormat (dsFPDTimeFormat_t *pTimeFormat);

/**
 * @note This API is deprecated.
 *
 * @brief Sets the display mode of the Front Panel Display LEDs
 *
 * This function sets the display mode (clock or text or both) for FPD.
 * It must return dsERR_OPERATION_NOT_SUPPORTED if the underlying hardware does not 
 * have support for Text or Clock.
 *
 * @note Whether this device has a 7-Segment display LEDs should be within the dsFPDSettings_template file.
 * 
 * @param[in] eMode - Indicates the mode. Please refer ::dsFPDMode_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported/FP State is "OFF". Please refer ::dsFPDState_t
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called and FP State must be "ON" before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 */
dsError_t dsSetFPDMode (dsFPDMode_t eMode);

/**
 * @brief Gets the current power Front Panel Display LED state
 * 
 * This function gets the current power LED state
 *
 * @param[out] state - Current LED state. Please refer ::dsFPDLedState_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsFPSetLEDState()
 */
dsError_t dsFPGetLEDState (dsFPDLedState_t* state);

/**
 * @brief Sets the current power Front Panel Display LED state
 * 
 * This function sets the current power LED state
 *
 * @param[in] state - LED state. Please refer ::dsFPDLedState_t
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 * 
 * @see dsFPGetLEDState()
 */
dsError_t dsFPSetLEDState (dsFPDLedState_t state);

/**
 * @brief Gets the supported led states
 * 
 * This function gets the supported led states
 *
 * @param[out] states - The bitwise value of all supported led states by the platform. refer ::dsFPDLedState_t
 *      e.g. *states = ((1<<dsFPD_LED_DEVICE_ACTIVE) | (1<<dsFPD_LED_DEVICE_STANDBY))
 *
 * @return dsError_t                      -  Status
 * @retval dsERR_NONE                     -  Success
 * @retval dsERR_NOT_INITIALIZED          -  Module is not initialised
 * @retval dsERR_INVALID_PARAM            -  Parameter passed to this function is invalid
 * @retval dsERR_OPERATION_NOT_SUPPORTED  -  The attempted operation is not supported
 * @retval dsERR_GENERAL                  -  Underlying undefined platform error
 * 
 * @pre dsFPInit() must be called before calling this API
 * 
 * @warning  This API is Not thread safe
 */
dsError_t dsFPGetSupportedLEDStates (unsigned int* states);


#ifdef __cplusplus
}
#endif
#endif /* __DS_FPD_H__ */

/** @} */ // End of DSHAL FPD API
/** @} */ // End of DS FPD HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
