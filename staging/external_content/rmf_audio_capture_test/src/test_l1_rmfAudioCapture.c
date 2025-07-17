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
 *
 */

/**
 * @addtogroup RMF_AudioCapture RMF Audio Capture
 * @{
 *
 */

/**
 * @addtogroup RMF_AudioCapture_HALTESTS RMF Audio Capture HAL Tests
 * @{
 * @par
 * Unit Testing Suite for RMF Audio Capture HAL
 */

/**
 * @defgroup RMF_AudioCapture_HALTESTS_L1 RMF Audio Capture HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for RMF Audio Capture HAL :
 *
 * Level 1 unit test cases for all APIs of Audio Capture HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [rmf-audio-capture_halSpec.md](../../docs/pages/rmf-audio-capture_halSpec.md)
 *
 * @endparblock
 *
 */


/**
* @file test_l1_rmfAudioCapture.c
*
*/


#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include "rmfAudioCapture.h"

// TODO: Need to remove this once the rmf_osal_error.h is included to original interface file file
#define TEST_RMF_ERRBASE_OSAL           (0x0100)
#define TEST_RMF_OSAL_EINVAL            (TEST_RMF_ERRBASE_OSAL+1)
#define TEST_RMF_OSAL_EBUSY             (TEST_RMF_ERRBASE_OSAL+3)

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

bool test_l1_is_aux_capture_supported()
{
    bool aux_capture_supported = UT_KVP_PROFILE_GET_BOOL("rmfaudiocapture/features/auxsupport");
    return aux_capture_supported;
}

rmf_Error test_l1_dummy_data_cb(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    return RMF_SUCCESS;
}

void test_l1_prepare_dummy_start_settings(RMF_AudioCapture_Settings * settings)
{
    settings->cbBufferReady = test_l1_dummy_data_cb;
    settings->cbStatusChange = NULL;
}

static rmf_Error test_l1_validate_settings(RMF_AudioCapture_Settings * settings)
{
    rmf_Error result = RMF_SUCCESS;

    if((racFormat_e16BitStereo  > settings->format) || (racFormat_eMax <= settings->format))
    {
        UT_FAIL("Error: invalid format detected.");
        return RMF_ERROR;
    }

    if((racFreq_e16000  > settings->samplingFreq) || (racFreq_eMax <= settings->samplingFreq))
    {
        UT_FAIL("Error: invalid samping frequency detected.");
        return RMF_ERROR;
    }
    return result;
}

rmf_Error test_l1_validate_status_active(RMF_AudioCapture_Status * status)
{
    rmf_Error result = RMF_SUCCESS;

    if(1 != status->started)
    {
        UT_FAIL("Error: wrong status - started should be 1.");
        return RMF_ERROR;
    }

    if((racFormat_e16BitStereo  > status->format) || (racFormat_eMax <= status->format))
    {
        UT_FAIL("Error: invalid format detected.");
        return RMF_ERROR;
    }

    if((racFreq_e16000  > status->samplingFreq) || (racFreq_eMax <= status->samplingFreq))
    {
        UT_FAIL("Error: invalid samping frequency detected.");
        return RMF_ERROR;
    }
    return result;
}

rmf_Error test_l1_compare_settings(RMF_AudioCapture_Settings * left, RMF_AudioCapture_Settings * right)
{
    if(0 == memcmp(left, right, sizeof(RMF_AudioCapture_Settings)))
        return RMF_SUCCESS;
    else
    {
        UT_FAIL("Error: settings mistmatch!");
        return RMF_ERROR;
    }
}

/**
* @brief Tests positive scenarios with primary audio for RMF_AudioCapture_Open_Type()
*
* Probe various input scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 001@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open_Type()` to check open->close->open sequence | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_primary (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 1;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Open interface
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step04: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios with primary audio for RMF_AudioCapture_Open_Type()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 002@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` with invalid handle | handle must be NULL, type=primary | RMF_INVALID_PARM | Should pass |
* | 02 | Call `RMF_AudioCapture_Open_Type()` with junk type and valid handle | handle is a valid pointer, type=xyz | RMF_INVALID_PARM | Should pass |
* | 03 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type=primary | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` when primary is already open | handle must be a valid pointer, type=primary | RMF_INVALID_STATE | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/

void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_primary (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 2;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface with invalid handle
    result = RMF_AudioCapture_Open_Type(NULL, RMF_AC_TYPE_PRIMARY);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step02: Open interface with invalid type
    result = RMF_AudioCapture_Open_Type(&handle, "junk");
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step03: Oen interface
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step04: Open inetrface when already open
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EBUSY);

    //Step05: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test positive scenarios for RMF_AudioCapture_Open()
*
* Probe various input scenarios for RMF_AudioCapture_Open() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 003@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open()` to check open->close->open sequence | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Open_Type()` to ensure open_type works after open | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Open()` to ensure open works after open_type | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 3;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step04: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step05: Open interface with type
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step06: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step07: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step08: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Open()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 004@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` with invalid handle | handle must be NULL | RMF_INVALID_PARM | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open()` when already open | handle must be a valid pointer | RMF_INVALID_STATE | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` when already open | handle must be a valid pointer, type is primary | RMF_INVALID_STATE | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Open_Type()` to open primary capture | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Open()` when already open | handle must be a valid pointer | RMF_INVALID_STATE | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 4;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface with invalid handle
    result = RMF_AudioCapture_Open(NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Open inetrface when already open
    result = RMF_AudioCapture_Open(&handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EBUSY);

    //Step04: Open interface with type
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EBUSY);

    //Step05: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step06: Open interface with type
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step07: Open interface again
    result = RMF_AudioCapture_Open(&handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EBUSY);
    //Step08: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test simplified positive scenarios for RMF_AudioCapture_Close()
*
* Probe various input scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 005@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 5;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test simple negative scenarios for RMF_AudioCapture_Close()
*
* Force various edge/error scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 006@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Close()` with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Close()` with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` with outdated handle | handle obtained in last open call | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close (void)
{
    RMF_AudioCaptureHandle handle = NULL;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 6;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Close interface with invalid handle
    result = RMF_AudioCapture_Close(NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_HANDLE, TEST_RMF_OSAL_EINVAL);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Close Interface with invalid handle
    result = RMF_AudioCapture_Close(NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_HANDLE, TEST_RMF_OSAL_EINVAL);

    //Step04: close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step05: Close interface again
    result = RMF_AudioCapture_Close(handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EINVAL);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple positive scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 007@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` with valid parameters and validate it | valid pointer for settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` again with valid parameters and validate it | valid pointer for settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` with current handle | Release resource after test | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings;

    gTestID = 7;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step03: get default settings again and validate it
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step04: close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple negative scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 008@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_GetDefaultSettings()` when not in open/started state | valid settings | RMF_INVALID_STATE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` with invalid parameters | settings=NULL | RMF_INVALID_PARM | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_GetDefaultSettings()` when not in open/started state | valid settings | RMF_INVALID_STATE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings;

    gTestID = 8;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Get default settings before open
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, RMF_SUCCESS);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step04: close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step05: Get default settings after close
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple positive scenarios for RMF_AudioCapture_Start()
*
* Probe various input scenarios for RMF_AudioCapture_Start() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 009@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` with caller-modified settings derived from above | settings - increase delayCompensation_ms by 2000, dummy data callback, empty status callback | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that passed parameters were set | valid handle | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 08 | Call `RMF_AudioCapture_Stop()` to stop audio capture | valid handle | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_Start()` with 16-bit PCM stereo format, 44.1kHz or 44kHz sampling rate | settings=default settings, format = racFormat_e16BitStereo, sampling rate = racFreq_e44100 and racFreq_e48000, dummy data callback, status callback NULL | At least one of the sampling rates should yield RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 12 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_Start()` with default settings to check start after stop->close->open | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 14 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 15 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/

void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings, current_settings;
    memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
    memset(&current_settings, 0, sizeof(current_settings));

    gTestID = 9;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    test_l1_prepare_dummy_start_settings(&settings);

    //Step03: audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    if(RMF_SUCCESS != result)
    {
        UT_LOG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    //Step04: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step05: Get default settings and validate
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    settings.delayCompensation_ms += 2000;
    test_l1_prepare_dummy_start_settings(&settings);
    //Step06: audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step07: Get teh current settings and compare with settings
    result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_compare_settings(&settings, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step08 audio capture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step09: Audioformat start with 16-bit PCM stereo format
    settings.format = racFormat_e16BitStereo;
    settings.samplingFreq = racFreq_e48000;
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = RMF_AudioCapture_Stop(handle);

    //Step10: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step11: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step12: Get default settings and validate
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step13: audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step14: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step15: Close intrerface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test negative scenarios for RMF_AudioCapture_Start()
*
* Force various edge/error scenarios for RMF_AudioCapture_Start() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 010@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Start()` with invalid handle | handle is NULL, settings is NULL | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Start()` with invalid handle and settings | handle is NULL, settings is NULL | RMF_INVALID_HANDLE | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` with invalid handle | handle is NULL, settings = settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` with invalid settings | current handle, settings is NULL | RMF_INVALID_PARM | Should pass |
* | 07 | Call `RMF_AudioCapture_Start()` with parameter outside bounds | current handle, settings = default settings, but format = racFormat_eMax | RMF_INVALID_PARM | Should pass |
* | 08 | Call `RMF_AudioCapture_Start()` with parameter outside bounds | current handle, settings = default settings, but samplingFreq = racFreq_eMax | RMF_INVALID_PARM | Should pass |
* | 09 | Call `RMF_AudioCapture_Start()` with parameter outside bounds | current handle, settings = default settings, but cbBufferReady = NULL | RMF_INVALID_PARM | Should pass |
* | 10 | Call `RMF_AudioCapture_Start()` with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_Start()` again to test API response when already in STARTED state | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_STATE | Should pass |
* | 12 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 14 | Call `RMF_AudioCapture_Start()` with outdated handle | handle obtained in last open call, settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should pass |
* | 15 | Call `RMF_AudioCapture_Start()` with unsupported format | current handle, settings = default settings, but format = unsupported format | RMF_INVALID_PARM | Should pass |
*/

//Note: Test case 15 is not implemented yet as it requires reading from profile.
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Start (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings, bad_settings;

    gTestID = 10;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: audiocapture start befor open
    result = RMF_AudioCapture_Start(NULL, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step04: Audiocapture start with invalid handle and settings
    result = RMF_AudioCapture_Start(NULL, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step05: Audiocapture start with invalid handle
    result = RMF_AudioCapture_Start(NULL, &settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step06: Audiocapture start with invalid settings
    result = RMF_AudioCapture_Start(handle, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step07: Audiocapture start with invalid format
    bad_settings = settings;
    bad_settings.format = racFormat_eMax; // bad format
    result = RMF_AudioCapture_Start(handle, &bad_settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    bad_settings = settings;
    bad_settings.samplingFreq = racFreq_eMax; // bad frequency
    //Step08: Audiocapture start with invalid frequency
    result = RMF_AudioCapture_Start(handle, &bad_settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    bad_settings = settings;
    bad_settings.cbBufferReady = NULL; // bad callback
    //Step09: Audiocapture start with invalid callback
    result = RMF_AudioCapture_Start(handle, &bad_settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step10: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step11: Audiocapture start again
    result = RMF_AudioCapture_Start(handle, &settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EBUSY);

    //Step12: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step13: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step14: Audiocapture stop with invalid handle
    result = RMF_AudioCapture_Start(handle, &settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_HANDLE, TEST_RMF_OSAL_EINVAL);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_Stop()
*
* Probe various input scenarios for RMF_AudioCapture_Stop() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 011@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` to restart defaults | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Stop()` to stop capture, tests start->stop->start->stop sequence | valid handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_Stop()` to stop capture, tests start->stop->close->open->start->stop sequence | valid handle | RMF_SUCCESS | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings;

    gTestID = 11;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);

    //Step03: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step04: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step05: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step06: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step07: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    ///Step08: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step09: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step10: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step11: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step12: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Stop()
*
* Force various edge/error scenarios for RMF_AudioCapture_Stop() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 012@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Stop()` with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Stop()` to test API response when not in STARTED state | current handle | RMF_INVALID_STATE | Should pass |
* | 04 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Stop()` with invalid handle | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 07 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Stop()` repeatedly, violating state machine | current handle | RMF_INVALID_STATE | Should pass |
* | 09 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Stop()` with outdated handle | handle obtained in last open call | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Stop (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings;

    gTestID = 12;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Audiocapture stop before open
    result = RMF_AudioCapture_Stop(NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Audiocapture stop before start
    result = RMF_AudioCapture_Stop(handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EINVAL);

    //Step04: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step05:Audiocapture start with 
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step06: Audiocapture stop with null handle
    result = RMF_AudioCapture_Stop(NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step07: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step08: Audiocapture stop again
    result = RMF_AudioCapture_Stop(handle);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EINVAL);

    //Step09: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step10: Audiocapture stop after close
    result = RMF_AudioCapture_Stop(NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_GetStatus()
*
* Probe various input scenarios for RMF_AudioCapture_GetStatus() and verify that it executes in accordance with the specification.
* @note Few params(muted, paused, volume) of RMF_AudioCapture_Status is deprecated
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 013@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetStatus()` to check status of open interface | current handle | return RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Start()` to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_GetStatus()` to check current status of started interface | current handle, valid settings | return RMF_SUCCESS, RMF_AudioCapture_Status.started must be 1, format and samplingFreq must have valid values | Should pass |
* | 06 | Call `RMF_AudioCapture_Stop()` to stop the interface | current handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetStatus()` to check current status of stopped/open interface | current handle, valid settings | return RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to release resources after test | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings;
    RMF_AudioCapture_Status status;

    gTestID = 13;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: Get the status of interface
    result = RMF_AudioCapture_GetStatus (handle, &status);
    UT_ASSERT_EQUAL(0, status.started);

    //Step03: Get the default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step04: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step05: Get the status of interface and verify
    result = RMF_AudioCapture_GetStatus (handle, &status);
    UT_ASSERT_EQUAL(1, status.started);
    result = test_l1_validate_status_active(&status);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step06: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step07: Get the status of interface and verify
    result = RMF_AudioCapture_GetStatus (handle, &status);
    UT_ASSERT_EQUAL(0, status.started);

    //Step08: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetStatus()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetStatus() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 014@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_GetStatus()` before open | handle=NULL | return RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetStatus()` with invalid handle after open | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 04 | Call `RMF_AudioCapture_GetStatus()` with valid handle and invalid status after open | current handle, status=NULL | RMF_INVALID_PARM | Should pass |
* | 05 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetStatus()` with invalid handle after start | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 08 | Call `RMF_AudioCapture_GetStatus()` with valid handle and invalid status after start | current handle, status=NULL | RMF_INVALID_PARM | Should pass |
* | 09 | Call `RMF_AudioCapture_Stop()` to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_GetStatus()` with invalid handle after stop | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 11 | Call `RMF_AudioCapture_GetStatus()` with valid handle and invalid status after stop | current handle, status=NULL | RMF_INVALID_PARM | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_GetStatus()` with outdated handle after close | previously valid handle | RMF_INVALID_HANDLE | Should pass |
* | 14 | Call `RMF_AudioCapture_GetStatus()` with outdated handle and invalid status after close | previously valid handle, status=NULL | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetStatus (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings;
    RMF_AudioCapture_Status status;

    gTestID = 14;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Get status with invalid handle
    result = RMF_AudioCapture_GetStatus (NULL, &status);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: get the status of interface with invalid handle
    result = RMF_AudioCapture_GetStatus(NULL, &status);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);
    //Step04: get the status of interface with invalid pointer
    result = RMF_AudioCapture_GetStatus(handle, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step05: Get the default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step06: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step07: get the status of interface with invalid handle
    result = RMF_AudioCapture_GetStatus(NULL, &status);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);
    //Step08: get the status of interface with invalid pointer
    result = RMF_AudioCapture_GetStatus(handle, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step09: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step10: get the status of interface with invalid handle
    result = RMF_AudioCapture_GetStatus(NULL, &status);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);
    //Step11: get the status of interface with invalid pointer
    result = RMF_AudioCapture_GetStatus(handle, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step12: Close Interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step13: get the status of interface after close
    result = RMF_AudioCapture_GetStatus(handle, &status);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_HANDLE, TEST_RMF_OSAL_EINVAL);
    //Step14: get the status of interface after close
    result = RMF_AudioCapture_GetStatus(handle, NULL);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_HANDLE, TEST_RMF_OSAL_EINVAL);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test positive scenarios for RMF_AudioCapture_GetCurrentSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetCurrentSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 015@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above plus some essential modifications | current handle, settings = default settings, but increase delayCompensation_ms by 1000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that the settings that were applied in start call are currently in effect | current handle, valid settings | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 05 | Call `RMF_AudioCapture_Stop()` to stop the capture| current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Start()` with settings obtained above plus some essential modifications | current handle, settings = default settings, but increase delayCompensation_ms by 2000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that the settings that were applied in start call are currently in effect | current handle, valid settings | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 09 | Call `RMF_AudioCapture_Stop()` to stop the capture | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings, current_settings;

    gTestID = 15;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
    memset(&current_settings, 0, sizeof(current_settings));

    //Step01: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    settings.delayCompensation_ms += 1000; // Increase delay compensation before start to introduce a deviation from the default parameters.
    test_l1_prepare_dummy_start_settings(&settings);

    //Step03: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step04: Get the cussrent settings and compare with settings
    result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_compare_settings(&settings, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step05: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step06: Get default settings
    settings.delayCompensation_ms = 0; //Set baseline as 0 before calling getDefaultSettings.
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    settings.delayCompensation_ms += 2000; // Increase delay compensation before start to introduce a deviation from the default parameters.

    //Step07: Audiocapture start with settings
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step08: Get current settings and compare with settings
    result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_compare_settings(&settings, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step09: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    //Step10: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetCurrentSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetCurrentSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 016@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | handle=NULL, settings is a valid pointer | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | current handle, valid settings | RMF_INVALID_STATE | Should pass |
* | 04 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` with settings obtained above plus some essential modifications | current handle, settings = default settings, but add dummy data callback | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_GetCurrentSettings()` with invalid handle | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 07 | Call `RMF_AudioCapture_GetCurrentSettings()` with valid handle, but settings NULL | current handle, settings=NULL | RMF_INVALID_PARM | Should pass |
* | 08 | Call `RMF_AudioCapture_Stop()` to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | current handle, valid settings | RMF_INVALID_STATE | Should pass |
* | 10 | Call `RMF_AudioCapture_GetCurrentSettings()` with valid handle, but settings NULL | current handle, settings=NULL | RMF_INVALID_PARM | Should pass |
* | 11 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | handle=NULL, settings is a valid pointer | RMF_INVALID_HANDLE | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | previously valid handle, valid settings | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetCurrentSettings (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings settings, current_settings;

    gTestID = 16;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Get current settings with invalid handle
    result = RMF_AudioCapture_GetCurrentSettings(NULL, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step02: Open interface
    result = RMF_AudioCapture_Open(&handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Get current settings before start
    result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, RMF_SUCCESS);

    //Step04: Get default settings
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l1_validate_settings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    test_l1_prepare_dummy_start_settings(&settings);
    //Step05: Audiocapture start
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step06: Get current settings with invalid handle
    result = RMF_AudioCapture_GetCurrentSettings(NULL, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);
    //Step07: Get current settings with invalid pointer
    result = RMF_AudioCapture_GetCurrentSettings(handle, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step08: Audiocapture stop
    result = RMF_AudioCapture_Stop(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    //Step09: Get current settings after stop
    result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, RMF_SUCCESS);
    //Step10: Get current settings with invalid pointer
    result = RMF_AudioCapture_GetCurrentSettings(handle, NULL);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);
    //Step11: Get current settings with invalid handle
    result = RMF_AudioCapture_GetCurrentSettings(NULL, &current_settings);
    UT_ASSERT_EQUAL(result, RMF_INVALID_HANDLE);

    //Step12: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    //Step13: Get current settings after close
    result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_HANDLE, TEST_RMF_OSAL_EINVAL);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test positive scenarios with auxiliary audio for RMF_AudioCapture_Open_Type()
*
* Probe various input scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 017@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open_Type()` to check open->close->open sequence | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_auxiliary (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 17;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface with auxilary type
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step02: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: Open interface with auxilary type
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step04: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios with auxiliary audio for RMF_AudioCapture_Open_Type()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 018@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` with invalid handle | handle must be NULL, type=auxiliary | RMF_INVALID_PARM | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type=auxiliary | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Open_Type()` when aux is already open | handle must be a valid pointer, type=auxiliary | RMF_INVALID_STATE | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_auxiliary (void)
{
    RMF_AudioCaptureHandle handle;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 18;
    UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    //Step01: Open interface with invalid handle
    result = RMF_AudioCapture_Open_Type(NULL, RMF_AC_TYPE_AUXILIARY);
    UT_ASSERT_EQUAL(result, RMF_INVALID_PARM);

    //Step02: Open interface
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);

    //Step03: open interface again
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    CHECK_FOR_EXTENDED_ERROR_CODE(result, RMF_INVALID_STATE, TEST_RMF_OSAL_EBUSY);

    //Step04: Close interface
    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL_FATAL(result, RMF_SUCCESS);
    UT_LOG("\n Out  %s\n",__FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_rmfAudioCapture_register ( void )
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L1 rmfAudioCapture]", NULL, NULL );
    if ( NULL == pSuite )
    {
        return -1;
    }

    UT_add_test( pSuite, "RMF_Open_Type_primary_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_primary );
    UT_add_test( pSuite, "RMF_Open_Type_primary_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_primary );
    UT_add_test( pSuite, "RMF_Open_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open );
    UT_add_test( pSuite, "RMF_Open_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open );
    UT_add_test( pSuite, "RMF_Close_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close );
    UT_add_test( pSuite, "RMF_Close_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close );

    UT_add_test( pSuite, "RMF_GetDefaultSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings );
    UT_add_test( pSuite, "RMF_GetDefaultSettings_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings );


    UT_add_test( pSuite, "RMF_Start_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start );
    UT_add_test( pSuite, "RMF_Start_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Start );
    UT_add_test( pSuite, "RMF_Stop_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop );
    UT_add_test( pSuite, "RMF_Stop_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Stop );

    UT_add_test( pSuite, "RMF_GetStatus_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus );
    UT_add_test( pSuite, "RMF_GetStatus_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetStatus );

    UT_add_test( pSuite, "RMF_GetCurrentSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings );
    UT_add_test( pSuite, "RMF_GetCurrentSettings_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetCurrentSettings );

    if(true == test_l1_is_aux_capture_supported())
    {
        UT_add_test( pSuite, "RMF_Open_Type_auxiliary_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_auxiliary );
        UT_add_test( pSuite, "RMF_Open_Type_auxiliary_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_auxiliary );
    }

    return 0;
}

/** @} */ // End of RMF Audio Capture HAL Tests L1 File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK
