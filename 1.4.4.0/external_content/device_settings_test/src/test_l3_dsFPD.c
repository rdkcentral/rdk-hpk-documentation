/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2022 RDK Management
 *
 *  Licensed under the Apache License, Version 2.0 (the License);
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an AS IS BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
 * @addtogroup Device_Settings_HALTEST Device Settings HAL Tests
 * @{
 */

/**
 * @defgroup DS_FPD_HALTEST Device Settings Front Panel Display HAL Tests
 * @{
 */

/**
 * @defgroup DS_FPD_HALTEST_L3 Device Settings Front Panel Display HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for DS Front Panel Display HAL :
 *
 * ## Module's Role
 * This module includes Level 3 functional test interfaces.
 * This Test Interface will provide the module functionalities with which the manual user or automation can create and execute the test cases.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [Device-Settings_front-panel_halSpec.md](../../docs/pages/ds-front-panel-display_High-Level_TestSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l3_dsFPD.c
 *
 */

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include <assert.h>
#include "dsFPD.h"

#define DS_FPD_KEY_SIZE 128
#define DS_ASSERT assert
#define UT_LOG_MENU_INFO UT_LOG_INFO

static int32_t gTestGroup = 3;
static int32_t gTestID = 1;
static int32_t eIndicator = 0;

const static ut_control_keyStringMapping_t dsFrontPanelErrorCodeTable[] = {
    {"dsERR_NONE", (int32_t)dsERR_NONE},
    {"dsERR_GENERAL", (int32_t)dsERR_GENERAL},
    {"dsERR_INVALID_PARAM", (int32_t)dsERR_INVALID_PARAM},
    {"dsERR_INVALID_STATE", (int32_t)dsERR_INVALID_STATE},
    {"dsERR_ALREADY_INITIALIZED", (int32_t)dsERR_ALREADY_INITIALIZED},
    {"dsERR_NOT_INITIALIZED", (int32_t)dsERR_NOT_INITIALIZED},
    {"dsERR_OPERATION_NOT_SUPPORTED", (int32_t)dsERR_OPERATION_NOT_SUPPORTED},
    {"dsERR_RESOURCE_NOT_AVAILABLE", (int32_t)dsERR_RESOURCE_NOT_AVAILABLE},
    {"dsERR_OPERATION_FAILED", (int32_t)dsERR_OPERATION_FAILED},
    {"dsErr_MAX", (int32_t)dsErr_MAX},
    {NULL, -1}};

const static ut_control_keyStringMapping_t dsFrontPanelIndicatorTable[] = {
    {"dsFPD_INDICATOR_MESSAGE", (int32_t)dsFPD_INDICATOR_MESSAGE},
    {"dsFPD_INDICATOR_POWER", (int32_t)dsFPD_INDICATOR_POWER},
    {"dsFPD_INDICATOR_RECORD", (int32_t)dsFPD_INDICATOR_RECORD},
    {"dsFPD_INDICATOR_REMOTE", (int32_t)dsFPD_INDICATOR_REMOTE},
    {"dsFPD_INDICATOR_RFBYPASS", (int32_t)dsFPD_INDICATOR_RFBYPASS},
    {"dsFPD_INDICATOR_MAX", (int32_t)dsFPD_INDICATOR_MAX},
    {NULL, -1}};

const static ut_control_keyStringMapping_t dsFrontPanelStateTable[] = {
    {"dsFPD_STATE_OFF", (int32_t)dsFPD_STATE_OFF},
    {"dsFPD_STATE_ON", (int32_t)dsFPD_STATE_ON},
    {"dsFPD_STATE_MAX", (int32_t)dsFPD_STATE_MAX},
    {NULL, -1}};
const static ut_control_keyStringMapping_t dsFrontPanelColorTable[] = {
    {"dsFPD_COLOR_BLUE", (int32_t)(0x0000FF)},
    {"dsFPD_COLOR_GREEN", (int32_t)(0x00FF00)},
    {"dsFPD_COLOR_RED", (int32_t)(0xFF0000)},
    {"dsFPD_COLOR_YELLOW", (int32_t)(0xFFFFE0)},
    {"dsFPD_COLOR_ORANGE", (int32_t)(0xFF8C00)},
    {"dsFPD_COLOR_WHITE", (int32_t)(0xFFFFFF)},
    {NULL, -1}};
const static ut_control_keyStringMapping_t dsFrontPanelLEDStateTable[] = {
    {"dsFPD_LED_DEVICE_NONE", (int32_t)dsFPD_LED_DEVICE_NONE},
    {"dsFPD_LED_DEVICE_ACTIVE", (int32_t)dsFPD_LED_DEVICE_ACTIVE},
    {"dsFPD_LED_DEVICE_STANDBY", (int32_t)dsFPD_LED_DEVICE_STANDBY},
    {"dsFPD_LED_DEVICE_WPS_CONNECTING", (int32_t)dsFPD_LED_DEVICE_WPS_CONNECTING},
    {"dsFPD_LED_DEVICE_WPS_CONNECTED", (int32_t)dsFPD_LED_DEVICE_WPS_CONNECTED},
    {"dsFPD_LED_DEVICE_WPS_ERROR", (int32_t)dsFPD_LED_DEVICE_WPS_ERROR},
    {"dsFPD_LED_DEVICE_FACTORY_RESET", (int32_t)dsFPD_LED_DEVICE_FACTORY_RESET},
    {"dsFPD_LED_DEVICE_USB_UPGRADE", (int32_t)dsFPD_LED_DEVICE_USB_UPGRADE},
    {"dsFPD_LED_DEVICE_SOFTWARE_DOWNLOAD_ERROR", (int32_t)dsFPD_LED_DEVICE_SOFTWARE_DOWNLOAD_ERROR},
    {"dsFPD_LED_DEVICE_MAX", (int32_t)dsFPD_LED_DEVICE_MAX},
    {NULL, -1}};

static void readAndDiscardRestOfLine(FILE *in)
{
    int32_t c;
    while ((c = fgetc(in)) != EOF && c != '\n')
        ;
}

static int32_t selectIndicator()
{

    UT_LOG_MENU_INFO(" \t  Supported Indicators are:");
    UT_LOG_MENU_INFO("------------------------------------------");
    for (int32_t indicator = 0; indicator <= dsFPD_INDICATOR_MAX; indicator++)
    {
        UT_LOG_INFO("\t%d.  %-20s\n", indicator, UT_Control_GetMapString(dsFrontPanelIndicatorTable, indicator));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Select Indicator : ");
    scanf("%d", &eIndicator);
    readAndDiscardRestOfLine(stdin);
    if (eIndicator < dsFPD_INDICATOR_MESSAGE || eIndicator >= dsFPD_INDICATOR_MAX)
    {
        UT_LOG_INFO("Unsupported Indicator %d", eIndicator);
        return 1;
    }
    return 0;
}

/**
* @brief Initialization of the Device settings - Front Panel Module
*
* This test provides a scope to Intialize the Front Panel module.

* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/

void test_l3_dsFPD_hal_Init(void)
{
    gTestID = 1;
    dsError_t status = dsERR_NONE;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    // Step 1: Call dsFPInit()
    UT_LOG_INFO("Calling dsFPInit()");
    status = dsFPInit();
    UT_LOG_INFO("Result dsFPInit() dsError_t:[%s]", UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to set Front panel LED state.
 *
 * This test case provides a scope to  set the State of FP indicator to ON and OFF.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 2 and provide indicator index and State to be set.
 *
 */
void test_l3_dsFPD_hal_SetFPState(void)
{
    gTestID = 2;
    int32_t ret = 0;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status = dsERR_NONE;
    int32_t iState = 0;

    ret = selectIndicator();
    if (ret)
    {
        goto exit;
    }
    UT_LOG_MENU_INFO(" \t  Supported Front Panel States are:");
    UT_LOG_MENU_INFO("------------------------------------------");
    for (int32_t state = 0; state < dsFPD_STATE_MAX; state++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-20s\n", state, UT_Control_GetMapString(dsFrontPanelStateTable, state));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Select State : ");
    scanf("%d", &iState);
    readAndDiscardRestOfLine(stdin);
    if (iState < 0 || iState >= dsFPD_STATE_MAX)
    {
        UT_LOG_INFO("Unsupported State input : %u", iState);
        goto exit;
    }

    /* Check that the Indicator is valid */
    UT_LOG_INFO("Calling dsSetFPState(IN:Indicator:[%d], IN:State:[%d]", eIndicator, iState);
    status = dsSetFPState((dsFPDIndicator_t)eIndicator, (dsFPDState_t)iState);
    UT_LOG_INFO("Result dsSetFPState(IN:Indicator:[%d], IN:State:[%d]) \
                                dsError_t:[%s]",
                eIndicator, iState,
                UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
 * @brief This test provides a scope to get Front panel LED state.
 *
 * This test case provides a scope to  get the State of FP indicator to ON and OFF.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 3 and provide indicator index.
 *
 */
void test_l3_dsFPD_hal_GetFPState(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;
    dsFPDState_t eState = 0;
    int32_t ret = 0;

    ret = selectIndicator();
    if (ret)
    {
        goto exit;
    }
    UT_LOG_INFO("Calling dsGetFPState(IN:Indicator:[%d], OUT:State[])", eIndicator);
    status = dsGetFPState((dsFPDIndicator_t)eIndicator, &eState);
    UT_LOG_INFO("Result dsGetFPState(IN:Indicator:[%d], OUT:State:[%s]) dsError_t:[%s])", eIndicator, UT_Control_GetMapString(dsFrontPanelStateTable, eState), UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
/* Check that the Indicator is valid */
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to set Front panel LED Blink functionality.
 *
 * This test case provides a scope to  set the Blink duration and iteration
 * of FP indicator.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel idicator state should be set to On through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 4 and provide indicator type, Blink duration
 * and Blink iteration.
 *
 */
void test_l3_dsFPD_hal_SetFPBlink(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int32_t ret = 0;
    dsError_t status = dsERR_NONE;
    uint32_t uBlinkDuration = 0;
    uint32_t uBlinkIterations = 0;

    ret = selectIndicator();
    if (ret)
        goto exit;
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Enter Blink Duration in ms: ");
    scanf("%u", &uBlinkDuration);
    readAndDiscardRestOfLine(stdin);

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Enter Blink iteration: ");
    scanf("%u", &uBlinkIterations);
    readAndDiscardRestOfLine(stdin);

    /* Check that the Indicator is valid */
    UT_LOG_INFO("Calling dsSetFPBlink(IN:Indicator:[%d], IN:Blink Duration in ms:[%d], IN:Blink Iteration:[%d])", eIndicator, uBlinkDuration, uBlinkIterations);
    status = dsSetFPBlink((dsFPDIndicator_t)eIndicator, uBlinkDuration, uBlinkIterations);
    UT_LOG_INFO("Result dsSetFPBlink(IN:Indicator:[%d], IN:Blink Duration in ms:[%d], IN:Blink Iteration:[%d]) \
                            dsError_t:[%s]",
                eIndicator, uBlinkDuration, uBlinkIterations,
                UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to set Front panel LED Brightness functionality.
 *
 * This test case provides a scope to  set the Brightness of selected FP indicator.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel indicator state should be set to ON through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 5 and provide indicator index and Brightness value.
 *
 */
void test_l3_dsFPD_hal_SetFPBrightness(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int32_t ret = 0;
    dsError_t status = dsERR_NONE;
    uint32_t uBrightness = 0;

    ret = selectIndicator();
    if (ret)
        goto exit;
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Enter LED Brightness (Range: 0-100): ");
    scanf("%u", &uBrightness);
    readAndDiscardRestOfLine(stdin);
    if (uBrightness < 0 || uBrightness > 100)
    {
        UT_LOG_INFO("Unsupported Brightness value : %u", uBrightness);
        goto exit;
    }
    /* Check that the Indicator is valid */
    UT_LOG_INFO("Calling dsSetFPBrightness(IN:Indicator:[%d], IN:Brightness:[%d])", eIndicator, uBrightness);
    status = dsSetFPBrightness((dsFPDIndicator_t)eIndicator, (dsFPDBrightness_t)uBrightness);
    UT_LOG_INFO("Result dsSetFPBrightness(IN:Indicator:[%d], IN:Brightness:[%d]) \
                            dsError_t:[%s]",
                eIndicator, uBrightness,
                UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to get Front panel LED Brightness.
 *
 * This test case provides a scope to  get the brightness of given Indicator.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel idicator state should be set to On through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 6 and provide indicator index.
 *
 */
void test_l3_dsFPD_hal_GetFPBrightness(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int32_t ret = 0;
    dsError_t status = dsERR_NONE;
    dsFPDBrightness_t brightness = 0;

    ret = selectIndicator();
    if (ret)
        goto exit;
    UT_LOG_INFO("Calling dsGetFPBrightness(IN:Indicator:[%d], OUT:Brightness[])", eIndicator);
    status = dsGetFPBrightness((dsFPDIndicator_t)eIndicator, &brightness);
    UT_LOG_INFO("Result dsGetFPBrightness(IN:Indicator:[%d], OUT:Brightness:[%d]) dsError_t:[%s]", eIndicator, brightness,
                UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
/* Check that the Indicator is valid */
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to set Power LED state.
 *
 * This test case provides a scope to  set the Power LED to different FP States.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel indicator state should be set to ON through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 7 and provide the FP state to check.
 *
 */
void test_l3_dsFPD_hal_FPGetSupportedLEDStates(void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status = dsERR_NONE;
    uint32_t uLedStates = 0;

    UT_LOG_INFO("Calling dsFPGetSupportedLEDStates(OUT:states[])");
    status = dsFPGetSupportedLEDStates(&uLedStates);
    UT_LOG_INFO("Result dsFPGetSupportedLEDStates(OUT:states:[0x%08X])dsError_t:[%s]", uLedStates, UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);

    /* Check that the Indicator is valid */
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to set Power LED state.
 *
 * This test case provides a scope to  set the Power LED to different FP States.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel indicator state should be set to ON through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 7 and provide the FP state to check.
 *
 */
void test_l3_dsFPD_hal_SetLEDState(void)
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;
    int32_t iLedState = 0;

    UT_LOG_MENU_INFO(" \t  Supported POWER LED States are:");
    UT_LOG_MENU_INFO("------------------------------------------");

    for (int32_t fpState = dsFPD_LED_DEVICE_NONE; fpState < dsFPD_LED_DEVICE_MAX; fpState++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-20s\n", fpState, UT_Control_GetMapString(dsFrontPanelLEDStateTable, fpState));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Select State: ");
    scanf("%d", &iLedState);
    readAndDiscardRestOfLine(stdin);
    if (iLedState < dsFPD_LED_DEVICE_NONE || iLedState >= dsFPD_LED_DEVICE_MAX)
    {
        UT_LOG_INFO("Unsupported LED State");
        goto exit;
    }

    /* Check that the Indicator is valid */
    UT_LOG_INFO("Calling dsFPSetLEDState(IN state:[%d])", iLedState);
    status = dsFPSetLEDState((dsFPDLedState_t)iLedState);
    UT_LOG_INFO("Result dsFPSetLEDState(IN state:[%s]) dsError_t:[%s]", \
                    UT_Control_GetMapString(dsFrontPanelLEDStateTable,iLedState), \
                    UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to get POWER LED State.
 *
 * This test case provides a scope to  get the State of FP Power LED.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel idicator state should be set to On through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 8 and provide indicator index.
 *
 */
void test_l3_dsFPD_hal_GetLEDState(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;
    dsFPDLedState_t state = 0;

    UT_LOG_INFO("Calling dsFPGetLEDState(OUT:FP LED State[])");
    status = dsFPGetLEDState(&state);
    UT_LOG_INFO("Result dsFPGetLEDState(OUT:FP LED State:[%s]) dsError_t:[%s]", \
                        UT_Control_GetMapString(dsFrontPanelLEDStateTable, state), \
                        UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
    /* Check that the Indicator is valid */

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to set LED indicator with suported colors.
 *
 * This test case provides a scope to  set the LED indicators to different Supported Colors.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel indicator state should be set to ON through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 9 and provide indicator value and Color value.
 *
 */
void test_l3_dsFPD_hal_SetFPColor(void)
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int32_t ret = 0;
    dsError_t status = dsERR_NONE;
    uint32_t uColor = 0xffffff; // White color as default value
    bool isValidColor = false;

    ret = selectIndicator();
    if (ret)
        goto exit;
    UT_LOG_MENU_INFO(" \t  Supported Front Panel color for the indicator %s are:", UT_Control_GetMapString(dsFrontPanelIndicatorTable, eIndicator));
    UT_LOG_MENU_INFO("------------------------------------------");

    for (int32_t j = 0; j < dsFPD_COLOR_MAX; j++)
    {
        UT_LOG_MENU_INFO("\t0X%06X.  %-20s\n", dsFrontPanelColorTable[j].value, dsFrontPanelColorTable[j].string);
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Select Color: ");
    scanf("%06X", &uColor);
    readAndDiscardRestOfLine(stdin);
    isValidColor = false;
    for (int32_t j = 0; j < dsFPD_COLOR_MAX; j++)
    {
        if (dsFrontPanelColorTable[j].value == uColor)
            isValidColor = true;
    }
    if (!isValidColor)
    {
        UT_LOG_INFO("Unsupported Color Input 0x%06X", uColor);
        goto exit;
    }

    /* Check that the Indicator is valid */
    UT_LOG_INFO("Calling dsSetFPColor(IN:Indicator:[%d], IN:Color:[0x%06X])", eIndicator, uColor);
    status = dsSetFPColor((dsFPDIndicator_t)eIndicator, uColor);
    UT_LOG_INFO("Result dsSetFPColor(IN:Indicator:[%d], IN:Color:[%s]) dsError_t:[%s]", \
                        eIndicator, UT_Control_GetMapString(dsFrontPanelColorTable,uColor), \
                        UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to get Color of the Indicator set.
 *
 * This test case provides a scope to  get the Color of Given indicator
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 * Front panel indicator state should be set to ON through Test 2 before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 10 and provide indicator index.
 *
 */
void test_l3_dsFPD_hal_GetFPColor(void)
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;
    dsFPDColor_t color = dsFPD_COLOR_WHITE;
    int32_t ret = 0;

    ret = selectIndicator();
    if (ret)
        goto exit;
    UT_LOG_INFO("Calling dsGetFPColor(IN:Indicator:[%d], OUT:Color[])", eIndicator);
    status = dsGetFPColor((dsFPDIndicator_t)eIndicator, &color);
    UT_LOG_INFO("Result dsGetFPColor(IN:Indicator:[%d], OUT:Color:[%s]) dsError_t:[%s]", eIndicator\
                , UT_Control_GetMapString(dsFrontPanelColorTable,color), UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);
/* Check that the Indicator is valid */
exit:
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}
/**
 * @brief This test provides a scope to Terimnate Device Settings Front panel Module.
 *
 * This test case provides a scope to terminate front panel module.
 *
 * **Pre-Conditions:** @n
 * Front panel Module should be intialized through Test 1 before calling this test.
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should select the Test 11.
 *
 */

void test_l3_dsFPD_hal_Term(void)
{
    gTestID = 12;
    dsError_t status = dsERR_NONE;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 1: Call dsFPInit()
    UT_LOG_INFO("Calling dsFPTerm()");
    status = dsFPTerm();
    UT_LOG_INFO("Result dsFPTerm()dsError_t:[%s]", UT_Control_GetMapString(dsFrontPanelErrorCodeTable, status));
    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t *pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int32_t - 0 on success, otherwise failure
 */
int32_t test_l3_dsFPD_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L3 Front Panel Functions] ", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings

    UT_add_test(pSuite, "Initialize dsFPD", test_l3_dsFPD_hal_Init);
    UT_add_test(pSuite, "Set Front Panel Indicator State", test_l3_dsFPD_hal_SetFPState);
    UT_add_test(pSuite, "Get Front Panel Indicator State", test_l3_dsFPD_hal_GetFPState);
    UT_add_test(pSuite, "Blink Front Panel Indicator", test_l3_dsFPD_hal_SetFPBlink);
    UT_add_test(pSuite, "Set Front Panel Indicaor Brightness", test_l3_dsFPD_hal_SetFPBrightness);
    UT_add_test(pSuite, "Get Front Panel Indicaor Brightness", test_l3_dsFPD_hal_GetFPBrightness);
    UT_add_test(pSuite, "Get Supported Front Panel State Patterns", test_l3_dsFPD_hal_FPGetSupportedLEDStates);
    UT_add_test(pSuite, "Set Front Panel State Pattern", test_l3_dsFPD_hal_SetLEDState);
    UT_add_test(pSuite, "Get Front Panel State Pattern", test_l3_dsFPD_hal_GetLEDState);
    UT_add_test(pSuite, "Set Front Panel Indicator Color", test_l3_dsFPD_hal_SetFPColor);
    UT_add_test(pSuite, "Get Front Panel Indicator Color", test_l3_dsFPD_hal_GetFPColor);
    UT_add_test(pSuite, "Terminate dsFPD", test_l3_dsFPD_hal_Term);

    return 0;
}

/** @} */ // End of DS_FPD_HALTEST_L3
/** @} */ // End of DS_FPD_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
