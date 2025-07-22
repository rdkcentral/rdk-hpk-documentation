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
 * @defgroup POWER_MANAGER Power Manager Module
 * @{
 */

/**
 * @defgroup POWER_MANAGER_HAL Power Manager HAL
 * @{
 * Described herein are the PWRMGR_HAL HAL types and functions that are part of the Power
 * Manager HAL. All listeners should releases resources when
 * entering POWER OFF/STANDBY state and re-acquire them when entering POWER ON state.
 */

/**
 * @defgroup PLAT_POWER_H Plat Power
 * @{
 */

/**
 * @file plat_power.h
 *
 * @par Abbreviations
 * - HAL:     Hardware Abstraction Layer
 * - CPE:     Customer Premises Equipment
 * - CPU:     Central Processing Unit
 * 
 *
 * @par Implementation Notes
 * -# None
 *
 */

#ifndef __POWERMGR_PLAT_H__
#define __POWERMGR_PLAT_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdbool.h>
/**
 * @brief Power manager API status codes
 * 
 * A non-zero value indicates an error
*/
typedef enum
{
    PWRMGR_SUCCESS = 0,             /*!< Success */
    PWRMGR_INVALID_ARGUMENT,        /*!< Invalid Argument */
    PWRMGR_ALREADY_INITIALIZED,     /*!< Module already initialized */
    PWRMGR_NOT_INITIALIZED,         /*!< Module is not  initialized */
    PWRMGR_INIT_FAILURE,            /*!< Module has failed to initialize */
    PWRMGR_SET_FAILURE,             /*!< Module failed to set the power state */
    PWRMGR_GET_FAILURE,             /*!< Module failed to get the power state */
    PWRMGR_OPERATION_NOT_SUPPORTED, /*!< Operation not supported */
    PWRMGR_TERM_FAILURE,            /*!< Module failed to terminate */
    PWRMGR_MAX                      /*!< Out of range - required to be the last item of the enum */
} pmStatus_t;

/**
 * @brief All possible power states
*/
typedef enum _Daemon_PowerState_t
{
    PWRMGR_POWERSTATE_OFF,                  /*!< Power state OFF */
    PWRMGR_POWERSTATE_STANDBY,              /*!< Power state STANDBY */
    PWRMGR_POWERSTATE_ON,                   /*!< Power state ON */
    PWRMGR_POWERSTATE_STANDBY_LIGHT_SLEEP,  /*!< Power state Standby Light Sleep */
    PWRMGR_POWERSTATE_STANDBY_DEEP_SLEEP,   /*!< Power state DeepSleep power saving mode */
    PWRMGR_POWERSTATE_MAX                   /*!< Out of range - required to be the last item of the enum */
} PWRMgr_PowerState_t;

/**
 *  @brief Enumerator which represents the  possible wakeup sources
 */
typedef enum _PWRMGR_WakeupSrcType_t
{
    PWRMGR_WAKEUPSRC_VOICE = 0,            /*!< Voice wake up */
    PWRMGR_WAKEUPSRC_PRESENCE_DETECTION,   /*!< Presence Detection wake up */
    PWRMGR_WAKEUPSRC_BLUETOOTH,            /*!< Bluetooth wake up */
    PWRMGR_WAKEUPSRC_WIFI,                 /*!< Wifi wake up */
    PWRMGR_WAKEUPSRC_IR,                   /*!< IR wake up */
    PWRMGR_WAKEUPSRC_POWER_KEY,            /*!< Power key wake up */
    PWRMGR_WAKEUPSRC_TIMER,                /*!< Timer wake up */
    PWRMGR_WAKEUPSRC_CEC,                  /*!< CEC wake up */
    PWRMGR_WAKEUPSRC_LAN,                  /*!< LAN wake up */
    PWRMGR_WAKEUPSRC_MAX                   /*!< Out of range - required to be the last item of the enum */
} PWRMGR_WakeupSrcType_t;

/**
 * @brief Initializes the underlying Power Management module
 *
 * This function must initialize all aspects of the CPE's Power Management module.
 *
 * @return    pmStatus_t                    - Status
 * @retval    PWRMGR_SUCCESS                - Success
 * @retval    PWRMGR_ALREADY_INITIALIZED    - Function is already initialized
 * @retval    PWRMGR_INIT_FAILURE           - Function has failed to properly initialize
 * 
 * @post PLAT_TERM() must be called to release resources.
 */
pmStatus_t PLAT_INIT( void );

/**
 * @brief Sets the CPE Power State
 *
 * This function is just required to hold the value of the current power state status.
 *
 * @param[in]  newState - The new power state value
 *
 * @return    pmStatus_t                - Status
 * @retval    PWRMGR_SUCCESS            - Success
 * @retval    PWRMGR_NOT_INITIALIZED    - Module is not initialised
 * @retval    PWRMGR_INVALID_ARGUMENT   - Parameter passed to this function is invalid
 * @retval    PWRMGR_SET_FAILURE        - Failed to update
 * 
 * @pre PLAT_INIT() must be called before calling this API
 * 
 * @warning This API is Not thread safe
 * 
 * @see PLAT_API_GetPowerState(), PWRMgr_PowerState_t
 * 
 */
pmStatus_t PLAT_API_SetPowerState( PWRMgr_PowerState_t newState );

/**
 * @brief Gets the CPE Power State
 *
 * @param[out] curState  - The current power state of the CPE
 *
 * @return    pmStatus_t                - Status
 * @retval    PWRMGR_SUCCESS            - Success
 * @retval    PWRMGR_NOT_INITIALIZED    - Module is not initialised
 * @retval    PWRMGR_INVALID_ARGUMENT   - Parameter passed to this function is invalid
 * @retval    PWRMGR_GET_FAILURE        - Failed to get
 * 
 * @pre PLAT_INIT() must be called before calling this API
 * 
 * @warning This API is Not thread safe
 * 
 * @see PLAT_API_SetPowerState(), PWRMgr_PowerState_t
 * 
 */
pmStatus_t PLAT_API_GetPowerState( PWRMgr_PowerState_t *curState );

/**
 * @brief Enables or disables the Wakeup source type
 *
 * @param [in] srcType  - Wake up source type
 * @param [in] enable   - Enable or disable Wake up source
 *                        True for enabled, false for disabled
 * 
 * @return    pmStatus_t                        - Status
 * @retval    PWRMGR_SUCCESS                    - Success
 * @retval    PWRMGR_NOT_INITIALIZED            - Module is not initialised
 * @retval    PWRMGR_OPERATION_NOT_SUPPORTED    - Wake up source type not supported
 * @retval    PWRMGR_INVALID_ARGUMENT           - Parameter passed to this function is invalid
 * @retval    PWRMGR_SET_FAILURE                - Failed to  power state
 * 
 * @pre PLAT_INIT() must be called before calling this API
 * 
 * @warning This API is Not thread safe
 * 
 * @see PLAT_API_GetWakeupSrc(), PWRMGR_WakeupSrcType_t
 */
pmStatus_t PLAT_API_SetWakeupSrc( PWRMGR_WakeupSrcType_t srcType, bool  enable );

/**
 * @brief Checks if the wake up source is enabled or disabled for the device
 *
 * @param [in] srcType  - Wake up source type
 * @param [out] enable  - Variable to store if wake up source type is enabled or disabled
 *                        True for enabled, false for disabled
 * 
 * @return    pmStatus_t                        - Status
 * @retval    PWRMGR_SUCCESS                    - Success
 * @retval    PWRMGR_NOT_INITIALIZED            - Module is not initialised
 * @retval    PWRMGR_OPERATION_NOT_SUPPORTED    - Wake up source type not supported
 * @retval    PWRMGR_INVALID_ARGUMENT           - Parameter passed to this function is invalid
 * @retval    PWRMGR_GET_FAILURE                - Failed to get
 * 
 * @pre PLAT_INIT() must be called before calling this API
 * 
 * @warning This API is Not thread safe
 * 
 * @see PWRMGR_WakeupSrcType_t, PLAT_API_SetWakeupSrc()
 */
pmStatus_t PLAT_API_GetWakeupSrc( PWRMGR_WakeupSrcType_t srcType, bool  *enable );

/**
 * @brief Resets the power state of the device
 * 
 * @note PLAT_Reset() is deprecated.
 *
 * @param[in] newState  - The state to be set
 * 
 * @return    pmStatus_t                - Status
 * @retval    PWRMGR_NOT_INITIALIZED    - Module is not initialised
 * @retval    PWRMGR_INVALID_ARGUMENT   - Parameter passed to this function is invalid
 * @retval    PWRMGR_SET_FAILURE        - Failed to update
 * 
 * @pre PLAT_INIT() must be called before calling this API
 * 
 * @warning This API is Not thread safe
 * 
 * @note Given that PLAT_Reset() reboots the device, it does not return on a success
 *
 * @see PWRMgr_PowerState_t
 * 
 */
pmStatus_t PLAT_Reset( PWRMgr_PowerState_t newState );

/**
 * @brief Terminates the CPE Power Management module
 *
 * All data structures used within Power Management module must be reset and release any
 * Power Management specific handles and resources.
 * 
 * @return    pmStatus_t              - Status
 * @retval    PWRMGR_SUCCESS          - Success
 * @retval    PWRMGR_NOT_INITIALIZED  - Function is already initialized
 * @retval    PWRMGR_TERM_FAILURE     - Module failed to terminate
 * 
 * @pre PLAT_INIT() must be called before calling this API
 * 
 */
pmStatus_t PLAT_TERM( void );


#ifdef __cplusplus
}
#endif
#endif // End of __POWERMGR_PLAT_H__

/** @} */ // End of PLAT_POWER_H
/** @} */ // End of POWER_MANAGER_HAL
/** @} */ // End of POWER_MANAGER
/** @} */ // End of HPK
