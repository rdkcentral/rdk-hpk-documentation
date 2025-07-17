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
 * @defgroup RMF_AudioCapture_HALTESTS_L3 RMF Audio Capture HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for RMF Audio Capture HAL :
 *
 * Level 3 module tests will perform module level testing by exercising the full set of APIs to validate various use cases.
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
* @file test_l3_rmfAudioCapture.c
*
*/

#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include <ut.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include <ut_kvp.h>

#include "rmfAudioCapture.h"

#define RMF_ASSERT assert
#define UT_LOG_MENU_INFO UT_LOG_INFO

#define MEASUREMENT_WINDOW_SECONDS 10 // Default duration for data capture test
#define MEASUREMENT_WINDOW_2MINUTES 120 // Default duration for jitter test
#define MONITOR_JITTER_MICROSECONDS 100000 //Default sleep interval to check jitter
#define DATA_RATE 192000    // Bytes per second

static int gTestGroup = 3;
static int gTestID = 1;
    
/* rmf_error */
const static ut_control_keyStringMapping_t rmfError_mapTable [] = {
  { "RMF_SUCCESS",                    (int32_t)RMF_SUCCESS               },
  { "RMF_ERROR",                      (int32_t)RMF_ERROR                 },
  { "RMF_INVALID_PARM",               (int32_t)RMF_INVALID_PARM          },
  { "RMF_INVALID_HANDLE",             (int32_t)RMF_INVALID_HANDLE        },
  { "RMF_NOT_INITIALIZED",            (int32_t)RMF_NOT_INITIALIZED       },
  { "RMF_INVALID_STATE",              (int32_t)RMF_INVALID_STATE         },
  {  NULL, -1 }
};

/* racFormat */
const static ut_control_keyStringMapping_t racFormatMappingTable [] = {
  { "racFormat_e16BitStereo",         (int32_t)racFormat_e16BitStereo    },
  { "racFormat_e24BitStereo",         (int32_t)racFormat_e24BitStereo    },
  { "racFormat_e16BitMonoLeft",       (int32_t)racFormat_e16BitMonoLeft  },
  { "racFormat_e16BitMonoRight",      (int32_t)racFormat_e16BitMonoRight },
  { "racFormat_e16BitMono",           (int32_t)racFormat_e16BitMono      },
  { "racFormat_e24Bit5_1",            (int32_t)racFormat_e24Bit5_1       },
  { "racFormat_eMax",                 (int32_t)racFormat_eMax            },
  {  NULL, -1 }
};

/* racFreq */
const static ut_control_keyStringMapping_t racFreqMappingTable [] = {
  { "racFreq_e16000",                  (int32_t)racFreq_e16000            },
  { "racFreq_e22050",                  (int32_t)racFreq_e22050            },
  { "racFreq_e24000",                  (int32_t)racFreq_e24000            },
  { "racFreq_e32000",                  (int32_t)racFreq_e32000            },
  { "racFreq_e44100",                  (int32_t)racFreq_e44100            },
  { "racFreq_e48000",                  (int32_t)racFreq_e48000            },
  { "racFreq_eMax",                    (int32_t)racFreq_eMax              },
  {  NULL, -1 }
};
    
/* Global variables */
typedef struct
{
    RMF_AudioCapture_Settings settings;
    RMF_AudioCaptureHandle handle;
    uint64_t buffer_size;
    uint32_t bytes_received;
    int32_t jitter_threshold; // Minimum threshold value to validate jitter against
    int32_t jitter_monitor_sleep_interval; // Jitter monitored once in given interval
    atomic_int cookie;
    pthread_t jitter_thread_id;
    uint8_t data_capture_test_duration; // Time for data capture
    uint8_t jitter_test_duration; // How long to test jitter levels
    unsigned char *data_buffer;
} RMF_audio_capture_struct;

RMF_audio_capture_struct gAudioCaptureData[2]; // 0 - primary, 1 - auxiliary

/**
 * @brief This function clears the stdin buffer.
 *
 * This function clears the stdin buffer.
 */
static void readAndDiscardRestOfLine(FILE* in)
{
   int c;
   while ( (c = fgetc(in)) != EOF && c != '\n');
}

static void readInt(int32_t *choice)
{
    scanf("%d", choice);
    readAndDiscardRestOfLine(stdin);
}

/**
 * @brief Function to extract values from RMF_AudioCapture_Settings
 *
 * This function is called when values from settings are required for other operations
 */
static rmf_Error getValuesFromSettings(RMF_AudioCapture_Settings *settings, uint16_t *num_channels, uint32_t *sampling_rate, uint16_t *bits_per_sample)
{    
    switch (settings->format)
    {
    case racFormat_e16BitStereo:
        *bits_per_sample = 16;
        *num_channels = 2;
        break;
    case racFormat_e24BitStereo:
        *bits_per_sample = 24;
        *num_channels = 2;
        break;
    case racFormat_e16BitMonoLeft:  // fall through
    case racFormat_e16BitMonoRight: // fall through
    case racFormat_e16BitMono:
        *bits_per_sample = 16;
        *num_channels = 1;
        break;
    case racFormat_e24Bit5_1:
        *bits_per_sample = 24;
        *num_channels = 6;
        break;
    default: // Unsupported format
        UT_LOG_DEBUG("Error: Invalid format detected.\n");
        return RMF_ERROR;
    }

    switch (settings->samplingFreq)
    {
    case racFreq_e16000:
        *sampling_rate = 16000;
        break;
    case racFreq_e22050:
        *sampling_rate = 22050;
        break;
    case racFreq_e24000:
        *sampling_rate = 24000;
        break;
    case racFreq_e32000:
        *sampling_rate = 32000;
        break;
    case racFreq_e44100:
        *sampling_rate = 44100;
        break;
    case racFreq_e48000:
        *sampling_rate = 48000;
        break;
    default: // unsupported sampling rate
        UT_LOG_DEBUG("Error: Invalid samping rate detected.\n");
        return RMF_ERROR;
    }    
    return RMF_SUCCESS;
}

/**
 * @brief Callback function for buffer ready
 *
 * This function is triggered when the audio samples are ready (for counting tests)
 */
static rmf_Error test_l3_counting_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;

    bool result = (AudioCaptureBuffer == NULL) || (context_blob == NULL) || (AudioCaptureBufferSize <= 0);
    if (result == true)
    {
        UT_LOG_ERROR ("Invalid values received in callback, audio capture failure");
    }
    RMF_ASSERT(result == false);

    ctx_data->bytes_received += AudioCaptureBufferSize;
    ctx_data->cookie = 1;

    return RMF_SUCCESS;
}

/**
 * @brief Function to set RMF_AudioCapture_Settings as required for the tests.
 *
 * This function is called to set buffer ready callback and caller context data
 */
static void test_l3_prepare_start_settings_for_data_counting(void *context_blob)
{
    UT_LOG_INFO("Setting byte counting cb buffer ready and caller context data");
    
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;
    
    ctx_data->settings.cbBufferReady = test_l3_counting_data_cb;
    ctx_data->settings.cbStatusChange = NULL;
    ctx_data->settings.cbBufferReadyParm = context_blob;
    
    ctx_data->buffer_size = 0;
    ctx_data->bytes_received = 0;
}

/**
 * @brief Callback function for buffer ready
 *
 * This function is triggered when the audio samples are ready (for tests that capture audio buffer)
 */
static rmf_Error test_l3_tracking_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;

    bool result = (AudioCaptureBuffer == NULL) || (context_blob == NULL) || (AudioCaptureBufferSize <= 0);
    if (result == true)
    {
        UT_LOG_ERROR ("Invalid values received in callback, audio capture failure");
    }
    RMF_ASSERT(result == false);

    ctx_data->cookie = 1;

    if ( ctx_data->bytes_received + AudioCaptureBufferSize > ctx_data->buffer_size)
    {
        int temp = ctx_data->buffer_size - ctx_data->bytes_received;
        if (temp <= 0) 
        {
            return RMF_ERROR; //If buffer is full after writing first X seconds, return error
        }
        AudioCaptureBufferSize = temp;
    }
    // Copy data into the global buffer
    memcpy(ctx_data->data_buffer + ctx_data->bytes_received, AudioCaptureBuffer, AudioCaptureBufferSize);
    ctx_data->bytes_received += AudioCaptureBufferSize;
    
    return RMF_SUCCESS;
}

/**
 * @brief Function to set RMF_AudioCapture_Settings as required for the tests.
 *
 * This function is called to set buffer ready callback and caller context data.
 */
static void test_l3_prepare_start_settings_for_data_tracking(void *context_blob)
{
    UT_LOG_INFO("Setting buffer saving cb buffer ready and caller context data");
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;
    ctx_data->settings.cbBufferReady = test_l3_tracking_data_cb;
    ctx_data->settings.cbStatusChange = NULL;
    ctx_data->settings.cbBufferReadyParm = context_blob;

    /* Calculate data size for audio data */
    uint16_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint16_t bits_per_sample = 0;
    ctx_data->data_buffer = NULL;
    ctx_data->buffer_size = 0;

    int32_t choice;
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("Enter test duration in seconds for data capture test :");
    UT_LOG_MENU_INFO("------------------------------------------");
    readInt(&choice);
    ctx_data->data_capture_test_duration = choice;
    if (choice <= 0) 
    {
        UT_LOG_ERROR("Invalid test duration, choosing default of %d seconds", MEASUREMENT_WINDOW_SECONDS);
        ctx_data->data_capture_test_duration = MEASUREMENT_WINDOW_SECONDS;
    }

    if (RMF_SUCCESS != getValuesFromSettings(&ctx_data->settings, &num_channels, &sampling_rate, &bits_per_sample))
    {
        ctx_data->buffer_size = ctx_data->data_capture_test_duration * DATA_RATE;
        UT_LOG_ERROR("Using default data rate of %d and calculated buffer size as %d", DATA_RATE, ctx_data->buffer_size);
    } else
    {    
        ctx_data->buffer_size = ctx_data->data_capture_test_duration * num_channels * sampling_rate * bits_per_sample / 8;
    }
    /* Allocate buffer to store audio data */
    ctx_data->data_buffer = (unsigned char *)malloc(ctx_data->buffer_size);
    if (ctx_data->data_buffer == NULL)
    {
        UT_LOG_ERROR("Aborting test - Error allocating buffer to store audio data");
    }
    RMF_ASSERT(ctx_data->data_buffer != NULL);
    
    ctx_data->bytes_received = 0;
}

/**
 * @brief Function to validate actual vs expected bytes received
 *
 * This function is called when data capture tests or jitter tests want to validate 
 * bytes received in given test duration
 */
static rmf_Error validateBytesReceived(void *context_blob, int testedTime)
{
    UT_LOG_INFO("Validate bytes received with expected bytes");
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;

    /* Get values based on settings */
    uint16_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint16_t bits_per_sample = 0;

    if (RMF_SUCCESS != getValuesFromSettings(&ctx_data->settings, &num_channels, &sampling_rate, &bits_per_sample) )
    {
        UT_LOG_DEBUG("Error: Invalid values detected in settings ! Not able to calculate data size !");
        return RMF_ERROR;
    }

    /* Check actual bytes received is over 90% of expected data size before  */
    uint64_t computed_bytes_received = testedTime * num_channels * sampling_rate * bits_per_sample / 8;
    double percentage_received = (double)ctx_data->bytes_received / (double)computed_bytes_received * 100;
    UT_LOG_DEBUG("Actual bytes received: %" PRIu32 ", Expected bytes received: %" PRIu64 ", Computed percentage: %f\n",
                 ctx_data->bytes_received, computed_bytes_received, percentage_received);
    if ((percentage_received <= 90.0) || (percentage_received >= 110.0))
    {
        UT_LOG_DEBUG("Error: data delivery does not meet tolerance!");
        return RMF_ERROR;
    }
    return RMF_SUCCESS;
}

/**
 * @brief Function to write captured audio data to a wav file
 *
 * This function is called after audio is captured, at end of test to create output file
 */
static rmf_Error test_l3_write_wav_file(void *context_blob, const char *filename)
{
    UT_LOG_INFO("Called test_l3_write_wav_file with output file name %s", filename);
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;
    
    /* Get values based on settings */
    uint16_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint16_t bits_per_sample = 0;
    uint32_t data_rate = 0;

    /* Validate if acceptable level of bytes received first */
    if (RMF_SUCCESS != validateBytesReceived((void *)context_blob, ctx_data->data_capture_test_duration) )
    {
        UT_LOG_ERROR ("Bytes received is not in acceptable levels. Output file will not be created !");
        return RMF_ERROR;
    }

    if (RMF_SUCCESS != getValuesFromSettings(&ctx_data->settings, &num_channels, &sampling_rate, &bits_per_sample) )
    {
        UT_LOG_DEBUG("Error: Invalid values detected in settings ! Not able to calculate data size !");
        if(ctx_data->data_buffer) 
        {
            free(ctx_data->data_buffer);
            ctx_data->data_buffer = NULL;
        }
        return RMF_ERROR;
    }

    /* Start writing wav file  */
    FILE *file = fopen(filename, "wb");
    if (file == NULL) 
    {
        UT_LOG_ERROR("Error with fopen for output wav file");
        if(ctx_data->data_buffer) 
        {
            free(ctx_data->data_buffer);
            ctx_data->data_buffer = NULL;
        }
        return RMF_ERROR;
    }
    uint32_t fmt_chunk_size = 16; // Size of the fmt chunk
    uint16_t audio_format = 1;  // PCM format
    
    /* Write WAV Header first */
    fwrite("RIFF", 1, 4, file);
    uint32_t fileSize = 36 + ctx_data->bytes_received;
    fwrite(&fileSize, 4, 1, file);
    fwrite("WAVE", 1, 4, file);
    fwrite("fmt ", 1, 4, file);
    fwrite(&fmt_chunk_size, 4, 1, file);
    fwrite(&audio_format, 2, 1, file);

    data_rate = sampling_rate * num_channels * bits_per_sample / 8;
    uint16_t block_align = num_channels * bits_per_sample / 8;
    fwrite(&num_channels, 2, 1, file);
    fwrite(&sampling_rate, 4, 1, file);
    fwrite(&data_rate, 4, 1, file);
    fwrite(&block_align, 2, 1, file);
    fwrite(&bits_per_sample, 2, 1, file);
    fwrite("data", 1, 4, file);
    fwrite(&ctx_data->bytes_received, 4, 1, file);

    /* Write PCM Data */
    fwrite(ctx_data->data_buffer, 1, ctx_data->bytes_received, file);

    fclose(file);
    if(ctx_data->data_buffer) 
    {
        free(ctx_data->data_buffer);
        ctx_data->data_buffer = NULL;
    }
    UT_LOG_INFO("test_l3_write_wav_file created output file : %s", filename);
    return RMF_SUCCESS;
}

/**
 * @brief Function to monitor bytes received to detect jitter
 *
 * This function checks byte received in intervals to make sure there is no jitter
 */
static void* monitorBufferCount(void* context_blob) 
{
    UT_LOG_INFO("Created thread to monitor buffer for jitter");
    RMF_audio_capture_struct *ctx_data = (RMF_audio_capture_struct *)context_blob;

    uint32_t bytes_received = 0;
    uint32_t difference_in_bytes = 0;
    time_t start_time = time(NULL);
    time_t end_time = start_time + ctx_data->jitter_test_duration;

    rmf_Error *result = malloc (sizeof(rmf_Error));
    if (result == NULL)
    {
        UT_LOG_ERROR ("malloc for storing rmf_Error failed, refer prints to confirm if jitter test passed");
    }

    while ((ctx_data->cookie == 1) && (time(NULL) < end_time))
    {
        difference_in_bytes = ctx_data->bytes_received - bytes_received;
        if (difference_in_bytes < ctx_data->jitter_threshold) 
        {
            UT_LOG_INFO ("Bytes received in last iteration : %d. This is less than threshold level of %d bytes.\n", difference_in_bytes, ctx_data->jitter_threshold);
            UT_LOG_ERROR ("Jitter detected !");
            *result = RMF_ERROR;
            return (void *)result;
        }
        bytes_received = ctx_data->bytes_received;
        usleep (ctx_data->jitter_monitor_sleep_interval);
    }
    UT_LOG_INFO("No jitter detected");
    *result = RMF_SUCCESS;
    return (void *)result;
}

/**
 * @brief Function to choose if test steps are for Primary/Auxiliary audio capture
 *
 * This function requires user to choose audio capture type.
 */
static int getAudioCaptureType(void)
{
    int32_t choice = 1;

    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("Supported RMF audio capture types are:");
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("\t#   %-20s","Supported capture type");
    UT_LOG_MENU_INFO("\t1.  %-20s","PRIMARY");
    UT_LOG_MENU_INFO("\t2.  %-20s","AUXILIARY");
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO(" Select the audio capture type :");
    readInt(&choice);

    if (choice < 1 || choice > 2)
    {
        UT_LOG_ERROR("Invalid capture type choice, choosing default : PRIMARY \n");
        choice = 1;
    }
    return choice;
}

/**
* @brief This test opens the audio capture interface
*
* This test function opens the audio capture interface and gets a handle.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_open_handle(void)
{
    gTestID = 1;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCaptureType rmfAcType = RMF_AC_TYPE_PRIMARY;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    switch(choice)
    {
        case 1:
        {
            rmfAcType = RMF_AC_TYPE_PRIMARY;
            break;
        }
        case 2:
        {
            rmfAcType = RMF_AC_TYPE_AUXILIARY;
            break;
        }
        default :
            UT_LOG_ERROR("Invalid handle choice\n");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:handle:[])", rmfAcType);
    result = RMF_AudioCapture_Open_Type(&gAudioCaptureData[audioCaptureIndex].handle, rmfAcType);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:handle:[0x%0X]) rmf_error:[%s]", rmfAcType, &gAudioCaptureData[audioCaptureIndex].handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_ERROR("Aborting test - unable to open capture.");
    }
    RMF_ASSERT(RMF_SUCCESS == result);
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test gets default values of RMF_AudioCapture_Settings and allows values to be updated.
*
* This test function gets default values of RMF_AudioCapture_Settings and allows values to be updated.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_update_settings(void)
{
    gTestID = 2;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    rmf_Error result = RMF_SUCCESS;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&gAudioCaptureData[audioCaptureIndex].settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:settings:[0x%0X]) rmf_error:[%s]", &gAudioCaptureData[audioCaptureIndex].settings, UT_Control_GetMapString(rmfError_mapTable, result));
    RMF_ASSERT(result == RMF_SUCCESS);

    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("Current values in settings :");
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("\t#   %-20s  %s","Settings", "Default Values");
    UT_LOG_MENU_INFO("\t1.  %-20s  %s","Capture Format", UT_Control_GetMapString(racFormatMappingTable, gAudioCaptureData[audioCaptureIndex].settings.format));
    UT_LOG_MENU_INFO("\t2.  %-20s  %s","Sampling Frequency", UT_Control_GetMapString(racFreqMappingTable, gAudioCaptureData[audioCaptureIndex].settings.samplingFreq));
    UT_LOG_MENU_INFO("\t3.  %-20s  %d","FIFO size", gAudioCaptureData[audioCaptureIndex].settings.fifoSize);
    UT_LOG_MENU_INFO("\t4.  %-20s  %d","Threshold", gAudioCaptureData[audioCaptureIndex].settings.threshold);
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO(" Do you want to update default settings ? (0 for No, 1 for Yes)");
    readInt(&choice);

    switch(choice)
    {
        case 0:
        {
            UT_LOG_INFO("Using above default values for settings");
            break;
        }
        case 1:
        {
            UT_LOG_MENU_INFO("------------------------------------------");
            UT_LOG_MENU_INFO("\t\t Supported RMF Audio Capture Formats ");
            UT_LOG_MENU_INFO("------------------------------------------");
            UT_LOG_MENU_INFO("\t#   %-30s","Capture format");
            for(int32_t i = racFormat_e16BitStereo; i < racFormat_eMax; i++)
            {
                UT_LOG_MENU_INFO("\t%d.  %-30s", i, UT_Control_GetMapString(racFormatMappingTable, i));
            }
            UT_LOG_MENU_INFO("------------------------------------------");
            UT_LOG_MENU_INFO(" Select the capture format to update, use -1 to retain default value :");
            readInt(&choice);

            if (choice != -1)
            {
                if(choice < racFormat_e16BitStereo || choice >= racFormat_eMax)
                {
                    UT_LOG_ERROR("Invalid Capture format, try again");
                    break;
                }
                gAudioCaptureData[audioCaptureIndex].settings.format = choice;
            }
            
            UT_LOG_MENU_INFO("------------------------------------------");
            UT_LOG_MENU_INFO("\t\t Supported RMF Audio Capture Sampling Rates ");
            UT_LOG_MENU_INFO("------------------------------------------");
            UT_LOG_MENU_INFO("\t#   %-30s","Sampling Rate");
            for(int32_t i = racFreq_e16000; i < racFreq_eMax; i++)
            {
                UT_LOG_MENU_INFO("\t%d.  %-30s", i, UT_Control_GetMapString(racFreqMappingTable, i));
            }
            UT_LOG_MENU_INFO("------------------------------------------");
            UT_LOG_MENU_INFO(" Select the Sampling Rate, use -1 to retain default value :");
            readInt(&choice);

            if (choice != -1)
            {
                if(choice < racFreq_e16000 || choice >= racFreq_eMax)
                {
                    UT_LOG_ERROR("Invalid Sampling Rate, try again");
                    break;
                }
                gAudioCaptureData[audioCaptureIndex].settings.samplingFreq = choice;
            }
            
            UT_LOG_MENU_INFO(" Enter FIFO size in bytes, use -1 to retain default value :");
            readInt(&choice);

            if (choice != -1)
            {
                if(choice <= 0 )
                {
                    UT_LOG_ERROR("Invalid FIFO size, try again");
                    break;
                }
                gAudioCaptureData[audioCaptureIndex].settings.fifoSize = choice;
            }

            UT_LOG_MENU_INFO(" Enter data callback threshold in bytes, used to check jitter (max 1/4th of FIFO), use -1 to retain default value :");
            readInt(&choice);

            if (choice != -1)
            {
                if(choice <= 0)
                {
                    UT_LOG_ERROR("Invalid threshold size, try again");
                    break;
                }
                gAudioCaptureData[audioCaptureIndex].settings.threshold = choice;
            }            

            break;
        }
        default :
            UT_LOG_ERROR("Invalid settings choice\n");
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test sets up buffer ready callbacks for counting, tracking
*
* This test function sets up buffer ready callbacks for counting, tracking
*
* **Test Group ID:** 03@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_setup_callbacks(void)
{
    gTestID = 3;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("Choose the type of test :");
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("\t#   %-20s","Supported types of test");
    UT_LOG_MENU_INFO("\t1.  %-20s","Byte counting tests (only bytes received is checked)");
    UT_LOG_MENU_INFO("\t2.  %-20s","Data capture tests (audio data is captured) ");
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("Select the type of test: ");
    readInt(&choice);

    switch(choice)
    {
        case 1:
        {
            test_l3_prepare_start_settings_for_data_counting(&gAudioCaptureData[audioCaptureIndex]);
            break;
        }
        case 2:
        {
            test_l3_prepare_start_settings_for_data_tracking(&gAudioCaptureData[audioCaptureIndex]);
            break;
        }
        default :
            UT_LOG_ERROR("Invalid callback type choice, callback not set up\n");
    }
    
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test starts audio capture
*
* This test starts audio capture
*
* **Test Group ID:** 03@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_start(void)
{
    gTestID = 4;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X])", &gAudioCaptureData[audioCaptureIndex].handle, &gAudioCaptureData[audioCaptureIndex].settings);
    result = RMF_AudioCapture_Start(gAudioCaptureData[audioCaptureIndex].handle, &gAudioCaptureData[audioCaptureIndex].settings);
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &gAudioCaptureData[audioCaptureIndex].handle, &gAudioCaptureData[audioCaptureIndex].settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(gAudioCaptureData[audioCaptureIndex].handle);
        if(gAudioCaptureData[audioCaptureIndex].data_buffer) 
        {
            free(gAudioCaptureData[audioCaptureIndex].data_buffer);
            gAudioCaptureData[audioCaptureIndex].data_buffer = NULL;
        }
        UT_LOG_ERROR("Aborting test - unable to start capture.");
    }
    RMF_ASSERT(result == RMF_SUCCESS);
    
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test returns bytes received so far
*
* This test returns bytes received so far
*
* **Test Group ID:** 03@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_bytes_received(void)
{
    gTestID = 5;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Bytes Received for PRIMARY capture %d\n", gAudioCaptureData[0].bytes_received);
    UT_LOG_INFO("Bytes Received for AUXILIARY capture %d\n", gAudioCaptureData[1].bytes_received);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test writes captured audio data to a wav file
*
* This test writes captured audio data to a wav file
*
* **Test Group ID:** 03@n
* **Test Case ID:** 006@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_write_output_file(void)
{
    gTestID = 6;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;

    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    char filepath[100];
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO("Enter file name and location to create output filename (example - /tmp/output.wav) :");
    UT_LOG_MENU_INFO("------------------------------------------");
    if (fgets(filepath, sizeof(filepath), stdin) != NULL) {
        // Remove newline character if present
        size_t len = strlen(filepath);
        if (len > 0 && filepath[len - 1] == '\n') {
            filepath[len - 1] = '\0';
        }
    } else {
        UT_LOG_ERROR("Error reading input, choosing default file path and location : /tmp/output.wav\n");
        strcpy(filepath, "/tmp/output.wav");
    }
    // Check if the filename ends with .wav
    const char *ext = strrchr(filepath, '.');
    if (ext == NULL || strcmp(ext, ".wav") != 0) {
        UT_LOG_ERROR("Provided file name is not a .wav file, choosing default file path and location : /tmp/output.wav\n");
        strcpy(filepath, "/tmp/output.wav");
    }

    result = test_l3_write_wav_file((void *)&gAudioCaptureData[audioCaptureIndex], filepath);
    RMF_ASSERT(result == RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test starts jitter test by creating a thread to monitor bytes received
*
* This test starts jitter test by creating a thread to monitor bytes received
*
* **Test Group ID:** 03@n
* **Test Case ID:** 007@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_jitter_monitor(void)
{
    gTestID = 7;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary
    int result = 0;

    UT_LOG_MENU_INFO("Enter minimum threshold in bytes to check jitter : ");
    readInt(&choice);

    if(choice <= 0)
    {
        UT_LOG_ERROR("Invalid threshold size, retaining value : %d", gAudioCaptureData[audioCaptureIndex].jitter_threshold);
    } 
    else 
    {
        gAudioCaptureData[audioCaptureIndex].jitter_threshold = choice;
    }

    UT_LOG_MENU_INFO("Enter interval in microseconds to monitor buffer for jitter : ");
    readInt(&choice);
    gAudioCaptureData[audioCaptureIndex].jitter_monitor_sleep_interval = choice;

    if(choice <= 0)
    {
        gAudioCaptureData[audioCaptureIndex].jitter_monitor_sleep_interval = MONITOR_JITTER_MICROSECONDS;
        UT_LOG_ERROR("Invalid sleep interval, setting a default value of %d microseconds", gAudioCaptureData[audioCaptureIndex].jitter_monitor_sleep_interval);
    }

    UT_LOG_MENU_INFO("Enter test duration in seconds for jitter test : ");
    readInt(&choice);
    gAudioCaptureData[audioCaptureIndex].jitter_test_duration = choice;

    if(choice <= 0)
    {
        gAudioCaptureData[audioCaptureIndex].jitter_test_duration = MEASUREMENT_WINDOW_2MINUTES;
        UT_LOG_ERROR("Invalid test duration, setting a default value of %d seconds", gAudioCaptureData[audioCaptureIndex].jitter_test_duration);
    }

    result = pthread_create(&gAudioCaptureData[audioCaptureIndex].jitter_thread_id, NULL, monitorBufferCount, (void *)&gAudioCaptureData[audioCaptureIndex]);
    if (result != 0)
    {
        UT_LOG_ERROR("Aborting test - Failed to create monitor thread");
    }
    RMF_ASSERT (result == 0);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test checks return state of jitter thread created and validates bytes received
*
* This test checks return state of jitter thread created and validates bytes received
*
* **Test Group ID:** 03@n
* **Test Case ID:** 008@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_jitter_result(void)
{
    gTestID = 8;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary
    void *ret_value;

    if (pthread_join(gAudioCaptureData[audioCaptureIndex].jitter_thread_id, &ret_value) != 0) 
    {
        UT_LOG_INFO("Error joining monitor thread");
    }
    if (ret_value != NULL) 
    {
        result = *(rmf_Error *)ret_value;
        if (result != RMF_SUCCESS)
        {
            UT_LOG_ERROR("Jitter Detected !");
        }
    
        free(ret_value);
    } else 
    {
        UT_LOG_INFO("Thread ret_value NULL, unable to assert for jitter check. Refer prints to confirm if test passed");
    }

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test gets current audio capture settings
*
* This test gets current audio capture settings
*
* **Test Group ID:** 03@n
* **Test Case ID:** 009@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_getCurrent_settings(void)
{
    gTestID = 9;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Settings current_settings;
    memset(&current_settings, 0, sizeof(current_settings));
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_INFO("Calling RMF_AudioCapture_GetCurrentSettings(IN:handle:[0x%0X] OUT:settings:[])", &gAudioCaptureData[audioCaptureIndex].handle);
    result = RMF_AudioCapture_GetCurrentSettings(gAudioCaptureData[audioCaptureIndex].handle, &current_settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetCurrentSettings(IN:handle:[0x%0X] OUT:settings:[0x%0X] settings.cbBufferReady:[0x%0X] settings.cbBufferReadyParm:[0x%0X]", &gAudioCaptureData[audioCaptureIndex].handle, &current_settings, (void*)current_settings.cbBufferReady, current_settings.cbBufferReadyParm);
    UT_LOG_INFO("Result RMF_AudioCapture_GetCurrentSettings(OUT:settings.cbStatusChange:[0x%0X] settings.cbStatusParm:[0x%0X] settings.fifoSize:[%zu]", (void *)current_settings.cbStatusChange, current_settings.cbStatusParm, current_settings.fifoSize);
    UT_LOG_INFO("Result RMF_AudioCapture_GetCurrentSettings(OUT: settings.threshold:[%zu] settings.racFormat:[%d] settings.racFreq:[%d] settings.delayCompensation_ms:[%u]", current_settings.threshold, current_settings.format, current_settings.samplingFreq, current_settings.delayCompensation_ms);
    RMF_ASSERT(result == RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}


/**
* @brief This test gets audio capture status
*
* This test gets audio capture status
*
* **Test Group ID:** 03@n
* **Test Case ID:** 0010@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_get_status(void)
{
    gTestID = 10;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;
    RMF_AudioCapture_Status status;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_INFO("Calling RMF_AudioCapture_GetStatus(IN:handle:[0x%0X] OUT:status:[])", &gAudioCaptureData[audioCaptureIndex].handle);
    result = RMF_AudioCapture_GetStatus(gAudioCaptureData[audioCaptureIndex].handle, &status);
    UT_LOG_INFO("Result RMF_AudioCapture_GetStatus(IN:handle:[0x%0X] OUT:status:[0x%0X] status.started:[%d] status.racFormat:[%d] status.racFreq:[%d] status.fifoDepth:[%zu]", &gAudioCaptureData[audioCaptureIndex].handle, &status, status.started, status.format, status.samplingFreq, status.fifoDepth);
    UT_LOG_INFO("Result RMF_AudioCapture_GetStatus(IN:handle:[0x%0X] OUT:status:[0x%0X] status.overflows:[%u] status.underflows:[%u]", &gAudioCaptureData[audioCaptureIndex].handle, &status, status.overflows, status.underflows);
    RMF_ASSERT(result == RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test stops audio capture
*
* This test stops audio capture
*
* **Test Group ID:** 03@n
* **Test Case ID:** 0011@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_stop(void)
{
    gTestID = 11;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:handle:[0x%0X])", &gAudioCaptureData[audioCaptureIndex].handle);
    result = RMF_AudioCapture_Stop(gAudioCaptureData[audioCaptureIndex].handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &gAudioCaptureData[audioCaptureIndex].handle, UT_Control_GetMapString(rmfError_mapTable, result));
    gAudioCaptureData[audioCaptureIndex].cookie = 0;
    RMF_ASSERT(result == RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    RMF_ASSERT(gAudioCaptureData[audioCaptureIndex].cookie == 0);
    
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test closes audio capture interface
*
* This test closes audio capture interface
*
* **Test Group ID:** 03@n
* **Test Case ID:** 0012@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L3-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L3-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_close(void)
{
    gTestID = 12;

    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);
    rmf_Error result = RMF_SUCCESS;
    int32_t choice = getAudioCaptureType();
    int audioCaptureIndex = choice - 1; //0 - primary, 1 - auxiliary

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:handle:[0x%0X])", &gAudioCaptureData[audioCaptureIndex].handle);
    result = RMF_AudioCapture_Close(gAudioCaptureData[audioCaptureIndex].handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &gAudioCaptureData[audioCaptureIndex].handle, UT_Control_GetMapString(rmfError_mapTable, result));
    RMF_ASSERT(result == RMF_SUCCESS);
    
    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_rmfAudioCapture_l3_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite_withGroupID("[L3 rmfAudioCapture]", NULL, NULL, UT_TESTS_L3);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings
    UT_add_test(pSuite, "Open RMF Audio Capture Handle", test_l3_rmfAudioCapture_open_handle);
    UT_add_test(pSuite, "Get and update default settings", test_l3_rmfAudioCapture_update_settings);
    UT_add_test(pSuite, "Select the type of test", test_l3_rmfAudioCapture_setup_callbacks);
    UT_add_test(pSuite, "Start RMF Audio Capture", test_l3_rmfAudioCapture_start);
    UT_add_test(pSuite, "Check Bytes Received", test_l3_rmfAudioCapture_bytes_received);
    UT_add_test(pSuite, "Write output wav file", test_l3_write_output_file);
    UT_add_test(pSuite, "Start Jitter test", test_l3_jitter_monitor);
    UT_add_test(pSuite, "Check jitter test result", test_l3_jitter_result);
    UT_add_test(pSuite, "Get current settings", test_l3_rmfAudioCapture_getCurrent_settings);
    UT_add_test(pSuite, "Get RMF Audio Capture status", test_l3_rmfAudioCapture_get_status);
    UT_add_test(pSuite, "Stop RMF Audio Capture", test_l3_rmfAudioCapture_stop);
    UT_add_test(pSuite, "Close RMF Audio Capture Handle", test_l3_rmfAudioCapture_close);
    
    return 0;
}

/** @} */ // End of RMF Audio Capture HAL Tests L3 File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK
