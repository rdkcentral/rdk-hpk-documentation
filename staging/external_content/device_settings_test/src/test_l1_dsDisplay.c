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
 * @defgroup DS_Display_HALTEST Device Settings Display HAL Tests
 * @{
 */

/**
 * @defgroup DS_Display_HALTEST_L1 Device Settings Display HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for DS Display HAL :
 *
 * Level 1 unit test cases for all APIs of Device Settings Display HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-display_halSpec.md](../../docs/pages/ds-display_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l1_dsDisplay.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "stdint.h"
#include "dsDisplay.h"
#include "test_parse_configuration.h"

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
 * @brief Ensure dsDisplayInit() initializes the DS Display sub-system correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsDisplayInit() to initialize the module| | dsERR_NONE | Initialization should succeed |
 * |02|Terminate the DS Display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |03|Reinitialize the display sub-system with dsDisplayInit() to check for reinitialization capability| | dsERR_NONE | Initialization should succeed |
 * |04|Terminate the DS Display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsDisplay_positive_dsDisplayInit(void) {
    // Start of the test
    gTestID = 1;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;

    // Step 01: Initialize the display module
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Terminate the display module
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Reinitialize the display module to check for reinitialization capability
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 04: Terminate the display module again
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsDisplayInit() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the module with dsDisplayInit() | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsDisplayInit() again to check if it returns an error when the module is already initialized | | dsERR_ALREADY_INITIALIZED | Should return error indicating the module is already initialized |
 * |03|Terminate the module with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 *
 * @note Scenarios like dsERR_GENERAL are not included in this test plan due to the challenges in realistic simulation. Each test case is functionally independent with proper initialization and termination steps.
 *
 */
void test_l1_dsDisplay_negative_dsDisplayInit(void) {
    // Start of the test
    gTestID = 2;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;

    // Step 01: Initialize the display module
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Attempt to initialize the display module again
    result = dsDisplayInit();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_ALREADY_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 03: Terminate the display module
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsDisplayTerm() terminates the display sub-system correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the display sub-system with dsDisplayInit() | | dsERR_NONE | Initialization should succeed |
 * |02|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |03|Reinitialize the display sub-system with dsDisplayInit() to check for reinitialization capability | | dsERR_NONE | Reinitialization should succeed |
 * |04|Terminate the display sub-system again with dsDisplayTerm() to confirm termination capability | | dsERR_NONE | Termination should succeed again |
 *
 */
void test_l1_dsDisplay_positive_dsDisplayTerm(void) {
    // Start of the test
    gTestID = 3;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;

    // Step 01: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Reinitialize the display sub-system to check for reinitialization
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 04: Terminate the display sub-system again to confirm termination capability
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsDisplayTerm() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsDisplayTerm() without initializing the display sub-system | | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the display sub-system with dsDisplayInit() | | dsERR_NONE | Initialization should succeed |
 * |03|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |04|Call dsDisplayTerm() again after termination | | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note The ability to test scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration.
 *
 */
void test_l1_dsDisplay_negative_dsDisplayTerm(void) {
    // Start of the test
    gTestID = 4;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;

    // Step 01: Call dsDisplayTerm() without initializing the display sub-system
    result = dsDisplayTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 02: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 04: Call dsDisplayTerm() again after termination
    result = dsDisplayTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsGetDisplay() retrieves the handle of the connected display device correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the display sub-system with dsDisplayInit() | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsGetDisplay() Loop through all valid ports |vType: [Valid Port Type], int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |03|Call a the last value again, and compare the results |  |Success | The values should be the same |
 * |04|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsDisplay_positive_dsGetDisplay(void) {
    // Start of the test
    gTestID = 5;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    dsVideoPortType_t vType;
    intptr_t displayHandle1, displayHandle2;
    uint32_t portIndex;

    // Step 01: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Port Type: [%d]\n", __FUNCTION__, vType);

        snprintf(key_str, sizeof(key_str), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Video Port Index: [%d]\n", __FUNCTION__, portIndex);

        // Step 02: Call dsGetDisplay() for each valid port with the index
        result = dsGetDisplay(vType, portIndex, &displayHandle1);
        UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
        UT_LOG("Display handle for port type %d with index %d: %ld\n", vType, portIndex, (long)displayHandle1);

        // Step 03: Call the last value again, and compare the results
        result = dsGetDisplay(vType, portIndex, &displayHandle2);
        UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, displayHandle1, displayHandle2);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
        UT_LOG("\n In %s Comparison: [%d = %d]\n", __FUNCTION__, result);
        UT_ASSERT_EQUAL(displayHandle1, displayHandle2);
        UT_LOG("Repeated display handle for port type %d with index %d: %ld\n", vType, portIndex, (long)displayHandle2);
    }

    // Step 04: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}


/**
 * @brief Ensure dsGetDisplay() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetDisplay() without initializing the display sub-system | vType: [Valid Port Type], int=1, intptr_t* | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the display sub-system with dsDisplayInit() | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsGetDisplay() with invalid parameter video type | vType: dsVIDEOPORT_TYPE_MAX, int=1, intptr_t* | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |04|Call dsGetDisplay() with invalid index | vType: [Valid Port Type], int=-1, intptr_t* | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |05|Call dsGetDisplay() with NULL | vType:dsVIDEOPORT_TYPE_HDMI, int=1, NULL | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |06|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |07|Call dsGetDisplay() without initializing the display sub-system |vType: [Valid Port Type], int=1, intptr_t* | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note The ability to test scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration.
 *
 */
void test_l1_dsDisplay_negative_dsGetDisplay(void) {
    // Start of the test
    gTestID = 6;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    intptr_t displayHandle;
    dsVideoPortType_t vType;

    // Step 01: Call dsGetDisplay() without initializing the display sub-system
    vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32("dsDisplay/Video_Ports/0");
    result = dsGetDisplay(vType, 0, &displayHandle);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 02: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Call dsGetDisplay() with invalid video type
    result = dsGetDisplay(dsVIDEOPORT_TYPE_MAX, (int)dsVIDEOPORT_TYPE_MAX, &displayHandle);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL( result, dsERR_INVALID_PARAM);

    // Step 04: Call dsGetDisplay() with invalid index
    result = dsGetDisplay(vType, -1, &displayHandle);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

    // Step 05: Call dsGetDisplay() with NULL handle
    result = dsGetDisplay(vType, 0, NULL);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

    // Step 06: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetDisplay() again after termination
    result = dsGetDisplay(vType, 0, &displayHandle);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsGetEDID() retrieves the EDID information correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the display sub-system and get a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |02|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |03|Call dsGetEDID() with the obtained display device handle | intptr_t handle, dsDisplayEDID_t *edid | dsERR_NONE and valid EDID info | EDID information should be retrieved successfully |
 * |04|Call dsGetEDID() with the obtained display device handle | intptr_t handle, dsDisplayEDID_t *edid | dsERR_NONE and valid EDID info | EDID information should be retrieved successfully |
 * |05|Compare the returned results |  | Success | The values should be the same |
 * |06|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsDisplay_positive_dsGetEDID(void) {
    // Start of the test
    gTestID = 7;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    intptr_t displayHandle;
    dsVideoPortType_t vType;
    dsDisplayEDID_t edid1;
    dsDisplayEDID_t edid2;
    uint32_t portIndex;

    // Step 01: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Loop through all valid ports
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Port Type: [%d]\n", __FUNCTION__, vType);

        snprintf(key_str, sizeof(key_str), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Video Port Index: [%d]\n", __FUNCTION__, portIndex);

        // Step 02: Get the display device handle using portIndex
        result = dsGetDisplay(vType, portIndex, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Call dsGetEDID() with the obtained handle
        result = dsGetEDID(displayHandle, &edid1);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetEDID() again with the same handle
        result = dsGetEDID(displayHandle, &edid2);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 05: Compare the returned results
        if(gSourceType == 0){
            UT_ASSERT_EQUAL(edid1.productCode , edid2.productCode);
            UT_ASSERT_EQUAL(edid1.serialNumber , edid2.serialNumber);
            UT_ASSERT_EQUAL(edid1.manufactureYear , edid2.manufactureYear);
            UT_ASSERT_EQUAL(edid1.manufactureWeek , edid2.manufactureWeek);
            UT_ASSERT_EQUAL(edid1.hdmiDeviceType , edid2.hdmiDeviceType);
            UT_ASSERT_EQUAL(edid1.isRepeater , edid2.isRepeater);
            UT_ASSERT_EQUAL(edid1.physicalAddressA , edid2.physicalAddressA);
            UT_ASSERT_EQUAL(edid1.physicalAddressB , edid2.physicalAddressB);
            UT_ASSERT_EQUAL(edid1.physicalAddressC , edid2.physicalAddressC);
            UT_ASSERT_EQUAL(edid1.physicalAddressD , edid2.physicalAddressD);
            UT_ASSERT_EQUAL(edid1.numOfSupportedResolution , edid2.numOfSupportedResolution);
            UT_ASSERT_STRING_EQUAL(edid2.monitorName, edid2.monitorName);
        } else if(gSourceType == 1){
            UT_ASSERT_EQUAL((memcmp(&edid1, &edid2, sizeof(dsDisplayEDID_t))), 0);
        }
    }

    // Step 06: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}


/**
 * @brief Ensure dsGetEDID() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetEDID() without initializing the display sub-system or obtaining a handle | intptr_t handle, dsDisplayEDID_t *edid | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the display sub-system and obtain a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |03|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |04|Call dsGetEDID() with an invalid handle | intptr_t NULL, dsDisplayEDID_t *edid | dsERR_INVALID_PARAM | Should return error indicating invalid handle |
 * |05|Call dsGetEDID() with a NULL dsDisplayEDID_t | intptr_t handle, NULL | dsERR_INVALID_PARAM | Should return error indicating invalid handle |
 * |06|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |07|Call dsGetEDID() without initializing the display sub-system or obtaining a handle | | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note The ability to test scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration.
 *
 */
void test_l1_dsDisplay_negative_dsGetEDID(void) {
    // Start of the test
    gTestID = 8;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    intptr_t displayHandle=-1;
    dsVideoPortType_t vType;
    dsDisplayEDID_t *edid = {0};

    // Step 01: Call dsGetEDID() without initializing the display sub-system
    result = dsGetEDID(displayHandle, edid);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 02: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain a display device handle
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        result = dsGetDisplay(vType, i, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetEDID() with an invalid handle
        result = dsGetEDID((intptr_t)NULL, edid);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetEDID() with a NULL dsDisplayEDID_t
        result = dsGetEDID(displayHandle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetEDID() without initializing the display sub-system
    result = dsGetEDID(displayHandle, edid);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsGetEDIDBytes() retrieves the EDID buffer and length correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the display sub-system and get a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |02|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |03|Call dsGetEDIDBytes() with the obtained handle | intptr_t handle, unsigned char *edid, int *length | dsERR_NONE, valid EDID data, and length | EDID buffer and length should be retrieved successfully |
 * |04|Compare the obtained value with the value from profile |  | Success | The results should be the same  |
 * |05|Free the allocated EDID buffer and terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Memory deallocation and termination should succeed |
 *
 */
void test_l1_dsDisplay_positive_dsGetEDIDBytes(void) {
    // Start of the test
    gTestID = 9;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    intptr_t displayHandle;
    dsVideoPortType_t vType;
    unsigned char *edid = NULL;
    int length = 0;
    uint32_t portIndex;

    // Step 01: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    edid = (unsigned char *)malloc(512);

    // Step 02: Call dsGetDisplay() for each valid port
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Port Type: [%d]\n", __FUNCTION__, vType);

        snprintf(key_str, sizeof(key_str), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Video Port Index: [%d]\n", __FUNCTION__, portIndex);

        result = dsGetDisplay(vType, portIndex, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
        if(result != dsERR_NONE)
        {
            UT_FAIL("Failed to get display handle");
        }

        // Step 03: Call dsGetEDIDBytes() with the obtained handle
        result = dsGetEDIDBytes(displayHandle, edid, &length);
        UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
        if(result != dsERR_NONE)
        {
            UT_FAIL("Failed to get EDID Bytes");
        }

        // Step 04: Compare the value with the value from profile
        if(gSourceType == 0){
            UT_ASSERT_KVP_EQUAL_PROFILE_UINT32(length, "dsDisplay/EDID_Data/edidbytesLength");
        }
    }
    free(edid);

    // Step 05: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}


/**
 * @brief Ensure dsGetEDIDBytes() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetEDIDBytes() without initializing the display sub-system or obtaining a handle | intptr_t handle, unsigned char *edid, int *length | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the display sub-system and obtain a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |03|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |04|Call dsGetEDIDBytes() with an invalid handle | intptr_t NULL, unsigned char *edid, int *length | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |05|Call dsGetEDIDBytes() with null edid | intptr_t, NULL, int *length | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |06|Call dsGetEDIDBytes() with null length | intptr_t, unsigned char *edid, NULL | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |07|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |08|Call dsGetEDIDBytes() without initializing the display sub-system or obtaining a handle | intptr_t handle, unsigned char *edid, int *length | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note The ability to test scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration. Ensure proper memory management in the test setup.
 *
 */
void test_l1_dsDisplay_negative_dsGetEDIDBytes(void) {
    // Start of the test
    gTestID = 10;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    dsVideoPortType_t vType;
    intptr_t displayHandle =-1;
    unsigned char *edid = NULL;
    int length = 0;

    // Step 01: Call dsGetEDIDBytes() without initializing or obtaining a handle
    result = dsGetEDIDBytes(displayHandle, edid, &length);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 02: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Allocate buffer for edid after successful initialization
    edid = (unsigned char *)malloc(512);

    // Step 03: Obtain the display handle with valid port
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        result = dsGetDisplay(vType, i, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetEDIDBytes() with an invalid handle
        result = dsGetEDIDBytes((intptr_t)NULL, edid, &length);
        UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetEDIDBytes() with null edid
        result = dsGetEDIDBytes(displayHandle, NULL, &length);
        UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 06: Call dsGetEDIDBytes() with null length
        result = dsGetEDIDBytes(displayHandle, edid, NULL);
        UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 07: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 08: Call dsGetEDIDBytes() without initializing or obtaining a handle
    result = dsGetEDIDBytes(displayHandle, edid, &length);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    free(edid);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsGetDisplayAspectRatio() retrieves the aspect ratio correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the display sub-system and get a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |02|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |03|Call dsGetDisplayAspectRatio() with the obtained display device handle | intptr_t handle, dsVideoAspectRatio_t *aspectRatio | dsERR_NONE and valid aspect ratio | Aspect ratio of the display device should be retrieved successfully |
 * |04|Compare the results with the value from the profile, and make sure the returned values are the same |  | Success | The values should be the same |
 * |05|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsDisplay_positive_dsGetDisplayAspectRatio(void) {
    // Start of the test
    gTestID = 11;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    intptr_t displayHandle;
    dsVideoPortType_t vType;
    dsVideoAspectRatio_t aspectRatio = dsVIDEO_ASPECT_RATIO_MAX;
    uint32_t portIndex;

    // Step 01: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain display handle with valid port
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Port Type: [%d]\n", __FUNCTION__, vType);

        snprintf(key_str, sizeof(key_str), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Video Port Index: [%d]\n", __FUNCTION__, portIndex);

        result = dsGetDisplay(vType, portIndex, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Call dsGetDisplayAspectRatio() with the obtained handle
        if(gSourceType == 1){
            result = dsGetDisplayAspectRatio(displayHandle, &aspectRatio);
            UT_ASSERT_EQUAL(result, dsERR_NONE);

            // Step 04: Compare the results with default value
            UT_ASSERT_EQUAL(aspectRatio, dsVIDEO_ASPECT_RATIO_16x9);
        } else if(gSourceType == 0){
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}


/**
 * @brief Ensure dsGetDisplayAspectRatio() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsGetDisplayAspectRatio() without initializing the display sub-system or obtaining a handle | intptr_t handle, dsVideoAspectRatio_t *aspectRatio | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the display sub-system and obtain a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |03|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |04|If the device is a source, call dsGetDisplayAspectRatio() with an invalid handle| NULL, dsVideoAspectRatio_t *aspectRatio | dsERR_INVALID_PARAM | Should return error indicating invalid handle |
 * |05|If the device is a source, call dsGetDisplayAspectRatio() with an NULL aspectRatio | intptr_t handle, NULL | dsERR_INVALID_PARAM | Should return error indicating invalid handle |
 * |06|If the device is a sink, call dsGetDisplayAspectRatio() | intptr_t handle, dsVideoAspectRatio_t *aspectRatio | dsERR_OPERATION_NOT_SUPPORTED | API is not supported for sink devices |
 * |07|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |08|Call dsGetDisplayAspectRatio() without initializing the display sub-system or obtaining a handle | intptr_t handle, dsVideoAspectRatio_t *aspectRatio  | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note The ability to test scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration.
 *
 */

void test_l1_dsDisplay_negative_dsGetDisplayAspectRatio(void) {
    // Start of the test
    gTestID = 12;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    intptr_t displayHandle = -1;
    dsVideoPortType_t vType;
    dsVideoAspectRatio_t aspectRatio;

    // Step 01: Call dsGetDisplayAspectRatio() without initializing the display sub-system
    result = dsGetDisplayAspectRatio(displayHandle, &aspectRatio);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 02: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain the display handle with valid port
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        result = dsGetDisplay(vType, i, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        if (gSourceType == 1) {
           // Step 04: Call dsGetDisplayAspectRatio() with an invalid handle for source devices
            result = dsGetDisplayAspectRatio((intptr_t)NULL, &aspectRatio);
            UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

            // Step 05: Call dsGetDisplayAspectRatio() with a NULL aspectRatio for source devices
            result = dsGetDisplayAspectRatio(displayHandle, NULL);
            UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        } else if (gSourceType == 0) {
            // Step 06: Expect dsERR_OPERATION_NOT_SUPPORTED for sink devices
            result = dsGetDisplayAspectRatio(displayHandle, &aspectRatio);
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 07: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 08: Call dsGetDisplayAspectRatio() without initializing the display sub-system
    result = dsGetDisplayAspectRatio(displayHandle, &aspectRatio);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsRegisterDisplayEventCallback() registers the callback correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the display sub-system and get a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |02|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |03|Call dsRegisterDisplayEventCallback() with the obtained display device handle and a valid callback function | Valid handle and callback function | dsERR_NONE | Callback registration should succeed |
 * |04|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 *
 */
void testDisplayCallback(int handle, dsDisplayEvent_t event,void* eventData) {
    // Callback function logic
}

void test_l1_dsDisplay_positive_dsRegisterDisplayEventCallback(void) {
 // Start of the test
    gTestID = 13;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    dsVideoPortType_t vType;
    intptr_t displayHandle;
    uint32_t portIndex;

    // Step 01: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Loop through all valid ports in kSupportedPortTypes
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Port Type: [%d]\n", __FUNCTION__, vType);

        snprintf(key_str, sizeof(key_str), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_str);
        UT_LOG("\n In %s Video Port Index: [%d]\n", __FUNCTION__, portIndex);

        result = dsGetDisplay(vType, portIndex, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Call dsRegisterDisplayEventCallback() with the obtained handle and a valid callback function
        result = dsRegisterDisplayEventCallback(displayHandle, testDisplayCallback);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
    }

    // Step 04: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}


/**
 * @brief Ensure dsRegisterDisplayEventCallback() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 *
 * **Pre-Conditions:**@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsRegisterDisplayEventCallback() without initializing the display sub-system or obtaining a handle |Valid handle and callback function | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the display sub-system and obtain a display device handle | | dsERR_NONE | Initialization and handle retrieval should succeed |
 * |03|Call dsGetDisplay() Loop through all valid ports in numPorts[]|vType: [Valid Port Type]_INPUT, int, intptr_t*  | dsERR_NONE and valid handle | Handle of the display device should be retrieved successfully |
 * |04|Call dsRegisterDisplayEventCallback() with an NULL handle | NULL, callback function | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |05|Call dsRegisterDisplayEventCallback() with an NULL callback function | Valid handle,  NULL | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |06|Terminate the display sub-system with dsDisplayTerm() | | dsERR_NONE | Termination should succeed |
 * |07|Call dsRegisterDisplayEventCallback() without initializing the display sub-system or obtaining a handle | Valid handle and callback function| dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note The ability to test scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration.
 *
 */
void test_l1_dsDisplay_negative_dsRegisterDisplayEventCallback(void) {
    // Start of the test
    gTestID = 14;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    int result;
    dsVideoPortType_t vType;
    intptr_t displayHandle =-1;
    // Step 01: Call dsRegisterDisplayEventCallback() without initializing the display sub-system
    result = dsRegisterDisplayEventCallback(displayHandle, testDisplayCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // Step 02: Initialize the display sub-system
    result = dsDisplayInit();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain the display handle with valid port
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {
        char key_str[64];
        snprintf(key_str, sizeof(key_str), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_str);
        result = dsGetDisplay(vType, i, &displayHandle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsRegisterDisplayEventCallback() with a NULL handle
        result = dsRegisterDisplayEventCallback((intptr_t)NULL, testDisplayCallback);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsRegisterDisplayEventCallback() with a NULL callback function
        result = dsRegisterDisplayEventCallback(displayHandle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: Terminate the display sub-system
    result = dsDisplayTerm();
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsRegisterDisplayEventCallback() without reinitializing the display sub-system
    result = dsRegisterDisplayEventCallback(displayHandle, testDisplayCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE( result, dsERR_NOT_INITIALIZED, dsERR_NONE);
    UT_LOG("\n In %s Return value: [%d]\n", __FUNCTION__, result);

    // End of the test
    UT_LOG("\n Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_dsDisplay_register ( void )
{
    ut_kvp_status_t status;

    /* Get the Device Type */
    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsDisplay.Type", gDeviceType, TEST_DS_DEVICE_TYPE_SIZE);

    if (status == UT_KVP_STATUS_SUCCESS )
    {
        if (!strncmp(gDeviceType, TEST_TYPE_SOURCE_VALUE, TEST_DS_DEVICE_TYPE_SIZE))
        {
            gSourceType = 1;
        }
        else if(!strncmp(gDeviceType, TEST_TYPE_SINK_VALUE, TEST_DS_DEVICE_TYPE_SIZE))
        {
            gSourceType = 0;
        }
        else
        {
            UT_LOG_ERROR("Invalid platform type: %s", gDeviceType);
            return -1;
        }
    }
    else {
        UT_LOG_ERROR("Failed to get the platform type");
        return -1;
    }

    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 dsDisplay]", NULL, NULL );
    if ( NULL == pSuite)
    {
        return -1;
    }

    UT_add_test( pSuite, "dsDisplayInit_L1_positive" ,test_l1_dsDisplay_positive_dsDisplayInit );
    UT_add_test( pSuite, "dsDisplayInit_L1_negative" ,test_l1_dsDisplay_negative_dsDisplayInit );
    UT_add_test( pSuite, "dsDisplayTerm_L1_positive" ,test_l1_dsDisplay_positive_dsDisplayTerm );
    UT_add_test( pSuite, "dsDisplayTerm_L1_negative" ,test_l1_dsDisplay_negative_dsDisplayTerm );
    UT_add_test( pSuite, "dsGetDisplay_L1_positive" ,test_l1_dsDisplay_positive_dsGetDisplay );
    UT_add_test( pSuite, "dsGetDisplay_L1_negative" ,test_l1_dsDisplay_negative_dsGetDisplay );
    UT_add_test( pSuite, "dsGetEDID_L1_positive" ,test_l1_dsDisplay_positive_dsGetEDID );
    UT_add_test( pSuite, "dsGetEDID_L1_negative" ,test_l1_dsDisplay_negative_dsGetEDID );
    UT_add_test( pSuite, "dsGetEDIDBytes_L1_positive" ,test_l1_dsDisplay_positive_dsGetEDIDBytes );
    UT_add_test( pSuite, "dsGetEDIDBytes_L1_negative" ,test_l1_dsDisplay_negative_dsGetEDIDBytes );
    UT_add_test( pSuite, "dsGetDisplayAspectRatio_L1_positive" ,test_l1_dsDisplay_positive_dsGetDisplayAspectRatio );
    UT_add_test( pSuite, "dsGetDisplayAspectRatio_L1_negative" ,test_l1_dsDisplay_negative_dsGetDisplayAspectRatio );
    UT_add_test( pSuite, "dsRegisterDisplayEventCallback_L1_positive" ,test_l1_dsDisplay_positive_dsRegisterDisplayEventCallback );
    UT_add_test( pSuite, "dsRegisterDisplayEventCallback_L1_negative" ,test_l1_dsDisplay_negative_dsRegisterDisplayEventCallback );

    extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "dsDisplay/features/extendedEnumsSupported" );

    return 0;
}


/** @} */ // End of DS_Display_HALTEST_L1
/** @} */ // End of DS_Display_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
