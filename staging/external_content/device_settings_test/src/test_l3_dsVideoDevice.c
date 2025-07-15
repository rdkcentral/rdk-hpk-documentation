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
 * @addtogroup HPK Hardware deviceing Kit
 * @{
 * @par The Hardware deviceing Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their devices before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware deviceing Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @addtogroup Device_Settings Device Settings Module
 * @{
 */

/**
 * @addtogroup Device_Settings_HALTEST Device Settings HAL Tests
 * @{
 */

/**
 * @defgroup DS_VideoDevice_HALTEST Device Settings Video Device HAL Tests
 * @{
 */

/**
 * @defgroup DS_VideoDevice_HALTEST_L3 Device Settings Video Device HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for DS Video Device HAL :
 *
 * Level 3 test cases for all APIs of Device Settings Video Device HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-video-device_halSpec.md](../../docs/pages/ds-video-device_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l3_dsVideoDevice.c
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include <ut_kvp.h>

#include "dsVideoDevice.h"


#define DS_ASSERT(actual,expected) assert(actual==expected)
#define UT_LOG_MENU_INFO UT_LOG_INFO
#define DS_FRAMERATE_KEY_SIZE 50

intptr_t gdeviceHandle = 0;

/* currently it support only one video deice as per HAL spec*/
int num_of_devices = 1; 

/* Global Variables */
static int32_t gTestGroup = 3;
static int32_t gTestID    = 1;


// Mapping table for dsError_t
const static ut_control_keyStringMapping_t  dsErrorMappingTable[] = {
    {"dsERR_NONE", (int32_t)dsERR_NONE},
    {"dsERR_GENERAL", (int32_t)dsERR_GENERAL},
    {"dsERR_INVALID_PARAM", (int32_t)dsERR_INVALID_PARAM},
    {"dsERR_INVALID_STATE", (int32_t)dsERR_INVALID_STATE},
    {"dsERR_ALREADY_INITIALIZED", (int32_t)dsERR_ALREADY_INITIALIZED},
    {"dsERR_NOT_INITIALIZED", (int32_t)dsERR_NOT_INITIALIZED},
    {"dsERR_OPERATION_NOT_SUPPORTED", (int32_t)dsERR_OPERATION_NOT_SUPPORTED},
    {"dsERR_RESOURCE_NOT_AVAILABLE", (int32_t)dsERR_RESOURCE_NOT_AVAILABLE},
    {"dsERR_OPERATION_FAILED", (int32_t)dsERR_OPERATION_FAILED},
    {"dsErr_MAX", (int32_t)dsErr_MAX},
    {NULL, -1}
};

// Mapping table for dsVideoZoom_t
const static ut_control_keyStringMapping_t dsVideoZoomMappingTable[] = {
    {"dsVIDEO_ZOOM_UNKNOWN", (int32_t)dsVIDEO_ZOOM_UNKNOWN},
    {"dsVIDEO_ZOOM_NONE", (int32_t)dsVIDEO_ZOOM_NONE},
    {"dsVIDEO_ZOOM_FULL", (int32_t)dsVIDEO_ZOOM_FULL},
    {"dsVIDEO_ZOOM_LB_16_9", (int32_t)dsVIDEO_ZOOM_LB_16_9},
    {"dsVIDEO_ZOOM_LB_14_9", (int32_t)dsVIDEO_ZOOM_LB_14_9},
    {"dsVIDEO_ZOOM_CCO", (int32_t)dsVIDEO_ZOOM_CCO},
    {"dsVIDEO_ZOOM_PAN_SCAN", (int32_t)dsVIDEO_ZOOM_PAN_SCAN},
    {"dsVIDEO_ZOOM_LB_2_21_1_ON_4_3", (int32_t)dsVIDEO_ZOOM_LB_2_21_1_ON_4_3},
    {"dsVIDEO_ZOOM_LB_2_21_1_ON_16_9", (int32_t)dsVIDEO_ZOOM_LB_2_21_1_ON_16_9},
    {"dsVIDEO_ZOOM_PLATFORM", (int32_t)dsVIDEO_ZOOM_PLATFORM},
    {"dsVIDEO_ZOOM_16_9_ZOOM", (int32_t)dsVIDEO_ZOOM_16_9_ZOOM},
    {"dsVIDEO_ZOOM_PILLARBOX_4_3", (int32_t)dsVIDEO_ZOOM_PILLARBOX_4_3},
    {"dsVIDEO_ZOOM_WIDE_4_3", (int32_t)dsVIDEO_ZOOM_WIDE_4_3},
    {"dsVIDEO_ZOOM_MAX", (int32_t)dsVIDEO_ZOOM_MAX},
    {NULL, -1}
};

// Mapping table for dsVideoCodingFormat_t
const static ut_control_keyStringMapping_t dsVideoCodingFormatMappingTable[] = {
    {"dsVIDEO_CODEC_MPEGHPART2", (int32_t)dsVIDEO_CODEC_MPEGHPART2},
    {"dsVIDEO_CODEC_MPEG4PART10", (int32_t)dsVIDEO_CODEC_MPEG4PART10},
    {"dsVIDEO_CODEC_MPEG2", (int32_t)dsVIDEO_CODEC_MPEG2},
    {"dsVIDEO_CODEC_MAX", (int32_t)dsVIDEO_CODEC_MAX},
    {NULL, -1}
};

// Mapping table for dsVideoCodecHevcProfiles_t
const static ut_control_keyStringMapping_t dsVideoCodecHevcProfilesMappingTable[] = {
    {"dsVIDEO_CODEC_HEVC_PROFILE_MAIN", (int32_t)dsVIDEO_CODEC_HEVC_PROFILE_MAIN},
    {"dsVIDEO_CODEC_HEVC_PROFILE_MAIN10", (int32_t)dsVIDEO_CODEC_HEVC_PROFILE_MAIN10},
    {"dsVIDEO_CODEC_HEVC_PROFILE_MAINSTILLPICTURE", (int32_t)dsVIDEO_CODEC_HEVC_PROFILE_MAINSTILLPICTURE},
    {"dsVIDEO_CODEC_HEVC_MAX", (int32_t)dsVIDEO_CODEC_HEVC_MAX},
    {NULL, -1}
};


/**
 * @brief This function clears the stdin buffer.
 *
 * This function clears the stdin buffer.
 */
static void readAndDiscardRestOfLine(FILE* in)
{
    int c;
    while ( (c = fgetc(in)) != EOF && c != '\n');
}

/*FrameratePreChange callback*/
void dsVideoDevice_FrameratePreChange(unsigned int tSecond)
{
    UT_LOG_INFO("FrameratePreChange callback tSecond: %d",tSecond);
}

/*FrameratePostChange callback*/
void dsVideoDevice_FrameratePostChange(unsigned int tSecond)
{
    UT_LOG_INFO("FrameratePostChange callback tSecond: %d",tSecond);
}

static void dsVideoDevice_getHandle()
{
    dsError_t status   = dsERR_NONE;
    int32_t device = 0, selectedVideoDevice = 0;
    UT_LOG_INFO("In %s [%02d]", __FUNCTION__, gTestGroup);

    UT_LOG_MENU_INFO(" Supported Video Device:");
    for (device = 0; device < num_of_devices; device++)
    {
        UT_LOG_MENU_INFO("\t%d. VideoDevice%d", device, device);
    }
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO(" Select the Video Device:");
    scanf("%d", &selectedVideoDevice);
    readAndDiscardRestOfLine(stdin);
    
    if(selectedVideoDevice < 0 || selectedVideoDevice >= 1) 
    {
       UT_LOG_ERROR("\nInvalid option selected\n");
       goto exit;
    }

    UT_LOG_INFO("Calling dsGetVideoDevice(IN:index:[%d], OUT:Handle[])", selectedVideoDevice);
    status = dsGetVideoDevice(selectedVideoDevice, &gdeviceHandle);
    UT_LOG_INFO("Result dsGetVideodevice(IN:index:[0x%0d], OUT:Handle[0x%0X]) dsError_t=[%s]",selectedVideoDevice, \
                                    gdeviceHandle, UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Initialization of the Device settings - Video Device Module
*
* This test provides a scope to Intialize the Video Device module.
* **Test Group ID:** 03@n
* **Test Case ID:** 001@n
*
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select VideoDevice_Init before running any test.
*
*/
void test_l3_dsVideoDevice_Init()
{
    gTestID = 1;
    dsError_t status   = dsERR_NONE;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int32_t choice = -1;

    UT_LOG_MENU_INFO("Select Device Type[0: Sink, 1: Source]: ");
    scanf("%d", &choice);
    readAndDiscardRestOfLine(stdin);

    if(choice < 0 || choice > 1)
    {
        UT_LOG_ERROR("Invalid device Type");
        goto exit;
    }

    UT_LOG_INFO("Calling dsVideoDeviceInit()");
    status = dsVideoDeviceInit();
    UT_LOG_INFO("Result dsVideoDeviceInit(), dsError_t=[%s]", UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);

    if(choice == 0)
    {
        UT_LOG_INFO("Calling dsRegisterFrameratePreChangeCB()");
        status = dsRegisterFrameratePreChangeCB(dsVideoDevice_FrameratePreChange);
        UT_LOG_INFO("Result dsRegisterFrameratePreChangeCB(), dsError_t=[%s]", UT_Control_GetMapString(dsErrorMappingTable, status));
        DS_ASSERT(status, dsERR_NONE);
        UT_LOG_INFO("Calling dsRegisterFrameratePostChangeCB()");
        status = dsRegisterFrameratePostChangeCB(dsVideoDevice_FrameratePostChange);
        UT_LOG_INFO("Result dsRegisterFrameratePostChangeCB(), dsError_t=[%s]", UT_Control_GetMapString(dsErrorMappingTable, status));
        DS_ASSERT(status, dsERR_NONE);
    }
 

    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Termination of the Device settings - Video Device Module
*
* This test provides a scope to Terminate the Video Device module.
* **Test Group ID:** 03@n
* **Test Case ID:** 011@n
* **Pre-Conditions:** None@n
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select VideoDevice_Term at end of every test.
*
*/
void test_l3_dsVideoDevice_Term()
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;

    UT_LOG_INFO("Calling dsVideoDevice_Term()");
    status = dsVideoDeviceTerm();
    UT_LOG_INFO("Result dsVideoDeviceTerm(), dsError_t=[%s]",  UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Set Zoom mode of Source device
*
* This test sets zoom mode of video device seected.
* **Test Group ID:** 03@n
* **Test Case ID:** 002@n
* **Pre-Conditions:** None@n
* VideoDevice_init Should be called before calling this test
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select SetZoomMode from list
*
*/

void test_l3_dsVideoDevice_SetZoomMode()
{
    dsError_t status   = dsERR_NONE;
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    int32_t choice = 0;
    int32_t j=0;

    dsVideoDevice_getHandle();
    UT_LOG_MENU_INFO(" \t  Supported Zoom Modes are:");
    for (j = dsVIDEO_ZOOM_NONE; j < dsVIDEO_ZOOM_MAX; j++)
    {
        UT_LOG_MENU_INFO("\t%d. %-20s", j, \
                    UT_Control_GetMapString(dsVideoZoomMappingTable, j));
    }

    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO(" Select the Zoom mode:");
    scanf("%d", &choice);
    readAndDiscardRestOfLine(stdin);

    if(choice < 0 || choice >= dsVIDEO_ZOOM_MAX) 
    {
       UT_LOG_ERROR("\nInvalid Zoom mode selected\n");
       goto exit;
    }

    UT_LOG_INFO("Calling dsSetDFC(IN:Handle:[0x%0X],IN:dsVideoZoom_t:[%s])",gdeviceHandle, \
                    UT_Control_GetMapString(dsVideoZoomMappingTable, choice));
    status = dsSetDFC(gdeviceHandle, choice);
    UT_LOG_INFO("Result dsSetDFC(IN:Handle:[0x%0X],IN:dsVideoZoom_t:[%s]), dsError_t=[%s]", gdeviceHandle, \
                    UT_Control_GetMapString(dsVideoZoomMappingTable, choice), \
                    UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    
    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Set Device Frame Rate of Sink device
*
* This test sets frame rate of video device selected.
* **Test Group ID:** 03@n
* **Test Case ID:** 003@n
* **Pre-Conditions:** None@n
* VideoDevice_init Should be called before calling this test
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select SetDisplayFrameRate from list
*
*/
void test_l3_dsVideoDevice_SetDisplayFramerate()
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    char frameRate[DS_FRAMERATE_KEY_SIZE] = {0};

    dsVideoDevice_getHandle();
    
    UT_LOG_MENU_INFO(" Provide Display Framerate :");
    scanf("%s", frameRate);
    readAndDiscardRestOfLine(stdin);

    UT_LOG_INFO("Calling dsSetDisplayframerate(IN:Handle:[0x%0X],IN:framerate:[%s])",gdeviceHandle, \
                        frameRate);

    status = dsSetDisplayframerate(gdeviceHandle, frameRate);
    UT_LOG_INFO("Result dsSetDisplayframerate(IN:Handle:[0x%0X],IN:framerate:[%s]), dsError_t=[%s]",gdeviceHandle, \
                        frameRate, UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Set Frame Rate Mode of Sink device
*
* This test sets Frame rate mode of video device selected.
* **Test Group ID:** 03@n
* **Test Case ID:** 004@n
* **Pre-Conditions:** None@n
* VideoDevice_init Should be called before calling this test
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select SetFRFMode from list
*
*/
void test_l3_dsVideoDevice_SetFRFMode()
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    int32_t choice = 0;
    int32_t j = 0;

    dsVideoDevice_getHandle();
    UT_LOG_MENU_INFO(" \t  Supported Display FRF Mode are:");
    for (j = 0; j < 2; j++)
    {
        UT_LOG_MENU_INFO("\t%d. %-20s", j,((j==0)?"Disable":"Enable"));
    }

    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO(" Select the Display FRF Mode Sample :");
    scanf("%d", &choice);
    readAndDiscardRestOfLine(stdin);

    if(choice < 0 || choice > 1) 
    {
       UT_LOG_ERROR("\nInvalid Display FRF mode selected\n");
       goto exit;
    }

    UT_LOG_INFO("Calling dsSetFRFMode(IN:Handle:[0x%0X],IN:framerate:[%s])",gdeviceHandle, \
                                                                ((choice==0)?"Disable":"Enable"));
    status = dsSetFRFMode(gdeviceHandle, choice);
    UT_LOG_INFO("Result dsSetFRFMode(IN:Handle:[0x%0X],IN:framerate:[%s]) dsError_t=[%s]",gdeviceHandle, \
                                                                ((choice==0)?"Disable":"Enable"), UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);

    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Get Video Codec info from source device
*
* This test gets Video Codec Info of video device seected.
* **Test Group ID:** 03@n
* **Test Case ID:** 005@n
* **Pre-Conditions:** None@n
* VideoDevice_init Should be called before calling this test
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select GetVideoCodecInfo from list
*
*/
void test_l3_dsVideoDevice_GetVideoCodecInfo()
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    dsVideoCodecInfo_t codecInfo;
    int choice = 0;
    int32_t index = 1;
    int32_t j = 0;

    dsVideoDevice_getHandle();
    UT_LOG_MENU_INFO(" \t  Supported Video Codec Info:");
    for (dsVideoCodingFormat_t codec = dsVIDEO_CODEC_MPEGHPART2; codec < dsVIDEO_CODEC_MAX;codec <<= 1)
    { 
            UT_LOG_MENU_INFO("\t%d. %-20s", index, UT_Control_GetMapString(dsVideoCodingFormatMappingTable, codec));
            index++;
    }
    UT_LOG_MENU_INFO("------------------------------------------");
    UT_LOG_MENU_INFO(" Select the Codec for Info :");
    scanf("%d", &choice);
    readAndDiscardRestOfLine(stdin);

    dsVideoCodingFormat_t selectedCodec = (dsVideoCodingFormat_t)(1 << (choice - 1));

    if(choice <=0 || selectedCodec >= dsVIDEO_CODEC_MAX) 
    {
       UT_LOG_ERROR("\nInvalid Codec selected\n");
       goto exit;
    }

    if(choice > 2)
    {
        choice = 1 << (choice-1);
    }

    UT_LOG_INFO("Calling dsGetVideoCodecInfo(IN:Handle:[0x%0X],IN:Codec[%s], OUT:CodecInfo[]))",gdeviceHandle,\
                                    UT_Control_GetMapString(dsVideoCodingFormatMappingTable, choice));
    
    status = dsGetVideoCodecInfo(gdeviceHandle, (dsVideoCodingFormat_t)choice, &codecInfo);
    UT_LOG_INFO("Result dsGetVideoCodecInfo(IN:Handle:[0x%0X],IN:Codec[%s], OUT:Codec number of Entires[%d]), dsError_t=[%s] ",gdeviceHandle,\
                                    UT_Control_GetMapString(dsVideoCodingFormatMappingTable, choice), \
            codecInfo.num_entries, UT_Control_GetMapString(dsErrorMappingTable, status));


    for(j = 0; j< codecInfo.num_entries; j++)
    {
            UT_LOG_INFO("OUT:CodecProfile[%-20s]",UT_Control_GetMapString(dsVideoCodecHevcProfilesMappingTable, \
                                               (int32_t)codecInfo.entries[j].profile));
    }
    DS_ASSERT(status, dsERR_NONE);
    
    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Get Supported Video Coding formats
*
* This test gets supported video coding formats of the device
* **Test Group ID:** 03@n
* **Test Case ID:** 006@n
* **Pre-Conditions:** None@n
* VideoDevice_init Should be called before calling this test
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select GetSupportedVideoCodingFormat from list
*
*/
void test_l3_dsVideoDevice_GetSupportedVideoCodingFormat()
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    uint32_t supportedFormat = 0;

    dsVideoDevice_getHandle();
    UT_LOG_INFO("Calling dsGetSupportedVideoCodingFormats(IN:Handle[0x%0X],OUT:supportedFormat[])",gdeviceHandle);
    status = dsGetSupportedVideoCodingFormats(gdeviceHandle, &supportedFormat);
    UT_LOG_INFO("Result dsGetSupportedVideoCodingFormats(IN:Handle[0x%0X],OUT:supportedFormat[0x08%x]), dsError_t=[%s]",gdeviceHandle, \
                                                supportedFormat,UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief Get Supported HDR capabilities
*
* This test gets supported HDR capabilities of the device
* **Test Group ID:** 03@n
* **Test Case ID:** 007@n
* **Pre-Conditions:** None@n
* VideoDevice_init Should be called before calling this test
*
* **Dependencies:** None@n
*
* **User Interaction:** @n
* User or Automation tool should select GetHDRCapabilities from list
*
*/
void test_l3_dsVideoDevice_GetHDRCapabilities()
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status = dsERR_NONE;
    int32_t HDRCapabilities = 0;

    dsVideoDevice_getHandle();
    UT_LOG_INFO("Calling dsGetHDRCapabilities(IN:Handle[0x%0X],OUT:HDRCapabilities[])",gdeviceHandle);
    status = dsGetHDRCapabilities(gdeviceHandle, &HDRCapabilities);
    UT_LOG_INFO("Result dsGetHDRCapabilities(IN:Handle[0x%0X],OUT:HDRCapabilites[0x08%x]), dsError_t=[%s] ",gdeviceHandle, \
                                               HDRCapabilities, UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Get Current Frame Rate Mode
 *
 * This test gets Current Frame Rate mode of the device
 * **Test Group ID:** 03@n
 * **Test Case ID:** 008@n
 * **Pre-Conditions:** None@n
 * VideoDevice_init Should be called before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should selected GetFRFMode from list
 *
 */
void test_l3_dsVideoDevice_dsGetFRFMode()
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    int32_t frfMode = 0;

    dsVideoDevice_getHandle();
    UT_LOG_INFO("Calling dsGetFRFMode(IN:Handle[0x%0X],OUT:frfMode[])",gdeviceHandle);
    status = dsGetFRFMode(gdeviceHandle, &frfMode);
    UT_LOG_INFO("Result dsGetFRFMode(IN:Handle[0x%0X],OUT:frfMode[%d]), dsError_t=[%s] ",gdeviceHandle, \
                                                frfMode,UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Get Current Frame Rate 
 *
 * This test gets Current Frame Rate of the device
 * **Test Group ID:** 03@n
 * **Test Case ID:** 009@n
 * **Pre-Conditions:** None@n
 * VideoDevice_init Should be called before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should selected GetCurrentDisplayframerate from list
 *
 */
void test_l3_dsVideoDevice_dsGetCurrentDisplayframerate()
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    char currentFrameRate[50] = {0};

    dsVideoDevice_getHandle();
    UT_LOG_INFO("Calling dsGetCurrentDisplayframerate(IN:Handle[0x%0X],OUT:currentFrameRate[])",gdeviceHandle);
    status = dsGetCurrentDisplayframerate(gdeviceHandle, currentFrameRate);
    UT_LOG_INFO("Result dsGetCurrentDisplayframerate(IN:Handle[0x%0X],OUT:currentFrameRate[%s]), dsError_t=[%s]",gdeviceHandle, \
                                                currentFrameRate, UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Get Current Zoom Mode 
 *
 * This test gets Current Zoom Mode of the device
 * **Test Group ID:** 03@n
 * **Test Case ID:** 010@n
 * **Pre-Conditions:** None@n
 * VideoDevice_init Should be called before calling this test
 *
 * **Dependencies:** None@n
 *
 * **User Interaction:** @n
 * User or Automation tool should selected GetZoomMode from list
 *
 */
void test_l3_dsVideoDevice_dsGetZoomMode()
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);
    dsError_t status   = dsERR_NONE;
    dsVideoZoom_t dfc;

    dsVideoDevice_getHandle();
    UT_LOG_INFO("Calling dsGetDFC(IN:Handle[0x%0X],OUT:CurrentZoomMode[])",gdeviceHandle);
    status = dsGetDFC(gdeviceHandle, &dfc);
    UT_LOG_INFO("Result dsGetDFC((IN:Handle[0x%0X],OUT:CurrentZoomMode[%s]), dsError_t=[%s]",gdeviceHandle, \
                                                UT_Control_GetMapString(dsVideoZoomMappingTable,(int32_t) dfc), \
                                                UT_Control_GetMapString(dsErrorMappingTable, status));
    DS_ASSERT(status, dsERR_NONE);
    UT_LOG_INFO("Out %s", __FUNCTION__);
}
static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l3_dsVideoDevice_register(void)
{
    pSuite = UT_add_suite_withGroupID("[L3 dsVideoDevice]", NULL, NULL,UT_TESTS_L3);
    if (pSuite == NULL)
    {
        UT_LOG_ERROR("Failed to create the test suite");
        return -1;
    }

    UT_add_test( pSuite, "VideoDevice_Init", test_l3_dsVideoDevice_Init);
    UT_add_test( pSuite, "VideoDevice_Term", test_l3_dsVideoDevice_Term);
    UT_add_test( pSuite, "GetHDRCapabilities", test_l3_dsVideoDevice_GetHDRCapabilities);
    UT_add_test( pSuite, "GetSupportedVideoCodingFormat", test_l3_dsVideoDevice_GetSupportedVideoCodingFormat);
    UT_add_test( pSuite, "SetDisplayFramerate", test_l3_dsVideoDevice_SetDisplayFramerate);
    UT_add_test( pSuite, "GetCurrentDisplayframerate", test_l3_dsVideoDevice_dsGetCurrentDisplayframerate);
    UT_add_test( pSuite, "SetFRFMode", test_l3_dsVideoDevice_SetFRFMode);
    UT_add_test( pSuite, "dsGetFRFMode", test_l3_dsVideoDevice_dsGetFRFMode);
    UT_add_test( pSuite, "SetZoomMode", test_l3_dsVideoDevice_SetZoomMode);
    UT_add_test( pSuite, "GetZoomMode", test_l3_dsVideoDevice_dsGetZoomMode);
    UT_add_test( pSuite, "GetVideoCodecInfo", test_l3_dsVideoDevice_GetVideoCodecInfo);

    return 0;
}

/** @} */ // End of DS_VideoDevice_HALTEST_L3
/** @} */ // End of DS_VideoDevice_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
