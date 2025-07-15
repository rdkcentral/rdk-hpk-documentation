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
/***************************************************************************
*
*  Rmf Audio Capture ->  RAC -> rac
*/

/**
 * @addtogroup HPK HPK
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
 * @addtogroup RMF_AudioCapture RMF Audio Capture
 * @{
 * @par Application API Specification
 * RMF AudioCapture is the RDK interface designed to tap into the audio output
 * of an STB/TV. When in use, the underlying SoC implementation will deliver a
 * steady stream of raw audio data (PCM). This audio will include the active
 * audio track of whatever content is being played, and may include other sounds
 * mixed in, such as voice-guidance, application-driven audio overlays (eg:
 * beeps) etc. RDK will attempt to configure audio format parameters (channels,
 * bit rate etc.) to its liking but not all combinations are expected to be
 * supported by the SoC implementation.
 *
 */

/**
 * @defgroup RMF_AudioCapture_HAL RMF Audio Capture HAL
 * @{
 */

#ifndef __RMF_AUDIO_CAPTURE_H__
#define __RMF_AUDIO_CAPTURE_H__

#include <stddef.h>  /* size_t */
#include <inttypes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque handle to an instance of RMF AudioCapture interface.
 *
 */
typedef struct RMF_AudioCapture_Struct *RMF_AudioCaptureHandle;

/**
 * @brief RMF Error Codes
 *
 */
#ifndef _RMF_ERROR_H_
typedef enum
{
    RMF_SUCCESS = 0,        //!< Success
    RMF_ERROR,              //!< Generic catch-all error code
    RMF_INVALID_PARM,       //!< Invalid parameter(s)
    RMF_INVALID_HANDLE,     //!< Invalid handle
    RMF_NOT_INITIALIZED,    //!< Interface not initialized
    RMF_INVALID_STATE       //!< Invalid state or unsupported sequence of API calls
} RMF_AudioCapture_Return_Code_t;

typedef RMF_AudioCapture_Return_Code_t rmf_Error; //!< Return code, for backward compatibility
#else //_RMF_ERROR_H_
// If rmf_error.h is in use, RMF_SUCCESS is already defined, so use that instead.
#include "rmf_error.h"

typedef enum
{
    RMF_ERROR = 1,          //!< Generic catch-all error code
    RMF_INVALID_PARM,       //!< Invalid parameter(s)
    RMF_INVALID_HANDLE,     //!< Invalid handle
    RMF_NOT_INITIALIZED,    //!< Interface not initialized
    RMF_INVALID_STATE       //!< Invalid state or unsupported sequence of API call
} RMF_AudioCapture_Return_Code_t;
#endif // _RMF_ERROR_H_


typedef char *RMF_AudioCaptureType;         //!< Audio source to be captured. Options are RMF_AC_TYPE_PRIMARY or RMF_AC_TYPE_AUXILIARY
#define RMF_AC_TYPE_PRIMARY "primary"       //!< Primary audio that is currently selected to play with video
#define RMF_AC_TYPE_AUXILIARY "auxiliary"   //!< Audio source that is different from primary audio - can be audio track with a different language

/**
 * @brief Describes more specifics about the audio parameters to be used for audio samples
 *
 */
typedef enum
{
    racFormat_e16BitStereo,    //!< Stereo, 16 bits per sample interleaved into a 32-bit word
    racFormat_e24BitStereo,    //!< Stereo, 24 bits per sample.  The data is aligned to 32-bits left-justified.  Left and right channels will interleave one sample per 32-bit word
    racFormat_e16BitMonoLeft,  //!< Mono, 16 bits per sample interleaved into a 32-bit word. Left channel samples only
    racFormat_e16BitMonoRight, //!< Mono, 16 bits per sample interleaved into a 32-bit word. Right channel samples only
    racFormat_e16BitMono,      //!< Mono, 16 bits per sample interleaved into a 32-bit word. Left and Right channels mixed
    racFormat_e24Bit5_1,       //!< 5.1 Multichannel, 24 bits per sample.  The data is aligned to 32-bits, left-justified. Channels will interleave one sample per 32-bit word, ordered L,R,Ls,Rs,C,LFE
    racFormat_eMax
} racFormat;

/**
 * @brief Audio sampling rate
 */
