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
from dsClasses.dsVideoDevice import dsVideoDeviceClass

class dsVideoDeviceHelperClass(utHelperClass):

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            testName (str) : name of the test
            qcId (str): QC ID of the test.
            log (class, optional): Parent log class. Defaults to None.
        """
        self.testName  = ""
        self.testSetupPath = os.path.join(dir_path, "dsVideoDevice_L3_testSetup.yml")
        self.moduleName = "dsVideoDevice"
        self.rackDevice = "dut"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")
        self.player_session = self.dut.getConsoleSession("ssh_player")

        deviceTestSetup = self.cpe.get("test")
        socVendor = self.cpe.get("soc_vendor")

        # Create player Class
        self.testPlayer = utPlayer(self.player_session, socVendor)

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))

        #self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = os.path.join(self.targetWorkspace, self.moduleName)
        self.streamDownloadURL = deviceTestSetup.get("streams_download_url")


    def testDownloadSingleStream(self, stream_url:str="") -> str:
        """
        Downloads a single stream listed in the test-setup configuration file to the dut.

        Args:
            stream_url (str): The URL of the stream to download.
        Return:
            Returns the stream path on device
        """
        url=[]
        self.testStreams = []
        if stream_url == "":
            self.streamPaths = self.testSetup.get("assets").get("device").get(self.testName).get("streams")
        # Download the specified stream to the device
        if stream_url != "":
            url.append(os.path.join(self.streamDownloadURL, stream_url))
            self.testStreams.append(os.path.join(self.targetWorkspace, os.path.basename(stream_url)))
            self.downloadToDevice(url, self.targetWorkspace, self.rackDevice)
            return self.testStreams[0]

        return None

    def testDeleteSingleStream(self, streamPath:str):
        """
        Removes the test stream from the DUT after test execution.

        Args:
            None
        """
        self.deleteFromDevice([streamPath])

    def testCleanAssets(self):
        """
        Removes the downloaded assets and test streams from the DUT after test execution.

        Args:
            None
        """
        self.deleteFromDevice(self.testStreams)


    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Creates dsVideoDeviceClass

        Returns:
            bool
        """

        self.testDownloadSingleStream()

        # Create the dsVideoDevice class
        self.testdsVideoDevice = dsVideoDeviceClass(self.moduleConfigProfileFile, self.hal_session, self.targetWorkspace)

        return True

    def testEndFunction(self, powerOff=True):
        # Clean up the dsVideoDevice instance
        self.testCleanAssets()
        del self.testdsVideoDevice

    def testExceptionCleanUp (self):
        # Clean the assets downloaded to the device
        self.testCleanAssets()