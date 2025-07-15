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
 * @file dsAVDTypes.h
 *
 * @brief Device Setting Audio,Video and Display HAL types.
 *
 * This file defines common types used by the above mentioned DS HAL Submodules.
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
 * - HAL:     Hardware Abstraction Layer.
 * - SAD:     Short Audio Descriptors
 * - EDID:    Extended Display Information Data.
 * - DTCP:    Digital Transmission Content Protection
 * - HDCP:    High-bandwidth Digital Copy Protection.
 * - HDR:     High Dynamic Range
 * - HDMI:    High-Definition Multimedia Interface
 * - ARC:     Audio Return Channel.
 * - eARC:    Enhance Audio Return Channel
 * - RF:      Radio Frequency.
 * - MS12:    MultiStream 12.
 * - SPDIF:   Sony/Phillips Digital Interface
 * - AC-3:    Audio Compression 3
 * - CEA:     Consumer Electronic Association
 * - ITU:     International Telecommunication Union
 * - ITU-R:   ITU Radiocommunication Sector
 * - BT:      British Telecomm
 * - SMPTE:   Society of Television and Motion Picture Engineers
 * - YCbCr:   Y is luma (brightness), Cb is blue minus luma (B-Y) and Cr is red minus luma (R-Y)
 * - XvYCC:   extended-gamut YCbCr.
 * - RGB:     Red Green Blue
 * - eDVI:    Digital Video Interface
 * - eFCC:    Future Computing and Communication
 * - IEC:     International Electrotechnical Commission
 * - MPEG4:   Moving Picture Experts Group version 4, part 14.
 * - LED:     Light-Emitting Diode.
 * - SCART:   SCART stands for Syndicat des Constructeursd’AppareilsRadiorécepteurs et Téléviseurs 
 *                      or Radio and Television Receiver Manufacturers.
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

/**
 * @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @addtogroup Device_Settings_HAL Device Settings HAL
 * @par Application API Specification
 * Described herein are the DeviceSettings HAL types and functions that are part of
 * the FPD subsystem. The FPD subsystem manages system-specific HAL operations.
 *  @{
 */

/**
 * @defgroup DSHAL_AVD_TYPES DS HAL Audio Video Display Types Definitions
 *  @ingroup dsAudio_HAL dsDisplay_HAL dsVideoPort_HAL
 *  @{
 */


#ifndef __DS_AVD_TYPES_H__
#define __DS_AVD_TYPES_H__

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#pragma once

/**
 * @ingroup DSHAL_VIDEOPORT_TYPES
 * @brief This enumeration defines all of the standard type of Video ports.
 *
 * @see dsVideoPortType_isValid
 * @note also used in @link dsDisplay.h @endlink
 */
typedef enum _dsVideoPortType_t {
    dsVIDEOPORT_TYPE_RF = 0,      ///< RF modulator (channel 3/4) video output          
    dsVIDEOPORT_TYPE_BB,          ///< Baseband (composite, RCA) video output            
    dsVIDEOPORT_TYPE_SVIDEO,      ///< S-Video video output                              
    dsVIDEOPORT_TYPE_1394,        ///< IEEE 1394 (Firewire) video output
    dsVIDEOPORT_TYPE_DVI,         ///< DVI (Panel-Link, HDCP) video output
    dsVIDEOPORT_TYPE_COMPONENT,   ///< Component video output
    dsVIDEOPORT_TYPE_HDMI,        ///< HDMI video output
    dsVIDEOPORT_TYPE_HDMI_INPUT,  ///< HDMI video input
    dsVIDEOPORT_TYPE_INTERNAL,    ///< Internal (integrated/internal display) video output                                
    dsVIDEOPORT_TYPE_MAX          ///< Out of range               
} dsVideoPortType_t;

/**
 * @brief Video output port type validation check.
 * @see dsVideoPortType_t
 */
#define dsVideoPortType_isValid(t)  (((t) >= dsVIDEOPORT_TYPE_RF ) && ((t) < dsVIDEOPORT_TYPE_MAX))


/**
 * @ingroup DSHAL_VIDEOPORT_TYPES
 * @brief Structure that defines port id associated with video port.
 */
typedef struct _dsVideoPortPortId_t {
    dsVideoPortType_t type; ///< Video port type
    int32_t index;          ///< Port ID/number
} dsVideoPortPortId_t;


/** @addtogroup DSHAL_AUDIO_TYPES Device Settings HAL Audio Types
 *  @{
 */

/**
 * @brief Audio output port type validation check.
 * @see dsAudioPortType_t
 */
#define dsAudioType_isValid(t)  (((t) >= dsAUDIOPORT_TYPE_ID_LR ) && ((t) < dsAUDIOPORT_TYPE_MAX))

/**
 * @brief Audio encoding type validation check.
 * @see dsAudioEncoding_t
 */