typedef enum
{
    racFreq_e16000,         //!< 16KHz
    racFreq_e22050,         //!< 22.05KHz
    racFreq_e24000,         //!< 24KHz
    racFreq_e32000,         //!< 32KHz
    racFreq_e44100,         //!< 44.1KHz
    racFreq_e48000,         //!< 48KHz
    racFreq_eMax
} racFreq;

/**
 * @brief Callbacks
 */

/**
 * @brief Callback via which SoC will send captured audio samples
 *
 * The callback will not take ownership of AudioCaptureBuffer. It will consume the audio data completely before returning and it is the responsibility
 * of the caller to free/manage this memory.
 *
 * @param [in] cbBufferReadyParm - Caller context data passed back (optional) in the callback
 * @param [in] AudioCaptureBuffer - Pointer to the buffer holding audio data
 * @param [in] AudioCaptureBufferSize - Size of audio data available in buffer
 *
 * @return Indicates an error if a non-zero value is returned
 *
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_PARM Invalid parameter
 *
 * @see RMF_AudioCapture_Settings, RMF_AudioCapture_Start()
 */
typedef rmf_Error (*RMF_AudioCaptureBufferReadyCb)(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize);

/**
 * @brief Callback for underlying implementation to signal a change in status
 *
 * RDK may then call RMF_AudioCapture_GetStatus() to get more details. This callback is optional and may be NULL. If not NULL, the underlying
 * implementation must invoke this callback when there is a change of state (started vs stopped), or audio parameters (format, sampling rate)
 * currently in use. If the implementation experiences any adverse events that hinder an active capture session, implementation shall change its 
 * internal state to stopped and invoke this callback to notify caller.
 *
 * @param [in] cbStatusParm - Caller context data passed back (optional) in the callback
 *
 * @return Indicates an error if a non-zero value is returned.
 *
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_PARM Invalid parameter
 *
 * @see RMF_AudioCapture_Settings, RMF_AudioCapture_Start()
 */
typedef rmf_Error (*RMF_AudioCapture_StatusChangeCb)(void *cbStatusParm);


/**
 * @brief Configuration parameters of audio capture interface
 */
typedef struct
{
    RMF_AudioCaptureBufferReadyCb cbBufferReady;    //!< Callback to send audio data to caller. Must be set in Open call
    void *cbBufferReadyParm;                        //!< Caller context data passed back in buffer-ready callback; can be NULL

    RMF_AudioCapture_StatusChangeCb cbStatusChange; //!< If cbStatusChange is not NULL, then the underlying implementation should invoke this callback when there is a change of state in the audio capture interface. Examples: OnStart of AC, Change from Start to Stop of AC. On underflow/overflow
    void *cbStatusParm;                             //!< Caller context data passed back in status change callback

    size_t fifoSize;                                //!< FIFO size in bytes.  This value is a total FIFO size to hold all channels of data. If not set, a default size will be used.  Needs to be big enough to avoid overflow (expected service time * byte rate). It should be large enough to hold at least 333 ms of audio data. If FIFO size is 0, implementation may choose a size.
    size_t threshold;                               //!< FIFO data callback threshold in bytes.  When the amount of data in the FIFO reaches this level, the buffer-ready will be invoked. It should at most be 1/4th of FIFO size. If FIFO size is 0, vendor may set a threshold that's at most 1/4th its chosen FIFO size.
    racFormat format;                               //!< Format of captured data.  Default is racFormat_e16BitStereo. This value is ignored for compressed data, and can not be changed while connected to any inputs. Little-endian byte order is assumed for PCM data.
    racFreq samplingFreq;                           //!< Sampling rate of captured audio. Not currently supported. TBD
    uint32_t    delayCompensation_ms;               //!< Delay compensation in milli seconds. This parameter is used to maintain AV sync when using latency-prone audio outputs like Bluetooth. It's the number of ms to delay video by in order to stay in sync with Bluetooth audio
} RMF_AudioCapture_Settings;

/**
 * @brief Status of audio capture interface
 *
 */
typedef struct
{
    int8_t  started;        //!< Indicates whether capture has started
    racFormat format;       //!< Current capture format (bit depth & channel)
    racFreq samplingFreq;   //!< Current capture sample rate
    size_t fifoDepth;       //!< Number of bytes in local fifo.
    uint32_t overflows;     //!< Overflow count
    uint32_t underflows;    //!< Underflow count
    int8_t muted;           //!< Deprecated. Indicates whether capture is muted
    int8_t paused;          //!< Deprecated. Indicates whether capture is paused
    float volume;           //!< Deprecated. Current capture volume

} RMF_AudioCapture_Status;


