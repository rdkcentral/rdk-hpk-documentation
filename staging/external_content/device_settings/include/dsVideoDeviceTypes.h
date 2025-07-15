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

/** @addtogroup Device_Settings Device Settings Module
 * @{
 */

/** @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the Video Device subsystem. The Video Device subsystem manages system-specific HAL operations.
 *  @{
 */

/** @addtogroup dsVIDEODEVICE_HAL DS Video Device HAL
 *  @{
 * @par Application API Specification
 * dsVideoDevice HAL provides an interface for managing the VideoDevice settings for the device settings module
 */

/** @defgroup DSHAL_VIDEODEVICE_TYPES DS Video Device Type Definitions
 *  @ingroup dsVIDEODEVICE_HAL
 *  @{
 */



/**
 * @file dsVideoDeviceTypes.h
 *
 * @brief Device Setting Video Device HAL types.
 *
 * This file defines common types used by the above mentioned DS Video Device HAL Submodule.
 *
 * @par Document
 * Document reference
 *
 * @par Open Issues (in no particular order)
 * -# None
 *
 * @par Assumptions
 * -# None
 *
 * @par Abbreviations
 * - FPD:     Front-Panel Display
 * - HAL:     Hardware Abstraction Layer
 * - SAD:     Short Audio Descriptors
 * - EDID:    Extended Display Information Data
 * - HEVC:    High Efficiency Video Coding
 * - DTCP:    Digital Transmission Content Protection
 * - HDCP:    High-bandwidth Digital Copy Protection
 * - HDR:     High Dynamic Range
 * - HDMI:    High-Definition Multimedia Interface
 * - ARC:     Audio Return Channel
 * - eARC:    Enhance Audio Return Channel
 * - RF:      Radio Frequency.
 * - MS12:    MultiStream 12.
 * - SPDIF:   Sony/Phillips Digital Interface
 * - AC-3:    Audio Compression 3
 * - CEA:     Consumer Electornic Association
 * - ITU:     International Telecommunication Union
 * - ITU-R:   ITU Radiocommunication Sector
 * - BT:      British Telecomm
 * - SMPTE:   Society of Television and Motion Picture Engineers
 * - YCbCr:   Y is luma (brightness), Cb is blue minus luma (B-Y) and Cr is red minus luma (R-Y)
 * - XvYCC:   extended-gamut YCbCr
 * - RGB:     Red Green Blue
 * - eDVI:    Digital Video Interface
 * - eFCC:    Future Computing and Communication
 * - IEC:     International Electrotechnical Commission
 * - MPEG4:   Moving Picture Experts Group version 4, part 14
 * - AVC:     Advanced Video Coding
 * - LED:     Light-Emitting Diode
 * - SCART:   SCART stands for Syndicat des Constructeursd’AppareilsRadiorécepteurs et Téléviseurs 
 *                      or Radio and Television Receiver Manufacturers
 *
 * @par Implementation Notes
 * -# None
 *
 */

#ifndef __DS_VIDEO_DEVICE_TYPES_H__
#define __DS_VIDEO_DEVICE_TYPES_H__

#pragma once
#include <sys/types.h>



/**
 * @brief This enumeration defines all of the standard screen zoom (format conversion) modes.
 * @note also used @link dsHdmiIn.h @endlink
 */
typedef enum _dsVideoZoom_t {
    dsVIDEO_ZOOM_UNKNOWN = -1,      /*!< Unknown mode */
    dsVIDEO_ZOOM_NONE = 0,          /*!< Decoder format conversion is inactive */
    dsVIDEO_ZOOM_FULL,              /*!< Full screen (16:9 video is streched to fit 4:3 frame) */
    dsVIDEO_ZOOM_LB_16_9,           /*!< 16:9 Letterbox (16:9 video is placed in a 4:3 frame with a full letterbox effect) */
    dsVIDEO_ZOOM_LB_14_9,           /*!< 14:9 Letterbox (16:9 video is transferred into a 4:3 frame with 14:9 */
                                    /*!< letterbox effect) */
    dsVIDEO_ZOOM_CCO,               /*!< Center Cut-out (16:9 video is cut to fill 4:3 frame with its center part) */
    dsVIDEO_ZOOM_PAN_SCAN,          /*!< Pan & Scan (16:9 is cut and panned to fille 4:3 frame, following MPEG pan & scan vectors) */
    dsVIDEO_ZOOM_LB_2_21_1_ON_4_3,  /*!< 2.21:1 Letterbox on 4:3 (video is transferred into 4:3 frame with a 2.21:1 letterbox effect) */
    dsVIDEO_ZOOM_LB_2_21_1_ON_16_9, /*!< 2.21:1 Letterbox on 16:9 (video is transferred into 16:9 frame with a 2.21:1 letterbox effect) */
    dsVIDEO_ZOOM_PLATFORM,          /*!< Control over the decoder format conversions is managed by the platform  */
    dsVIDEO_ZOOM_16_9_ZOOM,         /*!< 16:9 Zoom (4:3 video is zoomed to fill 16:9 frame) */
    dsVIDEO_ZOOM_PILLARBOX_4_3,     /*!< Pillarbox 4:3 (4:3 video is placed in a 16:9 frame with a pillarbox effect) */
    dsVIDEO_ZOOM_WIDE_4_3,          /*!< Wide 4:3 (4:3 video is stretched to fill 16:9 frame) */
    dsVIDEO_ZOOM_MAX                /*!< Out of range */
}dsVideoZoom_t;

/**
 * @brief Video screen zoom validation check
 */
#define dsVideoPortDFC_isValid(t)  (((t) >= dsVIDEO_ZOOM_NONE ) && ((t) < dsVIDEO_ZOOM_MAX))

/**
 * @brief Structure that defines video device configuration for the output
 */
typedef struct _dsVideoConfig_t {
    size_t numSupportedDFCs;            /*!< Number of zoom modes supported */
    const dsVideoZoom_t *supportedDFCs; /*!< List of zoom modes supported  */
    dsVideoZoom_t defaultDFC;           /*!< The default zoom mode         */
} dsVideoConfig_t;

/**
 * @brief List of video compression formats that may be supported by the decoder
 */
typedef enum
{
    dsVIDEO_CODEC_MPEGHPART2 = (0x01 << 0),     /*!< Also known HEVC, H.265 */
    dsVIDEO_CODEC_MPEG4PART10 = (0x01 << 1),    /*!< Also known as H.264, MPEG4 AVC */
    dsVIDEO_CODEC_MPEG2 = (0x01 << 2),          /*!< Also known as H.222/H.262 */
    dsVIDEO_CODEC_MAX   = (0x01 << 3)           /*!< Out of range */
} dsVideoCodingFormat_t;

/**
 * @brief List of HEVC Profiles
 * @note HEVC version 1 profiles are listed. More may be added to it as the support becomes available
 */
typedef enum
{
    dsVIDEO_CODEC_HEVC_PROFILE_MAIN = (0x01 << 0),              /*!< 8-bit HEVC video profile. */
    dsVIDEO_CODEC_HEVC_PROFILE_MAIN10 = (0x01 << 1),            /*!< 10-bit HEVC video profile. */
    dsVIDEO_CODEC_HEVC_PROFILE_MAINSTILLPICTURE = (0x01 << 2),  /*!< HECV Main Still Picture profile */
    dsVIDEO_CODEC_HEVC_MAX  = (0x01 << 3)                       /*!< Out of range  */
} dsVideoCodecHevcProfiles_t;

/**
 * @brief Structure type for HEVC profiles
 */
typedef struct
{
   dsVideoCodecHevcProfiles_t profile;  /*!< HEVC Profiles. See dsVideoCodecHevcProfiles_t */
   float level;                         /*!< level for the specieis HEVC profile */
} dsVideoCodecProfileSupport_t;

/**
 * @brief Structure type for Video codec info
 */
typedef struct
{
    unsigned int num_entries;                  /*!< Number of entries */
    dsVideoCodecProfileSupport_t entries[10];  /*!< Contains a list of the supported Codex profiles */
} dsVideoCodecInfo_t;

#endif // End of __DS_VIDEO_DEVICE_TYPES_H__

/** @} */ // End of DSHAL_VIDEODEVICE_TYPES
/** @} */ // End of DS Video Device HAL
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
