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
 * @defgroup DS_VideoPort_HALTEST_PARSE_CONFIG_HEADER Device Settings Video Port Parse Config Header File
 * @{
 * @parblock
 *
 * ### Configuration Header file for DS Video Port test suit :
 *
 *  DS Video Port Parse Configuration Header file
 *
 * @endparblock
 */


/**
 * @file test_dsVideoPort_parse_configuration.h
 *
 */

#ifndef __TEST_DS_VIDEO_PARSE_CONFIG_H__
#define __TEST_DS_VIDEO_PARSE_CONFIG_H__

#include "dsVideoPort.h"

#define DS_VIDEO_PORT_KVP_SIZE                128
#define DS_VIDEO_PORT_TYPE_SIZE               8
#define DS_VIDEO_PORT_NAME_SIZE               64
#define DS_VIDEO_PORT_MODULE_NAME_SIZE        32
#define DS_VIDEO_PORT_RESOLUTION_MAX          32
#define DS_VIDEO_PORT_RESOLUTION_NAME_MAX     32
#define DS_VIDEO_PORT_RESOLUTION_NUM_MAX      32

#define DS_VIDEO_PORT_MODULE_NAME             "dsVideoPort"

/* Video Port configuration */
typedef struct 
{
    dsVideoPortType_t typeid;
    uint16_t index;
    char     port_name[DS_VIDEO_PORT_NAME_SIZE];
    bool     dtcp_supported;
    bool     hdcp_supported;
    int32_t  numSupportedResolutions;
    dsVideoPortResolution_t  supportedResolutions[DS_VIDEO_PORT_RESOLUTION_NUM_MAX];
    char     defaultResolution[DS_VIDEO_PORT_RESOLUTION_NAME_MAX];
    dsDisplayColorSpace_t  colorspaces;
    bool     DisplaySurround;
    dsSURROUNDMode_t  SurroundMode;
    int32_t  Supported_tv_resolutions_capabilities;
    int32_t  hdr_capabilities;
    dsHdcpProtocolVersion_t  hdcp_protocol_version;
    dsDisplayQuantizationRange_t  quantization_ranges;
    dsDisplayMatrixCoefficients_t  matrix_coefficients;
}dsVideoPortConfiguration_t;

/* Global variables */
extern dsVideoPortConfiguration_t* gDSVideoPortConfiguration;
extern int32_t gDSvideoPort_NumberOfPorts;
extern int32_t gDSvideoPort_color_depth;
extern int32_t gDSvideoPort_connectedAOP;

/*Function prototypes */
int test_dsVideoPort_parse_configuration();
void test_dsVideoPort_parse_term();

#endif //__TEST_DS_VIDEO_PARSE_CONFIG_H__

/** @} */ // End of DS_VideoPort_HALTEST_PARSE_CONFIG_HEADER
/** @} */ // End of DS_VideoPort_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
