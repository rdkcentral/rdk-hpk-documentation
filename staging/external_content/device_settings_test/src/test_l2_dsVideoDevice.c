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
 * @defgroup DS_VideoDevice_HALTEST Device Settings Video Device HAL Tests
 * @{
 */

/**
 * @defgroup DS_VideoDevice_HALTEST_L2 Device Settings Video Device HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L2 Tests for DS Video Device HAL :
 *
 * Level 2 unit test cases for all APIs of Device Settings Video Device HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-video-device_halSpec.md](../../docs/pages/ds-video-device_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l2_dsVideoDevice.c
 *
 */

#include <stdlib.h>
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_kvp.h>

#include "test_parse_configuration.h"
#include "dsVideoDevice.h"

static int gTestGroup = 2;
static int gTestID = 1;

/**
* @brief Test for setting and getting DFC source in dsVideoDevice
*
* This test case is designed to verify the functionality of setting and getting DFC source in dsVideoDevice.
* The test involves initializing the video device, getting the video device, setting the DFC,
* getting the DFC and terminating the video device. The test asserts that the DFC set is the same as the DFC get.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-device_L2-Low-Level_TestSpec.md](../docs/pages/ds-video-device_L2-Low-Level_TestSpec.md)
*/

void test_l2_dsVideoDevice_SetAndGetDFC_source(void)
{
    dsError_t retStatus;
    intptr_t handle;
    dsVideoZoom_t dfc_get;

    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_DEBUG("Invoking dsVideoDeviceInit");
    retStatus = dsVideoDeviceInit();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        UT_LOG_DEBUG("Invoking dsGetVideoDevice with index: %d", i);
        retStatus = dsGetVideoDevice(i, &handle);
        UT_LOG_DEBUG("Return status: %d, Handle: %ld", retStatus, handle);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
        if (retStatus != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetVideoDevice failed with status: %d", retStatus);
            continue;
        }
        for (int j = 0;j < gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs; j++)
        {
            UT_LOG_DEBUG("Invoking dsSetDFC with handle: %ld, dfc: %d", handle, gDSVideoDeviceConfiguration[i].SupportedDFCs[j]);
            retStatus = dsSetDFC(handle, gDSVideoDeviceConfiguration[i].SupportedDFCs[j]);
            UT_LOG_DEBUG("Return status: %d", retStatus);
            UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

            if (retStatus != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetDFC failed with status: %d", retStatus);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetDFC with handle: %ld", handle);
            retStatus = dsGetDFC(handle, &dfc_get);
            UT_LOG_DEBUG("Return status: %d, dfc_get: %d", retStatus, dfc_get);
            UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

            UT_ASSERT_EQUAL(gDSVideoDeviceConfiguration[i].SupportedDFCs[j], dfc_get);
        } /* for(j)*/
    } /* for(i)*/

    UT_LOG_DEBUG("Invoking dsVideoDeviceTerm");
    retStatus = dsVideoDeviceTerm();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the HDR capabilities of the video device
*
* In this test, the HDR capabilities of the video device are retrieved and checked
* against the expected capabilities. This is done to ensure that the video device
* is functioning as expected and can support the required HDR capabilities.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-device_L2-Low-Level_TestSpec.md](../docs/pages/ds-video-device_L2-Low-Level_TestSpec.md)
*/

void test_l2_dsVideoDevice_GetHDRCapabilities(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int capabilities;

    UT_LOG_DEBUG("Invoking dsVideoDeviceInit");
    ret = dsVideoDeviceInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        UT_LOG_DEBUG("Invoking dsGetVideoDevice with index: %d", i);
        ret = dsGetVideoDevice(i, &handle);
        UT_LOG_DEBUG("Return status: %d, Handle: %ld", ret, handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetVideoDevice failed with status: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetHDRCapabilities with handle: %ld", handle);
        ret = dsGetHDRCapabilities(handle, &capabilities);
        UT_LOG_DEBUG("Return status: %d, Capabilities: %d", ret, capabilities);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        UT_ASSERT_EQUAL(capabilities, gDSVideoDeviceConfiguration[i].HDRCapabilities);
    } /* for(i)*/

    UT_LOG_DEBUG("Invoking dsVideoDeviceTerm");
    ret = dsVideoDeviceTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test checks the functionality of getting supported video coding formats for a video device
*
* This test initializes the video device, gets the video device for each index,
* checks the supported video coding formats for each device and finally terminates
* the video device. The test ensures that all the API calls return the expected status
* and the supported video coding formats are as expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-device_L2-Low-Level_TestSpec.md](../docs/pages/ds-video-device_L2-Low-Level_TestSpec.md)
*/