/*
 * Interfaces
 */


/**
 * @brief Opens the audio capture interface for primary audio
 *
 * This function or RMF_AudioCapture_Open_Type() will be the first call when this library is used. Any other interface exposed by this library.
 * Underlying implementation must acquire the necessary hardware and software resources necessary to capture audio.
 * RMF_AudioCapture doesn't expect more than one primary capture session to be available concurrently and will close the open session before calling open again.
 *
 * @param [out] handle - An opaque capture interface handle, which should not be modified by caller and has to be passed as an argument
 *                          for all subsequent API calls
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_PARM Invalid parameter
 * @retval RMF_INVALID_STATE Interface is already open for primary source
 *
 * @see RMF_AudioCapture_Close()
 * @note Should the application crash, it's recommended that the HAL be capable of automatically releasing any hardware resources, either when terminating or when
 * the application restarts and RMF_AudioCapture_Open()/RMF_AudioCapture_Open_Type() again
 */
rmf_Error RMF_AudioCapture_Open (RMF_AudioCaptureHandle* handle);


/**
 * @brief Opens the audio capture interface for specified audio source.
 *
 * This function or RMF_AudioCapture_Open() will be the first call when this library is used. Underlying implementation must acquire the necessary hardware and 
 * software resources to capture audio. RMF_AudioCapture may have both primary and auxiliary capture sessions open at the same time. 
 * There will not be more than one capture session open for the same capture type at any point of time.
 *
 * @param [out] handle - An opaque capture interface handle, which should not be modified by caller and has to be passed as an argument
 *                          for all subsequent API calls.
 * @param [in] rmfAcType - Source of audio to be captured (primary or auxiliary)
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_PARM Invalid parameter
 * @retval RMF_INVALID_STATE Interface is already open for this audio capture type
 *
 * @see RMF_AudioCapture_Close()
 * @note Should the application crash, it's recommended that the HAL be capable of automatically releasing any hardware resources, either when terminating or when
 * the application restarts and calls RMF_AudioCapture_Open()/RMF_AudioCapture_Open_Type() again.
 */
rmf_Error RMF_AudioCapture_Open_Type (RMF_AudioCaptureHandle* handle, RMF_AudioCaptureType rmfAcType);

/**
 * @brief Gets the current status of audio capture interface
 *
 * This API can be called as long as a valid handle is available (after opening and before close).
 * API may be invoked as a resonse to RMF_AudioCapture_StatusChangeCb(), possibly inside the callback itself.
 *
 * @param [in] handle - Handle of the audio capture interface.
 * @param [out] status - Status returned by the underlying implementation. The life-cycle of status will be managed by the caller.
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_PARM Invalid parameter
 * @retval RMF_INVALID_HANDLE Invalid handle
 *
 * @pre Must possess a valid handle obtained using RMF_AudioCapture_Open() before invoking this function.
 * @see RMF_AudioCapture_Open()
 */
rmf_Error RMF_AudioCapture_GetStatus (RMF_AudioCaptureHandle handle, RMF_AudioCapture_Status* status);


/**
 * @brief Returns friendly default values for @b RMF_AudioCapture_Settings
 *
 * Caller will use this to understand what audio-related parameters preferable for this interface. Caller may then use this structure as a
 * baseline and tweak only stricly necessary parameters before passing it with RMF_AudioCapture_Start(). The defaults are not expected to change
 * no matter how the capture interface was configured by caller previously (if at all). Caller will only call this API when the interface is in
 * OPEN or STARTED state.
 *
 * @param [out] settings - Default values for audio capture settings. The life-cycle of settings will be managed by the caller.
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_PARM Invalid parameter
 * @retval RMF_INVALID_STATE Interface is not in OPEN or STARTED state
 *
 * @see RMF_AudioCapture_Start()
 */
rmf_Error RMF_AudioCapture_GetDefaultSettings (RMF_AudioCapture_Settings* settings);

/**
 * @brief Returns current values of @b RMF_AudioCapture_Settings in effect
 *
 * The output should match the configuration previously set successfully via RMF_AudioCapture_Start()
 *
 * @param [in] handle - Handle of the audio capture interfac
 * @param [out] settings - Current values of audio capture settings. The life-cycle of settings will be managed by the caller.
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_HANDLE Invalid handle
 * @retval RMF_INVALID_PARM Invalid parameter
 * @retval RMF_INVALID_STATE Capture interface is not in STARTED state
 *
 * @see RMF_AudioCapture_Start()
 * @pre Must call RMF_AudioCapture_Start() before invoking this function
 */
