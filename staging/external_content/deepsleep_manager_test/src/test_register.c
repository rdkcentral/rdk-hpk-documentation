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

#include <ut.h>

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
 * @addtogroup Deepsleep_Manager_HALTEST Deep Sleep Manager HALTEST
 * @{
 */

/**
 * @defgroup Deepsleep_Mgr_HALTEST_REGISTER Deep Sleep Manager HALTEST Register File
 * @{
 * ### Registration of tests for Deepsleep Manager HAL :
 *
 * Registration of tests for Deepsleep Manager HAL.
 * This is to ensure that the APIs meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Ref to API Definition specification documentation : [deepsleep-manager_halSpec.md](../../docs/pages/deepsleep-manager_halSpec.md)
 *
 */


/**
* @file test_register.c
*
*/



/**
 * @brief Register test functionality
 * 
 */

/* L1 Testing Functions */
extern int test_l1_deepSleepMgr_register( void );


/* L2 Testing Functions */
extern int test_l2_deepSleepMgr_register( void );

/* L3 Testing Functions */
extern int test_l3_deepSleepMgr_register( void );

int UT_register_tests( void )
{
	int registerFailed=0;

	registerFailed |= test_l1_deepSleepMgr_register();
    registerFailed |= test_l2_deepSleepMgr_register();
    registerFailed |= test_l3_deepSleepMgr_register();

	return registerFailed;
}

/** @} */ // End of Deepsleep_Mgr_HALTEST_REGISTER
/** @} */ // End of Deepsleep_Manager_HALTEST
/** @} */ // End Deepsleep_Manager
/** @} */ // End of HPK
