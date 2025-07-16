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
 * @addtogroup Deepsleep_Manager Deep Sleep Manager
 * @{
 * @par Application API Specification
 */

/**
 * @defgroup Deepsleep_Manager_HALTEST Deep Sleep Manager HALTEST
 * @{
 *
 */

/**
 * @defgroup Deepsleep_Mgr_HALTEST_MAIN Deep Sleep Manager HALTEST Main File
 * @{
 * @parblock
 * ### Tests for Deepsleep Manager HAL :
 *
 * This is to ensure that the APIs meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [deepsleep-manager_halSpec.md](../../docs/pages/deepsleep-manager_halSpec.md)
 * @endparblock
 */

/**
* @file main.c
*
*/

#include <ut.h>

extern int UT_register_tests( void );

int main(int argc, char** argv) 
{
	int registerReturn = 0;

	/* Register tests as required, then call the UT-main to support switches and triggering */
	UT_init( argc, argv );

	/* Check if tests are registered successfully */

	registerReturn = UT_register_tests();
	if ( registerReturn == -1 )
	{
		printf("\n UT_register_tests() returned failure");
		return -1;
	}

	/* Begin test executions */
	UT_run_tests();

	return 0;

}

/** @} */ // End of Deepsleep_Mgr_HALTEST_MAIN
/** @} */ // End of Deepsleep_Manager_HALTEST
/** @} */ // End Deepsleep_Manager
/** @} */ // End of HPK
