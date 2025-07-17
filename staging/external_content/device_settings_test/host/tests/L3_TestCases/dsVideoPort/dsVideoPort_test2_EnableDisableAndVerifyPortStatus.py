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

# Get directory path and append to system path
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

# Import required classes from modules
from dsVideoPortHelperClass import dsVideoPortHelperClass
from raft.framework.core.logModule import logModule


class dsVideoPort_test2_EnableDisableAndVerifyPortStatus(dsVideoPortHelperClass):
    """
    A class to test the display connectivity via different video ports.

    Attributes:
        testName (str): The name of the test.
        testSetupPath (str): Path to the test setup configuration file.
        moduleName (str): Module name for the test.
        rackDevice (str): Device under test.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test and sets up sessions and player classes.

        Initializes test setup, player, and HAL test sessions, and prepares the
        user response and device profile paths.
        """
        self.testName = "test2_EnableDisableAndVerifyPortStatus"
        self.qcID = '2'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyDisplay(self, manual=False):
        """
        Verifies whether video is being displayed or not.

        Args:
            manual (bool, optional): Indicates if manual verification is required.
                                     Defaults to False (automatic verification).

        Returns:
            bool: Verification result (True if the video is displayed, False otherwise).
        """
        if manual == True:
            return self.testUserResponse.getUserYN("Is Video Display on the port? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testDisablePortAndVerify(self, port, index):
        """
        Disables a video port and verifies that no video is displayed.

        Args:
            port (str): The video port to be disabled.
            index (int): The index of the video port.

        Returns:
            bool: The result of the video port disable verification.
        """
        self.testdsVideoPort.disablePort(port, index)

        # Verify that the video is no longer displayed
        self.log.stepStart(f'Verify {port} Port')
        result = not self.testVerifyDisplay(True)
        self.log.stepResult(result, f'VideoPort Disable Verification {port} Port')
        return result

    def testFunction(self):
        """
        The main test function that checks display connectivity by enabling and disabling video ports.

        This function downloads assets, runs prerequisites, enables/disables ports, and verifies
        display connectivity for each port.

        Returns:
            bool: The result of the final video verification.
        """

        self.testPlayer.play(self.testStreams[0])  # Start playback
        #self.testdsVideoPort.initialise()

        # Loop through supported video ports and verify connectivity
        for port, index in self.testdsVideoPort.getSupportedPorts():
            # Enable and verify the video port
            self.testEnablePort(port, index)
            # Verify video display
            self.log.stepStart(f'Verify {port} Port')
            result = self.testVerifyDisplay(True)
            self.log.stepResult(result, f'VideoPort Verification {port} Port')

            # Disable and verify the video port
            result = self.testDisablePortAndVerify(port, index)

        #self.testdsVideoPort.terminate()
        self.testPlayer.stop()  # Stop playback

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test2_EnableDisableAndVerifyPortStatus(summeryLog)
    test.run(False)

