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
import time

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utPlayer import utPlayer
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.core.logModule import logModule
from dsClasses.dsHdmiIn import dsHdmiInClass

class dsHdmiInHelperClass(utHelperClass):
    """
    A helper class to support HDMI In testing functionalities, including device setup,
    configuration loading, asset management, and user interactions for manual test validation.

    This class is initialized with test-specific parameters and includes methods to:
    - Download test artifacts
    - Exporting prerequisite commands using utplayer
    - Interact with the HDMI In device for manual checks
    - Manage test assets on the Device Under Test (DUT)
    """

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the dsHdmiInHelperClass with test-specific details, including test name,
        configuration paths, and logging setup.

        Args:
            testName (str): Name of the test.
            qcId (str): Quality control ID for the test.
            log (logModule, optional): Logging module instance. Defaults to None.
        """
        self.testName  = ""
        self.testSetupPath = os.path.join(dir_path, "dsHdmiIn_L3_testSetup.yml")
        self.moduleName = "dsHdmiIn"
        self.rackDevice = "dut"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        # Establish SSH session for accessing HDMI test functions on the DUT
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")

        # Set up paths and URLs for device test setup
        socVendor = self.cpe.get("soc_vendor")
        deviceTestSetup = self.cpe.get("test")

         # Create player 
        self.testPlayer = utPlayer(self.hal_session, socVendor)

        # User response interface for manual testing
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))
        self.moduleConfigProfile = ConfigRead( self.moduleConfigProfileFile , self.moduleName)

        self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = os.path.join(self.targetWorkspace, self.moduleName)

    #TODO: Current version supports only manual verification.
    def CheckDeviceStatus(self, manual=False, port_type:str=0):
        """
        Verifies whether the particular port video scaled or not.

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool
        """
        if manual == True:
            self.testUserResponse.getUserYN(f'Please connect the {port_type} and press Enter:')
            time.sleep(3)
            return self.testUserResponse.getUserYN(f'Is HdmiIn device connected and Displayed is ON {port_type} press Y/N:')
        else:
            #TODO: Add automation verification methods
            return False

    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Creates dsHdmiInClass

        Returns:
            bool
        """

        # Create the dsHdmiIn class
        self.testdsHdmiIn = dsHdmiInClass(self.moduleConfigProfileFile, self.hal_session, self.targetWorkspace)

        return True

    def testEndFunction(self, powerOff=True):
        # Clean up the dsHdmiIn instance
        del self.testdsHdmiIn
