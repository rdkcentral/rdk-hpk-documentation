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
 * @defgroup DS_Audio_HALTEST_PARSE_CONFIG_HEADER Device Settings Audio Parse Config Header File
 * @{
 * @parblock
 *
 * ### Configuration Header file for DS Audio test suit :
 *
 *  DS Audio Parse Configuration Header file
 *
 * @endparblock
 */

/**
* @file test_dsAudio_parse_configuration.h
*
*/
#ifndef __TEST_DSAUDIO_PARSE_CONFIG_H__
#define __TEST_DSAUDIO_PARSE_CONFIG_H__

#include <dsAudio.h>

#define DS_AUDIO_KVP_SIZE              128
#define DS_AUDIO_PORT_NAME_SIZE        32
#define DS_AUDIO_MAX_MS12_PROFILE_LIST 10
#define DS_AUDIO_MAX_MS12_PROFILE_LEN  32
#define DS_AUDIO_DEVICE_NAME_SIZE      64

#define DS_AUDIO_MOULE_NAME             "dsAudio"

typedef struct
{
    uint16_t typeid;
    uint16_t index;
    char     port_name[DS_AUDIO_PORT_NAME_SIZE];
    int32_t  no_of_supported_compression;
    int32_t  supported_compressions[dsAUDIO_CMP_MAX];
    int32_t  no_of_supported_stereo_mode;
    int32_t  supported_stereo_mode[dsAUDIO_STEREO_MAX];
    bool     stereo_auto_mode;
    int32_t  ms12_capabilites;
    char     ms12_audio_profiles[DS_AUDIO_MAX_MS12_PROFILE_LIST][DS_AUDIO_MAX_MS12_PROFILE_LEN];
    int32_t  ms12_audioprofilecount;
    bool     isms12decode;
    bool     isms11decode;
    int32_t  atmos_capabilites;
    int32_t  arc_type;
    int32_t  min_dialog_enhancement_level;
    int32_t  max_dialog_enhancement_level;
}dsAudioPortConfiguration_t;

/* Global variables */
extern dsAudioPortConfiguration_t* gDSAudioPortConfiguration;
extern char gDSAudioDeviceName[];
extern int32_t gDSAudioNumberOfPorts;
extern int32_t gAudioCapabilities;

/*Function prototypes */
int test_dsAudio_parse_configuration();
void test_dsAudio_parse_configuration_term();

#endif //__TEST_DSAUDIO_PARSE_CONFIG_H__

/** @} */ // End of DS_Audio_HALTEST_PARSE_CONFIG_HEADER
/** @} */ // End of DS_Audio_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
