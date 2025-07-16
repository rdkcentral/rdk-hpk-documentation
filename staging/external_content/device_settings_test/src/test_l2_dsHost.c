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
 * @defgroup DS_Host_HALTEST_L2 Device Settings Host HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for DS Host HAL :
 *
 * Level 2 unit test cases for all APIs of Device Settings Host HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-host_halSpec.md](../../docs/pages/ds-host_halSpec.md)
 *
 * @endparblock
 */


/**
 * @file test_l2_dsAudio.c
 *
 */
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dsHost.h"
#include "test_parse_configuration.h"

#define DSHOST_SOC_LENGTH    20
#define DS_HOST_KVP_SIZE     128

static int gTestGroup = 2;
static int gTestID = 1;

/**
* @brief Test for getting CPU temperature from the Host HAL sub-system
*
* This test function initializes the Host HAL sub-system, gets the CPU temperature,
* checks if the temperature is within the valid range, and then terminates the
* Host sub-system. It uses the provided macro to check if the returned CPU temperature
* is equal to the value specified in the 'Sink_HostSettings.yaml' configuration file.
* If any of the API calls fail, the test function will immediately return, as
* indicated by the use of the UT_ASSERT_EQUAL_FATAL macro.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [dsHost_L2_Low-Level_TestSpecification.md](../docs/pages/ds-host_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsHost_GetCPUTemperature(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t retStatus = dsERR_NONE;
    float cpuTemperature = 0.0;
    int32_t minTemparature = 0;
    int32_t maxTemparature = 0;

    UT_LOG_DEBUG("Invoking dsHostInit with no input parameters");
    retStatus = dsHostInit();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetCPUTemperature with valid buffer");
    retStatus = dsGetCPUTemperature(&cpuTemperature);
    UT_LOG_DEBUG("CPU Temperature: %f, Return status: %d", cpuTemperature, retStatus);
    UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
    if (retStatus != dsERR_NONE)
    {
        UT_LOG_ERROR("dsGetCPUTemperature failed with status: %d", retStatus);
    }

    minTemparature = UT_KVP_PROFILE_GET_UINT32("dsHost.cpuTemperature.0");
    maxTemparature = UT_KVP_PROFILE_GET_UINT32("dsHost.cpuTemperature.1");

    UT_LOG_DEBUG("CPU Temperature from Profile: min:%d, max:%d", minTemparature, maxTemparature);

    if(cpuTemperature > maxTemparature || cpuTemperature < minTemparature)
    {
        UT_FAIL("Invalid temperature");
    }

    UT_LOG_DEBUG("Invoking dsHostTerm with no input parameters");
    retStatus = dsHostTerm();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to get and verify the SoC ID from the Host HAL sub-system
*
* This test function initializes the Host HAL sub-system, gets the SoC ID,
* verifies it with the value from the configuration file, and then terminates
* the Host HAL sub-system. It uses the provided macro to compare the SoC ID
* with the value from the configuration file. The function logs all the
* steps and checks the return values of the API calls.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [dsHost_L2_Low-Level_TestSpecification.md](../docs/pages/ds-host_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsHost_GetAndVerifySocID(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    ut_kvp_status_t status;

    dsError_t ret;
    char socID[DSHOST_SOC_LENGTH] = {0};
    char socIDProfile[DSHOST_SOC_LENGTH] = {0};

    UT_LOG_DEBUG("Invoking dsHostInit with no input parameters");
    ret = dsHostInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetSocIDFromSDK with valid buffer");
    ret = dsGetSocIDFromSDK(socID);
    UT_LOG_DEBUG("Return status: %d, socID: %s", ret, socID);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);
    if (ret != dsERR_NONE)
    {
        UT_LOG_ERROR("dsGetSocIDFromSDK failed with status: %d", ret);
    }

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsHost.socID", socIDProfile, DSHOST_SOC_LENGTH);
    UT_ASSERT( status == UT_KVP_STATUS_SUCCESS );
    if(!strstr(socID, socIDProfile))
    {
        UT_FAIL("Invalid SocID");
    }

    UT_LOG_DEBUG("Invoking dsHostTerm with no input parameters");
    ret = dsHostTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test validates the Host EDID sink of the L2 dsHost module
*
* This test function initializes the dsHost module, retrieves the Host EDID,
* validates the retrieved EDID and length, and then terminates the dsHost module.
* The purpose of this test is to ensure that the dsHost module correctly retrieves
* and validates the Host EDID.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to UT specification documentation [dsHost_L2_Low-Level_TestSpecification.md](../docs/pages/ds-host_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsHost_ValidateHostEDID_sink(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    unsigned char edid[EDID_MAX_DATA_SIZE];
    unsigned char edid_profile;
    char key_string[DS_HOST_KVP_SIZE];
    int i = 0;
    int length;

    UT_LOG_DEBUG("Invoking dsHostInit with no input parameters");
    ret = dsHostInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetHostEDID with valid pointers for edid and length");
    ret = dsGetHostEDID(edid, &length);
    UT_LOG_DEBUG("Return status: %d, EDID: %s, Length: %d", ret, edid, length);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);
    if (ret != dsERR_NONE)
    {
        UT_LOG_ERROR("dsGetHostEDID failed with status: %d", ret);
    }

    UT_ASSERT_KVP_EQUAL_PROFILE_UINT32(length, "dsHost.edidbytesLength");

    /* Checking only manufacture ID */
    for( i = 8; i < 9; i++)
    {
        snprintf(key_string, DS_HOST_KVP_SIZE, "dsHost.edidBytes.%d", i);
        edid_profile = UT_KVP_PROFILE_GET_UINT8(key_string);
        if(edid_profile != edid[i])
        {
            UT_FAIL("edid check failed");
            UT_LOG_ERROR("edid byte: %x, expected value: %x", edid[i], edid_profile);
            break;
        }
    }

    UT_LOG_DEBUG("Invoking dsHostTerm with no input parameters");
    ret = dsHostTerm();
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

int test_l2_dsHost_register(void)
{
    ut_kvp_status_t status;
    int32_t source_type = 0;

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsHost.Type", gDeviceType, TEST_DS_DEVICE_TYPE_SIZE);

    if (status == UT_KVP_STATUS_SUCCESS ) {
        if (!strncmp(gDeviceType, TEST_TYPE_SOURCE_VALUE, TEST_DS_DEVICE_TYPE_SIZE)) {
            // Create the test suite for source type
            pSuite = UT_add_suite("[L2 dsHost - Source]", NULL, NULL);
            if (pSuite == NULL) {
                UT_LOG_ERROR("Failed to create the test suite");
                return -1;
            }
            source_type = 1;
        }
        else if(!strncmp(gDeviceType, TEST_TYPE_SINK_VALUE, TEST_DS_DEVICE_TYPE_SIZE)) {
            // Create the test suite for sink type
            pSuite = UT_add_suite("[L2 dsHost - Sink]", NULL, NULL);
            if (pSuite == NULL) {
                UT_LOG_ERROR("Failed to create the test suite");
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

    // List of test function names and strings

    UT_add_test( pSuite, "L2_GetCPUTemperature", test_l2_dsHost_GetCPUTemperature);
    UT_add_test( pSuite, "L2_GetAndVerifySocID", test_l2_dsHost_GetAndVerifySocID);
    if(source_type == 0) {
        UT_add_test( pSuite, "L2_ValidateHostEDID", test_l2_dsHost_ValidateHostEDID_sink);
    }

    return 0;
}

/** @} */ // End of DS_Host_HALTEST_L2
/** @} */ // End of DS_Host_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK