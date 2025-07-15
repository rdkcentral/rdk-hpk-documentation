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
 * @defgroup RMF_AudioCapture_HALTESTS_L2 RMF Audio Capture HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for RMF Audio Capture HAL :
 *
 * Level 2 module tests will perform module level testing by exercising the full set of APIs to validate various use cases.
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
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
* @file test_l2_rmfAudioCapture.c
*
*/


#include <ut.h>
#include <ut_cunit.h>
#include <ut_kvp_profile.h>
#include <unistd.h>
#include <stdatomic.h>
#include "rmfAudioCapture.h"


#define MEASUREMENT_WINDOW_SECONDS 10

static int gTestGroup = 2;
static int gTestID = 1;

typedef struct
{
    uint64_t bytes_received;
    atomic_int cookie;
} capture_session_context_t;

static bool g_aux_capture_supported = false;

static rmf_Error test_l2_counting_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;

    UT_ASSERT_PTR_NOT_NULL(AudioCaptureBuffer);
    UT_ASSERT_PTR_NOT_NULL_FATAL(context_blob);
    UT_ASSERT_TRUE(AudioCaptureBufferSize > 0);

    ctx->bytes_received += AudioCaptureBufferSize;
    ctx->cookie = 1;
    return RMF_SUCCESS;
}

static void test_l2_prepare_start_settings_for_data_tracking(RMF_AudioCapture_Settings *settings, void *context_blob)
{
    settings->cbBufferReady = test_l2_counting_data_cb;
    settings->cbStatusChange = NULL;
    settings->cbBufferReadyParm = context_blob;
}

static rmf_Error test_l2_validate_bytes_received(RMF_AudioCapture_Settings *settings, uint32_t seconds, uint64_t bytes_received)
{
    uint8_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint8_t bits_per_sample = 0;
    switch (settings->format)
    {
    case racFormat_e16BitStereo:
        bits_per_sample = 16;
        num_channels = 2;
        break;
    case racFormat_e24BitStereo:
        bits_per_sample = 24;
        num_channels = 2;
        break;
    case racFormat_e16BitMonoLeft:  // fall through
    case racFormat_e16BitMonoRight: // fall through
    case racFormat_e16BitMono:
        bits_per_sample = 16;
        num_channels = 1;
        break;
    case racFormat_e24Bit5_1:
        bits_per_sample = 24;
        num_channels = 6;
        break;
    default: // Unsupported format
        UT_LOG_DEBUG("Error: Invalid format detected.\n");
        return RMF_ERROR;
    }

    switch (settings->samplingFreq)
    {
    case racFreq_e16000:
        sampling_rate = 16000;
        break;
    case racFreq_e22050:
        sampling_rate = 22050;
        break;
    case racFreq_e24000:
        sampling_rate = 24000;
        break;
    case racFreq_e32000:
        sampling_rate = 32000;
        break;
    case racFreq_e44100:
        sampling_rate = 44100;
        break;
    case racFreq_e48000:
        sampling_rate = 48000;
        break;
    default: // unsupported sampling rate
        UT_LOG_DEBUG("Error: Invalid samping rate detected.\n");
        return RMF_ERROR;
    }

    uint64_t computed_bytes_received = seconds * num_channels * sampling_rate * bits_per_sample / 8;
    double percentage_received = (double)bytes_received / (double)computed_bytes_received * 100;
    UT_LOG_DEBUG("Actual bytes received: %" PRIu64 ", Expected bytes received: %" PRIu64 ", Computed percentage: %f\n",
                 bytes_received, computed_bytes_received, percentage_received);
    if ((90.0 <= percentage_received) && (110.0 >= percentage_received))
        return RMF_SUCCESS;
    else
    {
        UT_LOG_DEBUG("Error: data delivery does not meet tolerance!");
        return RMF_ERROR;
    }
}

/**
* @brief Test the primary audio capture functionality
*
* This test verifies the primary audio capture functionality by opening an audio capture handle,
* getting default settings, modifying settings, starting the capture, capturing audio for a duration,
* stopping the capture, and finally closing the handle. The test ensures that each step returns
* the expected success status and that the callbacks are invoked correctly.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l2_rmfAudioCapture_primary_data_check(void)
{
    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    result = RMF_AudioCapture_Open(&handle);
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }


    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    capture_session_context_t ctx = {0, 0};
    test_l2_prepare_start_settings_for_data_tracking(&settings, (void *)&ctx);

    result = RMF_AudioCapture_Start(handle, &settings);
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    sleep(MEASUREMENT_WINDOW_SECONDS);
    result = RMF_AudioCapture_Stop(handle);
    ctx.cookie = 0; // Note: Doesn't account for all possible race conditions
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(ctx.cookie, 0);

    result = test_l2_validate_bytes_received(&settings, MEASUREMENT_WINDOW_SECONDS, ctx.bytes_received);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the auxiliary audio capture functionality
*
* This test verifies the complete flow of auxiliary audio capture including opening the capture,
* getting default settings, modifying settings, starting capture, capturing audio, stopping capture,
* and closing the capture. It ensures that each step returns the expected success status.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l2_rmfAudioCapture_auxiliary_data_check(void)
{
    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    capture_session_context_t ctx = {0, 0};
    test_l2_prepare_start_settings_for_data_tracking(&settings, (void *)&ctx);

    result = RMF_AudioCapture_Start(handle, &settings);
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }
    sleep(MEASUREMENT_WINDOW_SECONDS);
    result = RMF_AudioCapture_Stop(handle);
    ctx.cookie = 0; // Note: Doesn't account for all possible race conditions
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(ctx.cookie, 0);

    result = test_l2_validate_bytes_received(&settings, MEASUREMENT_WINDOW_SECONDS, ctx.bytes_received);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    result = RMF_AudioCapture_Close(handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the functionality of running auxiliary and primary audio capture simultaneously
*
* This test verifies the ability to open, start, capture, and stop audio capture on both primary and auxiliary interfaces. It ensures that the audio capture handles are correctly opened, the default settings are retrieved and modified, and audio capture is successfully started and stopped. The test also verifies that the appropriate callbacks are invoked and no more callbacks are issued after stopping the capture.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l2_rmfAudioCapture_combined_data_check(void)
{
    RMF_AudioCaptureHandle aux_handle, prim_handle;
    RMF_AudioCapture_Settings aux_settings, prim_settings;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    result = RMF_AudioCapture_GetDefaultSettings(&aux_settings);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    prim_settings = aux_settings;
    capture_session_context_t prim_ctx = {0, 0};
    capture_session_context_t aux_ctx = {0, 0};
    test_l2_prepare_start_settings_for_data_tracking(&aux_settings, (void *)&aux_ctx);
    test_l2_prepare_start_settings_for_data_tracking(&prim_settings, (void *)&prim_ctx);

    result = RMF_AudioCapture_Start(aux_handle, &aux_settings); // Started auxiliary capture
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to open primary capture interface. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to open primary capture interface.");
    }
    result = RMF_AudioCapture_Start(prim_handle, &prim_settings); // Started primary capture
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to start primary capture. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        result = RMF_AudioCapture_Close(prim_handle);
        UT_FAIL_FATAL("Aborting test - unable to start primary capture.");
    }
    sleep(MEASUREMENT_WINDOW_SECONDS);

    result = RMF_AudioCapture_Stop(prim_handle);
    prim_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = RMF_AudioCapture_Stop(aux_handle);
    aux_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(prim_ctx.cookie, 0);
    UT_ASSERT_EQUAL(aux_ctx.cookie, 0);

    result = test_l2_validate_bytes_received(&aux_settings, MEASUREMENT_WINDOW_SECONDS, aux_ctx.bytes_received);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l2_validate_bytes_received(&prim_settings, MEASUREMENT_WINDOW_SECONDS, prim_ctx.bytes_received);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    result = RMF_AudioCapture_Close(prim_handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = RMF_AudioCapture_Close(aux_handle);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_rmfAudioCapture_l2_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L2 rmfAudioCapture]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings
    UT_add_test(pSuite, "test_l2_rmfAudioCapture_primary_data_check", test_l2_rmfAudioCapture_primary_data_check);
    g_aux_capture_supported = ut_kvp_getBoolField(ut_kvp_profile_getInstance(), "rmfaudiocapture/features/auxsupport");
    if (true == g_aux_capture_supported)
    {
        UT_add_test(pSuite, "test_l2_rmfAudioCapture_auxiliary_data_check", test_l2_rmfAudioCapture_auxiliary_data_check);
        UT_add_test(pSuite, "test_l2_rmfAudioCapture_combined_data_check", test_l2_rmfAudioCapture_combined_data_check);
    }

    return 0;
}

/** @} */ // End of RMF Audio Capture HAL Tests L2 File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK
