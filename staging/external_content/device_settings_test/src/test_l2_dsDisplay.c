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
 * @defgroup DS_Display_HALTEST Device Settings Display HAL Tests
 * @{
 */

/**
 * @defgroup DS_Display_HALTEST_L2 Device Settings Display HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for DS Display HAL :
 *
 * Level 2 unit test cases for all APIs of Device Settings Display HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-display_halSpec.md](../../docs/pages/ds-display_halSpec.md)
 *
 * @endparblock
 */


/**
 * @file test_l2_dsDisplay.c
 *
 */

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dsDisplay.h"
#include "test_parse_configuration.h"

#define DS_DSIPLAY_KVP_SIZE 128

static int gTestGroup = 2;
static int gTestID = 1;
/**
* @brief This test aims to retrieve and validate the EDID of a sink in the L2 dsDisplay module
*
* This test function tests the retrieval and validation of the Extended Display Identification Data (EDID) of a sink in the L2 dsDisplay module. It ensures that the functions dsDisplayInit, dsGetDisplay, dsGetEDID, dsGetEDIDBytes, and dsDisplayTerm are working as expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [dsDisplay_L2_Low-Level_TestSpecification.md](../docs/pages/ds-display-L2-Low-Level_TestSpec.md)
*/

void test_l2_dsDisplay_RetrieveAndValidateEDID_sink(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE; // Initialize the return status to dsERR_NONE.
    intptr_t handle = 0; // Initialize handle to 0.
    dsDisplayEDID_t edid = { 0 }; // Initialize all edid fields to 0.
    unsigned char edidBytes[MAX_EDID_BYTES_LEN] = { 0 }; // Initialize the edidbytes to 0.
    int length = 0; // Initialize the length to 0.
    unsigned char edid_profile;
    char key_string[DS_DSIPLAY_KVP_SIZE];
    dsVideoPortType_t vType;
    uint32_t portIndex;

    // Step 1: Call dsDisplayInit
    ret = dsDisplayInit();
    UT_LOG_INFO("Invoked dsDisplayInit(), returned: %d\n", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    // Step 2: Call dsGetDisplay
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {

        // Fetch the video port type dynamically
        snprintf(key_string, sizeof(key_string), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_string);
        UT_LOG_INFO("Invoked dsGetDisplay() with video port type: %d\n", vType);

        // Fetch the video port index dynamically
        snprintf(key_string, sizeof(key_string), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_string);
        UT_LOG_INFO("Invoked dsGetDisplay() with port index: %d\n", portIndex);

        ret = dsGetDisplay(vType, portIndex, &handle);
        UT_LOG_INFO("Invoked dsGetDisplay() with vType: %d and portIndex: %d, returned: %d, handle: %ld\n", vType, portIndex, ret, handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            // Call dsDisplayTerm if dsGetDisplay fails
            dsDisplayTerm();
            return;
        }

        // Step 3: Call dsGetEDID
        ret = dsGetEDID(handle, &edid);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_INFO("Invoked dsGetEDID() with handle %ld, returned: %d, productCode: %d\n", handle, ret, edid.productCode);

        UT_ASSERT_KVP_EQUAL_PROFILE_UINT32(edid.productCode, "dsDisplay/EDID_Data/productCode");

        // Step 4: Call dsGetEDIDBytes
        ret = dsGetEDIDBytes(handle, edidBytes, &length);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_INFO("Invoked dsGetEDIDBytes() with handle %ld, returned: %d, Manufacturer ID: %d\n", handle, ret, edidBytes[8] << 8 | edidBytes[9]);
        if (ret != dsERR_NONE)
        {
            // Call dsDisplayTerm if dsGetEDIDBytes fails
            dsDisplayTerm();
            return;
        }

        // Manufacturer ID check
        for( uint8_t i = 8; i < 9; i++)
        {
            snprintf(key_string, DS_DSIPLAY_KVP_SIZE, "dsDisplay.edidBytes.%d", i);
            edid_profile = UT_KVP_PROFILE_GET_UINT8(key_string);
            if(edid_profile != edidBytes[i])
            {
                UT_FAIL("edid check failed");
                UT_LOG_ERROR("edid byte: %x, expected value: %x", edidBytes[i], edid_profile);
                break;
            }
        }
    }

    // Step 5: Call dsDisplayTerm
    ret = dsDisplayTerm();
    UT_LOG_INFO("Invoked dsDisplayTerm(), returned: %d\n", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}


/**
* @brief This test aims to verify the default aspect ratio of the source in the L2 dsDisplay module
*
* In this test, the dsDisplayInit(), dsGetDisplay(), dsGetDisplayAspectRatio(), and dsDisplayTerm() functions are called in sequence to check the default aspect ratio of the source. The test verifies that the aspect ratio is dsVIDEO_ASPECT_RATIO_16x9. If any of the function calls fail or the aspect ratio is not as expected, the test fails.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [dsDisplay_L2_Low-Level_TestSpecification.md](../docs/pages/ds-display-L2-Low-Level_TestSpec.md)
*/

void test_l2_dsDisplay_TestDefaultAspectRatio_source(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE; // Initialize the return status to dsERR_NONE.
    intptr_t handle = 0; //Initialize handle to 0.
    dsVideoAspectRatio_t aspectRatio = dsVIDEO_ASPECT_RATIO_MAX; // Initialize aspect ratio with MAX value.
    char key_string[DS_DSIPLAY_KVP_SIZE];
    dsVideoPortType_t vType;
    uint32_t portIndex;

    // Step 1: Call dsDisplayInit()
    UT_LOG_DEBUG("Invoking dsDisplayInit()");
    ret = dsDisplayInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    // Step 2: Call dsGetDisplay()
    uint32_t numPorts = UT_KVP_PROFILE_GET_UINT32("dsDisplay/Number_of_ports");
    for (size_t i = 0; i < numPorts; i++) {

        // Fetch the video port type dynamically
        snprintf(key_string, sizeof(key_string), "dsDisplay/Video_Ports/%ld", i);
        vType = (dsVideoPortType_t) UT_KVP_PROFILE_GET_UINT32(key_string);
        UT_LOG_DEBUG("Invoking dsGetDisplay() with video port type: %d", vType);

        // Fetch the video port index dynamically
        snprintf(key_string, sizeof(key_string), "dsDisplay/VideoPort_Index/%ld", i);
        portIndex = UT_KVP_PROFILE_GET_UINT32(key_string);
        UT_LOG_DEBUG("Invoking dsGetDisplay() with port index: %d", portIndex);

        ret = dsGetDisplay(vType, portIndex, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetDisplay() failed with error: %d\n", ret);
            dsDisplayTerm();
            return;
        }

        // Step 3: Call dsGetDisplayAspectRatio() with the obtained handle
        UT_LOG_DEBUG("Invoking dsGetDisplayAspectRatio() with handle obtained from dsGetDisplay()");
        ret = dsGetDisplayAspectRatio(handle, &aspectRatio);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(aspectRatio, dsVIDEO_ASPECT_RATIO_16x9);
        if (ret != dsERR_NONE || aspectRatio != dsVIDEO_ASPECT_RATIO_16x9)
        {
            UT_LOG_ERROR("dsGetDisplayAspectRatio() failed with error: %d\n", ret);
        }

        // Step 4: Verify aspect ratio
        UT_LOG_DEBUG("Verifying that the aspect ratio is dsVIDEO_ASPECT_RATIO_16x9");
        UT_ASSERT_EQUAL(aspectRatio, dsVIDEO_ASPECT_RATIO_16x9);
    }

    // Step 5: Call dsDisplayTerm()
    UT_LOG_DEBUG("Invoking dsDisplayTerm()");
    ret = dsDisplayTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}


static UT_test_suite_t * pSuite = NULL;
/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l2_dsDisplay_register(void)
{
     int32_t source_type = 0;
     ut_kvp_status_t status;

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsDisplay.Type", gDeviceType, TEST_DS_DEVICE_TYPE_SIZE);

    // Create the test suite
    if (status == UT_KVP_STATUS_SUCCESS ) {
        if (!strncmp(gDeviceType, TEST_TYPE_SINK_VALUE, TEST_DS_DEVICE_TYPE_SIZE)) {
            pSuite = UT_add_suite("[L2 dsDisplay Sink]", NULL, NULL);
            if (pSuite == NULL)
            {
                return -1;
            }
            source_type = 0;
        }
        else if(!strncmp(gDeviceType, TEST_TYPE_SOURCE_VALUE, TEST_DS_DEVICE_TYPE_SIZE)) {
            pSuite = UT_add_suite("[L2 dsDisplay Source ]", NULL, NULL);
            if (pSuite == NULL)
            {
                return -1;
            }
            source_type = 1;
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
    // List of test function names and strings
    if(source_type == 0) {
        UT_add_test( pSuite, "L2_RetrieveAndValidateEDID_sink", test_l2_dsDisplay_RetrieveAndValidateEDID_sink);
    }
    else if ( source_type == 1 ){
        UT_add_test( pSuite, "L2_TestDefaultAspectRatio_source", test_l2_dsDisplay_TestDefaultAspectRatio_source);
    }

    return 0;
}

/** @} */ // End of DS_Display_HALTEST_L2
/** @} */ // End of DS_Display_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK