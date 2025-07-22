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

class dsVideoPort_test7_ResetToSDRMode(dsVideoPortHelperClass):
    """
    Test class for resetting video output to SDR mode and verifying playback.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test7_ResetToSDRMode test case with required setup.

        Args:
            None.
        """
        self.testName  = "test7_ResetToSDRMode"
        self.qcID      = '7'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyPlayback(self, manual=False):
        """
        Verifies whether the Video&audio displayed or not.

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool: True if playback is verified successfully, False otherwise.
        """
        if manual == True:
            return self.testUserResponse.getUserYN("Is Video PlayBack is HDR on the port? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Main test function that resets video output to SDR mode and verifies playback.

        Returns:
            bool: Final test result.
        """

        # Start the stream playback
        self.testPlayer.play(self.testStreams[0])

        # Loop through supported video ports and verify playback after resetting to SDR mode
        for port, index in self.testdsVideoPort.getSupportedPorts():
            # Reset output to SDR mode
            self.testEnablePort(port, index)
            self.testdsVideoPort.resetOutputToSDR(port, index)
            self.log.stepStart(f'Verify SDR playback of {port} ')
            result = self.testVerifyPlayback(manual=True)
            # Log the verification result
            self.log.stepResult(not result, "Verified parameters using HDMI Analyzer")

        # Stop the stream playback
        self.testPlayer.stop()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test7_ResetToSDRMode(summeryLog)
    test.run(False)