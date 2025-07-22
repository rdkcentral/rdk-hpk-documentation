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
 * @addtogroup POWER_MANAGER Power Manager Module
 * @{
 */

/**
 * @addtogroup POWER_MANAGER_HALTEST Power Manager HAL Tests
 * @{
 */

/**
 * @defgroup PLAT_POWER_HALTEST_L2 Power Manager HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for Power Manager HAL :
 *
 * Level 2 unit test cases for all APIs of Power Manager HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [power-manager_halSpec.md](../../docs/pages/power-manager_halSpec.md)
 *
 * @endparblock
 */
/**
* @file test_l2_plat_power.c
*
*/

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "plat_power.h"

#define POWER_MANAGER_KEY_SIZE 50

static int gTestGroup = 2;
static int gTestID = 1;

/**
* @brief Test for setting and getting power state in the power management module
*
* This test function first initializes the power management module using `PLAT_INIT` and checks if it returns `PWRMGR_SUCCESS`.
* Then it sets and gets the power state for each power state defined in `PWRMgr_PowerState_t` using `PLAT_API_SetPowerState` and
* `PLAT_API_GetPowerState` respectively, and checks if the set and get operations are successful and the power state is as expected. Finally, it terminates the power management module using `PLAT_TERM` and checks if it returns `PWRMGR_SUCCESS`.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [plat_power_L2_Low-Level_TestSpecification.md](../docs/pages/power-manager_L2_Low-Level_TestSpec.md)
*/

void test_l2_plat_power_SetAndGetPowerState(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    pmStatus_t status = PWRMGR_SUCCESS;
    PWRMgr_PowerState_t powerState = PWRMGR_POWERSTATE_MAX, getState = PWRMGR_POWERSTATE_MAX;
    char keyvalue[POWER_MANAGER_KEY_SIZE] = {0};
    uint8_t countOfPowerState = 0;

    UT_LOG_DEBUG("Invoking PLAT_INIT");
    status = PLAT_INIT();
    UT_LOG_DEBUG("Return status: %d", status);
    UT_ASSERT_EQUAL_FATAL(status, PWRMGR_SUCCESS);

    countOfPowerState = ut_kvp_getUInt8Field(ut_kvp_profile_getInstance(), "powermanager.CountOfPowerState");
    UT_LOG_DEBUG ("Count Of Power States %d" , countOfPowerState);

    for(int i = 0; i < countOfPowerState; i++){
        snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager.PowerStates/%d", i);
        powerState = ut_kvp_getUInt8Field(ut_kvp_profile_getInstance(), keyvalue);

        UT_LOG_DEBUG("Invoking PLAT_API_SetPowerState with powerState: %d", powerState);
        status = PLAT_API_SetPowerState(powerState);
        UT_LOG_DEBUG("Return status: %d", status);
        UT_ASSERT_EQUAL(status, PWRMGR_SUCCESS);
        if (status != PWRMGR_SUCCESS)
        {
            UT_LOG_ERROR("Failed to set power state");
            continue;
        }

        UT_LOG_DEBUG("Invoking PLAT_API_GetPowerState");
        status = PLAT_API_GetPowerState(&getState);
        UT_LOG_DEBUG("Return powerState: %d, status: %d", getState, status);
        UT_ASSERT_EQUAL(status, PWRMGR_SUCCESS);

        UT_ASSERT_EQUAL(powerState, getState);
    }

    UT_LOG_DEBUG("Invoking PLAT_TERM");
    status = PLAT_TERM();
    UT_LOG_DEBUG("Return status: %d", status);
    UT_ASSERT_EQUAL_FATAL(status, PWRMGR_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for setting and getting the wakeup source in the platform power management module
*
* This test verifies the functionality of the SetWakeupSrc and GetWakeupSrc functions in the platform power management module.
* It checks if the wakeup source can be successfully set and retrieved, and if the correct status is returned for each operation.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [plat_power_L2_Low-Level_TestSpecification.md](../docs/pages/power-manager_L2_Low-Level_TestSpec.md)
*/

void test_l2_plat_power_SetAndGetWakeupSrc(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    pmStatus_t status = PWRMGR_SUCCESS;
    bool enable = false;
    PWRMGR_WakeupSrcType_t srcType = PWRMGR_WAKEUPSRC_MAX;
    char keyvalue[POWER_MANAGER_KEY_SIZE] = {0};
    uint8_t countOfWakeupSources = 0;

    UT_LOG_DEBUG("Invoking PLAT_INIT");
    status = PLAT_INIT();
    UT_LOG_DEBUG("Return status: %d", status);
    UT_ASSERT_EQUAL_FATAL(status, PWRMGR_SUCCESS);

    /*Getting the count of the supported wakeup sources*/
    countOfWakeupSources = ut_kvp_getUInt8Field(ut_kvp_profile_getInstance(), "powermanager.CountOfWakeupSources");
    UT_LOG_DEBUG ("Count Of Wakeup Sources %d" , countOfWakeupSources);

    /*Getting the supported wakeup sources*/
    for(int i = 0; i < countOfWakeupSources; i++){
        snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager.WakeupSources/%d", i);
        srcType = ut_kvp_getUInt8Field(ut_kvp_profile_getInstance(), keyvalue);

        UT_LOG_DEBUG("Invoking PLAT_API_SetWakeupSrc with srcType: %d and enable: true", srcType);

        status = PLAT_API_SetWakeupSrc(srcType, false);
        UT_LOG_DEBUG("Return status: %d", status);
        UT_ASSERT_EQUAL(status, PWRMGR_SUCCESS);
        if (status != PWRMGR_SUCCESS)
        {
            UT_LOG_ERROR("Failed to set Wakeup Src");
            continue;
        }

        UT_LOG_DEBUG("Invoking PLAT_API_GetWakeupSrc with srcType: %d", srcType);
        status = PLAT_API_GetWakeupSrc(srcType, &enable);
        UT_LOG_DEBUG("Return status: %d, enable: %d", status, enable);
        UT_ASSERT_EQUAL(status, PWRMGR_SUCCESS);
        UT_ASSERT_EQUAL(enable, false);

        UT_LOG_DEBUG("Invoking PLAT_API_SetWakeupSrc with srcType: %d and enable: false", srcType);
        status = PLAT_API_SetWakeupSrc(srcType, true);
        UT_LOG_DEBUG("Return status: %d", status);
        UT_ASSERT_EQUAL(status, PWRMGR_SUCCESS);
        if (status != PWRMGR_SUCCESS)
        {
            UT_LOG_ERROR("Failed to set Wakeup Src");
            continue;
        }

        UT_LOG_DEBUG("Invoking PLAT_API_GetWakeupSrc with srcType: %d", srcType);
        status = PLAT_API_GetWakeupSrc(srcType, &enable);
        UT_LOG_DEBUG("Return status: %d, enable: %d", status, enable);
        UT_ASSERT_EQUAL(status, PWRMGR_SUCCESS);
        UT_ASSERT_EQUAL(enable, true);
    }

    UT_LOG_DEBUG("Invoking PLAT_TERM");
    status = PLAT_TERM();
    UT_LOG_DEBUG("Return status: %d", status);
    UT_ASSERT_EQUAL_FATAL(status, PWRMGR_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l2_plat_power_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L2 plat_power]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings

    UT_add_test( pSuite, "L2_SetAndGetPowerState", test_l2_plat_power_SetAndGetPowerState);
    UT_add_test( pSuite, "L2_SetAndGetWakeupSrc", test_l2_plat_power_SetAndGetWakeupSrc);

    return 0;
}
/** @} */ // End of PLAT_POWER_HALTEST_L2
/** @} */ // End of POWER_MANAGER_HALTEST
/** @} */ // End of POWER_MANAGER
/** @} */ // End of HPK