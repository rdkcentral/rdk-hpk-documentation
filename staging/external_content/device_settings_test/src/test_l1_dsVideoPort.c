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
 * @defgroup DS_VideoPort_HALTEST Device Settings Video Port HAL Tests
 * @{
 */

/**
 * @defgroup DS_VideoPort_HALTEST_L1 Device Settings Video Port HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for DS Video Port HAL :
 *
 * Level 1 unit test cases for all APIs of Device Settings Video Port HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-video-port_halSpec.md](../../docs/pages/ds-video-port_halSpec.md)
 *
 * @endparblock
 */


/**
 * @file test_l1_dsVideoPort.c
 *
 */

#include <string.h>
#include <stdlib.h>
#include "dsVideoPort.h"
#include "test_parse_configuration.h"

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>

static int gTestGroup = 1;
static int gTestID = 1;

/* Global flags to support features */
static bool extendedEnumsSupported=false; //Default to not supported

#define CHECK_FOR_EXTENDED_ERROR_CODE( result, enhanced, old )\
{\
   if ( extendedEnumsSupported == true )\
   {\
      UT_ASSERT_EQUAL( result, enhanced );\
   }\
   else\
   {\
       UT_ASSERT_EQUAL( result, old );\
   }\
}

/**
 * @brief Ensure dsVideoPortInit() returns correct status codes during positive scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize the video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |03|Call dsVideoPortInit() - Reinitialize the video port system | | dsERR_NONE | Make sure that the video ports can be reinitialized after termination |
 * |04|Call dsVideoPortTerm() - Re-terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsVideoPortInit(void) {
    gTestID = 1;
    UT_LOG_INFO("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Initialize the video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Reinitialize the video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 04: Re-terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsVideoPortInit() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize the video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsVideoPortInit() - Attempt to initialize again without terminating the first initialization | | dsERR_ALREADY_INITIALIZED | Initialization must fail as it is already initialized |
 * |03|Call dsVideoPortTerm()  - Terminate video port system | | dsERR_NONE | Termination must be successful |
 * 
 * 
 * @note Testing for the `dsERR_RESOURCE_NOT_AVAILABLE` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsVideoPortInit(void) {
    gTestID = 2;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Initialize the video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Attempt to initialize again without terminating the first initialization
    status = dsVideoPortInit();
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_ALREADY_INITIALIZED, dsERR_GENERAL);

    // Step 03: Terminate video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsVideoPortTerm() returns correct status codes during positive scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * 
 * **Pre-Conditions:**None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsVideoPortTerm() - Terminate video port system | | dsERR_NONE | Termination must be successful |
 * |03|Call dsVideoPortInit() - Reinitialize video port system | | dsERR_NONE | Initialization must be successful |
 * |04|Call dsVideoPortTerm() - Re-terminate video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsVideoPortTerm(void) {
    gTestID = 3;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Terminate video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Reinitialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 04: Re-terminate video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsVideoPortTerm() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortTerm() - Attempt to terminate the video port without prior initialization | | dsERR_NOT_INITIALIZED | Termination must fail as video port is not initialized |
 * |02|Call dsVideoPortInit() - Initialize the video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |04|Call dsVideoPortTerm() - Attempt to terminate the video port again after termination | | dsERR_NOT_INITIALIZED | Termination must fail as video port is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsVideoPortTerm(void) {
    gTestID = 4;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Attempt to terminate the video port without prior initialization
    status = dsVideoPortTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 04: Attempt to terminate the video port again after termination
    status = dsVideoPortTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsGetVideoPort() returns correct handles for the available video ports
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Get the video port handle for last element , store it in a new variable and compare it with previous step | | dsERR_NONE | Port handle obtained from previous step must be equal to handle obtained in this step
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetVideoPort(void) {
    gTestID = 5;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    intptr_t handle[gDSvideoPort_NumberOfPorts];
    intptr_t lastHandle;
    intptr_t newHandle;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle for valid video port type and index
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle[i]));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        if(i == gDSvideoPort_NumberOfPorts-1)
        {
            lastHandle = handle[i];
        }
    }

    // Step 03: Compare with the last handle
    status = dsGetVideoPort(gDSVideoPortConfiguration[gDSvideoPort_NumberOfPorts-1].typeid, gDSVideoPortConfiguration[gDSvideoPort_NumberOfPorts-1].index, &(handle[gDSvideoPort_NumberOfPorts-1]));
    UT_ASSERT_EQUAL(status, dsERR_NONE);

    //getting last handle in to new handle for comparision
    newHandle = handle[gDSvideoPort_NumberOfPorts-1];
    UT_ASSERT_EQUAL(lastHandle , newHandle);

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Ensure dsGetVideoPort() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetVideoPort() - Attempt to get the Video Port handle without initializing video ports | `type`,`index`= kPorts[0], `handle`= valid handle | dsERR_NOT_INITIALIZED | Get Video Port Handle must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetVideoPort() - by looping through valid port indexes but with invalid type  | type = [not in kPorts] , index = [ loop through kPorts ], handle = [ valid handle ] | dsERR_OPERATION_NOT_SUPPORTED | Operation not supported error must be returned |
 * |04|Call dsGetVideoPort() - by looping through valid types but with invalid index | type = [loop through types in kPorts], index = [ invalid index which is not in kPorts ], handle: [ valid handle ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |05|Call dsGetVideoPort() - by looping through valid types and index but with a NULL handle | type, index: [ loop through kPorts ], handle: [ NULL pointer ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetVideoPort() - Attempt to get the video port handle again after termination | type, index:  [ loop through kPorts ], handle: [ valid handle ] | dsERR_NOT_INITIALIZED | Get Video Port Handle must fail as module is not initialized |
 * 
 * 
 * @note Testing for `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetVideoPort(void) {
    gTestID = 6;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Attempt to get the Video Port handle without initializing
    status = dsGetVideoPort(gDSVideoPortConfiguration[0].typeid, gDSVideoPortConfiguration[0].index, &(handle));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Attempt to get the Video Port handle with invalid port type
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(dsVIDEOPORT_TYPE_MAX, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 04: Attempt to get the Video Port handle invalid port index
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, -1 , &(handle));
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 05: NULL handle
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index ,  NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get the video port handle again after termination
    status = dsGetVideoPort(gDSVideoPortConfiguration[0].typeid, gDSVideoPortConfiguration[0].index, &(handle));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsIsVideoPortEnabled() correctly indicates the enabled status of a video port
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform | type ,  index = [ Loop through kPorts ]  | dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |03|Call dsIsVideoPortEnabled - by looping through the acquired port handles to check if a video port is enabled or not and store the value | handle: [valid handle], enabled: [pointer to hold the enabled/disabled status of video port] | dsERR_NONE | Flag which holds the enabled status of Video Port must be returned |
 * |04|Call dsIsVideoPortEnabled - Again by looping through the acquired port handles to check if a video port is enabled or not and store the value | handle: [valid handle], enabled: [pointer to hold the enabled/disabled status of video port] | dsERR_NONE | Flag which holds the enabled status of Video Port must be returned |
 * |05|Compare the values and make sure they are equal| | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsIsVideoPortEnabled(void) {
    gTestID = 7;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for all supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        bool isEnabled1;
        bool isEnabled2;
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Check if video port is enabled or not
        status = dsIsVideoPortEnabled(handle, &isEnabled1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Repeat the check for enabled status
        status = dsIsVideoPortEnabled(handle, &isEnabled2);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check if the values are equal
        UT_ASSERT_EQUAL(isEnabled1, isEnabled2);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsIsVideoPortEnabled() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsIsVideoPortEnabled() - Attempt to check if video port is enabled or not without initializing video ports | handle: [ invalid handle ], connected: [ pointer to hold enabled status]| dsERR_NOT_INITIALIZED | Call to check if video port is enabled or not must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsIsVideoPortEnabled() using an invalid handle but with a valid pointer to hold the enabled status | handle: [ invalid handle ], enabled: [ pointer to hold enabled status] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsIsVideoPortEnabled() by looping through acquired port handles but with a null pointer for enabled status | handle: [ loop through valid handles ], enabled: [ NULL ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsIsVideoPortEnabled() - Attempt to check if video port is enabled or not without initializing video ports| handle: [ valid handle ], connected: [ pointer to hold enabled status] | dsERR_NOT_INITIALIZED | Call to check if video port is enabled or not must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsIsVideoPortEnabled(void) {
    gTestID = 8;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool enabled = false;

    // Step 01: Attempt to check enabled status without initialization
    status = dsIsVideoPortEnabled(-1, &enabled);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsIsVideoPortEnabled(handle, &enabled);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check enabled status with null pointer
        status = dsIsVideoPortEnabled(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Check enabled status after termination
    status = dsIsVideoPortEnabled(handle, &enabled);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Ensure dsIsDisplayConnected() behaves as expected during positive scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * 
 * **Pre-Conditions:**@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsIsDisplayConnected() - by looping through the acquired port handles to check if video port is connected to a display or not | handle: [ loop through valid handles ] , connected: [pointer to hold the connection status of Video Port] | dsERR_NONE | Flag which holds the connection status of Video Port must be returned |
 * |04|Compare the value with the value for source/sink device respectively | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsIsDisplayConnected(void) {
    gTestID = 9;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool isConnected1 = false;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts ; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Check if video port is connected
        status = dsIsDisplayConnected(handle, &isConnected1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with the value for source/sink device
        if (gSourceType == 0) {
            UT_ASSERT_EQUAL(isConnected1, true);
        } else if (gSourceType == 1) {
            UT_ASSERT_EQUAL(isConnected1, false);
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsIsDisplayConnected() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsIsDisplayConnected() - Attempt to check if video port is connected without initializing video ports | handle: [ invalid handle ], connected: [ pointer to hold connection status] | dsERR_NOT_INITIALIZED | Call to check if display is connected to video port must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsIsDisplayConnected() using an invalid handle but with a valid pointer to hold the connection status | handle: [ invalid handle ], connected: [ pointer to hold connection status] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsIsDisplayConnected() by looping through acquired port handles but with a null pointer for display connection | handle: [ loop through valid handles ], connected: [ NULL ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsIsDisplayConnected() - Attempt to check if video port is connected without initializing video ports|  handle: [ valid handle ], connected: [ pointer to hold connection status] | dsERR_NOT_INITIALIZED | Call to check if display is connected to video port must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsIsDisplayConnected(void) {
    gTestID = 10;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool connected = false;

    // Step 01: Attempt to check connection status without initialization
    status = dsIsDisplayConnected(-1 , &(connected));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsIsDisplayConnected(handle, &connected);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check connection status with null pointer
        status = dsIsDisplayConnected(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Check connection status after termination
    status = dsIsDisplayConnected(handle, &connected);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsIsDisplaySurround()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsIsDisplaySurround() - by looping through the acquired port handles to check if display connected to video port supports the audio surround | handle: [ loop through valid handles ] , surround: [pointer to hold the audio surround support] | dsERR_NONE | Audio surround support of Video Port must be returned |
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsIsDisplaySurround(void) {
    gTestID = 11;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool isSurround1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);

        // Step 03: Check if the connected display supports audio surround
        status = dsIsDisplaySurround(handle, &isSurround1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Compare the value with value from profile file
        UT_ASSERT_EQUAL(isSurround1, gDSVideoPortConfiguration[i].DisplaySurround);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Ensure dsIsDisplaySurround() returns correct error codes during negative scenarios
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * 
 * **Pre-Conditions:**@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsIsDisplaySurround() - Attempt to check if display connected to video port has surround support without initializing video ports | handle: [ invalid handle ], , surround: [ pointer to hold Audio surround support] | dsERR_NOT_INITIALIZED | Call to check if display connected to video port has surround support must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsIsDisplaySurround() using an invalid handle but with a valid pointer to hold the surround support | handle: [ invalid handle ], surround: [ pointer to hold Audio surround support] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsIsDisplaySurround() by looping through acquired port handles but with a null pointer for audio surround | handle: [ loop through valid handles ], surround: [ NULL ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsIsDisplayConnected() - Attempt to check if display connected to video port has surround support without initializing video ports | handle: [ valid handle ], , surround: [ pointer to hold Audio surround support] | dsERR_NOT_INITIALIZED | Call to check if display connected to video port has surround support must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsIsDisplaySurround(void) {
    gTestID = 12;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool surround = false;

    // Step 01: Attempt to check audio surround support without initialization
    status = dsIsDisplaySurround(-1, &surround);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsIsDisplaySurround(handle, &surround);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check audio surround support with null pointer
        status = dsIsDisplaySurround(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Check audio surround support after termination
    status = dsIsDisplaySurround(handle, &surround);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetSurroundMode()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetSurroundMode() - by looping through the acquired port handles to get the surround mode of each video port | handle: [ loop through valid handles ] , surround: [pointer to hold the Surround mode] | dsERR_NONE | Surround mode (dsSURROUNDMode_t)of Video Port must be returned or indicate that the operation isn't supported |
 * |04|Compare the values with the value from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetSurroundMode(void) {
    gTestID = 13;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    int surroundMode1 = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Get the surround mode of each video port
        status = dsGetSurroundMode(handle, &surroundMode1);
        if (gSourceType == 1) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
            // Step 04: Compare value with profile file
            UT_ASSERT_EQUAL(surroundMode1,gDSVideoPortConfiguration[i].SurroundMode);
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetSurroundMode()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetSurroundMode() - Attempt to get the surround mode without initializing video ports | handle: [ invalid handle ], surround: [pointer to hold the surround mode] | dsERR_NOT_INITIALIZED | Get surround mode of video ports must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetSurroundMode() using an invalid handle but with a valid pointer to hold the surround mode | handle: [ invalid handle ], surround: [ pointer to hold surround mode] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsGetSurroundMode() by looping through acquired port handles but with a null pointer for surround mode | handle: [ loop through valid handles ], surround: [ NULL ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetSurroundMode() - Attempt to get the surround mode without initializing video ports | handle: [ valid handle ], surround: [pointer to hold the surround mode] | dsERR_NOT_INITIALIZED | Get surround mode of video ports must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetSurroundMode(void) {
    gTestID = 14;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    int surroundMode = 0;

    // Step 01: Attempt to get surround mode without initialization
    status = dsGetSurroundMode(-1, &surroundMode);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetSurroundMode(handle, &surroundMode);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check surround mode with null pointer
        status = dsGetSurroundMode(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get surround mode after termination
    status = dsGetSurroundMode(handle, &surroundMode);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsIsVideoPortActive()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsIsVideoPortActive() - by looping through the acquired port handles to check whether a video port is active or not for each port and store it | handle: [ loop through valid handles ] , active: [pointer to hold the active status] | dsERR_NONE | Active Status(A Video port is active if it is connected to the active port of sink device) of Video Port must be returned |
 * |04|Call dsIsVideoPortActive() - by looping through the acquired port handles to check whether a video port is active or not for each port and store it | handle: [ loop through valid handles ] , active: [pointer to hold the active status] | dsERR_NONE | Active Status(A Video port is active if it is connected to the active port of sink device) of Video Port must be returned |
 * |05|Check if the values are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsIsVideoPortActive(void) {
    gTestID = 15;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        bool isActive1 = false;
        bool isActive2 = false;
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Check whether each video port is active
        status = dsIsVideoPortActive(handle, &isActive1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Repeat the active status check
        status = dsIsVideoPortActive(handle, &isActive2);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check if the values are equal
        UT_ASSERT_EQUAL(isActive1, isActive2);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsIsVideoPortActive()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsIsVideoPortActive() - Attempt to check whether a video port is active or not without initializing video ports |  handle: [ invalid handle ], active: [pointer to hold the active status of video port] | dsERR_NOT_INITIALIZED | Call to check if video port is active or not must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsIsVideoPortActive() using an invalid handle but with a valid pointer to hold the active status | handle: [ invalid handle ], active: [ pointer to hold active status] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsIsVideoPortActive() by looping through acquired port handles but with a null pointer for active status | handle: [ loop through valid handles ], active: [ NULL ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsIsVideoPortActive() - Attempt to check whether a video port is active or not without initializing video ports | handle: [ valid handle ], active: [pointer to hold the active status of video port] | dsERR_NOT_INITIALIZED | Call to check if video port is active or not must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsIsVideoPortActive(void) {
    gTestID = 16;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool active = false;

    // Step 01: Attempt to check active status without initialization
    status = dsIsVideoPortActive(-1, &active);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsIsVideoPortActive(handle, &active);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check active status with null pointer
        status = dsIsVideoPortActive(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to check active status after termination
    status = dsIsVideoPortActive(handle, &active);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsEnableHDCP()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsEnableHDCP() by looping through the acquired port handles to enable HDCP for source device ports which support HDCP | handle: [ loop through valid handles ] , contentProtect: [1] (enable HDCP), hdcpKey: [ HDCP key ], keySize: [HDCP key size]|dsERR_NONE| API must either successfully enable HDCP or indicate that the operation isn't supported if the particular video does not have HDCP support|
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsEnableHDCP(void) {
    gTestID = 17;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool enableHDCP = false;
    char hdcpKey[HDCP_KEY_MAX_SIZE] = "ADEF";
    int keySize = HDCP_KEY_MAX_SIZE;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);

        // Step 03: Enable HDCP
        enableHDCP = gDSVideoPortConfiguration[i].hdcp_supported;
        status = dsEnableHDCP(handle, enableHDCP, hdcpKey, keySize);
        if (gSourceType == 1) {
            if (gDSVideoPortConfiguration[i].hdcp_supported == true) {
                UT_ASSERT_EQUAL(status, dsERR_NONE);
            } else {
                UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
            }
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        } 
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsEnableHDCP()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsEnableHDCP() - Attempt to enable HDCP without initializing the video ports | handle: [ invalid handle ], contentProtect: [true = enable/ false = disable], hdcpKey: [valid hdcp key], keySize: [valid key size]| dsERR_NOT_INITIALIZED | Enable HDCP must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsEnableHDCP() - Attempt to enable HDCP with an invalid handle |handle: [ invalid handle ], contentProtect: [true = enable/ false = disable], hdcpKey: [valid hdcp key], keySize: [valid key size (HDCP_KEY_MAX_SIZE)]| dsERR_INVALID_PARAM | Invalid Parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsEnableHDCP() by looping through acquired port handles(which support HDCP) with an invalid HDCP key size |handle: [ valid handle ], contentProtect: [true = enable/ false = disable], hdcpKey: [valid hdcp key], keySize: [invalid key size (> HDCP_KEY_MAX_SIZE)| dsERR_INVALID_PARAM | Invalid Parameter error must be returned |
 * |06|Call dsEnableHDCP() by looping through acquired port handles with an NULL hdcpKey pointer |handle: [ valid handle ], contentProtect: [true = enable/ false = disable], hdcpKey: [NULL], keySize: [valid key size] | dsERR_INVALID_PARAM | Invalid Parameter error must be returned |
 * |07|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |08|Call dsEnableHDCP() Attempt to enable HDCP without initializing the video ports | handle: [ valid handle ], contentProtect: [true = enable/ false = disable], hdcpKey: [valid hdcp key], keySize: [valid key size] | dsERR_NOT_INITIALIZED | Enable HDCP must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsEnableHDCP(void) {
    gTestID = 18;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool enableHDCP = gDSVideoPortConfiguration[0].hdcp_supported;
    char hdcpKey[HDCP_KEY_MAX_SIZE] = "ADEF";
    int keySize = HDCP_KEY_MAX_SIZE;

    // Step 01: Attempt to enable HDCP without initialization
    status = dsEnableHDCP(-1, enableHDCP, hdcpKey, keySize);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Attempt to enable HDCP with an invalid handle
    status = dsEnableHDCP(handle, enableHDCP, hdcpKey, keySize);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the video port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);

        // Step 05: Enable HDCP with invalid key size
        enableHDCP = gDSVideoPortConfiguration[i].hdcp_supported;
        status = dsEnableHDCP(handle,enableHDCP, hdcpKey, HDCP_KEY_MAX_SIZE+1);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

        // Step 06: Enable HDCP with NULL hdcpKey pointer
        status = dsEnableHDCP(handle, enableHDCP, NULL, keySize);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 07: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 08: Attempt to enable HDCP after termination
    status = dsEnableHDCP(handle, enableHDCP, hdcpKey, keySize);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsIsHDCPEnabled()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsIsHDCPEnabled() by looping through the acquired port handles  and checks whether HDCP enabled or not(.ie TRUE / FALSE) | handle : [loop through valid handles] , pContentProtected = [pointer to hold data ] | dsERR_NONE | dsIsHDCPEnabled must return a valid value or indicate that the operation isn't supported|
 * |04|Compare the values with values of profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video ports of a system| |dsERR_NONE|Termination must be successful|
 * 
 */
void test_l1_dsVideoPort_positive_dsIsHDCPEnabled(void) {
    gTestID = 19;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool isHDCPEnabled1 = false;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Check if HDCP is enabled
        status = dsIsHDCPEnabled(handle, &isHDCPEnabled1);
        // Step 04: Compare the value with profile values
        if (gSourceType == 1) {
            if (gDSVideoPortConfiguration[i].hdcp_supported == false) {
                UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
            } else {
                UT_ASSERT_EQUAL(status, dsERR_NONE);
                UT_ASSERT_EQUAL(isHDCPEnabled1, gDSVideoPortConfiguration[i].hdcp_supported);
            }
        } else if ((gSourceType == 0)) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
            UT_ASSERT_EQUAL(isHDCPEnabled1, true);
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsIsHDCPEnabled()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsIsHDCPEnabled() - Attempt to get the HDCP status with out initializing video ports| handle [invalid handle] , pContentProtected = [valid pointer]|dsERR_NOT_INITIALIZED| dsIsHDCPEnabled call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |03|Call dsIsHDCPEnabled() - Using an invalid handle but with valid pointer | handle = [invalid handle], pContentProtected = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort()  - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsIsHDCPEnabled() - By looping through acquired port handles but with a invalid pointer | handle = [valid handle], pContentProtected = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video ports of a system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsIsHDCPEnabled() - Again after terminating video ports attempt to get HDCP status | handle= [valid handle from step 04 ] , pContentProtected = [valid pointer] | dsERR_NOT_INITIALIZED | dsIsHDCPEnabled call must fail as module is not initialized |
 *
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsIsHDCPEnabled(void) {
    gTestID = 20;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool contentProtected = false;

    // Step 01: Attempt to get HDCP status without initialization
    status = dsIsHDCPEnabled(-1, &(contentProtected));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Attempt to get HDCP status with an invalid handle
    status = dsIsHDCPEnabled(handle, &(contentProtected));
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the video port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get HDCP status with invalid pointer
        status = dsIsHDCPEnabled(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get HDCP status after termination
    status = dsIsHDCPEnabled(handle, &(contentProtected));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsEnableVideoPort()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] | dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsEnableVideoPort() by looping through the acquired port handles to enable video ports | handle  = [loop through valid handles] , enabled = [TRUE] | dsERR_NONE | Video ports must be enabled |
 * |04|Call dsVideoPortTerm() - Terminate the video ports of a system| |dsERR_NONE|Termination must be successful|
 * 
 */
void test_l1_dsVideoPort_positive_dsEnableVideoPort(void) {
    gTestID = 21;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;
    
    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Enable the video port
        status = dsEnableVideoPort(handle, true);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsEnableVideoPort()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsEnableVideoPort() - Attempt to enable or disable video port  with out initializing video ports| handle [invalid handle] , enabled = [valid bool value]| dsERR_NOT_INITIALIZED| dsEnableVideoPort call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |03|Call dsEnableVideoPort() - Using an invalid handle but with valid enabled value(i.e TRUE/FALSE) | handle = [invalid handle], enabled = [valid bool value] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort()  - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsVideoPortTerm() - Terminate the video ports of a system | | dsERR_NONE | Termination must be successful |
 * |06|Call dsEnableVideoPort() - Again after terminating video ports attempt to enable or disable video port | handle = [valid handle ] , enabled = [valid bool value] | dsERR_NOT_INITIALIZED | dsEnableVideoPort call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsEnableVideoPort(void) {
    gTestID = 22;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    intptr_t handle = 0;
    bool enabled = true; // Example value for enabling/disabling the port

    // Step 01: Attempt to enable/disable video port without initialization
    status = dsEnableVideoPort(-1, enabled);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsEnableVideoPort(handle, enabled);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 06: Attempt to enable/disable video port after termination
    status = dsEnableVideoPort(handle, enabled);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsSetResolution()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsSetResolution() by looping through the acquired port handles and valid resolution pointer to set the video port resolution |handle  = [loop through valid handles] , resolution = [valid pointer]|dsERR_NONE|Resolution must be set successfully or indicate that the operation isn't supported|
 * |04|Call dsVideoPortTerm() - Terminate the video ports of a system| |dsERR_NONE|Termination must be successful|
 * 
 */
void test_l1_dsVideoPort_positive_dsSetResolution(void) {
    gTestID = 23;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;


    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Set the resolution for the video port
        for(int j =0; j < gDSVideoPortConfiguration[i].numSupportedResolutions; j++) {
            //strcpy(setResolution.name, gDSVideoPortConfiguration[i].supportedResolutions[j]);
            status = dsSetResolution(handle, &(gDSVideoPortConfiguration[i].supportedResolutions[j]));
            if (gSourceType == 1) {
                UT_ASSERT_EQUAL(status, dsERR_NONE);
            } else if (gSourceType == 0) {
                UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
            }
        }
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSetResolution()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSetResolution() - Attempt to set the resolution with out initializing video ports| handle = [invalid handle] , resolution = [valid pointer]| dsERR_NOT_INITIALIZED| dsSetResolution call must fail as module is not initialized |
 * |02|dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |03|Call dsSetResolution() - Using an invalid handle but with valid resolution pointer | handle = [invalid handle], resolution = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsSetResolution() - By looping through acquired port handles but with a invalid pointer | handle = [valid handle], resolution = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsSetResolution() - By looping through acquired port handles but with a out of range resolution paramerters | handle = [valid handle], resolution = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |07|Call dsVideoPortTerm() - Terminate the video ports of a system | | dsERR_NONE | Termination must be successful |
 * |08|Call dsSetResolution() - Again after terminating video ports attempt to set the resolution | handle= [valid handle from step 04 ] , resolution = [valid pointer] | dsERR_NOT_INITIALIZED | dsSetResolution call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSetResolution(void) {
    gTestID = 24;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Attempt to set resolution without initialization
    status = dsSetResolution(-1, &(gDSVideoPortConfiguration[0].supportedResolutions[0]));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsSetResolution(handle, &(gDSVideoPortConfiguration[0].supportedResolutions[0]));
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Set resolution with invalid pointer
        status = dsSetResolution(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 06: Set resolution with invalid resolution parameters
        dsVideoPortResolution_t  resolutions;
        strcpy(resolutions.name, "NULL");
        resolutions.pixelResolution = dsVIDEO_PIXELRES_MAX;
        resolutions.aspectRatio = dsVIDEO_ASPECT_RATIO_MAX;
        resolutions.stereoScopicMode = dsVIDEO_SSMODE_MAX;
        resolutions.frameRate = dsVIDEO_FRAMERATE_MAX;
        resolutions.interlaced = dsVIDEO_SCANMODE_PROGRESSIVE;
        status = dsSetResolution(handle, &resolutions);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 07: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 08: Attempt to set resolution after termination
    status = dsSetResolution(handle, &(gDSVideoPortConfiguration[0].supportedResolutions[0]));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetResolution()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsGetResolution() by looping through the acquired port handles and valid resolution pointer to retrieve the resolution of a video port | handle  = [loop through valid handles] , resolution = [valid pointer] |dsERR_NONE|Resolution must be retrieved successfully|
 * |04|Compare the values with default values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video ports of a system| |dsERR_NONE|Termination must be successful|
 * 
 */
void test_l1_dsVideoPort_positive_dsGetResolution(void) {
    gTestID = 25;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0; 

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        dsVideoPortResolution_t resolution1;
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the resolution of the video port
        status = dsGetResolution(handle, &(resolution1));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        for (int j = 0; j < gDSVideoPortConfiguration[i].numSupportedResolutions; j++) {
            if(!strncmp(gDSVideoPortConfiguration[i].supportedResolutions[j].name, gDSVideoPortConfiguration[i].defaultResolution, DS_VIDEO_PORT_RESOLUTION_NAME_MAX)) {
                UT_ASSERT_EQUAL(resolution1.pixelResolution, gDSVideoPortConfiguration[i].supportedResolutions[j].pixelResolution);
                UT_ASSERT_EQUAL(resolution1.aspectRatio, gDSVideoPortConfiguration[i].supportedResolutions[j].aspectRatio);
                UT_ASSERT_EQUAL(resolution1.stereoScopicMode, gDSVideoPortConfiguration[i].supportedResolutions[j].stereoScopicMode);
                UT_ASSERT_EQUAL(resolution1.frameRate, gDSVideoPortConfiguration[i].supportedResolutions[j].frameRate);
                UT_ASSERT_EQUAL(resolution1.interlaced, gDSVideoPortConfiguration[i].supportedResolutions[j].interlaced);
            }
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetResolution()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetResolution() - Attempt to get the resolution without initializing video ports| handle = [invalid handle] , resolution = [valid pointer]| dsERR_NOT_INITIALIZED| Get Resolution call must fail as module is not initialized |
 * |02|dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |03|Call dsGetResolution() - Using an invalid handle but with valid resolution pointer | handle = [invalid handle], resolution = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |04|Call dsGetResolution() - By looping through acquired port handles but with a invalid pointer | handle = [valid handle], resolution = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |05|Call dsVideoPortTerm() - Terminate the video ports of a system | | dsERR_NONE | Termination must be successful |
 * |06|Call dsGetResolution() - Again after terminating video ports attempt to get the resolution of video ports | handle = [valid handle] , resolution = [valid pointer] | dsERR_NOT_INITIALIZED | Get Resolution call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetResolution(void) {
    gTestID = 26;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsVideoPortResolution_t resolution;

    // Step 01: Attempt to get resolution without initialization
    status = dsGetResolution(-1, &(resolution));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetResolution( handle , &(resolution));
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get resolution with invalid pointer
        status = dsGetResolution(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get resolution after termination
    status = dsGetResolution(handle, &(resolution));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsVideoFormatUpdateRegisterCB()
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 *
 * **Pre-Conditions:** None@n
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsVideoFormatUpdateRegisterCB() - Register a valid Video Format update callback | cb = [valid callback function] | dsERR_NONE | Video Format Update callback registration must be successful |
 * |03|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 *
 */

/*callback*/
void mockVideoFormatCallback(dsHDRStandard_t videoFormat){
    // Mock implementation, can be customized for testing
}

void test_l1_dsVideoPort_positive_dsVideoFormatUpdateRegisterCB(void) {
    gTestID = 27;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Register a valid Video Format update callback
    status = dsVideoFormatUpdateRegisterCB(mockVideoFormatCallback);
    UT_ASSERT_EQUAL(status, dsERR_NONE);

    // Step 03: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsVideoFormatUpdateRegisterCB()
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 *
 * **Pre-Conditions:** None@n
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoFormatUpdateRegisterCB() - Attempt to register for video format update callback without initializing the video ports | cb = [valid callback function ] | dsERR_NOT_INITIALIZED | Callback registration must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsVideoFormatUpdateRegisterCB() with an invalid callback (NULL)| cb = NULL | dsERR_INVALID_PARAM | Invalid paramter error must be returned |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |05|Call dsVideoFormatUpdateRegisterCB() - Attempt to register for video format update callback without initializing the video ports | cb = [valid callback function ] | dsERR_NOT_INITIALIZED | Callback registration must fail as module is not initialized |
 *
 *
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsVideoFormatUpdateRegisterCB(void) {
    gTestID = 28;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Attempt to register callback without initialization
    status = dsVideoFormatUpdateRegisterCB(mockVideoFormatCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Register with invalid callback (NULL)
    status = dsVideoFormatUpdateRegisterCB(NULL);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 05: Attempt to register callback after termination
    status = dsVideoFormatUpdateRegisterCB(mockVideoFormatCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Positive Test Scenarios for dsRegisterHdcpStatusCallback()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsRegisterHdcpStatusCallback() - Register a valid HDCP status change callback |handle = [valid handle], cb = [Callback function] | dsERR_NONE | Callback must be registered successfully |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */

/*callback */
void myHdcpStatusCallbackFunction(intptr_t handle, dsHdcpStatus_t status)
{
    /*stub callback*/
}

void test_l1_dsVideoPort_positive_dsRegisterHdcpStatusCallback(void) {
    gTestID = 29;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Assume dsHdcpStatusCallback_t is defined and a valid callback function is implemented
    dsHDCPStatusCallback_t validHdcpStatusCallback = myHdcpStatusCallbackFunction;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle and register HDCP status callback
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);

        // Step 03: Register HDCP status change callback
        status = dsRegisterHdcpStatusCallback(handle, validHdcpStatusCallback);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsRegisterHdcpStatusCallback()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsRegisterHdcpStatusCallback() - Attempt to register HDCP status change callback without initializing the video ports | handle= [invalid handle], cb = [valid callback function] | dsERR_NOT_INITIALIZED | Callback Registration for HDCP status change must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsRegisterHdcpStatusCallback() with an invalid handle but with a valid callback function as parameter | handle= [invalid handle], cb = [valid callback function] | dsERR_INVALID_PARAM | Invalid Parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsRegisterHdcpStatusCallback() with a valid handle and a NULL callback function | handle= [valid handle], cb = NULL | dsERR_INVALID_PARAM | Invalid Parameter error must be returned |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |06|Call dsRegisterHdcpStatusCallback() - Attempt to register HDCP status change callback without initializing the video ports | handle= [valid handle], cb = [valid callback function] | dsERR_NOT_INITIALIZED | Callback Registration for HDCP status change must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsRegisterHdcpStatusCallback(void) {
    gTestID = 30;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0 /* Initialize with invalid handle */;

    // Assume dsHdcpStatusCallback_t is defined
    dsHDCPStatusCallback_t validHdcpStatusCallback = myHdcpStatusCallbackFunction;

    // Step 01: Attempt to register HDCP status callback without initialization
    status = dsRegisterHdcpStatusCallback(-1, validHdcpStatusCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsRegisterHdcpStatusCallback(handle, validHdcpStatusCallback);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Register HDCP status callback with NULL callback function
        status = dsRegisterHdcpStatusCallback(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to register HDCP status callback after termination
    status = dsRegisterHdcpStatusCallback(handle, validHdcpStatusCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetHDCPStatus()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetHDCPStatus() by looping through the acquired port handles and valid pointer to retrieve HDCP status | handle  = [valid handles] status = [valid pointer] | dsERR_NONE | The HDCP status must be successfully fetched and stored in the given pointer or indicate that the operation isn't supported|
 * |04|Compare the values with the HDCP status for sink and source devices respectively | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetHDCPStatus(void) {
    gTestID = 31;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID); 

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        dsHdcpStatus_t hdcpStatus1;
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the HDCP status
        status = dsGetHDCPStatus(handle, &(hdcpStatus1));
        // Step 04: Compare the value with values of profile file
        if (gSourceType == 0) {
            if (gDSVideoPortConfiguration[i].hdcp_supported == false) {
                UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
            } else {
                UT_ASSERT_EQUAL(status, dsERR_NONE);
                UT_ASSERT_EQUAL(hdcpStatus1, dsHDCP_STATUS_AUTHENTICATED);
            }
        } else if(gSourceType == 1) {
            UT_ASSERT_EQUAL(hdcpStatus1, dsHDCP_STATUS_PORTDISABLED);
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetHDCPStatus()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 032@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetHDCPStatus() Attempt to get the HDCP status  with out initializing video ports| handle = [invalid handle] , status = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetHDCPStatus call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetHDCPStatus() Using an invalid handle but with valid status pointer | handle = [invalid handle], status = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |05|Call dsGetHDCPStatus() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], status = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetHDCPStatus() Again after terminating video ports attempt to get the status of video ports | handle= [valid handle ] , status = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetHDCPStatus call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetHDCPStatus(void) {
    gTestID = 32;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    intptr_t handle = 0;
    dsHdcpStatus_t hdcpStatus;

    // Step 01: Attempt to get HDCP status without initialization
    status = dsGetHDCPStatus(-1, &(hdcpStatus));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetHDCPStatus(handle, &(hdcpStatus));
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get HDCP status with invalid pointer
        status = dsGetHDCPStatus(handle , NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get HDCP status after termination
    status = dsGetHDCPStatus(handle, &(hdcpStatus));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}



/**
 * @brief Positive Test Scenarios for dsGetHDCPProtocol()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 033@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetHDCPProtocol() by looping through the acquired suported port handles and valid pointer to retrieve the protocol version  of a video port |handle  = [valid handles] , protocolVersion = [valid pointer] |dsERR_NONE|The HDCP protocol version must be successfully fetched |
 * |04|Compare the values with the values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetHDCPProtocol(void) {
    gTestID = 33;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHdcpProtocolVersion_t protocolVersion1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the HDCP protocol version
        status = dsGetHDCPProtocol(handle, &(protocolVersion1));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        UT_ASSERT_EQUAL(protocolVersion1, gDSVideoPortConfiguration[i].hdcp_protocol_version);
    }


    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Negative Test Scenarios for dsGetHDCPProtocol()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetHDCPProtocol()  Attempt to get the HDCP protocol version with out initializing video ports| handle = [invalid handle] , protocolVersion = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetHDCPProtocol call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetHDCPProtocol() Using an invalid handle but with valid pointer | handle = [invalid handle], protocolVersion = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |05|Call dsGetHDCPProtocol() By looping through acquired supported port handles but with a invalid pointer | handle = [valid handle], protocolVersion = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetHDCPProtocol() Again after terminating video ports attempt to get the HDCP protocol version of video ports | handle= [ valid handle ] , protocolVersion = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetHDCPProtocol call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetHDCPProtocol(void) {
    gTestID = 34;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHdcpProtocolVersion_t protocolVersion;

    // Step 01: Attempt to get HDCP protocol without initialization
    status = dsGetHDCPProtocol(-1, &(protocolVersion));
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetHDCPProtocol(handle, &(protocolVersion));
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get HDCP protocol with invalid pointer
        status = dsGetHDCPProtocol(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get HDCP protocol after termination
    status = dsGetHDCPProtocol(handle, &protocolVersion);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetHDCPReceiverProtocol()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 035@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetHDCPReceiverProtocol() by looping through the acquired supported port handles and valid pointer to retrieve the sink device protocol version of a video port |handle  = [loop through valid handles] , protocolVersion = [valid pointer] |dsERR_NONE|The Receiver HDCP protocol version must be successfully fetched and stored in the given pointer|
 * |05|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetHDCPReceiverProtocol(void) {
    gTestID = 35;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    intptr_t  handle = 0;

    dsHdcpProtocolVersion_t protocolVersion1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the Receiver HDCP protocol version
        status = dsGetHDCPReceiverProtocol(handle, &(protocolVersion1));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        UT_ASSERT_EQUAL(protocolVersion1, gDSVideoPortConfiguration[i].hdcp_protocol_version);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetHDCPReceiverProtocol()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetHDCPReceiverProtocol()  Attempt to get the sink device HDCP protocol version of sink device with out initializing video ports| handle = [invalid handle] , protocolVersion = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetHDCPReceiverProtocol call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetHDCPReceiverProtocol() Using an invalid handle but with valid pointer | handle = [invalid handle], protocolVersion = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |05|Call dsGetHDCPReceiverProtocol() By looping through acquired supported port handles but with a invalid pointer | handle = [valid handle], protocolVersion = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetHDCPReceiverProtocol() Again after terminating video ports attempt to get the sink device HDCP protocol version of video ports | handle= [valid handle ] , protocolVersion = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetHDCPReceiverProtocol call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetHDCPReceiverProtocol(void) {
    gTestID = 36;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHdcpProtocolVersion_t receiverProtocolVersion;

    // Step 01: Attempt to get Receiver HDCP protocol without initialization
    status = dsGetHDCPReceiverProtocol(-1, &receiverProtocolVersion);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetHDCPReceiverProtocol(handle, &receiverProtocolVersion);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get Receiver HDCP protocol with invalid pointer
        status = dsGetHDCPReceiverProtocol(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get Receiver HDCP protocol after termination
    status = dsGetHDCPReceiverProtocol(handle, &receiverProtocolVersion);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetHDCPCurrentProtocol()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 037@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetHDCPCurrentProtocol() by looping through the acquired supported port handles and valid pointer to retrieve the current negotiated protocol version  of a video port |handle  = [valid handles] , protocolVersion = [valid pointer] |dsERR_NONE|The current HDCP protocol version must be successfully fetched |
 * |05|Compare the values with value from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetHDCPCurrentProtocol(void) {
    gTestID = 37;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t  handle = 0;

    dsHdcpProtocolVersion_t currentProtocol1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the current negotiated HDCP protocol version
        status = dsGetHDCPCurrentProtocol(handle, &currentProtocol1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with profile file values
        UT_ASSERT_EQUAL(currentProtocol1, gDSVideoPortConfiguration[i].hdcp_protocol_version);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetHDCPCurrentProtocol()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetHDCPCurrentProtocol()  Attempt to get the current negotiated HDCP protocol version with out initializing video ports| handle = [invalid handle] , protocolVersion = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetHDCPCurrentProtocol call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetHDCPCurrentProtocol() Using an invalid handle but with valid pointer | handle = [invalid handle], protocolVersion = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |05|Call dsGetHDCPCurrentProtocol() By looping through acquired supported port handles but with a invalid pointer | handle = [valid handle], protocolVersion = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetHDCPCurrentProtocol() Again after terminating video ports attempt to get the current negotiated  HDCP protocol version of video ports | handle= [valid handle ] , protocolVersion = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetHDCPCurrentProtocol call must fail as module is not initialized |
 *
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetHDCPCurrentProtocol(void) {
    gTestID = 38;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t  handle = 0;

    dsHdcpProtocolVersion_t currentProtocolVersion;

    // Step 01: Attempt to get current negotiated HDCP protocol without initialization
    status = dsGetHDCPCurrentProtocol(-1, &currentProtocolVersion);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetHDCPCurrentProtocol(handle, &currentProtocolVersion);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get current HDCP protocol with invalid pointer
        status = dsGetHDCPCurrentProtocol(handle , NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get current negotiated HDCP protocol after termination
    status = dsGetHDCPCurrentProtocol(handle, &currentProtocolVersion);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetTVHDRCapabilities()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 039@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsGetTVHDRCapabilities() by looping through the acquired port handles and valid pointer to retrieve the HDR capabilities of a video port | handle  = [loop through valid handles] , capabilities = [valid pointer] |dsERR_NONE|The HDR capabilities must be successfully fetched and stored in the given pointer|
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetTVHDRCapabilities(void) {
    gTestID = 39;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    int capabilities1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the HDR capabilities
        status = dsGetTVHDRCapabilities(handle, &capabilities1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with profile file values
        UT_ASSERT_EQUAL(capabilities1, gDSVideoPortConfiguration[i].hdr_capabilities);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetTVHDRCapabilities()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetTVHDRCapabilities() Attempt to get the HDR capabilites  with out initializing video ports| handle = [invalid handle] , capabilities = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetTVHDRCapabilities call must fail as module is not initialized |
 * |02|dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |03|Call dsGetTVHDRCapabilities() Using an invalid handle but with valid pointer | handle = [invalid handle], capabilities = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |04|Call dsGetTVHDRCapabilities() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], capabilities = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |06|Call dsGetTVHDRCapabilities() Again after terminating video ports attempt to get the HDR capabilites of video ports | handle= [valid handle ] , capabilities = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetTVHDRCapabilities call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetTVHDRCapabilities(void) {
    gTestID = 40;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    int capabilities;

    // Step 01: Attempt to get HDR capabilities without initialization
    status = dsGetTVHDRCapabilities(-1, &capabilities);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetTVHDRCapabilities(handle, &capabilities);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get HDR capabilities with invalid pointer
        status = dsGetTVHDRCapabilities(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get HDR capabilities after termination
    status = dsGetTVHDRCapabilities(handle, &capabilities);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsSupportedTvResolutions()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 041@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsSupportedTvResolutions() by looping through the acquired port handles and valid pointer to retrieve the resolutions of a video port |handle  = [loop through valid handles] , resolutions = [valid pointer] |dsERR_NONE|Resolutions must be set successfully|
 * |05|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video ports of a system| |dsERR_NONE|Termination must be successful|
 * 
 */
void test_l1_dsVideoPort_positive_dsSupportedTvResolutions(void) {
    gTestID = 41;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    int resolutions1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the supported TV resolutions
        status = dsSupportedTvResolutions(handle, &resolutions1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        UT_ASSERT_EQUAL(resolutions1, gDSVideoPortConfiguration[i].Supported_tv_resolutions_capabilities);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSupportedTvResolutions()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 042@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSupportedTvResolutions() - Attempt to get the supported resolutions with out initializing video ports| handle =  [invalid handle] , resolutions = [valid pointer]| dsERR_NOT_INITIALIZED| dsSupportedTvResolutions call must fail as module is not initialized |
 * |02|dsVideoPortInit() - Initialize video ports of a system | |dsERR_NONE| Initialization must be successful |
 * |03|Call dsSupportedTvResolutions() - Using an invalid handle but with valid resolutions pointer | handle = [invalid handle], resolutions = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |04|Call dsSupportedTvResolutions() - By looping through acquired port handles but with a invalid pointer | handle = [valid handle], resolutions = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |05|Call dsVideoPortTerm() - Terminate the video ports of a system | | dsERR_NONE | Termination must be successful |
 * |06|Call dsSupportedTvResolutions() - Again after terminating video ports attempt to get the supported resolutions of video ports | handle= [valid handle ] , resolutions = [valid pointer] | dsERR_NOT_INITIALIZED | dsSupportedTvResolutions call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSupportedTvResolutions(void) {
    gTestID = 42;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    int resolutions;

    // Step 01: Attempt to get supported resolutions without initialization
    status = dsSupportedTvResolutions(-1, &resolutions);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsSupportedTvResolutions(handle, &resolutions);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get supported resolutions with invalid pointer
        status = dsSupportedTvResolutions(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get supported resolutions after termination
    status = dsSupportedTvResolutions(handle, &resolutions);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsSetForceDisable4KSupport()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 043@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsSetForceDisable4KSupport() by looping through the acquired port handles and valid value to Set the 4K support to be forcefully disabled or not | handle  = [loop through valid handles] , disable= [valid value] | dsERR_NONE | The 4K support must be successfully disabled/not or indicate that the operation isn't supported |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsSetForceDisable4KSupport(void) {
    gTestID = 43;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool disable4K = true;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Set force disable 4K support
        status = dsSetForceDisable4KSupport(handle, disable4K);
        UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSetForceDisable4KSupport()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSetForceDisable4KSupport() Attempt to disable or not to disable 4K support with out initializing video ports| handle = [invalid handle] , disable = [valid value]| dsERR_NOT_INITIALIZED| dsSetForceDisable4KSupport call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsSetForceDisable4KSupport() Using an invalid handle but with valid disable parameter value | handle = [invalid handle], disable = [valid value] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |06|Call dsSetForceDisable4KSupport() Again after terminating video ports attempt to disable or not to disable 4k support of video ports | handle= [valid handle ] , disable = [valid value] | dsERR_NOT_INITIALIZED | dsSetForceDisable4KSupport call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSetForceDisable4KSupport(void) {
    gTestID = 44;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool disable4K = true;

    // Step 01: Attempt to set force disable 4K support without initialization
    status = dsSetForceDisable4KSupport(-1, disable4K);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsSetForceDisable4KSupport(handle, disable4K);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 06: Attempt to set force disable 4K support after termination
    status = dsSetForceDisable4KSupport(handle, disable4K);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetForceDisable4KSupport()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 045@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call  dsGetForceDisable4KSupport() by looping through the acquired port handles and valid pointer to get the 4K support disabled or not and store it | handle  = [loop through valid handles] , disable= [valid pointer] | dsERR_NONE | The 4K support disabled or not must be successfully retrieved |
 * |04|Call  dsGetForceDisable4KSupport() - Again by looping through the acquired port handles and valid pointer to get the 4K support disabled or not and store it | handle  = [loop through valid handles] , disable= [valid pointer] | dsERR_NONE | The 4K support disabled or not must be successfully retrieved or indicate that the operation isn't supported |
 * |05|Check if the values are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful | 
 * 
 */
void test_l1_dsVideoPort_positive_dsGetForceDisable4KSupport(void) {
    gTestID = 45;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool disable4K1 = false;
    bool disable4K2 = false;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve whether 4K support is disabled
        status = dsGetForceDisable4KSupport(handle, &disable4K1);
        UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        // Step 04: Repeat the retrieval
        status = dsGetForceDisable4KSupport(handle, &disable4K2);
        UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        // Step 05: Check if the values are equal
        UT_ASSERT_EQUAL(disable4K1, disable4K2);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetForceDisable4KSupport()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetForceDisable4KSupport() Attempt to get 4k support disable or not with out initializing video ports| handle = [invalid handle] , disable = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetForceDisable4KSupport call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetForceDisable4KSupport() Using an invalid handle but with valid pointer | handle = [invalid handle], disable = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetForceDisable4KSupport() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], disable = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetForceDisable4KSupport() Again after terminating video ports attempt to get whether 4K support of video ports disabled or not | handle= [valid handle ] , disable = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetForceDisable4KSupport call must fail as module is not initialized | 
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetForceDisable4KSupport(void) {
    gTestID = 46;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool disable4K = false;

    // Step 01: Attempt to get 4K support status without initialization
    status = dsGetForceDisable4KSupport(-1, &disable4K);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetForceDisable4KSupport(handle, &disable4K);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get 4K support status with invalid pointer
        status = dsGetForceDisable4KSupport(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get 4K support status after termination
    status = dsGetForceDisable4KSupport(handle, &disable4K);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetVideoEOTF()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 047@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetVideoEOTF() by looping through the acquired port handles and valid pointer to retrieve the current Electro-Optical Transfer Function (EOTF) value and store it | handle = [loop through ports] , video_eotf = [valid pointer] |dsERR_NONE | Valid EOTF value must return a valid EOTF value of the specified video port|
 * |04|Call dsGetVideoEOTF() again by looping through the acquired port handles and valid pointer to retrieve the current Electro-Optical Transfer Function (EOTF) value and store it | handle = [loop through ports] , video_eotf = [valid pointer] |dsERR_NONE | Valid EOTF value must return a valid EOTF value of the specified video port|
 * |05|Check if the values are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetVideoEOTF(void) {
    gTestID = 47;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHDRStandard_t eotf1;
    dsHDRStandard_t eotf2;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the EOTF value
        status = dsGetVideoEOTF(handle, &eotf1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Retrieve the EOTF value
        status = dsGetVideoEOTF(handle, &eotf2);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check if the values are equal
        UT_ASSERT_EQUAL(eotf1, eotf2);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetVideoEOTF()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 048@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetVideoEOTF() Attempt to get the EOTF  with out initializing video ports| handle = [invalid handle] , video_eotf = [valid pointer] | dsERR_NOT_INITIALIZED| dsGetVideoEOTF call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetVideoEOTF() Using an invalid handle but with valid pointer | handle = [invalid handle], video_eotf = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetVideoEOTF() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], video_eotf = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetVideoEOTF() Again after terminating video ports attempt to get the EOTF of video ports | handle= [invalid handle ] , video_eotf = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetVideoEOTF call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetVideoEOTF(void) {
    gTestID = 48;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHDRStandard_t eotf;

    // Step 01: Attempt to get EOTF without initialization
    status = dsGetVideoEOTF(-1, &eotf);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetVideoEOTF(handle, &eotf);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get EOTF value with invalid pointer
        status = dsGetVideoEOTF(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get EOTF after termination
    status = dsGetVideoEOTF(handle, &eotf);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetMatrixCoefficients()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 049@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetMatrixCoefficients() by looping through the acquired port handles and valid resolution pointer to retrieve the current matrix coefficients of a specified port  |handle  = [loop through valid handles] , matrix_coefficients = [valid pointer] |dsERR_NONE|must return a valid matrix coefficient value of the specified video port|
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetMatrixCoefficients(void) {
    gTestID = 49;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayMatrixCoefficients_t matrixCoefficients1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the matrix coefficients
        status = dsGetMatrixCoefficients(handle, &matrixCoefficients1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        UT_ASSERT_EQUAL(matrixCoefficients1, gDSVideoPortConfiguration[i].matrix_coefficients);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetMatrixCoefficients()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 050@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetMatrixCoefficients() Attempt to get the matrix coefficients with out initializing video ports| handle = [invalid handle] , matrix_coefficients = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetMatrixCoefficients call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetMatrixCoefficients() Using an invalid handle but with valid pointer | handle = [invalid handle], matrix_coefficients = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetMatrixCoefficients() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], matrix_coefficients = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetMatrixCoefficients() Again after terminating video ports attempt to get the matrix coefficients  of video ports | handle= [ valid handle ] , matrix_coefficients = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetMatrixCoefficients call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetMatrixCoefficients(void) {
    gTestID = 50;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayMatrixCoefficients_t matrixCoefficients;

    // Step 01: Attempt to get matrix coefficients without initialization
    status = dsGetMatrixCoefficients(-1, &matrixCoefficients);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetMatrixCoefficients(handle, &matrixCoefficients);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get matrix coefficients with invalid pointer
        status = dsGetMatrixCoefficients(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get matrix coefficients after termination
    status = dsGetMatrixCoefficients(handle, &matrixCoefficients);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetColorDepth()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 051@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetColorDepth() by looping through the acquired port handles and valid pointer to retrieve the current color depth  | handle  = [loop through valid handles] , color_depth = [valid pointer] | dsERR_NONE | must return a valid color depth value of the specified video port|
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetColorDepth(void) {
    gTestID = 51;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    unsigned int colorDepth1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the color depth
        status = dsGetColorDepth(handle, &colorDepth1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with values from profile file
        UT_ASSERT_EQUAL(colorDepth1, gDSvideoPort_color_depth);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetColorDepth()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 052@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetColorDepth() Attempt to get the color depth with out initializing video ports| handle = [invalid handle] , color_depth = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetColorDepth call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetColorDepth() Using an invalid handle but with valid pointer | handle = [invalid handle], color_depth = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetColorDepth() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], color_depth = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetColorDepth() Again after terminating video ports attempt to get the color depth of video ports | handle= [invalid handle] , color_depth = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetColorDepth call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetColorDepth(void) {
    gTestID = 52;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    unsigned int colorDepth; // To store the color depth

    // Step 01: Attempt to get color depth without initialization
    status = dsGetColorDepth(-1, &colorDepth);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetColorDepth(handle, &colorDepth);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get color depth with invalid pointer
        status = dsGetColorDepth(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get color depth after termination
    status = dsGetColorDepth(handle, &colorDepth);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetColorSpace()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 053@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetColorSpace() by looping through the acquired port handles and valid pointer to retrieve the current color space setting of video port |handle  = [loop through valid handles] , color_space = [valid pointer] |Valid color space value|must return a valid color space setting of the specified video port|
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetColorSpace(void) {
    gTestID = 53;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayColorSpace_t colorSpace1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the color space
        status = dsGetColorSpace(handle, &colorSpace1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        UT_ASSERT_EQUAL(colorSpace1, gDSVideoPortConfiguration[i].colorspaces);

    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Negative Test Scenarios for dsGetColorSpace()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 054@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetColorSpace() Attempt to get the color space with out initializing video ports| handle = [invalid handle] , color_space = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetColorSpace call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetColorSpace() Using an invalid handle but with valid pointer | handle = [invalid handle], color_space = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetColorSpace() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], color_space = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetColorSpace() Again after terminating video ports attempt to get the color space of video ports | handle= [invalid handle] , color_space = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetColorSpace call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetColorSpace(void) {
    gTestID = 54;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayColorSpace_t colorSpace;

    // Step 01: Attempt to get color space without initialization
    status = dsGetColorSpace(-1, &colorSpace);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetColorSpace(handle, &colorSpace);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get color space with invalid pointer
        status = dsGetColorSpace(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get color space after termination
    status = dsGetColorSpace(handle, &colorSpace);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Positive Test Scenarios for dsGetQuantizationRange()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 055@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetQuantizationRange() by looping through the acquired port handles and valid pointer to retrieve the current quantization range  |handle  = [loop through valid handles] , quantization_range = [valid pointer] |dsERR_NONE | must return a valid quantization range of the specified video port|
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetQuantizationRange(void) {
    gTestID = 55;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayQuantizationRange_t quantizationRange1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Retrieve the quantization range
        status = dsGetQuantizationRange(handle, &quantizationRange1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value with value from profile file
        UT_ASSERT_EQUAL(quantizationRange1, gDSVideoPortConfiguration[i].quantization_ranges);
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Negative Test Scenarios for dsGetQuantizationRange()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 056@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetQuantizationRange() Attempt to get the quantization range with out initializing video ports| handle = [invalid handle] , quantization_range = [valid pointer]| dsERR_NOT_INITIALIZED| dsGetQuantizationRange call must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetQuantizationRange() Using an invalid handle but with valid pointer | handle = [invalid handle], quantization_range = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetQuantizationRange() By looping through acquired port handles but with a invalid pointer | handle = [valid handle], quantization_range = [invalid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetQuantizationRange() Again after terminating video ports attempt to get the quantization of video ports | handle= [valid handle from step 04 ] , quantization_range = [valid pointer] | dsERR_NOT_INITIALIZED | dsGetQuantizationRange call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetQuantizationRange(void) {
    gTestID = 56;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayQuantizationRange_t quantizationRange;

    // Step 01: Attempt to get quantization range without initialization
    status = dsGetQuantizationRange(-1, &quantizationRange);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetQuantizationRange(handle, &quantizationRange);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get quantization range with invalid pointer
        status = dsGetQuantizationRange(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get quantization range after termination
    status = dsGetQuantizationRange(handle, &quantizationRange);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetCurrentOutputSettings()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 057@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetCurrentOutputSettings() by looping through the acquired port handles and valid pointer to retrieve the current output settings and store it | handle= [loop through valid handles] , video_eotf = [valid EOTF pointer], matrix_coefficients = = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer]| dsERR_NONE and valid settings| All the output settings for the specified video port must be returned |
 * |04|Call dsGetCurrentOutputSettings() again by looping through the acquired port handles and valid pointer to retrieve the current output settings and store it | handle= [loop through valid handles] , video_eotf = [valid EOTF pointer], matrix_coefficients = = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer]| dsERR_NONE and valid settings| All the output settings for the specified video port must be returned |
 * |05| Check if the values are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetCurrentOutputSettings(void) {
    gTestID = 57;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle[gDSvideoPort_NumberOfPorts];

    // Assume OutputSettings is a struct that includes EOTF, matrix coefficients, color space, color depth, and quantization range

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        dsHDRStandard_t hdrstandard1;
        dsDisplayMatrixCoefficients_t matrixcoef1;
        dsDisplayColorSpace_t colorspace1;
        unsigned int colordepth1;
        dsDisplayQuantizationRange_t quant_range1;

        dsHDRStandard_t hdrstandard2;
        dsDisplayMatrixCoefficients_t matrixcoef2;
        dsDisplayColorSpace_t colorspace2;
        unsigned int colordepth2;
        dsDisplayQuantizationRange_t quant_range2;
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle[i]);
        UT_ASSERT_EQUAL(status, dsERR_NONE);

        // Step 03: Retrieve the current output settings
        status = dsGetCurrentOutputSettings(handle[i], &hdrstandard1 ,&matrixcoef1, &colorspace1,\
                &colordepth1, &quant_range1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Retrieve the current output settings
        status = dsGetCurrentOutputSettings(handle[i], &hdrstandard2 ,&matrixcoef2, &colorspace2,\
                &colordepth2, &quant_range2);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check if the values are equal
        UT_ASSERT_EQUAL(hdrstandard1, hdrstandard2);
        UT_ASSERT_EQUAL(matrixcoef1, matrixcoef2);
        UT_ASSERT_EQUAL(colorspace1, colorspace2);
        UT_ASSERT_EQUAL(colordepth1, colordepth2);
        UT_ASSERT_EQUAL(quant_range1, quant_range2);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetCurrentOutputSettings()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 058@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetCurrentOutputSettings() - Attempt to get the current output settings without initializing the video ports | handle = [invalid handle],  video_eotf = [valid EOTF pointer], matrix_coefficients = = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer] | dsERR_NOT_INITIALIZED | Get Output settings must fail as module is not initialized|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetCurrentOutputSettings() with an invalid handle but with valid pointers for output settings |handle = [valid handle], video_eotf = [valid EOTF pointer], matrix_coefficients = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer] |dsERR_INVALID_PARAM| Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetCurrentOutputSettings() by looping through valid handles but invalid pointer for parameters | handle = [valid handle], video_eotf = [invalid EOTF pointer], matrix_coefficients =[valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer] |dsERR_INVALID_PARAM| Invalid parameter error must be returned |
 * |06|Call dsGetCurrentOutputSettings() by looping through valid handles but invalid pointer for parameters | handle = [valid handle], video_eotf = [valid EOTF pointer], matrix_coefficients = [invalid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer] |dsERR_INVALID_PARAM| Invalid parameter error must be returned |
 * |07|Call dsGetCurrentOutputSettings() by looping through valid handles but invalid pointer for parameters | handle = [valid handle], video_eotf = [valid EOTF pointer], matrix_coefficients = [valid matrix coefficient pointer], color_space = [invalid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer] |dsERR_INVALID_PARAM| Invalid parameter error must be returned |
 * |08|Call dsGetCurrentOutputSettings() by looping through valid handles but invalid pointer for parameters | handle = [valid handle], video_eotf = [valid EOTF pointer], matrix_coefficients = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [invalid color depth pointer], quantization_range = [valid quantization range pointer] |dsERR_INVALID_PARAM| Invalid parameter error must be returned |
 * |09|Call dsGetCurrentOutputSettings() by looping through valid handles but invalid pointer for parameters | handle = [valid handle], video_eotf = [valid EOTF pointer], matrix_coefficients = = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [invalid quantization range pointer] |dsERR_INVALID_PARAM| Invalid parameter error must be returned |
 * |10|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |11|Call dsGetCurrentOutputSettings() - Attempt to get the current output settings without initializing the video ports | handle = [valid handle],  video_eotf = [valid EOTF pointer], matrix_coefficients = = [valid matrix coefficient pointer], color_space = [valid color space pointer], color_depth = [valid color depth pointer], quantization_range = [valid quantization range pointer] | dsERR_NOT_INITIALIZED | Get Output settings must fail as module is not initialized|
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetCurrentOutputSettings(void) {
    gTestID = 58;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status; 
    intptr_t handle = 0;
    dsHDRStandard_t hdrstandard;
    dsDisplayMatrixCoefficients_t matrixcoef;
    dsDisplayColorSpace_t colorspace;
    unsigned int colordepth;
    dsDisplayQuantizationRange_t quant_range;

    // Step 01: Attempt to get output settings without initialization
    status = dsGetCurrentOutputSettings(-1, &hdrstandard, &matrixcoef, &colorspace,\
            &colordepth, &quant_range);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsGetCurrentOutputSettings(handle, &hdrstandard, &matrixcoef, &colorspace,\
            &colordepth, &quant_range);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle*/
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get output settings with invalid pointer
        status = dsGetCurrentOutputSettings(handle, NULL, &matrixcoef, &colorspace,\
                &colordepth, &quant_range);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 06: Get output settings with invalid pointer
        status = dsGetCurrentOutputSettings(handle, &hdrstandard, NULL, &colorspace,\
                &colordepth, &quant_range);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 07: Get output settings with invalid pointer
        status = dsGetCurrentOutputSettings(handle, &hdrstandard, &matrixcoef, NULL,\
                &colordepth, &quant_range);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 08: Get output settings with invalid pointer
        status = dsGetCurrentOutputSettings(handle, &hdrstandard, &matrixcoef, &colorspace,\
                NULL, &quant_range);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
        // Step 09: Get output settings with invalid pointer
        status = dsGetCurrentOutputSettings(handle, &hdrstandard, &matrixcoef, &colorspace,\
                &colordepth, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 10: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 11: Attempt to get output settings after termination
    status = dsGetCurrentOutputSettings(handle, &hdrstandard, &matrixcoef, &colorspace,\
            &colordepth, &quant_range);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsIsOutputHDR()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsIsOutputHDR() - by looping through acquired port handles to check if the video output is HDR or not & store it |  handle = [valid handle], hdr = [valid pointer to hold hdr status value](true = HDR) | dsERR_NONE | Flag which holds the HDR status( @a true if Video Output is HDR and @a false otherwise ) of Video Port must be returned |
 * |04|Call dsIsOutputHDR() - Again by looping through acquired port handles to check if the video output is HDR or not & store it |  handle = [valid handle], hdr = [valid pointer to hold hdr status value](true = HDR) | dsERR_NONE | Flag which holds the HDR status( @a true if Video Output is HDR and @a false otherwise ) of Video Port must be returned |
 * |05|Check if the values are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 * 
 */
void test_l1_dsVideoPort_positive_dsIsOutputHDR(void) {
    gTestID = 59;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool hdr1 = false;
    bool hdr2 = false;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Check HDR status
        status = dsIsOutputHDR(handle, &hdr1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Repeat the HDR status check
        status = dsIsOutputHDR(handle, &hdr2);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check if the values are equal
        UT_ASSERT_EQUAL(hdr1, hdr2);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsIsOutputHDR()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 060@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsIsOutputHDR() - Attempt to check if video output is HDR or not without initializing video ports | handle: [ invalid handle ], hdr: [ pointer to hold HDR status]| dsERR_NOT_INITIALIZED | Call to check if video output is HDR or not must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsIsOutputHDR() using an invalid handle but with a valid pointer to hold the HDR status | handle: [ invalid handle ], hdr: [ pointer to hold HDR status] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |04|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |05|Call dsIsOutputHDR() by looping through acquired port handles but with a null pointer for HDR status | handle: [ loop through valid handles ], hdr: [ NULL ] | dsERR_INVALID_PARAM | Invalid parameter error must be returned |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsIsOutputHDR() - Attempt to check if video output is HDR or not without initializing video ports | handle: [ valid handle ], hdr: [ pointer to hold HDR status] | dsERR_NOT_INITIALIZED | Call to check if video output is HDR or not must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsIsOutputHDR(void) {
    gTestID = 60;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool hdrStatus = false;

    // Step 01: Attempt to check HDR status without initialization
    status = dsIsOutputHDR(-1, &hdrStatus);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Invalid handle check
    status = dsIsOutputHDR(handle, &hdrStatus);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get valid video port handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Check HDR status with null pointer
        status = dsIsOutputHDR(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to check HDR status after termination
    status = dsIsOutputHDR(handle, &hdrStatus);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsResetOutputToSDR()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 061@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|dsResetOutputToSDR() - To reset the video output to SDR | | dsERR_NONE | Video Output must be resetted to SDR or indicate that the operation isn't supported |
 * |03|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 * 
 */
void test_l1_dsVideoPort_positive_dsResetOutputToSDR(void) {
    gTestID = 61;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Reset video output to SDR
    status = dsResetOutputToSDR();
    if (gSourceType == 1) {
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    } else if (gSourceType == 0) {
        UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
    }

    // Step 03: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsResetOutputToSDR()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 062@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsResetOutputToSDR() - Attempt to reset the video output to SDR without initializing the video ports | dsERR_NOT_INITIALIZED | Reset Video output to SDR must fail as video port is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |04|Call dsResetOutputToSDR() - Again attempt to reset the video output to SDR after terminating the video ports | dsERR_NOT_INITIALIZED| Reset Video output to SDR must fail as video port is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsResetOutputToSDR(void) {
    gTestID = 62;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;

    // Step 01: Attempt reset without initialization
    status = dsResetOutputToSDR();
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 04: Attempt reset after termination
    status = dsResetOutputToSDR();
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsSetHdmiPreference()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 063@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned for all supported video ports|
 * |03|Call dsSetHdmiPreference() - by looping through the acquired port handles and set the HDMI preference from dsHdcpProtocolVersion_t | handle = [Valid handle], hdcpCurrentProtocol=[valid HDCP Protocol] | dsERR_NONE | HDMI Preference must be set successfully |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsSetHdmiPreference(void) {
    gTestID = 63;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for all supported video ports
    // Loop through kPorts to get each handle
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Set the HDMI preference
        status = dsSetHdmiPreference(handle, &(gDSVideoPortConfiguration[i].hdcp_protocol_version));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSetHdmiPreference()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 064@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSetHdmiPreference() - Attempt to set the HDMI Preference without initializing the video ports | handle = [invalid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_NOT_INITIALIZED | Set Hdmi Preference must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsSetHdmiPreference() - Using an invalid handle but with valid HDCP Protcol pointer | handle = [invalid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] | dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsSetHdmiPreference() - by looping through the acquired port handles and set an invalid HDMI preference| handle = [Valid handle], hdcpCurrentProtocol=[invalid hdcp protocol] | dsERR_NONE | HDMI Preference must be set successfully |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsSetHdmiPreference() - Attempt to set the HDMI Preference without initializing the video ports | handle = [valid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_NOT_INITIALIZED | Set Hdmi Preference must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSetHdmiPreference(void) {
    gTestID = 64;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHdcpProtocolVersion_t in_range = dsHDCP_VERSION_1X , out_range = dsHDCP_VERSION_MAX; 

    // Step 01: Attempt set HDMI preference without initialization
    status = dsSetHdmiPreference(-1, &in_range);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Set HDMI preference with invalid handle
    status = dsSetHdmiPreference(handle, &in_range);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for all supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Set HDMI preference with valid handle and invalid HDCP protocol
        status = dsSetHdmiPreference(handle, &out_range);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt set HDMI preference after termination
    status = dsSetHdmiPreference(handle, &in_range);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetHdmiPreference()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 065@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetHdmiPreference() - by looping through the acquired port handles to get the HDMI Preference of each video ports which supports HDCP | handle = [valid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_NONE |  The Current HDMI Preference must be returned |
 * |04|Compare the values with values from profile file and make sure they are equal | | dsERR_NONE | The values must be equal |
 * |05|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetHdmiPreference(void) {
    gTestID = 65;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHdcpProtocolVersion_t  hdcpCurrentProtocol1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for all supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Get the HDMI preference
        status = dsGetHdmiPreference(handle, &hdcpCurrentProtocol1);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 04: Compare the value and make sure they are equal
        UT_ASSERT_EQUAL(hdcpCurrentProtocol1, gDSVideoPortConfiguration[i].hdcp_protocol_version);

    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetHdmiPreference()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 066@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetHdmiPreference() - Attempt to get the HDMI Preference without prior initializing the video ports| handle = [invalid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_NOT_INITIALIZED | Get HDMI Preference of video ports must fail as module is not initialized |
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetHdmiPreference() - Using an invalid handle but with valid HDCP Protcol pointer | handle = [invalid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_INVALID_PARAM | Invalid paramerter must be returned |
 * |04|Call dsGetVideoPort() - Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] | dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetHdmiPreference() - by looping through the acquired port handles but with NULL pointer for HDMI preference | handle = [Valid handle], hdcpCurrentProtocol = [NULL] | dsERR_NONE | Invalid Parameter error must be returned|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsSetHdmiPreference() - Attempt to get the HDMI Preference without initializing the video ports | handle = [valid handle], hdcpCurrentProtocol = [valid pointer] | dsERR_NOT_INITIALIZED | Get Hdmi Preference must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetHdmiPreference(void) {
    gTestID = 66;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsHdcpProtocolVersion_t hdcpCurrentProtocol;

    // Step 01: Attempt to get HDMI preference without initialization
    status = dsGetHdmiPreference(-1, &hdcpCurrentProtocol);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Get HDMI preference with invalid handle
    status = dsGetHdmiPreference(handle, &hdcpCurrentProtocol);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for all supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get HDMI preference with valid handle but with NULL pointer for HDMI Preference
        status = dsGetHdmiPreference(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get HDMI preference after termination
    status = dsGetHdmiPreference(handle, &hdcpCurrentProtocol);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsGetIgnoreEDIDStatus()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 067@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetIgnoreEDIDStatus() by looping through acquired handles and valid pointer to get IgnoreEDID status & store it |handle=[valid handle] ,status=[valid pointer] |dsERR_NONE | must successfully get the IgnoreEDID status or indicate that the operation isn't supported|
 * |04|Call dsGetIgnoreEDIDStatus() - Again by looping through acquired handles and valid pointer to get IgnoreEDID status & store it |handle=[valid handle] ,status=[valid pointer] |dsERR_NONE | must successfully get the IgnoreEDID status|
 * |05|Check if the values are equal | | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetIgnoreEDIDStatus(void) {
    gTestID = 67;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool ignoreEDIDStatus1 = false;
    bool ignoreEDIDStatus2 = false;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Get IgnoreEDID status
        status = dsGetIgnoreEDIDStatus(handle, &ignoreEDIDStatus1);
        if (gSourceType == 1) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
            // Step 04: Repeat getting IgnoreEDID status
            status = dsGetIgnoreEDIDStatus(handle, &ignoreEDIDStatus2);
            UT_ASSERT_EQUAL(status, dsERR_NONE);
            // Step 05: Check if the values are equal
            UT_ASSERT_EQUAL(ignoreEDIDStatus1, ignoreEDIDStatus2);
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetIgnoreEDIDStatus()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 068@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetIgnoreEDIDStatus() without prior initialization of video ports| handle=[invalid handle] ,status=[invalid pointer] |dsERR_NOT_INITIALIZED|call must be fail as module is not initialzed|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetIgnoreEDIDStatus() with an invalid handle but with valid pointer|handle=[invalid handle] ,status=[valid pointer]|dsERR_INVALID_PARAM|Invalid parameter must be returned|
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type , index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetIgnoreEDIDStatus() by looping through valid handles with a NULL status pointer | handle=[valid handle] , status=[NULL pointer]|dsERR_INVALID_PARAM|Invalid parameter must be returned|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetIgnoreEDIDStatus()  Again after terminating video ports | handle=[valid handle from step 04 ] , status=[valid pointer] | dsERR_NOT_INITIALIZED | call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetIgnoreEDIDStatus(void) {
    gTestID = 68;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    bool ignoreEDIDStatus = false;

    // Step 01: Attempt to get IgnoreEDID status without initialization
    status = dsGetIgnoreEDIDStatus(-1, &ignoreEDIDStatus);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Get IgnoreEDID status with invalid handle
    status = dsGetIgnoreEDIDStatus(handle, &ignoreEDIDStatus);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get IgnoreEDID status with valid handle but null pointer
        status = dsGetIgnoreEDIDStatus(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get IgnoreEDID status after termination
    status = dsGetIgnoreEDIDStatus(handle, &ignoreEDIDStatus);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsSetBackgroundColor()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 069@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsSetBackgroundColor by looping through the acquired prots and valid color values| handle=[Valid handle], color=[valid values] | dsERR_NONE|Background color must be set successfully or indicate that the operation isn't supported |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsSetBackgroundColor(void) {
    gTestID = 69;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &handle);
        UT_ASSERT_EQUAL(status, dsERR_NONE);
            // Step 03: Set background color for each port by looping through dsVideoBackgroundColor_t enum
        for(dsVideoBackgroundColor_t color = dsVIDEO_BGCOLOR_BLUE; color < dsVIDEO_BGCOLOR_MAX ; color++){
            status = dsSetBackgroundColor(handle, color);
            if (gSourceType == 1) {
                UT_ASSERT_EQUAL(status, dsERR_NONE);
            } else if (gSourceType == 0) {
                UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
            }
        } 
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSetBackgroundColor()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 070@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSetBackgroundColor() without prior initialization of video ports| handle=[inValid handle], color=[valid values] |dsERR_NOT_INITIALIZED|call must fail as module not initialized|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsSetBackgroundColor()  with an invalid handle and valid color values |handle=[inValid handle], color=[valid values]|dsERR_INVALID_PARAM|Invalid parameter must return |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsSetBackgroundColor() by looping through acquired handles and invalid color value|handle=[valid handle], color=[Invalid color value|dsERR_INVALID_PARAM|Invalid parameter must return|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsSetBackgroundColor() after termination video ports |handle= [valid handle from step 04 ] , color= [valid values] | dsERR_NOT_INITIALIZED | call must fail as module is not initialized |
 *
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSetBackgroundColor(void) {
    gTestID = 70;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Attempt to set background color without initialization
    status = dsSetBackgroundColor(-1, dsVIDEO_BGCOLOR_BLUE);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Set background color with invalid handle
    status = dsSetBackgroundColor(handle, dsVIDEO_BGCOLOR_BLUE);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Set background color with valid handle but invalid color
        status = dsSetBackgroundColor(handle, dsVIDEO_BGCOLOR_MAX);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to set background color after termination
    status = dsSetBackgroundColor(handle, dsVIDEO_BGCOLOR_BLACK);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsSetForceHDRMode()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 071@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsSetForceHDRMode() by looping through acquired handles and valid mode values|handle=[valid handle], mode=[valid values]|dsERR_NONE|HDR mode must be set successfully or indicate that the operation isn't supported|
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsSetForceHDRMode(void) {
    gTestID = 71;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Set HDR mode
        status = dsSetForceHDRMode(handle, gDSVideoPortConfiguration[i].hdr_capabilities);
        if (gSourceType == 1) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 04: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSetForceHDRMode()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 072@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSetForceHDRMode() without prior initialization of video ports| handle=[invalid handle], mode=[valid values] |dsERR_NOT_INITIALIZED|call must fail as module not initialized|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsSetForceHDRMode() with an invalid handle and valid mode values|handle=[inValid handle], mode=[valid values]|dsERR_INVALID_PARAM|Invalid parameter must return|
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsSetForceHDRMode() by looping through acquired handles and valid mode values|handle=[Valid handle], mode=[invalid values]|dsERR_INVALID_PARAM|Invalid parameter must return|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsSetForceHDRMode() after termination video ports |handle=[valid handle from step 04 ] , mode=[valid values] | dsERR_NOT_INITIALIZED | call must fail as module is not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSetForceHDRMode(void) {
    gTestID = 72;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;
    dsHDRStandard_t mode = gDSVideoPortConfiguration[0].hdr_capabilities;

    // Step 01: Attempt to set HDR mode without initialization
    status = dsSetForceHDRMode(-1, mode );
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Set HDR mode with invalid handle
    status = dsSetForceHDRMode(handle,mode );
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Set HDR mode with invalid mode values
        status = dsSetForceHDRMode(handle, dsHDRSTANDARD_Invalid );
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to set HDR mode after termination
    status = dsSetForceHDRMode(handle, mode);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Positive Test Scenarios for dsColorDepthCapabilities()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 073@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsColorDepthCapabilities() by looping through the acquired ports and valid pointer to get the color depth capabilities |handle=[vallid handle], colorDepthCapability=[valid pointer] |dsERR_NONE  Color depth capabilities must be retrieved successfully or indicate that the operation isn't supported |
 * |05|Compare the values with the value read from profile file | dsERR_NONE | The values must be equal |
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsColorDepthCapabilities(void) {
    gTestID = 73;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle;

    unsigned int colorDepthCapability1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Get color depth capabilities
        status = dsColorDepthCapabilities(handle, &colorDepthCapability1);
        if (gSourceType == 1) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
            // Step 04: Compare value with value from profile file
            UT_ASSERT_EQUAL(colorDepthCapability1, gDSvideoPort_color_depth);
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsColorDepthCapabilities()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 074@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsColorDepthCapabilities() without prior initialization of video ports|handle=[invalid handle], colorDepthCapability=[valid pointer]|dsERR_NOT_INITIALIZED|call must fail as module not initialized|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsColorDepthCapabilities() with an invalid handle and valid pointer |handle=[valid handle], colorDepthCapability=[valid pointer] |dsERR_INVALID_PARAM|Invalid parameter must be returned |
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsColorDepthCapabilities() by looping through acquired handles and invalid pointer | handle=[valid handle], colorDepthCapability=[Invalid pointer]|dsERR_INVALID_PARAM|Invalid parameter must return|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsColorDepthCapabilities() after terminating videop ports |handle= [valid handle from step 04 ] , colorDepthCapability= [valid pointer] | dsERR_NOT_INITIALIZED|call must fail as module not initialized |
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsColorDepthCapabilities(void) {
    gTestID = 74;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    unsigned int  colorDepthCapability;

    // Step 01: Attempt to get capabilities without initialization
    status = dsColorDepthCapabilities(-1, &colorDepthCapability);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Get capabilities with invalid handle
    status = dsColorDepthCapabilities(handle, &colorDepthCapability);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get capabilities with invalid pointer
        status = dsColorDepthCapabilities(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get capabilities after termination
    status = dsColorDepthCapabilities(handle, &colorDepthCapability);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Positive Test Scenarios for dsGetPreferredColorDepth()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 075@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsGetPreferredColorDepth by looping through acquired handles and valid pointer to get the preferred color depth of each port |handle=[valid handle] , colorDepth=[valid pointer]|dsERR_NONE|Preferred Color depth must be retrieved successfully or indicate that the operation isn't supported|
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsGetPreferredColorDepth(void) {
    gTestID = 75;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle;

    dsDisplayColorDepth_t colorDepth1;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for all supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
    }

    // Step 03: Get preferred color depth
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetPreferredColorDepth(handle, &colorDepth1);
        if (gSourceType == 1) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
            UT_ASSERT_EQUAL(colorDepth1, gDSvideoPort_color_depth);
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsGetPreferredColorDepth()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 076@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetPreferredColorDepth() without prior initialization of video ports|handle=[invalid handle] , colorDepth=[valid pointer]|dsERR_NOT_INITIALIZED|call must fail as module is not initialized|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsGetPreferredColorDepth() with an invalid handle and with valid pointer|handle=[invalid handle] , colorDepth=[valid pointer]|dsERR_INVALID_PARAM|Invalid parameter must be returned|
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsGetPreferredColorDepth() by looping through valid handes and with invalid pointer|handle=[valid handle] , colorDepth=[invalid pointer]|dsERR_INVALID_PARAM|Invalid parameter must be returned|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsGetPreferredColorDepth() after terminating video ports |handle=[valid handle] , colorDepth=[valid pointer] |dsERR_NOT_INITIALIZED|call must fail as module is not initialized|
 *
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsGetPreferredColorDepth(void) {
    gTestID = 76;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    dsDisplayColorDepth_t colorDepth;

    // Step 01: Attempt to get color depth without initialization
    status = dsGetPreferredColorDepth(-1, &colorDepth);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Get color depth with invalid handle
    status = dsGetPreferredColorDepth(handle, &colorDepth);
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Get color depth with invalid pointer
        status = dsGetPreferredColorDepth(handle, NULL);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to get color depth after termination
    status = dsGetPreferredColorDepth(handle, &colorDepth);
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}

/**
 * @brief Positive Test Scenarios for dsSetPreferredColorDepth()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 077@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |02|Call dsGetVideoPort() - Get the video port handle for valid video port type and valid index | type, index = [Loop through kPorts] , handle = [valid handle] | dsERR_NONE | Valid port handle must be returned |
 * |03|Call dsSetPreferredColorDepth() by looping through the acquired handles and Valid colorDepth values|handle=[invalid handle] , colorDepth=[valid values] |dsERR_NONE|Color depth must be set successfully or indicate that the operation isn't supported |
 * |04|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * 
 */
void test_l1_dsVideoPort_positive_dsSetPreferredColorDepth(void) {
    gTestID = 77;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 02: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 03: Set preferred color depth
        status = dsSetPreferredColorDepth(handle, gDSvideoPort_color_depth);
        if (gSourceType == 1) {
            UT_ASSERT_EQUAL(status, dsERR_NONE);
        } else if (gSourceType == 0) {
            UT_ASSERT_EQUAL(status, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


/**
 * @brief Negative Test Scenarios for dsSetPreferredColorDepth()
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 078@n
 * 
 * **Pre-Conditions:** None@n
 * 
 * **Dependencies:** None@n
 * 
 * **User Interaction:** None
 * 
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsSetPreferredColorDepth() without prior initialization of video ports|handle=[invalid handle] , colorDepth=[valid values]|dsERR_NOT_INITIALIZED|call must fail as module not initialized|
 * |02|Call dsVideoPortInit() - Initialize video port system | | dsERR_NONE | Initialization must be successful |
 * |03|Call dsSetPreferredColorDepth() with an invalid handle with valid color depth values|handle=[Invalid handle],colorDepth=[valid values]|dsERR_INVALID_PARAM|Invalid parameter must be return|
 * |04|Call dsGetVideoPort() Get the port handle for all supported video ports on the platform  |type ,  index = [ Loop through kPorts ] |dsERR_NONE | Valid port handle must be returned for all supported video ports |
 * |05|Call dsSetPreferredColorDepth() by looping through valid handles  with an unsupported colorDepth value|handle=[valid handle],colorDepth=[invalid values]|dsERR_INVALID_PARAM|Invalid parameter must be return|
 * |06|Call dsVideoPortTerm() - Terminate the video port system | | dsERR_NONE | Termination must be successful |
 * |07|Call dsSetPreferredColorDepth() after terminating video ports|handle=[valid handle] , colorDepth=[valid values]|dsERR_NOT_INITIALIZED|call must fail as module not initialized|
 * 
 * 
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` and `dsERR_GENERAL` might be challenging since it requires a specific scenarios.
 */
void test_l1_dsVideoPort_negative_dsSetPreferredColorDepth(void) {
    gTestID = 78;
    UT_LOG_INFO(" In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status;
    intptr_t handle = 0;

    // Step 01: Attempt to set color depth without initialization
    status = dsSetPreferredColorDepth(-1, dsDISPLAY_COLORDEPTH_8BIT );
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video port system
    status = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 03: Set color depth with invalid handle
    status = dsSetPreferredColorDepth(handle, dsDISPLAY_COLORDEPTH_8BIT );
    UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);

    // Step 04: Get the port handle for supported video ports
    for (int i = 0; i < gDSvideoPort_NumberOfPorts; i++) {
        status = dsGetVideoPort(gDSVideoPortConfiguration[i].typeid, gDSVideoPortConfiguration[i].index, &(handle));
        UT_ASSERT_EQUAL(status, dsERR_NONE);
        // Step 05: Set unsupported color depth
        status = dsSetPreferredColorDepth(handle, 0x60);
        UT_ASSERT_EQUAL(status, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the video port system
    status = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(status, dsERR_NONE);

    // Step 07: Attempt to set color depth after termination
    status = dsSetPreferredColorDepth(handle, dsDISPLAY_COLORDEPTH_12BIT );
    CHECK_FOR_EXTENDED_ERROR_CODE(status, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO(" Out %s", __FUNCTION__);
}


static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_dsVideoPort_register ( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 dsVideoPort]", NULL, NULL );
    if ( NULL == pSuite )
    {
        return -1;
    }	

    UT_add_test( pSuite, "dsVideoPortInit_L1_positive" ,test_l1_dsVideoPort_positive_dsVideoPortInit );
    UT_add_test( pSuite, "dsVideoPortInit_L1_negative" ,test_l1_dsVideoPort_negative_dsVideoPortInit );
    UT_add_test( pSuite, "dsVideoPortTerm_L1_positive" ,test_l1_dsVideoPort_positive_dsVideoPortTerm );
    UT_add_test( pSuite, "dsVideoPortTerm_L1_negative" ,test_l1_dsVideoPort_negative_dsVideoPortTerm );
    UT_add_test( pSuite, "dsGetVideoPort_L1_positive" ,test_l1_dsVideoPort_positive_dsGetVideoPort );
    UT_add_test( pSuite, "dsGetVideoPort_L1_negative" ,test_l1_dsVideoPort_negative_dsGetVideoPort );
    UT_add_test( pSuite, "dsIsVideoPortEnabled_L1_positive" ,test_l1_dsVideoPort_positive_dsIsVideoPortEnabled );
    UT_add_test( pSuite, "dsIsVideoPortEnabled_L1_negative" ,test_l1_dsVideoPort_negative_dsIsVideoPortEnabled );
    UT_add_test( pSuite, "dsIsDisplayConnected_L1_positive" ,test_l1_dsVideoPort_positive_dsIsDisplayConnected );
    UT_add_test( pSuite, "dsIsDisplayConnected_L1_negative" ,test_l1_dsVideoPort_negative_dsIsDisplayConnected );
    UT_add_test( pSuite, "dsIsDisplaySurround_L1_positive" ,test_l1_dsVideoPort_positive_dsIsDisplaySurround );
    UT_add_test( pSuite, "dsIsDisplaySurround_L1_negative" ,test_l1_dsVideoPort_negative_dsIsDisplaySurround );
    UT_add_test( pSuite, "dsGetSurroundMode_L1_positive" ,test_l1_dsVideoPort_positive_dsGetSurroundMode );
    UT_add_test( pSuite, "dsGetSurroundMode_L1_negative" ,test_l1_dsVideoPort_negative_dsGetSurroundMode );
    UT_add_test( pSuite, "dsVideoFormatUpdateRegisterCB_L1_positive" ,test_l1_dsVideoPort_positive_dsVideoFormatUpdateRegisterCB );
    UT_add_test( pSuite, "dsVideoFormatUpdateRegisterCB_L1_negative" ,test_l1_dsVideoPort_negative_dsVideoFormatUpdateRegisterCB );
    UT_add_test( pSuite, "dsIsVideoPortActive_L1_positive" ,test_l1_dsVideoPort_positive_dsIsVideoPortActive );
    UT_add_test( pSuite, "dsIsVideoPortActive_L1_negative" ,test_l1_dsVideoPort_negative_dsIsVideoPortActive );
    UT_add_test( pSuite, "dsEnableHDCP_L1_positive" ,test_l1_dsVideoPort_positive_dsEnableHDCP );
    UT_add_test( pSuite, "dsEnableHDCP_L1_negative" ,test_l1_dsVideoPort_negative_dsEnableHDCP );
    UT_add_test( pSuite, "dsIsHDCPEnabled_L1_positive" ,test_l1_dsVideoPort_positive_dsIsHDCPEnabled );
    UT_add_test( pSuite, "dsIsHDCPEnabled_L1_negative" ,test_l1_dsVideoPort_negative_dsIsHDCPEnabled );
    UT_add_test( pSuite, "dsEnableVideoPort_L1_positive" ,test_l1_dsVideoPort_positive_dsEnableVideoPort );
    UT_add_test( pSuite, "dsEnableVideoPort_L1_negative" ,test_l1_dsVideoPort_negative_dsEnableVideoPort );
    UT_add_test( pSuite, "dsSetResolution_L1_positive" ,test_l1_dsVideoPort_positive_dsSetResolution );
    UT_add_test( pSuite, "dsSetResolution_L1_negative" ,test_l1_dsVideoPort_negative_dsSetResolution );
    UT_add_test( pSuite, "dsGetResolution_L1_positive" ,test_l1_dsVideoPort_positive_dsGetResolution );
    UT_add_test( pSuite, "dsGetResolution_L1_negative" ,test_l1_dsVideoPort_negative_dsGetResolution );
    UT_add_test( pSuite, "dsRegisterHdcpStatusCallback_L1_positive" ,test_l1_dsVideoPort_positive_dsRegisterHdcpStatusCallback );
    UT_add_test( pSuite, "dsRegisterHdcpStatusCallback_L1_negative" ,test_l1_dsVideoPort_negative_dsRegisterHdcpStatusCallback );
    UT_add_test( pSuite, "dsGetHDCPStatus_L1_positive" ,test_l1_dsVideoPort_positive_dsGetHDCPStatus );
    UT_add_test( pSuite, "dsGetHDCPStatus_L1_negative" ,test_l1_dsVideoPort_negative_dsGetHDCPStatus );
    UT_add_test( pSuite, "dsGetHDCPProtocol_L1_positive" ,test_l1_dsVideoPort_positive_dsGetHDCPProtocol );
    UT_add_test( pSuite, "dsGetHDCPProtocol_L1_negative" ,test_l1_dsVideoPort_negative_dsGetHDCPProtocol );
    UT_add_test( pSuite, "dsGetHDCPReceiverProtocol_L1_positive" ,test_l1_dsVideoPort_positive_dsGetHDCPReceiverProtocol );
    UT_add_test( pSuite, "dsGetHDCPReceiverProtocol_L1_negative" ,test_l1_dsVideoPort_negative_dsGetHDCPReceiverProtocol );
    UT_add_test( pSuite, "dsGetHDCPCurrentProtocol_L1_positive" ,test_l1_dsVideoPort_positive_dsGetHDCPCurrentProtocol );
    UT_add_test( pSuite, "dsGetHDCPCurrentProtocol_L1_negative" ,test_l1_dsVideoPort_negative_dsGetHDCPCurrentProtocol );
    UT_add_test( pSuite, "dsGetTVHDRCapabilities_L1_positive" ,test_l1_dsVideoPort_positive_dsGetTVHDRCapabilities );
    UT_add_test( pSuite, "dsGetTVHDRCapabilities_L1_negative" ,test_l1_dsVideoPort_negative_dsGetTVHDRCapabilities );
    UT_add_test( pSuite, "dsSupportedTvResolutions_L1_positive" ,test_l1_dsVideoPort_positive_dsSupportedTvResolutions );
    UT_add_test( pSuite, "dsSupportedTvResolutions_L1_negative" ,test_l1_dsVideoPort_negative_dsSupportedTvResolutions );
    UT_add_test( pSuite, "dsSetForceDisable4KSupport_L1_positive" ,test_l1_dsVideoPort_positive_dsSetForceDisable4KSupport );
    UT_add_test( pSuite, "dsSetForceDisable4KSupport_L1_negative" ,test_l1_dsVideoPort_negative_dsSetForceDisable4KSupport );
    UT_add_test( pSuite, "dsGetForceDisable4KSupport_L1_positive" ,test_l1_dsVideoPort_positive_dsGetForceDisable4KSupport );
    UT_add_test( pSuite, "dsGetForceDisable4KSupport_L1_negative" ,test_l1_dsVideoPort_negative_dsGetForceDisable4KSupport );
    UT_add_test( pSuite, "dsGetVideoEOTF_L1_positive" ,test_l1_dsVideoPort_positive_dsGetVideoEOTF );
    UT_add_test( pSuite, "dsGetVideoEOTF_L1_negative" ,test_l1_dsVideoPort_negative_dsGetVideoEOTF );
    UT_add_test( pSuite, "dsGetMatrixCoefficients_L1_positive" ,test_l1_dsVideoPort_positive_dsGetMatrixCoefficients );
    UT_add_test( pSuite, "dsGetMatrixCoefficients_L1_negative" ,test_l1_dsVideoPort_negative_dsGetMatrixCoefficients );
    UT_add_test( pSuite, "dsGetColorDepth_L1_positive" ,test_l1_dsVideoPort_positive_dsGetColorDepth );
    UT_add_test( pSuite, "dsGetColorDepth_L1_negative" ,test_l1_dsVideoPort_negative_dsGetColorDepth );
    UT_add_test( pSuite, "dsGetColorSpace_L1_positive" ,test_l1_dsVideoPort_positive_dsGetColorSpace );
    UT_add_test( pSuite, "dsGetColorSpace_L1_negative" ,test_l1_dsVideoPort_negative_dsGetColorSpace );
    UT_add_test( pSuite, "dsGetQuantizationRange_L1_positive" ,test_l1_dsVideoPort_positive_dsGetQuantizationRange );
    UT_add_test( pSuite, "dsGetQuantizationRange_L1_negative" ,test_l1_dsVideoPort_negative_dsGetQuantizationRange );
    UT_add_test( pSuite, "dsGetCurrentOutputSettings_L1_positive" ,test_l1_dsVideoPort_positive_dsGetCurrentOutputSettings );
    UT_add_test( pSuite, "dsGetCurrentOutputSettings_L1_negative" ,test_l1_dsVideoPort_negative_dsGetCurrentOutputSettings );
    UT_add_test( pSuite, "dsIsOutputHDR_L1_positive" ,test_l1_dsVideoPort_positive_dsIsOutputHDR );
    UT_add_test( pSuite, "dsIsOutputHDR_L1_negative" ,test_l1_dsVideoPort_negative_dsIsOutputHDR );
    UT_add_test( pSuite, "dsResetOutputToSDR_L1_positive" ,test_l1_dsVideoPort_positive_dsResetOutputToSDR );
    UT_add_test( pSuite, "dsResetOutputToSDR_L1_negative" ,test_l1_dsVideoPort_negative_dsResetOutputToSDR );
    UT_add_test( pSuite, "dsSetHdmiPreference_L1_positive" ,test_l1_dsVideoPort_positive_dsSetHdmiPreference );
    UT_add_test( pSuite, "dsSetHdmiPreference_L1_negative" ,test_l1_dsVideoPort_negative_dsSetHdmiPreference );
    UT_add_test( pSuite, "dsGetHdmiPreference_L1_positive" ,test_l1_dsVideoPort_positive_dsGetHdmiPreference );
    UT_add_test( pSuite, "dsGetHdmiPreference_L1_negative" ,test_l1_dsVideoPort_negative_dsGetHdmiPreference );
    UT_add_test( pSuite, "dsGetIgnoreEDIDStatus_L1_positive" ,test_l1_dsVideoPort_positive_dsGetIgnoreEDIDStatus );
    UT_add_test( pSuite, "dsGetIgnoreEDIDStatus_L1_negative" ,test_l1_dsVideoPort_negative_dsGetIgnoreEDIDStatus );
    UT_add_test( pSuite, "dsSetBackgroundColor_L1_positive" ,test_l1_dsVideoPort_positive_dsSetBackgroundColor );
    UT_add_test( pSuite, "dsSetBackgroundColor_L1_negative" ,test_l1_dsVideoPort_negative_dsSetBackgroundColor );
    UT_add_test( pSuite, "dsSetForceHDRMode_L1_positive" ,test_l1_dsVideoPort_positive_dsSetForceHDRMode );
    UT_add_test( pSuite, "dsSetForceHDRMode_L1_negative" ,test_l1_dsVideoPort_negative_dsSetForceHDRMode );
    UT_add_test( pSuite, "dsColorDepthCapabilities_L1_positive" ,test_l1_dsVideoPort_positive_dsColorDepthCapabilities );
    UT_add_test( pSuite, "dsColorDepthCapabilities_L1_negative" ,test_l1_dsVideoPort_negative_dsColorDepthCapabilities );
    UT_add_test( pSuite, "dsGetPreferredColorDepth_L1_positive" ,test_l1_dsVideoPort_positive_dsGetPreferredColorDepth );
    UT_add_test( pSuite, "dsGetPreferredColorDepth_L1_negative" ,test_l1_dsVideoPort_negative_dsGetPreferredColorDepth );
    UT_add_test( pSuite, "dsSetPreferredColorDepth_L1_positive" ,test_l1_dsVideoPort_positive_dsSetPreferredColorDepth );
    UT_add_test( pSuite, "dsSetPreferredColorDepth_L1_negative" ,test_l1_dsVideoPort_negative_dsSetPreferredColorDepth );

   extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "dsVideoPort/features/extendedEnumsSupported" );

    return 0;
} 

/** @} */ // End of DS_VideoPort_HALTEST_L1
/** @} */ // End of DS_VideoPort_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
