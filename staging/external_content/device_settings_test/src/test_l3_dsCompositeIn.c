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
 * @defgroup DS_CompositeIn_HALTEST Device Settings Composite Input HAL Tests
 * @{
 */

/**
 * @defgroup DS_CompositeIn_HALTEST_L3 Device Settings Composite Input HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for DS Composite Input HAL :
 *
 * Level 3 test cases for all APIs of Device Settings Composite Input HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-composite-in_halSpec.md](../../docs/pages/ds-composite-in_halSpec.md)
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

#include "test_parse_configuration.h"
#include "dsCompositeIn.h"

#define ASSERT assert

/* Global Variables */
static int32_t gTestGroup = 3;
static int32_t gTestID    = 1;

static bool                    gConnectionStatus = false;
static dsCompInSignalStatus_t  gSignalStatus  = dsCOMP_IN_SIGNAL_STATUS_NONE;
static dsCompositeInStatus_t        gStatusChange;
static dsVideoResolution_t gVideoResolution = dsVIDEO_PIXELRES_720x480;
static dsVideoFrameRate_t gFrameRate = dsVIDEO_FRAMERATE_UNKNOWN;

/* Mapping table for dsError_t */
const static ut_control_keyStringMapping_t dsError_mapTable [] = {
  { "dsERR_NONE",                    (int32_t)dsERR_NONE                    },
  { "dsERR_GENERAL",                 (int32_t)dsERR_GENERAL                 },
  { "dsERR_INVALID_PARAM",           (int32_t)dsERR_INVALID_PARAM           },
  { "dsERR_INVALID_STATE",           (int32_t)dsERR_INVALID_STATE           },
  { "dsERR_ALREADY_INITIALIZED",     (int32_t)dsERR_ALREADY_INITIALIZED     },
  { "dsERR_NOT_INITIALIZED",         (int32_t)dsERR_NOT_INITIALIZED         },
  { "dsERR_OPERATION_NOT_SUPPORTED", (int32_t)dsERR_OPERATION_NOT_SUPPORTED },
  { "dsERR_RESOURCE_NOT_AVAILABLE",  (int32_t)dsERR_RESOURCE_NOT_AVAILABLE  },
  { "dsERR_OPERATION_FAILED",        (int32_t)dsERR_OPERATION_FAILED        },
  { "dsErr_MAX",                     (int32_t)dsErr_MAX                     },
  {  NULL, -1 }
};

/* Mapping table for dsCompInSignalStatus_t */
const static ut_control_keyStringMapping_t dsCompInSignalStatusMappingTable[] = {
    {"dsCOMP_IN_SIGNAL_STATUS_NONE", (int32_t)dsCOMP_IN_SIGNAL_STATUS_NONE},
    {"dsCOMP_IN_SIGNAL_STATUS_NOSIGNAL", (int32_t)dsCOMP_IN_SIGNAL_STATUS_NOSIGNAL},
    {"dsCOMP_IN_SIGNAL_STATUS_UNSTABLE", (int32_t)dsCOMP_IN_SIGNAL_STATUS_UNSTABLE},
    {"dsCOMP_IN_SIGNAL_STATUS_NOTSUPPORTED", (int32_t)dsCOMP_IN_SIGNAL_STATUS_NOTSUPPORTED},
    {"dsCOMP_IN_SIGNAL_STATUS_STABLE", (int32_t)dsCOMP_IN_SIGNAL_STATUS_STABLE},
    {"dsCOMP_IN_SIGNAL_STATUS_MAX", (int32_t)dsCOMP_IN_SIGNAL_STATUS_MAX},
    {NULL, -1}
};

/* Mapping table for dsVideoResolution_t */
const static ut_control_keyStringMapping_t dsCompInResolutionMappingTable[] = {
    {"dsVIDEO_PIXELRES_720x480", (int32_t)dsVIDEO_PIXELRES_720x480},
    {"dsVIDEO_PIXELRES_720x576", (int32_t)dsVIDEO_PIXELRES_720x576},
    {"dsVIDEO_PIXELRES_1280x720", (int32_t)dsVIDEO_PIXELRES_1280x720},
    {"dsVIDEO_PIXELRES_1366x768", (int32_t)dsVIDEO_PIXELRES_1366x768},
    {"dsVIDEO_PIXELRES_1920x1080", (int32_t)dsVIDEO_PIXELRES_1920x1080},
    {"dsVIDEO_PIXELRES_3840x2160", (int32_t)dsVIDEO_PIXELRES_3840x2160},
    {"dsVIDEO_PIXELRES_4096x2160", (int32_t)dsVIDEO_PIXELRES_4096x2160},
    {"dsVIDEO_PIXELRES_MAX", (int32_t)dsVIDEO_PIXELRES_MAX},
    {NULL, -1}
};

/* Mapping table for dsVideoFrameRate_t */
const static ut_control_keyStringMapping_t dsCompInFrameRateMappingTable[] = {
    {"dsVIDEO_FRAMERATE_UNKNOWN", (int32_t)dsVIDEO_FRAMERATE_UNKNOWN},
    {"dsVIDEO_FRAMERATE_24", (int32_t)dsVIDEO_FRAMERATE_24},
    {"dsVIDEO_FRAMERATE_25", (int32_t)dsVIDEO_FRAMERATE_25},
    {"dsVIDEO_FRAMERATE_30", (int32_t)dsVIDEO_FRAMERATE_30},
    {"dsVIDEO_FRAMERATE_60", (int32_t)dsVIDEO_FRAMERATE_60},
    {"dsVIDEO_FRAMERATE_23dot98", (int32_t)dsVIDEO_FRAMERATE_23dot98},
    {"dsVIDEO_FRAMERATE_29dot97", (int32_t)dsVIDEO_FRAMERATE_29dot97},
    {"dsVIDEO_FRAMERATE_50", (int32_t)dsVIDEO_FRAMERATE_50},
    {"dsVIDEO_FRAMERATE_59dot94", (int32_t)dsVIDEO_FRAMERATE_59dot94},
    {"dsVIDEO_FRAMERATE_MAX", (int32_t)dsVIDEO_FRAMERATE_MAX},
    {NULL, -1}
};

/* Mapping table for dsCompositeInPort_t */
const static ut_control_keyStringMapping_t dsCompositeInPortMappingTable[] = {
    {"dsCOMPOSITE_IN_PORT_NONE", (int32_t)dsCOMPOSITE_IN_PORT_NONE},
    {"dsCOMPOSITE_IN_PORT_0", (int32_t)dsCOMPOSITE_IN_PORT_0},
    {"dsCOMPOSITE_IN_PORT_1", (int32_t)dsCOMPOSITE_IN_PORT_1},
    {"dsCOMPOSITE_IN_PORT_MAX", (int32_t)dsCOMPOSITE_IN_PORT_MAX},
    {NULL, -1}
};

/* Mapping table for boolean values */
const static ut_control_keyStringMapping_t bool_mapTable [] = {
  { "false", (int32_t)false },
  { "true",  (int32_t)true  },
  {  NULL, -1 }
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
*
*This function read the input from the stdin.
*/
static void readInput(int *choice)
{
    scanf("%d",choice);
    readAndDiscardRestOfLine(stdin);
}

/**
 * @brief Callback function for Composite Input hot plug status.
 *
 * This function is invoked whenever a change occurs in the Composite Input connection.
 */
static void compositeInConnectCB(dsCompositeInPort_t Port, bool isPortConnected)
{
    UT_LOG_INFO("Received Connection status callback port: [%s], Connection: [%s]\n",
                 UT_Control_GetMapString(dsCompositeInPortMappingTable, Port),
                 UT_Control_GetMapString(bool_mapTable, isPortConnected));

    gConnectionStatus = isPortConnected;
}

/**
 * @brief Callback function for CompositeIn signal change.
 *
 * This function is invoked whenever a signal change occurs in the Comosite Input connection.
 */
static void compositeInSignalChangeCB(dsCompositeInPort_t port, dsCompInSignalStatus_t sigStatus)
{
    UT_LOG_INFO("Received SignalChange status callback port: [%s], sigstatus: [%s]\n",
                 UT_Control_GetMapString(dsCompositeInPortMappingTable, port),
                 UT_Control_GetMapString(dsCompInSignalStatusMappingTable, sigStatus));

    gSignalStatus = sigStatus;
}

/**
 * @brief Callback function for CompositeIn status change.
 *
 * This function is invoked whenever a status change occurs in the CompositeIn connection.
 */
static void compositeInStatusChangeCB(dsCompositeInStatus_t inputStatus)
{
    UT_LOG_INFO("Received statuschange callback isPresented: [%s], activeport: [%s]\n",
                 UT_Control_GetMapString(bool_mapTable, inputStatus.isPresented),
                 UT_Control_GetMapString(dsCompositeInPortMappingTable, inputStatus.activePort));

    for(int i = 0 ; i < dsCOMPOSITE_IN_PORT_MAX ; i++) 
    {
         if(!(inputStatus.isPresented))
                 continue;

         UT_LOG_INFO("Received statuschange callback isPortConnected: [%s], activeport: [%s]\n",
                 UT_Control_GetMapString(bool_mapTable, inputStatus.isPortConnected[i]),
                 UT_Control_GetMapString(dsCompositeInPortMappingTable, inputStatus.activePort));
    }

    gStatusChange = inputStatus;
}

/**
 * @brief Callback function for CompositeIn port video mode change.
 *
 * This function is invoked whenever a video mode change occurs in the Comosite Input video.
 */
static void compositeInVideoModeChangeCB(dsCompositeInPort_t port, dsVideoPortResolution_t videoResolution)
{
    UT_LOG_INFO("Received VideomodeChange callback port: [%s], videoResolution: [%s], videoFrameRate: [%s]\n",
                 UT_Control_GetMapString(dsCompositeInPortMappingTable, port),
                 UT_Control_GetMapString(dsCompInResolutionMappingTable, videoResolution.pixelResolution),
                 UT_Control_GetMapString(dsCompInFrameRateMappingTable, videoResolution.frameRate));

    gVideoResolution = videoResolution.pixelResolution;
    gFrameRate = videoResolution.frameRate;
}

/**
* @brief This test initializes the CompositeIn Module.
*
* This test function initializes the CompositeIn Module.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 001@n
*
*/
void test_l3_CompositeIn_initialize(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;

    /* Initialize the CompositeIn Module */
    UT_LOG_INFO("Calling dsCompositeInInit()");
    ret = dsCompositeInInit(); 
    UT_LOG_INFO("Result dsCompositeInInit() dsError_t:[%s]", UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    /* Register connection status callback */
    UT_LOG_INFO("Calling dsCompositeInRegisterConnectCB(IN:CBFunc:[0x%0X])", compositeInConnectCB);
    ret = dsCompositeInRegisterConnectCB(compositeInConnectCB);
    UT_LOG_INFO("Result dsCompositeInRegisterConnectCB(IN:CBFunc:[0x%0X]) dsError_t:[%s]",
                        compositeInConnectCB, UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    /* Register Signal change callback */
    UT_LOG_INFO("Calling dsCompositeInRegisterSignalChangeCB(IN:CBFunc:[0x%0X])", compositeInSignalChangeCB);
    ret = dsCompositeInRegisterSignalChangeCB(compositeInSignalChangeCB);
    UT_LOG_INFO("Result dsCompositeInRegisterSignalChangeCB(IN:CBFunc:[0x%0X]) dsError_t:[%s]",
                        compositeInSignalChangeCB, UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    /* Register Status change callback */
    UT_LOG_INFO("Calling dsCompositeInRegisterStatusChangeCB(IN:CBFunc:[0x%0X])", compositeInStatusChangeCB);
    ret = dsCompositeInRegisterStatusChangeCB(compositeInStatusChangeCB);
    UT_LOG_INFO("Result dsCompositeInRegisterStatusChangeCB(IN:CBFunc:[0x%0X]) dsError_t:[%s]",
                        compositeInStatusChangeCB, UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    /* Register videomode change callback */
    UT_LOG_INFO("Calling dsCompositeInRegisterVideoModeUpdateCB(IN:CBFunc:[0x%0X])", compositeInVideoModeChangeCB);
    ret = dsCompositeInRegisterVideoModeUpdateCB(compositeInVideoModeChangeCB);
    UT_LOG_INFO("Result dsCompositeInRegisterVideoModeUpdateCB(IN:CBFunc:[0x%0X]) dsError_t:[%s]",
                        compositeInVideoModeChangeCB, UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test get status of CompositeIn ports.
*
* This test function gets status of CompositeIn ports on platform.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 002n
*
*/
void test_l3_CompositeIn_get_status(void)
{
    gTestID = 2;
    UT_LOG_INFO("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    dsCompositeInStatus_t inputstatus;

    UT_LOG_INFO("Calling dsCompositeInGetStatus(OUT:inputstatus:[])");
    ret = dsCompositeInGetStatus(&inputstatus);
    UT_LOG_INFO("Result dsCompositeInGetStatus dsError_t:[%s]", UT_Control_GetMapString(dsError_mapTable, ret));
    for(int i = 0 ; i < dsCOMPOSITE_IN_PORT_MAX ; i++) 
    {
        UT_LOG_INFO("Result dsCompositeInGetStatus(OUT:inputstatus[isPresented:%s, isPortConnected:%s, activeport:%s])",
                UT_Control_GetMapString(bool_mapTable, inputstatus.isPresented),
                UT_Control_GetMapString(bool_mapTable, inputstatus.isPortConnected[i]),
                UT_Control_GetMapString(dsCompositeInPortMappingTable, inputstatus.activePort));
    }
    ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}


/**
* @brief This test selects the CompositeIn ports.
*
* This test function selects the CompositeIn ports on platform.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 003n
*
*/
void test_l3_CompositeIn_select_port(void)
{
    gTestID = 3;
    UT_LOG_INFO("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    dsCompositeInPort_t port = dsCOMPOSITE_IN_PORT_MAX;
    int32_t select = 0; 

    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("Available CompositeIn Ports");
    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\t#  %-20s","CompositeIn Port");
    for(int32_t i = 0; i < dsCOMPOSITE_IN_PORT_MAX; i++)
    {
        UT_LOG_INFO("\t%d.  %-20s", i, UT_Control_GetMapString(dsCompositeInPortMappingTable, i));
    }
    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("Enter the port to select: ");
    readInput(&select);

    if(select < 0 || select >= dsCOMPOSITE_IN_PORT_MAX) 
    {
       UT_LOG_ERROR("\nInvalid port selected\n");
       goto exit;
    }

    port = (dsCompositeInPort_t)select;

    UT_LOG_INFO("Calling dsCompositeInSelectPort(IN:port[%s])",
                UT_Control_GetMapString(dsCompositeInPortMappingTable, port));
    ret = dsCompositeInSelectPort(port);
    UT_LOG_INFO("Result dsCompositeInSelectPort(IN:port[%s] dsError_t:[%s])",
                UT_Control_GetMapString(dsCompositeInPortMappingTable, port), 
                UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test to scale video.
*
* This test function is to scale video of selected CompositeIn port on platform.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsCompositeIn_L3_Low-Level_TestSpecification.md](../docs/pages/ds-compositeIn_L3_Low-Level_TestSpecification.md)
*/
void test_l3_CompositeIn_scale_video(void)
{
    gTestID = 4;
    UT_LOG_INFO("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    int32_t x = 0 , y = 0, width = 0 , height = 0;

    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nAcceptable inputs for x coordinate:\t"
                     "Min is 0\t"
                     "Max is based on the current resolution\t");
    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nEnter the x coordinate to scale: ");
    readInput(&x);
    if(x < 0)
    {
        UT_LOG_ERROR("\n Invalid x coordinate selected \n");
        goto exit;
    }

    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nAcceptable inputs for y coordinate:\t"
                     "Min is 0\t"
                     "Max is based on the current resolution\t");
    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nEnter the y coordinate to scale: ");
    readInput(&y);
    if(y < 0)
    {
        UT_LOG_ERROR("\n Invalid y coordinate selected \n");
        goto exit;
    }

    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nAcceptable inputs for width coordinate:\t"
                     "Min is 0\t"
                     "Max is based on the current resolution\t");
    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nEnter the width coordinate to scale: ");
    readInput(&width);
    if(width < 0)
    {
        UT_LOG_ERROR("\n Invalid width selected \n");
        goto exit;
    }

    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nAcceptable inputs for height coordinate:\t"
                     "Min is 0\t"
                     "Max is based on the current resolution\t");
    UT_LOG_INFO("----------------------------------------------------------");
    UT_LOG_INFO("\nEnter the height coordinate to scale: ");
    readInput(&height);
    if(height < 0)
    {
        UT_LOG_ERROR("\n Invalid height selected \n");
        goto exit;
    }

    UT_LOG_INFO("Calling dsCompositeInScaleVideo(IN:x[%d], IN:y[%d], IN:width[%d], IN:height[%d])",
                x, y, width, height);
    ret = dsCompositeInScaleVideo(x, y, width, height);
    UT_LOG_INFO("Result : dsCompositeInScaleVideo(IN:x[%d], IN:y[%d], IN:width[%d], IN:height[%d]) dsError_t:[%s]",
                x, y , width, height, UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    exit:
        UT_LOG_INFO("Out %s", __FUNCTION__);
}

/**
* @brief This test terminates the dsCompositeIn.
*
* This test function terminates the dsCompositeIn.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 004@n
*
*/
void test_l3_dsCompositeIn_terminate(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;

    UT_LOG_INFO("Calling dsCompositeInTerm()");
    ret = dsCompositeInTerm();
    UT_LOG_INFO("Result dsCompositeInTerm() dsError_t:[%s]", UT_Control_GetMapString(dsError_mapTable, ret));
    ASSERT(ret == dsERR_NONE);

    UT_LOG_INFO("Out %s", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l3_dsCompositeIn_register(void)
{
    // Create the test suite for sink type
    pSuite = UT_add_suite_withGroupID("[L3 dsCompositeIn]", NULL, NULL, UT_TESTS_L3);
    ASSERT( pSuite != NULL );

    UT_add_test( pSuite, "Initialize CompositeIn" ,test_l3_CompositeIn_initialize );
    UT_add_test( pSuite, "Get status of ports" ,test_l3_CompositeIn_get_status );
    UT_add_test( pSuite, "Select port" ,test_l3_CompositeIn_select_port );
    UT_add_test( pSuite, "Scale the compositeIn video" ,test_l3_CompositeIn_scale_video );
    UT_add_test( pSuite, "Terminate CompositeIn" ,test_l3_dsCompositeIn_terminate );


    return 0;
}

/** @} */ // End of DS_CompositeIn_HALTEST_L3
/** @} */ // End of DS_CompositeIn_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK
