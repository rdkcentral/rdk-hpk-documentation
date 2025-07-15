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
 * @defgroup DS_Host_HALTEST Device Settings Host HAL Tests
 * @{
 */

/**
 * @defgroup DS_Host_HALTEST_L1 Device Settings Host HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for DS Host HAL :
 *
 * Level 1 unit test cases for all APIs of Device Settings Host HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-host_halSpec.md](../../docs/pages/ds-host_halSpec.md)
 * @endparblock
 */


/**
 * @file test_l1_dsHost.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dsHost.h"
#include "dsError.h"

static int gTestGroup = 1;
static int gTestID = 1;

/* Global flags to support features */
static bool extendedEnumsSupported=false; //Default to not supported

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
 * @brief Ensure dsHostInit() returns correct error codes during positive scenarios
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
 * |01|Call dsHostInit() Initialize dsHost | | dsERR_NONE | Should pass|
 * |02|Call dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful after initialization |
 * |03|Call dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass after termination |
 * |04|Call dsHostTerm() Terminate dsHost again | | dsERR_NONE | Termination should be successful after initialization |
 * 
 */
void test_l1_dsHost_positive_dsHostInit(void) {
    gTestID = 1;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call dsHostInit() Initialize dsHost
    dsError_t result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 01: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 02: Call dsHostTerm() Terminate dsHost
     result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 02: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: Call dsHostInit() Initialize dsHost
    result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 03: Initialize dsHost again -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 04: Call dsHostTerm() Terminate dsHost again
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 04: Terminate dsHost again -> Expected: dsERR_NONE, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsHostInit() returns correct error codes during negative scenarios
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
 * |01|Call dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass as it's the first call |
 * |02|Call dsHostInit() Attempt to initialize dsHost again | | dsERR_ALREADY_INITIALIZED | Initialization should fail as it's already initialized |
 * |03|Call dsHostTerm() Terminate dsHost| | dsERR_NONE | Should pass |
 * 
 * @note Testing for dsERR_GENERAL might be challenging as it represents undefined platform errors. Such errors can be hard to simulate consistently in a controlled testing environment.
 */
void test_l1_dsHost_negative_dsHostInit(void) {
    gTestID = 2;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call dsHostInit() Initialize dsHost
    dsError_t result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 01: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 02: Call dsHostInit() Attempt to initialize dsHost again
    result = dsHostInit();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_ALREADY_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 02: Attempt to initialize dsHost again -> Expected: dsERR_ALREADY_INITIALIZED, Got: %d\n", result);
      
    // Step 03: Call dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 03: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsHostTerm() returns correct error codes during positive scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * 
 * **Pre-Conditions:**@n
 * None.
 * 
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |02|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful after initialization |
 * |03|dsHostInit() Initialize dsHost again | | dsERR_NONE | Initialization should pass |
 * |04|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful after re-initialization |
 * 
 */
void test_l1_dsHost_positive_dsHostTerm(void) {
    gTestID = 3;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: dsHostInit() Initialize dsHost
    dsError_t result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 01: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 02: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 02: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsHostInit() Initialize dsHost again
    result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 03: Initialize dsHost again -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 04: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 04: Terminate dsHost again -> Expected: dsERR_NONE, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsHostTerm() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * 
 * **Pre-Conditions:**@n
 * None.
 * 
 * **Dependencies:** dsHostInit()@n
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|dsHostTerm() Attempt to terminate dsHost| | dsERR_NOT_INITIALIZED | Termination should fail as module is not initialized |
 * |02|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |03|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful after initialization |
 * |04|dsHostTerm() Attempt to terminate dsHost | | dsERR_NOT_INITIALIZED | Termination should fail as module is not initialized |
 * 
 * @note Testing for dsERR_GENERAL in dsHostTerm() might be challenging as it represents undefined platform errors. Such errors can be hard to simulate consistently in a controlled testing environment.
 */
void test_l1_dsHost_negative_dsHostTerm(void) {
    gTestID = 4;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: dsHostTerm() Attempt to terminate dsHost without initialization
    dsError_t result = dsHostTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 01: Attempt to terminate dsHost without initialization -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

    // Step 02: dsHostInit() Initialize dsHost
    result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 02: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsHostTerm() Terminate dsHost after initialization
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 03: Terminate dsHost after initialization -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 04: dsHostTerm() Attempt to terminate dsHost again
    result = dsHostTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 04: Attempt to terminate dsHost again -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsGetCPUTemperature() returns correct error codes during positive scenarios
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
 * |01|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |02|dsGetCPUTemperature() Call with valid pointer to store temperature | Valid float pointer | dsERR_NONE | Temperature value should be fetched successfully |
 * |03|dsGetCPUTemperature() Call with valid pointer to store temperature | Valid float pointer | dsERR_NONE | Temperature value should be fetched successfully |
 * |04|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful |
 * 
 */
void test_l1_dsHost_positive_dsGetCPUTemperature(void) {
    gTestID = 5;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    float temperatureValue = 0.0f;

    // Step 01: dsHostInit() Initialize dsHost
    dsError_t result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 01: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 02: dsGetCPUTemperature() Call with valid pointer to store temperature
    result = dsGetCPUTemperature(&temperatureValue);
    UT_ASSERT_EQUAL(result, dsERR_NONE);
    UT_LOG("Step 02: Fetch CPU Temperature (1st time) -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsGetCPUTemperature() Call with valid pointer to store temperature again
    result = dsGetCPUTemperature(&temperatureValue);
    UT_ASSERT_EQUAL(result, dsERR_NONE);
    UT_LOG("Step 03: Fetch CPU Temperature (2nd time) -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 04: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 04: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsGetCPUTemperature() returns correct error codes during negative scenarios
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
 * |01|dsGetCPUTemperature() Call without prior initialization | Valid float pointer | dsERR_NOT_INITIALIZED | Fetching temperature should fail as module is not initialized |
 * |02|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |03|dsGetCPUTemperature() Call with NULL pointer | NULL | dsERR_INVALID_PARAM | Should return invalid parameter error |
 * |04|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful |
 * |05|dsGetCPUTemperature() Call after termination | Valid float pointer | dsERR_NOT_INITIALIZED | Fetching temperature should fail as module is terminated |
 * 
 * @note Testing for dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED in dsGetCPUTemperature() might be challenging as these represent undefined platform errors or specific hardware constraints. Such errors can be hard to simulate consistently in a controlled testing environment.
 */
void test_l1_dsHost_negative_dsGetCPUTemperature(void) {
    gTestID = 6;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    float temperatureValue = 0.0f;

    // Step 01: dsGetCPUTemperature() Call without prior initialization
    dsError_t result = dsGetCPUTemperature(&temperatureValue);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 01: Fetch CPU Temperature without initialization -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

    // Step 02: dsHostInit() Initialize dsHost
    result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 02: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsGetCPUTemperature() Call with NULL pointer
    result = dsGetCPUTemperature(NULL);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    UT_LOG("Step 03: Fetch CPU Temperature with NULL pointer -> Expected: dsERR_INVALID_PARAM, Got: %d\n", result);
    
    // Step 04: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 04: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 05: dsGetCPUTemperature() Call after termination
    result = dsGetCPUTemperature(&temperatureValue);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 05: Fetch CPU Temperature after termination -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsGetSocIDFromSDK() returns correct error codes during positive scenarios
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
 * |01|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |02|dsGetSocIDFromSDK() Call with a valid pointer to store the SOC ID | Valid char pointer (size of 8 bytes) | dsERR_NONE | SOC ID should be fetched successfully |
 * |03|dsGetSocIDFromSDK() Call with a valid pointer to store the SOC ID again | Valid char pointer (size of 8 bytes) | dsERR_NONE | SOC ID should be fetched successfully |
 * |04|Compare return values from step 2/3 to ensure that they are the same |  |  | Success |
 * |05|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful |
 * 
 */
void test_l1_dsHost_positive_dsGetSocIDFromSDK(void) {
    gTestID = 7;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char socID1[1024]= {0}; 
    char socID2[1024]= {0};

    // Step 01: dsHostInit() Initialize dsHost
    dsError_t result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 01: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 02: dsGetSocIDFromSDK() Call with a valid pointer to store the SOC ID
    result = dsGetSocIDFromSDK(socID1);
    UT_ASSERT_EQUAL(result, dsERR_NONE);
    UT_LOG("Step 02: Fetch SOC ID (1st time) -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsGetSocIDFromSDK() Call with a valid pointer to store the SOC ID again
    result = dsGetSocIDFromSDK(socID2);
    UT_ASSERT_EQUAL(result, dsERR_NONE);
    UT_LOG("Step 03: Fetch SOC ID (2nd time) -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 04: Compare return values from step 2/3 to ensure they are the same
    UT_ASSERT_EQUAL(strcmp(socID1, socID2),0);
    UT_LOG("Step 04: Compare SOC IDs from Step 2 and Step 3 -> Expected: Match, Result: Matched\n");

    // Step 05: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 05: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsGetSocIDFromSDK() returns correct error codes during negative scenarios
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
 * |01|dsGetSocIDFromSDK() Call without prior initialization | Valid char pointer (size of 8 bytes) | dsERR_NOT_INITIALIZED | Fetching SOC ID should fail as module is not initialized |
 * |02|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |03|dsGetSocIDFromSDK() Call with NULL pointer | NULL | dsERR_INVALID_PARAM | Should return invalid parameter error |
 * |04|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful |
 * |05|dsGetSocIDFromSDK() Call after termination | Valid char pointer (size of 8 bytes) | dsERR_NOT_INITIALIZED | Fetching SOC ID should fail as module is terminated |
 * 
 * @note Testing for dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED in dsGetSocIDFromSDK() might be challenging as these represent undefined platform
 * 
 */
void test_l1_dsHost_negative_dsGetSocIDFromSDK(void) {
    gTestID = 8;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    char socID[1024] = {0};

    // Step 01: dsGetSocIDFromSDK() Call without prior initialization
    dsError_t result = dsGetSocIDFromSDK(socID);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 01: Fetch SOC ID without initialization -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

    // Step 02: dsHostInit() Initialize dsHost
    result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 02: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsGetSocIDFromSDK() Call with NULL pointer
    result = dsGetSocIDFromSDK(NULL);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    UT_LOG("Step 03: Fetch SOC ID with NULL pointer -> Expected: dsERR_INVALID_PARAM, Got: %d\n", result);

    // Step 04: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 04: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 05: dsGetSocIDFromSDK() Call after termination
     result = dsGetSocIDFromSDK(socID);
     CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
     UT_LOG("Step 05: Fetch SOC ID after termination -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

     UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsGetHostEDID() returns correct error codes during positive scenarios
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
 * |01|dsHostInit() Initialize dsHost | | dsERR_NONE | Initialization should pass |
 * |02|dsGetHostEDID() Call with valid pointers for edid and length | Valid unsigned char pointer, valid int pointer | dsERR_NONE | EDID and its length should be fetched successfully |
 * |03|dsGetHostEDID() Call with valid pointers for edid and length | Valid unsigned char pointer, valid int pointer | dsERR_NONE | EDID and its length should be fetched successfully |
 * |04|Compare returns from step 2/3 and verify they are the same| |  | Should succeed |
 * |05|dsHostTerm() Terminate dsHost | | dsERR_NONE | Termination should be successful |
 * 
 */
void test_l1_dsHost_positive_dsGetHostEDID(void) {
    gTestID = 9;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    unsigned char edid1[EDID_MAX_DATA_SIZE] = {0};  
    unsigned char edid2[EDID_MAX_DATA_SIZE] = {0};
    int length1 = 0;
    int length2 = 0;

    // Step 01: dsHostInit() Initialize dsHost
    dsError_t result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 01: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 02: dsGetHostEDID() Call with valid pointers for edid and length
    result = dsGetHostEDID(edid1, &length1);
    UT_ASSERT_EQUAL(result, dsERR_NONE);
    UT_LOG("Step 02: Fetch Host EDID (1st time) -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsGetHostEDID() Call with valid pointers for edid and length again
    result = dsGetHostEDID(edid2, &length2);
    UT_ASSERT_EQUAL(result, dsERR_NONE);
    UT_LOG("Step 03: Fetch Host EDID (2nd time) -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 04: Compare EDIDs from step 2/3 and verify they are the same
     UT_ASSERT_EQUAL(length1, length2);
     UT_ASSERT_EQUAL(memcmp(edid1, edid2, length1), 0);
    UT_LOG("Step 04: Compare EDIDs from Step 2 and Step 3 -> Expected: Match, Result: Matched\n");

    // Step 05: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 05: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsGetHostEDID() returns correct error codes during negative scenarios
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
 * |01|dsGetHostEDID() Call without prior initialization | Valid unsigned char pointer, valid int pointer | dsERR_NOT_INITIALIZED | Fetching EDID should fail as module is not initialized |
 * |02|dsHostInit() Initialize dsHost| | dsERR_NONE | Initialization should pass |
 * |03|dsGetHostEDID() Call with NULL pointers | Valid char, NULL | dsERR_INVALID_PARAM | Should return invalid parameter error |
 * |04|dsGetHostEDID() Call with NULL pointers | NULL, Valid length | dsERR_INVALID_PARAM | Should return invalid parameter error |
 * |05|dsHostTerm() Terminate dsHost| | dsERR_NONE | Termination should be successful |
 * |06|dsGetHostEDID() Call after termination | Valid unsigned char pointer, valid int pointer | dsERR_NOT_INITIALIZED | Fetching EDID should fail as module is terminated |
 * 
 * @note Testing for dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED in dsGetHostEDID() might be challenging as these represent undefined platform errors or specific hardware constraints. Such errors can be hard to simulate consistently in a controlled testing environment.
 */
void test_l1_dsHost_negative_dsGetHostEDID(void) {
    gTestID = 10;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    unsigned char edid[EDID_MAX_DATA_SIZE]= {0};  // Making an assumption about maximum EDID size.
    int length = 0;

    // Step 01: dsGetHostEDID() Call without prior initialization
    dsError_t result = dsGetHostEDID(edid, &length);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 01: Fetch Host EDID without initialization -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

    // Step 02: dsHostInit() Initialize dsHost
    result = dsHostInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 02: Initialize dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 03: dsGetHostEDID() Call with NULL pointers
    result = dsGetHostEDID(edid, NULL);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    UT_LOG("Step 03: Fetch Host EDID with NULL pointers -> Expected: dsERR_INVALID_PARAM, Got: %d\n", result);

    // Step 04: dsGetHostEDID() Call with NULL pointers
    result = dsGetHostEDID(NULL, &length);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    UT_LOG("Step 03: Fetch Host EDID with NULL pointers -> Expected: dsERR_INVALID_PARAM, Got: %d\n", result);

    // Step 05: dsHostTerm() Terminate dsHost
    result = dsHostTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG("Step 04: Terminate dsHost -> Expected: dsERR_NONE, Got: %d\n", result);

    // Step 06: dsGetHostEDID() Call after termination
    result = dsGetHostEDID(edid, &length);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("Step 05: Fetch Host EDID after termination -> Expected: dsERR_NOT_INITIALIZED, Got: %d\n", result);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;
static UT_test_suite_t * pSuite2 = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_dsHost_register ( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 dsHost]", NULL, NULL );
    pSuite2 = UT_add_suite( "[L1 dsHost -- advanced]", NULL, NULL );
    if ( NULL == pSuite )
    {
        return -1;
    }
    if ( NULL == pSuite2 )
    {
        return -1;
    }

    UT_add_test( pSuite, "dsHostInit_L1_positive" ,test_l1_dsHost_positive_dsHostInit );
    UT_add_test( pSuite, "dsHostInit_L1_negative" ,test_l1_dsHost_negative_dsHostInit );
    UT_add_test( pSuite, "dsHostTerm_L1_positive" ,test_l1_dsHost_positive_dsHostTerm );
    UT_add_test( pSuite, "dsHostTerm_L1_negative" ,test_l1_dsHost_negative_dsHostTerm );
    UT_add_test( pSuite, "dsGetCPUTemperature_L1_positive" ,test_l1_dsHost_positive_dsGetCPUTemperature );
    UT_add_test( pSuite, "dsGetSocIDFromSDK_L1_positive" ,test_l1_dsHost_positive_dsGetSocIDFromSDK );

    UT_add_test( pSuite, "dsGetSocIDFromSDK_L1_negative" ,test_l1_dsHost_negative_dsGetSocIDFromSDK );
    UT_add_test( pSuite, "dsGetCPUTemperature_L1_negative" ,test_l1_dsHost_negative_dsGetCPUTemperature );


    UT_add_test( pSuite2, "dsGetHostEDID_L1_positive" ,test_l1_dsHost_positive_dsGetHostEDID );
    UT_add_test( pSuite2, "dsGetHostEDID_L1_negative" ,test_l1_dsHost_negative_dsGetHostEDID );

    extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "dsHost/features/extendedEnumsSupported" );

    return 0;
} 

/** @} */ // End of DS_Host_HALTEST_L1
/** @} */ // End of DS_Host_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
