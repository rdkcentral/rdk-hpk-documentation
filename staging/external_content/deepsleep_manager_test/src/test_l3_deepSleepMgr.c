/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2024 RDK Management
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
 * @defgroup Deepsleep_Mgr_HALTEST_L3 Deep Sleep Manager HALTEST L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for Deep Sleep Manager HAL :
 *
 * This module includes Level 3 functional tests for APIs of Deepsleep Manager HAL
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
#include <ut_control_plane.h>

#define UT_LOG_MENU_INFO UT_LOG_INFO

#define DS_ASSERT assert

static int gTestGroup = 3;
static int gTestID = 1;

/* DeepSleep_Return_Status_t */
const static ut_control_keyStringMapping_t DeepSleep_Return_Status_mapTable [] = {
  { "DEEPSLEEPMGR_SUCCESS",             (int32_t)DEEPSLEEPMGR_SUCCESS},
  { "DEEPSLEEPMGR_ALREADY_INITIALIZED", (int32_t)DEEPSLEEPMGR_ALREADY_INITIALIZED},
  { "DEEPSLEEPMGR_NOT_INITIALIZED",     (int32_t)DEEPSLEEPMGR_NOT_INITIALIZED},
  { "DEEPSLEEPMGR_INIT_FAILURE",        (int32_t)DEEPSLEEPMGR_INIT_FAILURE},
  { "DEEPSLEEPMGR_SET_FAILURE",         (int32_t)DEEPSLEEPMGR_SET_FAILURE},
  { "DEEPSLEEPMGR_WAKEUP_FAILURE",      (int32_t)DEEPSLEEPMGR_WAKEUP_FAILURE},
  { "DEEPSLEEPMGR_TERM_FAILURE",        (int32_t)DEEPSLEEPMGR_TERM_FAILURE},
  { "DEEPSLEEPMGR_MAX",                 (int32_t)DEEPSLEEPMGR_MAX},
  {  NULL, -1 }
};

/* DeepSleep_WakeupReason_t */
const static ut_control_keyStringMapping_t DeepSleep_WakeupReason_mapTable [] = {
  { "DEEPSLEEP_WAKEUPREASON_IR",             (int32_t)DEEPSLEEP_WAKEUPREASON_IR},
  { "DEEPSLEEP_WAKEUPREASON_RCU_BT",         (int32_t)DEEPSLEEP_WAKEUPREASON_RCU_BT},
  { "DEEPSLEEP_WAKEUPREASON_RCU_RF4CE",      (int32_t)DEEPSLEEP_WAKEUPREASON_RCU_RF4CE},
  { "DEEPSLEEP_WAKEUPREASON_GPIO",           (int32_t)DEEPSLEEP_WAKEUPREASON_GPIO},
  { "DEEPSLEEP_WAKEUPREASON_LAN",            (int32_t)DEEPSLEEP_WAKEUPREASON_LAN},
  { "DEEPSLEEP_WAKEUPREASON_WLAN",           (int32_t)DEEPSLEEP_WAKEUPREASON_WLAN},
  { "DEEPSLEEP_WAKEUPREASON_TIMER",          (int32_t)DEEPSLEEP_WAKEUPREASON_TIMER},
  { "DEEPSLEEP_WAKEUPREASON_FRONT_PANEL",    (int32_t)DEEPSLEEP_WAKEUPREASON_FRONT_PANEL},
  { "DEEPSLEEP_WAKEUPREASON_WATCHDOG",       (int32_t)DEEPSLEEP_WAKEUPREASON_WATCHDOG},
  { "DEEPSLEEP_WAKEUPREASON_SOFTWARE_RESET", (int32_t)DEEPSLEEP_WAKEUPREASON_SOFTWARE_RESET},
  { "DEEPSLEEP_WAKEUPREASON_THERMAL_RESET",  (int32_t)DEEPSLEEP_WAKEUPREASON_THERMAL_RESET},
  { "DEEPSLEEP_WAKEUPREASON_WARM_RESET",     (int32_t)DEEPSLEEP_WAKEUPREASON_WARM_RESET},
  { "DEEPSLEEP_WAKEUPREASON_COLDBOOT",       (int32_t)DEEPSLEEP_WAKEUPREASON_COLDBOOT},
  { "DEEPSLEEP_WAKEUPREASON_STR_AUTH_FAILURE", (int32_t)DEEPSLEEP_WAKEUPREASON_STR_AUTH_FAILURE},
  { "DEEPSLEEP_WAKEUPREASON_CEC",            (int32_t)DEEPSLEEP_WAKEUPREASON_CEC},
  { "DEEPSLEEP_WAKEUPREASON_PRESENCE",       (int32_t)DEEPSLEEP_WAKEUPREASON_PRESENCE},
  { "DEEPSLEEP_WAKEUPREASON_VOICE",          (int32_t)DEEPSLEEP_WAKEUPREASON_VOICE},
  { "DEEPSLEEP_WAKEUPREASON_UNKNOWN",        (int32_t)DEEPSLEEP_WAKEUPREASON_UNKNOWN},
  { "DEEPSLEEP_WAKEUPREASON_MAX",            (int32_t)DEEPSLEEP_WAKEUPREASON_MAX},
  {  NULL, -1 }
};

/**
 * @brief This function clears the stdin buffer.
 *
 * This function clears the stdin buffer.
 */
void readAndDiscardRestOfLine(FILE* in)
{
   int c;
   while ( (c = fgetc(in)) != EOF && c != '\n');
}

/**
* @brief Initialization of the HAL Deepsleep Manager Module
*
* This test provides a scope to open the HAL Deepsleep Manager module.
*
* **Test Group ID:** 03@n
*
* **Test Case ID:** 001@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/

void test_l3_deepsleep_manager_hal_Init(void)
{
   gTestID = 1;
   DeepSleep_Return_Status_t status = DEEPSLEEPMGR_SUCCESS;

   UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

   // Step 1: Call PLAT_DS_INIT()
   UT_LOG_INFO("Calling PLAT_DS_INIT()");
   status = PLAT_DS_INIT();
   UT_LOG_INFO("Result PLAT_DS_INIT: DeepSleep_Return_Status_t:[%s]",
                UT_Control_GetMapString(DeepSleep_Return_Status_mapTable, status));

    assert(status == DEEPSLEEPMGR_SUCCESS);

   UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Trigger deepsleep from the HAL Deepsleep Manager Module
*
* This test provides a scope to trigger deepsleep with a timeout.
*
* **Test Group ID:** 03@n
*
* **Test Case ID:** 002@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/
void test_l3_deepsleep_manager_hal_Trigger_Deepsleep(void)
{
   gTestID = 2;
   DeepSleep_Return_Status_t status = DEEPSLEEPMGR_SUCCESS;
    int enableGet = 0;


   UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

   int deep_sleep_timeout;
   bool isGPIOWakeup = false;

    // Step 1: Get the Deep Sleep Timeout
   UT_LOG_MENU_INFO("----------------------------------------------------------");
   UT_LOG_MENU_INFO("Specify the Deep Sleep Timeout in seconds between 0 and 604800. 0 for no timeout.  ");
   UT_LOG_MENU_INFO("----------------------------------------------------------");
   UT_LOG_MENU_INFO("Input timeout: ");
   scanf("%d", &deep_sleep_timeout);
   readAndDiscardRestOfLine(stdin);
   if(deep_sleep_timeout < 0 || deep_sleep_timeout > 604800 )
        {
            UT_LOG_ERROR("Invalid timeout value");
            goto exit;
        }
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Network Standby enabled/disabled");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#  %-20s","Enable/Disable Network Standby");
    UT_LOG_MENU_INFO("\t%d  %-20s", 0, "Disable");
    UT_LOG_MENU_INFO("\t%d  %-20s", 1, "Enable");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Select Enable/Disable Network standby: ");
    scanf("%d", &enableGet);
    readAndDiscardRestOfLine(stdin);
    if(enableGet != 0 && enableGet != 1)
    {
        UT_LOG_ERROR("Invalid enable type");
        goto exit;
    }
     bool networkStandby = (bool)enableGet;
    
   // Step 2: Call PLAT_DS_SetDeepSleep()
   UT_LOG_INFO("Calling PLAT_DS_SetDeepSleep(IN:deep_sleep_timeout[%d], OUT:isGPIOWakeup[], IN:networkStandby[%s])",
        deep_sleep_timeout, networkStandby ? "true" : "false");
   status = PLAT_DS_SetDeepSleep(deep_sleep_timeout, &isGPIOWakeup, networkStandby);
   UT_LOG_INFO("Calling PLAT_DS_SetDeepSleep(IN:deep_sleep_timeout[%d], OUT:isGPIOWakeup[%d], IN:networkStandby[%s]) DeepSleep_Return_Status_t:[%s]", 
                    deep_sleep_timeout,isGPIOWakeup, networkStandby ? "true" : "false", 
                    UT_Control_GetMapString(DeepSleep_Return_Status_mapTable, status));

    assert(status == DEEPSLEEPMGR_SUCCESS);

  exit:
   UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Trigger deepsleepwakeup from the HAL Deepsleep Manager Module
*
* This test provides a scope to trigger deepsleep wakeup.
*
* **Test Group ID:** 03@n
*
* **Test Case ID:** 003@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/
void test_l3_deepsleep_manager_hal_wakeup(void){
    gTestID = 3;
    DeepSleep_Return_Status_t status = DEEPSLEEPMGR_SUCCESS;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 1: Call PLAT_DS_DeepSleepWakeup()
    UT_LOG_INFO("Calling PLAT_DS_DeepSleepWakeup()");
    status = PLAT_DS_DeepSleepWakeup();
    UT_LOG_INFO("Result PLAT_DS_DeepSleepWakeup() DeepSleep_Return_Status_t:[%s]",
                UT_Control_GetMapString(DeepSleep_Return_Status_mapTable, status));

    assert(status == DEEPSLEEPMGR_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Trigger deepsleep last wakeup reason from the HAL Deepsleep Manager Module
*
* This test provides a scope to trigger deepsleep last wakeup reason.
*
* **Test Group ID:** 03@n
*
* **Test Case ID:** 004@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/
void test_l3_deepsleep_manager_hal_wakeupreason(void){
    gTestID = 4;
    DeepSleep_Return_Status_t status = DEEPSLEEPMGR_SUCCESS;
    DeepSleep_WakeupReason_t wakeupReason;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 1: Call PLAT_DS_GetLastWakeupReason()
    UT_LOG_INFO("Calling PLAT_DS_GetLastWakeupReason(OUT:wakeupReason[]");
    status = PLAT_DS_GetLastWakeupReason(&wakeupReason);
    UT_LOG_INFO("Result PLAT_DS_GetLastWakeupReason(wakeupReason:[%s])  DeepSleep_Return_Status_t:[%s]",
                UT_Control_GetMapString(DeepSleep_WakeupReason_mapTable, wakeupReason), 
                UT_Control_GetMapString(DeepSleep_Return_Status_mapTable, status));

    assert(status == DEEPSLEEPMGR_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Trigger deepsleep last wakeup keycode reason from the HAL Deepsleep Manager Module
*
* This test provides a scope to trigger deepsleep last wakeup keycode.
*
* **Test Group ID:** 03@n
*
* **Test Case ID:** 005@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/
void test_l3_deepsleep_manager_hal_lastwakeupkeycode(void){
    gTestID = 5;
    DeepSleep_Return_Status_t status = DEEPSLEEPMGR_SUCCESS;
    DeepSleepMgr_WakeupKeyCode_Param_t  wakeupKeyCode;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 1: Call PLAT_DS_GetLastWakeupKeyCode()
    UT_LOG_INFO("Calling PLAT_DS_GetLastWakeupKeyCode(OUT:wakeupKeyCode[])");
    status = PLAT_DS_GetLastWakeupKeyCode(&wakeupKeyCode);
    UT_LOG_INFO("Result PLAT_DS_GetLastWakeupKeyCode(wakeupKeyCode:[%d]) DeepSleep_Return_Status_t:[%s]",
                wakeupKeyCode, UT_Control_GetMapString(DeepSleep_Return_Status_mapTable, status));

    assert(status == DEEPSLEEPMGR_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Termination of the HAL Deepsleep Manager Module
*
* This test provides a scope to close the HAL Deepsleep Manager module.

*
* **Test Group ID:** 03@n
*
* **Test Case ID:** 006@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select the Test 1 to before running any test.
*
*/

void test_l3_deepsleep_manager_hal_Term(void)
{
   gTestID = 6;
   DeepSleep_Return_Status_t status = DEEPSLEEPMGR_SUCCESS;

   UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

   // Step 1: Call PLAT_DS_TERM()
   UT_LOG_INFO("Calling PLAT_DS_TERM()");
   status = PLAT_DS_TERM();
   UT_LOG_INFO("Result PLAT_DS_TERM() DeepSleep_Return_Status_t:[%s]",
                UT_Control_GetMapString(DeepSleep_Return_Status_mapTable, status));

    assert(status == DEEPSLEEPMGR_SUCCESS);

   UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l3_deepSleepMgr_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite_withGroupID("[L3 Deepsleep manager ]", NULL, NULL,UT_TESTS_L3);
    if (pSuite == NULL)
    {
         return -1;
    }
    // List of test function names and strings

    UT_add_test( pSuite, "Initialize Deepsleep Manager", test_l3_deepsleep_manager_hal_Init);
    UT_add_test( pSuite, "Trigger Deepsleep", test_l3_deepsleep_manager_hal_Trigger_Deepsleep);
    UT_add_test( pSuite, "Deepsleep Wakeup", test_l3_deepsleep_manager_hal_wakeup);
    UT_add_test( pSuite, "Get last wakeup reason", test_l3_deepsleep_manager_hal_wakeupreason);
    UT_add_test( pSuite, "Get last wakeup keycode", test_l3_deepsleep_manager_hal_lastwakeupkeycode);
    UT_add_test( pSuite, "Terminate Deepsleep Manager", test_l3_deepsleep_manager_hal_Term);

    return 0;
}

/** @} */ // End of Deepsleep_Mgr_HALTEST_L3
/** @} */ // End of Deepsleep_Manager_HALTEST
/** @} */ // End Deepsleep_Manager
/** @} */ // End of HPK