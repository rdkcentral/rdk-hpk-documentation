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
 * @defgroup DS_VideoPort_HALTEST Device Settings Video Port HAL Tests
 * @{
 */

/**
 * @defgroup DS_VideoPort_HALTEST_L2 Device Settings Video Port HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for DS Video Port HAL :
 *
 * Level 2 unit test cases for all APIs of Device Settings Video Port HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [ds-video-port_halSpec.md](../../docs/pages/ds-video-port_halSpec.md)
 *
 * @endparblock
 */


/**
 * @file test_l2_dsVideoPort.c
 *
 */

#include <stdlib.h>
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_kvp.h>

#include "test_parse_configuration.h"
#include "dsVideoPort.h"

static int gTestGroup = 2;
static int gTestID = 1;

/**
* @brief This test aims to enable disabled video ports and verify their status
*
* In this test, we initialize the video port, get the video port for supported type, check if it's enabled.
* If not, we enable it and verify if it's enabled successfully.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_EnableDisabledVideoPorts(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    bool enabled    = false;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsEnableVideoPort() with handle: %ld and enabled: true", handle);
        ret = dsEnableVideoPort(handle, true);
        UT_LOG_DEBUG("Return status: %d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        UT_LOG_DEBUG("Invoking dsIsVideoPortEnabled() with handle: %ld", handle);
        ret = dsIsVideoPortEnabled(handle, &enabled);
        UT_LOG_DEBUG("Return status: %d, Enabled: %d", ret, enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(enabled, true);

        UT_LOG_DEBUG("Invoking dsEnableVideoPort() with handle: %ld and enabled: true", handle);
        ret = dsEnableVideoPort(handle, false);
        UT_LOG_DEBUG("Return status: %d", ret);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        UT_LOG_DEBUG("Invoking dsIsVideoPortEnabled() with handle: %ld", handle);
        ret = dsIsVideoPortEnabled(handle, &enabled);
        UT_LOG_DEBUG("Return status: %d, Enabled: %d", ret, enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(enabled, false);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test verifies the display and port status of the L2 dsVideoPort
*
* This test function initializes the video port, gets the video port for each type,
* checks if the display is connected and if the video port is active, and finally terminates the video port.
* The test is designed to ensure that the video port functionalities are working as expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_VerifyDisplayAndPortStatus(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    bool connected  = false;
    bool active     = false;

    UT_LOG_DEBUG("Invoking dsVideoPortInit");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsIsDisplayConnected with handle: %ld", handle);
        ret = dsIsDisplayConnected(handle, &connected);
        UT_LOG_DEBUG("Return status: %d, Connected: %d", ret, connected);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        /* check for source */
        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(connected, false);
        }
        else if(gSourceType == 0) {
            if(gDSVideoPortConfiguration[port].typeid == dsVIDEOPORT_TYPE_INTERNAL) {
                UT_ASSERT_EQUAL(connected, true);
            }
            else {
                UT_ASSERT_EQUAL(connected, false);
            }
        }

        UT_LOG_DEBUG("Invoking dsIsVideoPortActive with handle: %ld", handle);
        ret = dsIsVideoPortActive(handle, &active);
        UT_LOG_DEBUG("Return status: %d, Active: %d", ret, active);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(connected, false);
        }
        else if(gSourceType == 0) {
            if(gDSVideoPortConfiguration[port].typeid == dsVIDEOPORT_TYPE_INTERNAL) {
                UT_ASSERT_EQUAL(connected, true);
            }
            else {
                UT_ASSERT_EQUAL(connected, false);
            }
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to retrieve and verify the surround mode capabilities of the video port
*
* In this test, the dsVideoPortInit() function is invoked to initialize the video port.
* Then, the dsGetVideoPort() function is called with dsVIDEOPORT_TYPE_INTERNAL and index 0 to get the video port.
* The dsIsDisplaySurround() function is then invoked with the obtained handle to check if the display is in surround mode.
* The result is then compared with the expected value. Finally, the dsVideoPortTerm() function is invoked to terminate the video port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_RetrieveAndVerifySurroundModeCapabilities(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    bool surround   = false;
    int surround_mode = 0;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);
    UT_LOG_DEBUG("dsVideoPortInit() returned %d", ret);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsIsDisplaySurround() with handle %ld", handle);
        ret = dsIsDisplaySurround(handle, &surround);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_DEBUG("dsIsDisplaySurround() returned %d and surround %d", ret, surround);

        if (gSourceType == 1) {
            /*check for source device */
            UT_ASSERT_EQUAL(surround, false);
            UT_LOG_DEBUG("Invoking dsGetSurroundMode() with valid handle");
            ret = dsGetSurroundMode(handle, &surround_mode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_LOG_DEBUG("Surround mode: %d and return status: %d", surround_mode, ret);

            UT_ASSERT_EQUAL(surround_mode, dsSURROUNDMODE_NONE);
        }
        else if(gSourceType == 0){
            /*check for sink device*/
            UT_ASSERT_EQUAL(surround, gDSVideoPortConfiguration[port].DisplaySurround);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("dsVideoPortTerm() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for setting and getting the resolution of a video Port
*
* This test checks if the dsSetResolution and dsGetResolution functions work as expected.
* It sets the resolution of a video port and then gets the resolution to verify if the set resolution is correctly applied.
* This is done for all possible combinations of pixel resolution, aspect ratio, stereoscopic mode, and frame rate.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_SetAndGetResolution_source(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        for (int i = 0; i < gDSVideoPortConfiguration[port].numSupportedResolutions; i++) {
            dsVideoPortResolution_t getResolution;
            dsVideoPortResolution_t *pSetResolution = &gDSVideoPortConfiguration[port].supportedResolutions[i];

            UT_LOG_DEBUG("Invoking dsSetResolution() with handle and setResolution");

            ret = dsSetResolution(handle, pSetResolution);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetResolution() failed with status: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetResolution() with handle and getResolution");
            ret = dsGetResolution(handle, &getResolution);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsGetResolution() failed with status: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Comparing setResolution and getResolution");
            UT_LOG_DEBUG("getResolution.pixelResolution= %d ",getResolution.pixelResolution);
            UT_LOG_DEBUG("getResolution.aspectRatio= %d ",getResolution.aspectRatio);
            UT_LOG_DEBUG("getResolution.stereoScopicMode= %d ",getResolution.stereoScopicMode);
            UT_LOG_DEBUG("getResolution.frameRate= %d ",getResolution.frameRate);
            UT_LOG_DEBUG("getResolution.interlaced= %d ",getResolution.interlaced);

            UT_ASSERT_STRING_EQUAL(pSetResolution->name, getResolution.name);
            UT_ASSERT_EQUAL(pSetResolution->pixelResolution, getResolution.pixelResolution);
            UT_ASSERT_EQUAL(pSetResolution->aspectRatio, getResolution.aspectRatio);
            UT_ASSERT_EQUAL(pSetResolution->stereoScopicMode, getResolution.stereoScopicMode);
            UT_ASSERT_EQUAL(pSetResolution->frameRate, getResolution.frameRate);
            UT_ASSERT_EQUAL(pSetResolution->interlaced, getResolution.interlaced);
        } /* for (i) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the supported TV resolutions
*
* This test verifies if the dsVideoPort API correctly identifies and returns the supported TV resolutions.
* The test is crucial to ensure the correct functioning of the video port in different resolution settings.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_VerifySupportedTvResolutions(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    int resolutions;

    UT_LOG_DEBUG("Invoking dsVideoPortInit");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Returned status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsSupportedTvResolutions with handle: %ld", handle);
        ret = dsSupportedTvResolutions(handle, &resolutions);
        UT_LOG_DEBUG("Returned status: %d, resolutions: %d", ret, resolutions);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(resolutions, 0);
        }
        else if(gSourceType == 0) {
            UT_ASSERT_EQUAL(resolutions, gDSVideoPortConfiguration[port].Supported_tv_resolutions_capabilities);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Returned status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for getting HDR capabilities
*
* This test verifies if the dsGetTVHDRCapabilities() function can successfully get the HDR capabilities.
* The test first initializes the video port, gets the video port with dsVIDEOPORT_TYPE_INTERNAL and index 0,
* then gets the HDR capabilities. It asserts that all these operations return dsERR_NONE,
* and that the obtained capabilities match the expected value.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 06@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_GetHDRCapabilities(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    int capabilities;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Returned status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetTVHDRCapabilities() with handle: %ld", handle);
        ret = dsGetTVHDRCapabilities(handle, &capabilities);
        UT_LOG_DEBUG("Returned status: %d, capabilities: %d", ret, capabilities);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(capabilities, dsHDRSTANDARD_SDR);
        }
        else if(gSourceType == 0) {
            UT_ASSERT_EQUAL(capabilities, gDSVideoPortConfiguration[port].hdr_capabilities);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Returned status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test checks the HDCP status of the video port
*
* This test initializes the video port, gets the video port handle for each type, checks the
* HDCP status and finally terminates the video port. The test is designed to verify if the
* HDCP status is authenticated for each video port type.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 007@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_GetHDCPStatus(void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    dsHdcpStatus_t status;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetHDCPStatus() with handle: %ld", handle);
        ret = dsGetHDCPStatus(handle, &status);
        if (ret != dsERR_NONE)
        {
            UT_LOG_ERROR("dsGetHDCPStatus() failed with error: %d", ret);
            continue;
        }
        UT_LOG_DEBUG("Return status: %d, HDCP Status: %d", ret, status);

        /* check for source */
        if(gSourceType == 1) {
            UT_ASSERT_TRUE(status == dsHDCP_STATUS_UNPOWERED || status == dsHDCP_STATUS_PORTDISABLED);
            if (status != dsHDCP_STATUS_UNPOWERED || status != dsHDCP_STATUS_PORTDISABLED) {
                UT_LOG_ERROR("HDCP status is not unpowered or portdisabled. Status: %d", status);
            }
        }
        else if(gSourceType == 0) {
            /*check for sink*/
            UT_ASSERT_EQUAL(status, dsHDCP_STATUS_AUTHENTICATED);
            if (status != dsHDCP_STATUS_AUTHENTICATED) {
                UT_LOG_ERROR("HDCP status is not authenticated. Status: %d", status);
            }
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the HDCP Protocol Status
*
* This test case verifies the HDCP Protocol Status by invoking the dsVideoPortInit, dsGetVideoPort, dsGetHDCPProtocol and dsVideoPortTerm APIs.
* The HDCP protocol version obtained from the API is compared with the one retrieved from the yaml file.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 008@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_VerifyHDCPProtocolStatus(void)
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    dsHdcpProtocolVersion_t protocolVersion;

    UT_LOG_DEBUG("Invoking dsVideoPortInit");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetHDCPProtocol with handle: %ld", handle);
        ret = dsGetHDCPProtocol(handle, &protocolVersion);
        UT_LOG_DEBUG("Return status: %d, Protocol Version: %d", ret, protocolVersion);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(protocolVersion, gDSVideoPortConfiguration[port].hdcp_protocol_version);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to set and get the HDMI preference for a video port
*
* In this test, the HDMI preference for each type of video port is set and then
* retrieved to verify if the set operation was successful. This is done for all types of
* HDCP protocol versions. The test is important to ensure the correct functioning of the
* HDMI preference setting and getting operations for different types of video ports and HDCP protocol versions.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 009@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_SetAndGetHdmiPreference(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    dsHdcpProtocolVersion_t protocolVersion, hdcpCurrentProtocolGet;

    UT_LOG_DEBUG("Invoking dsVideoPortInit");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetHDCPProtocol with handle: %ld", handle);
        ret = dsGetHDCPProtocol(handle, &protocolVersion);
        UT_LOG_DEBUG("Return status: %d, Protocol Version: %d", ret, protocolVersion);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(protocolVersion, gDSVideoPortConfiguration[port].hdcp_protocol_version);

        for(dsHdcpProtocolVersion_t version = dsHDCP_VERSION_1X; version <= protocolVersion; version++) {
            UT_LOG_DEBUG("Invoking dsSetHdmiPreference with handle: %ld and hdcpCurrentProtocol: %d", handle, version);
            ret = dsSetHdmiPreference(handle, &version);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetHdmiPreference failed with error: %d", ret);
            }

            UT_LOG_DEBUG("Invoking dsGetHdmiPreference with handle: %ld", handle);
            ret = dsGetHdmiPreference(handle, &hdcpCurrentProtocolGet);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetHdmiPreference failed with error: %d", ret);
            }
            UT_LOG_DEBUG("HDCP protocol version set: %d, HDCP protocol version get: %d", version, hdcpCurrentProtocolGet);
            UT_ASSERT_EQUAL(version, hdcpCurrentProtocolGet);
        } /* for (version) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of dsVideoPort_GetColorSpace
*
* This test is designed to check the correct operation of the dsVideoPort_GetColorSpace function.
* It does this by initializing the video port, getting the video port with type as dsVIDEOPORT_TYPE_INTERNAL and index as 0,
* then getting the color space with the handle obtained from the previous step. The test asserts that all these operations return dsERR_NONE,
* indicating successful operation. The color space obtained is then compared with the expected value.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 010@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_GetColorSpace(void)
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    dsDisplayColorSpace_t color_space;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("dsVideoPortInit() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetColorSpace() with handle obtained from previous step");
        ret = dsGetColorSpace(handle, &color_space);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_DEBUG("dsGetColorSpace() returned color_space %d and status %d", color_space, ret);

        UT_ASSERT_EQUAL(color_space, gDSVideoPortConfiguration[port].colorspaces);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("dsVideoPortTerm() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to check the color depth capabilities of the video port
*
* This test checks the color depth capabilities of the video port by invoking the dsVideoPortInit,
* dsGetVideoPort, dsColorDepthCapabilities, and dsVideoPortTerm functions.
* The test verifies that all these functions return the expected status and that the color depth capability matches the expected value.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 011@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_CheckColorDepthCapabilities_source(void)
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    unsigned int colorDepthCapability;

    UT_LOG_DEBUG("Invoking dsVideoPortInit");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsColorDepthCapabilities with handle");
        ret = dsColorDepthCapabilities(handle, &colorDepthCapability);
        UT_LOG_DEBUG("Return status: %d, Color Depth Capability: %u", ret, colorDepthCapability);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(colorDepthCapability, dsDISPLAY_COLORDEPTH_8BIT);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of the dsVideoPort_GetColorDepth
*
* In this test, the dsVideoPort_GetColorDepth is tested to ensure it correctly retrieves the color depth of the video port.
* The test involves initializing the video port, getting the video port with type=dsVIDEOPORT_TYPE_INTERNAL and index=0, getting the color depth,
* and then terminating the video port. The test asserts that all these operations return dsERR_NONE, indicating successful execution.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 012@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_GetColorDepth(void)
{
    gTestID = 12;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    unsigned int color_depth;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("dsVideoPortInit() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetColorDepth() with handle=%ld", handle);
        ret = dsGetColorDepth(handle, &color_depth);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_DEBUG("dsGetColorDepth() returned color_depth=%u and status=%d", color_depth, ret);

        UT_ASSERT_EQUAL(color_depth, gDSvideoPort_color_depth);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);
    UT_LOG_DEBUG("dsVideoPortTerm() returned %d", ret);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for setting and getting the preferred color depth of a video port
*
* This test verifies if the dsSetPreferredColorDepth and dsGetPreferredColorDepth functions work as expected.
* It sets a preferred color depth for a video port and then retrieves it to check if the set and retrieved values match.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 013@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/
void test_l2_dsVideoPort_SetAndGetPreferredColorDepth_source(void)
{
    gTestID = 13;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    dsDisplayColorDepth_t colorDepthSet, colorDepthGet;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("dsVideoPortInit() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        for (int j = dsDISPLAY_COLORDEPTH_8BIT; j <= dsDISPLAY_COLORDEPTH_AUTO; j++) {
            colorDepthSet = j;
            UT_LOG_DEBUG("Invoking dsSetPreferredColorDepth() with handle %ld and colorDepth %d", handle, colorDepthSet);
            ret = dsSetPreferredColorDepth(handle, colorDepthSet);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsSetPreferredColorDepth() failed with error %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetPreferredColorDepth() with handle %ld", handle);
            ret = dsGetPreferredColorDepth(handle, &colorDepthGet);
            if (ret != dsERR_NONE)
            {
                UT_LOG_ERROR("dsGetPreferredColorDepth() failed with error %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Color depth set: %d, Color depth got: %d", colorDepthSet, colorDepthGet);
            UT_ASSERT_EQUAL(colorDepthGet, dsDISPLAY_COLORDEPTH_UNKNOWN);
        } /* for (j) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("dsVideoPortTerm() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of dsVideoPort_GetQuantizationRang
*
* This test case is designed to test the dsVideoPort_GetQuantizationRange function.
* It checks if the function correctly initializes the video port, gets the video port with dsVIDEOPORT_TYPE_INTERNAL as type and 0 as index,
* gets the quantization range with the handle obtained from the previous step, and finally terminates the video port.
* The test asserts that all these operations return dsERR_NONE, indicating successful execution.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 014@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_GetQuantizationRange(void)
{
    gTestID = 14;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    dsDisplayQuantizationRange_t quantization_range;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("dsVideoPortInit() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetQuantizationRange() with handle obtained from previous step");
        ret = dsGetQuantizationRange(handle, &quantization_range);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_DEBUG("Quantization Range: %d", quantization_range);

        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(quantization_range, dsDISPLAY_QUANTIZATIONRANGE_UNKNOWN);
        }
        else if(gSourceType == 0) {
            UT_ASSERT_EQUAL(quantization_range, gDSVideoPortConfiguration[port].quantization_ranges);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
    UT_LOG_DEBUG("dsVideoPortTerm() returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the functionality of dsVideoPort_GetMatrixCoefficients API
*
* This test case verifies the functionality of dsVideoPort_GetMatrixCoefficients API.
* The test case initializes the video port, gets the video port handle, retrieves the matrix coefficients and verifies it with the expected value.
* The test case also ensures that the API returns the correct status codes at each step.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 015@n
*
* **Test Procedure:**
* Refer to Test specification documentation [ds-video-port_L2_Low-Level_TestSpecification.md](../docs/pages/ds-video-port_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsVideoPort_GetMatrixCoefficients(void)
{
    gTestID = 15;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    dsDisplayMatrixCoefficients_t matrix_coefficients;

    UT_LOG_DEBUG("Invoking dsVideoPortInit()");
    ret = dsVideoPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSvideoPort_NumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetVideoPort with type: %d and index: %d",
                           gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index);

        ret = dsGetVideoPort(gDSVideoPortConfiguration[port].typeid, gDSVideoPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetVideoPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetMatrixCoefficients() with handle obtained from previous step");
        ret = dsGetMatrixCoefficients(handle, &matrix_coefficients);
        UT_LOG_DEBUG("Return status: %d, Matrix Coefficients: %d", ret, matrix_coefficients);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(matrix_coefficients, dsDISPLAY_MATRIXCOEFFICIENT_UNKNOWN);
        }
        else if(gSourceType == 0) {
            UT_ASSERT_EQUAL(matrix_coefficients, gDSVideoPortConfiguration[port].matrix_coefficients);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsVideoPortTerm()");
    ret = dsVideoPortTerm();
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

int test_l2_dsVideoPort_register(void)
{
    // Create the test suite
    if(gSourceType == 1) {
        pSuite = UT_add_suite("[L2 dsVideoPort - Source]", NULL, NULL);
    }
    else {
        pSuite = UT_add_suite("[L2 dsVideoPort - Sink]", NULL, NULL);
    }
    if (pSuite == NULL) {
        return -1;
    }

    UT_add_test( pSuite, "L2_EnableDisabledVideoPorts", test_l2_dsVideoPort_EnableDisabledVideoPorts);
    UT_add_test( pSuite, "L2_VerifyDisplayAndPortStatus", test_l2_dsVideoPort_VerifyDisplayAndPortStatus);
    UT_add_test( pSuite, "L2_RetrieveAndVerifySurroundModeCapabilities", test_l2_dsVideoPort_RetrieveAndVerifySurroundModeCapabilities);
    UT_add_test( pSuite, "L2_VerifySupportedTvResolutions", test_l2_dsVideoPort_VerifySupportedTvResolutions);
    UT_add_test( pSuite, "L2_GetHDRCapabilities", test_l2_dsVideoPort_GetHDRCapabilities);
    UT_add_test( pSuite, "L2_GetHDCPStatus", test_l2_dsVideoPort_GetHDCPStatus);
    UT_add_test( pSuite, "L2_VerifyHDCPProtocolStatus", test_l2_dsVideoPort_VerifyHDCPProtocolStatus);
    UT_add_test( pSuite, "L2_SetAndGetHdmiPreference", test_l2_dsVideoPort_SetAndGetHdmiPreference);
    UT_add_test( pSuite, "L2_GetColorSpace", test_l2_dsVideoPort_GetColorSpace);
    UT_add_test( pSuite, "L2_GetColorDepth", test_l2_dsVideoPort_GetColorDepth);
    UT_add_test( pSuite, "L2_GetQuantizationRange", test_l2_dsVideoPort_GetQuantizationRange);
    UT_add_test( pSuite, "L2_GetMatrixCoefficients", test_l2_dsVideoPort_GetMatrixCoefficients);
    if(gSourceType == 1) {
        // add the test suite for source type
        UT_add_test( pSuite, "L2_SetAndGetResolution_source", test_l2_dsVideoPort_SetAndGetResolution_source);
        UT_add_test( pSuite, "L2_SetAndGetPreferredColorDepth_source", test_l2_dsVideoPort_SetAndGetPreferredColorDepth_source);
        UT_add_test( pSuite, "L2_CheckColorDepthCapabilities_source", test_l2_dsVideoPort_CheckColorDepthCapabilities_source);
    }

    return 0;
}

/** @} */ // End of DS_VideoPort_HALTEST_L2
/** @} */ // End of DS_VideoPort_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
