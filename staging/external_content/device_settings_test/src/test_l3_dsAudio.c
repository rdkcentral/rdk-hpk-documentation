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
 * @addtogroup Device_Settings_HALTEST Device Settings HAL Tests
 * @{
 */

/**
 * @addtogroup DS_Audio_HALTEST Device Settings Audio HAL Tests
 * @{
 */

/**
 * @defgroup DS_Audio_HALTEST_L3 Device Settings Audio HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for DS Audio HAL :
 *
 * Level 3 unit test cases for all APIs of Device Settings Audio HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-audio_halSpec.md](../../docs/pages/ds-audio_halSpec.md)
 *
 * @endparblock
 */


/**
 * @file test_l3_dsAudio.c
 *
 */
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "dsAudio.h"

#define DS_AUDIO_MAX_PORTS 20
#define DS_AUDIO_MAX_INDEX 10
#define DS_AUDIO_MAX_DAP 20
#define DS_AUDIO_MAX_MS12_PROFILES 10
#define DS_AUDIO_MAX_MS12_LENGTH 32
#define DS_MAX_FILE_SIZE 64

#define UT_LOG_MENU_INFO UT_LOG_INFO

#define DS_ASSERT assert

#define DS_CONNECTION_CB_FILE "dsAudio_connection_callback.txt"
#define DS_FORMAT_CB_FILE "dsAudio_format_callback.txt"
#define DS_ATMOS_CB_FILE "dsAudio_atmos_callback.txt"

/* Global Variables */
static int32_t gTestGroup = 3;
static int32_t gTestID = 1;
static bool gConnectionStatus = false;
static bool gAtmosStatus = false;

static dsAudioFormat_t gAudioFormat = dsAUDIO_FORMAT_NONE;
static dsATMOSCapability_t gAtosCapablity = dsAUDIO_ATMOS_NOTSUPPORTED;
static dsAudioARCStatus_t gARCStatus = {0};
static pthread_mutex_t gCallbackMutex = PTHREAD_MUTEX_INITIALIZER;

static char gConnectionCBFile[DS_MAX_FILE_SIZE];
static char gFormatCBFile[DS_MAX_FILE_SIZE];
static char gATMOSCapsCBFile[DS_MAX_FILE_SIZE];

/* Enum mapping tables */

/* dsAudioPortType_t */
const static ut_control_keyStringMapping_t dsAudioPortType_mapTable[] =
{
    {"dsAUDIOPORT_TYPE_ID_LR", (int32_t)dsAUDIOPORT_TYPE_ID_LR},
    {"dsAUDIOPORT_TYPE_HDMI", (int32_t)dsAUDIOPORT_TYPE_HDMI},
    {"dsAUDIOPORT_TYPE_SPDIF", (int32_t)dsAUDIOPORT_TYPE_SPDIF},
    {"dsAUDIOPORT_TYPE_SPEAKER", (int32_t)dsAUDIOPORT_TYPE_SPEAKER},
    {"dsAUDIOPORT_TYPE_HDMI_ARC", (int32_t)dsAUDIOPORT_TYPE_HDMI_ARC},
    {"dsAUDIOPORT_TYPE_HEADPHONE", (int32_t)dsAUDIOPORT_TYPE_HEADPHONE},
    {"dsAUDIOPORT_TYPE_MAX", (int32_t)dsAUDIOPORT_TYPE_MAX},
    {NULL, -1}};

/* dsAudioFormat_t */
const static ut_control_keyStringMapping_t dsAudioFormat_mapTable[] =
{
    {"dsAUDIO_FORMAT_NONE", (int32_t)dsAUDIO_FORMAT_NONE},
    {"dsAUDIO_FORMAT_PCM", (int32_t)dsAUDIO_FORMAT_PCM},
    {"dsAUDIO_FORMAT_DOLBY_AC3", (int32_t)dsAUDIO_FORMAT_DOLBY_AC3},
    {"dsAUDIO_FORMAT_DOLBY_EAC3", (int32_t)dsAUDIO_FORMAT_DOLBY_EAC3},
    {"dsAUDIO_FORMAT_DOLBY_AC4", (int32_t)dsAUDIO_FORMAT_DOLBY_AC4},
    {"dsAUDIO_FORMAT_DOLBY_MAT", (int32_t)dsAUDIO_FORMAT_DOLBY_MAT},
    {"dsAUDIO_FORMAT_DOLBY_TRUEHD", (int32_t)dsAUDIO_FORMAT_DOLBY_TRUEHD},
    {"dsAUDIO_FORMAT_DOLBY_EAC3_ATMOS", (int32_t)dsAUDIO_FORMAT_DOLBY_EAC3_ATMOS},
    {"dsAUDIO_FORMAT_DOLBY_TRUEHD_ATMOS", (int32_t)dsAUDIO_FORMAT_DOLBY_TRUEHD_ATMOS},
    {"dsAUDIO_FORMAT_DOLBY_MAT_ATMOS", (int32_t)dsAUDIO_FORMAT_DOLBY_MAT_ATMOS},
    {"dsAUDIO_FORMAT_DOLBY_AC4_ATMOS", (int32_t)dsAUDIO_FORMAT_DOLBY_AC4_ATMOS},
    {"dsAUDIO_FORMAT_AAC", (int32_t)dsAUDIO_FORMAT_AAC},
    {"dsAUDIO_FORMAT_VORBIS", (int32_t)dsAUDIO_FORMAT_VORBIS},
    {"dsAUDIO_FORMAT_WMA", (int32_t)dsAUDIO_FORMAT_WMA},
    {"dsAUDIO_FORMAT_UNKNOWN", (int32_t)dsAUDIO_FORMAT_UNKNOWN},
    {"dsAUDIO_FORMAT_MAX", (int32_t)dsAUDIO_FORMAT_MAX},
    {NULL, -1}};

/* dsATMOSCapability_t */
const static ut_control_keyStringMapping_t dsATMOSCapability_mapTable[] =
{
    {"dsAUDIO_ATMOS_NOTSUPPORTED", (int32_t)dsAUDIO_ATMOS_NOTSUPPORTED},
    {"dsAUDIO_ATMOS_DDPLUSSTREAM", (int32_t)dsAUDIO_ATMOS_DDPLUSSTREAM},
    {"dsAUDIO_ATMOS_ATMOSMETADATA", (int32_t)dsAUDIO_ATMOS_ATMOSMETADATA},
    {"dsAUDIO_ATMOS_MAX", (int32_t)dsAUDIO_ATMOS_MAX},
    {NULL, -1}};

/* dsAudioARCTypes_t */
const static ut_control_keyStringMapping_t dsAudioARCTypes_mapTable[] =
{
    {"dsAUDIOARCSUPPORT_NONE", (int32_t)dsAUDIOARCSUPPORT_NONE},
    {"dsAUDIOARCSUPPORT_ARC", (int32_t)dsAUDIOARCSUPPORT_ARC},
    {"dsAUDIOARCSUPPORT_eARC", (int32_t)dsAUDIOARCSUPPORT_eARC},
    {NULL, -1}};

/* dsAudioStereoMode_t */
const static ut_control_keyStringMapping_t dsAudioStereoMode_mapTable[] =
{
    {"dsAUDIO_STEREO_UNKNOWN", (int32_t)dsAUDIO_STEREO_UNKNOWN},
    {"dsAUDIO_STEREO_MONO", (int32_t)dsAUDIO_STEREO_MONO},
    {"dsAUDIO_STEREO_STEREO", (int32_t)dsAUDIO_STEREO_STEREO},
    {"dsAUDIO_STEREO_SURROUND", (int32_t)dsAUDIO_STEREO_SURROUND},
    {"dsAUDIO_STEREO_PASSTHRU", (int32_t)dsAUDIO_STEREO_PASSTHRU},
    {"dsAUDIO_STEREO_DD", (int32_t)dsAUDIO_STEREO_DD},
    {"dsAUDIO_STEREO_DDPLUS", (int32_t)dsAUDIO_STEREO_DDPLUS},
    {"dsAUDIO_STEREO_MAX", (int32_t)dsAUDIO_STEREO_MAX},
    {NULL, -1}};

/* dsAudioInput_t */
const static ut_control_keyStringMapping_t dsAudioInput_mapTable[] =
{
    {"dsAUDIO_INPUT_PRIMARY", (int32_t)dsAUDIO_INPUT_PRIMARY},
    {"dsAUDIO_INPUT_SYSTEM", (int32_t)dsAUDIO_INPUT_SYSTEM},
    {"dsAUDIO_INPUT_MAX", (int32_t)dsAUDIO_INPUT_MAX},
    {NULL, -1}};

/* dsError_t */
const static ut_control_keyStringMapping_t dsError_mapTable[] =
{
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
    {NULL, -1}};

/* bool */
const static ut_control_keyStringMapping_t bool_mapTable[] =
{
    {"false", (int32_t) false},
    {"true", (int32_t) true},
    {NULL, -1}};

/**
 * @brief This function clears the stdin buffer.
 *
 * This function clears the stdin buffer.
 */
static void readAndDiscardRestOfLine(FILE *in)
{
    int c;
    while ((c = fgetc(in)) != EOF && c != '\n');
}

static void readInt(int32_t *choice)
{
    scanf("%d", choice);
    readAndDiscardRestOfLine(stdin);
}

static void readFloat(float_t *choice)
{
    scanf("%f", choice);
    readAndDiscardRestOfLine(stdin);
}

static void readString(char *choice)
{
    scanf("%s", choice);
    readAndDiscardRestOfLine(stdin);
}

static void writeCallbackLog(char *logPath, const char *format, ...)
{
    va_list args;
    va_start(args, format);


    // Get the current timestamp in seconds and microseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Format the timestamp with year, month, day, time, and microseconds
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));

    // Append the microseconds to the timestamp
    snprintf(&timestamp[strlen(timestamp)], sizeof(timestamp) - strlen(timestamp), ".%06ld", tv.tv_usec);

    pthread_mutex_lock(&gCallbackMutex);

    // Open the log file in append mode
    FILE *fp = fopen(logPath, "a");
    if (fp == NULL) {
        UT_LOG_ERROR("Error opening log file: %s", strerror(errno));
        return;
    }

    // Print the timestamp and formatted message
    fprintf(fp, "[%s] ", timestamp);
    vfprintf(fp, format, args);
    fprintf(fp, "\n");

    // Close the log file
    fclose(fp);

    pthread_mutex_unlock(&gCallbackMutex);

    va_end(args);
}

/**
 * @brief Callback function for Headphone connection status.
 *
 * This function is invoked whenever a change occurs in the Headphone connection.
 */
static void audioOutPortConnectCB(dsAudioPortType_t portType, uint32_t uiPortNo, bool isPortCon)
{
    UT_LOG_INFO("Received Connection status callback port: %s, port number: %d, Connection: %s",
                 UT_Control_GetMapString(dsAudioPortType_mapTable, portType),uiPortNo,
                 UT_Control_GetMapString(bool_mapTable, isPortCon));

    writeCallbackLog(gConnectionCBFile, "Received Connection status callback port: %s, port number: %d, Connection: %s",
                     UT_Control_GetMapString(dsAudioPortType_mapTable, portType),uiPortNo,
                     UT_Control_GetMapString(bool_mapTable, isPortCon));

    gConnectionStatus = isPortCon;
}

/**
 * @brief Callback function for detecting audio format changes.
 *
 * This function is triggered whenever the audio format of the playback stream changes.
 */
static void audioFormatUpdateCB(dsAudioFormat_t audioFormat)
{
    UT_LOG_INFO("Received Format update callback : %s", UT_Control_GetMapString(dsAudioFormat_mapTable, audioFormat));

    writeCallbackLog(gFormatCBFile, "Received Format update callback : %s", UT_Control_GetMapString(dsAudioFormat_mapTable, audioFormat));

    gAudioFormat = audioFormat;
}

/**
 * @brief Callback function for detecting ATMOS Capabilities changes.
 *
 * This function is triggered whenever the ATMOS Capabilities changes.
 */
static void atmosCapsChange(dsATMOSCapability_t atmosCaps, bool status)
{
    UT_LOG_INFO("Received ATMOS Capablity Change callback, Capability: %s, Status: %s",
                 UT_Control_GetMapString(dsATMOSCapability_mapTable, atmosCaps),
                 UT_Control_GetMapString(bool_mapTable, status));

    writeCallbackLog(gATMOSCapsCBFile, "Received ATMOS Capablity Change callback, Capability: %s, Status: %s",
                 UT_Control_GetMapString(dsATMOSCapability_mapTable, atmosCaps),
                 UT_Control_GetMapString(bool_mapTable, status));

    gAtosCapablity = atmosCaps;
    gAtmosStatus = status;
}

/**
* @brief This test initializes the dsAudio Module.
*
* This test function initializes the dsAudio Module.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_initialize(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t choice = -1;

    UT_LOG_MENU_INFO("Select Device Type[0: Sink, 1: Source]: ");
    readInt(&choice);
    if(choice < 0 || choice > 1)
    {
        UT_LOG_ERROR("Invalid device Type");
        goto exit;
    }

    //TODO: replace with user input
    strncpy(gConnectionCBFile, DS_CONNECTION_CB_FILE, DS_MAX_FILE_SIZE);
    strncpy(gFormatCBFile, DS_FORMAT_CB_FILE, DS_MAX_FILE_SIZE);
    strncpy(gATMOSCapsCBFile, DS_ATMOS_CB_FILE, DS_MAX_FILE_SIZE);

    /* Initialize the dsAudio Module */
    UT_LOG_INFO("Calling dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_LOG_INFO("Result dsAudioPortInit() dsError_t:[%s]", UT_Control_GetMapString(dsError_mapTable, ret));
    DS_ASSERT(ret == dsERR_NONE);

    if(choice == 0)
    {
        /* Register connection status callback */
        UT_LOG_INFO("Calling dsAudioOutRegisterConnectCB(IN:CBFunc:[0x%0X])", audioOutPortConnectCB);
        ret = dsAudioOutRegisterConnectCB(audioOutPortConnectCB);
        UT_LOG_INFO("Result dsAudioOutRegisterConnectCB(IN:CBFunc:[0x%0X]) dsError_t:[%s]",
                     audioOutPortConnectCB, UT_Control_GetMapString(dsError_mapTable, ret));
        DS_ASSERT(ret == dsERR_NONE);

        /* Register ATMOS capabilities change callback */
        UT_LOG_INFO("Calling dsAudioAtmosCapsChangeRegisterCB(IN:cbFun:[0x%0X])", atmosCapsChange);
        ret = dsAudioAtmosCapsChangeRegisterCB (atmosCapsChange);
        UT_LOG_INFO("Result dsAudioAtmosCapsChangeRegisterCB(IN:cbFun:[0x%0X]) dsError_t:[%s]",
                    atmosCapsChange, UT_Control_GetMapString(dsError_mapTable, ret));
        DS_ASSERT(ret == dsERR_NONE);
    }

    /* Register audio format change callback */
    UT_LOG_INFO("Calling dsAudioFormatUpdateRegisterCB(IN:cbFun:[0x%0X])", audioFormatUpdateCB);
    ret = dsAudioFormatUpdateRegisterCB(audioFormatUpdateCB);
    UT_LOG_INFO("Result dsAudioFormatUpdateRegisterCB(IN:cbFun:[0x%0X]) dsError_t:[%s]",
                 audioFormatUpdateCB, UT_Control_GetMapString(dsError_mapTable, ret));
    DS_ASSERT(ret == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

static int32_t dsAudio_list_select_ports(dsAudioPortType_t *pAudioPort, int32_t *pIndex)
{
    int32_t choice = -1;

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t\tdsAudio Port");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#   %-30s","Audio Port");
    for(int32_t i = dsAUDIOPORT_TYPE_ID_LR; i < dsAUDIOPORT_TYPE_MAX; i++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-30s", i, UT_Control_GetMapString(dsAudioPortType_mapTable, i));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");

    UT_LOG_MENU_INFO("Select dsAudio Port: ");
    readInt(&choice);
    if(choice < dsAUDIOPORT_TYPE_ID_LR || choice >= dsAUDIOPORT_TYPE_MAX)
    {
        UT_LOG_ERROR("Invalid Port choice");
        return -1;
    }

    *pAudioPort = choice;

    UT_LOG_MENU_INFO("Select dsAudio Port Index[0-%d]: ", DS_AUDIO_MAX_INDEX);
    readInt(&choice);
    if(choice < 0 || choice > DS_AUDIO_MAX_INDEX)
    {
        UT_LOG_ERROR("Invalid Port choice");
        return -1;
    }

    *pIndex = choice;

    return 0;
}

static intptr_t dsAudio_getPort(dsAudioPortType_t audioPort, int32_t index, bool *pEnabled)
{
    intptr_t handle = (intptr_t)NULL;
    int32_t ret;

    UT_LOG_INFO("Calling dsGetAudioPort(IN:type:[%s], IN:index:[%d], OUT:handle:[])",
                 UT_Control_GetMapString(dsAudioPortType_mapTable, audioPort), index);

    ret = dsGetAudioPort(audioPort, index, &handle);

    UT_LOG_INFO("Result dsGetAudioPort(IN:type:[%s], IN:index:[%d], OUT:handle:[0x%0X]) dsError_t:[%s]",
                 UT_Control_GetMapString(dsAudioPortType_mapTable, audioPort), index,
                 handle,
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsIsAudioPortEnabled(IN:handle:[0x%0X], OUT:enabled:[])",
                 handle);

    ret = dsIsAudioPortEnabled(handle, pEnabled);

    UT_LOG_INFO("Result dsIsAudioPortEnabled(IN:handle:[0x%0X], OUT:enabled:[%s]) dsError_t:[%s]",
                 handle, UT_Control_GetMapString(bool_mapTable, *pEnabled),
                 UT_Control_GetMapString(dsError_mapTable, ret));

    return handle;
}

/**
* @brief This test enables the dsAudio port.
*
* This test function enables the dsAudio port.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_enable_port(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    UT_LOG_INFO("Calling dsEnableAudioPort(IN:handle:[0x%0X], IN:enabled:[%s])",
                 handle,
                 UT_Control_GetMapString(bool_mapTable, true));

    ret = dsEnableAudioPort(handle, true);

    UT_LOG_INFO("Result dsEnableAudioPort(IN:handle:[0x%0X], IN:enabled:[%s]) dsError_t:[%s]",
                 handle,
                 UT_Control_GetMapString(bool_mapTable, true),
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    if(port == dsAUDIOPORT_TYPE_HDMI_ARC)
    {
        int32_t archType = 0;

        memset(&gARCStatus, 0, sizeof(dsAudioARCStatus_t));

        UT_LOG_MENU_INFO("----------------------------------------------------------");
        UT_LOG_MENU_INFO("Supported ARC Types");
        UT_LOG_MENU_INFO("----------------------------------------------------------");
        UT_LOG_MENU_INFO("\t#  %-20s","ARC Type");
        for(int32_t i = dsAUDIOARCSUPPORT_ARC; i <= dsAUDIOARCSUPPORT_eARC; i++)
        {
            UT_LOG_MENU_INFO("\t%d.  %-20s", i, UT_Control_GetMapString(dsAudioARCTypes_mapTable, i));
        }
        UT_LOG_MENU_INFO("----------------------------------------------------------");
        UT_LOG_MENU_INFO("Select ARC Type: ");
        readInt(&archType);
        if(archType < dsAUDIOARCSUPPORT_ARC || archType > dsAUDIOARCSUPPORT_eARC)
        {
            UT_LOG_ERROR("Invalid ARC Type");
            goto exit;
        }

        gARCStatus.status = true;
        gARCStatus.type   = (dsAudioARCTypes_t)archType;

        UT_LOG_INFO("Calling dsAudioEnableARC(IN:handle:[0x%0X], IN:arcStatus:[.type=%s, .status=%s])",
                    handle,
                    UT_Control_GetMapString(dsAudioARCTypes_mapTable, gARCStatus.type),
                    UT_Control_GetMapString(bool_mapTable, gARCStatus.status));

        ret = dsAudioEnableARC(handle, gARCStatus);

        UT_LOG_INFO("Result dsAudioEnableARC(IN:handle:[0x%0X], IN:arcStatus:[.type=%s, .status=%s]) dsError_t:[%s]",
                     handle,
                     UT_Control_GetMapString(dsAudioARCTypes_mapTable, gARCStatus.type),
                     UT_Control_GetMapString(bool_mapTable, gARCStatus.status),
                     UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);
    } //if(gDSAudioPortConfiguration[port].typeid == dsAUDIOPORT_TYPE_HDMI_ARC)

    UT_LOG_INFO("Calling dsIsAudioPortEnabled(IN:handle:[0x%0X], OUT:enabled:[])", handle);

    ret = dsIsAudioPortEnabled(handle, &enabled);

    UT_LOG_INFO("Result dsIsAudioPortEnabled(IN:handle:[0x%0X], OUT:enabled:[%s]) dsError_t:[%s]",
                 handle, UT_Control_GetMapString(bool_mapTable, enabled),
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE && enabled);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test disables the dsAudio port.
*
* This test function disables the dsAudio port.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_disable_port(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;

    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(port == dsAUDIOPORT_TYPE_HDMI_ARC)
    {
        gARCStatus.status = false;

        UT_LOG_INFO("Calling dsAudioEnableARC(IN:handle:[0x%0X], IN:arcStatus:[.type=%s, .status=%s])",
                    handle,
                    UT_Control_GetMapString(dsAudioARCTypes_mapTable, gARCStatus.type),
                    UT_Control_GetMapString(bool_mapTable, gARCStatus.status));

        ret = dsAudioEnableARC(handle, gARCStatus);

        UT_LOG_INFO("Result dsAudioEnableARC(IN:handle:[0x%0X], IN:arcStatus:[.type=%s, .status=%s]) dsError_t:[%s]",
                     handle,
                     UT_Control_GetMapString(dsAudioARCTypes_mapTable, gARCStatus.type),
                     UT_Control_GetMapString(bool_mapTable, gARCStatus.status),
                     UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);
    }

    UT_LOG_INFO("Calling dsEnableAudioPort(IN:handle:[0x%0X], IN:enabled:[%s])",
                 handle,
                 UT_Control_GetMapString(bool_mapTable, false));

    ret = dsEnableAudioPort(handle, false);

    UT_LOG_INFO("Result dsEnableAudioPort(IN:handle:[0x%0X], IN:enabled:[%s]) dsError_t:[%s]",
                 handle,
                 UT_Control_GetMapString(bool_mapTable, false),
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsIsAudioPortEnabled(IN:handle:[0x%0X], OUT:enabled:[])",
                 handle);

    ret = dsIsAudioPortEnabled(handle, &enabled);

    UT_LOG_INFO("Result dsIsAudioPortEnabled(IN:handle:[0x%0X], OUT:enabled:[%s]) dsError_t:[%s]",
                 handle, UT_Control_GetMapString(bool_mapTable, enabled),
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE && !enabled);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test checks for headphone connection status.
*
* This test function checks for headphone connection status.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_headphone_connection(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    bool isConnected = false;
    bool enabled = false;

    handle = dsAudio_getPort(dsAUDIOPORT_TYPE_HEADPHONE, 0, &enabled);

    if(handle == (intptr_t)NULL)
    {
        UT_LOG_ERROR("Platform doesn't support Headphone");
    }
    else
    {
        UT_LOG_INFO("Calling dsAudioOutIsConnected(IN:handle:[0x%0X], OUT:isConnected:[])",
                    handle);

        ret = dsAudioOutIsConnected(handle, &isConnected);

        UT_LOG_INFO("Result dsAudioOutIsConnected(IN:handle:[0x%0X], OUT:isConnected:[%s]) dsError_t:[%s]",
                    handle, UT_Control_GetMapString(bool_mapTable, isConnected),
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);
    }

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets the audio compression levels.
*
* This test function sets the audio compression levels.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_audio_compression(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t compression = 0;
    int32_t compression_g = 0;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Select Compression[0-10]: ");
    readInt(&compression);
    if(compression < 0 || compression > 10)
    {
        UT_LOG_MENU_INFO("Invalid Compression Type");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetAudioCompression(IN:handle:[0x%0X], IN:compression:[%d])",
                 handle, compression);

    ret = dsSetAudioCompression(handle, (dsAudioCompression_t)compression);

    UT_LOG_INFO("Result dsSetAudioCompression(IN:handle:[0x%0X], IN:compression:[%d]) dsError_t:[%s]",
                 handle, compression,
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetAudioCompression(IN:handle:[0x%0X], OUT:compression:[])", handle);

    ret = dsGetAudioCompression(handle, &compression_g);

    UT_LOG_INFO("Result dsGetAudioCompression(IN:handle:[0x%0X], OUT:compression:[%d]) dsError_t:[%s]",
                 handle, compression_g,
                 UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(compression_g == compression);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test checks for MS12 DAP features.
*
* This test function checks for MS12 DAP features.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 006@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_ms12_dap(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t choice = -1;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("MS12 DAP Feature");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#   %-20s","MS12 DAP Feature");
    UT_LOG_MENU_INFO("\t1.  %-20s","DolbyVolume");
    UT_LOG_MENU_INFO("\t2.  %-20s","IntelligentEqualizer");
    UT_LOG_MENU_INFO("\t3.  %-20s","DialogueEnhancer");
    UT_LOG_MENU_INFO("\t4.  %-20s","Volumeleveller");
    UT_LOG_MENU_INFO("\t5.  %-20s","BassEnhancer");
    UT_LOG_MENU_INFO("\t6.  %-20s","SurroundDecoder");
    UT_LOG_MENU_INFO("\t7.  %-20s","DRCMode");
    UT_LOG_MENU_INFO("\t8.  %-20s","SurroundVirtualizer");
    UT_LOG_MENU_INFO("\t9.  %-20s","MISteering");
    UT_LOG_MENU_INFO("\t10. %-20s","GraphicEqualizer");
    UT_LOG_MENU_INFO("\t11. %-20s","LEConfig");
    UT_LOG_MENU_INFO("----------------------------------------------------------");

    UT_LOG_MENU_INFO("Select MS12 DAP Feature: ");
    readInt(&choice);

    switch(choice)
    {
        case 1: //DolbyVolume
        {
            bool mode = false, mode_g = false;

            UT_LOG_MENU_INFO("Dolby Volume[1:Enable, 2:Disable]: ");
            readInt(&choice);
            if(choice < 1 || choice > 2)
            {
                UT_LOG_ERROR("Invalid Dolby Volume mode");
                break;
            }

            mode = (choice == 1) ? true : false;

            UT_LOG_INFO("Calling dsSetDolbyVolumeMode(IN:handle:[0x%0X], IN:mode:[%s])",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode));

            ret = dsSetDolbyVolumeMode(handle, mode);

            UT_LOG_INFO("Result dsSetDolbyVolumeMode(IN:handle:[0x%0X], IN:mode:[%s]) dsError_t:[%s]",
                         handle,
                         UT_Control_GetMapString(bool_mapTable, mode),
                         UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetDolbyVolumeMode(IN:handle:[0x%0X], OUT:mode:[])",
                        handle);

            ret = dsGetDolbyVolumeMode(handle, &mode_g);

            UT_LOG_INFO("Result dsGetDolbyVolumeMode(IN:handle:[0x%0X], OUT:mode:[%s]) dsError_t:[%s]",
                         handle,
                         UT_Control_GetMapString(bool_mapTable, mode_g),
                         UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);

            break;
        }
        case 2: //IntelligentEqualizer
        {
            int32_t mode = 0, mode_g = 0;

            UT_LOG_MENU_INFO("----------------------------------------------------------");
            UT_LOG_MENU_INFO("IntelligentEqualizer Mode");
            UT_LOG_MENU_INFO("----------------------------------------------------------");
            UT_LOG_MENU_INFO("\t#   %-20s","Mode");
            UT_LOG_MENU_INFO("\t0.  %-20s","OFF");
            UT_LOG_MENU_INFO("\t1.  %-20s","Open");
            UT_LOG_MENU_INFO("\t2.  %-20s","Rich");
            UT_LOG_MENU_INFO("\t3.  %-20s","Focused");
            UT_LOG_MENU_INFO("\t4.  %-20s","Balanced");
            UT_LOG_MENU_INFO("\t5.  %-20s","Warm");
            UT_LOG_MENU_INFO("\t6.  %-20s","Detailed");
            UT_LOG_MENU_INFO("----------------------------------------------------------");

            UT_LOG_MENU_INFO("Select IntelligentEqualizer Mode: ");
            readInt(&mode);

            if(mode < 0 || mode > 6)
            {
                UT_LOG_ERROR("Invalid IntelligentEqualizer Mode choice");
                break;
            }

            UT_LOG_INFO("Calling dsSetIntelligentEqualizerMode(IN:handle:[0x%0X], IN:mode:[%d])",
                        handle, mode);

            ret = dsSetIntelligentEqualizerMode(handle, mode);

            UT_LOG_INFO("Result dsSetIntelligentEqualizerMode(IN:handle:[0x%0X], IN:mode:[%d]) dsError_t:[%s]",
                        handle, mode,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetIntelligentEqualizerMode(IN:handle:[0x%0X], OUT:mode:[])", handle);

            ret = dsGetIntelligentEqualizerMode(handle, &mode_g);

            UT_LOG_INFO("Result dsGetIntelligentEqualizerMode(IN:handle:[0x%0X], OUT:mode:[%d]) dsError_t:[%s]",
                        handle, mode_g,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);
            break;
        }
        case 3: //DialogueEnhancer
        {
            int32_t level = 0, level_g = 0;

            UT_LOG_MENU_INFO("Select DialogueEnhancer Level[0-16]: ");
            readInt(&level);
            if(level < 0 || level > 16)
            {
                UT_LOG_ERROR("Invalid DialogueEnhancer Level");
                break;
            }

            UT_LOG_INFO("Calling dsSetDialogEnhancement(IN:handle:[0x%0X], IN:level:[%d])",
                        handle, level);

            ret = dsSetDialogEnhancement(handle, level);

            UT_LOG_INFO("Result dsSetDialogEnhancement(IN:handle:[0x%0X], IN:level:[%d]) dsError_t:[%s]",
                        handle, level,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetDialogEnhancement(IN:handle:[0x%0X], OUT:level:[]])",
                        handle);

            ret = dsGetDialogEnhancement(handle, &level_g);

            UT_LOG_INFO("Result dsGetDialogEnhancement(IN:handle:[0x%0X], OUT:level:[%d]) dsError_t:[%s]",
                        handle, level_g,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(level == level_g);
            break;
        }
        case 4: //VolumeLeveller
        {
            dsVolumeLeveller_t volume_level = {0};
            dsVolumeLeveller_t volume_level_g = {0};

            UT_LOG_MENU_INFO("Select Volumeleveller Mode[0:Off, 1:On, 2:Auto]: ");
            readInt(&volume_level.mode);
            if(volume_level.mode < 0 || volume_level.mode > 2)
            {
                UT_LOG_ERROR("Invalid Volumeleveller Mode");
                break;
            }
            UT_LOG_MENU_INFO("Select Volumeleveller Level[0-10]: ");
            readInt(&volume_level.level);
            if(volume_level.level < 0 || volume_level.level > 10)
            {
                UT_LOG_ERROR("Invalid Volumeleveller level");
                break;
            }

            UT_LOG_INFO("Calling dsSetVolumeLeveller(IN:handle:[0x%0X], IN:volLeveller:[mode:%d, level:%d])",
                         handle, volume_level.mode, volume_level.level);

            ret = dsSetVolumeLeveller(handle, volume_level);

            UT_LOG_INFO("Result dsSetVolumeLeveller(IN:handle:[0x%0X], IN:volLeveller:[mode:%d, level:%d]) dsError_t:[%s]",
                        handle, volume_level.mode, volume_level.level,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetVolumeLeveller(IN:handle:[0x%0X], OUT:volLeveller:[])",
                         handle);

            ret = dsGetVolumeLeveller(handle, &volume_level_g);

            UT_LOG_INFO("Result dsGetVolumeLeveller(IN:handle:[0x%0X], OUT:volLeveller:[mode:%d, level:%d]) dsError_t:[%s]",
                        handle, volume_level_g.mode, volume_level_g.level,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(volume_level.mode == volume_level_g.mode);
            DS_ASSERT(volume_level.level == volume_level_g.level);
            break;
        }
        case 5: //BassEnhancer
        {
            int32_t boost = 0, boost_g = 0;

            UT_LOG_MENU_INFO("Select BassEnhancer Boost[0-100]: ");
            readInt(&boost);
            if(boost < 0 || boost > 100)
            {
                UT_LOG_ERROR("Invalid BassEnhancer Boost");
                break;
            }

            UT_LOG_INFO("Calling dsSetBassEnhancer(IN:handle:[0x%0X], IN:boost:[%d])",
                        handle, boost);

            ret = dsSetBassEnhancer(handle, boost);

            UT_LOG_INFO("Result dsSetBassEnhancer(IN:handle:[0x%0X], IN:boost:[%d]) dsError_t:[%s]",
                        handle, boost,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetBassEnhancer(IN:handle:[0x%0X], OUT:boost:[])", handle);

            ret = dsGetBassEnhancer(handle, &boost_g);

            UT_LOG_INFO("Result dsGetBassEnhancer(IN:handle:[0x%0X], OUT:boost:[%d]) dsError_t:[%s]",
                        handle, boost_g,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(boost == boost_g);
            break;
        }
        case 6: //SurroundDecoder
        {
            bool mode = false, mode_g = false;

            UT_LOG_MENU_INFO("Surround Decoder[1:Enable, 2:Disable]: ");
            readInt(&choice);
            if(choice < 1 || choice > 2)
            {
                UT_LOG_ERROR("Invalid choice");
                break;
            }

            mode = (choice == 1) ? true : false;

            UT_LOG_INFO("Calling dsEnableSurroundDecoder(IN:handle:[0x%0X], IN:mode:[%s])",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode));

            ret = dsEnableSurroundDecoder(handle, mode);

            UT_LOG_INFO("Result dsEnableSurroundDecoder(IN:handle:[0x%0X], IN:mode:[%s]) dsError_t:[%s]",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode),
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsIsSurroundDecoderEnabled(IN:handle:[0x%0X], OUT:mode:[])", handle);

            ret = dsIsSurroundDecoderEnabled(handle, &mode_g);

            UT_LOG_INFO("Result dsIsSurroundDecoderEnabled(IN:handle:[0x%0X], OUT:mode:[%s]) dsError_t:[%s]",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode_g),
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);
            break;
        }
        case 7: //DRCMode
        {
            int32_t mode = 0, mode_g = 0;

            UT_LOG_MENU_INFO("Select DRC Mode[0: DRC Line Mode, 1: DRC RF Mode]: ");
            readInt(&mode);
            if(mode < 0 || mode > 1)
            {
                UT_LOG_ERROR("Invalid DRC Mode");
                break;
            }

            UT_LOG_INFO("Calling dsSetDRCMode(IN:handle:[0x%0X], IN:mode:[%d])",
                         handle, mode);

            ret = dsSetDRCMode(handle, mode);

            UT_LOG_INFO("Result dsSetDRCMode(IN:handle:[0x%0X], IN:mode:[%d]) dsError_t:[%s]",
                        handle, mode,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetDRCMode(IN:handle:[0x%0X], OUT:mode:[])", handle);

            ret = dsGetDRCMode(handle, &mode_g);

            UT_LOG_INFO("Result dsGetDRCMode(IN:handle:[0x%0X], OUT:mode:[%d]) dsError_t:[%s]",
                        handle, mode_g,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);
            break;
        }
        case 8: //SurroundVirtualizer
        {
            dsSurroundVirtualizer_t surround_virtualize = {0};
            dsSurroundVirtualizer_t surround_virtualize_g = {0};

            UT_LOG_MENU_INFO("Select SurroundVirtualizer Mode[0:Off, 1:On, 2:Auto]: ");
            readInt(&surround_virtualize.mode);
            if(surround_virtualize.mode < 0 || surround_virtualize.mode > 2)
            {
                UT_LOG_ERROR("Invalid SurroundVirtualizer Mode");
                break;
            }
            UT_LOG_MENU_INFO("Select SurroundVirtualizer Boost[0-96]: ");
            readInt(&surround_virtualize.boost);
            if(surround_virtualize.boost < 0 || surround_virtualize.boost > 10)
            {
                UT_LOG_ERROR("Invalid Volumeleveller boost");
                break;
            }

            UT_LOG_INFO("Calling dsSetSurroundVirtualizer(IN:handle:[0x%0X], IN:virtualizer:[mode:%d, boost:%d])",
                         handle, surround_virtualize.mode, surround_virtualize.boost);

            ret = dsSetSurroundVirtualizer(handle, surround_virtualize);

            UT_LOG_INFO("Result dsSetSurroundVirtualizer(IN:handle:[0x%0X], IN:virtualizer:[mode:%d, boost:%d]) dsError_t:[%s]",
                        handle, surround_virtualize.mode, surround_virtualize.boost,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetSurroundVirtualizer(IN:handle:[0x%0X], OUT:virtualizer:[])",
                         handle, surround_virtualize.mode, surround_virtualize.boost);

            ret = dsGetSurroundVirtualizer(handle, &surround_virtualize_g);

            UT_LOG_INFO("Result dsGetSurroundVirtualizer(IN:handle:[0x%0X], OUT:virtualizer:[mode:%d, boost:%d]) dsError_t:[%s]",
                        handle, surround_virtualize_g.mode, surround_virtualize_g.boost,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(surround_virtualize.mode == surround_virtualize_g.mode);
            DS_ASSERT(surround_virtualize.boost == surround_virtualize_g.boost);
            break;
        }
        case 9: //MISteering
        {
            bool mode = false, mode_g = false;

            UT_LOG_MENU_INFO("MI Steering[1:Enable, 2:Disable]: ");
            readInt(&choice);
            if(choice < 1 || choice > 2)
            {
                UT_LOG_ERROR("Invalid choice");
                break;
            }

            mode = (choice == 1) ? true : false;

            UT_LOG_INFO("Calling dsSetMISteering(IN:handle:[0x%0X], IN:mode:[%s])",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode));

            ret = dsSetMISteering(handle, mode);

            UT_LOG_INFO("Result dsSetMISteering(IN:handle:[0x%0X], IN:mode:[%s]) dsError_t:[%s]",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode),
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetMISteering(IN:handle:[0x%0X], OUT:mode:[])", handle);

            ret = dsGetMISteering(handle, &mode_g);

            UT_LOG_INFO("Result dsGetMISteering(handle:[0x%0X], OUT:mode:[%s]) dsError_t:[%s]",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode_g),
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);

            break;
        }
        case 10: //GraphicEqualizer
        {
            int32_t mode = 0, mode_g = 0;

            UT_LOG_MENU_INFO("Select GraphicEqualizer Mode[0: OFF, 1: EQ Open, 2: EQ Rich, 3: EQ Focused]: ");
            readInt(&mode);
            if(mode < 0 || mode > 3)
            {
                UT_LOG_ERROR("Invalid GraphicEqualizer Mode");
                break;
            }

            UT_LOG_INFO("Calling dsSetGraphicEqualizerMode(IN:handle:[0x%0X], IN:mode:[%d])",
                        handle, mode);

            ret = dsSetGraphicEqualizerMode(handle, mode);

            UT_LOG_INFO("Result dsSetGraphicEqualizerMode(IN:handle:[0x%0X], IN:mode:[%d]) dsError_t:[%s]",
                        handle, mode,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetGraphicEqualizerMode(IN:handle:[0x%0X], OUT:mode:[])", handle);

            ret = dsGetGraphicEqualizerMode(handle, &mode_g);

            UT_LOG_INFO("Result dsGetGraphicEqualizerMode(IN:handle:[0x%0X], OUT:mode:[%d]) dsError_t:[%s]",
                        handle, mode_g,
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);
            break;
        }
        case 11: //LEConfig
        {
            bool mode = false, mode_g = false;

            UT_LOG_MENU_INFO("LE Configuration[1:Enable, 2:Disable]: ");
            readInt(&choice);
            if(choice < 1 || choice > 2)
            {
                UT_LOG_ERROR("Invalid choice");
                break;
            }

            mode = (choice == 1) ? true : false;

            UT_LOG_INFO("Calling dsEnableLEConfig(IN:handle:[0x%0X], IN:mode:[%s])",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode));

            ret = dsEnableLEConfig(handle, mode);

            UT_LOG_INFO("Result dsEnableLEConfig(IN:handle:[0x%0X], IN:mode:[%s]) dsError_t:[%s]",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode),
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);

            UT_LOG_INFO("Calling dsGetLEConfig(IN:handle:[0x%0X], OUT:mode:[])", handle);

            ret = dsGetLEConfig(handle, &mode_g);

            UT_LOG_INFO("Result dsGetLEConfig(IN:handle:[0x%0X], OUT:mode:[%s]) dsError_t:[%s]",
                        handle,
                        UT_Control_GetMapString(bool_mapTable, mode_g),
                        UT_Control_GetMapString(dsError_mapTable, ret));

            DS_ASSERT(ret == dsERR_NONE);
            DS_ASSERT(mode == mode_g);
            break;
        }
        default:
            UT_LOG_MENU_INFO("Invalid DAP Feature Choice");
    }
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets output mode.
*
* This test function sets output mode.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 007@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_stereo_mode(void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t mode = 0;
    int32_t mode_g = 0;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Stereo Mode");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#  %-20s","Stereo Mode");
    for(int32_t i = dsAUDIO_STEREO_MONO; i < dsAUDIO_STEREO_MAX; i++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-30s", i,
                     UT_Control_GetMapString(dsAudioStereoMode_mapTable, i));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");

    UT_LOG_MENU_INFO("Select Stereo Mode: ");
    readInt(&mode);
    if(mode < dsAUDIO_STEREO_MONO || mode >= dsAUDIO_STEREO_MAX)
    {
        UT_LOG_ERROR("Invalid Mode choice");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetStereoMode(IN:handle:[0x%0X], IN:mode:[%s])",
                 handle,
                 UT_Control_GetMapString(dsAudioStereoMode_mapTable, mode));

    ret = dsSetStereoMode(handle, (dsAudioStereoMode_t)mode);

    UT_LOG_INFO("Result dsSetStereoMode(IN:handle:[0x%0X], IN:mode:[%s]) dsError_t:[%s]",
                handle, UT_Control_GetMapString(dsAudioStereoMode_mapTable, mode),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetStereoMode(IN:handle:[0x%0X], OUT:mode:[])", handle);

    ret = dsGetStereoMode(handle, (dsAudioStereoMode_t *)&mode_g);

    UT_LOG_INFO("Result dsGetStereoMode(IN:handle:[0x%0X], OUT:mode:[%s]) dsError_t:[%s]",
                handle, UT_Control_GetMapString(dsAudioStereoMode_mapTable, mode_g),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(mode == mode_g);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets auto output mode.
*
* This test function sets auto output mode.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 008@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_stereo_auto_mode(void)
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t mode = 0;
    int32_t mode_g = 0;
    int32_t choice = -1;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Stereo Auto Mode[1:Enable, 2:Disable]: ");
    readInt(&choice);
    if(choice < 1 || choice > 2)
    {
        UT_LOG_ERROR("Invalid choice");
        goto exit;
    }

    mode = (choice == 1) ? true : false;

    UT_LOG_INFO("Calling dsSetStereoAuto(IN:handle:[0x%0X], IN:autoMode:[%s])",
                handle,
                UT_Control_GetMapString(bool_mapTable, mode));

    ret = dsSetStereoAuto(handle, mode);

    UT_LOG_INFO("Result dsSetStereoAuto(IN:handle:[0x%0X], IN:autoMode:[%s]) dsError_t:[%s]",
                handle, UT_Control_GetMapString(bool_mapTable, mode),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetStereoAuto(IN:handle:[0x%0X], OUT:autoMode:[])",
                handle);

    ret = dsGetStereoAuto(handle, &mode_g);

    UT_LOG_INFO("Result dsGetStereoAuto(IN:handle:[0x%0X], OUT:autoMode:[%s]) dsError_t:[%s]",
                handle, UT_Control_GetMapString(bool_mapTable, mode_g),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(mode == mode_g);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets the audio levels.
*
* This test function sets the audio levels.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 009@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_audio_level(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    float_t gain_level = 0;
    float_t gain_level_g = 0;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Enter Gain Level[0.0 to 100.0]: ");
    readFloat(&gain_level);
    if(gain_level < 0 || gain_level > 100)
    {
        UT_LOG_ERROR("Invalid Audio Gain Level");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetAudioLevel(IN:handle:[0x%0X], IN:level:[%f])",
                 handle, gain_level);

    ret = dsSetAudioLevel(handle, gain_level);

    UT_LOG_INFO("Result dsSetAudioLevel(IN:handle:[0x%0X], IN:level:[%f]) dsError_t:[%s]",
                handle, gain_level,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetAudioLevel(IN:handle:[0x%0X], OUT:level:[])", handle);

    ret = dsGetAudioLevel(handle, &gain_level_g);

    UT_LOG_INFO("Result dsGetAudioLevel(IN:handle:[0x%0X], OUT:level:[%f]) dsError_t:[%s]",
                handle, gain_level_g,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(gain_level == gain_level_g);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets the audio gain for SPEAKER.
*
* This test function sets the audio levels.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 009@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_audio_gain(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    float_t gain = 0.0;
    float_t gain_g = 0.0;

    handle = dsAudio_getPort(dsAUDIOPORT_TYPE_SPEAKER, 0, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Enter Gain[-2080.0 to 480.0]: ");
    readFloat(&gain);
    if(gain < -2080 || gain > 480)
    {
        UT_LOG_ERROR("Invalid Audio Gain value");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetAudioGain(IN:handle:[0x%0X], IN:gain:[%f])",
                 handle, gain);

    ret = dsSetAudioGain(handle, gain);

    UT_LOG_INFO("Result dsSetAudioGain(IN:handle:[0x%0X], IN:gain:[%f]) dsError_t:[%s]",
                handle, gain,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetAudioGain(IN:handle:[0x%0X], OUT:gain:[])",
                 handle);

    ret = dsGetAudioGain(handle, &gain_g);

    UT_LOG_INFO("Result dsGetAudioGain(IN:handle:[0x%0X], OUT:gain:[%f]) dsError_t:[%s]",
                handle, gain_g,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(gain == gain_g);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test Mutes or UnMutes the Audio.
*
* This test function Mutes or UnMutes the Audio.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 010@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_audio_mute(void)
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t choice = -1;
    bool mute = false;
    bool mute_g  = false;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Audio Mute/UnMute[1:Mute, 2:UnMute]: ");
    readInt(&choice);
    if(choice < 1 || choice > 2)
    {
        UT_LOG_ERROR("Invalid Mute choice");
        goto exit;
    }

    mute = (choice == 1) ? true : false;

    UT_LOG_INFO("Calling dsSetAudioMute(IN:handle:[0x%0X], IN:mute:[%s])",
                handle,
                UT_Control_GetMapString(bool_mapTable, mute));

    ret = dsSetAudioMute(handle, mute);

    UT_LOG_INFO("Result dsSetAudioMute(IN:handle:[0x%0X], IN:mute:[%s]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(bool_mapTable, mute),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsIsAudioMute(IN:handle:[0x%0X], OUT:muted:[])", handle);

    ret = dsIsAudioMute(handle, &mute_g);

    UT_LOG_INFO("Result dsIsAudioMute(IN:handle:[0x%0X], OUT:muted:[%s]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(bool_mapTable, mute_g),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(mute == mute_g);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets audio delay.
*
* This test function sets audio delay.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 011@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_audio_delay(void)
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    int32_t audioDelayMs = 0;
    uint32_t audioDelayMs_g = 0;
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Enter Audio Delay in milli seconds[0 to 200]: ");
    readInt(&audioDelayMs);
    if(audioDelayMs < 0 || audioDelayMs > 200)
    {
        UT_LOG_ERROR("Invalid Audio Delay");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetAudioDelay(IN:handle:[0x%0X], IN:audioDelayMs:[%d])",
                 handle, audioDelayMs);

    ret = dsSetAudioDelay(handle, audioDelayMs);

    UT_LOG_INFO("Result dsSetAudioDelay(IN:handle:[0x%0X], IN:audioDelayMs:[%d]) dsError_t:[%s]",
                handle, audioDelayMs,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetAudioDelay(IN:handle:[0x%0X], OUT:audioDelayMs:[%d])",
                 handle, audioDelayMs);

    ret = dsGetAudioDelay(handle, &audioDelayMs_g);

    UT_LOG_INFO("Result dsGetAudioDelay(IN:handle:[0x%0X], OUT:audioDelayMs:[%d]) dsError_t:[%s]",
                handle, audioDelayMs_g,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(audioDelayMs == audioDelayMs_g);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test gets audio format of playback stream.
*
* This test function gets audio format of playback stream.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 012@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_get_audio_format(void)
{
    gTestID = 12;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    dsAudioFormat_t getAudioFormat = dsAUDIO_FORMAT_NONE;

    UT_LOG_INFO("Calling dsGetAudioFormat(IN:handle:[0x%0X], OUT:audioFormat:[])", handle);

    ret = dsGetAudioFormat(handle, &getAudioFormat);

    UT_LOG_INFO("Result dsGetAudioFormat(IN:handle:[0x%0X], OUT:audioFormat:[%s]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(dsAudioFormat_mapTable, getAudioFormat),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets atmos mode.
*
* This test function sets atmos mode.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 013@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_atmos_mode(void)
{
    gTestID = 13;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    int32_t choice = 0;
    bool enable = false;

    UT_LOG_MENU_INFO("Enable/Disable ATMOS Mode[1:Enable, 2:Disable]: ");
    readInt(&choice);
    if(choice < 1 || choice > 2)
    {
        UT_LOG_ERROR("Invalid ATMOS mode");
        goto exit;
    }
    enable = (choice == 1) ? true : false;

    UT_LOG_INFO("Calling dsSetAudioAtmosOutputMode(IN:handle:[0x%0X], IN:enable:[%s])",
                handle,
                UT_Control_GetMapString(bool_mapTable, enable));

    ret = dsSetAudioAtmosOutputMode(handle, enable);

    UT_LOG_INFO("Result dsSetAudioAtmosOutputMode(IN:handle:[0x%0X], IN:enable:[%s]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(bool_mapTable, enable),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test gets connected device atmos capabilities.
*
* This test function gets connected device atmos capabilities.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 014@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_getConnected_atmoscaps(void)
{
    gTestID = 14;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsATMOSCapability_t capability = dsAUDIO_ATMOS_NOTSUPPORTED;
    dsAudioPortType_t port         = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_INFO("Calling dsGetSinkDeviceAtmosCapability(IN:handle:[0x%0X], OUT:capability:[])", handle);

    ret = dsGetSinkDeviceAtmosCapability(handle, &capability);

    UT_LOG_INFO("Result dsGetSinkDeviceAtmosCapability(IN:handle:[0x%0X], OUT:capability:[%s]) dsError_t:[%s]",
                handle, UT_Control_GetMapString(dsATMOSCapability_mapTable, capability),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets ms12 profiles.
*
* This test function sets ms12 profiles.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 015@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_ms12Profile(void)
{
    gTestID = 15;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    int32_t choice = -1;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    char *profileList[] = {"Off", "Music" ,"Movie", "Sports" , "Entertainment", "Night", "Party" , "User"};
    dsAudioPortType_t port = dsAUDIOPORT_TYPE_MAX;
    int32_t portIndex = 0;
    int32_t profileSize = sizeof(profileList)/sizeof(char *);
    char profile[DS_AUDIO_MAX_MS12_LENGTH]  = {0};

    if(dsAudio_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    handle = dsAudio_getPort(port, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("MS12 Profile");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#  %-20s","MS12 Profile");
    for (int i = 0; i < profileSize; i++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-20s", i, profileList[i]);
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");

    UT_LOG_MENU_INFO("Select MS12 Profile: ");
    readInt(&choice);
    if(choice < 0 || choice >= profileSize)
    {
        UT_LOG_ERROR("Invalid profile choice");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetMS12AudioProfile(IN:handle:[0x%0X], IN:profile:[%s])",
                 handle, profileList[choice-1]);

    ret = dsSetMS12AudioProfile(handle, profileList[choice]);

    UT_LOG_INFO("Result dsSetMS12AudioProfile(IN:handle:[0x%0X], IN:profile:[%s]) dsError_t:[%s]",
                handle, profileList[choice-1],
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetMS12AudioProfile(IN:handle:[0x%0X], OUT:profile:[])", handle);

    ret = dsGetMS12AudioProfile(handle, profile);

    UT_LOG_INFO("Result dsGetMS12AudioProfile(IN:handle:[0x%0X], OUT:profile:[%s]) dsError_t:[%s]",
                handle, profile,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(!strcmp(profile, profileList[choice]));

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets associate audio mixing.
*
* This test function sets associate audio mixing.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 016@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_associate_audio_mixing(void)
{
    gTestID = 16;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    int32_t choice = -1;
    bool mixing = false;
    bool mixing_g = false;

    UT_LOG_MENU_INFO("Enable/Disable Associated Audio Mixing[1:Enable, 2:Disable]: ");
    readInt(&choice);
    if(choice < 1 || choice > 2)
    {
        UT_LOG_ERROR("Invalid Associated Audio Mixing mode");
        goto exit;
    }
    mixing = (choice == 1) ? true : false;

    UT_LOG_INFO("Calling dsSetAssociatedAudioMixing(IN:handle:[0x%0X], IN:mixing:[%s])",
                handle,
                UT_Control_GetMapString(bool_mapTable, mixing));

    ret = dsSetAssociatedAudioMixing(handle, mixing);

    UT_LOG_INFO("Result dsSetAssociatedAudioMixing(IN:handle:[0x%0X], IN:mixing:[%s]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(bool_mapTable, mixing),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Calling dsGetAssociatedAudioMixing(IN:handle:[0x%0X], OUT:mixing:[])", handle);

    ret = dsGetAssociatedAudioMixing(handle, &mixing_g);

    UT_LOG_INFO("Result dsGetAssociatedAudioMixing(IN:handle:[0x%0X], OUT:mixing:[%s]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(bool_mapTable, mixing_g),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);
    DS_ASSERT(mixing == mixing_g);

    if(mixing == true)
    {
        int32_t mixerbalance = 0, mixerbalance_g = 0;

        UT_LOG_MENU_INFO("Set Fader Control[-32(mute associated audio) to 32(mute main audio)]: ");
        readInt(&mixerbalance);
        if(mixerbalance < -32 || mixerbalance > 32)
        {
            UT_LOG_ERROR("Invalid Fader Control");
            goto exit;
        }

        UT_LOG_INFO("Calling dsSetFaderControl(IN:handle:[0x%0X], IN:mixerbalance:[%d])",
                    handle, mixerbalance);

        ret = dsSetFaderControl(handle, mixerbalance);

        UT_LOG_INFO("Result dsSetFaderControl(IN:handle:[0x%0X], IN:mixerbalance:[%d]) dsError_t:[%s]",
                    handle, mixerbalance,
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);

        UT_LOG_INFO("Calling dsGetFaderControl(IN:handle:[0x%0X], OUT:mixerbalance:[])", handle);

        ret = dsGetFaderControl(handle, &mixerbalance_g);

        UT_LOG_INFO("Result dsGetFaderControl(IN:handle:[0x%0X], OUT:mixerbalance:[%d]) dsError_t:[%s]",
                    handle, mixerbalance_g,
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);
        DS_ASSERT(mixerbalance == mixerbalance_g);
    } //if(mixing == true)

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets audio mixer levels.
*
* This test function sets audio mixer levels.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 017@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_audio_mixerlevels(void)
{
    gTestID = 17;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    int32_t volume = 0;
    int32_t aInput = 0;

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("Mixer Input");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#  %-20s","Mixer Input");
    for(int32_t i = dsAUDIO_INPUT_PRIMARY ; i < dsAUDIO_INPUT_MAX; i++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-20s", i,
                     UT_Control_GetMapString(dsAudioInput_mapTable, i));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");

    UT_LOG_MENU_INFO("Select Mixer Input: ");
    readInt(&aInput);
    if(aInput < dsAUDIO_INPUT_PRIMARY || aInput > dsAUDIO_INPUT_MAX-1)
    {
        UT_LOG_MENU_INFO("Invalid mixer input");
        goto exit;
    }
    UT_LOG_MENU_INFO("Set the Volume[0 to 100]: ");
    readInt(&volume);
    if(volume < 0 || volume > 100)
    {
        UT_LOG_ERROR("Invalid volume");
        goto exit;
    }

    UT_LOG_INFO("Calling dsSetAudioMixerLevels(IN:handle:[0x%0X], IN:aInput:[%s], IN:volume[%d])",
                 handle,
                 UT_Control_GetMapString(dsAudioInput_mapTable, aInput), volume);

    ret = dsSetAudioMixerLevels (handle, (dsAudioInput_t)aInput, volume);

    UT_LOG_INFO("Result dsSetAudioMixerLevels(IN:handle:[0x%0X], IN:aInput:[%s], IN:volume[%d]) dsError_t:[%s]",
                handle,
                UT_Control_GetMapString(dsAudioInput_mapTable, aInput), volume,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets audio language.
*
* This test function sets audio language.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 018@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_set_language(void)
{
    gTestID = 18;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    int32_t language_type = 0;
    char language[4] = {0};
    char language_g[4] = {0};

    UT_LOG_MENU_INFO("Select the Language Type[1: Primary, 2: Secondary]: ");
    readInt(&language_type);
    if(language_type < 1 || language_type > 2)
    {
        UT_LOG_ERROR("Invalid language type");
        goto exit;
    }

    UT_LOG_MENU_INFO("Enter 3 letter long language as per ISO 639-3: ");
    readString(language);
    if(language_type == 1)
    {
        UT_LOG_INFO("Calling dsSetPrimaryLanguage(IN:handle:[0x%0X], IN:pLang:[%s])",
                    handle, language);

        ret = dsSetPrimaryLanguage(handle, language);

        UT_LOG_INFO("Result dsSetPrimaryLanguage(IN:handle:[0x%0X], IN:pLang:[%s]) dsError_t:[%s]",
                    handle, language,
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);

        UT_LOG_INFO("Calling dsGetPrimaryLanguage(IN:handle:[0x%0X], OUT:pLang:[])", handle);

        ret = dsGetPrimaryLanguage(handle, language_g);

        UT_LOG_INFO("Result dsGetPrimaryLanguage(IN:handle:[0x%0X], OUT:pLang:[%s]) dsError_t:[%s]",
                    handle, language_g,
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);
        DS_ASSERT(!strcmp(language, language_g));
    }
    else
    {
        UT_LOG_INFO("Calling dsSetSecondaryLanguage(IN:handle:[0x%0X], IN:pLang:[%s])",
                    handle, language);

        ret = dsSetSecondaryLanguage(handle, language);

        UT_LOG_INFO("Result dsSetSecondaryLanguage(IN:handle:[0x%0X], IN:pLang:[%s]) dsError_t:[%s]",
                    handle, language,
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);

        UT_LOG_INFO("Calling dsGetSecondaryLanguage(IN:handle:[0x%0X], OUT:pLang:[])", handle);

        ret = dsGetSecondaryLanguage(handle, language_g);

        UT_LOG_INFO("Result dsGetSecondaryLanguage(IN:handle:[0x%0X], OUT:pLang:[%s]) dsError_t:[%s]",
                    handle, language_g,
                    UT_Control_GetMapString(dsError_mapTable, ret));

        DS_ASSERT(ret == dsERR_NONE);
        DS_ASSERT(!strcmp(language, language_g));
    }

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test gets arc type of connected device.
*
* This test function gets arc type of connected device.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 019@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_getArc_type(void)
{
    gTestID = 19;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    int32_t types   = 0;
    bool enabled = false;
    int32_t portIndex = 0;

    handle = dsAudio_getPort(dsAUDIOPORT_TYPE_HDMI_ARC, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_INFO("Calling dsGetSupportedARCTypes(IN:handle:[0x%0X], OUT:types:[])", handle);

    ret = dsGetSupportedARCTypes(handle, &types);

    UT_LOG_INFO("Result dsGetSupportedARCTypes(IN:handle:[0x%0X], OUT:types:[%s]) dsError_t:[%s]",
                handle, UT_Control_GetMapString(dsAudioARCTypes_mapTable, types),
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test sets arc SAD.
*
* This test function sets arc SAD.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 020@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_setSAD(void)
{
    gTestID = 20;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = (intptr_t)NULL;
    bool enabled = false;
    dsAudioSADList_t sad_list = {0};
    int32_t portIndex = 0;

    handle = dsAudio_getPort(dsAUDIOPORT_TYPE_HDMI_ARC, portIndex, &enabled);

    if(enabled == false)
    {
        UT_LOG_WARNING("Port is not enabled");
        goto exit;
    }

    UT_LOG_MENU_INFO("Enter SAD List count[1-15]: ");
    readInt(&sad_list.count);
    if(sad_list.count < 1 || sad_list.count > 15)
    {
        UT_LOG_ERROR("Invalid SAD Count");
        goto exit;
    }

    for(int32_t i = 0; i < sad_list.count; i++)
    {
        UT_LOG_MENU_INFO("Enter %d SAD Value: ", i);
        readInt(&sad_list.sad[i]);
    }

    UT_LOG_INFO("Calling dsAudioSetSAD(IN:handle:[0x%0X], IN:sad_list:[count: %d])",
                 handle, sad_list.count);

    ret = dsAudioSetSAD(handle, sad_list);

    UT_LOG_INFO("Result dsAudioSetSAD(IN:handle:[0x%0X], IN:sad_list:[count: %d]) dsError_t:[%s]",
                handle, sad_list.count,
                UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test terminates the dsAudio.
*
* This test function terminates the dsAudio.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 021@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L3_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L3_Low-Level_TestSpecification.md)
*/
void test_l3_dsAudio_terminate(void)
{
    gTestID = 21;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;

    UT_LOG_INFO("Calling dsAudioPortTerm()");

    ret = dsAudioPortTerm();

    UT_LOG_INFO("Result dsAudioPortTerm() dsError_t:[%s]", UT_Control_GetMapString(dsError_mapTable, ret));

    DS_ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int32_t - 0 on success, otherwise failure
 */

int32_t test_l3_dsAudio_register(void)
{
    // Create the test suite for source type
    pSuite = UT_add_suite_withGroupID("[L3 dsAudio]", NULL, NULL,UT_TESTS_L3);
    if (pSuite == NULL)
    {
        UT_LOG_ERROR("Failed to create the test suite");
        return -1;
    }

    UT_add_test(pSuite, "Initialize dsAudio", test_l3_dsAudio_initialize);
    UT_add_test(pSuite, "Enable Audio Port", test_l3_dsAudio_enable_port);
    UT_add_test(pSuite, "Disable Audio Port", test_l3_dsAudio_disable_port);
    UT_add_test(pSuite, "Headphone Connection", test_l3_dsAudio_headphone_connection);
    UT_add_test(pSuite, "Audio Compression", test_l3_dsAudio_audio_compression);
    UT_add_test(pSuite, "MS12 DAP Features", test_l3_dsAudio_ms12_dap);
    UT_add_test(pSuite, "Set Stereo Mode", test_l3_dsAudio_stereo_mode);
    UT_add_test(pSuite, "Enable/Disable Stereo Auto", test_l3_dsAudio_stereo_auto_mode);
    UT_add_test(pSuite, "Set Audio Level", test_l3_dsAudio_set_audio_level);
    UT_add_test(pSuite, "Set Audio Gain For Speaker", test_l3_dsAudio_set_audio_gain);
    UT_add_test(pSuite, "Audio Mute/UnMute", test_l3_dsAudio_set_audio_mute);
    UT_add_test(pSuite, "Set Audio Delay", test_l3_dsAudio_set_audio_delay);
    UT_add_test(pSuite, "Get Audio Format", test_l3_dsAudio_get_audio_format);
    UT_add_test(pSuite, "Set ATMOS Output Mode", test_l3_dsAudio_set_atmos_mode);
    UT_add_test(pSuite, "Get ATMOS Capabilities", test_l3_dsAudio_getConnected_atmoscaps);
    UT_add_test(pSuite, "Set MS12 Profiles", test_l3_dsAudio_ms12Profile);
    UT_add_test(pSuite, "Set Associate Audio Mixing", test_l3_dsAudio_set_associate_audio_mixing);
    UT_add_test(pSuite, "Set Audio Mixer Levels", test_l3_dsAudio_set_audio_mixerlevels);
    UT_add_test(pSuite, "Primary/Secondary Language", test_l3_dsAudio_set_language);
    UT_add_test(pSuite, "Get ARC Type", test_l3_dsAudio_getArc_type);
    UT_add_test(pSuite, "Set SAD List", test_l3_dsAudio_setSAD);
    UT_add_test(pSuite, "Terminate dsAudio", test_l3_dsAudio_terminate);

    return 0;
}

/** @} */ // End of DS_Audio_HALTEST_L3
/** @} */ // End of DS_Audio_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
