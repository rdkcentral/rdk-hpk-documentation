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
 * @addtogroup Deepsleep_Manager Deep Sleep Manager
 * @{
 *
 */

/**
 * @addtogroup Deepsleep_Manager_HALTEST Deep Sleep Manager HALTEST
 * @{
 */

/**
 * @defgroup Deepsleep_Mgr_HALTEST_L1 Deep Sleep Manager HALTEST L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for Deep Sleep Manager HAL :
 *
 * This module includes Level 1 functional tests (success and failure scenarios) for all APIs of Deepsleep Manager HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [deepsleep-manager_halSpec.md](../../docs/pages/deepsleep-manager_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l1_deepSleepMgr.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include "deepSleepMgr.h"
#include <ut.h>
#include <ut_log.h>
#include "ut_kvp_profile.h"


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
 * @brief Ensure PLAT_DS_INIT() returns correct error codes during positive scenarios
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
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |04|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_positive_PLAT_DS_INIT (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 1;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 04: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_INIT() returns correct error codes during negative scenarios
 * 
 * @note The error code DEEPSLEEPMGR_INIT_FAILURE is not able to be tested here.
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
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_INIT() again | | DEEPSLEEP_ALREADY_INITIALIZED | Should Pass |
 * |03|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_negative_PLAT_DS_INIT (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 2;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_INIT() again and expect it to be already initialized
    result = PLAT_DS_INIT();	
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_ALREADY_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_TERM() returns correct error codes during positive scenarios
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
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_positive_PLAT_DS_TERM (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 3;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_TERM() returns correct error codes during negative scenarios
 * 
 * @note The error code DEEPSLEEPMGR_TERM_FAILURE is not able to be tested here.
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
 * |01|Call PLAT_DS_TERM() - attempt to close interface before initialization | | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |04|Call PLAT_DS_TERM() - attempt to close interface after termination | | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_negative_PLAT_DS_TERM (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 4;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Attempt to close interface before initialization and check the result
    result = PLAT_DS_TERM();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 04: Attempt to close interface after termination and check the result
    result = PLAT_DS_TERM();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_DeepSleepWakeup() returns correct error codes during positive scenarios
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
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_DeepSleepWakeup() Trigger deep sleep wake up | | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_positive_PLAT_DS_DeepSleepWakeup (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 5;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_DeepSleepWakeup() and check the result
    result = PLAT_DS_DeepSleepWakeup();
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_DeepSleepWakeup() returns correct error codes during negative scenarios
 * 
 * @note The error code DEEPSLEEP_WAKEUP_FAILURE cannot be tested
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
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_DS_DeepSleepWakeup() - call before initialization| | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |04|Call PLAT_DS_DeepSleepWakeup() - call after termination| | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_negative_PLAT_DS_DeepSleepWakeup (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 6;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_DeepSleepWakeup() before initialization and check the result
    result = PLAT_DS_DeepSleepWakeup();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 04: Call PLAT_DS_DeepSleepWakeup() after termination and check the result
    result = PLAT_DS_DeepSleepWakeup();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_SetDeepSleep() returns correct error codes during positive scenarios
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
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_SetDeepSleep() - initiate device deep sleep | deep_sleep_timeout=30, *isGPIOWakeup=false, networkStandby=false | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_DeepSleepWakeup() Trigger deep sleep wake up | | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |04|Call PLAT_DS_SetDeepSleep() - initiate device deep sleepp | deep_sleep_timeout=30, *isGPIOWakeup=false, networkStandby=true | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |05|Call PLAT_DS_DeepSleepWakeup() Trigger deep sleep wake up | | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |06|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_positive_PLAT_DS_SetDeepSleep (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    gTestID = 7;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Parameters for PLAT_DS_SetDeepSleep
    int deep_sleep_timeout = 30;
    bool isGPIOWakeup = false;
    bool networkStandby = false;

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_SetDeepSleep() with networkStandby=false and check the result
    result = PLAT_DS_SetDeepSleep(deep_sleep_timeout, &isGPIOWakeup, networkStandby);
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_DeepSleepWakeup() and check the result
    result = PLAT_DS_DeepSleepWakeup();
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);

    // Update parameters for next call
    networkStandby = true;

    // Variation 04: Call PLAT_DS_SetDeepSleep() with networkStandby=true and check the result
    result = PLAT_DS_SetDeepSleep(deep_sleep_timeout, &isGPIOWakeup, networkStandby);
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 05: Call PLAT_DS_DeepSleepWakeup() and check the result
    result = PLAT_DS_DeepSleepWakeup();
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 06: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_SetDeepSleep() returns correct error codes during negative scenarios
 * 
 * @note The error code DEEPSLEEP_SET_FAILURE is not able to be tested here.
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
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_DS_SetDeepSleep() - call before initialization | deep_sleep_timeout=60, *isGPIOWakeup=false, networkStandby=false | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_SetDeepSleep() set deep sleep | deep_sleep_timeout=30, *isGPIOWakeup=NULL, networkStandby=false | DEEPSLEEPMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_DS_SetDeepSleep() set deep sleep | deep_sleep_timeout=604801, *isGPIOWakeup=false, networkStandby=false | DEEPSLEEPMGR_INVALID_ARGUMENT | Should Pass |
 * |05|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |06|Call PLAT_DS_SetDeepSleep() - call after termination | deep_sleep_timeout=60, *isGPIOWakeup=false, networkStandby=false | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_negative_PLAT_DS_SetDeepSleep (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    int deep_sleep_timeout;
    bool isGPIOWakeup;
    bool networkStandby;

    gTestID = 8;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call before initialization
    deep_sleep_timeout = 60;
    isGPIOWakeup = false;
    networkStandby = false;
    result = PLAT_DS_SetDeepSleep(deep_sleep_timeout, &isGPIOWakeup, networkStandby);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_SetDeepSleep() with NULL pointer for isGPIOWakeup
    deep_sleep_timeout = 30;
    result = PLAT_DS_SetDeepSleep(deep_sleep_timeout, NULL, networkStandby);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_INVALID_ARGUMENT, DEEPSLEEPMGR_SUCCESS);

    // Variation 04: Call PLAT_DS_SetDeepSleep() with invalid timeout value pointer for isGPIOWakeup
    deep_sleep_timeout = 604801; //Max value is 604800
    result = PLAT_DS_SetDeepSleep(deep_sleep_timeout, &isGPIOWakeup, networkStandby);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_INVALID_ARGUMENT, DEEPSLEEPMGR_SUCCESS);

    // Variation 05: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 06: Call PLAT_DS_SetDeepSleep() after termination
    deep_sleep_timeout = 60;
    result = PLAT_DS_SetDeepSleep(deep_sleep_timeout, &isGPIOWakeup, networkStandby);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_GetLastWakeupReason() returns correct error codes during positive scenarios
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
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_GetLastWakeupReason() - get last wakeup reason | *wakeupReason | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_positive_PLAT_DS_GetLastWakeupReason (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    DeepSleep_WakeupReason_t wakeupReason;
    gTestID = 9;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_GetLastWakeupReason() and check the result
    result = PLAT_DS_GetLastWakeupReason(&wakeupReason);
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);
    UT_LOG("\n %s  wakeupReason=%d \n",__FUNCTION__,wakeupReason);

    // At this point, you may want to further verify if 'wakeupReason' contains valid data.
    // This is dependent on your specific requirements and what PLAT_DS_GetLastWakeupReason() actually does.

    // Variation 03: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_GetLastWakeupReason() returns correct error codes during negative scenarios
 * 
 * @note The error code DEEPSLEEPMGR_TERM_FAILURE is not able to be tested here.
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
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_DS_GetLastWakeupReason() - call function without initializing | *wakeupReason | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_GetLastWakeupReason() - call function invalid inputs | *wakeupReason=NULL | DEEPSLEEPMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |05|Call PLAT_DS_GetLastWakeupReason() - call after termination | *wakeupReason | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_negative_PLAT_DS_GetLastWakeupReason (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    DeepSleep_WakeupReason_t wakeupReason;
    gTestID = 10;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_GetLastWakeupReason() before initialization and check the result
    result = PLAT_DS_GetLastWakeupReason(&wakeupReason);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_GetLastWakeupReason() with NULL pointer and check the result
    result = PLAT_DS_GetLastWakeupReason(NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_INVALID_ARGUMENT, DEEPSLEEPMGR_SUCCESS);

    // Variation 04: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 05: Call PLAT_DS_GetLastWakeupReason() after termination and check the result
    result = PLAT_DS_GetLastWakeupReason(&wakeupReason);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_GetLastWakeupKeyCode() returns correct error codes during positive scenarios
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
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |02|Call PLAT_DS_GetLastWakeupKeyCode() - call get last wakeup keycode | *wakeupKeyCode | DEEPSLEEPMGR_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_positive_PLAT_DS_GetLastWakeupKeyCode (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    DeepSleepMgr_WakeupKeyCode_Param_t wakeupKeyCode;
    gTestID = 11;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_GetLastWakeupKeyCode() and check the result
    result = PLAT_DS_GetLastWakeupKeyCode(&wakeupKeyCode);
    UT_ASSERT_EQUAL(result, DEEPSLEEPMGR_SUCCESS);
    UT_LOG("\n %s  wakeupKeyCode=%d \n",__FUNCTION__,wakeupKeyCode);

    // Variation 03: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure PLAT_DS_GetLastWakeupKeyCode() returns correct error codes during negative scenarios
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
 * |:--:|---------|----------|--------------|-----|
 * |01|Call PLAT_DS_GetLastWakeupKeyCode() - call without initializing | *wakeupKeyCode | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * |02|Call PLAT_DS_INIT() - open interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |03|Call PLAT_DS_GetLastWakeupKeyCode() - call with invalid inputs | *wakeupKeyCode=NULL | DEEPSLEEPMGR_INVALID_ARGUMENT | Should Pass |
 * |04|Call PLAT_DS_TERM() - close interface | | DEEPSLEEP_SUCCESS | Should Pass |
 * |05|Call PLAT_DS_GetLastWakeupKeyCode() - call after closing module | *wakeupKeyCode | DEEPSLEEPMGR_NOT_INITIALIZED | Should Pass |
 * 
 */
void test_l1_deepSleepMgr_negative_PLAT_DS_GetLastWakeupKeyCode (void)
{
    int result = DEEPSLEEPMGR_SUCCESS;
    DeepSleepMgr_WakeupKeyCode_Param_t wakeupKeyCode;
    gTestID = 12;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Variation 01: Call PLAT_DS_GetLastWakeupKeyCode() before initialization and check the result
    result = PLAT_DS_GetLastWakeupKeyCode(&wakeupKeyCode);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    // Variation 02: Call PLAT_DS_INIT() and check the result
    result = PLAT_DS_INIT();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 03: Call PLAT_DS_GetLastWakeupKeyCode() with NULL pointer and check the result
    result = PLAT_DS_GetLastWakeupKeyCode(NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_INVALID_ARGUMENT, DEEPSLEEPMGR_SUCCESS);

    // Variation 04: Call PLAT_DS_TERM() and check the result
    result = PLAT_DS_TERM();
    UT_ASSERT_EQUAL_FATAL(result, DEEPSLEEPMGR_SUCCESS);

    // Variation 05: Call PLAT_DS_GetLastWakeupKeyCode() after termination and check the result
    result = PLAT_DS_GetLastWakeupKeyCode(&wakeupKeyCode);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, DEEPSLEEPMGR_NOT_INITIALIZED, DEEPSLEEPMGR_SUCCESS);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_deepSleepMgr_register ( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 deepSleepMgr]", NULL, NULL );
    if ( NULL == pSuite )
    {
        return -1;
    }
 
    UT_add_test( pSuite, "PLAT_DS_INIT_L1_positive" ,test_l1_deepSleepMgr_positive_PLAT_DS_INIT );
    UT_add_test( pSuite, "PLAT_DS_INIT_L1_negative" ,test_l1_deepSleepMgr_negative_PLAT_DS_INIT );
    UT_add_test( pSuite, "PLAT_DS_TERM_L1_positive" ,test_l1_deepSleepMgr_positive_PLAT_DS_TERM );
    UT_add_test( pSuite, "PLAT_DS_TERM_L1_negative" ,test_l1_deepSleepMgr_negative_PLAT_DS_TERM );
    UT_add_test( pSuite, "PLAT_DS_DeepSleepWakeup_L1_positive" ,test_l1_deepSleepMgr_positive_PLAT_DS_DeepSleepWakeup );
    UT_add_test( pSuite, "PLAT_DS_DeepSleepWakeup_L1_negative" ,test_l1_deepSleepMgr_negative_PLAT_DS_DeepSleepWakeup );
    UT_add_test( pSuite, "PLAT_DS_SetDeepSleep_L1_positive" ,test_l1_deepSleepMgr_positive_PLAT_DS_SetDeepSleep );
    UT_add_test( pSuite, "PLAT_DS_SetDeepSleep_L1_negative" ,test_l1_deepSleepMgr_negative_PLAT_DS_SetDeepSleep );
    UT_add_test( pSuite, "PLAT_DS_GetLastWakeupReason_L1_positive" ,test_l1_deepSleepMgr_positive_PLAT_DS_GetLastWakeupReason );
    UT_add_test( pSuite, "PLAT_DS_GetLastWakeupReason_L1_negative" ,test_l1_deepSleepMgr_negative_PLAT_DS_GetLastWakeupReason );
    UT_add_test( pSuite, "PLAT_DS_GetLastWakeupKeyCode_L1_positive" ,test_l1_deepSleepMgr_positive_PLAT_DS_GetLastWakeupKeyCode );
    UT_add_test( pSuite, "PLAT_DS_GetLastWakeupKeyCode_L1_negative" ,test_l1_deepSleepMgr_negative_PLAT_DS_GetLastWakeupKeyCode );

    extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "deepsleepmanager/features/extendedEnumsSupported" );

    return 0;
}
/** @} */ // End of Deepsleep_Mgr_HALTEST_L1
/** @} */ // End of Deepsleep_Manager_HALTEST
/** @} */ // End Deepsleep_Manager
/** @} */ // End of HPK
