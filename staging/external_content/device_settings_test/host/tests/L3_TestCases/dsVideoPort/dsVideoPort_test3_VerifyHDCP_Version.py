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

class dsVideoPort_test3_VerifyHDCP_Version(dsVideoPortHelperClass):
    """
    A class to test and verify HDCP version through supported video ports.

    Attributes:
        testName (str): Name of the test.
        testSetupPath (str): Path to the test setup configuration file.
        moduleName (str): Name of the module being tested.
        rackDevice (str): Device under test (DUT).
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test3_VerifyHDCP_Version test setup and configuration.

        Initializes sessions, reads the test setup, and prepares the user response.
        """
        self.testName  = "test3_VerifyHDCP_Version"
        self.qcID      = '3'

        super().__init__(self.testName, self.qcID, log)


    #TODO: Current version supports only manual verification.
    def testVerifyHDCPVersion(self, manual=False):
        """
        Verifies the HDCP version for the current port.

        Args:
            manual (bool, optional): If True, manual verification is done using user response; otherwise,
                                     automated verification is used (yet to be implemented).

        Returns:
            bool: Result of the HDCP version verification.
        """
        if manual == True:
            hdcpVersion = self.testdsVideoPort.getHDCPVersion()
            return self.testUserResponse.getUserYN(f'is {hdcpVersion} HDCP Version displayed on Analyzer (Y/N): ')
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Main test function that enables video ports and verifies the HDCP version.

        Downloads assets, runs prerequisites, enables/disables ports, and verifies HDCP version.

        Returns:
            bool: Final result of the HDCP version verification.
        """

        # Loop through supported video ports and verify HDCP version
        for port, index in self.testdsVideoPort.getSupportedPorts():
            self.testEnablePort(port, index)
            # Set HDMI preference to the current HDCP version
            self.testdsVideoPort.select_HdmiPreference(port, index, self.testdsVideoPort.getHDCPVersion())

            # Verify HDCP version
            self.log.stepStart(f'Verify {self.testdsVideoPort.getHDCPVersion()} Version')
            result = self.testVerifyHDCPVersion(True)
            self.log.stepResult(result, f'Verified {self.testdsVideoPort.getHDCPVersion()} Version')

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test3_VerifyHDCP_Version(summeryLog)
    test.run(False)
