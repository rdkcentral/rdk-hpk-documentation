/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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
 * @file dsError.h
 *
 * @brief Device Settings HAL error codes.
 *
 * This API defines common error codes used by the Device Settings HAL.
 *
 * @par Document
 * Document reference.
 *
 * @par Open Issues (in no particular order)
 * -# None
 *
 * @par Assumptions
 * -# None
 *
 * @par Abbreviations
 * - cb:      Callback function (suffix).
 * - DS:      Device Settings.
 * - HAL:     Hardware Abstraction Layer.
 *
 * @par Implementation Notes
 * -# None
 *
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

/** @addtogroup Device_Settings Device Settings Module
 * @{
 */

/** @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 *  @{
 */



/** @defgroup DSHAL_ERROR_H Device Settings Status Codes
*
*  Defined hereunder are the error codes that are used in the Device Settings library.
*
*  @{
*/

#ifndef __DS_ERROR_H__
#define __DS_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif


/** 
 * @brief Device Settings API Error return codes.
 * 
 * A non-zero value indicates an error.
 * 
 */
typedef enum {
    dsERR_NONE = 0,                 ///< Input output operation is successful
    dsERR_GENERAL = 1,              ///< Operation general error.
    dsERR_INVALID_PARAM,            ///< Invalid parameter is passed to the module
    dsERR_INVALID_STATE,            ///< Module is in an invalid state
    dsERR_ALREADY_INITIALIZED,      ///< Module is already initialised
    dsERR_NOT_INITIALIZED,          ///< Module is not initialised
    dsERR_OPERATION_NOT_SUPPORTED,  ///< Operation not supported
    dsERR_RESOURCE_NOT_AVAILABLE,   ///< Resources have failed to allocate
    dsERR_OPERATION_FAILED,         ///< The attempted operation has failed. Used when a SoC call has a failure
    dsErr_MAX
} dsError_t;



#ifdef __cplusplus
}
#endif
#endif /* __DS_ERROR_H__ */

/** @} */ // End of DSHAL_ERROR_H
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
