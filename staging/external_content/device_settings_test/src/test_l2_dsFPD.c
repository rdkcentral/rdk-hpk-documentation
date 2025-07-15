/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:*
* Copyright 2024 RDK Management
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
 * @addtogroup Device_Settings_HALTEST Device Settings HAL Tests
 * @{
 */

/**
 * @addtogroup DS_FPD_HALTEST Device Settings Front Panel Display HAL Tests
 * @{
 */

/**
 * @defgroup DS_FPD_HALTEST_L2 Device Settings Front Panel Display HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for DS Front Panel Display HAL :
 *
 * Level 2 unit test cases for all APIs of Device Settings Front Panel Display HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [dsFPD_halSpec.md](../../docs/pages/ds-front-panel-display_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l2_dsFPD.c
 *
 */

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dsFPD.h"
#include <stdlib.h>

#define DS_FPD_KEY_SIZE 64

static int gTestGroup = 2;
static int gTestID = 1;

/**
* @brief This test function is designed to test the functionality of the Front Panel Display (FPD) indicators.
*
* This test function iterates over the supported FPD indicators, gets their current state, sets their state to ON
* if it's OFF, sets the brightness of the LED, verifies the brightness, and finally terminates the FPD sub-module.
* The purpose of this test is to ensure that the FPD indicators are functioning as expected and that the associated
* functions are working correctly.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_SetFPstateON_SetBrightness(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    dsFPDState_t state;
    dsFPDBrightness_t brightness;
    dsFPDBrightness_t getbrightness;
    dsFPDBrightness_t minBrightness;
    dsFPDBrightness_t maxBrightness;
    dsFPDIndicator_t eIndicator;
    int count = 0;
    char buffer[DS_FPD_KEY_SIZE];
    char minbuffer[DS_FPD_KEY_SIZE], maxbuffer[DS_FPD_KEY_SIZE];
    ut_kvp_instance_t *pInstance = NULL;

    // Initialize the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPInit()");
    ret = dsFPInit();
    UT_LOG_DEBUG("dsFPInit returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    pInstance = ut_kvp_profile_getInstance();
    count = ut_kvp_getUInt32Field(pInstance,"dsFPD/Number_of_Indicators");
    for (int i=1;i<=count;i++)
    {
        snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/Indicator_Type", i);
        eIndicator = ut_kvp_getUInt32Field( pInstance, buffer);

        // Get the current state of the FP
        UT_LOG_DEBUG("Invoking dsGetFPState() with eIndicator=%d", eIndicator);
        ret = dsGetFPState(eIndicator, &state);
        UT_LOG_DEBUG("dsGetFPState() returned state=%d and status=%d", state, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetFPState failed with status: %d", ret);
            continue;
        }

        // If the current state of the FP is OFF, set it to ON
        if (state == dsFPD_STATE_OFF)
        {
            UT_LOG_DEBUG("Invoking dsSetFPState() with eIndicator=%d and state=dsFPD_STATE_ON", eIndicator);
            ret = dsSetFPState(eIndicator, dsFPD_STATE_ON);
            UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
                continue;
            }
        }

        // Set the brightness of the LED
        snprintf(minbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/MIN_BRIGHTNESS",i);
        minBrightness = ut_kvp_getUInt32Field( pInstance, minbuffer);
        snprintf(maxbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/MAX_BRIGHTNESS",i);
        maxBrightness = ut_kvp_getUInt32Field( pInstance, maxbuffer);
        brightness = minBrightness + rand() % (maxBrightness+1 - minBrightness);
        UT_LOG_DEBUG("Invoking dsSetFPBrightness() with eIndicator=%d and eBrightness=%d", eIndicator, brightness);
        ret = dsSetFPBrightness(eIndicator, brightness);
        UT_LOG_DEBUG("dsSetFPBrightness() returned status=%d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsSetFPBrightness failed with status: %d", ret);
            continue;
        }

        // Verify the brightness of the LED
        UT_LOG_DEBUG("Invoking dsGetFPBrightness() with eIndicator=%d", eIndicator);
        ret = dsGetFPBrightness(eIndicator, &getbrightness);
        UT_LOG_DEBUG("dsGetFPBrightness() returned brightness=%d and status=%d", getbrightness, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(brightness, getbrightness);
        if (ret != dsERR_NONE || brightness != getbrightness)
        {
            UT_LOG_ERROR("Mismatch in set and get brightness");
        }
    }

    // Terminate the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPTerm()");
    ret = dsFPTerm();
    UT_LOG_DEBUG("TvTerm returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the functionality of dsFPD_IterateAndSetFPDIndicators
*
* This test case verifies the functionality of dsFPD_IterateAndSetFPDIndicators API.
* It checks if the API is able to iterate over the supported FPD indicators and set
* the state and brightness of each indicator correctly.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_SetFPstateOFF_SetBrightness(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    dsFPDState_t state;
    dsFPDBrightness_t eBrightness;
    dsFPDIndicator_t eIndicator;
    int count = 0;
    char buffer[DS_FPD_KEY_SIZE];
    ut_kvp_instance_t *pInstance = NULL;

    // Initialize the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPInit()");
    ret = dsFPInit();
    UT_LOG_DEBUG("dsFPInit returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    pInstance = ut_kvp_profile_getInstance();
    count = ut_kvp_getUInt32Field(pInstance,"dsFPD/Number_of_Indicators");
    for (int i=1;i<=count;i++)
    {
        snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/Indicator_Type", i);
        eIndicator = ut_kvp_getUInt32Field( pInstance, buffer);

        // Get the current state of the FP
        UT_LOG_DEBUG("Invoking dsGetFPState() with eIndicator=%d", eIndicator);
        ret = dsGetFPState(eIndicator, &state);
        UT_LOG_DEBUG("dsGetFPState() returned state=%d and status=%d", state, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetFPState failed with status: %d", ret);
            continue;
        }

        // If the current state of the FP is ON, set it to OFF
        if (state == dsFPD_STATE_ON)
        {
            UT_LOG_DEBUG("Invoking dsSetFPState() with eIndicator=%d and state=dsFPD_STATE_OFF", eIndicator);
            ret = dsSetFPState(eIndicator, dsFPD_STATE_OFF);
            UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
                continue;
            }
        }

        // Set the brightness of the discrete LED (0-100)
        eBrightness = rand() % 101;
        UT_LOG_DEBUG("Invoking dsSetFPBrightness() with eIndicator=%d and eBrightness=%d", eIndicator, eBrightness);
        ret = dsSetFPBrightness(eIndicator, eBrightness);
        UT_LOG_DEBUG("dsSetFPBrightness() returned status=%d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_OPERATION_NOT_SUPPORTED);
        if (ret != dsERR_OPERATION_NOT_SUPPORTED)
        {
            UT_LOG_ERROR("dsSetFPBrightness failed with status: %d", ret);
        }
    }

    // Terminate the Front Panel Display sub-module
    UT_LOG_DEBUG("Invoking dsFPTerm()");
    ret = dsFPTerm();
    UT_LOG_DEBUG("dsFPTerm() returned status=%d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test function is designed to test the functionality of the dsFPD_IterateAndSetFPDIndicators API
*
* This test function invokes the dsFPD_IterateAndSetFPDIndicators API and checks
*if the API is able to iterate and set the FPD indicators correctly. The test
* function also checks if the API returns the correct error codes when expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_SetFPstateOFF_SetBlink(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    dsFPDState_t state;
    dsFPDIndicator_t eIndicator;
    int count = 0;
    char buffer[DS_FPD_KEY_SIZE];
    ut_kvp_instance_t *pInstance = NULL;

    // Initialize the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPInit()");
    ret = dsFPInit();
    UT_LOG_DEBUG("dsFPInit returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    pInstance = ut_kvp_profile_getInstance();
    count = ut_kvp_getUInt32Field(pInstance,"dsFPD/Number_of_Indicators");
    for (int i=1;i<=count;i++)
    {
        snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/Indicator_Type", i);
        eIndicator = ut_kvp_getUInt32Field( pInstance, buffer);

        // Get the current state of the FP
        UT_LOG_DEBUG("Invoking dsGetFPState() with eIndicator=%d", eIndicator);
        ret = dsGetFPState(eIndicator, &state);
        UT_LOG_DEBUG("dsGetFPState() returned state=%d and status=%d", state, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetFPState failed with status: %d", ret);
            continue;
        }

        // If the current state of the FP is ON, set it to OFF
        if (state == dsFPD_STATE_ON)
        {
            UT_LOG_DEBUG("Invoking dsSetFPState() with eIndicator=%d and state=dsFPD_STATE_OFF", eIndicator);
            ret = dsSetFPState(eIndicator, dsFPD_STATE_OFF);
            UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
                continue;
            }
        }

        UT_LOG_DEBUG("Invoking dsSetFPBlink() with eIndicator=%d, uBlinkDuration= 500, uBlinkIterations= 10", eIndicator);
        ret = dsSetFPBlink(eIndicator, 500, 10);
        UT_LOG_DEBUG("dsSetFPBlink() returned status=%d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_OPERATION_NOT_SUPPORTED);
        if (ret != dsERR_OPERATION_NOT_SUPPORTED)
        {
            UT_LOG_ERROR("dsSetFPBlink() failed with status=%d", ret);

        }
    }
    UT_LOG_DEBUG("Invoking dsFPTerm()");
    ret = dsFPTerm();
    UT_LOG_DEBUG("dsFPTerm() returned status=%d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test function is designed to iterate and set FPD indicators
*
* This test function iterates over all FPD indicators and sets their state and color.
* It tests the functionality of dsFPInit, dsGetFPState, dsSetFPState, dsSetFPColor,
* dsGetFPColor, and dsFPTerm functions. The test ensures that these functions return
* the expected results and the FPD indicators are set correctly.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_SetFPstateON_Multi_SetColor(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    dsFPDState_t state;
    dsFPDColor_t color;
    dsFPDColor_t getcolor;
    dsFPDIndicator_t eIndicator;
    int count = 0;
    int mode = 0;
    int numOfSupportedColors = 0;
    char buffer[DS_FPD_KEY_SIZE];
    char colorbuffer[DS_FPD_KEY_SIZE];
    char supportedColorbuffer[DS_FPD_KEY_SIZE];
    ut_kvp_instance_t *pInstance = NULL;

    // Initialize the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPInit()");
    ret = dsFPInit();
    UT_LOG_DEBUG("dsFPInit returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    pInstance = ut_kvp_profile_getInstance();
    count = ut_kvp_getUInt32Field(pInstance,"dsFPD/Number_of_Indicators");
    for (int i=1;i<=count;i++)
    {
        snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/Indicator_Type", i);
        eIndicator = ut_kvp_getUInt32Field( pInstance, buffer);

        // Get the current state of the FP
        UT_LOG_DEBUG("Invoking dsGetFPState() with eIndicator=%d", eIndicator);
        ret = dsGetFPState(eIndicator, &state);
        UT_LOG_DEBUG("dsGetFPState() returned state=%d and status=%d", state, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetFPState failed with status: %d", ret);
            continue;
        }

        // If the current state of the FP is OFF, set it to ON
        if (state == dsFPD_STATE_OFF)
        {
            UT_LOG_DEBUG("Invoking dsSetFPState() with eIndicator=%d and state=dsFPD_STATE_ON", eIndicator);
            ret = dsSetFPState(eIndicator, dsFPD_STATE_ON);
            UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
                continue;
            }
        }

        //Check the indicator supports multi-colored
        snprintf(colorbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/DEFAULT_COLOR_MODE",i);
        mode = ut_kvp_getUInt32Field(pInstance,colorbuffer);
        UT_LOG_DEBUG("Default color mode from profile file : %d", mode);

        //set and get different colors supported by the discrete `LED`
        if (mode == 1)
        {
            UT_LOG_DEBUG("The indicator supports multicolored");
            snprintf(supportedColorbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/supportedColors",i);
            numOfSupportedColors = ut_kvp_getListCount(pInstance,supportedColorbuffer);
            for (int j=0;j<numOfSupportedColors;j++)
            {
                snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/supportedColors/%d", i, j);
                color = (dsFPDColor_t)ut_kvp_getUInt32Field( pInstance, buffer);
                UT_LOG_DEBUG("Invoking dsSetFPColor with eIndicator: %d and color: 0x%X", eIndicator, color);
                ret = dsSetFPColor(eIndicator, color);
                UT_LOG_DEBUG("Return status: %d", ret);
                UT_ASSERT_EQUAL(ret, dsERR_NONE);
                if (ret != dsERR_NONE)
                {
                    UT_LOG_ERROR("dsSetFPColor failed with status: %d", ret);
                    continue;
                }

                ret = dsGetFPColor(eIndicator, &getcolor);
                UT_LOG_DEBUG("Invoking dsGetFPColor with eIndicator: %d", eIndicator);
                UT_LOG_DEBUG("Color: 0x%X and return status: %d", getcolor, ret);
                UT_ASSERT_EQUAL(ret, dsERR_NONE);
                UT_ASSERT_EQUAL(color, getcolor);
                if (ret != dsERR_NONE || color != getcolor)
                {
                    UT_LOG_ERROR("Mismatch in set and get color");
                }
            }
        }
        else
        {
            UT_LOG_DEBUG("The indicator supports only single color and Set Color not supported");
        }

        ret = dsSetFPState(eIndicator, dsFPD_STATE_OFF);
        UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
            continue;
        }

    }

    ret = dsFPTerm();
    UT_LOG_DEBUG("Invoking dsFPTerm");
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to iterate over the supported Front Panel Display (FPD) indicators and check their current state
*
* This test function iterates over the supported Front Panel Display (FPD) indicators
* and checks their current state. If the state is ON, it sets it to OFF and then tries
* to set the color functionality of the discrete LED. It checks that the dsSetFPColor()
* API returns `dsERR_OPERATION_NOT_SUPPORTED`. Finally, it terminates the Front Panel Display sub-module.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_SetFPstateOFF_SetColor(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    dsFPDState_t state;
    dsFPDColor_t eColor;
    dsFPDIndicator_t eIndicator;
    int count = 0;
    int numOfSupportedColors = 0;
    char buffer[DS_FPD_KEY_SIZE];
    ut_kvp_instance_t *pInstance = NULL;
    char supportedColorbuffer[DS_FPD_KEY_SIZE];

    // Initialize the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPInit()");
    ret = dsFPInit();
    UT_LOG_DEBUG("dsFPInit returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    pInstance = ut_kvp_profile_getInstance();
    count = ut_kvp_getUInt32Field(pInstance,"dsFPD/Number_of_Indicators");
    for (int i=1;i<=count;i++)
    {
        snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/Indicator_Type", i);
        eIndicator = ut_kvp_getUInt32Field( pInstance, buffer);

        // Get the current state of the FP
        UT_LOG_DEBUG("Invoking dsGetFPState() with eIndicator=%d", eIndicator);
        ret = dsGetFPState(eIndicator, &state);
        UT_LOG_DEBUG("dsGetFPState() returned state=%d and status=%d", state, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetFPState failed with status: %d", ret);
            continue;
        }

        // If the current state of the FP is ON, set it to OFF
        if (state == dsFPD_STATE_ON)
        {
            UT_LOG_DEBUG("Invoking dsSetFPState() with eIndicator=%d and state=dsFPD_STATE_OFF", eIndicator);
            ret = dsSetFPState(eIndicator, dsFPD_STATE_OFF);
            UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
                continue;
            }
        }

        snprintf(supportedColorbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/supportedColors",i);
        numOfSupportedColors = ut_kvp_getListCount(pInstance,supportedColorbuffer);
        for (int j=0;j<numOfSupportedColors;j++)
        {
            snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/supportedColors/%d", i, j);
            eColor = (dsFPDColor_t)ut_kvp_getUInt32Field( pInstance, buffer);
            UT_LOG_DEBUG("Invoking dsSetFPColor with eIndicator: %d and color: %d", eIndicator, eColor);
            ret = dsSetFPColor(eIndicator, eColor);
            UT_LOG_DEBUG("Return status: %d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_OPERATION_NOT_SUPPORTED);
            if (ret != dsERR_OPERATION_NOT_SUPPORTED)
            {
                UT_LOG_ERROR("dsSetFPColor failed with status: %d", ret);
                continue;
            }
        }
    }

    UT_LOG_DEBUG("Invoking dsFPTerm");
    ret = dsFPTerm();
    UT_LOG_DEBUG("dsFPTerm returned status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to iterate and check FPD indicators
*
* This test function iterates over all FPD indicators and checks their state and color.
* It tests the functionality of dsFPInit(), dsGetFPState(), dsSetFPState(), dsSetFPColor(),
* and dsFPTerm() functions. The test is designed to ensure that these functions are working
* as expected and handling the FPD indicators correctly.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 006@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_SetFPstateON_Single_SetColor(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    dsFPDState_t state;
    dsFPDColor_t color;
    dsFPDIndicator_t eIndicator;
    int count = 0;
    int mode = 0;
    int numOfSupportedColors = 0;
    char buffer[DS_FPD_KEY_SIZE];
    ut_kvp_instance_t *pInstance = NULL;
    char colorbuffer[DS_FPD_KEY_SIZE];
    char supportedColorbuffer[DS_FPD_KEY_SIZE];

    // Initialize the Front Panel Display (FPD) sub-module
    UT_LOG_DEBUG("Invoking dsFPInit()");
    ret = dsFPInit();
    UT_LOG_DEBUG("dsFPInit returned : %d",ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    pInstance = ut_kvp_profile_getInstance();
    count = ut_kvp_getUInt32Field(pInstance,"dsFPD/Number_of_Indicators");
    for (int i=1;i<=count;i++)
    {
        snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/Indicator_Type", i);
        eIndicator = ut_kvp_getUInt32Field( pInstance, buffer);

        // Get the current state of the FP
        UT_LOG_DEBUG("Invoking dsGetFPState() with eIndicator=%d", eIndicator);
        ret = dsGetFPState(eIndicator, &state);
        UT_LOG_DEBUG("dsGetFPState() returned state=%d and status=%d", state, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetFPState failed with status: %d", ret);
            continue;
        }

        // If the current state of the FP is OFF, set it to ON
        if (state == dsFPD_STATE_OFF)
        {
            UT_LOG_DEBUG("Invoking dsSetFPState() with eIndicator=%d and state=dsFPD_STATE_ON", eIndicator);
            ret = dsSetFPState(eIndicator, dsFPD_STATE_ON);
            UT_LOG_DEBUG("dsSetFPState() returned status=%d", ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetFPState failed with status: %d", ret);
                continue;
            }
        }
        //Check the indicator supports single-coloured
        snprintf(colorbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/DEFAULT_COLOR_MODE",i);
        mode = ut_kvp_getUInt32Field(pInstance,colorbuffer);
        UT_LOG_DEBUG("Default color mode from profile file : %d", mode);

        //set the color functionality of discrete `LED`
        if (mode == 0)
        {
            UT_LOG_DEBUG("The indicator supports single colored");
            snprintf(supportedColorbuffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/supportedColors",i);
            numOfSupportedColors = ut_kvp_getListCount(pInstance,supportedColorbuffer);
            for (int j=0;j<numOfSupportedColors;j++)
            {
                snprintf(buffer, DS_FPD_KEY_SIZE, "dsFPD/SupportedFPDIndicators/%d/supportedColors/%d", i, j);
                color = (dsFPDColor_t)ut_kvp_getUInt32Field( pInstance, buffer);
                UT_LOG_DEBUG("Invoking dsSetFPColor with eIndicator: %d and color: %d", eIndicator, color);
                ret = dsSetFPColor(eIndicator, color);
                UT_LOG_DEBUG("Return status: %d", ret);
                UT_ASSERT_EQUAL(ret, dsERR_OPERATION_NOT_SUPPORTED);
                if (ret != dsERR_OPERATION_NOT_SUPPORTED)
                {
                    UT_LOG_ERROR("dsSetFPColor failed with status: %d", ret);
                }
            }
        }
        else
        {
            UT_LOG_DEBUG("The indicator supports multi color");
        }
    }

    ret = dsFPTerm();
    UT_LOG_DEBUG("Invoking dsFPTerm");
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test verifies the LED state transitions in the dsFPD module
*
* This test function invokes the necessary APIs in the dsFPD module to transition through
* different LED states and verifies their return status and output values. It also includes
* logging and error handling as per the rules.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 007@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-front-panel-display_L2_Low-Level_TestSpecification.md](../docs/pages/ds-front-panel-display_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsFPD_VerifyLEDStateTransitions(void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

   dsError_t ret = dsERR_NONE;
    unsigned int supportedStates;
    dsFPDLedState_t state, getState;

    UT_LOG_DEBUG("Invoking dsFPInit");
    ret = dsFPInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsFPGetSupportedLEDStates with valid buffer");
    ret = dsFPGetSupportedLEDStates(&supportedStates);
    UT_LOG_DEBUG("Supported states: %u, Return status: %d", supportedStates, ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_ASSERT_KVP_EQUAL_PROFILE_UINT32(supportedStates, "dsFPD/SupportedLEDStates");

    for (state = dsFPD_LED_DEVICE_NONE; state < dsFPD_LED_DEVICE_MAX; state++)
    {
        if (!(supportedStates & (1 << state)))
            continue;

        UT_LOG_DEBUG("Invoking dsFPSetLEDState with state: %d", state);
        ret = dsFPSetLEDState(state);
        UT_LOG_DEBUG("Return status: %d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsFPSetLEDState failed for state: %d", state);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsFPGetLEDState with valid buffer");
        ret = dsFPGetLEDState(&getState);
        UT_LOG_DEBUG("Current state: %d, Return status: %d", getState, ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsFPGetLEDState failed for state: %d", state);
            continue;
        }

        UT_ASSERT_EQUAL(state, getState);
        if (state != getState)
        {
            UT_LOG_ERROR("Mismatch in set and get state : %d", state);
        }
    }

    UT_LOG_DEBUG("Invoking dsFPTerm");
    ret = dsFPTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l2_dsFPD_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L2 dsFPD]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings

    UT_add_test( pSuite, "L2_dsFPD_SetFPstateON_SetBrightness", test_l2_dsFPD_SetFPstateON_SetBrightness);
    UT_add_test( pSuite, "L2_dsFPD_SetFPstateOFF_SetBrightness", test_l2_dsFPD_SetFPstateOFF_SetBrightness);
    UT_add_test( pSuite, "L2_dsFPD_SetFPstateOFF_SetBlink", test_l2_dsFPD_SetFPstateOFF_SetBlink);
    UT_add_test( pSuite, "L2_dsFPD_SetFPstateON_Multi_SetColor", test_l2_dsFPD_SetFPstateON_Multi_SetColor);
    UT_add_test( pSuite, "L2_dsFPD_SetFPstateOFF_SetColor", test_l2_dsFPD_SetFPstateOFF_SetColor);
    UT_add_test( pSuite, "L2_dsFPD_SetFPstateON_Single_SetColor", test_l2_dsFPD_SetFPstateON_Single_SetColor);
    UT_add_test( pSuite, "L2_dsFPD_VerifyLEDStateTransitions", test_l2_dsFPD_VerifyLEDStateTransitions);

    return 0;
}

/** @} */ // End of DS_FPD_HALTEST_L2
/** @} */ // End of DS_FPD_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK