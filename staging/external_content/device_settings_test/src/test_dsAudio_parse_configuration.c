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
 * @defgroup DS_Audio_HALTEST Device Settings Audio HAL Tests
 * @{
 */

/**
 * @defgroup DS_Audio_HALTEST_PARSE_CONFIG Device Settings Audio Parse Config File
 * @{
 * @parblock
 *
 * ### Configuration file for DS Audio test suit :
 *
 *  DS Audio Parse Configuration file
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 *
 * @endparblock
 */


/**
* @file test_dsAudio_parse_configuration.c
*
*/
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <stdlib.h>

#include <dsAudio.h>
#include "test_parse_configuration.h"

/* Global variables */
dsAudioPortConfiguration_t* gDSAudioPortConfiguration   = NULL;
char gDSAudioDeviceName[DS_AUDIO_DEVICE_NAME_SIZE]      = {0};
int32_t gDSAudioNumberOfPorts = 0;
int32_t gAudioCapabilities = 0x0;

/* Parse Audio Configuration file */
int test_dsAudio_parse_configuration()
{
    char key_string[DS_AUDIO_KVP_SIZE];
    ut_kvp_status_t status;

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsAudio.Type", gDeviceType, TEST_DS_DEVICE_TYPE_SIZE);

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
    else {
        UT_LOG_ERROR("Failed to get the platform type");
        return -1;
    }

    status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), "dsAudio/Name", gDSAudioDeviceName, DS_AUDIO_DEVICE_NAME_SIZE);

    UT_LOG_DEBUG("Device Type: %s, Device Name: %s", gDeviceType, gDSAudioDeviceName);

    gAudioCapabilities    = UT_KVP_PROFILE_GET_UINT32("dsAudio/Audio_Capabilities");
    gDSAudioNumberOfPorts = UT_KVP_PROFILE_GET_UINT32("dsAudio/Number_of_supported_ports");
    UT_LOG_DEBUG("gDSAudioNumberOfPorts =%d ",gDSAudioNumberOfPorts);

    gDSAudioPortConfiguration = (dsAudioPortConfiguration_t*) calloc(gDSAudioNumberOfPorts, sizeof(dsAudioPortConfiguration_t));
    if(gDSAudioPortConfiguration == NULL)
    {
        UT_LOG_ERROR("Failed to allocate memory for audio configuration structure");
        return -1;
    }

    for(int i = 0; i < gDSAudioNumberOfPorts; i++)
    {

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/Typeid" , i+1);
        gDSAudioPortConfiguration[i].typeid = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string);

        if(gDSAudioPortConfiguration[i].typeid == dsAUDIOPORT_TYPE_HDMI_ARC)
        {
            snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/Arc_Types" , i+1);
            gDSAudioPortConfiguration[i].arc_type = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string);
        }

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/Index" , i+1);
        gDSAudioPortConfiguration[i].index = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/Name" , i+1);
        status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), key_string, gDSAudioPortConfiguration[i].port_name, DS_AUDIO_PORT_NAME_SIZE);

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/number_of_supported_compressions" , i+1);
        gDSAudioPortConfiguration[i].no_of_supported_compression = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/number_of_supported_stereo_modes" , i+1);
        gDSAudioPortConfiguration[i].no_of_supported_stereo_mode = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/stereo_auto_mode" , i+1);
        gDSAudioPortConfiguration[i].stereo_auto_mode = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/MS12_Capabilities" , i+1);
        gDSAudioPortConfiguration[i].ms12_capabilites = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string );

        memset(key_string, 0, DS_AUDIO_KVP_SIZE);
        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/MS12_AudioProfileCount" , i+1);
        gDSAudioPortConfiguration[i].ms12_audioprofilecount = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/IsMS12Decode" , i+1);
        gDSAudioPortConfiguration[i].isms12decode = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/IsMS11Decode" , i+1);
        gDSAudioPortConfiguration[i].isms11decode = ut_kvp_getBoolField( ut_kvp_profile_getInstance(), key_string );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/ATMOS_Capabilities" , i+1);
        gDSAudioPortConfiguration[i].atmos_capabilites = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string  );

        snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/dialog_enhancement_level" , i+1);
        if(ut_kvp_fieldPresent(ut_kvp_profile_getInstance(),key_string))
        {
            snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/dialog_enhancement_level/min" , i+1);
            gDSAudioPortConfiguration[i].min_dialog_enhancement_level = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string );

            snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/dialog_enhancement_level/max" , i+1);
            gDSAudioPortConfiguration[i].max_dialog_enhancement_level = ut_kvp_getUInt16Field( ut_kvp_profile_getInstance(), key_string );
        }

         // loop to get supported compressions in array
        for(int j = 0; j < gDSAudioPortConfiguration[i].no_of_supported_compression; j++)
        {
            snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/compressions/%d" , i+1 , j);
            gDSAudioPortConfiguration[i].supported_compressions[j] = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string);
        }

        //loop to get supported stereo modes in array
        for(int j = 0; j < gDSAudioPortConfiguration[i].no_of_supported_stereo_mode; j++)
        {
            snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/stereo_modes/%d" , i+1 , j);
            gDSAudioPortConfiguration[i].supported_stereo_mode[j] = ut_kvp_getUInt32Field(ut_kvp_profile_getInstance(), key_string);
        }

        // loop to get ms12 audio profiles
        for(int j = 0; j < gDSAudioPortConfiguration[i].ms12_audioprofilecount; j++)
        {
            snprintf(key_string, DS_AUDIO_KVP_SIZE, "dsAudio/Ports/%d/MS12_AudioProfiles/%d" , i+1 , j);
            status = ut_kvp_getStringField(ut_kvp_profile_getInstance(), key_string, gDSAudioPortConfiguration[i].ms12_audio_profiles[j], DS_AUDIO_MAX_MS12_PROFILE_LEN);
        }
    }

    return 0;
}

void test_dsAudio_parse_configuration_term()
{
    if(gDSAudioPortConfiguration)
    {
        free(gDSAudioPortConfiguration);
    }
}

/** @} */ // End of DS_Audio_HALTEST_PARSE_CONFIG
/** @} */ // End of DS_Audio_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK

