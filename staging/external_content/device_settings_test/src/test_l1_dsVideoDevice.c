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
 * @defgroup DS_VideoDevice_HALTEST Device Settings Video Device HAL Tests
 * @{
 */

/**
 * @defgroup DS_VideoDevice_HALTEST_L1 Device Settings Video Device HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for DS Video Device HAL :
 *
 * Level 1 unit test cases for all APIs of Device Settings Video Device HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-video-device_halSpec.md](../../docs/pages/ds-video-device_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l1_dsVideoDevice.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "dsVideoDevice.h"
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
 * @brief Ensure dsVideoDeviceInit() correctly initializes all video devices during positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsVideoDeviceInit() to initialize all video devices | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|De-initialize the video devices using dsVideoDeviceTerm() for cleanup | | dsERR_NONE | Clean up after test |
 * |03|Call dsVideoDeviceInit() to initialize all video devices | | dsERR_NONE | Video devices should be initialized successfully |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() for cleanup | | dsERR_NONE | Clean up after test |
 *
 */
void test_l1_dsVideoDevice_positive_dsVideoDeviceInit(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize video devices
    int result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: De-initialize the video devices for cleanup
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Initialize video devices again
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 04: De-initialize the video devices for cleanup
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Ensure dsVideoDeviceInit() returns correct error codes during negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsVideoDeviceInit() to initialize all video devices | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Call dsVideoDeviceInit() again without terminating the previous initialization | | dsERR_ALREADY_INITIALIZED | Should not allow double initialization |
 * |03|De-initialize the video devices using dsVideoDeviceTerm() for cleanup | | dsERR_NONE | Clean up after test |
 *
 * @note The return value dsERR_GENERAL may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsVideoDeviceInit(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize video devices
    int result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Try initializing again without terminating
    result = dsVideoDeviceInit();
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_ALREADY_INITIALIZED, dsERR_NONE);

    // Step 03: De-initialize the video devices for cleanup
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Ensure dsVideoDeviceTerm() correctly de-initializes all video devices during positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsVideoDeviceInit() to initialize all video devices | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|De-initialize the video devices using dsVideoDeviceTerm() for cleanup | | dsERR_NONE | Clean up after test |
 * |03|Call dsVideoDeviceInit() to initialize all video devices | | dsERR_NONE | Video devices should be initialized successfully |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() for cleanup | | dsERR_NONE | Clean up after test |
 *
 */
void test_l1_dsVideoDevice_positive_dsVideoDeviceTerm(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize video devices
    int result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: De-initialize the video devices for cleanup
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Initialize video devices again
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 04: De-initialize the video devices for cleanup
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Ensure dsVideoDeviceTerm() returns correct error codes during negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsVideoDeviceTerm() without prior initialization of video devices | | dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Call dsVideoDeviceTerm() to de-initialize all video devices | | dsERR_NONE | Video devices should be de-initialized successfully |
 * |04|Call dsVideoDeviceTerm() again without re-initialization | | dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsVideoDeviceTerm(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Call de-initialization without prior initialization
    int result = dsVideoDeviceTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 04: Call de-initialization again without re-initialization
    result = dsVideoDeviceTerm();
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetVideoDevice() correctly fetches the handle for the video device in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Call dsGetVideoDevice() using a pointer to retrieve the handle | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetVideoDevice(void)
{
    gTestID = 5;
    intptr_t handle = -1;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    // Step 01: Initialize video devices
    int result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Get the video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
    }

    // Step 03: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetVideoDevice() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetVideoDevice() without prior initialization of video devices|index=i, int=*handle | dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Call dsGetVideoDevice() with index > 0 and a valid pointer to retrieve handle | int=-1, int=*handle | dsERR_INVALID_PARAM | Should report invalid parameter since index should always be 0 |
 * |04|Call dsGetVideoDevice() with index set to 0 but a null pointer for handle | int=index, NULL | dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |06|Call dsGetVideoDevice() after prior termination of video devices| int=index, int=*handle | dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetVideoDevice(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;

    // Step 01: Get the video device handle without prior initialization
    intptr_t handle = -1;
    result = dsGetVideoDevice(0, &handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Get the video device handle with invalid index
    result = dsGetVideoDevice(-1, &handle);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

    // Step 04: Get the video device handle with valid index but null pointer for handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 06: Get the video device handle after prior termination
    result = dsGetVideoDevice(0, &handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsSetDFC() correctly sets the screen zoom mode in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Set the DFC mode by looping through the supported zoom modes | int=handle | dsERR_NONE | Should successfully set the DFC mode |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsSetDFC (void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t result;
    intptr_t handle = -1;

    // 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // 02: Obtain video device handle
    for (int i = 0; i < gDSvideoDevice_NumVideoDevices; ++i) {
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
        // 03: Set DFC mode with various zoom modes
        for (int j = 0; j < gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs; ++j)
        {
            result = dsSetDFC(handle, gDSVideoDeviceConfiguration[i].SupportedDFCs[j]);
            if (gSourceType == 1)
            {
                UT_ASSERT_EQUAL(result, dsERR_NONE);
            }
            else if (gSourceType == 0)
            {
                UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
            }
        }
    }

    // 04: De-initialize video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsSetDFC() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsSetDFC() without prior initialization of video devices | int=handle, dsVideoZoom_t=dsVIDEO_ZOOM_NONE | dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsSetDFC() with an invalid handle | int=-1, dsVideoZoom_t=dsVIDEO_ZOOM_NONE | dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsSetDFC() with an invalid zoom mode | int=handle, dsVideoZoom_t=dsVIDEO_ZOOM_MAX | dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsSetDFC() after termination of video devices | int=handle, dsVideoZoom_t=dsVIDEO_ZOOM_NONE | dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsSetDFC (void)
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t result;
    intptr_t handle = -1;

    // 01: Call dsSetDFC() without initialization
    result = dsSetDFC(-1, gDSVideoDeviceConfiguration[0].DefaultDFC);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // 04: Call dsSetDFC() with an invalid handle
        result = dsSetDFC(-1, gDSVideoDeviceConfiguration[i].DefaultDFC);
        if (gSourceType == 1)
        {
            UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
        }
        else if (gSourceType == 0)
        {
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }

        // 05: Call dsSetDFC() with an invalid zoom mode
        int32_t numSupportedDFCs = gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs;
        result = dsSetDFC(handle, gDSVideoDeviceConfiguration[i].SupportedDFCs[numSupportedDFCs - 1] + 1);
        if (gSourceType == 1)
        {
            UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
        }
        else if (gSourceType == 0)
        {
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // 06: De-initialize video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // 07: Call dsSetDFC() after termination
    result = dsSetDFC(handle, gDSVideoDeviceConfiguration[0].DefaultDFC);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetDFC() correctly fetches the screen zoom mode in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Get the DFC mode using dsGetDFC() with the obtained handle |int=handle, dsVideoZoom_t*| dsERR_NONE | Should successfully fetch the DFC mode |
 * |04|Compare the dfc mode with the value from profile to make sure they match || Success | Should be equal |
 * |05|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetDFC(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    int count = 0;
    intptr_t handle = -1;
    dsVideoZoom_t dfc_mode;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Get the DFC mode using dsGetDFC() with the obtained handle
        result = dsGetDFC(handle, &dfc_mode);
        if(gSourceType == 1) {
            UT_ASSERT_EQUAL(result, dsERR_NONE);
            // Step 04: Compare the DFC mode with value from the profile
            for (int j = 0; j < gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs; ++j) {
                UT_LOG_DEBUG("SupportedDFCs: %d",gDSVideoDeviceConfiguration[i].SupportedDFCs[j]);
                if(dfc_mode == gDSVideoDeviceConfiguration[i].SupportedDFCs[j]) {
                    count++;
                    UT_LOG_DEBUG("SupportedDFCs: %d dfc_mode %d ",gDSVideoDeviceConfiguration[i].SupportedDFCs[j],dfc_mode);
                }
                if(count < 1) {
                    UT_FAIL("SupportedDFCs are not present");
                }
            }
        } else if(gSourceType == 0) {
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetDFC() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetDFC() without prior initialization of video devices |int=handle, dsVideoZoom_t*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsGetDFC() with an invalid handle |int=-1, dsVideoZoom_t*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsGetDFC() with an invalid pointer |int=handle, NULL| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsGetDFC() after termination of video devices |int=handle, dsVideoZoom_t*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetDFC(void)
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    dsVideoZoom_t dfc_mode;

    // Step 01: Call dsGetDFC() without prior initialization
    result = dsGetDFC(handle, &dfc_mode);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetDFC() with an invalid handle
        result = dsGetDFC(-1, &dfc_mode);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetDFC() with an invalid pointer
        result = dsGetDFC(handle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetDFC() after termination
    result = dsGetDFC(handle, &dfc_mode);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetHDRCapabilities() correctly fetches the HDR capabilities in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Get HDR capabilities using dsGetHDRCapabilities() with the obtained handle |int=handle, int*| dsERR_NONE | Should successfully fetch the HDR capabilities |
 * |04|Compare the result with the value from the profile to make sure they are the same || Success | The returned values should be the same |
 * |05|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetHDRCapabilities(void)
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    int32_t hdr_capabilities = dsHDRSTANDARD_NONE;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_LOG_INFO("Handle: %d", handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Get HDR capabilities using dsGetHDRCapabilities() with the obtained handle
        result = dsGetHDRCapabilities(handle, &hdr_capabilities);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Compare the HDR capabilities with the value from the profile
        UT_ASSERT_EQUAL(hdr_capabilities, gDSVideoDeviceConfiguration[i].HDRCapabilities);
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetHDRCapabilities() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetHDRCapabilities() without prior initialization of video devices |int=handle, int*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsGetHDRCapabilities() with an invalid handle |int=-1, int*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsGetHDRCapabilities() with an null parameter |int=handle, NULL| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsGetHDRCapabilities() after termination of video devices |int=handle, int*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetHDRCapabilities(void)
{
    gTestID = 12;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    int32_t hdr_capabilities;

    // Step 01: Call dsGetHDRCapabilities() without prior initialization
    result = dsGetHDRCapabilities(handle, &hdr_capabilities); // Note: using handle for HDR capabilities for this error case
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetHDRCapabilities() with an invalid handle
        result = dsGetHDRCapabilities(-1, &hdr_capabilities);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetHDRCapabilities() with a null parameter
        result = dsGetHDRCapabilities(handle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetHDRCapabilities() after termination
    result = dsGetHDRCapabilities(handle, &hdr_capabilities);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetSupportedVideoCodingFormats() correctly fetches supported video formats in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Get supported video formats using dsGetSupportedVideoCodingFormats() with the obtained handle |int=handle, unsigned int*| dsERR_NONE | Should successfully fetch the supported video formats |
 * |04|Compare the returned value with the value from the profile  to make sure they return the same value || Success | The values should be equal |
 * |05|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetSupportedVideoCodingFormats(void)
{
    gTestID = 13;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    unsigned int supported_formats;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Get supported video formats using dsGetSupportedVideoCodingFormats() with the obtained handle
        result = dsGetSupportedVideoCodingFormats(handle, &supported_formats);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Compare the supported video formats with the value from the profile
        UT_ASSERT_EQUAL(supported_formats, gDSVideoDeviceConfiguration[i].SupportedVideoCodingFormats);
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetSupportedVideoCodingFormats() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetSupportedVideoCodingFormats() without prior initialization of video devices |int=handle, unsigned int*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsGetSupportedVideoCodingFormats() with an invalid handle |int=-1, unsigned int*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsGetSupportedVideoCodingFormats() with null value |int=handle, NULL| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsGetSupportedVideoCodingFormats() after termination of video devices |int=handle, unsigned int*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetSupportedVideoCodingFormats(void)
{
    gTestID = 14;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    unsigned int supported_formats;

    // Step 01: Call dsGetSupportedVideoCodingFormats() without prior initialization
    result = dsGetSupportedVideoCodingFormats(handle, &supported_formats); // Note: using handle for supported formats for this error case
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetSupportedVideoCodingFormats() with an invalid handle
        result = dsGetSupportedVideoCodingFormats(-1, &supported_formats);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetSupportedVideoCodingFormats() with a null value
        result = dsGetSupportedVideoCodingFormats(handle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetSupportedVideoCodingFormats() after termination
    result = dsGetSupportedVideoCodingFormats(handle, &supported_formats);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetVideoCodecInfo() correctly fetches the video codec information in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Get video codec information using dsGetVideoCodecInfo() with the obtained handle and compare with the profile file | int=handle, dsVIDEO_CODEC_MPEGHPART2, dsVideoCodecInfo_t*| dsERR_NONE | Should successfully fetch the supported video formats |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetVideoCodecInfo(void)
{
    gTestID = 15;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    dsVideoCodecInfo_t codecInfo;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    int j = 1;
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);
        // Step 03: Iterate over all codecs
        for(dsVideoCodingFormat_t codec = dsVIDEO_CODEC_MPEGHPART2; codec < dsVIDEO_CODEC_MAX; codec = (dsVideoCodingFormat_t)(codec + 1)){
            result = dsGetVideoCodecInfo(handle, codec, &codecInfo);
            if (gSourceType == 1)
            {
                if (!(gDSVideoDeviceConfiguration[i].SupportedVideoCodingFormats & codec))
                {
                    continue;
                }
                UT_ASSERT_EQUAL(result, dsERR_NONE);
                // Compare with profile file
                UT_ASSERT_EQUAL(codecInfo.num_entries, gDSVideoDeviceConfiguration[i].num_codec_entries);
                if (codec == dsVIDEO_CODEC_MPEGHPART2)
                {
                    UT_ASSERT_EQUAL(codecInfo.entries->profile, gDSVideoDeviceConfiguration[i].profile);
                    // Support for float values in KVP
                    UT_ASSERT_EQUAL(codecInfo.entries->level, gDSVideoDeviceConfiguration[i].level);
                }
                codec = (dsVideoCodingFormat_t)(0x01 << (j++));
            } else if(gSourceType == 0) {
                UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
            }
        }
    }

    // Step 04: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetVideoCodecInfo() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetVideoCodecInfo() without prior initialization of video devices | int=handle, dsVIDEO_CODEC_MPEGHPART2, dsVideoCodecInfo_t*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsGetVideoCodecInfo() with an invalid handle | int=-1, dsVIDEO_CODEC_MPEGHPART2, dsVideoCodecInfo_t*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsGetVideoCodecInfo() with an invalid coding format | int=handle, dsVIDEO_CODEC_MAX, dsVideoCodecInfo_t*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|Call dsGetVideoCodecInfo() with null parameter | int=handle, dsVIDEO_CODEC_MPEGHPART2, NULL| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |07|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |08|Call dsGetVideoCodecInfo() without prior initialization of video devices | int=handle, dsVIDEO_CODEC_MPEGHPART2, dsVideoCodecInfo_t*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetVideoCodecInfo(void)
{
    gTestID = 16;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    dsVideoCodecInfo_t codecInfo;

    // Step 01: Call dsGetVideoCodecInfo() without prior initialization
    result = dsGetVideoCodecInfo(handle, dsVIDEO_CODEC_MPEGHPART2, &codecInfo);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_OPERATION_NOT_SUPPORTED);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetVideoCodecInfo() with an invalid handle
        result = dsGetVideoCodecInfo(-1, dsVIDEO_CODEC_MPEGHPART2, &codecInfo);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetVideoCodecInfo() with an invalid coding format
        result = dsGetVideoCodecInfo(handle, dsVIDEO_CODEC_MAX, &codecInfo);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 06: Call dsGetVideoCodecInfo() with a null parameter
        result = dsGetVideoCodecInfo(handle, dsVIDEO_CODEC_MPEGHPART2, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 07: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 08: Call dsGetVideoCodecInfo() after termination
    result = dsGetVideoCodecInfo(handle, dsVIDEO_CODEC_MPEGHPART2, &codecInfo);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_OPERATION_NOT_SUPPORTED);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsForceDisableHDRSupport() correctly force disables the HDR support in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Force disable HDR support using dsForceDisableHDRSupport() with the obtained handle |int=handle, bool=true| dsERR_NONE | HDR support should be force disabled successfully |
 * |04|Force disable HDR support using dsForceDisableHDRSupport() with the obtained handle |int=handle, bool=false| dsERR_NONE | HDR support should be force disabled successfully |
 * |05|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsForceDisableHDRSupport(void)
{
    gTestID = 17;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Force disable HDR support (set to true)
        result = dsForceDisableHDRSupport(handle, true);
        UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);

        // Step 04: Force disable HDR support (set to false)
        result = dsForceDisableHDRSupport(handle, false);
        UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsForceDisableHDRSupport() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsForceDisableHDRSupport() without prior initialization of video devices |int=handle, bool=true| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsForceDisableHDRSupport() with an invalid handle |int=-1, bool=true| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |06|Call dsForceDisableHDRSupport() after termination of video devices |int=handle, bool=true| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsForceDisableHDRSupport(void)
{
    gTestID = 18;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;

    // Step 01: Call dsForceDisableHDRSupport() without prior initialization
    result = dsForceDisableHDRSupport(handle, true); // Note: uninitialized 'handle' used here
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_OPERATION_NOT_SUPPORTED);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsForceDisableHDRSupport() with an invalid handle
        result = dsForceDisableHDRSupport(-1, true);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 06: Call dsForceDisableHDRSupport() after termination
    result = dsForceDisableHDRSupport(handle, true);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_OPERATION_NOT_SUPPORTED);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsSetFRFMode() correctly sets the FRF mode of the device in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Set the FRF mode using dsSetFRFMode() with the obtained handle and a valid framerate value | int=handle, int| dsERR_NONE | FRF mode should be set successfully |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsSetFRFMode(void)
{
    gTestID = 19;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Set the FRF mode using a valid framerate
        uint32_t framerateMode = 0;
        result = dsSetFRFMode(handle, framerateMode);
        if (gSourceType == 1)
        {
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
        else if (gSourceType == 0)
        {
            UT_ASSERT_EQUAL(result, dsERR_NONE);
        }
        framerateMode = 1;
        result = dsSetFRFMode(handle, framerateMode);
        if (gSourceType == 1)
        {
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
        else if (gSourceType == 0)
        {
            UT_ASSERT_EQUAL(result, dsERR_NONE);
        }
    }

    // Step 04: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsSetFRFMode() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsSetFRFMode() without prior initialization of video devices | int=handle, int| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsSetFRFMode() with an invalid handle | int=-1, int| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsSetFRFMode() with an invalid framerate | int=handle, int=-1| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsSetFRFMode() after termination of video devices | int=handle, int| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsSetFRFMode(void)
{
    gTestID = 20;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;

    // Step 01: Call dsSetFRFMode() without prior initialization
    result = dsSetFRFMode(handle, 1); // Note: uninitialized 'handle' used here
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsSetFRFMode() with an invalid handle
        result = dsSetFRFMode(-1, 1);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsSetFRFMode() with an invalid framerate
        result = dsSetFRFMode(handle, -1);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsSetFRFMode() after termination
    result = dsSetFRFMode(handle, 1);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetFRFMode() correctly fetches the FRF mode of the device in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Get the FRF mode using dsGetFRFMode() with the obtained handle |int=handle,int*| dsERR_NONE | Should fetch the FRF mode successfully |
 * |04|Get the FRF mode using dsGetFRFMode() again |int=handle,int*| dsERR_NONE | Should fetch the FRF mode successfully |
 * |05|Compare the returned values to make sure they are the same || Success | The values should be the same value |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetFRFMode(void)
{
    gTestID = 21;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    int fetchedFRFMode;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Get the FRF mode using the obtained handle
        result = dsGetFRFMode(handle, &fetchedFRFMode);
        if(gSourceType == 0) {
            UT_ASSERT_EQUAL(result, dsERR_NONE);

            // Step 04: Compare the FRF mode with the value from the profile
            UT_ASSERT_EQUAL(fetchedFRFMode, gDSVideoDeviceConfiguration[i].DefaultDFC);
        } else if(gSourceType == 1) {
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetFRFMode() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetFRFMode() without prior initialization of video devices |int=handle,int*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsGetFRFMode() with an invalid handle |int=-1,int*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsGetFRFMode() with null value |int=handle,NULL| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsGetFRFMode() after termination of video devices |int=handle,int*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetFRFMode(void)
{
    gTestID = 22;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    int fetchedFRFMode;

    // Step 01: Call dsGetFRFMode() without prior initialization
    result = dsGetFRFMode(handle, &fetchedFRFMode); // Note: uninitialized 'handle' used here
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetFRFMode() with an invalid handle
        result = dsGetFRFMode(-1, &fetchedFRFMode);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetFRFMode() with null value
        result = dsGetFRFMode(handle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetFRFMode() after termination
    result = dsGetFRFMode(handle, &fetchedFRFMode);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetCurrentDisplayframerate() correctly fetches the current framerate of the device in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Get the current display framerate using dsGetCurrentDisplayframerate() with the obtained handle |int=handle,char*| dsERR_NONE | Should fetch the current framerate successfully |
 * |04|Get the current display framerate using dsGetCurrentDisplayframerate() again |int=handle,char*| dsERR_NONE | Should fetch the current framerate successfully |
 * |05|Compare the results to make sure they are the same || Success | The results should equal one another |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsGetCurrentDisplayframerate(void)
{
    gTestID = 23;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    int count = 0;;
    intptr_t handle = -1;
    char fetchedFramerate[50];

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Get the current display framerate using the obtained handle
        result = dsGetCurrentDisplayframerate(handle, fetchedFramerate);
        if(gSourceType == 0) {
            UT_ASSERT_EQUAL(result, dsERR_NONE);

            // Step 04: Compare the current display framerate with the value from the profile
            for (int j = 0; j < gDSVideoDeviceConfiguration[i].NoOfSupportedDFR ;j++) {
                UT_LOG_INFO("SupportedDisplayFramerate %s",gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j]);
                if(strncmp(fetchedFramerate,gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j],sizeof(fetchedFramerate)) == 0) {
                    UT_LOG_INFO("SupportedDisplayFramerate %s fetchedFramerate %s ",gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j], fetchedFramerate);
                    count++;
                }
                if(count < 1) {
                    UT_FAIL("SupportedDisplayFramerate are not present");
                }
            }
        } else if(gSourceType == 1){
                UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 05: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsGetCurrentDisplayframerate() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsGetCurrentDisplayframerate() without prior initialization of video devices|int=handle,char*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsGetCurrentDisplayframerate() with an invalid handle |int=-1,char* |dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsGetCurrentDisplayframerate() with NULL value |int=handle, NULL |dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |07|Call dsGetCurrentDisplayframerate() after termination of video devices|int=handle,char*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsGetCurrentDisplayframerate(void)
{
    gTestID = 24;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;
    char fetchedFramerate[50]; // Assuming a buffer size, modify as necessary.

    // Step 01: Call dsGetCurrentDisplayframerate() without prior initialization
    result = dsGetCurrentDisplayframerate(handle, fetchedFramerate); // Note: uninitialized 'handle' used here
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsGetCurrentDisplayframerate() with an invalid handle
        result = dsGetCurrentDisplayframerate(-1, fetchedFramerate);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsGetCurrentDisplayframerate() with NULL value
        result = dsGetCurrentDisplayframerate(handle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 07: Call dsGetCurrentDisplayframerate() after termination
    result = dsGetCurrentDisplayframerate(handle, fetchedFramerate);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsSetDisplayframerate() correctly sets the display framerate for the device in positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |03|Set the display framerate using dsSetDisplayframerate() with the obtained handle and a valid framerate |int=handle,char*| dsERR_NONE | Display framerate should be set successfully |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 */
void test_l1_dsVideoDevice_positive_dsSetDisplayframerate(void)
{
    gTestID = 25;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 03: Set the display framerate using the obtained handle
        if(gSourceType == 0) {
            for(int j=0;j<gDSVideoDeviceConfiguration[i].NoOfSupportedDFR;j++){
                result = dsSetDisplayframerate(handle, gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j]);
                UT_ASSERT_EQUAL(result, dsERR_NONE);
            }
        } else if(gSourceType == 1){
            UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
        }
    }

    // Step 04: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsSetDisplayframerate() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsSetDisplayframerate() without prior initialization of video devices |int=handle,char*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Obtain video device handle using dsGetVideoDevice() | int=index, int=*handle | dsERR_NONE and (handle >= 0) | Should obtain a valid handle successfully |
 * |04|Call dsSetDisplayframerate() with an invalid handle |int=-1,char*| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |05|Call dsSetDisplayframerate() with NULL char*|int=handle,NULL| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |06|Call dsSetDisplayframerate() with invalid char* |int=-1,char*="junk"| dsERR_INVALID_PARAM | Should report invalid parameter |
 * |07|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |08|Call dsSetDisplayframerate() after termination of video devices |int=handle,char*| dsERR_NOT_INITIALIZED | Should report module not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsSetDisplayframerate(void)
{
    gTestID = 26;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;
    intptr_t handle = -1;

    // Step 01: Call dsSetDisplayframerate() without prior initialization
    result = dsSetDisplayframerate(handle, "30fps"); // Note: uninitialized 'handle' used here
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Obtain video device handle
    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++){
        result = dsGetVideoDevice(i, &handle);
        UT_ASSERT_EQUAL(result, dsERR_NONE);

        // Step 04: Call dsSetDisplayframerate() with an invalid handle
        result = dsSetDisplayframerate(-1, "30fps");
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

        // Step 05: Call dsSetDisplayframerate() with NULL char*
        result = dsSetDisplayframerate(handle, NULL);
        UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);
    }

    // Step 06: Call dsSetDisplayframerate() with invalid char*
    result = dsSetDisplayframerate(handle, "junk");
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

    // Step 07: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 08: Call dsSetDisplayframerate() after termination
    result = dsSetDisplayframerate(handle, "30fps");
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsRegisterFrameratePreChangeCB() returns correct error codes for positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Call dsRegisterFrameratePreChangeCB() - Register a valid callback for Display framerate pre change event | cb = [valid callback function] | dsERR_NONE | Display framerate pre change event update callback registration must be successful |
 * |03|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */

/*callback*/
void mockFrameRatePreChangeCallback(unsigned int tSecond){
        // Mock implementation, can be customized for testing
}

void test_l1_dsVideoDevice_positive_dsRegisterFrameratePreChangeCB(void)
{
    gTestID = 27;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Register a valid callback function for Display framerate pre change event
    result = dsRegisterFrameratePreChangeCB(mockFrameRatePreChangeCallback);
    if(gSourceType == 1) {
        UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
    } else if(gSourceType == 0) {
        UT_ASSERT_EQUAL(result, dsERR_NONE);
    }

    // Step 03: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsRegisterFrameratePreChangeCB() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsRegisterFrameratePostChangeCB() - Attempt to register for display framerate pre change event callback without initializing the video device | cb = [valid callback function ] | dsERR_NOT_INITIALIZED | Callback registration must fail as module is not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Call dsRegisterFrameratePostChangeCB() with an invalid callback (NULL)| cb = NULL | dsERR_INVALID_PARAM | Invalid paramter error must be returned |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |05|Call dsRegisterFrameratePostChangeCB() - Attempt to register for display framerate pre change event callback without initializing the video devices | cb = [valid callback function ] | dsERR_NOT_INITIALIZED | Callback registration must fail as module is not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsRegisterFrameratePreChangeCB(void)
{
    gTestID = 28;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;

    // Step 01: Attempt to register callback without initialization
    result = dsRegisterFrameratePreChangeCB(mockFrameRatePreChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Register with invalid callback (NULL)
    result = dsRegisterFrameratePreChangeCB(NULL);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

    // Step 04: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 05: Attempt to register callback after termination
    result = dsRegisterFrameratePreChangeCB(mockFrameRatePreChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsRegisterFrameratePostChangeCB() returns correct error codes for positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |02|Call dsRegisterFrameratePostChangeCB() - Register a valid callback for Display framerate post change event | cb = [valid callback function] | dsERR_NONE | Display framerate Update callback registration must be successful |
 * |03|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */

/*callback*/
void mockFrameRatePostChangeCallback(unsigned int tSecond){
	// Mock implementation, can be customized for testing
}

void test_l1_dsVideoDevice_positive_dsRegisterFrameratePostChangeCB(void)
{
    gTestID = 29;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;

    // Step 01: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 02: Register a valid callback function for Display framerate post change event
    result = dsRegisterFrameratePostChangeCB(mockFrameRatePostChangeCallback);
    if(gSourceType == 1) {
        UT_ASSERT_EQUAL(result, dsERR_OPERATION_NOT_SUPPORTED);
    } else if(gSourceType == 0) {
        UT_ASSERT_EQUAL(result, dsERR_NONE);
    }

    // Step 03: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Test guidance to ensure dsRegisterFrameratePostChangeCB() returns correct error codes for negative scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 *
 * **Dependencies:** None@n
 * **User Interaction:** None
 *
 * **Test Procedure:**@n
 * |Variation / Step|Description|Expected Result|Notes|
 * |:--:|-----------|----------|--------------|-----|
 * |01|Call dsRegisterFrameratePostChangeCB() - Attempt to register for frame rate update callback without initializing the video device | cb = [valid callback function ] | dsERR_NOT_INITIALIZED | Callback registration must fail as module is not initialized |
 * |02|Initialize video devices using dsVideoDeviceInit() | | dsERR_NONE | Video devices should be initialized successfully |
 * |03|Call dsRegisterFrameratePostChangeCB() with an invalid callback (NULL)| cb = NULL | dsERR_INVALID_PARAM | Invalid paramter error must be returned |
 * |04|De-initialize the video devices using dsVideoDeviceTerm() | | dsERR_NONE | Video devices should be de-initialized successfully|
 * |05|Call dsRegisterFrameratePostChangeCB() - Attempt to register for frame rate update callback without initializing the video devices | cb = [valid callback function ] | dsERR_NOT_INITIALIZED | Callback registration must fail as module is not initialized |
 *
 * @note The return value dsERR_GENERAL and dsERR_OPERATION_NOT_SUPPORTED may be difficult to test in a simulated environment
 *
 */
void test_l1_dsVideoDevice_negative_dsRegisterFrameratePostChangeCB(void)
{
    gTestID = 30;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int result;

    // Step 01: Attempt to register callback without initialization
    result = dsRegisterFrameratePostChangeCB(mockFrameRatePostChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    // Step 02: Initialize video devices
    result = dsVideoDeviceInit();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 03: Register with invalid callback (NULL)
    result = dsRegisterFrameratePostChangeCB(NULL);
    UT_ASSERT_EQUAL(result, dsERR_INVALID_PARAM);

    // Step 04: De-initialize the video devices
    result = dsVideoDeviceTerm();
    UT_ASSERT_EQUAL_FATAL(result, dsERR_NONE);

    // Step 05: Attempt to register callback after termination
    result = dsRegisterFrameratePostChangeCB(mockFrameRatePostChangeCallback);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, dsERR_NOT_INITIALIZED, dsERR_INVALID_PARAM);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_dsVideoDevice_register ( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 dsVideoDevice]", NULL, NULL );
    if ( NULL == pSuite )
    {
        return -1;
    }

    UT_add_test( pSuite, "dsVideoDeviceInit_L1_positive" ,test_l1_dsVideoDevice_positive_dsVideoDeviceInit );
    UT_add_test( pSuite, "dsVideoDeviceTerm_L1_positive" ,test_l1_dsVideoDevice_positive_dsVideoDeviceTerm );
    UT_add_test( pSuite, "dsGetVideoDevice_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetVideoDevice );
    UT_add_test( pSuite, "dsSetDFC_L1_positive" ,test_l1_dsVideoDevice_positive_dsSetDFC );
    UT_add_test( pSuite, "dsGetHDRCapabilities_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetHDRCapabilities );
    UT_add_test( pSuite, "dsGetSupportedVideoCodingFormats_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetSupportedVideoCodingFormats );
    UT_add_test( pSuite, "dsGetVideoCodecInfo_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetVideoCodecInfo );
    UT_add_test( pSuite, "dsSetFRFMode_L1_positive" ,test_l1_dsVideoDevice_positive_dsSetFRFMode );
    UT_add_test( pSuite, "dsGetFRFMode_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetFRFMode );
    UT_add_test( pSuite, "dsGetCurrentDisplayframerate_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetCurrentDisplayframerate );
    UT_add_test( pSuite, "dsSetDisplayframerate_L1_positive" ,test_l1_dsVideoDevice_positive_dsSetDisplayframerate );
    UT_add_test( pSuite, "dsRegisterFrameratePreChangeCB_L1_positive" ,test_l1_dsVideoDevice_positive_dsRegisterFrameratePreChangeCB );
    UT_add_test( pSuite, "dsRegisterFrameratePostChangeCB_L1_positive" ,test_l1_dsVideoDevice_positive_dsRegisterFrameratePostChangeCB );
    UT_add_test( pSuite, "dsVideoDeviceInit_L1_negative" ,test_l1_dsVideoDevice_negative_dsVideoDeviceInit );
    UT_add_test( pSuite, "dsVideoDeviceTerm_L1_negative" ,test_l1_dsVideoDevice_negative_dsVideoDeviceTerm );
    UT_add_test( pSuite, "dsGetVideoDevice_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetVideoDevice );
    UT_add_test( pSuite, "dsSetDFC_L1_negative" ,test_l1_dsVideoDevice_negative_dsSetDFC );
    UT_add_test( pSuite, "dsGetHDRCapabilities_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetHDRCapabilities );
    UT_add_test( pSuite, "dsGetSupportedVideoCodingFormats_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetSupportedVideoCodingFormats );
    UT_add_test( pSuite, "dsGetVideoCodecInfo_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetVideoCodecInfo );
    UT_add_test( pSuite, "dsSetFRFMode_L1_negative" ,test_l1_dsVideoDevice_negative_dsSetFRFMode );
    UT_add_test( pSuite, "dsGetFRFMode_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetFRFMode );
    UT_add_test( pSuite, "dsGetCurrentDisplayframerate_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetCurrentDisplayframerate );
    UT_add_test( pSuite, "dsSetDisplayframerate_L1_negative" ,test_l1_dsVideoDevice_negative_dsSetDisplayframerate );
    UT_add_test( pSuite, "dsRegisterFrameratePreChangeCB_L1_negative" ,test_l1_dsVideoDevice_negative_dsRegisterFrameratePreChangeCB );
    UT_add_test( pSuite, "dsRegisterFrameratePostChangeCB_L1_negative" ,test_l1_dsVideoDevice_negative_dsRegisterFrameratePostChangeCB );
    UT_add_test( pSuite, "dsGetDFC_L1_positive" ,test_l1_dsVideoDevice_positive_dsGetDFC );
    UT_add_test( pSuite, "dsForceDisableHDRSupport_L1_positive" ,test_l1_dsVideoDevice_positive_dsForceDisableHDRSupport );
    UT_add_test( pSuite, "dsGetDFC_L1_negative" ,test_l1_dsVideoDevice_negative_dsGetDFC );
    UT_add_test( pSuite, "dsForceDisableHDRSupport_L1_negative" ,test_l1_dsVideoDevice_negative_dsForceDisableHDRSupport );
    extendedEnumsSupported = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), "dsVideoDevice/features/extendedEnumsSupported" );
    return 0;
}

/** @} */ // End of DS_VideoDevice_HALTEST_L1
/** @} */ // End of DS_VideoDevice_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
