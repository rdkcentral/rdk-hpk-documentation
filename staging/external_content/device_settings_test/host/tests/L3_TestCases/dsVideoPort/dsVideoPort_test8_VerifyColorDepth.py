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

class dsVideoPort_test8_VerifyColorDepth(dsVideoPortHelperClass):
    """
    Test class for verifying color depth on video ports.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the dsVideoPortTest8VerifyColorDepth test.

        Args:
            None.
        """
        self.testName  = "test8_VerifyColorDepth"
        self.qcID      = '8'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyColorDepth(self, manual=False):
        """
        Verifies the Color Depth .

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool: Verification result.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f'is {self.testdsVideoPort.getColorDepth()} is same displayed on Analyzer (Y/N): ')
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the color depth verification test by enabling and disabling video ports.

        Returns:
            bool: Final verification result.
        """

        # Loop through the supported Video ports
        for port,index in self.testdsVideoPort.getSupportedPorts():
            self.log.step(f'Enable {port} Port')

            # Enable the Video port
            self.testEnablePort(port, index)

            self.testdsVideoPort.select_PreferredColorDepth(port, index, self.testdsVideoPort.getColorDepth())

            self.log.stepStart(f'Verify {self.testdsVideoPort.getColorDepth()} using Analyzer ')
            result = self.testVerifyColorDepth(True)

            self.log.stepResult(result, "All parameters verified using HDMI Analyzer")


        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test8_VerifyColorDepth(summeryLog)
    test.run(False)