rmf_Error RMF_AudioCapture_GetCurrentSettings (RMF_AudioCaptureHandle handle, RMF_AudioCapture_Settings* settings);


/**
 * @brief Starts audio capture
 *
 * HAL must apply the new settings before starting audio capture. Underlying implementation must invoke RMF_AudioCaptureBufferReadyCb() repeatedly to deliver
 * the data in accordance with the FIFO thresholds set. This process must continue until RMF_AudioCapture_Stop() is called. Once stopped, RMF_AudioCapture may
 * call RMF_AudioCapture_Start() again so long as RMF_AudioCapture_Close() hasn't been invoked yet. settings.cbBufferReady is not allowed to be NULL.
 * 
 * If there is no capturable audio data (eg: muted audio or no active playback), implementation must still invoke RMF_AudioCaptureBufferReadyCb() filled with
 * silence and maintain expected data rate.
 * 
 * If any of the parameters (eg: audio format) passed in settings are unsupported or invalid, implementation must return RMF_INVALID_PARM. Implementation is not
 * required to support all defined audio formats and/or sampling rates. However, it must support at least 16-bit stereo PCM format.
 *
 * @param [in] handle - Handle of the audio capture interface.
 * @param [in] settings - Capture settings to use. The life-cycle of settings will be managed by the caller.
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_HANDLE Invalid handle
 * @retval RMF_INVALID_PARM Invalid or unsupported setting parameter(s)
 * @retval RMF_INVALID_STATE Capture interface is already in started state
 *
 * @see RMF_AudioCapture_Stop(), RMF_AudioCaptureBufferReadyCb(), RMF_AudioCapture_GetDefaultSettings(), RMF_AudioCapture_Close()
 * @pre Must call RMF_AudioCapture_Open() before invoking this function
 */
rmf_Error RMF_AudioCapture_Start (RMF_AudioCaptureHandle handle, RMF_AudioCapture_Settings* settings);



/**
 * @brief Stops audio capture
 *
 * Underlying implementation must no longer generate RMF_AudioCaptureBufferReadyCb() calls after this. Caller may
 * choose to call RMF_AudioCapture_Start() again to restart capture as long as RMF_AudioCapture_Close() hasn't been called.
 *
 * @param [in] handle - Handle of the audio capture interface
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error.
 * @retval RMF_INVALID_HANDLE Invalid handle.
 * @retval RMF_INVALID_STATE Capture interface is not in STARTED state
 *
 * @see RMF_AudioCapture_Start(), RMF_AudioCaptureBufferReadyCb(), RMF_AudioCapture_Close()
 * @pre Must call RMF_AudioCapture_Start() before invoking this function
 */
rmf_Error RMF_AudioCapture_Stop (RMF_AudioCaptureHandle handle);


/**
 * @brief Closes the audio capture interface
 *
 * This call must free all resources acquired since RMF_AudioCapture_Open() call and will invalidate the handle. The caller will not go directly from
 * STARTED to CLOSED state; it will call stop beforehand. RMF_AudioCapture may choose to open the interface again using RMF_AudioCapture_Open() in future.
 *
 * @param [in] handle - Handle of the audio capture interface
 *
 * @return rmf_Error
 * @retval RMF_SUCCESS Success
 * @retval RMF_ERROR General error
 * @retval RMF_INVALID_HANDLE Invalid handle
 * @retval RMF_INVALID_STATE Capture interface is in STARTED state
 *
 * @see RMF_AudioCapture_Start(), RMF_AudioCapture_Close(), RMF_AudioCapture_Open()
 * @pre Must call RMF_AudioCapture_Open() before invoking this function
 * @pre If RMF_AudioCapture_Start() was invoked, capture must be stopped by calling RMF_AudioCapture_Stop() first
 */
rmf_Error RMF_AudioCapture_Close (RMF_AudioCaptureHandle handle);
#ifdef __cplusplus
}
#endif
/** @} */ // End of RMF_AudioCapture_HAL
/** @} */ // End of RMF_AudioCapture
/** @} */ // End of HPK

#endif /* __RMF_AUDIO_CAPTURE_H__ */
