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
 * @defgroup DS_VIdeoDevice_HALTEST Device Settings VIdeo Device HAL Tests
 * @{
 */

/**
 * @defgroup DS_VIdeoDevice_HALTEST_PARSE_CONFIG Device Settings VIdeo Device Parse Config File
 * @{
 * @parblock
 *
 * ### Configuration file for DS VIdeo Device test suit :
 *
 *  DS VIdeo Device Parse Configuration file
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 *
 * @endparblock
 */

/**
 * @file test_dsVideoDevice_parse_configuration.c
 *
 */
#include <stdlib.h>
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>

#include "test_parse_configuration.h"
#include "test_dsVideoDevice_parse_configuration.h"

/* Global variables */
dsVideoDeviceConfiguration_t* gDSVideoDeviceConfiguration = NULL;
char gDSVideoDeviceName[DS_VIDEO_DEVICE_NAME_SIZE]      = {0};
int32_t gDSvideoDevice_NumVideoDevices = 0;


/* Parse Video Device Configuration file */
int test_dsVideoDevice_parse_configuration()
{
    char key_string[DS_VIDEO_DEVICE_KVP_SIZE];
    ut_kvp_status_t status;

    gDSvideoDevice_NumVideoDevices = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), "dsVideoDevice/NumVideoDevices" );

    UT_LOG_DEBUG("gDSvideoDevice_NumVideoDevices: %d",gDSvideoDevice_NumVideoDevices);

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsVideoDevice/Type", gDeviceType, TEST_DS_DEVICE_TYPE_SIZE);
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
    else
    {
        UT_LOG_ERROR("Failed to get the platform type");
        return -1;
    }

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsVideoDevice/Name", gDSVideoDeviceName, DS_VIDEO_DEVICE_NAME_SIZE);
    if (status == UT_KVP_STATUS_SUCCESS )
    {
        UT_LOG_DEBUG("Device Type: %s, Device Name: %s", gDeviceType, gDSVideoDeviceName);
    }
    else
    {
        UT_LOG_ERROR("Failed to get the Device Name ");
        return -1;
    }

    gDSVideoDeviceConfiguration = (dsVideoDeviceConfiguration_t*) calloc(gDSvideoDevice_NumVideoDevices, sizeof(dsVideoDeviceConfiguration_t));
    if(gDSVideoDeviceConfiguration == NULL)
    {
         UT_LOG_ERROR("Failed to allocate memory for Video Device configuration structure");
         return -1;
    }

    for(int i = 0; i < gDSvideoDevice_NumVideoDevices; i++)
    {
        if(gSourceType == 1)
        {
            snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/SupportedDFCs" , i+1);
            gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs = ut_kvp_getListCount(ut_kvp_profile_getInstance(), key_string);
            UT_LOG_DEBUG("NoOfSupportedDFCs: %d",gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs);

            // loop to get supported SupportedDFCs in array
            for(int j = 0; j < gDSVideoDeviceConfiguration[i].NoOfSupportedDFCs; j++)
            {
                snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/SupportedDFCs/%d" , i+1 , j);
                gDSVideoDeviceConfiguration[i].SupportedDFCs[j] = (dsVideoZoom_t)ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string);
                UT_LOG_DEBUG("SupportedDFCs: %d",gDSVideoDeviceConfiguration[i].SupportedDFCs[j]);
            }
        }

        snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/DefaultDFC" , i+1);
        gDSVideoDeviceConfiguration[i].DefaultDFC = (dsVideoZoom_t)ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string );
        UT_LOG_DEBUG("gDSVideoDeviceConfiguration[i].DefaultDFC :%d",gDSVideoDeviceConfiguration[i].DefaultDFC);

        snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/HDRCapabilities" , i+1);
        gDSVideoDeviceConfiguration[i].HDRCapabilities = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string );
        UT_LOG_DEBUG("gDSVideoDeviceConfiguration[i].HDRCapabilities :%d",gDSVideoDeviceConfiguration[i].HDRCapabilities);

        snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/SupportedVideoCodingFormats" , i+1);
        gDSVideoDeviceConfiguration[i].SupportedVideoCodingFormats = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string );
        UT_LOG_DEBUG("SupportedVideoCodingFormats: %d ",gDSVideoDeviceConfiguration[i].SupportedVideoCodingFormats);

        if(gSourceType == 0){
            snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/SupportedDisplayFramerate" , i+1);
            gDSVideoDeviceConfiguration[i].NoOfSupportedDFR = ut_kvp_getListCount(ut_kvp_profile_getInstance(), key_string);
            UT_LOG_DEBUG("NoOfSupportedDFR: %d ",gDSVideoDeviceConfiguration[i].NoOfSupportedDFR);
            // loop to get supported SupportedDFR in array
            for(int j = 0; j < gDSVideoDeviceConfiguration[i].NoOfSupportedDFR; j++)
            {
                snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/SupportedDisplayFramerate/%d" , i+1 , j);
                status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), key_string, gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j], sizeof(gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j]));
                if (status == UT_KVP_STATUS_SUCCESS )
                {
                    UT_LOG_DEBUG("SupportedDisplayFramerate: %s ",gDSVideoDeviceConfiguration[i].SupportedDisplayFramerate[j]);
                }
                else
                {
                    UT_LOG_ERROR("Failed to get the platform Device SupportedDisplayFramerate");
                    return -1;
                }
            }
        }

        UT_LOG_DEBUG("gSourceType %d ",gSourceType);
        /* check for only source */
        if(gSourceType == 1)
        {
            snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/VideoCodecInfo/num_entries" , i+1);
            gDSVideoDeviceConfiguration[i].num_codec_entries = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string );
            UT_LOG_DEBUG("num_codec_entries %d",gDSVideoDeviceConfiguration[i].num_codec_entries);

            /* kvp profile support for float type */
            //snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/VideoCodecInfo/VideoCodec0/level" , i+1);
            //status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), key_string, gDSVideoDeviceConfiguration[i].level, sizeof(key_string));
            snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/VideoCodecInfo/VideoCodec0/level" , i+1);
            gDSVideoDeviceConfiguration[i].level = ut_kvp_getFloatField(ut_kvp_profile_getInstance(), key_string );
            UT_LOG_DEBUG("gDSVideoDeviceConfiguration[i].level:%f",gDSVideoDeviceConfiguration[i].level);
            snprintf(key_string, DS_VIDEO_DEVICE_KVP_SIZE, "dsVideoDevice/Device/%d/VideoCodecInfo/VideoCodec0/profile" , i+1);
            gDSVideoDeviceConfiguration[i].profile = (dsVideoCodecHevcProfiles_t)ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string );
            UT_LOG_DEBUG("gDSVideoDeviceConfiguration[i].profile :%d",gDSVideoDeviceConfiguration[i].profile);
        }
    }

    return 0;
}

/* Free Parse Video Device Configuration */
void test_dsVideoDevice_parse_configuration_term()
{
    if(gDSVideoDeviceConfiguration)
    {
        free(gDSVideoDeviceConfiguration);
    }
}


/** @} */ // End of DS_VideoDevice_HALTEST_PARSE_CONFIG
/** @} */ // End of DS_VideoDevice_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
