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
 * @defgroup DS_Audio_HALTEST_L2 Device Settings Audio HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for DS Audio HAL :
 *
 * Level 2 unit test cases for all APIs of Device Settings Audio HAL
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [ds-audio_halSpec.md](../../docs/pages/ds-audio_halSpec.md)
 *
 * @endparblock
 */


/**
 * @file test_l2_dsAudio.c
 *
 */

#include <ut.h>
#include <ut_log.h>

#include <ut_kvp_profile.h>
#include <stdlib.h>
#include "dsAudio.h"
#include "test_parse_configuration.h"

static int gTestGroup = 2;
static int gTestID    = 1;

/**
* @brief This test verifies the functionality of enabling, disabling and
*        verifying the status of an audio port.
*
* This test function initializes an audio port, enables it, verifies its status,
* disables it, and then verifies its status again. This is done for all types of
* audio ports and for all indices. The test ensures that the audio port can be
* successfully enabled and disabled, and that the status of the port accurately
* reflects these changes.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_EnableDisableAndVerifyAudioPortStatus(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret   = dsERR_NONE;
    intptr_t handle = 0;
    bool enabled    = false;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d",
                           gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);

        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsEnableAudioPort with handle: %p and enabled: true", handle);
        ret = dsEnableAudioPort(handle, true);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsEnableAudioPort failed with error: %d", ret);
        }

        UT_LOG_DEBUG("Invoking dsIsAudioPortEnabled with handle: %p", handle);
        ret = dsIsAudioPortEnabled(handle, &enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(enabled, true);
        if (ret != dsERR_NONE || !enabled) {
            UT_LOG_ERROR("dsIsAudioPortEnabled failed with error: %d or port is not enabled", ret);
        }

        UT_LOG_DEBUG("Invoking dsEnableAudioPort with handle: %p and enabled: false", handle);
        ret = dsEnableAudioPort(handle, false);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsEnableAudioPort failed with error: %d", ret);
        }

        UT_LOG_DEBUG("Invoking dsIsAudioPortEnabled with handle: %p", handle);
        ret = dsIsAudioPortEnabled(handle, &enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(enabled, false);
        if (ret != dsERR_NONE || enabled) {
            UT_LOG_ERROR("dsIsAudioPortEnabled failed with error: %d or port is not disabled", ret);
        }
    } //for (port)

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test checks the connection status of the headphone port on sink platform
*
* This test initializes the audio port, gets the audio port for each type, checks
* if the headphone port is connected, and finally terminates the audio port. The test
* is designed to verify the correct functioning of the dsAudio module's headphone
* connection status checking mechanism.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_CheckHeadphoneConnectionStatus_sink(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    bool isConnected;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        if(gDSAudioPortConfiguration[port].typeid != dsAUDIOPORT_TYPE_HEADPHONE) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d",
                           gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);

        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_LOG_DEBUG("Return status: %d, Handle: %p", ret, handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsAudioOutIsConnected with handle: %p", handle);
        ret = dsAudioOutIsConnected(handle, &isConnected);
        UT_LOG_DEBUG("Return status: %d, isConnected: %d", ret, isConnected);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(isConnected, false);
        if (ret != dsERR_NONE || isConnected) {
            UT_LOG_ERROR("dsAudioOutIsConnected failed with error %d or Audio out is connected", ret);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to retrieve and verify MS12 capabilities of the platform
*
* This test function is designed to retrieve and verify the MS12 capabilities
* of the audio sink. It initializes the audio port, gets the audio port with a
* specific type and index, retrieves the MS12 capabilities, and then terminates
* the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_RetrieveAndVerifyMS12Capabilities(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t retStatus;
    intptr_t handle;
    int capabilities;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    retStatus = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        if(gDSAudioPortConfiguration[port].ms12_capabilites == 0) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d",
                           gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);

        retStatus = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_LOG_DEBUG("Return status: %d, Handle: %p", retStatus, handle);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
        if (retStatus != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", retStatus);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetMS12Capabilities with handle: %p", handle);
        retStatus = dsGetMS12Capabilities(handle, &capabilities);
        UT_LOG_DEBUG("Return status: %d, Capabilities: %d", retStatus, capabilities);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
        if (retStatus != dsERR_NONE) {
            UT_LOG_ERROR("dsGetMS12Capabilities failed with error %d", retStatus);
        }
        UT_ASSERT_EQUAL(capabilities, gDSAudioPortConfiguration[port].ms12_capabilites);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    retStatus = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of setting and getting
* audio compression for different audio port types.
*
* In this test, the dsAudioPortInit function is first invoked to initialize
* the audio port. Then, for each audio port type, the dsGetAudioPort function
* is invoked to get the handle of the audio port. For each compression level
* from 0 to 10, the dsSetAudioCompression function is invoked to set the
* audio compression level, and the dsGetAudioCompression function is invoked
* to get the set compression level. The set and got compression levels are then
* compared to verify if they are equal. Finally, the dsAudioPortTerm function is
* invoked to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetAudioCompression(void)
{
    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int compression, getCompression;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);

        if(gDSAudioPortConfiguration[port].no_of_supported_compression == 0) {
            continue;
        }

        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error: %d", ret);
            continue;
        }

        for (compression = 0; compression < gDSAudioPortConfiguration[port].no_of_supported_compression; compression++) {
            UT_LOG_DEBUG("Invoking dsSetAudioCompression with handle: %p and compression: %d",
                                handle, gDSAudioPortConfiguration[port].supported_compressions[compression]);
            ret = dsSetAudioCompression(handle, gDSAudioPortConfiguration[port].supported_compressions[compression]);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetAudioCompression failed with error: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetAudioCompression with handle: %p", handle);
            ret = dsGetAudioCompression(handle, &getCompression);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetAudioCompression failed with error: %d", ret);
            }

            UT_LOG_DEBUG("Compression set: %d, Compression got: %d", gDSAudioPortConfiguration[port].supported_compressions[compression], getCompression);
            UT_ASSERT_EQUAL(gDSAudioPortConfiguration[port].supported_compressions[compression], getCompression);
        } /* for (compression) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test is designed to set and get the Dialog Enhancement for dsAudio
*
* This test sets the Dialog Enhancement for dsAudio and then gets the
* Dialog Enhancement to verify if the set operation was successful.
* This is done for all audio port types and for two indexes. The test is
* designed to ensure the proper functioning of the dsAudio's Dialog Enhancement
* set and get operations.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to Test specification documentation
*   [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetDialogEnhancement(void)
{
    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int level, getLevel;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & dsMS12SUPPORT_DialogueEnhancer)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort() failed with error: %d", ret);
            continue;
        }

        for (level = gDSAudioPortConfiguration[port].min_dialog_enhancement_level; level <= gDSAudioPortConfiguration[port].max_dialog_enhancement_level; level++) {
            UT_LOG_DEBUG("Invoking dsSetDialogEnhancement() with handle: %p and level: %d", handle, level);
            ret = dsSetDialogEnhancement(handle, level);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetDialogEnhancement() failed with error: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetDialogEnhancement() with handle: %p", handle);
            ret = dsGetDialogEnhancement(handle, &getLevel);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetDialogEnhancement() failed with error: %d", ret);
            }

            UT_LOG_DEBUG("Returned level: %d", getLevel);
            UT_ASSERT_EQUAL(getLevel, level);
        }/* for (level) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test is designed to set and get the Dolby volume mode for different audio port types.
*
* This test initializes the audio port, then for each audio port type,
* it gets the audio port, sets the Dolby volume mode to true, gets the
* Dolby volume mode and checks if it is true, sets the Dolby volume mode
* to false, gets the Dolby volume mode and checks if it is false, and
* finally terminates the audio port. The test is designed to verify the
* functionality of setting and getting the Dolby volume mode for different audio port types.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 06@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetDolbyVolumeMode(void)
{
    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    bool mode;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & dsMS12SUPPORT_DolbyVolume)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsSetDolbyVolumeMode with handle %p and mode true", handle);
        ret = dsSetDolbyVolumeMode(handle, true);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsSetDolbyVolumeMode failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetDolbyVolumeMode with handle %p", handle);
        ret = dsGetDolbyVolumeMode(handle, &mode);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(mode,true);
        if (ret != dsERR_NONE || !mode) {
            UT_LOG_ERROR("dsGetDolbyVolumeMode failed with error %d and mode %d", ret, mode);
        }

        UT_LOG_DEBUG("Invoking dsSetDolbyVolumeMode with handle %p and mode false", handle);
        ret = dsSetDolbyVolumeMode(handle, false);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsSetDolbyVolumeMode failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetDolbyVolumeMode with handle %p", handle);
        ret = dsGetDolbyVolumeMode(handle, &mode);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(mode,false);
        if (ret != dsERR_NONE || mode) {
            UT_LOG_ERROR("dsGetDolbyVolumeMode failed with error %d and mode %d", ret, mode);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to set and get the Intelligent Equalizer Mode for dsAudio
*
* In this test, the Intelligent Equalizer Mode for dsAudio is set and then
* retrieved to verify if the set operation was successful. This is done for
* all audio port types and for all modes from 0 to 6. The test is crucial
* to ensure the correct functioning of the Intelligent Equalizer Mode
* setting and retrieval operations in dsAudio.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 07@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetIntelligentEqualizerMode(void)
{
    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    int mode = 0;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & dsMS12SUPPORT_InteligentEqualizer)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        for (mode = 0; mode <= 6; mode++) {
            UT_LOG_DEBUG("Invoking dsSetIntelligentEqualizerMode with handle %p and mode %d", handle, mode);
            ret = dsSetIntelligentEqualizerMode(handle, mode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetIntelligentEqualizerMode failed with error %d", ret);
                continue;
            }

            int getMode = 0;
            UT_LOG_DEBUG("Invoking dsGetIntelligentEqualizerMode with handle %p", handle);
            ret = dsGetIntelligentEqualizerMode(handle, &getMode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetIntelligentEqualizerMode failed with error %d", ret);
            }

            UT_LOG_DEBUG("Returned mode %d", getMode);
            UT_ASSERT_EQUAL(mode, getMode);
        } /* for (mode) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of setting and getting
* the volume leveller for a given audio sink
*
* In this test, the dsAudioPortInit function is first invoked to initialize
* the audio port. Then, for each audio port type, the dsGetAudioPort function
* is called to get the handle of the audio port. The dsSetVolumeLeveller function
* is then used to set the volume leveller for the obtained audio port.
* The dsGetVolumeLeveller function is subsequently called to retrieve the volume
* leveller that was set. The retrieved volume leveller is then compared with the set
* volume leveller to verify if they match. Finally, the dsAudioPortTerm function is
* invoked to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 08@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetVolumeLeveller(void)
{
    gTestID = 8;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t retStatus;
    intptr_t handle;
    dsVolumeLeveller_t volLevellerSet = {0, 0}, volLevellerGet;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    retStatus = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_Volumeleveller is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x08)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        retStatus = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
        if (retStatus != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", retStatus);
            continue;
        }

        for(int mode = 0; mode <= 2; mode++) {
            for(int level = 0; level <= 10; level++) {
                volLevellerSet.mode  = mode;
                volLevellerSet.level = level;
                UT_LOG_DEBUG("Invoking dsSetVolumeLeveller with handle: %p and volLeveller: {mode: %d, level: %d}", handle, volLevellerSet.mode, volLevellerSet.level);
                retStatus = dsSetVolumeLeveller(handle, volLevellerSet);
                UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
                if (retStatus != dsERR_NONE) {
                    UT_LOG_ERROR("dsSetVolumeLeveller failed with status: %d", retStatus);
                    continue;
                }

                UT_LOG_DEBUG("Invoking dsGetVolumeLeveller with handle: %p", handle);
                retStatus = dsGetVolumeLeveller(handle, &volLevellerGet);
                UT_ASSERT_EQUAL(retStatus, dsERR_NONE);
                if (retStatus != dsERR_NONE) {
                    UT_LOG_ERROR("dsGetVolumeLeveller failed with status: %d", retStatus);
                }

                UT_LOG_DEBUG("Retrieved volLeveller: {mode: %d, level: %d}", volLevellerGet.mode, volLevellerGet.level);
                UT_ASSERT_EQUAL(volLevellerGet.mode, volLevellerSet.mode);
                UT_ASSERT_EQUAL(volLevellerGet.level, volLevellerSet.level);
            } /* for (mode) */
        } /* for (mode) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    retStatus = dsAudioPortTerm();
    UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to validate the setting and getting of Bass Enhancer for audio sink
*
* In this test, the dsAudioPortInit() function is invoked to initialize the audio port.
* Then, for each type of audio port and for each index, the dsGetAudioPort() function
* is invoked to get the audio port. A random boost value is set using dsSetBassEnhancer()
* function and then retrieved using dsGetBassEnhancer() function. The set and retrieved
* boost values are then compared for equality. Finally, the dsAudioPortTerm() function is
* invoked to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 09@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetBassEnhancer(void)
{
    gTestID = 9;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    int boost = 0;
    int setBoost = 0;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_BassEnhancer is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x10)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }
        for (setBoost = 0; setBoost <= 100; setBoost++) {
            UT_LOG_DEBUG("Invoking dsSetBassEnhancer() with handle: %p and boost: %d", handle, setBoost);
            ret = dsSetBassEnhancer(handle, setBoost);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetBassEnhancer() failed with error: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetBassEnhancer() with handle: %p", handle);
            ret = dsGetBassEnhancer(handle, &boost);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetBassEnhancer() failed with error: %d", ret);
            }

            UT_LOG_DEBUG("Boost value: %d", boost);
            UT_ASSERT_EQUAL(boost, setBoost);
        } /* for (setBoost) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test verifies the enabling and functionality of the
* Surround Decoder sink in the L2 dsAudio module
*
* This test initializes the dsAudio port, gets the audio port for each type,
* enables the surround decoder, checks if the surround decoder is enabled, and
* finally terminates the dsAudio port. The test is designed to ensure that the
* surround decoder can be successfully enabled and function as expected in the
* L2 dsAudio module.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 010@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_EnableAndVerifySurroundDecoder(void)
{
    gTestID = 10;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    bool enabled = true;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_SurroundDecoder is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x20)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsEnableSurroundDecoder with handle %p and enabled set to true", handle);
        ret = dsEnableSurroundDecoder(handle, enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsEnableSurroundDecoder failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsIsSurroundDecoderEnabled with handle %p", handle);
        ret = dsIsSurroundDecoderEnabled(handle, &enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(enabled,true);
        if (ret != dsERR_NONE || !enabled) {
            UT_LOG_ERROR("dsIsSurroundDecoderEnabled failed with error %d or enabled is not set", ret);
        }

        enabled = false;
        UT_LOG_DEBUG("Invoking dsEnableSurroundDecoder with handle %p and enabled set to false", handle);
        ret = dsEnableSurroundDecoder(handle, enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsEnableSurroundDecoder failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsIsSurroundDecoderEnabled with handle %p", handle);
        ret = dsIsSurroundDecoderEnabled(handle, &enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(enabled,false);
        if (ret != dsERR_NONE || enabled) {
            UT_LOG_ERROR("dsIsSurroundDecoderEnabled failed with error %d or enabled is not set", ret);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of setting and getting DRC mode in dsAudio.
*
* In this test, the dsAudioPortInit() function is invoked to initialize the audio port.
* Then, for each type of audio port and for each index, the dsGetAudioPort() function
* is invoked to get the audio port. For each mode (0 and 1), the dsSetDRCMode() function
* is invoked to set the DRC mode and the dsGetDRCMode() function is invoked to
* get the DRC mode. The test asserts that the mode set is the same as the mode got.
* Finally, the dsAudioPortTerm() function is invoked to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 011@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetDRCMode(void)
{
    gTestID = 11;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int mode;
    int getMode;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_DRCMode is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x40)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        for (mode = 0; mode <= 1; mode++) {
            UT_LOG_DEBUG("Invoking dsSetDRCMode() with handle=%p and mode=%d", handle, mode);
            ret = dsSetDRCMode(handle, mode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetDRCMode() failed with status: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetDRCMode() with handle=%p", handle);
            ret = dsGetDRCMode(handle, &getMode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetDRCMode() failed with status: %d", ret);
            }

            UT_LOG_DEBUG("Returned mode: %d", getMode);
            UT_ASSERT_EQUAL(getMode, mode);
        } /* for (mode) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to validate the setting and getting of Surround Virtualizer in dsAudio
*
* In this test, the dsAudioPortInit function is invoked to initialize the audio port.
* Then, for each audio port type and index, the dsGetAudioPort function is invoked
* to get the audio port handle. The dsSetSurroundVirtualizer function is then invoked
* to set the surround virtualizer for the obtained handle. The dsGetSurroundVirtualizer
* function is then invoked to get the surround virtualizer for the same handle.
* The obtained virtualizer is then compared with the set virtualizer to validate
* the set and get functionality. Finally, the dsAudioPortTerm function is invoked
* to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 012@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetSurroundVirtualizer(void)
{
    gTestID = 12;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    dsSurroundVirtualizer_t virtualizer = {0, 0}, getVirtualizer;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_SurroundVirtualizer is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x80)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        for(int mode = 0; mode <= 2; mode++) {
            for(int boost = 0; boost <= 96; boost +=16) {
                virtualizer.mode = mode;
                virtualizer.boost = boost;
                UT_LOG_DEBUG("Invoking dsSetSurroundVirtualizer with handle: %p and virtualizer: {mode: %d, boost: %d}", handle, virtualizer.mode, virtualizer.boost);
                ret = dsSetSurroundVirtualizer(handle, virtualizer);
                UT_ASSERT_EQUAL(ret, dsERR_NONE);
                if (ret != dsERR_NONE) {
                    UT_LOG_ERROR("dsSetSurroundVirtualizer failed with error: %d", ret);
                    continue;
                }

                UT_LOG_DEBUG("Invoking dsGetSurroundVirtualizer with handle: %p", handle);
                ret = dsGetSurroundVirtualizer(handle, &getVirtualizer);
                UT_ASSERT_EQUAL(ret, dsERR_NONE);
                if (ret != dsERR_NONE) {
                    UT_LOG_ERROR("dsGetSurroundVirtualizer failed with error: %d", ret);
                }

                UT_LOG_DEBUG("Received virtualizer: {mode: %d, boost: %d}", getVirtualizer.mode, getVirtualizer.boost);
                UT_ASSERT_EQUAL(getVirtualizer.mode, virtualizer.mode);
                UT_ASSERT_EQUAL(getVirtualizer.boost, virtualizer.boost);
            } /* for (boost) */
        } /* for (mode) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of setting and getting MISteering in dsAudio.
*
* In this test, we initialize the audio port and then for each type of audio port
* and for each index, we set the MISteering to true and then get the MISteering
* to verify if it has been set correctly. If any of these operations fail,
* we log the error and continue with the next iteration. Finally, we terminate
* the audio port and assert that the operation was successful.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 013@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetMISteering(void)
{
    gTestID = 13;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    bool enabled;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_MISteering is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x100)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsSetMISteering() with handle=%p and enabled=true", handle);
        ret = dsSetMISteering(handle, true);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsSetMISteering() failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetMISteering() with handle=%p", handle);
        ret = dsGetMISteering(handle, &enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetMISteering() failed with error: %d", ret);
        }

        UT_LOG_DEBUG("Enabled status: %d", enabled);
        UT_ASSERT_EQUAL(enabled, true);

        UT_LOG_DEBUG("Invoking dsSetMISteering() with handle=%p and enabled=false", handle);
        ret = dsSetMISteering(handle, false);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsSetMISteering() failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetMISteering() with handle=%p", handle);
        ret = dsGetMISteering(handle, &enabled);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetMISteering() failed with error: %d", ret);
        }

        UT_LOG_DEBUG("Enabled status: %d", enabled);
        UT_ASSERT_EQUAL(enabled, false);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to validate the setting and getting of Graphic Equalizer Mode in dsAudio
*
* In this test, the dsAudioPortInit() function is first invoked to initialize
* the audio port. Then, for each type of audio port and for each index,
* the dsGetAudioPort() function is invoked to get the audio port.
* For each mode from 0 to 3, the dsSetGraphicEqualizerMode() function is
* invoked to set the graphic equalizer mode and the dsGetGraphicEqualizerMode()
* function is invoked to get the graphic equalizer mode. The mode set and
* the mode got are then compared to ensure they are equal. Finally, the dsAudioPortTerm()
* function is invoked to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 014@n
*
* **Test Procedure:**
* Refer to Test specification documentation [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetGraphicEqualizerMode(void)
{
    gTestID = 14;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    int mode = 0;
    int getMode = 0;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_GraphicEqualizer is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x200)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }
        for (mode = 0; mode <= 3; mode++) {
            UT_LOG_DEBUG("Invoking dsSetGraphicEqualizerMode() with handle: %p and mode: %d", handle, mode);
            ret = dsSetGraphicEqualizerMode(handle, mode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetGraphicEqualizerMode() failed with error: %d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetGraphicEqualizerMode() with handle: %p", handle);
            ret = dsGetGraphicEqualizerMode(handle, &getMode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetGraphicEqualizerMode() failed with error: %d", ret);
            }

            UT_LOG_DEBUG("Returned mode: %d", getMode);
            UT_ASSERT_EQUAL(mode, getMode);
        } /* for (mode) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to enable, disable and retrieve the LE configuration for the audio sink
*
* In this test, the dsAudioPortInit() function is invoked to initialize the audio port.
* Then, for each type of audio port and for each index up to 10, the dsGetAudioPort()
* function is invoked to get the audio port. If successful, the dsEnableLEConfig()
* function is invoked to enable the LE configuration. The dsGetLEConfig() function is
* then invoked to retrieve the LE configuration. If successful, the dsEnableLEConfig()
* function is invoked again to disable the LE configuration. The dsGetLEConfig() function
* is then invoked again to check if the LE configuration has been disabled.
* Finally, the dsAudioPortTerm() function is invoked to terminate the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 015@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_EnableDisableAndRetrieveLEConfig(void)
{
    gTestID = 15;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    bool enable = false;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        //Note: dsMS12SUPPORT_LEConfig is not defined yet in interface file
        if(!(gDSAudioPortConfiguration[port].ms12_capabilites & 0x400)) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsEnableLEConfig() with handle=%p and enable=true", handle);
        ret = dsEnableLEConfig(handle, true);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsEnableLEConfig() failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetLEConfig() with handle=%p", handle);
        ret = dsGetLEConfig(handle, &enable);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE || !enable) {
            UT_LOG_ERROR("dsGetLEConfig() failed with error: %d", ret);
        }
        UT_ASSERT_EQUAL(enable,true);

        UT_LOG_DEBUG("Invoking dsEnableLEConfig() with handle=%p and enable=false", handle);
        ret = dsEnableLEConfig(handle, false);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsEnableLEConfig() failed with error: %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetLEConfig() with handle=%p", handle);
        ret = dsGetLEConfig(handle, &enable);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE || enable) {
            UT_LOG_ERROR("dsGetLEConfig() failed with error: %d", ret);
        }
        UT_ASSERT_EQUAL(enable,false);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to check MS12 decode support for dsAudio
*
* This test checks if the MS12 decode support is available for dsAudio.
* It initializes the audio port, gets the audio port for each type and index,
* checks if MS12 decode is available, and finally terminates the audio port.
* The test is designed to ensure the correct functioning of dsAudio's MS12 decode support.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 016@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_CheckMS12DecodeSupport(void)
{
    gTestID = 16;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    bool hasMS12Decode;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("dsAudioPortInit returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }
        UT_LOG_DEBUG("dsGetAudioPort returned handle %p", handle);

        UT_LOG_DEBUG("Invoking dsIsAudioMS12Decode with handle %p", handle);
        ret = dsIsAudioMS12Decode(handle, &hasMS12Decode);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsIsAudioMS12Decode returned %d", ret);
        }
        UT_LOG_DEBUG("dsIsAudioMS12Decode returned %d", hasMS12Decode);

        UT_ASSERT_EQUAL(hasMS12Decode, gDSAudioPortConfiguration[port].isms12decode);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("dsAudioPortTerm returned %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test checks the support for MS11 decoding in dsAudio
*
* This test initializes the audio port and checks if MS11 decoding is supported
* for each audio port type. If the decoding is supported, it verifies the obtained
* values with the values retrieved from the 'Sink_AudioSettings.yaml' file.
* The test ends by terminating the audio port.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 017@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_CheckMS11DecodeSupport(void)
{
    gTestID = 17;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    bool HasMS11Decode = false;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsIsAudioMSDecode with handle: %p", handle);
        ret = dsIsAudioMSDecode(handle, &HasMS11Decode);
        UT_LOG_DEBUG("Return status: %d, HasMS11Decode: %d, SupportedMS11: %d",
                        ret, HasMS11Decode, gDSAudioPortConfiguration[port].isms11decode);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(HasMS11Decode, gDSAudioPortConfiguration[port].isms11decode);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test to verify the MS12 Audio Profiles for a given sink
*
* This test case verifies the MS12 Audio Profiles for a given sink.
* It checks if the correct audio profiles are returned for a valid sink.
* This is important to ensure that the audio profiles are correctly configured and retrieved.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 018@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_VerifyMS12AudioProfiles_sink(void)
{
    gTestID = 18;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    dsMS12AudioProfileList_t profiles;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        if(gDSAudioPortConfiguration[port].ms12_audioprofilecount <= 0) {
            continue;
        }
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetMS12AudioProfileList with valid handle");
        ret = dsGetMS12AudioProfileList(handle, &profiles);
        UT_LOG_DEBUG("Return status: %d, Profiles: %s, Profile Count: %d", ret, profiles.audioProfileList, profiles.audioProfileCount);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(profiles.audioProfileCount, gDSAudioPortConfiguration[port].ms12_audioprofilecount);

        for (int i = 0; i < profiles.audioProfileCount; i++) {
            if(strstr(profiles.audioProfileList, gDSAudioPortConfiguration[port].ms12_audio_profiles[i]) == NULL) {
                UT_FAIL("Audioprofile mismatch");
                UT_LOG_ERROR("%s is not in %s", gDSAudioPortConfiguration[port].ms12_audio_profiles[i], profiles.audioProfileList);
            }
        } /* for (i) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to validate the setting and getting of MS12 Audio Profile for different audio ports.
*
* In this test, the dsAudioPortInit() function is first invoked to
* initialize the audio port. Then, for each type of audio port,
* the dsGetAudioPort() function is invoked to get the handle of the audio port.
* The dsGetMS12AudioProfileList() function is then invoked to get the list of MS12 audio profiles.
* For each profile in the list, the dsSetMS12AudioProfile() function is invoked to set
* the profile, and the dsGetMS12AudioProfile() function is invoked to get the profile.
* The set and get profiles are then compared for equality. Finally, the dsAudioPortTerm()
* function is invoked to terminate the audio port. The test ensures that all these functions
* work as expected for different types of audio ports and profiles.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 019@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetMS12AudioProfile_sink(void)
{
    gTestID = 19;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    dsMS12AudioProfileList_t profiles;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        if(gDSAudioPortConfiguration[port].ms12_audioprofilecount <= 0) {
            continue;
        }
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetMS12AudioProfileList() with handle=%p", handle);
        ret = dsGetMS12AudioProfileList(handle, &profiles);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetMS12AudioProfileList() failed with status=%d", ret);
        }

        char *profileName = strtok(profiles.audioProfileList, ",");
        char profile[DS_AUDIO_MAX_MS12_PROFILE_LEN];
        while (profileName != NULL) {
            UT_LOG_DEBUG("Invoking dsSetMS12AudioProfile() with handle=%p and profile=%s", handle, profileName);
            ret = dsSetMS12AudioProfile(handle, profileName);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsSetMS12AudioProfile() failed with status=%d", ret);
                continue;
            }

            UT_LOG_DEBUG("Invoking dsGetMS12AudioProfile() with handle=%p", handle);
            ret = dsGetMS12AudioProfile(handle, profile);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetMS12AudioProfile() failed with status=%d", ret);
            }

            UT_ASSERT_STRING_EQUAL(profile, profileName);
            profileName = strtok(NULL, ",");
        } /* while (profileName) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for setting and getting the stereo mode of an audio sink
*
* This test checks if the stereo mode of an audio sink can be set and
* retrieved correctly. It does this by first initializing the audio port,
* then getting the audio port for each type and index. It then sets the
* stereo mode for the retrieved audio port and checks if the set mode
* can be retrieved correctly. If any of these operations fail, it logs
* an error and continues with the next iteration. Finally, it terminates
* the audio port and checks if the termination was successful.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 020@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetStereoMode(void)
{
    gTestID = 20;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    dsAudioStereoMode_t getmode ,mode;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        if(gDSAudioPortConfiguration[port].no_of_supported_stereo_mode <= 0) {
            continue;
        }
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }
        for (int j = 0; j < gDSAudioPortConfiguration[port].no_of_supported_stereo_mode; j++) {
            mode = gDSAudioPortConfiguration[port].supported_stereo_mode[j];
            UT_LOG_DEBUG("Invoking dsSetStereoMode() with handle=%p and mode=%d", handle, mode);
            ret = dsSetStereoMode(handle, mode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);

            UT_LOG_DEBUG("Invoking dsGetStereoMode() with handle=%p", handle);
            ret = dsGetStereoMode(handle, &getmode);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);

            UT_LOG_DEBUG("Stereo mode: %d and return status: %d", getmode, ret);
            UT_ASSERT_EQUAL(mode, getmode);
        } /* for (j) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to validate the setting and getting of Stereo Auto mode in dsAudio API
*
* This test function tests the setting and getting of Stereo Auto mode
* in dsAudio API. It first initializes the audio port, then for each
* audio port type, it gets the audio port, sets the Stereo Auto mode,
* gets the Stereo Auto mode and validates it. The test is designed
* to ensure that the dsAudio API's set and get functions for
* Stereo Auto mode are working as expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 021@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetStereoAuto_sink(void)
{
    gTestID = 21;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int autoMode = 1;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(!gDSAudioPortConfiguration[port].stereo_auto_mode) {
            continue;
        }
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }
        UT_LOG_DEBUG("Handle: %p", handle);

        UT_LOG_DEBUG("Invoking dsSetStereoAuto with handle %p and autoMode %d", handle, autoMode);
        ret = dsSetStereoAuto(handle, autoMode);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        int getAutoMode;
        UT_LOG_DEBUG("Invoking dsGetStereoAuto with handle %p", handle);
        ret = dsGetStereoAuto(handle, &getAutoMode);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetStereoAuto failed with error %d", ret);
        }
        UT_LOG_DEBUG("AutoMode: %d", getAutoMode);

        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(getAutoMode, autoMode);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for setting and getting audio gain for different audio ports
*
* This test is designed to verify the functionality of setting and getting
* audio gain for different audio ports. It tests the dsSetAudioGain and
* dsGetAudioGain functions by setting a range of gain values and then
* getting the gain value to verify if the set value is correctly retrieved.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 022@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetAudioGain_sink(void)
{
    gTestID = 22;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    float gain, getGain;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(gDSAudioPortConfiguration[port].typeid != dsAUDIOPORT_TYPE_SPEAKER) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        for (gain = -2080; gain <= 480; gain += 10) {
            UT_LOG_DEBUG("Invoking dsSetAudioGain with handle=%p and gain=%f", handle, gain);
            ret = dsSetAudioGain(handle, gain);

            UT_ASSERT_EQUAL(ret, dsERR_NONE);

            UT_LOG_DEBUG("Invoking dsGetAudioGain with handle=%p", handle);
            ret = dsGetAudioGain(handle, &getGain);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetAudioGain failed with status=%d", ret);
            }
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_ASSERT_EQUAL(gain, getGain);
        } /* for (gain) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to set and get the audio level for a sink
*
* In this test, the audio level for a sink is set and then retrieved
* to verify if the set operation was successful. This is done for all
* audio port types and for all ports. The test is crucial to ensure
* the correct functioning of the audio level setting and getting operations.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 023@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetAudioLevel_sink(void)
{
    gTestID = 23;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    float level, getLevel;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(gDSAudioPortConfiguration[port].typeid != dsAUDIOPORT_TYPE_SPEAKER) {
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        for (level = 0; level <= 100; level+=10) {
            UT_LOG_DEBUG("Invoking dsSetAudioLevel with handle=%p and level=%f", handle, level);
            ret = dsSetAudioLevel(handle, level);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);

            UT_LOG_DEBUG("Invoking dsGetAudioLevel with handle=%p", handle);
            ret = dsGetAudioLevel(handle, &getLevel);
            if (ret != dsERR_NONE) {
                UT_LOG_ERROR("dsGetAudioLevel failed with error: %d", ret);
            }

            UT_LOG_DEBUG("Returned level=%f and status=%d", getLevel, ret);
            UT_ASSERT_EQUAL(ret, dsERR_NONE);
            UT_ASSERT_EQUAL(level, getLevel);
        } /* for (level) */
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test verifies the functionality of audio mute in dsAudio API
*
* This test case is designed to verify the audio mute functionality of
* the dsAudio API. It tests the ability of the API to mute and unmute the audio,
* and to correctly report the mute status. This is important to ensure that
* the API is correctly controlling the audio output and that it can
* accurately report the current mute status.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 024@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_AudioMuteVerification(void)
{
    gTestID = 24;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    bool mute = false;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {

        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsSetAudioMute with handle: %p and mute: true", handle);
        ret = dsSetAudioMute(handle, true);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        UT_LOG_DEBUG("Invoking dsIsAudioMute with handle: %p", handle);
        ret = dsIsAudioMute(handle, &mute);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(mute, true);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsIsAudioMute failed with error: %d", ret);
        }

        UT_LOG_DEBUG("Invoking dsSetAudioMute with handle: %p and mute: false", handle);
        ret = dsSetAudioMute(handle, false);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        UT_LOG_DEBUG("Invoking dsIsAudioMute with handle: %p", handle);
        ret = dsIsAudioMute(handle, &mute);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(mute, false);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsIsAudioMute failed with error: %d", ret);
        }
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to set and get the audio delay for a sink
*
* In this test, the audio delay for a sink is set and then retrieved
* to verify if the set value is correctly stored and retrieved.
* This is done for all audio port types. The test is crucial to ensure
* the correct functioning of audio delay setting and retrieval in the system.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 025@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetAudioDelay(void)
{
    gTestID = 25;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    uint32_t setDelay = 100;
    uint32_t getDelay;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(gDSAudioPortConfiguration[port].typeid != dsAUDIOPORT_TYPE_SPDIF &&
           gDSAudioPortConfiguration[port].typeid != dsAUDIOPORT_TYPE_HDMI_ARC &&
           gDSAudioPortConfiguration[port].typeid != dsAUDIOPORT_TYPE_HDMI) {
               continue;
        }
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsSetAudioDelay with handle and delay %d ms", setDelay);
        ret = dsSetAudioDelay(handle, setDelay);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        UT_LOG_DEBUG("Invoking dsGetAudioDelay with handle");
        ret = dsGetAudioDelay(handle, &getDelay);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioDelay failed with error %d", ret);
        }

        UT_LOG_DEBUG("Retrieved audio delay: %d ms", getDelay);
        UT_ASSERT_EQUAL(getDelay, setDelay);
    } /* for (port) */

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test verifies the Atmos capabilities of the sink
*
* In this test, the Atmos capabilities of the sink are verified by
* initializing the audio port, getting the audio port with type speaker and index 1,
* getting the sink device Atmos capability, and finally terminating the audio port.
* The test ensures that all these operations are successful and the returned values are as expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 026@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_VerifyAtmosCapabilities_sink(void)
{
    gTestID = 26;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    dsATMOSCapability_t capability;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("dsAudioPortInit returned %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    for (int port = 0; port < gDSAudioNumberOfPorts; port++) {
        if(gDSAudioPortConfiguration[port].atmos_capabilites == 0) {
               continue;
        }
        UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index);
        ret = dsGetAudioPort(gDSAudioPortConfiguration[port].typeid, gDSAudioPortConfiguration[port].index, &handle);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        if (ret != dsERR_NONE) {
            UT_LOG_ERROR("dsGetAudioPort failed with error %d", ret);
            continue;
        }

        UT_LOG_DEBUG("Invoking dsGetSinkDeviceAtmosCapability with handle=%p", handle);
        ret = dsGetSinkDeviceAtmosCapability(handle, &capability);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_LOG_DEBUG("dsGetSinkDeviceAtmosCapability returned %d and capability=%d", ret, capability);

        UT_ASSERT_EQUAL(capability, gDSAudioPortConfiguration[port].atmos_capabilites);
    }

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("dsAudioPortTerm returned %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to verify the functionality of the dsAudio_GetAudioCapabilities_sink API
*
* This test case is designed to validate the dsAudio_GetAudioCapabilities_sink API.
* The test case will initialize the audio port, get the audio port with type as
* sink and index as 0, get the audio capabilities with the handle obtained from
* the previous step, and finally terminate the audio port. The test case will
* assert the return status of each operation to ensure they are successful.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 027@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_GetAudioCapabilities(void)
{
    gTestID = 27;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t retStatus;
    intptr_t handle;
    int capabilities;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    retStatus = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL_FATAL(retStatus, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAudioPort with type as sink and index as 0");
    retStatus = dsGetAudioPort(gDSAudioPortConfiguration[0].typeid, gDSAudioPortConfiguration[0].index, &handle);
    UT_LOG_DEBUG("Return status: %d, Handle: %p", retStatus, handle);
    UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAudioCapabilities with handle obtained from previous step");
    retStatus = dsGetAudioCapabilities(handle, &capabilities);
    UT_LOG_DEBUG("Return status: %d, Capabilities: %d", retStatus, capabilities);
    UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

    UT_ASSERT_EQUAL(capabilities, gAudioCapabilities);

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    retStatus = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", retStatus);
    UT_ASSERT_EQUAL(retStatus, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test function is designed to test the enabling, disabling and
* retrieval of audio mixing in the dsAudio API.
*
* This test function iterates over all audio ports, enabling and disabling
* audio mixing, and then retrieving the state to verify the operation was successful.
* It tests the robustness and correctness of the dsAudio API's audio mixing functionality.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 028@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_EnableDisableRetrieveAudioMixing(void)
{
    gTestID = 28;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret = dsERR_NONE;
    intptr_t handle = 0;
    bool mixing = false;

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAudioPort with type: %d and index: %d", gDSAudioPortConfiguration[0].typeid,
                             gDSAudioPortConfiguration[0].index);
    ret = dsGetAudioPort(gDSAudioPortConfiguration[0].typeid, gDSAudioPortConfiguration[0].index, &handle);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsSetAssociatedAudioMixing with handle: %p and mixing: true", handle);
    ret = dsSetAssociatedAudioMixing(handle, true);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAssociatedAudioMixing with handle: %p", handle);
    ret = dsGetAssociatedAudioMixing(handle, &mixing);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);
    UT_ASSERT_EQUAL(mixing, true);

    UT_LOG_DEBUG("Invoking dsSetAssociatedAudioMixing with handle: %p and mixing: false", handle);
    ret = dsSetAssociatedAudioMixing(handle, false);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAssociatedAudioMixing with handle: %p", handle);
    ret = dsGetAssociatedAudioMixing(handle, &mixing);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);
    UT_ASSERT_EQUAL(mixing, false);

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test function is designed to test the audio port control sink in the L2 dsAudio module.
*
* This function tests the initialization, setting, and termination of
* the audio port in the L2 dsAudio module. It also tests the associated
* audio mixing and fader control functionalities. The test is performed
* for different types of audio ports and for different mixer balance values.
* The function asserts that all the invoked functions return no error and that
* the mixer balance value set is the same as the one retrieved.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 029@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_AudioPortControl(void)
{
    gTestID = 29;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    int mixerbalance;

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAudioPort() with type=%d and index=%d", gDSAudioPortConfiguration[0].typeid,
                                          gDSAudioPortConfiguration[0].index);
    ret = dsGetAudioPort(gDSAudioPortConfiguration[0].typeid, gDSAudioPortConfiguration[0].index, &handle);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsSetAssociatedAudioMixing() with handle=%p and mixing=true", handle);
    ret = dsSetAssociatedAudioMixing(handle, true);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    for (mixerbalance = -32; mixerbalance <= 32; mixerbalance+=8) {
        UT_LOG_DEBUG("Invoking dsSetFaderControl() with handle=%p and mixerbalance=%d", handle, mixerbalance);
        ret = dsSetFaderControl(handle, mixerbalance);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);

        int getMixerbalance;
        UT_LOG_DEBUG("Invoking dsGetFaderControl() with handle=%p", handle);
        ret = dsGetFaderControl(handle, &getMixerbalance);
        UT_ASSERT_EQUAL(ret, dsERR_NONE);
        UT_ASSERT_EQUAL(getMixerbalance, mixerbalance);
    }

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief This test aims to validate the setting and getting of primary language in dsAudio
*
* In this test, we initialize the audio port, set a primary language,
* get the primary language and validate if the set and get languages are same.
* This is to ensure the dsAudio's set and get primary language
* functionalities are working as expected.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 030@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetPrimaryLanguage(void)
{
    gTestID = 30;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    const char* setLang = "eng";
    char getLang[4];

    UT_LOG_DEBUG("Invoking dsAudioPortInit()");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAudioPort() with valid parameters");
    ret = dsGetAudioPort(gDSAudioPortConfiguration[0].typeid, gDSAudioPortConfiguration[0].index, &handle);
    UT_LOG_DEBUG("Return status: %d, Handle: %p", ret, handle);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsSetPrimaryLanguage() with handle and valid language code");
    ret = dsSetPrimaryLanguage(handle, setLang);
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetPrimaryLanguage() with handle");
    ret = dsGetPrimaryLanguage(handle, getLang);
    UT_LOG_DEBUG("Return status: %d, Language: %s", ret, getLang);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);
    UT_ASSERT_STRING_EQUAL(getLang, setLang);

    UT_LOG_DEBUG("Invoking dsAudioPortTerm()");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test for setting and getting the secondary language for the audio port
*
* This function tests the setting and getting of the secondary language
* for the audio port. It first initializes the audio port, gets the audio port handle,
* sets the secondary language, gets the secondary language, and finally terminates
* the audio port. It uses Cunit assertions to check the return values of the API calls.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 031@n
*
* **Test Procedure:**
* Refer to Test specification documentation
* [dsAudio_L2_Low-Level_TestSpecification.md](../docs/pages/ds-audio_L2_Low-Level_TestSpecification.md)
*/

void test_l2_dsAudio_SetAndGetSecondaryLanguage(void)
{
    gTestID = 31;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    dsError_t ret;
    intptr_t handle;
    const char* setLang = "eng";
    char getLang[4];

    UT_LOG_DEBUG("Invoking dsAudioPortInit");
    ret = dsAudioPortInit();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetAudioPort with valid parameters");
    ret = dsGetAudioPort(gDSAudioPortConfiguration[0].typeid, gDSAudioPortConfiguration[0].index, &handle);
    UT_LOG_DEBUG("Return status: %d, Handle: %p", ret, handle);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsSetSecondaryLanguage with handle and valid language code");
    ret = dsSetSecondaryLanguage(handle, setLang);
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_DEBUG("Invoking dsGetSecondaryLanguage with handle");
    ret = dsGetSecondaryLanguage(handle, getLang);
    UT_LOG_DEBUG("Return status: %d, Language: %s", ret, getLang);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);
    UT_ASSERT_STRING_EQUAL(getLang, setLang);

    UT_LOG_DEBUG("Invoking dsAudioPortTerm");
    ret = dsAudioPortTerm();
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, dsERR_NONE);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_l2_dsAudio_register(void)
{
    if(gSourceType == 1) {
        // Create the test suite for source type
        pSuite = UT_add_suite("[L2 dsAudio - Source]", NULL, NULL);
        if (pSuite == NULL) {
            UT_LOG_ERROR("Failed to create the test suite");
            return -1;
        }
    }
    else if(gSourceType == 0) {
        // Create the test suite for sink type
        pSuite = UT_add_suite("[L2 dsAudio - Sink]", NULL, NULL);
        if (pSuite == NULL) {
            UT_LOG_ERROR("Failed to create the test suite");
            return -1;
        }
    }
    else {
        UT_LOG_ERROR("Invalid platform type");
        return -1;
    }

    // List of test function names and strings
    UT_add_test( pSuite, "L2_EnableDisableAndVerifyAudioPortStatus", test_l2_dsAudio_EnableDisableAndVerifyAudioPortStatus);
    if(gSourceType == 0) {
        UT_add_test( pSuite, "l2_dsAudio_CheckHeadphoneConnectionStatus_sink", test_l2_dsAudio_CheckHeadphoneConnectionStatus_sink);
    }
    UT_add_test( pSuite, "L2_RetrieveAndVerifyMS12Capabilities", test_l2_dsAudio_RetrieveAndVerifyMS12Capabilities);
    UT_add_test( pSuite, "L2_SetAndGetAudioCompression", test_l2_dsAudio_SetAndGetAudioCompression);
    UT_add_test( pSuite, "L2_SetAndGetDialogEnhancement", test_l2_dsAudio_SetAndGetDialogEnhancement);
    UT_add_test( pSuite, "L2_SetAndGetDolbyVolumeMode", test_l2_dsAudio_SetAndGetDolbyVolumeMode);
    UT_add_test( pSuite, "L2_SetAndGetIntelligentEqualizerMode", test_l2_dsAudio_SetAndGetIntelligentEqualizerMode);
    UT_add_test( pSuite, "L2_SetAndGetVolumeLeveller", test_l2_dsAudio_SetAndGetVolumeLeveller);
    UT_add_test( pSuite, "L2_SetAndGetBassEnhancer", test_l2_dsAudio_SetAndGetBassEnhancer);
    UT_add_test( pSuite, "L2_EnableAndVerifySurroundDecoder", test_l2_dsAudio_EnableAndVerifySurroundDecoder);
    UT_add_test( pSuite, "L2_SetAndGetDRCMode", test_l2_dsAudio_SetAndGetDRCMode);
    UT_add_test( pSuite, "L2_SetAndGetSurroundVirtualizer", test_l2_dsAudio_SetAndGetSurroundVirtualizer);
    UT_add_test( pSuite, "L2_SetAndGetMISteering", test_l2_dsAudio_SetAndGetMISteering);
    UT_add_test( pSuite, "L2_SetAndGetGraphicEqualizerMode", test_l2_dsAudio_SetAndGetGraphicEqualizerMode);
    UT_add_test( pSuite, "L2_EnableDisableAndRetrieveLEConfig", test_l2_dsAudio_EnableDisableAndRetrieveLEConfig);
    UT_add_test( pSuite, "L2_CheckMS12DecodeSupport", test_l2_dsAudio_CheckMS12DecodeSupport);
    UT_add_test( pSuite, "L2_CheckMS11DecodeSupport", test_l2_dsAudio_CheckMS11DecodeSupport);
    if(gSourceType == 0) {
        UT_add_test( pSuite, "L2_VerifyMS12AudioProfiles_sink", test_l2_dsAudio_VerifyMS12AudioProfiles_sink);
        UT_add_test( pSuite, "L2_SetAndGetMS12AudioProfile_sink", test_l2_dsAudio_SetAndGetMS12AudioProfile_sink);
    }

    UT_add_test( pSuite, "L2_SetAndGetStereoMode", test_l2_dsAudio_SetAndGetStereoMode);
    if(gSourceType == 0) {
        UT_add_test( pSuite, "L2_SetAndGetStereoAuto_sink", test_l2_dsAudio_SetAndGetStereoAuto_sink);
        UT_add_test( pSuite, "L2_SetAndGetAudioGain_sink", test_l2_dsAudio_SetAndGetAudioGain_sink);
        UT_add_test( pSuite, "L2_SetAndGetAudioLevel_sink", test_l2_dsAudio_SetAndGetAudioLevel_sink);
    }
    UT_add_test( pSuite, "L2_AudioMuteVerification", test_l2_dsAudio_AudioMuteVerification);
    UT_add_test( pSuite, "L2_SetAndGetAudioDelay", test_l2_dsAudio_SetAndGetAudioDelay);
    if(gSourceType == 0) {
        UT_add_test( pSuite, "L2_VerifyAtmosCapabilities_sink", test_l2_dsAudio_VerifyAtmosCapabilities_sink);
    }
    UT_add_test( pSuite, "L2_GetAudioCapabilities", test_l2_dsAudio_GetAudioCapabilities);
    UT_add_test( pSuite, "L2_EnableDisableRetrieveAudioMixing", test_l2_dsAudio_EnableDisableRetrieveAudioMixing);
    UT_add_test( pSuite, "L2_AudioPortControl", test_l2_dsAudio_AudioPortControl);
    UT_add_test( pSuite, "L2_SetAndGetPrimaryLanguage", test_l2_dsAudio_SetAndGetPrimaryLanguage);
    UT_add_test( pSuite, "L2_SetAndGetSecondaryLanguage", test_l2_dsAudio_SetAndGetSecondaryLanguage);

    return 0;
}

/** @} */ // End of DS_Audio_HALTEST_L2
/** @} */ // End of DS_Audio_HALTEST
/** @} */ // End of Device_Settings_HALTEST
/** @} */ // End of Device_Settings
/** @} */ // End of HPK