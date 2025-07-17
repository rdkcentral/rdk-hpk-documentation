
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
 * @defgroup DS_Display_HALTEST Device Settings Display HAL Tests
 * @{
 */

/**
 * @defgroup DS_Display_HALTEST_L3 Device Settings Display HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for DS Display HAL :
 *
 * Level 3 unit test cases for all APIs of Device Settings Display HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-display_halSpec.md](../../docs/pages/ds-display_halSpec.md)
 *
 * @endparblock
 */

/**
 * @file test_l3_dsDisplay.c
 *
 */

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include "stdint.h"
#include "dsDisplay.h"
#include "test_parse_configuration.h"

#define UT_LOG_MENU_INFO UT_LOG_INFO
#define DS_ASSERT assert
#define EDID_MAX_DATA_SIZE 256
#define DS_VIDEO_MAX_INDEX 10

/* Global Variables */

static int32_t gTestGroup = 3;
static int32_t gTestID    = 1;
static intptr_t gDisplayHandle = 0;
static dsDisplayEDID_t gEdid = {0};
static uint8_t gEdidBuffer[EDID_MAX_DATA_SIZE];

/* Enum mapping tables */

/* dsError_t */
const static ut_control_keyStringMapping_t dsDisplayError_mapTable[] = {
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

/* dsVideoPortType_t */
const static ut_control_keyStringMapping_t dsVideoPortType_mapTable[] = {
    {"dsVIDEOPORT_TYPE_RF", (int32_t)dsVIDEOPORT_TYPE_RF},
    {"dsVIDEOPORT_TYPE_BB", (int32_t)dsVIDEOPORT_TYPE_BB},
    {"dsVIDEOPORT_TYPE_SVIDEO", (int32_t)dsVIDEOPORT_TYPE_SVIDEO},
    {"dsVIDEOPORT_TYPE_1394", (int32_t)dsVIDEOPORT_TYPE_1394},
    {"dsVIDEOPORT_TYPE_DVI", (int32_t)dsVIDEOPORT_TYPE_DVI},
    {"dsVIDEOPORT_TYPE_COMPONENT", (int32_t)dsVIDEOPORT_TYPE_COMPONENT},
    {"dsVIDEOPORT_TYPE_HDMI", (int32_t)dsVIDEOPORT_TYPE_HDMI},
    {"dsVIDEOPORT_TYPE_HDMI_INPUT", (int32_t)dsVIDEOPORT_TYPE_HDMI_INPUT},
    {"dsVIDEOPORT_TYPE_INTERNAL", (int32_t)dsVIDEOPORT_TYPE_INTERNAL},
    {"dsVIDEOPORT_TYPE_MAX", (int32_t)dsVIDEOPORT_TYPE_MAX},
    {NULL, -1}
};

/* dsDisplayEvent_t */
const static ut_control_keyStringMapping_t dsDisplayEvent_mapTable[] = {
    {"dsDISPLAY_EVENT_CONNECTED", (int32_t)dsDISPLAY_EVENT_CONNECTED},
    {"dsDISPLAY_EVENT_DISCONNECTED", (int32_t)dsDISPLAY_EVENT_DISCONNECTED},
    {"dsDISPLAY_RXSENSE_ON", (int32_t)dsDISPLAY_RXSENSE_ON},
    {"dsDISPLAY_RXSENSE_OFF", (int32_t)dsDISPLAY_RXSENSE_OFF},
    {"dsDISPLAY_HDCPPROTOCOL_CHANGE", (int32_t)dsDISPLAY_HDCPPROTOCOL_CHANGE},
    {"dsDISPLAY_EVENT_MAX", (int32_t)dsDISPLAY_EVENT_MAX},
    {NULL, -1}
};

/* dsVideoAspectRatio_t */
const static ut_control_keyStringMapping_t dsVideoAspectRatio_mapTable[] = {
    {"dsVIDEO_ASPECT_RATIO_4x3", (int32_t)dsVIDEO_ASPECT_RATIO_4x3},
    {"dsVIDEO_ASPECT_RATIO_16x9", (int32_t)dsVIDEO_ASPECT_RATIO_16x9},
    {"dsVIDEO_ASPECT_RATIO_MAX", (int32_t)dsVIDEO_ASPECT_RATIO_MAX},
    {NULL, -1}
};

/* bool */
const static ut_control_keyStringMapping_t bool_mapTable[] =
{
    {"false", (int32_t) false},
    {"true", (int32_t) true},
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

/**
 * @brief Reads an integer input.
 *
 * Reads an integer input from the standard input stream.
 */
static void readInt(int32_t *choice)
{
    scanf("%d", choice);
    readAndDiscardRestOfLine(stdin);
}

/**
 * @brief Callback function for display events.
 *
 * This function is invoked when a display event occurs, providing information about the event type.
 */
static void displayEventCallback(int32_t handle, dsDisplayEvent_t event, void* eventData)
{
    UT_LOG_INFO("Display EventCallback(IN:handle:[%d], dsDisplayEvent_t:[%s]", 
                handle, 
                UT_Control_GetMapString(dsDisplayEvent_mapTable, event));

    if (eventData != NULL) 
    {
        char* eventDataStr = (char*)eventData;
        UT_LOG_INFO("Event Data: %s", eventDataStr);
    } 
    else 
    {
        UT_LOG_INFO("No Event Data provided");
    }
}

static int32_t dsDisplay_list_select_ports(dsVideoPortType_t *pVideoPort, int32_t *pIndex)
{
    int32_t choice = -1;

    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t\tdsVideo Port");
    UT_LOG_MENU_INFO("----------------------------------------------------------");
    UT_LOG_MENU_INFO("\t#   %-30s","Video Port");
    for(int32_t i = dsVIDEOPORT_TYPE_RF; i < dsVIDEOPORT_TYPE_MAX; i++)
    {
        UT_LOG_MENU_INFO("\t%d.  %-30s", i, UT_Control_GetMapString(dsVideoPortType_mapTable, i));
    }
    UT_LOG_MENU_INFO("----------------------------------------------------------");

    UT_LOG_MENU_INFO("Select dsVideo Port: ");
    readInt(&choice);
    if(choice < dsVIDEOPORT_TYPE_RF || choice >= dsVIDEOPORT_TYPE_MAX)
    {
        UT_LOG_ERROR("Invalid Port choice");
        return -1;
    }

    *pVideoPort = choice;

    UT_LOG_MENU_INFO("Select dsVideo Port Index[0-%d]: ", DS_VIDEO_MAX_INDEX);
    readInt(&choice);
    if(choice < 0 || choice > DS_VIDEO_MAX_INDEX)
    {
        UT_LOG_ERROR("Invalid Port choice");
        return -1;
    }

    *pIndex = choice;

    return 0;
}

/**
 * @brief This test initializes the dsDisplay Module.
 *
 * This test function initializes the dsDisplay Module.
 *
 * **Test Group ID:** 03@n
 * **Test Case ID:** 001@n
 *
 * **Test Procedure:**
 * Refer to Test specification documentation
 * [ds-display_L3_Low-Level_TestSpecification.md](../../docs/pages/ds-display_L3_Low-Level_TestSpecification.md)
 */
void test_l3_dsDisplay_initialize(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status   = dsERR_NONE;

    UT_LOG_INFO("Calling dsDisplayInit()");

    status = dsDisplayInit();

    UT_LOG_INFO("Result dsDisplayInit() dsError_t=[%s]", 
                UT_Control_GetMapString(dsDisplayError_mapTable, status));

    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);

}

/**
 * @brief Retrieves the handle of the connected display device.
 *
 * This test function retrieves the handle of the connected display device
 * corresponding to the specified video port and index. It also registers
 * a display event callback for the retrieved handle.
 *
 * **Test Group ID:** 03@n
 * **Test Case ID:** 002@n
 *
 * **Test Procedure:**
 * Refer to Test specification documentation
 * [ds-display_L3_Low-Level_TestSpecification.md](../../docs/pages/ds-display_L3_Low-Level_TestSpecification.md)
 */
void test_l3_dsDisplay_select_display_port(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;
    int32_t ret;
    int32_t portIndex = 0;
    dsVideoPortType_t port = dsVIDEOPORT_TYPE_MAX;

    if(dsDisplay_list_select_ports(&port, &portIndex))
    {
        goto exit;
    }

    UT_LOG_INFO("Calling dsGetDisplay(IN:vType:[%s], IN:index:[%d], OUT:handle:[])",
                UT_Control_GetMapString(dsVideoPortType_mapTable, port), portIndex);

    status = dsGetDisplay(port, portIndex, &gDisplayHandle);

    UT_LOG_INFO("Result dsGetDisplay(IN:vType:[%s], IN:index:[%d], OUT:handle:[0x%0X]) dsError_t:[%s]",
                UT_Control_GetMapString(dsVideoPortType_mapTable, port), portIndex,
                gDisplayHandle,
                UT_Control_GetMapString(dsDisplayError_mapTable, status));

    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_MENU_INFO("Calling dsRegisterDisplayEventCallback");

    UT_LOG_INFO("Calling dsRegisterDisplayEventCallback(IN:handle:[0x%0X], IN:cb[0x%0X]) ", gDisplayHandle, displayEventCallback);

    ret = dsRegisterDisplayEventCallback(gDisplayHandle, displayEventCallback);

    UT_LOG_INFO("Result dsRegisterDisplayEventCallback(IN:handle:[0x%0X], IN:cb[0x%0X]), dsError_t=[%s]", 
                gDisplayHandle, 
                displayEventCallback,
                UT_Control_GetMapString(dsDisplayError_mapTable, ret));

    DS_ASSERT(status == dsERR_NONE);

exit:
    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Retrieves Extended Display Identification Data (EDID) for the display.
 *
 * Retrieves the EDID information for the display associated with the selected handle.
 *
 * **Test Group ID:** 03@n
 * **Test Case ID:** 003@n
 *
 * **Test Procedure:**
 * Refer to Test specification documentation
 * [ds-display_L3_Low-Level_TestSpecification.md](../../docs/pages/ds-display_L3_Low-Level_TestSpecification.md)
 */
void test_l3_dsDisplay_get_edid(void) 
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;

    UT_LOG_INFO("Calling dsGetEDID(IN:handle:[0x%0X], OUT:edid:[])", gDisplayHandle);

    status = dsGetEDID(gDisplayHandle, &gEdid);

    UT_LOG_INFO("Result dsGetEDID(dsDisplayEDID_t(productCode:[%d], serialNumber:[%d], manufactureYear:[%d],manufactureWeek:[%d],hdmiDeviceType:[%s], isRepeater:[%s])",
                gEdid.productCode, gEdid.serialNumber, 
                gEdid.manufactureYear, gEdid.manufactureWeek, 
                gEdid.hdmiDeviceType?"HDMI":"DVI", UT_Control_GetMapString(bool_mapTable, gEdid.isRepeater));
    UT_LOG_INFO("Result dsGetEDID(dsDisplayEDID_t(physicalAddress:[%u.%u.%u.%u], numOfSupportedResolution:[%d], monitorName:[%s])",
                gEdid.physicalAddressA, gEdid.physicalAddressB, 
                gEdid.physicalAddressC, gEdid.physicalAddressD, 
                gEdid.numOfSupportedResolution, gEdid.monitorName);

    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
 * @brief Retrieves the Extended Display Identification Data (EDID) bytes for the display.
 *
 * Retrieves the EDID data bytes for the display associated with the selected handle.
 *
 * **Test Group ID:** 03@n
 * **Test Case ID:** 004@n
 *
 * **Test Procedure:**
 * Refer to Test specification documentation
 * [ds-display_L3_Low-Level_TestSpecification.md](../../docs/pages/ds-display_L3_Low-Level_TestSpecification.md)
 */
void test_l3_dsDisplay_get_edidbytes(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;
    int32_t length = 0;

    UT_LOG_INFO("Calling dsGetEDIDBytes(IN:Handle:[0x%0X], OUT:EDID:[], OUT:Length:[])", gDisplayHandle);

    status = dsGetEDIDBytes(gDisplayHandle, gEdidBuffer, &length);

    UT_LOG_INFO("Result dsGetEDIDBytes(IN:Handle:[0x%0X], OUT:EDID:[0x%0X], OUT:Length:[%d]) dsError_t=[%s]",
                gDisplayHandle, gEdidBuffer, 
                length, UT_Control_GetMapString(dsDisplayError_mapTable, status));

    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("EDID Bytes:");

    for (int32_t i = 0; i < length; i++)
    {
	UT_LOG_INFO("edidbyte[%d]:[%02X] ", i, gEdidBuffer[i]);
    }

    UT_LOG_INFO("Out %s", __FUNCTION__);

}

/**
 * @brief Retrieves the aspect ratio of the display.
 *
 * Retrieves the aspect ratio of the display.
 *
 * **Test Group ID:** 03@n
 * **Test Case ID:** 005@n
 *
 * **Test Procedure:**
 * Refer to Test specification documentation
 * [ds-display_L3_Low-Level_TestSpecification.md](../../docs/pages/ds-display_L3_Low-Level_TestSpecification.md)
 */
void test_l3_dsDisplay_get_aspectratio(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status   = dsERR_NONE;
    dsVideoAspectRatio_t displayAspectRatio = dsVIDEO_ASPECT_RATIO_MAX;

    UT_LOG_INFO("Calling dsGetDisplayAspectRatio(IN:handle:[0x%0X], OUT:aspectRatio:[]) ", gDisplayHandle);

    status = dsGetDisplayAspectRatio(gDisplayHandle, &displayAspectRatio);

    UT_LOG_INFO("Result dsGetDisplayAspectRatio(handle:[0x%0X], dsVideoAspectRatio_t:[%s], dsError_t=[%s])",
                gDisplayHandle,
                UT_Control_GetMapString(dsVideoAspectRatio_mapTable, displayAspectRatio), 
                UT_Control_GetMapString(dsDisplayError_mapTable, status));

    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);

}

/**
 * @brief This test terminates the dsDisplay Module.
 *
 * This test function terminates the dsDisplay Module.
 *
 * **Test Group ID:** 03@n
 * **Test Case ID:** 006@n
 *
 * **Test Procedure:**
 * Refer to Test specification documentation
 * [ds-display_L3_Low-Level_TestSpecification.md](../../docs/pages/ds-display_L3_Low-Level_TestSpecification.md)
 */
void test_l3_dsDisplay_terminate(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t status = dsERR_NONE;

    UT_LOG_INFO("Calling dsDisplayTerm()");

    status = dsDisplayTerm();

    UT_LOG_INFO("Result dsDisplayTerm() dsError_t=[%s]", 
                UT_Control_GetMapString(dsDisplayError_mapTable, status));

    DS_ASSERT(status == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l3_dsDisplay_register (void)
{
    /* add a suite to the registry */
    pSuite = UT_add_suite( "[L3 dsDisplay]", NULL, NULL );
    if ( NULL == pSuite)
    {
        return -1;
    }

    UT_add_test( pSuite, "Initialize dsDisplay", test_l3_dsDisplay_initialize);
    UT_add_test( pSuite, "Select Display Port", test_l3_dsDisplay_select_display_port);
    UT_add_test( pSuite, "Get display EDID", test_l3_dsDisplay_get_edid);
    UT_add_test( pSuite, "Get display EDIDBytes", test_l3_dsDisplay_get_edidbytes);
    UT_add_test( pSuite, "Get display AspectRatio",test_l3_dsDisplay_get_aspectratio);
    UT_add_test( pSuite, "Terminate dsDisplay", test_l3_dsDisplay_terminate);

    return 0;
}


/** @} */ // End of DS_Display_HALTEST_L3
/** @} */ // End of DS_Display_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