#define dsAudioEncoding_isValid(t)  (((t) >= dsAUDIO_ENC_NONE ) && ((t) < dsAUDIO_ENC_MAX))

/**
 * @brief Audio compression type validation check.
 * @see dsAudioCompression_t
 */
#define dsAudioCompression_isValid(t)  (((t) >= dsAUDIO_CMP_NONE ) && ((t) < dsAUDIO_CMP_MAX))

/**
 * @brief Audio stereo mode type validation check.
 * @see dsAudioStereoMode_t
 */
#define dsAudioStereoMode_isValid(t)  (((t) >= dsAUDIO_STEREO_UNKNOWN ) && ((t) < dsAUDIO_STEREO_MAX))

/**
 * @brief Max values for SADs.
 */
#define MAX_SAD 15

/**
 * @brief Max length of Language Code String.
 */
#define MAX_LANGUAGE_LEN 4

/**
 * @brief This enumeration defines the audio port types.
 * @see dsAudioType_isValid
 */
typedef enum _dsAudioPortType_t{
    dsAUDIOPORT_TYPE_ID_LR,     ///< RCA audio output               
    dsAUDIOPORT_TYPE_HDMI,      ///< HDMI audio output               
    dsAUDIOPORT_TYPE_SPDIF,     ///< SPDIF audio output             
    dsAUDIOPORT_TYPE_SPEAKER,   ///< SPEAKER audio output            
    dsAUDIOPORT_TYPE_HDMI_ARC,  ///< HDMI ARC/EARC audio output      
    dsAUDIOPORT_TYPE_HEADPHONE, ///< Headphone jack 
    dsAUDIOPORT_TYPE_MAX        ///< Out of range 
} dsAudioPortType_t;

/**
 * @brief Structure that defines the audio port type and associated ID.
 */
typedef struct _dsAudioPortId_t {
    dsAudioPortType_t type; ///< Audio port type
    int32_t index;          ///< Port ID/number
} dsAudioPortId_t;

/**
 * @brief This enumeration defines the audio encoding types
 * @see dsAudioEncoding_isValid
 */
typedef enum _dsAudioEncoding_t{
    dsAUDIO_ENC_NONE = 0,   ///< No digital audio output            
    dsAUDIO_ENC_DISPLAY,    ///< Platform-selected digital audio encoding format
    dsAUDIO_ENC_PCM,        ///< PCM digital audio encoding format
    dsAUDIO_ENC_AC3,        ///< AC-3 digital audio encoding format            
    dsAUDIO_ENC_EAC3,       ///< DD+/E-AC-3 digital audio encoding format
    dsAUDIO_ENC_MAX         ///< Out of range         
} dsAudioEncoding_t;

/**
 * @brief This enumeration defines the type of audio compression.
 * @see dsAudioCompression_isValid
 */
typedef enum _dsAudioCompression_t{
    dsAUDIO_CMP_NONE,   ///< No audio compression.                    
    dsAUDIO_CMP_LIGHT,  ///< Light audio level compression.           
    dsAUDIO_CMP_MEDIUM, ///< Medium audio level compression.          
    dsAUDIO_CMP_HEAVY,  ///< Heavy audio level compression.           
    dsAUDIO_CMP_MAX     ///< Out of range 
} dsAudioCompression_t;

/**
 * @brief This enumeration defines the type of audio format
 */
typedef enum _dsAudioFormat_t {
    dsAUDIO_FORMAT_NONE,                ///< No audio format
    dsAUDIO_FORMAT_PCM,                 ///< Pulse-code Modulation audio format
    dsAUDIO_FORMAT_DOLBY_AC3,           ///< Audio Compression 3 audio format
    dsAUDIO_FORMAT_DOLBY_EAC3,          ///< Enhance Audio Compression 3 audio format
    dsAUDIO_FORMAT_DOLBY_AC4,           ///< Audio Compression 4 audio format
    dsAUDIO_FORMAT_DOLBY_MAT,           ///< Metadata-enhanced Audio Transmission audio format
    dsAUDIO_FORMAT_DOLBY_TRUEHD,        ///< Dolby TrueHD audio format
    dsAUDIO_FORMAT_DOLBY_EAC3_ATMOS,    ///< Audio Compression 3 Dolby Atmos audio format
    dsAUDIO_FORMAT_DOLBY_TRUEHD_ATMOS,  ///< Dolby TrueHD Dolby Atmos audio format
    dsAUDIO_FORMAT_DOLBY_MAT_ATMOS,     ///< Metadata-enhanced Audio Transmission Dolby Atmos audio format
    dsAUDIO_FORMAT_DOLBY_AC4_ATMOS,     ///< Audio Compression 4 Dolby Atmos audio format
    dsAUDIO_FORMAT_AAC,                 ///< Advanced Audio Coding
    dsAUDIO_FORMAT_VORBIS,              ///< Vorbis sound audio format
    dsAUDIO_FORMAT_WMA,                 ///< Windows Media audio format
    dsAUDIO_FORMAT_UNKNOWN,             ///< Unknown audio format
    dsAUDIO_FORMAT_MAX                  ///< Out of range 
}dsAudioFormat_t;

/**
 * @brief This enumeration defines the type of audio ducking action.
 */
typedef enum _dsAudioDuckingAction_t{
    dsAUDIO_DUCKINGACTION_START,    ///< Audio ducking start
    dsAUDIO_DUCKINGACTION_STOP,     ///< Audio ducking stop 
    dsAudio_DUCKINGACTION_MAX,      ///< Out of range 
} dsAudioDuckingAction_t;

/**
 * @brief This enumeration defines the type of audio ducking type.
 */
typedef enum _dsAudioDuckingType_t{
    dsAUDIO_DUCKINGTYPE_ABSOLUTE,   ///< Audio ducking absolute
    dsAUDIO_DUCKINGTYPE_RELATIVE,   ///< Audio ducking relative
    dsAudio_DUCKINGTYPE_MAX,        ///< Out of range 
} dsAudioDuckingType_t;

/**
 * @brief Enumeration defines all of the supported Audio types.
 * Each bit of uint32_t represent a standard. 
 * If a device supports multiple standards, the capability is the bitwise OR
* of the standards.
 */
typedef enum _dsAudioCapabilities_t {
    dsAUDIOSUPPORT_NONE = 0x0,      ///< None
    dsAUDIOSUPPORT_ATMOS = 0x01,    ///< Dolby Atmos
    dsAUDIOSUPPORT_DD = 0x02,       ///< Dolby Digitial
    dsAUDIOSUPPORT_DDPLUS = 0x04,   ///< Dolby Digital Plus
    dsAUDIOSUPPORT_DAD = 0x08,      ///< Digital Audio Delivery
    dsAUDIOSUPPORT_DAPv2 = 0x10,    ///< Digital Audio Processing version 2
    dsAUDIOSUPPORT_MS12 = 0x20,     ///< Multi Stream 12
    dsAUDIOSUPPORT_MS12V2 = 0x40,   ///< Multi Stream Version 2
    dsAUDIOSUPPORT_Invalid = 0x80,  ///< Invalid
} dsAudioCapabilities_t;

/**
 * @brief Enumeration defines all of the supported Arc types.
 * Each bit of uint32_t represent an ARC support type.
 * If a device multiple ARC types support, it's represented as bitwise OR
* of the types.
 */
typedef enum _dsAudioARCTypes_t {
    dsAUDIOARCSUPPORT_NONE = 0x0,   ///< None
    dsAUDIOARCSUPPORT_ARC = 0x01,   ///< Audio Return Channel
    dsAUDIOARCSUPPORT_eARC = 0x02,  ///< Enhanced Audio Return Channel
} dsAudioARCTypes_t;

/**
 * @brief Structure that holds Short Audio Descriptors retrieved from 
 * connected ARC device
 */
typedef struct _dsAudioSADList_t {
    int sad[MAX_SAD];   ///< Array of SADs
    int count;          ///< Amount of items in SAD array
} dsAudioSADList_t;

/**
 * @brief Structure that holds ARC status for the HDMI ARC/EARC port.
 */
typedef struct _dsAudioARCStatus_t {
   dsAudioARCTypes_t type;  ///< ARC type
   bool status;             ///< Enabled ARC status for the port
} dsAudioARCStatus_t;

/**
 * @brief Enumeration defines all of the supported MS12 types.
 * Each bit of uint32_t represent a standard. 
 * If a device supports multiple standards, the capability is the bitwise OR
 * of the standards.
 */
typedef enum _dsMS12Capabilities_t {
    dsMS12SUPPORT_NONE = 0x0,                   ///< MS12 Supported None
    dsMS12SUPPORT_DolbyVolume = 0x01,           ///< MS12 supported Dolby Volume
    dsMS12SUPPORT_InteligentEqualizer = 0x02,   ///< MS12 supported Intelligent Equalizer
    dsMS12SUPPORT_DialogueEnhancer = 0x04,      ///< MS12 Dialogue Enhancer supported
    dsMS12SUPPORT_Invalid = 0x80               ///< Invalid / Out of range
} dsMS12Capabilities_t;

/**
 * @brief Structure that captures MS12 Audio Profile list 
 */
#define MAX_PROFILE_LIST_BUFFER_LEN 1024
typedef struct _dsMS12AudioProfileList_t {
    char audioProfileList[MAX_PROFILE_LIST_BUFFER_LEN]; ///< buffer containing the list of comma separated audio profile names for menu list
                                                        ///< (Supported MS12 Audio Profiles: "Music,Movie,Sports,Entertainment,Night,Party,User")
    int audioProfileCount;                              ///< Total number of profiles
} dsMS12AudioProfileList_t;

/**
 * @brief This defines the type of volume leveller mode.
 */
typedef struct _dsVolumeLeveller_t {
    int mode;   ///< 0 = off, 1 = on, 2 = auto
    int level;  ///< Value for the volume leveller. From 0 to 10
} dsVolumeLeveller_t;

/**
 * @brief This defines the type of surround virtualizer mode.
 */
typedef struct _dsSurroundVirtualizer_t {
    int mode;   ///< 0 = off, 1 = on, 2 = auto
    int boost;  ///< Value for the boost level. From 0 to 96
} dsSurroundVirtualizer_t;

/**
 * @brief This enumeration defines the type of audio stereo mode.
 * @see dsAudioStereoMode_isValid
 */
typedef enum StereoMode{
    dsAUDIO_STEREO_UNKNOWN,   ///< Stereo mode none
    dsAUDIO_STEREO_MONO = 1,  ///< Mono mode
    dsAUDIO_STEREO_STEREO,    ///< Normal stereo mode (L+R)
    dsAUDIO_STEREO_SURROUND,  ///< Surround mode
    dsAUDIO_STEREO_PASSTHRU,  ///< Passthrough mode
    dsAUDIO_STEREO_DD,        ///< Dolby Digital
    dsAUDIO_STEREO_DDPLUS,    ///< Dolby Digital Plus
    dsAUDIO_STEREO_MAX        ///< Out of range          
} dsAudioStereoMode_t;

/** 
 * @brief This enumeration defines ATMOS protocol version types 
 */
typedef enum _dsATMOSCapability_t{
    dsAUDIO_ATMOS_NOTSUPPORTED= 0,  ///< ATMOS audio not supported
    dsAUDIO_ATMOS_DDPLUSSTREAM,     ///< can handle dd plus stream which is only way to pass ATMOS metadata
    dsAUDIO_ATMOS_ATMOSMETADATA,    ///< capable of parsing ATMOS metadata
    dsAUDIO_ATMOS_MAX,              ///< Out of range 
} dsATMOSCapability_t;

/**
 * @brief Structure that defines audio output device configuration.
 */
typedef struct _dsAudioTypeConfig_t {
    int32_t  typeId;                            ///< The audio output type
    const char *name;                           ///< Name of the audio output device
    size_t numSupportedCompressions;            ///< Number of supported audio compression methods
    const dsAudioCompression_t *compressions;   ///< List of audio compression methods supported
    size_t numSupportedEncodings;               ///< Number of supported audio encodings
    const dsAudioEncoding_t *encodings;         ///< List of audio encodings supported
    size_t numSupportedStereoModes;             ///< Number of supported stereo modes
    const dsAudioStereoMode_t *stereoModes;     ///< List of stereo modes supported
} dsAudioTypeConfig_t;

/**
 * @brief Enumeration defines surround mode.
 * Each bit of uint32_t represent supported surround mode. 
 */
typedef enum _dsSURROUNDMode_t {
    dsSURROUNDMODE_NONE = 0x0,      ///< No surround mode
    dsSURROUNDMODE_DD,        ///< Surround mode Dolby Digital
    dsSURROUNDMODE_DDPLUS,    ///< Surround mode Dolby Digital Plus
    dsSURROUNDMODE_MAX,             ///< Out of range
} dsSURROUNDMode_t;

/**
 * @brief Enumeration defines MS12 feature.
 */
typedef enum _dsMS12FEATURE_t {
    dsMS12FEATURE_DAPV2 = 0x0,  ///< Dolby Audio Processing Version 2
    dsMS12FEATURE_DE = 0x1,     ///< Dialogue Enhancement
    dsMS12FEATURE_MAX = 0x2,    ///< Out of range
} dsMS12FEATURE_t;

/**
 * @ingroup DSHAL_VIDEOPORT
 * @brief Structure that defines audio port configuration.
 */
typedef struct _dsAudioPortConfig_t {
    dsAudioPortId_t id;                         ///< Port ID
    const dsVideoPortPortId_t *connectedVOPs;   ///< Connected video port
} dsAudioPortConfig_t;

/* End of DSHAL_AUDIO_TYPES doxygen group */
/**
 * @}
 */

/** @addtogroup DSHAL_VIDEOPORT_TYPES Device Settings HAL VideoPort Type Definitions
 *  @ingroup DSHAL_VIDEOPORT
 *  @{
 */


/**
 * @brief This enumeration defines all of the standard video port resolutions.
 * @see dsVideoPortPixelResolution_isValid
 */
typedef enum _dsVideoResolution_t{
    dsVIDEO_PIXELRES_720x480,     ///< 720x480 Resolution
    dsVIDEO_PIXELRES_720x576,     ///< 720x576 Resolution
    dsVIDEO_PIXELRES_1280x720,    ///< 1280x720 Resolution
    dsVIDEO_PIXELRES_1366x768,    ///< 1366x768 Resolution
    dsVIDEO_PIXELRES_1920x1080,   ///< 1920x1080 Resolution
    dsVIDEO_PIXELRES_3840x2160,   ///< 3840x2160 Resolution
    dsVIDEO_PIXELRES_4096x2160,   ///< 3840x2160 Resolution
    dsVIDEO_PIXELRES_MAX          ///< Out of range 
}dsVideoResolution_t;

/**
 * @brief Video output resolutions type validation check.
 * @see dsVideoResolution_t
 */
#define dsVideoPortPixelResolution_isValid(t)  (((t) >= dsVIDEO_PIXELRES_720x480 ) && ((t) < dsVIDEO_PIXELRES_MAX))

/**
 * @brief This enumeration defines all of the standard TV 
 *      supported resolution with interlace information.
 */
typedef enum _dsTVResolution_t{
    dsTV_RESOLUTION_480i = 0x000001,     ///< 480i Resolution
    dsTV_RESOLUTION_480p = 0x000002,     ///< 480p Resolution
    dsTV_RESOLUTION_576i = 0x000004,     ///< 576i Resolution
    dsTV_RESOLUTION_576p = 0x000008,     ///< 576p Resolution
    dsTV_RESOLUTION_576p50 = 0x000010,   ///< 576p50 Resolution
    dsTV_RESOLUTION_720p = 0x000020,     ///< 720p Resolution
    dsTV_RESOLUTION_720p50 = 0x000040,   ///< 720p50 Resolution
    dsTV_RESOLUTION_1080i = 0x000080,    ///< 1080i Resolution
    dsTV_RESOLUTION_1080p = 0x000100,    ///< 1080p Resolution
    dsTV_RESOLUTION_1080p24 = 0x000200,  ///< 1080p24 Resolution
    dsTV_RESOLUTION_1080i25 = 0x000400,  ///< 1080i25 Resolution
    dsTV_RESOLUTION_1080p25 = 0x000800,  ///< 1080p25 Resolution
    dsTV_RESOLUTION_1080p30 = 0x001000,  ///< 1080p30 Resolution
    dsTV_RESOLUTION_1080i50 = 0x002000,  ///< 1080i50 Resolution
    dsTV_RESOLUTION_1080p50 = 0x004000,  ///< 1080p50 Resolution
    dsTV_RESOLUTION_1080p60 = 0x008000,  ///< 1080p60 Resolution
    dsTV_RESOLUTION_2160p24 = 0x010000,  ///< 2160p24 Resolution
    dsTV_RESOLUTION_2160p25 = 0x020000,  ///< 2160p25 Resolution
    dsTV_RESOLUTION_2160p30 = 0x040000,  ///< 2160p30 Resolution
    dsTV_RESOLUTION_2160p50 = 0x080000,  ///< 2160p50 Resolution
    dsTV_RESOLUTION_2160p60 = 0x100000,  ///< 2160p60 Resolution
}dsTVResolution_t;

/**
 * @brief This enumeration defines all of the standard frame rates at which 
 *      video may be played out of the video port.
 * @see dsVideoPortFrameRate_isValid
 * @note Also used in @link dsVideoDevice.h @endlink
 */
typedef enum _dsVideoFrameRate_t{
    dsVIDEO_FRAMERATE_UNKNOWN,  ///< Unknown frame rate
    dsVIDEO_FRAMERATE_24,       ///< Played at 24 frames per second
    dsVIDEO_FRAMERATE_25,       ///< Played at 25 frames per second
    dsVIDEO_FRAMERATE_30,       ///< Played at 30 frames per second
    dsVIDEO_FRAMERATE_60,       ///< Played at 60 frames per second
    dsVIDEO_FRAMERATE_23dot98,  ///< Played at 23.98 frames per second
    dsVIDEO_FRAMERATE_29dot97,  ///< Played at 29.97 frames per second
    dsVIDEO_FRAMERATE_50,       ///< Played at 50 frames per second
    dsVIDEO_FRAMERATE_59dot94,  ///< Played at 59.94 frames per second
    dsVIDEO_FRAMERATE_MAX       ///< Out of range
}dsVideoFrameRate_t;

/**
 * @brief Video output framerate validation check.
 * @see dsVideoFrameRate_t
 */
#define dsVideoPortFrameRate_isValid(t)  (((t) >= dsVIDEO_FRAMERATE_UNKNOWN ) && ((t) < dsVIDEO_FRAMERATE_MAX))

/**
 * @brief This enumeration defines all of the standard video port scan modes.
 * @see dsVideoPortScanMode_isValid
 */
typedef enum _dsVideoScanMode_t{
    dsVIDEO_SCANMODE_INTERLACED,    ///< Interlaced video
    dsVIDEO_SCANMODE_PROGRESSIVE,   ///< Progressive video
    dsVIDEO_SCANMODE_MAX            ///< Out of range
}dsVideoScanMode_t;

/**
 * @brief Video scan mode type validation check.
 * @see dsVideoScanMode_t
 */
#define dsVideoPortScanMode_isValid(t)  (((t) >= dsVIDEO_SCANMODE_INTERLACED ) && ((t) < dsVIDEO_SCANMODE_MAX))

/**
 * @brief This enumeration defines background color for video port.
 */
typedef enum _dsVideoBackgroundColor_t{
    dsVIDEO_BGCOLOR_BLUE,   ///< Background color BLUE
    dsVIDEO_BGCOLOR_BLACK,  ///< Background color BLACK
    dsVIDEO_BGCOLOR_NONE,   ///< Background color NONE
    dsVIDEO_BGCOLOR_MAX     ///< Out of range
}dsVideoBackgroundColor_t;

/**
 * @brief This enumeration defines all of the standard video Stereo Scopic modes.
 * @see dsVideoPortStereoScopicMode_isValid
 */
typedef enum _dsVideoStereoScopicMode_t {
    dsVIDEO_SSMODE_UNKNOWN = 0,         ///< Unknown mode
    dsVIDEO_SSMODE_2D,                  ///< 2D mode
    dsVIDEO_SSMODE_3D_SIDE_BY_SIDE,     ///< 3D side by side (L/R) stereo mode
    dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM,   ///< 3D top & bottom stereo mode
    dsVIDEO_SSMODE_MAX                  ///< Out of range
}dsVideoStereoScopicMode_t;

/**
 * @brief Video Stereo Scopic modes type validation check.
 * @see dsVideoStereoScopicMode_t
 */
#define dsVideoPortStereoScopicMode_isValid(t)  (((t) >= dsVIDEO_SSMODE_UNKNOWN ) && ((t) < dsVIDEO_SSMODE_MAX))

/**
 * @brief This enumeration defines all of the standard video aspect ratios.
 * @see dsVideoPortAspectRatio_isValid
 * @note also used in @link dsDisplay.h @endlink
 */
typedef enum _dsVideoAspectRatio_t{
    dsVIDEO_ASPECT_RATIO_4x3,    ///< 4:3 aspect ratio
    dsVIDEO_ASPECT_RATIO_16x9,   ///< 16:9 aspect ratio
    dsVIDEO_ASPECT_RATIO_MAX     ///< Out of range
}dsVideoAspectRatio_t;

/**
 * @brief Video aspect ratio type validation check.
 * @see dsVideoAspectRatio_t
 */
#define dsVideoPortAspectRatio_isValid(t)  (((t)  >= dsVIDEO_ASPECT_RATIO_4x3 ) && ((t) < dsVIDEO_ASPECT_RATIO_MAX))


/**
 * @brief Structure that defines video port resolution settings of output video device.
 * @note also used in @link dsHdmiIn.h @endlink
 */
typedef struct _dsVideoPortResolution_t {
    char name[32];                                  ///< Name the resolution
    dsVideoResolution_t  pixelResolution;           ///< The resolution associated with the name
    dsVideoAspectRatio_t  aspectRatio;              ///< The associated aspect ratio
    dsVideoStereoScopicMode_t  stereoScopicMode;    ///< The associated stereoscopic mode
    dsVideoFrameRate_t  frameRate;                  ///< The associated frame rate
    bool interlaced;                                ///< The associated scan mode( @a true if interlaced, @a false if progressive )
}dsVideoPortResolution_t;

/**
 * @brief Structure that defines the video output port configuration.
 */
typedef struct _dsVideoPortTypeConfig_t {
    dsVideoPortType_t typeId;                       ///< The video output type
    const char *name;                               ///< Name of the video output port
    bool dtcpSupported;                             ///< Is DTCP supported?                 
    bool hdcpSupported;                             ///< Is HDCP supported?                 
    int32_t restrictedResollution;                  ///< Any restricted resolution; -1 if no.
    size_t numSupportedResolutions;                 ///< Number of supported resolutions
    dsVideoPortResolution_t *supportedResolutions;  ///< List of supported resolutions
} dsVideoPortTypeConfig_t;

/**
 * @brief Max Key size.
 */
#define HDCP_KEY_MAX_SIZE  (4*1024)

/**
 * @brief Enumeration defines all of the standard HDR types.
 * Each bit of uint32_t represent a standard. 
 * If a device supports multiple standards, the capability is the bitwise OR
 * of the standards.
 * @note Also used in @link dsVideoDevice.h @endlink
 */
typedef enum _dsHDRStandard_t {
    dsHDRSTANDARD_NONE = 0x0,               ///< When No video format is decoded
    dsHDRSTANDARD_HDR10 = 0x01,             ///< Video Format HDR
    dsHDRSTANDARD_HLG = 0x02,               ///< Video Format HLG
    dsHDRSTANDARD_DolbyVision = 0x04,       ///< Video Format Dolby Vision
    dsHDRSTANDARD_TechnicolorPrime = 0x08,  ///< Video Format Techinicolor Prime
    dsHDRSTANDARD_HDR10PLUS = 0x10,         ///< Video Format HDRPlus 
    dsHDRSTANDARD_SDR= 0x20,                ///< Video Format SDR 
    dsHDRSTANDARD_Invalid = 0x80,           ///< When invalid value observed
} dsHDRStandard_t;

/**
 * @ingroup DSHAL_VIDEOPORT_TYPES
 * @brief Structure that defines video port configuration settings.
 */
typedef struct _dsVideoPortPortConfig_t {
    dsVideoPortPortId_t id;         ///< Port ID
    dsAudioPortId_t connectedAOP;   ///< Connected audio port
    const char *defaultResolution;  ///< Default resolution's name
} dsVideoPortPortConfig_t;

/** 
 * @brief This enumeration defines all HDCP Authentication Status 
 * @see dsHdcpStatus_isValid
*/
typedef enum _dsHdcpStatus_t {
    dsHDCP_STATUS_UNPOWERED = 0,            ///< Connected Sink Device does not support HDCP
    dsHDCP_STATUS_UNAUTHENTICATED,          ///< HDCP Authentication Process is not initiated
    dsHDCP_STATUS_AUTHENTICATED,            ///< HDCP Authentication Process is initiated and Passed
    dsHDCP_STATUS_AUTHENTICATIONFAILURE,    ///< HDCP Authentication Failure or Link Integroty Failure
    dsHDCP_STATUS_INPROGRESS,               ///< HDCP Authentication in Progress
    dsHDCP_STATUS_PORTDISABLED,             ///< HDMI output port disabled
    dsHDCP_STATUS_MAX                       ///< Out of range 
} dsHdcpStatus_t;

/**
 * @brief HDCP status validation check.
 * @see dsHdcpStatus_t
*/
#define dsHdcpStatus_isValid(t)  (((t) >= dsHDCP_STATUS_UNPOWERED ) && ((t) < dsHDCP_STATUS_MAX))

/**
 * @brief This enumeration defines HDCP protocol version types 
 */
typedef enum _dsHdcpProtocolVersion_t {
    dsHDCP_VERSION_1X = 0,  ///< HDCP Protocol version 1.x 
    dsHDCP_VERSION_2X,      ///< HDCP Protocol version 2.x 
    dsHDCP_VERSION_MAX      ///< Out of range 
} dsHdcpProtocolVersion_t;

/* End of DSHAL_VIDEOPORT_TYPES doxygen group. */
/**
 * @}
 */

/** @addtogroup DSHAL_DISPLAY_TYPES Device Settings HAL Display Type Definitions
 *  @ingroup DSHAL_DISPLAY
 *  @{
 */

/**
 * @brief Maximum Value  video modes are described in CEA specifictaion
 */
#define dsEEDID_MAX_VIDEO_CODE  64  ///< Maximum Value EEDID Video Code

/**
 * @brief Maximum length for for Connected Display Monitor names.
*/
#define dsEEDID_MAX_MON_NAME_LENGTH 14  ///< Maximum Length of Connected Display Monitor Name

/**
 * @brief Maximum length for EDID data.
*/
#define MAX_EDID_BYTES_LEN  (1024)  ///< Maximum Byte length of EDID data

/**
 * @brief Structure for the EDID display
*/
typedef struct _dsDisplayEDID_t {
    int32_t productCode;               ///< Product code of the display device
    int32_t serialNumber;              ///< Serial number of the display device
    int32_t manufactureYear;           ///< Year of manufacture of the display device
    int32_t manufactureWeek;           ///< Week of manufacture of the display device
    bool hdmiDeviceType;               ///< Device type ( @a true if HDMI, @a false if DVI )
    bool  isRepeater;                  ///<  Receiver is a repeater  ( @a true if Repeater, @a false if connected Receiver is not a repeater)
    uint8_t physicalAddressA;          ///<  Physical Address for HDMI node A
    uint8_t physicalAddressB;          ///<  Physical Address for HDMI node B
    uint8_t physicalAddressC;          ///<  Physical Address for HDMI node C
    uint8_t physicalAddressD;          ///<  Physical Address for HDMI node D
    int32_t numOfSupportedResolution;  ///<  Number of Supported Resolution
    dsVideoPortResolution_t suppResolutionList [dsEEDID_MAX_VIDEO_CODE * dsVIDEO_SSMODE_MAX];   ///<  EDID Supported Resoultion List
    char monitorName[dsEEDID_MAX_MON_NAME_LENGTH];  ///<  Connected Display Monitor Name
} dsDisplayEDID_t;

/**
 * @brief This enumeration defines the types of powerstate
 */
typedef enum _dsPowerState_t{
    dsPOWER_ON =1,      ///< Host Power-on state
    dsPOWER_STANDBY,    ///< Host Standby state
    dsPOWER_OFF,        ///< Host Power-off state
    dsPOWER_MAX,        ///< Out of range
}dsPowerState_t;

/**
 * @brief This enumeration defines the type of display color spaces supported
 * @note Used in @link dsVideoPort.h @endlink
 */
typedef enum _dsDisplayColorSpace_t
{
    dsDISPLAY_COLORSPACE_UNKNOWN = 0,   ///< Unknown color space
    dsDISPLAY_COLORSPACE_RGB = 1,       ///< RGB color space
    dsDISPLAY_COLORSPACE_YCbCr422 = 2,  ///< YCbCr4.2.2 color space
    dsDISPLAY_COLORSPACE_YCbCr444 = 3,  ///< YCbCr4.4.4 color space
    dsDISPLAY_COLORSPACE_YCbCr420 = 4,  ///< YCbCr4.2.0 color space
    dsDISPLAY_COLORSPACE_AUTO = 5,      ///< Automatic color space
    dsDISPLAY_COLORSPACE_MAX            ///< Out of range 
} dsDisplayColorSpace_t;

/**
 * @brief This enumeration defines the type of display quantization ranges
 * @note Used in @link dsVideoPort.h @endlink
 */
typedef enum _dsDisplayQuantizationRange_t
{
    dsDISPLAY_QUANTIZATIONRANGE_UNKNOWN = 0,    ///< Unknown quantization range
    dsDISPLAY_QUANTIZATIONRANGE_LIMITED = 1,    ///< Limited quantization range
    dsDISPLAY_QUANTIZATIONRANGE_FULL = 2,       ///< Full quantization range
    dsDISPLAY_QUANTIZATIONRANGE_MAX             ///< Out of range 
} dsDisplayQuantizationRange_t;

/**
 * @brief This enumeration defines the type of display Color depth.
 * @note Used in @link dsVideoPort.h @endlink
 */
typedef enum _dsDisplayColorDepth_t
{
    dsDISPLAY_COLORDEPTH_UNKNOWN = 0x0, ///< Unknown color depth
    dsDISPLAY_COLORDEPTH_8BIT = 0x01,   ///< 8 bit color depth
    dsDISPLAY_COLORDEPTH_10BIT = 0x02,  ///< 10 bit color depth
    dsDISPLAY_COLORDEPTH_12BIT = 0x04,  ///< 12 bit color depth
    dsDISPLAY_COLORDEPTH_AUTO = 0x08,   ///< Automatic color depth
} dsDisplayColorDepth_t;

/**
 * @brief This enumeration defines the type of Matrix coefficients.
 * @note Used in @link dsVideoPort.h @endlink
 */
typedef enum _dsDisplayMatrixCoefficients_t
{
    dsDISPLAY_MATRIXCOEFFICIENT_UNKNOWN = 0,   ///< Unknown Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_BT_709,        ///< ITU BT 709 Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_BT_470_2_BG,   ///< ITU BT 470_2_BG Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_SMPTE_170M ,   ///< SMPTE 170M Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_XvYCC_709,     ///< XvYCC_709 Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_eXvYCC_601,    ///< XvYCC_601 Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_BT_2020_NCL,   ///< ITU BT 2020 non constant luminance Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_BT_2020_CL,    ///< ITU BT 2020 constant luminance Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_eDVI_FR_RGB,   ///< eDVI Full Range RGB Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_eHDMI_RGB,     ///< eHDMI RGB Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_eFCC,          ///< eFCC Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_eSMPTE_240M,   ///< eSMPTE 240M Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_eHDMI_FR_YCbCr, ///< eHDMI Full Range YcbCr Matrix Coefficient
    dsDISPLAY_MATRIXCOEFFICIENT_MAX            ///< Out of range
} dsDisplayMatrixCoefficients_t;

/**
 * @brief This enumeration defines the type of Audio Input to Audio mixer.
 * @note Used in @link dsAudio.h @endlink
 */
typedef enum _dsAudioInput_t
{
    dsAUDIO_INPUT_PRIMARY = 0,   ///< Primary Audio Input is main audio input provided to audio mixer
    dsAUDIO_INPUT_SYSTEM,        ///< System Audio Input to audio mixer , e.g Beep Sounds
    dsAUDIO_INPUT_MAX            ///< Out of range
} dsAudioInput_t;

/* End of DSHAL_DISPLAY_TYPES doxygen group */
/**
 * @}
 */

#endif /* __DS_AVD_TYPES_H__ */

/** @} */ // End of DSHAL_AVD_TYPES
/** @} */ // End of Device Settings HAL
/** @} */ // End of Device Settings Module
/** @} */ // End of HPK
