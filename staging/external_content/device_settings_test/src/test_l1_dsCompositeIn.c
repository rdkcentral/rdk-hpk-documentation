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
 * @defgroup DS_CompositeIn_HALTEST Device Settings Composite Input HAL Tests
 * @{
 */

/**
 * @defgroup DS_CompositeIn_HALTEST_L1 Device Settings Composite Input HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for DS Composite Input HAL :
 *
 * Level 1 unit test cases for all APIs of Device Settings Composite Input HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-composite-in_halSpec.md](../../docs/pages/ds-composite-in_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l1_dsCompositeIn.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dsCompositeIn.h"
#include "test_parse_configuration.h"

static int gTestGroup = 1;
static int gTestID = 1;

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

/* Global flags to support features */
static bool extendedEnumsSupported=false; //Default to not supported

/**
 * @brief Ensure dsCompositeInInit() returns correct error codes during basic positive scenarios
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
 * |01|Call dsCompositeInInit() to initialize the module | | dsERR_NONE | First-time initialization should succeed |
 * |02|Call dsCompositeInTerm() to terminate the module | | dsERR_NONE | Termination should succeed |
 * |03|Call dsCompositeInInit() again initialize after termination | | dsERR_NONE | Re-initialization should succeed |
 * |04|Call dsCompositeInTerm() again to terminate the module | | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInInit(void)
{
    gTestID = 1;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: First-time initialization
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Termination
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 03: Re-initialization
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 04: Termination again
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInInit() returns correct error codes during negative scenarios with proper initialization and termination
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
 * |01|Call dsCompositeInInit() to initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInInit() again while already initialized | | dsERR_ALREADY_INITIALIZED | Should return error indicating the module is already initialized |
 * |03|Call dsCompositeInTerm() again to terminate the module | | dsERR_NONE | Termination should succeed |
 *
 * @note The ability to simulate specific conditions like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration. These cases are not included in this test plan due to the challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInInit(void)
{
    gTestID = 2;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Attempt to initialize again
    dsError_t result = dsCompositeInInit();
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_ALREADY_INITIALIZED, dsERR_NONE);

    // Step 03: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInTerm() returns correct error codes during positive scenarios
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |03|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInTerm(void)
{
    gTestID = 3;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 03: Re-initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 04: Terminate the module again
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInTerm() returns correct error codes during negative scenarios
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
 * |01|Call dsCompositeInTerm() without initializing the module | | dsERR_NOT_INITIALIZED | Termination should fail due to the module not being initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |04|Call dsCompositeInTerm() again after proper termination | | dsERR_NOT_INITIALIZED | Termination should fail, indicating the module is already terminated |
 *
 * @note The ability to simulate specific conditions like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL might require specific setup or environment configuration. These cases are not included in this test plan due to the challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInTerm(void)
{
    gTestID = 4;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call Termination without initialization
    dsError_t result = dsCompositeInTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 04: Attempt to terminate again after proper termination
    result = dsCompositeInTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInGetNumberOfInputs() returns consistent error codes and output during positive scenarios
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInGetNumberOfInputs() with a valid pointer | uint8_t *pNumberOfInputs | dsERR_NONE and a valid number of inputs | Number of inputs should be returned correctly |
 * |03|Get the number of inputs from the profile and compare it with the value obtained by calling dsCompositeInGetNumberOfInputs()| | dsERR_NONE | |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInGetNumberOfInputs(void)
{
    gTestID = 5;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    uint8_t numberOfInputs = 0;

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Get number of inputs for the first time
    UT_ASSERT_EQUAL(dsCompositeInGetNumberOfInputs(&numberOfInputs), dsERR_NONE);

    // Step 03: Compare the result with the value from the profile
    UT_ASSERT_KVP_EQUAL_PROFILE_UINT8(numberOfInputs, "dsCompositeIn/composite_input_configurations/number_of_ports");

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInGetNumberOfInputs() returns correct error codes during negative scenarios
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
 * |01|Call dsCompositeInGetNumberOfInputs() without initializing the module | uint8_t *pNumberOfInputs | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInGetNumberOfInputs() with a NULL pointer | NULL pointer | dsERR_INVALID_PARAM | Should return error indicating invalid parameter |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |05|Call dsCompositeInGetNumberOfInputs() after terminating the module | uint8_t *pNumberOfInputs | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL are not included in this test plan due to the challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInGetNumberOfInputs(void)
{
    gTestID = 6;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    uint8_t pNumberOfInputs = 0;

    // Step 01: Call GetNumberOfInputs without initialization
    dsError_t result = dsCompositeInGetNumberOfInputs(&pNumberOfInputs);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Call GetNumberOfInputs with NULL pointer
    UT_ASSERT_EQUAL(dsCompositeInGetNumberOfInputs(NULL), dsERR_INVALID_PARAM);

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 05: Call GetNumberOfInputs after termination
    result = dsCompositeInGetNumberOfInputs(&pNumberOfInputs);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInGetStatus() returns correct status and error codes during positive scenarios
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInGetStatus() with a valid pointer | dsCompositeInStatus_t *pStatus | dsERR_NONE and valid status | Status should be retrieved correctly |
 * |03|Call dsCompositeInGetStatus() with a valid pointer | dsCompositeInStatus_t *pStatus | dsERR_NONE and valid status | Status should be retrieved correctly |
 * |04|Verify the retrieved statuses are the same |  | Success | Ensures the statuses are equal|
 * |05|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInGetStatus(void)
{
    gTestID = 7;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    dsCompositeInStatus_t status1 = {0}, status2 = {0};

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Get status for the first time
    UT_ASSERT_EQUAL(dsCompositeInGetStatus(&status1), dsERR_NONE);

    // Step 03: Get status for the second time
    UT_ASSERT_EQUAL(dsCompositeInGetStatus(&status2), dsERR_NONE);

    // Step 04: Verify the retrieved statuses are the same
    UT_ASSERT_EQUAL(memcmp(&status1, &status2, sizeof(dsCompositeInStatus_t)), 0);

    // Step 05: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInGetStatus() returns correct error codes during negative scenarios
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
 * |01|Call dsCompositeInGetStatus() without initializing the module | | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInGetStatus() with a NULL pointer | NULL pointer | dsERR_INVALID_PARAM | Should return error indicating invalid parameter |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |05|Call dsCompositeInGetStatus() after terminating the module | | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL are not included due to challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInGetStatus(void)
{
    gTestID = 8;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    dsCompositeInStatus_t status = {0};

    // Step 01: Call GetStatus without initialization
    dsError_t result = dsCompositeInGetStatus(&status);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Call GetStatus with NULL pointer
    UT_ASSERT_EQUAL(dsCompositeInGetStatus(NULL), dsERR_INVALID_PARAM);

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 05: Call GetStatus after termination
    result = dsCompositeInGetStatus(&status);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInSelectPort() sets the COMPOSITE Input port correctly during positive scenarios
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInGetNumberOfInputs() with a valid pointer | uint8_t *pNumberOfInputs | dsERR_NONE and a valid number of inputs | Number of inputs should be returned correctly |
 * |03|Call dsCompositeInSelectPort() and loop through all valid ports(based on the number of ports) | dsCOMPOSITE_IN_PORT_0 | dsERR_NONE | Port should be set correctly |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInSelectPort(void)
{
    gTestID = 9;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    uint8_t numberOfInputs = 0;

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Get number of inputs
    UT_ASSERT_EQUAL(dsCompositeInGetNumberOfInputs(&numberOfInputs), dsERR_NONE);

    // Step 03: Loop through all valid ports and set them
    for (int i = 0; i < numberOfInputs; ++i) {
        UT_ASSERT_EQUAL(dsCompositeInSelectPort((dsCompositeInPort_t)(i)), dsERR_NONE);
    }

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInSelectPort() returns correct error codes during negative scenarios
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
 * |01|Call dsCompositeInSelectPort() without initializing the module | Port value | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInGetNumberOfInputs() with a valid pointer | uint8_t *pNumberOfInputs | dsERR_NONE and a valid number of inputs | Number of inputs should be returned correctly |
 * |04|Call dsCompositeInSelectPort() with an invalid port number (out of range) | dsCOMPOSITE_IN_PORT_MAX| dsERR_INVALID_PARAM | Should return error indicating invalid port number |
 * |05|Call dsCompositeInSelectPort() with an invalid port number (pNumberOfInputs +1) | [Invalid port number] dsERR_INVALID_PARAM | Should return error indicating invalid port number |
 * |06|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |07|Call dsCompositeInSelectPort() without initializing the module | Port value | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED are not included due to challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInSelectPort(void)
{
    gTestID = 10;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    uint8_t numberOfInputs = 0;

    // Step 01: Call SelectPort without initialization
    dsError_t result = dsCompositeInSelectPort(dsCOMPOSITE_IN_PORT_0);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Get number of inputs
    UT_ASSERT_EQUAL(dsCompositeInGetNumberOfInputs(&numberOfInputs), dsERR_NONE);

    // Step 04: Call SelectPort with an out-of-range port number
    UT_ASSERT_EQUAL(dsCompositeInSelectPort(dsCOMPOSITE_IN_PORT_MAX), dsERR_INVALID_PARAM);

    // Step 05: Call SelectPort with an invalid port number (pNumberOfInputs + 1)
    UT_ASSERT_EQUAL(dsCompositeInSelectPort(numberOfInputs + 1), dsERR_INVALID_PARAM);

    // Step 06: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 07: Call SelectPort after termination
    result = dsCompositeInSelectPort(dsCOMPOSITE_IN_PORT_0);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInScaleVideo() scales the COMPOSITE In video correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 *
 * **Pre-Conditions:**None@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|---------|----------|--------------|-----|
 * |01|Initialize the module and select a COMPOSITE input port | | dsERR_NONE | Initialization and port selection should succeed |
 * |02|Call dsCompositeInGetNumberOfInputs() with a valid pointer | uint8_t *pNumberOfInputs | dsERR_NONE and a valid number of inputs | Number of inputs should be returned correctly |
 * |03|Call dsCompositeInSelectPort() with a valid port number based on pNumberOfInputs | dsCOMPOSITE_IN_PORT_0 | dsERR_NONE | Port should be set correctly |
 * |04|Call dsCompositeInScaleVideo() with valid parameters within current resolution | x=10, y=10, width=800, height=800 | dsERR_NONE | Video scaling should succeed |
 * |05|Call dsCompositeInScaleVideo() with different valid parameters to verify flexibility | Different x=50, y=50, width=600, height=600 | dsERR_NONE | Video should be scaled correctly with different parameters |
 * |06|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInScaleVideo(void)
{
    gTestID = 11;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    uint8_t numberOfInputs = 0;

    // Step 01: Initialize the module and select a port
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);
    UT_ASSERT_EQUAL(dsCompositeInSelectPort(dsCOMPOSITE_IN_PORT_0), dsERR_NONE);

    // Step 02: Get number of inputs
    UT_ASSERT_EQUAL(dsCompositeInGetNumberOfInputs(&numberOfInputs), dsERR_NONE);

    // Step 03: Select a valid port number
    for(int x = 0; x < numberOfInputs; x++){
        UT_ASSERT_EQUAL(dsCompositeInSelectPort((dsCompositeInPort_t)(x)), dsERR_NONE);

         // Step 04: Scale video with first set of parameters
        UT_ASSERT_EQUAL(dsCompositeInScaleVideo(10, 10, 800, 800), dsERR_NONE);

        // Step 05: Scale video with a different set of parameters
        UT_ASSERT_EQUAL(dsCompositeInScaleVideo(50, 50, 600, 600), dsERR_NONE);
    }

    // Step 06: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInScaleVideo() returns correct error codes during negative scenarios
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
 * |01|Call dsCompositeInScaleVideo() without initializing the module or selecting a port | x=10, y=10, width=800, height=800  | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Initialize the module and select a COMPOSITE input port | | dsERR_NONE | Initialization and port selection should succeed |
 * |03|Call dsCompositeInScaleVideo() without selecting a port | x=10, y=10, width=800, height=800 | dsERR_OPERATION_NOT_SUPPORTED | Should return an error based on new valid screen being connected |
 * |04|Call dsCompositeInSelectPort() with a valid port number based on pNumberOfInputs | dsCOMPOSITE_IN_PORT_0 | dsERR_NONE | Port should be set correctly |
 * |05|Call dsCompositeInScaleVideo() with parameters out of current resolution | x=-1, y=10, width=10, height=10 | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |06|Call dsCompositeInScaleVideo() with parameters out of current resolution | x=10, y=-1, width=10, height=10 | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |07|Call dsCompositeInScaleVideo() with parameters out of current resolution | x=10, y=10, width=-1, height=10 | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |08|Call dsCompositeInScaleVideo() with parameters out of current resolution | x=10, y=10, width=10, height=-1 | dsERR_INVALID_PARAM | Should return error indicating invalid parameters |
 * |09|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |10|Call dsCompositeInScaleVideo() without initializing the module or selecting a port | | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_GENERAL are not included due to challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInScaleVideo(void)
{
    gTestID = 12;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call ScaleVideo without initialization or port selection
    dsError_t result = dsCompositeInScaleVideo(10, 10, 800, 800);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module and select a port
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Initialize the module and select a port
    UT_ASSERT_EQUAL(dsCompositeInScaleVideo(10, 10, 800, 800), dsERR_OPERATION_NOT_SUPPORTED);

    // Step 04: Initialize the module and select a port
    UT_ASSERT_EQUAL(dsCompositeInSelectPort(dsCOMPOSITE_IN_PORT_0), dsERR_NONE);

    // Step 05 to 08: Test ScaleVideo with invalid parameters
    UT_ASSERT_EQUAL(dsCompositeInScaleVideo(-1, 10, 10, 10), dsERR_INVALID_PARAM);
    UT_ASSERT_EQUAL(dsCompositeInScaleVideo(10, -1, 10, 10), dsERR_INVALID_PARAM);
    UT_ASSERT_EQUAL(dsCompositeInScaleVideo(10, 10, -1, 10), dsERR_INVALID_PARAM);
    UT_ASSERT_EQUAL(dsCompositeInScaleVideo(10, 10, 10, -1), dsERR_INVALID_PARAM);

    // Step 09: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 10: Call ScaleVideo without initialization or port selection
    result = dsCompositeInScaleVideo(10, 10, 800, 800);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterConnectCB() registers the COMPOSITE Input hot plug event correctly during positive scenarios
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInRegisterConnectCB() with a valid callback function | dsCompositeInConnectCB_t CBFunc | dsERR_NONE | Callback registration should succeed |
 * |03|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
// Example callback function
void exampleConnectCallback(dsCompositeInPort_t Port, bool isPortConnected) {
    // Implementation of the callback (dummy for testing)
}

void test_l1_dsCompositeIn_positive_dsCompositeInRegisterConnectCB(void)
{
    gTestID = 13;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Register the callback function
    UT_ASSERT_EQUAL(dsCompositeInRegisterConnectCB(exampleConnectCallback), dsERR_NONE);

    // Step 03: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterConnectCB() returns correct error codes during negative scenarios
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
 * |:--:|---------|----------|--------------|-----|
 * |01|Call dsCompositeInRegisterConnectCB() without initializing the module | dsCompositeInConnectCB_t CBFunc| dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInRegisterConnectCB() with a NULL or invalid callback function | NULL or invalid callback | dsERR_INVALID_PARAM | Should return error indicating invalid parameter |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |05|Call dsCompositeInRegisterConnectCB() without initializing the module | dsCompositeInConnectCB_t CBFunc | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL are not included due to challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInRegisterConnectCB(void)
{
    gTestID = 14;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call RegisterConnectCB without initialization
    dsError_t result = dsCompositeInRegisterConnectCB(exampleConnectCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Register the callback with NULL or invalid function
    UT_ASSERT_EQUAL(dsCompositeInRegisterConnectCB(NULL), dsERR_INVALID_PARAM);

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 05: Call RegisterConnectCB after termination
    result = dsCompositeInRegisterConnectCB(exampleConnectCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterSignalChangeCB() registers the Composite Input Signal Change event correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInRegisterSignalChangeCB() with a valid callback function | dsCompositeInSignalChangeCB_t CBFunc | dsERR_NONE | Callback registration should succeed |
 * |03|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
// Example callback function for signal change
void exampleSignalChangeCallback(dsCompositeInPort_t port, dsCompInSignalStatus_t sigStatus) {
    // Implementation of the callback (dummy for testing)
}

void test_l1_dsCompositeIn_positive_dsCompositeInRegisterSignalChangeCB(void)
{
    gTestID = 15;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Register the signal change callback function
    UT_ASSERT_EQUAL(dsCompositeInRegisterSignalChangeCB(exampleSignalChangeCallback), dsERR_NONE);

    // Step 03: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterSignalChangeCB() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
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
 * |01|Call dsCompositeInRegisterSignalChangeCB() without initializing the module | dsCompositeInSignalChangeCB_t CBFunc | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInRegisterSignalChangeCB() with a NULL or invalid callback function | NULL or invalid callback | dsERR_INVALID_PARAM | Should return error indicating invalid parameter |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |05|Call dsCompositeInRegisterSignalChangeCB() without initializing the module | dsCompositeInSignalChangeCB_t CBFunc | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL are not included due to challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInRegisterSignalChangeCB(void)
{
    gTestID = 16;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call RegisterSignalChangeCB without initialization
    dsError_t result = dsCompositeInRegisterSignalChangeCB(exampleSignalChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Register the callback with NULL or invalid function
    UT_ASSERT_EQUAL(dsCompositeInRegisterSignalChangeCB(NULL), dsERR_INVALID_PARAM);

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 05: Call RegisterSignalChangeCB after termination
    result = dsCompositeInRegisterSignalChangeCB(exampleSignalChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterStatusChangeCB() registers the Composite Input Status Change event correctly during positive scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
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
 * |01|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |02|Call dsCompositeInRegisterStatusChangeCB() with a valid callback function | dsCompositeInStatusChangeCB_t CBFunc | dsERR_NONE | Callback registration should succeed |
 * |03|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 *
 */
// Example callback function for status change
void exampleStatusChangeCallback(dsCompositeInStatus_t inputStatus) {
    // Implementation of the callback (dummy for testing)
}

void test_l1_dsCompositeIn_positive_dsCompositeInRegisterStatusChangeCB(void)
{
    gTestID = 17;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 02: Register the status change callback function
    UT_ASSERT_EQUAL(dsCompositeInRegisterStatusChangeCB(exampleStatusChangeCallback), dsERR_NONE);

    // Step 03: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterStatusChangeCB() returns correct error codes during negative scenarios
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
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
 * |01|Call dsCompositeInRegisterStatusChangeCB() without initializing the module | dsCompositeInStatusChangeCB_t CBFunc | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 * |02|Call dsCompositeInInit() and initialize the module | | dsERR_NONE | Initialization should succeed |
 * |03|Call dsCompositeInRegisterStatusChangeCB() with a NULL or invalid callback function | NULL or invalid callback | dsERR_INVALID_PARAM | Should return error indicating invalid parameter |
 * |04|Call dsCompositeInTerm() to terminate the module| | dsERR_NONE | Termination should succeed |
 * |05|Call dsCompositeInRegisterStatusChangeCB() without initializing the module | dsCompositeInStatusChangeCB_t CBFunc | dsERR_NOT_INITIALIZED | Should return error indicating the module is not initialized |
 *
 * @note Scenarios like dsERR_OPERATION_NOT_SUPPORTED and dsERR_GENERAL are not included due to challenges in realistic simulation.
 */
void test_l1_dsCompositeIn_negative_dsCompositeInRegisterStatusChangeCB(void)
{
    gTestID = 18;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call RegisterStatusChangeCB without initialization
    dsError_t result = dsCompositeInRegisterStatusChangeCB(exampleStatusChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 03: Register the callback with NULL or invalid function
    UT_ASSERT_EQUAL(dsCompositeInRegisterStatusChangeCB(NULL), dsERR_INVALID_PARAM);

    // Step 04: Terminate the module
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 05: Call RegisterStatusChangeCB after termination
    result = dsCompositeInRegisterStatusChangeCB(exampleStatusChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

void mockCompositeInVideoModeUpdateCallback(dsCompositeInPort_t port, dsVideoPortResolution_t videoResolution){
// Mock implementation, can be customized for testing
}

/**
 * @brief Ensure dsCompositeInRegisterVideoModeUpdateCB() correctly registers a callback during positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize the Composite input sub-system using dsCompositeInInit() | | dsERR_NONE | Should Pass |
 * |02|Call dsCompositeInRegisterVideoModeUpdateCB() with valid callback function |dsCompositeInVideoModeUpdateCB_t| dsERR_NONE | Callback should be registered |
 * |03|Call dsCompositeInTerm() to ensure deinitialization | | dsERR_NONE | Clean up after test |
 *
 */
void test_l1_dsCompositeIn_positive_dsCompositeInRegisterVideoModeUpdateCB(void) {

    gTestID = 19;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 1: Initialize the Composite input sub-system using dsCompositeInInit()
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 2: Call dsCompositeInRegisterVideoModeUpdateCB() 
    UT_ASSERT_EQUAL(dsCompositeInRegisterVideoModeUpdateCB(mockCompositeInVideoModeUpdateCallback), dsERR_NONE);


    // Step 3: Call dsCompositeInTerm() to ensure deinitialization
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

/**
 * @brief Ensure dsCompositeInRegisterVideoModeUpdateCB() returns correct error codes during negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsCompositeInRegisterVideoModeUpdateCB() without initializing the Composite input sub-system | dsERR_NOT_INITIALIZED | Should Pass |
 * |02|Initialize the Composite input sub-system using dsCompositeInInit() | | dsERR_NONE | Should Pass |
 * |03|Call dsCompositeInRegisterVideoModeUpdateCB() with invalid input |NULL| dsERR_INVALID_PARAM | Should Pass |
 * |04|Call dsCompositeInTerm() to ensure deinitialization | | dsERR_NONE | Clean up after test |
 * |05|Call dsCompositeInRegisterVideoModeUpdateCB() after terminating the Composite input sub-system | dsERR_NOT_INITIALIZED | Should Pass |
 *
 * @note Testing for the `dsERR_OPERATION_NOT_SUPPORTED` might be challenging since it requires a specific scenario where the attempted operation is not supported.
 *
 */
void test_l1_dsCompositeIn_negative_dsCompositeInRegisterVideoModeUpdateCB(void) {

    gTestID = 20;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    // Step 1: Call dsCompositeInRegisterVideoModeUpdateCB() without initializing the Composite input sub-system
    dsError_t result = dsCompositeInRegisterVideoModeUpdateCB(mockCompositeInVideoModeUpdateCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 2: Initialize the Composite input sub-system using dsCompositeInInit()
    UT_ASSERT_EQUAL_FATAL(dsCompositeInInit(), dsERR_NONE);

    // Step 3: Call dsCompositeInRegisterVideoModeUpdateCB() with invalid input
    UT_ASSERT_EQUAL(dsCompositeInRegisterVideoModeUpdateCB(NULL), dsERR_INVALID_PARAM);

    // Step 4: Call dsCompositeInTerm() to ensure deinitialization
    UT_ASSERT_EQUAL_FATAL(dsCompositeInTerm(), dsERR_NONE);

    // Step 5: Call dsCompositeInRegisterVideoModeUpdateCB() after terminating the Composite input sub-system
    result = dsCompositeInRegisterVideoModeUpdateCB(mockCompositeInVideoModeUpdateCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG("\n Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_dsCompositeIn_register ( void )
{
    ut_kvp_status_t status = UT_KVP_STATUS_SUCCESS;
    int32_t source_type = 0;

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsCompositeIn.Type", gDeviceType, TEST_DS_DEVICE_TYPE_SIZE);
    if (status == UT_KVP_STATUS_SUCCESS ) {
        if (!strncmp(gDeviceType, TEST_TYPE_SINK_VALUE, TEST_DS_DEVICE_TYPE_SIZE)) {
            // Create the test suite for sink type
            pSuite = UT_add_suite("[L1 dsCompositeIn - Sink]", NULL, NULL);
            if (pSuite == NULL)
            {
                return -1;
            }
            source_type = 0;
        }
        else {
            UT_LOG_ERROR("Invalid platform type: %s", gDeviceType);
            return -1;
        }
    }
    else {
        UT_LOG_ERROR("Failed to get the platform type");
        return -1;
    }

    if ( source_type == 0 ){
        UT_add_test( pSuite, "dsCompositeInInit_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInInit );
        UT_add_test( pSuite, "dsCompositeInInit_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInInit );
        UT_add_test( pSuite, "dsCompositeInTerm_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInTerm );
        UT_add_test( pSuite, "dsCompositeInTerm_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInTerm );
        UT_add_test( pSuite, "dsCompositeInGetNumberOfInputs_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInGetNumberOfInputs );
        UT_add_test( pSuite, "dsCompositeInGetNumberOfInputs_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInGetNumberOfInputs );
        UT_add_test( pSuite, "dsCompositeInGetStatus_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInGetStatus );
        UT_add_test( pSuite, "dsCompositeInGetStatus_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInGetStatus );
        UT_add_test( pSuite, "dsCompositeInSelectPort_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInSelectPort );
        UT_add_test( pSuite, "dsCompositeInSelectPort_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInSelectPort );
        UT_add_test( pSuite, "dsCompositeInScaleVideo_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInScaleVideo );
        UT_add_test( pSuite, "dsCompositeInScaleVideo_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInScaleVideo );
        UT_add_test( pSuite, "dsCompositeInRegisterConnectCB_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInRegisterConnectCB );
        UT_add_test( pSuite, "dsCompositeInRegisterConnectCB_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInRegisterConnectCB );
        UT_add_test( pSuite, "dsCompositeInRegisterSignalChangeCB_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInRegisterSignalChangeCB );
        UT_add_test( pSuite, "dsCompositeInRegisterSignalChangeCB_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInRegisterSignalChangeCB );
        UT_add_test( pSuite, "dsCompositeInRegisterStatusChangeCB_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInRegisterStatusChangeCB );
        UT_add_test( pSuite, "dsCompositeInRegisterStatusChangeCB_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInRegisterStatusChangeCB );
	UT_add_test( pSuite, "dsCompositeInRegisterVideoModeUpdateCB_L1_positive" ,test_l1_dsCompositeIn_positive_dsCompositeInRegisterVideoModeUpdateCB );
	UT_add_test( pSuite, "dsCompositeInRegisterVideoModeUpdateCB_L1_negative" ,test_l1_dsCompositeIn_negative_dsCompositeInRegisterVideoModeUpdateCB );
	extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "dsCompositeIn/features/extendedEnumsSupported" );
    }
    return 0;
}

/** @} */ // End of DS_CompositeIn_HALTEST_L1
/** @} */ // End of DS_CompositeIn_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
