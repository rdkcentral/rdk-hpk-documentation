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
 * @defgroup Device_Settings_HALTEST Device Settings HAL Tests
 * @{
 */

/**
 * @defgroup Device_Settings_MAIN Device Settings HAL Tests Main File
 *  @{
 * @parblock
 *
 * ### Tests for Device Settings HAL :
 *
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 *
 * Refer to Device Settings HAL Documentation Guide : [README.md](../../docs/pages/README.md)
 *
 * @endparblock
 */

/**
 * @file main.c
 *
 */

#include <ut.h>
#include "test_parse_configuration.h"

extern int UT_register_tests(void);

int main(int argc, char** argv)
{
    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init(argc, argv);

    if (test_parse_configuration() == -1)
    {
        UT_FAIL(" Failed to parse the configuration file");
        test_parse_configuration_term();
        return -1;
    }

    UT_register_tests();

    /* Begin test executions */
    UT_run_tests();

    test_parse_configuration_term();

    return 0;
}
/** @} */ // End of Device_Settings_MAIN
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
