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

class dsVideoPort_test5_VerifyHDCP_Callback(dsVideoPortHelperClass):
    """
    Test class for verifying HDCP callback during HDMI plug/unplug.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test5_VerifyHDCP_Callback test with necessary setup.

        Args:
            None.
        """
        self.testName  = "test5_VerifyHDCP_Callback"
        self.qcID      = '5'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testUnplugHDMI(self, unplug:True, manual=False):
        """
        Verifies whether the Video&audio displayed or not.

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool
        """
        if manual == True:
            if unplug == True:
                return self.testUserResponse.getUserYN("UnPlug the HDMI Cable? (Y/N):")
            else :
                return self.testUserResponse.getUserYN("Plug the HDMI Cable? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def find_HDCPStatus(self, input_str: str, status: str) -> bool:
        """
        Finds HDCP status in a given input string.

        Args:
            input_str (str): The input log containing HDCP status.
            status (str): The HDCP status string to look for.

        Returns:
            bool: True if the status is found, False otherwise.
        """
        if status in input_str:
            return True
        return False

    def testVerifyHDCP(self ):
        """
        Enables a video port, performs HDMI plug/unplug actions, and verifies HDCP callbacks.

        Args:
            port (str): The video port name.
            index (int): The port index.

        Returns:
            bool: Result of the HDCP verification.
        """

        # Verify unplug HDMI event
        result = self.testUnplugHDMI(unplug=True, manual=True)
        result = self.testdsVideoPort.read_Callbacks("HDCP Status Callback dsHdcpStatus_t:")
        if self.find_HDCPStatus(result, "dsHDCP_STATUS_UNPOWERED"):
            self.log.stepResult(True, "HDMI Unplug Callback found")
        else:
            self.log.stepResult(False, "HDMI Unplug Callback Not found")

        # Verify plug HDMI event
        result = self.testUnplugHDMI(unplug=False, manual=True)
        result = self.testdsVideoPort.read_Callbacks("HDCP Status Callback dsHdcpStatus_t:")
        if self.find_HDCPStatus(result, "dsHDCP_STATUS_AUTHENTICATED"):
            self.log.stepResult(True, "HDMI Plug Callback found")
        else:
            self.log.stepResult(False, "HDMI Plug Callback Not found")

        return result

    def testFunction(self):
        """
        Main test function to verify HDCP status callbacks during HDMI plug/unplug events.

        Returns:
            bool: Final result of the test.
        """

        # Loop through all supported video ports and verify HDCP callbacks
        for port, index in self.testdsVideoPort.getSupportedPorts():
            self.testEnablePort(port, index)
            self.log.stepStart(f'Verify Callback of {port} ')
            result = self.testVerifyHDCP()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test5_VerifyHDCP_Callback(summeryLog)
    test.run(False)
