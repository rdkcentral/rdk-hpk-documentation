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
 * @defgroup HDMI_CEC HDMI CEC Module
 * @{
 *
 */

/**
 * @defgroup HDMI_CEC_HAL HDMI CEC HAL
 * @{
 * @par Application API Specification
 * HDMI CEC HAL provides an API to send and receive CEC messages with other
 * CEC devices. The HDMI CEC HAL also provides API functions to retrieve the
 * logical address and physical address of the host device.
 * CEC messages are transmitted on the bus with a synchronous function call
 * that returns the acknowledgment of the message when a receiver on the bus
 * ACKs it.
 *
 */

/**
 * @defgroup HDMI_CEC_DRIVER_H HDMI CEC Header
 * @{
 */

/**
 * @file hdmi_cec_driver.h
 */

#ifndef __HDMI_CEC_DRIVER_H__
#define __HDMI_CEC_DRIVER_H__
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error Codes
 * 
 * 
 */
typedef enum HDMI_CEC_IO_ERROR
{
    HDMI_CEC_IO_SUCCESS = 0,                ///< Input output operation is successful
    HDMI_CEC_IO_SENT_AND_ACKD = 1,          ///< Send and acknowledgement received
    HDMI_CEC_IO_SENT_BUT_NOT_ACKD,          ///< Sent but acknowledgement not received
    HDMI_CEC_IO_SENT_FAILED,                ///< Operation failed
    HDMI_CEC_IO_NOT_OPENED,                 ///< Module is not initialised
    HDMI_CEC_IO_INVALID_ARGUMENT,           ///< Invalid argument is passed to the module
    HDMI_CEC_IO_LOGICALADDRESS_UNAVAILABLE, ///< Logical address is not available
    HDMI_CEC_IO_GENERAL_ERROR,              ///< Operation general error.
    HDMI_CEC_IO_ALREADY_OPEN,               ///< Module is already initialised
    HDMI_CEC_IO_ALREADY_REMOVED,            ///< Removal operation is already executed
    HDMI_CEC_IO_INVALID_OUTPUT,             ///< Output arguments fall outside the valid range
    HDMI_CEC_IO_INVALID_HANDLE,             ///< An invalid handle argument has been passed
    HDMI_CEC_IO_OPERATION_NOT_SUPPORTED,    ///< Operation not supported
    HDMI_CEC_IO_NOT_ADDED,                  ///< Not added
    HDMI_CEC_IO_MAX                         ///< Out of range - required to be the 
                                            ///< last item of the enum
} HDMI_CEC_STATUS;

/**
 * @brief Callback function triggered when a complete CEC message is received
 *
 * Upon each callback, only 1 complete message should be contained in the buffer.
 * The message data must be copied in the call because the buf pointer is no longer valid when this callback returns.
 *
 * @param[in] handle       - The handle used by application to uniquely identify the HAL instance. Non zero value
 * @param[in] callbackData - Callback data for the receive callback
 * @param[in] buf          - Buffer passed in receive callback
 * @param[in] len          - Length of buffer passed in receive callback
 */
typedef void (*HdmiCecRxCallback_t)(int handle, void *callbackData, unsigned char *buf, int len);

/**
 * @note This API is deprecated.
 *
 * @brief Callback function triggered to report the status of the latest transmit message
 *
 * @param[in] handle       - The handle used by application to uniquely identify the HAL instance. Non zero value
 * @param[in] callbackData - Callback data for the transmit callback
 * @param[in] result       - Async transmit result from the platform implementation
 */
typedef void (*HdmiCecTxCallback_t)(int handle, void *callbackData, int result);

/**
 * @brief Initializes the HDMI CEC HAL
 *
 * This function is required to be called before the other APIs in this module.@n
 * Subsequent calls to this API will return HDMI_CEC_IO_SUCCESS.
 * For HDMI source devices, logical address discovery takes place during HdmiCecOpen() and
 * can be obtained via HdmiCecGetLogicalAddress().
 * For HDMI sink devices, logical address discovery does not occur during HdmiCecOpen() and
 * must be managed by the caller.
 *
 * @param [out] handle                    - The handle used by application to uniquely 
 *                                          identify the HAL instance
 *
 * @return HDMI_CEC_STATUS                        - Status
 * @retval HDMI_CEC_IO_SUCCESS                    - Success
 * @retval HDMI_CEC_IO_ALREADY_OPEN               - Function is already open. 
 *                                                  This error code will deprecated in the next phase.
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT           - Parameter passed to this function is invalid
 * @retval HDMI_CEC_IO_LOGICALADDRESS_UNAVAILABLE - Logical address is not available for source devices. 
 * 
 * 
 * @post HdmiCecClose() must be called to release resources.
 * @warning This API is NOT thread safe.
 *
 * @see HdmiCecClose()
 *
 */
HDMI_CEC_STATUS HdmiCecOpen(int *handle);

/**
 * @brief Closes an instance of HDMI CEC HAL
 *
 * This function will uninitialise the module.@n
 * Close will clear up registered logical addresses.@n
 * Subsequent calls to this API will return HDMI_CEC_IO_SUCCESS.
 *
 * @param[in] handle - The handle returned from the HdmiCecOpen(). Non zero value
 *
 * @return HDMI_CEC_STATUS              - Status
 * @retval HDMI_CEC_IO_SUCCESS          - Success
 * @retval HDMI_CEC_IO_NOT_OPENED       - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_HANDLE   - An invalid handle argument has been passed
 *
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 *
 * @see HdmiCecOpen()
 *
 */
HDMI_CEC_STATUS HdmiCecClose(int handle);

/**
 * @brief Sets the logical address assignment for a HDMI sink device.
 *
 * Caller will take care of discovery of Logical Address and sets the available logical addresses through this API.@n
 * This API is only applicable for sink devices.@n
 * Invoking this API in source device must return HDMI_CEC_IO_INVALID_ARGUMENT@n@n
 *
 *
 * @param[in] handle                              - The handle returned from the HdmiCecOpen() 
 *                                                    function. Non zero value
 * @param[in] logicalAddresses                    - The logical address to be acquired
 *
 * @return HDMI_CEC_STATUS                        - Status
 * @retval HDMI_CEC_IO_SUCCESS                    - POLL message is sent successfully and not 
 *                                                    ACK'd by any device on the bus
 * @retval HDMI_CEC_IO_NOT_OPENED                 - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT           - Parameter passed to this function is invalid
 *                                                  i.e. be if any logical address less than 0x0 and greater than 0xF is given as argument
 * @retval HDMI_CEC_IO_INVALID_HANDLE             - An invalid handle argument has been passed
 *
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 * 
 * @see HdmiCecRemoveLogicalAddress(), HdmiCecGetLogicalAddress()
 */ 
HDMI_CEC_STATUS HdmiCecAddLogicalAddress(int handle, int logicalAddresses);

/**
 * @brief Clears the Logical Addresses claimed by the host device
 *
 * This function releases the previously acquired logical address.@n
 * Once released,
 * 1. This API must set the logical address to the default value (0xF).
 * 2. Also the module must not ACK any POLL message destined to the released address.@n
 *
 * Subsequent calls to this API will return HDMI_CEC_IO_SUCCESS.
 *
 * This API is only applicable for sink devices. Invoking this API in source device must return HDMI_CEC_IO_INVALID_ARGUMENT@n@n
 * 
 *
 * @param[in] handle                   - The handle returned from the HdmiCecOpen(). Non zero value
 * @param[in] logicalAddresses         - The logicalAddresses to be released
 *
 * @return HDMI_CEC_STATUS                        - Status
 * @retval HDMI_CEC_IO_SUCCESS                    - Success
 * @retval HDMI_CEC_IO_NOT_OPENED                 - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT           - Parameter passed to this function is invalid -
 *                                                  i.e. if any logical address less than 0x0 and greater than 0xF is given as argument
 * @retval HDMI_CEC_IO_NOT_ADDED                  - Logical address was never added before [or] was previously removed successfully
 * @retval HDMI_CEC_IO_INVALID_HANDLE             - An invalid handle argument has been passed
 * @retval HDMI_CEC_IO_OPERATION_NOT_SUPPORTED    - Operation not supported. This API is not required if the SOC is performing the logical address discovery.
 *
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 * @see HdmiCecAddLogicalAddress(), HdmiCecGetLogicalAddress()
 * 
 */
HDMI_CEC_STATUS HdmiCecRemoveLogicalAddress(int handle, int logicalAddresses);

/**
 * @brief Gets the Logical Address obtained by the module
 *
 * This function gets the logical address for the specified device type. @n
 * For sink devices, if logical address is not added or removed, 
 *    the logical address returned will be 0x0F.
 * For source devices, logical address returned must be based on the device type
 *    as defined in HDMI Specification.
 * 
 * @param[in] handle                    - The handle returned from the HdmiCecOpen(). Non zero value
 * @param[out] logicalAddress           - The logical address acquired
 *
 * @return HDMI_CEC_STATUS              - Status
 * @retval HDMI_CEC_IO_SUCCESS          - Success
 * @retval HDMI_CEC_IO_NOT_OPENED       - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT - Parameter passed to this function is invalid
 * @retval HDMI_CEC_IO_INVALID_HANDLE   - An invalid handle argument has been passed
 *
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 * @note This API is not required if the SOC is performing the logical address discovery.
 * @see HdmiCecAddLogicalAddress(), HdmiCecRemoveLogicalAddress()
 *
 * 
 */
HDMI_CEC_STATUS HdmiCecGetLogicalAddress(int handle, int *logicalAddress);


/**
 * @brief Gets the Physical Address obtained by the module
 *
 * This function gets the Physical address for the specified device type.
 *
 * @param[in] handle            - The handle returned from the HdmiCecOpen(). Non zero value
 * @param[out] physicalAddress  - Physical address acquired
 *    The valid Physical address is less than F.F.F.F
 *    The Sink device at root will take 0.0.0.0 as the Physical Address
 * 
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 * @see HdmiCecGetLogicalAddress()
 *
 * @return HDMI_CEC_STATUS              - Status
 * @retval HDMI_CEC_IO_SUCCESS          - Success
 * @retval HDMI_CEC_IO_NOT_OPENED       - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT - Parameter passed to this function is invalid
 * @retval HDMI_CEC_IO_INVALID_HANDLE   - An invalid handle argument has been passed
 * @retval HDMI_CEC_IO_INVALID_OUTPUT   - Physical address can't be retrieved because it is outside the valid range
 * 
 */
HDMI_CEC_STATUS HdmiCecGetPhysicalAddress(int handle, unsigned int *physicalAddress);


/**
 * @brief Sets CEC message receive callback
 *
 * This function sets the callback function to be invoked for each message arrival@n
 * The message contained in the buffer will follow this format
 *     (ref <HDMI Specification 1-4> Section <CEC 6.1>) :
 * 
 * complete message  = header block + data block@n
 * header block     = destination logical address (4-bit) + source address (4-bit)@n
 * data block       = opcode block (8-bit) + operand block (N-bytes)
 *
 * @code
 * |------------------------------------------------
 * | header block  |          data blocks          |
 * |------------------------------------------------
 * |3|2|1|0|3|2|1|0|7|6|5|4|3|2|1|0|7|6|5|4|3|2|1|0|  
 * |------------------------------------------------
 * | Dest  |  src  |  opcode block | operand block |
 * |------------------------------------------------
 * @endcode
 *
 * When receiving, the returned buffer should not contain EOM and ACK bits. HAL internal logic.@n
 * HAL implementation should remove the EOM and ACK bits in the returned buffer
 * 
 * When transmitting, it is HAL's responsibility to insert EOM bit and ACK bit 
 * for each header or data block.
 *
 * When HdmiCecSetRxCallback() is called, it replaces the previous set cbfunc and data
 * values. Setting a value of (cbfunc=null) disables the callback.
 *
 * This function will block if callback invocation is in progress.
 *
 * @param[in] handle                    - The handle returned from the HdmiCecOpen(() function. Non zero value
 * @param[in] cbfunc                    - Function pointer to be invoked 
 *                                          when a complete message is received
 * @param[in] data                      - Callback data
 *
 * @return HDMI_CEC_STATUS              - Status
 * @retval HDMI_CEC_IO_SUCCESS          - Success
 * @retval HDMI_CEC_IO_NOT_OPENED       - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_HANDLE   - An invalid handle argument has been passed
 *
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 * @see HdmiCecTx(), HdmiCecTxAsync(), HdmiCecSetTxCallback()
 * 
 */
HDMI_CEC_STATUS HdmiCecSetRxCallback(int handle, HdmiCecRxCallback_t cbfunc, void *data);

/**
 * @note This API is deprecated.
 *
 * @brief Sets CEC message transmit callback
 *
 * This function sets a callback which will be invoked once the async transmit
 * result is available. This is only necessary if the caller chooses to transmit
 * the message asynchronously.
 *
 * This function will block if callback invocation is in progress.
 *
 * @param[in] handle                    - The handle returned from the HdmiCecOpen(). Non zero value.
 * @param[in] cbfunc                    - Function pointer to be invoked 
 *                                          when a complete message is transmitted
 * @param[in] data                      - Callback data
 * 
 * @return HDMI_CEC_STATUS              - Status
 * @retval HDMI_CEC_IO_SUCCESS          - Success
 * @retval HDMI_CEC_IO_NOT_OPENED       - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_HANDLE   - An invalid handle argument has been passed
 *
 * @pre HdmiCecOpen() must be called before calling this API.
 * @warning This API is NOT thread safe.
 * @see HdmiCecTx(), HdmiCecTxAsync()
 * 
 */
HDMI_CEC_STATUS HdmiCecSetTxCallback(int handle, HdmiCecTxCallback_t cbfunc, void *data);

/**
 * @brief Synchronous transmit call
 *
 * This function writes a complete CEC message onto the bus and waits for ACK.
 *
 * The message contained in the buffer will follow the format detailed in HdmiCecSetRxCallback_t().
 * (ref <HDMI Specification 1-4> Section <CEC 6.1>)
 *
 *
 * @param[in] handle                              - The handle returned from the 
 *                                                    HdmiCecOpen() function. Non zero value
 * @param[in] buf                                 - The buffer contains a complete 
 *                                                    CEC message to send.
 * @param[in] len                                 - Number of bytes in the message.
 * @param[out] result                             - send status buffer. Possible results(valid only for directly addressed messages) are 
 *                    HDMI_CEC_IO_SENT_AND_ACKD,
 *                    HDMI_CEC_IO_SENT_BUT_NOT_ACKD (e.g. no follower at the destination),
 *                    HDMI_CEC_IO_SENT_FAILED (e.g. collision).
 *
 * @return HDMI_CEC_STATUS                        - Status
 * @retval HDMI_CEC_IO_SUCCESS                    - Success
 * @retval HDMI_CEC_IO_NOT_OPENED                 - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT           - Parameter passed to this function is invalid
 * @retval HDMI_CEC_IO_INVALID_HANDLE             - An invalid handle argument has been passed
 * @retval HDMI_CEC_IO_SENT_AND_ACKD              - Cec message is send and acknowledged.
 * @retval HDMI_CEC_IO_SENT_BUT_NOT_ACKD          - Message sent but not acknowledged 
 *                                                    by the receiver. Host device is trying to 
 *                                                    send an invalid logical address
 * @retval HDMI_CEC_IO_SENT_FAILED                - Send message failed
 *
 * @pre  HdmiCecOpen() should be called before calling this API.
 * @warning  This API is Not thread safe.
 * @see HdmiCecTxAsync(), HdmiCecSetRxCallback()
 *
 */
HDMI_CEC_STATUS HdmiCecTx(int handle, const unsigned char *buf, int len, int *result);

/**
 * @note This API is deprecated.
 *
 * @brief Writes CEC message onto bus asynchronously.
 *
 * This function writes a complete CEC message onto the bus but does not wait
 * for ACK. The result will be reported via HdmiCecRxCallback_t()
 *
 *
 * @param[in] handle                              - The handle returned from the
 *                                                    HdmiCecOpen() function. Non zero value
 * @param[in] buf                                 - The buffer contains a complete
 *                                                    CEC message to send
 * @param[in] len                                 - Number of bytes in the message
 *
 * @return HDMI_CEC_STATUS                        - Status
 * @retval HDMI_CEC_IO_SUCCESS                    - Success
 * @retval HDMI_CEC_IO_NOT_OPENED                 - Module is not initialised
 * @retval HDMI_CEC_IO_INVALID_ARGUMENT           - Parameter passed to this function is invalid
 * @retval HDMI_CEC_IO_INVALID_HANDLE             - An invalid handle argument has been passed
 *
 * @pre  HdmiCecOpen(), HdmiCecSetRxCallback(), HdmiCecSetTxCallback()  should be called before calling this API.
 * @warning  This API is Not thread safe.
 * @see HdmiCecTx(), HdmiCecSetRxCallback()
 *
 */
HDMI_CEC_STATUS HdmiCecTxAsync(int handle, const unsigned char *buf, int len);
#ifdef __cplusplus
}
#endif
#endif // End of __HDMI_CEC_DRIVER_H__

/** @} */ // End of HDMI_CEC_DRIVER_H
/** @} */ // End of HDMI_CEC_HAL
/** @} */ // End of HDMI_CEC
/** @} */ // End of HPK
