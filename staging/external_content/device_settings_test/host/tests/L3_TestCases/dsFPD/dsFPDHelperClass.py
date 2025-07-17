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
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.core.logModule import logModule
from dsClasses.dsFPD import dsFPDClass

class dsFPDHelperClass(utHelperClass):

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            testName (str) : name of the test
            qcId (str): QC ID of the test.
            log (class, optional): Parent log class. Defaults to None.
        """
        self.testName  = ""
        self.testSetupPath = os.path.join(dir_path, "dsFPD_L3_testSetup.yml")
        self.moduleName = "dsFPD"
        self.rackDevice = "dut"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)
        #open Hal Session
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")
        self.deviceTestSetup = self.cpe.get("test")

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, self.deviceTestSetup.get("profile"))

        self.targetPath = self.cpe.get("target_directory")
        self.targetPath = os.path.join(self.targetPath, self.moduleName)

    #TODO: Current version supports only manual verification.
    def testVerifyIndicator(self, indicator, state, manual=False):
        """
        Verifies whether the Front panel Indicator is set to specified state

        Args:
            indicator (str) : Front Panel Indicator
            state (str) : Front Panel Indicator State
            manual (bool, optional): Manual verification (True: manual, False: automated).
                                     Defaults to False

        Returns:
            bool : Returns the status of the indicator state verification.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is {indicator} state {state}? (Y/N):")
        else :
            #TODO: add automation verification methods
            return False


    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Creates dsFPDClass

        Returns:
            bool
        """

        # Create the dsFPD class
        self.testdsFPD = dsFPDClass(self.moduleConfigProfileFile, self.hal_session, self.targetPath)

        return True

    def testEndFunction(self, powerOff=True):

        # Clean up the dsFPD instance
        del self.testdsFPD