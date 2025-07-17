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
 * @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the Audio subsystem. The Audio subsystem manages audio-specific HAL operations.
 *  @{
 */

/**
 * @addtogroup dsAudio_HAL DS Audio HAL
 *  @{
 * @par Application API Specification
 * dsAudio HAL provides an interface for managing Audio related settings
 */

/**
 * @defgroup dsAudio_Settings_H DS Audio Settings Header
 *  @{
 * @par Application API Specification
 * dsAudio provides a template for various Audio configurations
 */

#ifndef __DS_AUDIO_SETTINGS_H__
#define __DS_AUDIO_SETTINGS_H__

#include "dsUtl.h"
#include "dsAVDTypes.h"

#ifdef __cplusplus
extern "C" {
namespace  {
#endif



#define dsAUDIOPORT_TYPE_NUM_MAX 4     /**<  Maximum number of audio output port types */
#define dsAudioPORT_NUM_MAX 4          /**<  Maximum number of audio output ports */

/**
 * @brief This enumeration defines the supported audio configurations
 */

const dsAudioPortType_t 	 kSupportedPortTypes[] 				= { dsAUDIOPORT_TYPE_HDMI_ARC, dsAUDIOPORT_TYPE_SPDIF };

const dsAudioEncoding_t 	kSupportedARCEncodings[]			= { dsAUDIO_ENC_PCM, dsAUDIO_ENC_AC3};  /**< Audio supported port types */
const dsAudioCompression_t 	kSupportedARCCompressions[] 			= { dsAUDIO_CMP_NONE, dsAUDIO_CMP_LIGHT, dsAUDIO_CMP_MEDIUM, dsAUDIO_CMP_HEAVY, }; /**< Audio supported HDMI compression types */
const dsAudioStereoMode_t 	kSupportedARCStereoModes[] 			= { dsAUDIO_STEREO_STEREO, dsAUDIO_STEREO_SURROUND, }; /**< Audio supported HDMI stereo mode types */

const dsAudioEncoding_t 	kSupportedSPDIFEncodings[] 			= { dsAUDIO_ENC_PCM, dsAUDIO_ENC_AC3, }; /**< Audio supported SPDIF encoding types */
const dsAudioCompression_t 	kSupportedSPDIFCompressions[] 			= { dsAUDIO_CMP_NONE, dsAUDIO_CMP_LIGHT, dsAUDIO_CMP_MEDIUM, dsAUDIO_CMP_HEAVY, }; /**< Audio supported SPDIF compression types */
const dsAudioStereoMode_t 	kSupportedSPDIFStereoModes[] 			= { dsAUDIO_STEREO_STEREO, dsAUDIO_STEREO_SURROUND, }; /**< Audio supported SPDIF stereo mode types */

/**
 * @brief This enumeration defines the supported Audio port types
 */
const dsAudioTypeConfig_t 	kConfigs[]= {
		{
		/*.typeId = */				dsAUDIOPORT_TYPE_HDMI_ARC,
		/*.name = */				"HDMI_ARC", //HDMI
		/*.numSupportedCompressions = */	dsUTL_DIM(kSupportedARCCompressions),
		/*.compressions = */			kSupportedARCCompressions,
		/*.numSupportedEncodings = */		dsUTL_DIM(kSupportedARCEncodings),
		/*.encodings = */			kSupportedARCEncodings,
		/*.numSupportedStereoModes = */		dsUTL_DIM(kSupportedARCStereoModes),
		/*.stereoModes = */			kSupportedARCStereoModes,
		},

		{
		/*.typeId = */				dsAUDIOPORT_TYPE_SPDIF,
		/*.name = */				"SPDIF", //SPDIF
		/*.numSupportedCompressions = */	dsUTL_DIM(kSupportedSPDIFCompressions),
		/*.compressions = */			kSupportedSPDIFCompressions,
		/*.numSupportedEncodings = */		dsUTL_DIM(kSupportedSPDIFEncodings),
		/*.encodings = */			kSupportedSPDIFEncodings,
		/*.numSupportedStereoModes = */		dsUTL_DIM(kSupportedSPDIFStereoModes),
		/*.stereoModes = */			kSupportedSPDIFStereoModes,
		}
};

/**
 * @brief This enumeration defines the video ports that are connected to each audio port
 *
 * This is left empty since in this example there is no LR Audio port but needs to
 * corresponds to @link kPorts @endlink
 *
 */
const dsVideoPortPortId_t connectedVOPs[dsAUDIOPORT_TYPE_MAX][dsVIDEOPORT_TYPE_MAX] = {
		/**
		 * VOPs connected to LR Audio
		 *
		*/
		{

		},
		{/*VOPs connected to HDMI Audio */

		},
		{/*VOPs connected to SPDIF Audio */
				{dsVIDEOPORT_TYPE_INTERNAL, 0},
		},
		{/*VOPs connected to Speaker Audio */

		},
		{/*VOPs connected to HDMI ARC */
				{dsVIDEOPORT_TYPE_INTERNAL, 0},
		},
		{/*VOPs connected to Headphone */

		},
};

/**
 * @brief This enumeration defines the available audio ports on the device
 */
const dsAudioPortConfig_t kPorts[] = {
		{
		/*.typeId = */ 				{dsAUDIOPORT_TYPE_HDMI_ARC, 0},
		/*.connectedVOPs = */			connectedVOPs[dsAUDIOPORT_TYPE_HDMI_ARC],
		},
		{
		/*.typeId = */ 				{dsAUDIOPORT_TYPE_SPDIF, 0},
		/*.connectedVOPs = */			connectedVOPs[dsAUDIOPORT_TYPE_SPDIF],
		},
};

#ifdef __cplusplus
}
}
#endif
#endif /* __DS_AUDIO_SETTINGS_H__ */

/** @} */ // End of DS Audio Settings Header
/** @} */ // End of DS Audio HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK

