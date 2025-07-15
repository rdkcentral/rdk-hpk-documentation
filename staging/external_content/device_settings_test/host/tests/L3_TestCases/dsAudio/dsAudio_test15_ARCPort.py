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
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from dsAudioHelperClass import dsAudioHelperClass
from raft.framework.core.logModule import logModule

class dsAudio_test15_ARCPort(dsAudioHelperClass):
    """
    Test class for verifying ARC (Audio Return Channel) port functionality.

    This class handles the setup, execution, and cleanup of tests that check
    the connection and disconnection of ARC and eARC devices to the DUT (Device Under Test).
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test15_ARCPort test.

        Sets up the test by reading the configuration, establishing necessary
        sessions, and preparing the environment for the test execution.

        Args:
            None.
        """
        # Class variables
        self.qcID = '15'
        self.testName  = "test15_ARCPort"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual.
    def testWaitForConnectionChange(self, connection, type = "NONE", manual=False):
        """
        Waits for a change in port connection state (connection or disconnection).

        Depending on the specified parameters, it can prompt the user to
        connect or disconnect a device, or it can implement automated methods
        for managing the connection.

        Args:
            connection (bool): If True, waits for a connection; if False, waits for disconnection.
            type (str): Type of device being connected (e.g., "ARC", "eARC").
            manual (bool, optional): Indicates if manual intervention is required
                                     (True for manual, False for automated methods). Defaults to False.

        Returns:
            None
        """
        if manual == True:
            if connection == True:
                self.testUserResponse.getUserYN(f"Connect the {type} device and press Enter:")
            else:
                self.testUserResponse.getUserYN(f"Disconnect the ARC and press Enter:")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the main testing routine for verifying ARC and eARC port functionality.

        This method orchestrate
        - Checking the connection status of ARC/eARC devices.

        Returns:
            bool: Indicates the success of the test execution (True if successful).
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        # Loop through the supported audio ports and initialize ARC port
        for port,index in self.testdsAudio.getSupportedPorts():
            if "ARC" in port:
                # Enable the audio port
                self.testdsAudio.enablePort(port, index)

                self.log.stepStart('ARC Type Test No Device Connected')

                # Wait for ARC Port Disconnection
                self.testWaitForConnectionChange(False, "NONE", True)

                portType = self.testdsAudio.getConnectedARCType()

                self.log.stepResult(portType == "NONE", 'ARC Type Test No Device Connected')

                self.log.stepStart('ARC Test')

                # Wait for ARC device connection
                self.testWaitForConnectionChange(True, "ARC", True)

                portType = self.testdsAudio.getConnectedARCType()

                self.log.stepResult(portType == "ARC", 'ARC Test')

                self.log.stepStart('eARC Test')

                # Wait for ARC device connection
                self.testWaitForConnectionChange(True, "eARC", True)

                portType = self.testdsAudio.getConnectedARCType()

                self.log.stepResult(portType == "eARC", 'eARC Test')

                # Disable the audio port
                self.testdsAudio.disablePort(port, index)

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return True

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test15_ARCPort(summeryLog)
    test.run(False)
