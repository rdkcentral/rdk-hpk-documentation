#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2024 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************

import os
import sys

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utPlayer import utPlayer
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.core.logModule import logModule
from dsClasses.dsVideoPort import dsVideoPortClass

class dsVideoPortHelperClass(utHelperClass):

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            testName (str) : name of the test
            qcId (str): QC ID of the test.
            log (class, optional): Parent log class. Defaults to None.
        """
        self.testName  = testName
        self.qcId      = qcId
        self.testSetupPath = os.path.join(dir_path, "dsVideoPort_L3_testSetup.yml")
        self.moduleName = "dsVideoPort"
        self.rackDevice = "dut"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        # Open Sessions for player, secondary player and for hal test
        self.player_session = self.dut.getConsoleSession("ssh_player")
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")

        deviceTestSetup = self.cpe.get("test")
        socVendor = self.cpe.get("soc_vendor")

        self.testPlayer = utPlayer(self.player_session, socVendor)

        # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))

        self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = os.path.join(self.targetWorkspace, self.moduleName)
        self.streamDownloadURL = deviceTestSetup.get("streams_download_url")

    def testDownloadAssets(self):
        """
        Downloads the test artifacts and streams listed in the test setup configuration.

        This function retrieves audio streams and other necessary files and
        saves them on the DUT (Device Under Test).

        Args:
            None
        """

        # List of streams with path
        self.testStreams = []
        url = []

        streamPaths = self.testSetup.assets.device.get(self.testName).streams

        # Download test streams to device
        if streamPaths and self.streamDownloadURL:
            for streamPath in streamPaths:
                url.append(os.path.join(self.streamDownloadURL, streamPath))
                self.testStreams.append(os.path.join(self.targetWorkspace, os.path.basename(streamPath)))
            self.downloadToDevice(url, self.targetWorkspace, self.rackDevice)

    def test_downloadSingleStream(self, format_index=0):
        """
        Downloads the specific stream for the HDR format being tested.

        Args:
            format_index (int): The index corresponding to the HDR format in the test setup.

        Returns:
            str: The full path of the downloaded stream.
        """
        url = []
        self.testStreams = []

        stream_name = ""

        if self.testSetup.assets.device.get(self.testName).streams != None:
            stream_name = self.testSetup.assets.device.get(self.testName).streams[format_index]
            if stream_name != "":
                url.append(os.path.join(self.streamDownloadURL, stream_name))
                self.testStreams.append(os.path.join(self.targetWorkspace, os.path.basename(stream_name)))
                # Download the stream to the device as a list
                self.downloadToDevice(url, self.targetWorkspace, self.rackDevice)
                return self.testStreams[0]

        return stream_name

    def testCleanSingleAsset(self):
        """
        Cleans up by removing a single stream from the DUT after it has been played.

        Args:
            stream_path (str): The full path of the stream to be deleted.
        """
        self.deleteFromDevice(self.testStreams)

    def testRunPrerequisites(self):
        """
        Runs Prerequisite commands listed in test-setup configuration file on the dut.

        Args:
            None.
        """

        #Run test specific commands
        cmds = self.testSetup.assets.device.get(self.testName).execute
        if cmds is not None:
            for cmd in cmds:
                self.writeCommands(cmd)

    def testEnablePort(self, port, index):
        """
        Enables a video port and verifies video output.

        Args:
            port (str): The video port to be enabled.
            index (int): The index of the video port.

        Returns:
            bool: The result of the video port verification.
        """
        # Enable video port
        self.testdsVideoPort.enablePort(port, index)

        # Enable HDCP for source devices
        if self.testdsVideoPort.getDeviceType():
            self.testdsVideoPort.enable_HDCP(port, index)

    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Creates dsVideoPortClass

        Returns:
            bool
        """
        # Download the assets listed in test setup configuration file
        #self.testDownloadAssets()
        self.test_downloadSingleStream()

        # Create the dsVideoPort class
        self.testdsVideoPort = dsVideoPortClass(self.moduleConfigProfileFile, self.hal_session, self.targetWorkspace)
        self.log.testStart(self.testName, self.qcId)
        self.testdsVideoPort.initialise()

        return True

    def testEndFunction(self, powerOff=True):

        self.testdsVideoPort.terminate()

        # Clean the assets downloaded to the device
        self.testCleanSingleAsset()

        self.log.testResult(f'{self.testName} Finished')
        # Clean up the dsVideoPort instance
        del self.testdsVideoPort

    def testExceptionCleanUp (self):
        # Clean the assets downloaded to the device
        self.testCleanAssets()