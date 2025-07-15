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
 * @addtogroup POWER_MANAGER Power Manager Module
 * @{
 */

/**
 * @addtogroup POWER_MANAGER_HALTEST Power Manager HAL Tests
 * @{
 */

/**
 * @defgroup PLAT_POWER_HALTEST_L1 Power Manager HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for Power Manager HAL :
 *
 * Level 1 unit test cases for all APIs of Power Manager HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [power-manager_halSpec.md](../../docs/pages/power-manager_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l1_plat_power.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include "plat_power.h"
#include <stdbool.h>
#include <ut.h>
#include <ut_log.h>
#include "ut_kvp_profile.h"

#define POWER_MANAGER_KEY_SIZE 50

static int gTestGroup = 1;
static int gTestID = 1;
static bool extendedEnumsSupported=false;

#define CHECK_FOR_EXTENDED_ERROR_CODE( result, enhanced, old )\
{\
   if ( extendedEnumsSupported == true )\
   {\
      UT_ASSERT_EQUAL( enhanced, result );\
   }\
   else\
   {\
       UT_ASSERT_EQUAL( old, result );\
   }\
}

/**
 * @brief Ensure PLAT_INIT() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 */
void test_l1_plat_power_positive_PLAT_INIT (void)
{
    gTestID = 1;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID); 
    pmStatus_t ret;

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_INIT() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * 
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Call PLAT_INIT() - again open interface | | PWRMGR_ALREADY_INITIALIZED | Should Pass |
 * |03|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 * @note The error code PWRMGR_INIT_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_INIT (void)
{
    gTestID = 2;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - again open interface
    ret = PLAT_INIT();
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_ALREADY_INITIALIZED, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_TERM() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 */
void test_l1_plat_power_positive_PLAT_TERM (void)
{
    gTestID = 3;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_TERM() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_TERM() - close interface | | PWRMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 * |04|Call PLAT_TERM() - close interface again | | PWRMGR_NOT_INITIALIZED | Should Pass |
 *
 * @note The error code PWRMGR_TERM_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_TERM (void)
{
    gTestID = 4;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;

    // Step 01: Call PLAT_TERM() - close interface without initializing
    ret = PLAT_TERM();
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 04: Call PLAT_TERM() - close interface again
    ret = PLAT_TERM();
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_SetPowerState() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Call PLAT_API_SetPowerState() - Iterate through various power states fetched from the profile file | newState=PWRMGR_POWERSTATE_STANDBY_DEEP_SLEEP | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 */
void test_l1_plat_power_positive_PLAT_API_SetPowerState (void)
{
    gTestID = 5;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    PWRMgr_PowerState_t powerState;
    char keyvalue[POWER_MANAGER_KEY_SIZE] = {0};
    uint8_t countOfPowerState = 0;

    countOfPowerState = UT_KVP_PROFILE_GET_UINT8("powermanager/CountOfPowerState");
    UT_LOG_DEBUG ("Count Of Power States %d" , countOfPowerState);

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // step 02:Call PLAT_API_SetPowerState() -Iterate through various power states fetched from the profile file
    for(int i = 0; i < countOfPowerState; i++) {
        snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager/PowerStates/%d", i);
        powerState =(PWRMgr_PowerState_t)UT_KVP_PROFILE_GET_UINT32(keyvalue);
        UT_LOG_DEBUG("Invoking PLAT_API_SetPowerState with powerState: %d", powerState);
        ret = PLAT_API_SetPowerState(powerState);
        UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
    }

    // Step 03: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_SetPowerState() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |01|Call PLAT_API_SetPowerState() - call without initializing the module | newState=PWRMGR_POWERSTATE_ON | PWRMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_API_SetPowerState() - call with invalid value | newState=PWRMGR_POWERSTATE_MAX | PWRMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 * |05|Call PLAT_API_SetPowerState() - call after terminating the module | newState=PWRMGR_POWERSTATE_ON | PWRMGR_NOT_INITIALIZED | Should Pass |
 * 
 * @note The error code PWRMGR_SET_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_API_SetPowerState (void)
{
    gTestID = 6;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;

    // Step 01: Call PLAT_API_SetPowerState() - call without initializing the module
    ret = PLAT_API_SetPowerState(PWRMGR_POWERSTATE_ON);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_API_SetPowerState() - call with invalid value
    ret = PLAT_API_SetPowerState(PWRMGR_POWERSTATE_MAX);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_INVALID_ARGUMENT, PWRMGR_SUCCESS);

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 05: Call PLAT_API_SetPowerState() - call after terminating the module
    ret = PLAT_API_SetPowerState(PWRMGR_POWERSTATE_ON);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_GetPowerState() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Call PLAT_API_GetPowerState() - get power state | curState* | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_API_GetPowerState() - get power state | curState* | PWRMGR_SUCCESS | Should Pass |
 * |04|Verify that PLAT_API_GetPowerState() calls returns the same value | | | Should Pass |
 * |05|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 */
void test_l1_plat_power_positive_PLAT_API_GetPowerState (void)
{
    gTestID = 7;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    PWRMgr_PowerState_t state1, state2;

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_API_GetPowerState() - get power state
    ret = PLAT_API_GetPowerState(&state1);
    UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_API_GetPowerState() - get power state
    ret = PLAT_API_GetPowerState(&state2);
    UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);

    // Step 04: Verify that PLAT_API_GetPowerState() calls return the same value
    UT_ASSERT_EQUAL(state1, state2);

    // Step 05: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_GetPowerState() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |01|Call PLAT_API_GetPowerState() - call without initializing the module | curState* | PWRMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_API_GetPowerState() - call with NULL pointer | curState*=NULL | PWRMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 * |05|Call PLAT_API_GetPowerState() - call after terminating the module | curState* | PWRMGR_NOT_INITIALIZED | Should Pass |
 * 
 * @note The error code PWRMGR_GET_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_API_GetPowerState (void)
{ 
    gTestID = 8;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    PWRMgr_PowerState_t state;

    // Step 01: Call PLAT_API_GetPowerState() - call without initializing the module
    ret = PLAT_API_GetPowerState(&state);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_API_GetPowerState() - call with NULL pointer
    ret = PLAT_API_GetPowerState(NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_INVALID_ARGUMENT, PWRMGR_SUCCESS);

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 05: Call PLAT_API_GetPowerState() - call after terminating the module
    ret = PLAT_API_GetPowerState(&state);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_SetWakeupSrc() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Call PLAT_API_SetWakeupSrc() - Loop through all wakeup sources and set to false| srcType=PWRMGR_WAKEUPSRC_MAX, enabled=false | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_API_SetWakeupSrc() - Loop through all wakeup sources and set to true| srcType=PWRMGR_WAKEUPSRC_MAX, enabled=true | PWRMGR_SUCCESS | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 */
void test_l1_plat_power_positive_PLAT_API_SetWakeupSrc (void)
{
    gTestID = 9;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    PWRMGR_WakeupSrcType_t srcType = PWRMGR_WAKEUPSRC_MAX;
    char keyvalue[POWER_MANAGER_KEY_SIZE] = {0};
    uint8_t countOfWakeupSources = 0;

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 02:Loop through all wakeup sources and set to false
    countOfWakeupSources = UT_KVP_PROFILE_GET_UINT8("powermanager/CountOfWakeupSources");
    UT_LOG_DEBUG ("Count Of Wakeup Sources %d" , countOfWakeupSources);

    for(int i = 0; i < countOfWakeupSources; i++){
        snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager/WakeupSources/%d", i);
        srcType =(PWRMGR_WakeupSrcType_t)UT_KVP_PROFILE_GET_UINT32(keyvalue);
        ret = PLAT_API_SetWakeupSrc(srcType, false);
        UT_LOG_DEBUG("Return status: %d", ret);
        if(ret == PWRMGR_OPERATION_NOT_SUPPORTED){

            UT_LOG("\n Function: %s Wake up source is not supported\n", __FUNCTION__);
        }
        else{
            UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
        }

    }

    // Step 03:Loop through all wakeup sources and set to true
    for(int i = 0; i < countOfWakeupSources; i++){
        snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager/WakeupSources/%d", i);
        srcType =(PWRMGR_WakeupSrcType_t)UT_KVP_PROFILE_GET_UINT32(keyvalue);
    	ret = PLAT_API_SetWakeupSrc(srcType, true);
        UT_LOG_DEBUG("Return status: %d", ret);
        if(ret == PWRMGR_OPERATION_NOT_SUPPORTED){

            UT_LOG("\n Function: %s Wake up source is not supported\n", __FUNCTION__);
        } 
        else{
            UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
        }

    }

    // Step 4: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_SetWakeupSrc() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |01|Call PLAT_API_SetWakeupSrc() - call without initializing the module | newState=PWRMGR_WAKEUPSRC_IR, enabled=false | PWRMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_API_SetWakeupSrc() - call with invalid value | newState=PWRMGR_WAKEUPSRC_MAX | PWRMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 * |05|Call PLAT_API_SetWakeupSrc() - call after terminating the module | newState=PWRMGR_WAKEUPSRC_IR, enabled=false | PWRMGR_NOT_INITIALIZED | Should Pass |
 *
 * @note The error code PWRMGR_SET_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_API_SetWakeupSrc (void)
{
    gTestID = 10;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;

    // Step 01: Call PLAT_API_SetWakeupSrc() - call without initializing the module
    ret = PLAT_API_SetWakeupSrc(PWRMGR_WAKEUPSRC_IR, false);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_API_SetWakeupSrc() - call with invalid value
    ret = PLAT_API_SetWakeupSrc(PWRMGR_WAKEUPSRC_MAX, false);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_INVALID_ARGUMENT, PWRMGR_SUCCESS);

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 05: Call PLAT_API_SetWakeupSrc() - call after terminating the module
    ret = PLAT_API_SetWakeupSrc(PWRMGR_WAKEUPSRC_IR, false);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_GetWakeupSrc() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * todo Do the double check first
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Retrieve the count of supported wakeup sources from the profile file
 * |03|Call PLAT_API_GetWakeupSrc() - Retrieve and set each supported wakeup source, then get and verify its state | srcType=PWRMGR_WAKEUPSRC_MAX, *enable| PWRMGR_SUCCESS | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 */
void test_l1_plat_power_positive_PLAT_API_GetWakeupSrc (void)
{
    gTestID = 11;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    bool enable = false;
    PWRMGR_WakeupSrcType_t srcType = PWRMGR_WAKEUPSRC_MAX;
    char keyvalue[POWER_MANAGER_KEY_SIZE] = {0};
    uint8_t countOfWakeupSources = 0;

    // Step 01: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    //Step 02:Getting the count of the supported wakeup sources
    countOfWakeupSources = UT_KVP_PROFILE_GET_UINT8("powermanager/CountOfWakeupSources");
    UT_LOG_DEBUG ("Count Of Wakeup Sources %d" , countOfWakeupSources);

    //Step 03:Getting and setting supported wakeup sources, then verifying their states
    for(int i = 0; i < countOfWakeupSources; i++){
        snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager/WakeupSources/%d", i);
        srcType =(PWRMGR_WakeupSrcType_t)UT_KVP_PROFILE_GET_UINT32(keyvalue);
        UT_LOG_DEBUG("Invoking PLAT_API_SetWakeupSrc with srcType: %d and enable: true", srcType);

        ret = PLAT_API_SetWakeupSrc(srcType, false);
        UT_LOG_DEBUG("Return status: %d", ret);
        UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
        if (ret != PWRMGR_SUCCESS) {
            UT_LOG_ERROR("Failed to set Wakeup Src");

        }

       UT_LOG_DEBUG("Invoking PLAT_API_GetWakeupSrc with srcType: %d", srcType);
       ret = PLAT_API_GetWakeupSrc(srcType, &enable);
       UT_LOG_DEBUG("Return status: %d, enable: %d", ret, enable);
       UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
       UT_ASSERT_EQUAL(enable, false);

       UT_LOG_DEBUG("Invoking PLAT_API_SetWakeupSrc with srcType: %d and enable: false", srcType);
       ret = PLAT_API_SetWakeupSrc(srcType, true);
       UT_LOG_DEBUG("Return status: %d", ret);
       UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
       if (ret != PWRMGR_SUCCESS) {
            UT_LOG_ERROR("Failed to set Wakeup Src");

       }

       UT_LOG_DEBUG("Invoking PLAT_API_GetWakeupSrc with srcType: %d", srcType);
       ret= PLAT_API_GetWakeupSrc(srcType, &enable);
       UT_LOG_DEBUG("Return status: %d, enable: %d", ret, enable);
       UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS);
       UT_ASSERT_EQUAL(enable, true);
    }

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_API_GetWakeupSrc() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |01|Call PLAT_API_GetWakeupSrc() - call without initializing the module | srcType, *enable | PWRMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_API_GetWakeupSrc() - call with NULL pointer | srcType, *enable=NULL | PWRMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 * |05|Call PLAT_API_GetWakeupSrc() - call after terminating the module | srcType, *enable | PWRMGR_NOT_INITIALIZED | Should Pass |
 *
 * @note The error code PWRMGR_GET_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_API_GetWakeupSrc (void)
{
    gTestID = 12;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    bool enable;
    PWRMGR_WakeupSrcType_t srcType = PWRMGR_WAKEUPSRC_IR;

    // Step 01: Call PLAT_API_GetWakeupSrc() - call without initializing the module
    ret = PLAT_API_GetWakeupSrc(srcType, &enable);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_API_GetWakeupSrc() - call with NULL pointer
    ret = PLAT_API_GetWakeupSrc(srcType, NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_INVALID_ARGUMENT, PWRMGR_SUCCESS);

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 05: Call PLAT_API_GetWakeupSrc() - call after terminating the module
    ret = PLAT_API_GetWakeupSrc(srcType, &enable);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_Reset() returns correct error codes during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |02|Retrieve the count of power states from the profile file
 * |03|Call PLAT_Reset() - Loop over each power state and call PLAT_Reset() | newState=PWRMGR_POWERSTATE_STANDBY_DEEP_SLEEP | PWRMGR_SUCCESS | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 *
 * @note This test case is deprecated.
 */
void test_l1_plat_power_positive_PLAT_Reset (void)
{
    gTestID = 13;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;
    char keyvalue[POWER_MANAGER_KEY_SIZE] = {0};
    uint8_t countOfPowerState = 0;
    PWRMgr_PowerState_t powerState;

    // Step 1: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    //Step 02:Getting the count of the power states
    countOfPowerState = UT_KVP_PROFILE_GET_UINT8("powermanager/CountOfPowerState");
    UT_LOG_DEBUG ("Count Of Power States %d" , countOfPowerState);

    // Step 03: Loop over each power state and call PLAT_Reset()
    for(int i = 0; i < countOfPowerState; i++) {
	snprintf(keyvalue, POWER_MANAGER_KEY_SIZE, "powermanager/PowerStates/%d", i);
        powerState =(PWRMgr_PowerState_t)UT_KVP_PROFILE_GET_UINT32(keyvalue);
        UT_LOG_DEBUG("Invoking PLAT_API_SetPowerState with powerState: %d", powerState);
        ret = PLAT_Reset(powerState);
        UT_ASSERT_EQUAL(ret, PWRMGR_SUCCESS); // Ensure the returned value is PWRMGR_SUCCESS
    }

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure PLAT_Reset() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 *
 * **Pre-Conditions:**@n
 * None.
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |01|Call PLAT_Reset() - call without initializing the module | newState=PWRMGR_POWERSTATE_ON | PWRMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_INIT() - open interface | | PWRMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_Reset() - call with invalid value | newState=PWRMGR_POWERSTATE_MAX | PWRMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_TERM() - close interface | | PWRMGR_SUCCESS | Should Pass |
 * |05|Call PLAT_Reset() - call after terminating the module | newState=PWRMGR_POWERSTATE_ON | PWRMGR_NOT_INITIALIZED | Should Pass |
 *
 * @note This test case is deprecated.
 * @note The error code PWRMGR_SET_FAILURE is not able to be tested here.
 */
void test_l1_plat_power_negative_PLAT_Reset (void)
{
    gTestID = 14;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    pmStatus_t ret;

    // Step 01: Call PLAT_Reset() - call without initializing the module
    ret = PLAT_Reset(PWRMGR_POWERSTATE_ON);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    // Step 02: Call PLAT_INIT() - open interface
    ret = PLAT_INIT();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 03: Call PLAT_Reset() - call with invalid value
    ret = PLAT_Reset(PWRMGR_POWERSTATE_MAX);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_INVALID_ARGUMENT, PWRMGR_SUCCESS);

    // Step 04: Call PLAT_TERM() - close interface
    ret = PLAT_TERM();
    UT_ASSERT_EQUAL_FATAL(ret, PWRMGR_SUCCESS);

    // Step 05: Call PLAT_Reset() - call after terminating the module
    ret = PLAT_Reset(PWRMGR_POWERSTATE_ON);
    CHECK_FOR_EXTENDED_ERROR_CODE( ret, PWRMGR_NOT_INITIALIZED, PWRMGR_SUCCESS);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_plat_power_register ( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 plat_power]", NULL, NULL );
    if ( NULL == pSuite )
    {
       return -1;
    }

    UT_add_test( pSuite, "PLAT_INIT_L1_positive" ,test_l1_plat_power_positive_PLAT_INIT );
    UT_add_test( pSuite, "PLAT_INIT_L1_negative" ,test_l1_plat_power_negative_PLAT_INIT );
    UT_add_test( pSuite, "PLAT_TERM_L1_positive" ,test_l1_plat_power_positive_PLAT_TERM );
    UT_add_test( pSuite, "PLAT_TERM_L1_negative" ,test_l1_plat_power_negative_PLAT_TERM );
    UT_add_test( pSuite, "PLAT_API_SetPowerState_L1_positive" ,test_l1_plat_power_positive_PLAT_API_SetPowerState );
    UT_add_test( pSuite, "PLAT_API_SetPowerState_L1_negative" ,test_l1_plat_power_negative_PLAT_API_SetPowerState );
    UT_add_test( pSuite, "PLAT_API_GetPowerState_L1_positive" ,test_l1_plat_power_positive_PLAT_API_GetPowerState );
    UT_add_test( pSuite, "PLAT_API_GetPowerState_L1_negative" ,test_l1_plat_power_negative_PLAT_API_GetPowerState );
    UT_add_test( pSuite, "PLAT_API_SetWakeupSrc_L1_positive" ,test_l1_plat_power_positive_PLAT_API_SetWakeupSrc );
    UT_add_test( pSuite, "PLAT_API_SetWakeupSrc_L1_negative" ,test_l1_plat_power_negative_PLAT_API_SetWakeupSrc );
    UT_add_test( pSuite, "PLAT_API_GetWakeupSrc_L1_positive" ,test_l1_plat_power_positive_PLAT_API_GetWakeupSrc );
    UT_add_test( pSuite, "PLAT_API_GetWakeupSrc_L1_negative" ,test_l1_plat_power_negative_PLAT_API_GetWakeupSrc );
    UT_add_test( pSuite, "PLAT_Reset_L1_positive" ,test_l1_plat_power_positive_PLAT_Reset );
    UT_add_test( pSuite, "PLAT_Reset_L1_negative" ,test_l1_plat_power_negative_PLAT_Reset );
    extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "powermanager/features/extendedEnumsSupported" );

    return 0;
}
/** @} */ // End of PLAT_POWER_HALTEST_L1
/** @} */ // End of POWER_MANAGER_HALTEST
/** @} */ // End of POWER_MANAGER
/** @} */ // End of HPK