void test_l2_dsVideoDevice_GetSupportedVideoCodingFormats(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t retStatus;
    intptr_t handle;
    unsigned int supported_formats;

    UT_LOG_DEBUG("Invoking dsVideoDeviceInit()");
    retStatus = dsVideoDeviceInit();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        UT_LOG_DEBUG("Invoking dsGetVideoDevice() with index: %d", i);
        retStatus = dsGetVideoDevice(i, &handle);
        UT_LOG_DEBUG("Return status: %d, Handle: %ld", retStatus, handle);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
        UT_ASSERT_PTR_NOT_NULL(handle);
        if(retStatus != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetVideoDevice failed with error %d", retStatus);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetSupportedVideoCodingFormats() with handle: %ld", handle);
        retStatus = dsGetSupportedVideoCodingFormats(handle, &supported_formats);
        UT_LOG_DEBUG("Return status: %d, Supported formats: %u", retStatus, supported_formats);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

        UT_ASSERT_EQUAL(supported_formats, gDSVideoDeviceConfiguration[i].SupportedVideoCodingFormats);
    } /* for(i)*/

    UT_LOG_DEBUG("Invoking dsVideoDeviceTerm()");
    retStatus = dsVideoDeviceTerm();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for getting video codec information from a source
*
* This test verifies if the dsGetVideoCodecInfo function can successfully
* retrieve video codec information from a source. It checks if the function
* returns the correct status and if the retrieved information matches the expected values.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-device_L2-Low-Level_TestSpec.md](../docs/pages/ds-video-device_L2-Low-Level_TestSpec.md)
*/

void test_l2_dsVideoDevice_GetVideoCodecInfo_source(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t retStatus;
    intptr_t handle;
    dsVideoCodecInfo_t info;
    dsVideoCodingFormat_t codec = dsVIDEO_CODEC_MPEGHPART2;
    int j = 1;

    UT_LOG_DEBUG("Invoking dsVideoDeviceInit");
    retStatus = dsVideoDeviceInit();
    UT_LOG_DEBUG("dsVideoDeviceInit return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        UT_LOG_DEBUG("Invoking dsGetVideoDevice with index: %d", i);
        retStatus = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
        UT_LOG_DEBUG("dsGetVideoDevice return status: %d, handle: %ld", retStatus, handle);
        if(retStatus != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetVideoDevice failed with error %d", retStatus);
            continue;
        }

        for(codec = dsVIDEO_CODEC_MPEGHPART2 ; codec < dsVIDEO_CODEC_MAX; )
        {
            if(!(gDSVideoDeviceConfiguration[i].SupportedVideoCodingFormats & codec))
            {
                continue;
            }
            UT_LOG_DEBUG("Invoking dsGetVideoCodecInfo with handle: %ld, codec: %d", handle, codec);
            retStatus = dsGetVideoCodecInfo(handle, codec, &info);
            UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
            UT_LOG_DEBUG("dsGetVideoCodecInfo return status: %d, num_entries: %d", retStatus, info.num_entries);

            UT_ASSERT_EQUAL(info.num_entries, gDSVideoDeviceConfiguration[i].num_codec_entries);
            /* for now hal get info supports only MPEGHPART2 */
            if(codec == dsVIDEO_CODEC_MPEGHPART2)
            {
                UT_LOG_DEBUG("Profile: %d", info.entries->profile);
                UT_ASSERT_EQUAL(info.entries->profile,gDSVideoDeviceConfiguration[i].profile);
                //TODO : kvp profile support for float type is not available now
                //UT_ASSERT_EQUAL(info.entries->level,gDSVideoDeviceConfiguration[i].level);
            }
            codec = (0x01 << (j++));
        } /* for(codec)*/
    } /* for(i)*/
    UT_LOG_DEBUG("Invoking dsVideoDeviceTerm");
    retStatus = dsVideoDeviceTerm();
    UT_LOG_DEBUG("dsVideoDeviceTerm return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the setting and getting of display framerate for dsVideoDevice
*
* This test case is designed to validate the functionality of dsSetDisplayframerate and
* dsGetCurrentDisplayframerate APIs. The test involves setting a display framerate for
* a video device and then getting the current display framerate to verify if the set value
* is correctly retrieved.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-device_L2-Low-Level_TestSpec.md](../docs/pages/ds-video-device_L2-Low-Level_TestSpec.md)
*/
void test_l2_dsVideoDevice_SetAndVerifyDisplayframerate_sink(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    char getframerate[DS_VIDEO_DEVICE_MAX_FRAMERATE_LEN] = {0};

    UT_LOG_DEBUG("Invoking dsVideoDeviceInit");
    ret = dsVideoDeviceInit();
    UT_LOG_DEBUG("dsVideoDeviceInit return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        UT_LOG_DEBUG("Invoking dsGetVideoDevice with index :%d",i);
        ret = dsGetVideoDevice(i, &handle);
        UT_LOG_DEBUG("dsGetVideoDevice return status: %d, handle: %ld", ret, handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if(ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetVideoDevice failed with error %d", ret);
            continue;
        }

        for (int j=0;j<gDSVideoDeviceConfiguration[i].NoOfSupportedDFR;j++)
        {
            UT_LOG_DEBUG("Invoking dsSetDisplayframerate with handle: %ld", handle);
            ret = dsSetDisplayframerate(handle, gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j]);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_LOG_DEBUG("dsSetDisplayframerate return status: %d", ret);
            if(ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsGetVideoDevice failed with error %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetCurrentDisplayframerate with handle: %ld", handle);
            ret = dsGetCurrentDisplayframerate(handle, getframerate);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_LOG_DEBUG("dsGetCurrentDisplayframerate return status: %d, framerate: %s", ret, getframerate);
            UT_ASSERT_STRING_EQUAL(gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j], getframerate);
        } /* for(j) */
    } /* for(i) */

    UT_LOG_DEBUG("Invoking dsVideoDeviceTerm");
    ret = dsVideoDeviceTerm();
    UT_LOG_DEBUG("dsVideoDeviceTerm return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the setting and getting of display FRF mode for dsVideoDevice
*
* This test case is designed to validate the functionality of dsSetFRFMode and
* dsGetFRFMode APIs. The test involves setting a FRF Mode for
* a video device and then getting the current FRF Mode to verify if the set value
* is correctly retrieved.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 006@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-device_L2-Low-Level_TestSpec.md](../docs/pages/ds-video-device_L2-Low-Level_TestSpec.md)
*/
void test_l2_dsVideoDevice_SetAndVerifyFRFMode_sink(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int FRFMode;

    UT_LOG_DEBUG("Invoking dsVideoDeviceInit");
    ret = dsVideoDeviceInit();
    UT_LOG_DEBUG("dsVideoDeviceInit return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        UT_LOG_DEBUG("Invoking dsGetVideoDevice with index :%d",i);
        ret = dsGetVideoDevice(i, &handle);
        UT_LOG_DEBUG("dsGetVideoDevice return status: %d, handle: %ld", ret, handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if(ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetVideoDevice failed with error %d", ret);
            continue;
        }
        //FRF Mode is 0 or disable 1 for Enable.Loop through twice to set 0 and 1
        for (int j=0;j<2;j++)
        {
            UT_LOG_DEBUG("Invoking dsSetFRFMode with handle: %ld", handle);
            ret = dsSetFRFMode(handle, j);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_LOG_DEBUG("dsSetFRFMode return status: %d", ret);
            if(ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsGetVideoDevice failed with error %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetFRFMode with handle: %ld", handle);
            ret = dsGetFRFMode(handle, &FRFMode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_LOG_DEBUG("dsGetFRFMode return status: %d, framerate: %d", ret, FRFMode);
            UT_ASSERT_EQUAL(j,FRFMode);
        } /* for(j) */
    } /* for(i) */

    UT_LOG_DEBUG("Invoking dsVideoDeviceTerm");
    ret = dsVideoDeviceTerm();
    UT_LOG_DEBUG("dsVideoDeviceTerm return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l2_dsVideoDevice_register(void)
{
    if(gSourceType == 0) {
        // Create the test suite
        pSuite = UT_add_suite("[L2 dsVideoDevice - Sink]", NULL, NULL);
    }
    else
    {
        // Create the test suite
        pSuite = UT_add_suite("[L2 dsVideoDevice - Source]", NULL, NULL);
    }

    if (pSuite == NULL)
    {
        return -1;
    }

    UT_add_test( pSuite, "L2_GetHDRCapabilities", test_l2_dsVideoDevice_GetHDRCapabilities);
    UT_add_test( pSuite, "L2_GetSupportedVideoCodingFormats", test_l2_dsVideoDevice_GetSupportedVideoCodingFormats);

    if(gSourceType == 0) {
        UT_add_test( pSuite, "L2_SetAndVerifyDisplayframerate_Sink", test_l2_dsVideoDevice_SetAndVerifyDisplayframerate_sink);
        UT_add_test( pSuite, "L2_SetAndVerifyFRFMode_Sink", test_l2_dsVideoDevice_SetAndVerifyFRFMode_sink);
    }

    if(gSourceType == 1) {
        UT_add_test( pSuite, "L2_SetAndGetDFC_source", test_l2_dsVideoDevice_SetAndGetDFC_source);
        UT_add_test( pSuite, "L2_GetVideoCodecInfo_source", test_l2_dsVideoDevice_GetVideoCodecInfo_source);
    }

    return 0;
}

/** @} */ // End of DS_VideoDevice_HALTEST_L2
/** @} */ // End of DS_VideoDevice_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK