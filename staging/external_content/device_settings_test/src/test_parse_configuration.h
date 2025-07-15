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
 * @defgroup Device_Settings_PARSE_CONFIG_HEADER Device Settings HAL Tests configuration parser Header File
 * @{
 * @parblock
 *
 * ### Configuration parser functions for Device Settings HAL :
 *
 * Configuration parser functions required for the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
* @file test_parse_configuration.h
*
*/
#ifndef __TEST_PARSE_CONFIG_H__
#define __TEST_PARSE_CONFIG_H__

#include "test_dsAudio_parse_configuration.h"
#include "test_dsVideoDevice_parse_configuration.h"
#include "test_dsVideoPort_parse_configuration.h"

#define TEST_DS_DEVICE_TYPE_SIZE     8
#define TEST_DS_MODULE_NAME_SIZE  32

#define TEST_TYPE_SOURCE_VALUE     "source"
#define TEST_TYPE_SINK_VALUE       "sink"

/* Global variables */
extern char gDeviceType[TEST_DS_DEVICE_TYPE_SIZE];
extern int32_t gSourceType;
extern int32_t gDSModule;

/* CODING STANDARDS: Use Typedef or enum/struct not BOTH

// Correctly defined typedef
typedef enum
{
...
}dsModule_t; // This a typedef

usage `dsModule_t variable;`
...
enum _dsModule_t
{
...
};

usage `enum _dsModule_t variable;`

usage
*/

typedef enum 
{
    dsNone        = (0x0 << 0),
    dsAudioPort   = (0x1 << 0),
    dsVideoPort   = (0x1 << 1),
    dsComposite   = (0x1 << 2),
    dsHdmiIn      = (0x1 << 3),
    dsVideoDevice = (0x1 << 4),
    dsDisplay     = (0x1 << 5),
    dsFPD         = (0x1 << 6),
    dsHost        = (0x1 << 7),
}dsModule_t;

/*Function prototypes */
int test_parse_configuration();
void test_parse_configuration_term();

#endif //__TEST_PARSE_CONFIG_H__

/** @} */ // End of Device_Settings_PARSE_CONFIG_HEADER
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
