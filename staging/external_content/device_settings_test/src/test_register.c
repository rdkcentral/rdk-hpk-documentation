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
 * @defgroup Device_Settings_REGISTER Device Settings HAL Tests Register File
 * @{
 * @parblock
 *
 * ### Registration of tests for Device Settings HAL :
 *
 * This is to ensure that the APIs meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * Refer to Device Settings HAL Documentation Guide : [README.md](../../docs/pages/README.md)
 *
 * @endparblock
 *
 */

/**
 * @file test_register.c
 *
 */

#include <stdint.h>
#include <ut.h>
#include "test_parse_configuration.h"

/**
 * @brief Register test functionality
 *
 */

/* L1 Testing Functions */
extern int test_l1_dsHost_register(void);
extern int test_l1_dsAudio_register(void);
extern int test_l1_dsVideoDevice_register(void);
extern int test_l1_dsHdmiIn_register(void);
extern int test_l1_dsFPD_register(void);
extern int test_l1_dsVideoPort_register(void);
extern int test_l1_dsCompositeIn_register(void);
extern int test_l1_dsDisplay_register(void);

/* L2 Testing Functions */
extern int test_l2_dsHost_register( void );
extern int test_l2_dsAudio_register( void );
extern int test_l2_dsVideoDevice_register( void );
extern int test_l2_dsHdmiIn_register( void );
extern int test_l2_dsFPD_register( void );
extern int test_l2_dsVideoPort_register( void );
extern int test_l2_dsCompositeIn_register( void );
extern int test_l2_dsDisplay_register( void );

/* L3 Testing Functions */
extern int test_l3_dsFPD_register( void);
extern int test_l3_dsAudio_register( void );
extern int test_l3_dsVideoPort_register( void );
extern int test_l3_dsCompositeIn_register( void );
extern int test_l3_dsHost_register( void );
extern int test_l3_dsDisplay_register( void );
extern int test_l3_dsVideoDevice_register( void );
extern int test_l3_dsHdmiIn_register( void );


static void dsAudio_register()
{
    if (gDSModule & dsAudioPort)
    {
        test_l1_dsAudio_register();
        test_l2_dsAudio_register();
    }
    test_l3_dsAudio_register();
}

static void dsVideoPort_register()
{
    if (gDSModule & dsVideoPort)
    {
        test_l1_dsVideoPort_register();
        test_l2_dsVideoPort_register();
    }
    test_l3_dsVideoPort_register();
}

static void dsHost_register()
{
    if (gDSModule & dsHost)
    {
        test_l1_dsHost_register();
        test_l2_dsHost_register();
    }
    test_l3_dsHost_register();
}

static void dsVideoDevice_register()
{
    if (gDSModule & dsVideoDevice)
    {
        test_l1_dsVideoDevice_register();
        test_l2_dsVideoDevice_register();
    }
    test_l3_dsVideoDevice_register();
}

static void dsHdmiIn_register()
{
    if (gDSModule & dsHdmiIn)
    {
        test_l1_dsHdmiIn_register();
        test_l2_dsHdmiIn_register();
    }
    test_l3_dsHdmiIn_register();
}

static void dsFPD_register()
{
    if (gDSModule & dsFPD)
    {
        test_l1_dsFPD_register();
        test_l2_dsFPD_register();
    }
    test_l3_dsFPD_register();
}

static void dsCompositeIn_register()
{
    if (gDSModule & dsComposite)
    {
        test_l1_dsCompositeIn_register();
        test_l2_dsCompositeIn_register();
    }
    test_l3_dsCompositeIn_register();
}

static void dsDisplay_register()
{
    if (gDSModule & dsDisplay)
    {
        test_l1_dsDisplay_register();
        test_l2_dsDisplay_register();
    }
    test_l3_dsDisplay_register();
}

int UT_register_tests(void)
{
    int registerFailed = 0;

    dsHost_register();
    dsAudio_register();
    dsVideoDevice_register();
    dsHdmiIn_register();
    dsFPD_register();
    dsVideoPort_register();
    dsCompositeIn_register();
    dsDisplay_register();

    return registerFailed;
}

/** @} */ // End of Device_Settings_REGISTER
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
