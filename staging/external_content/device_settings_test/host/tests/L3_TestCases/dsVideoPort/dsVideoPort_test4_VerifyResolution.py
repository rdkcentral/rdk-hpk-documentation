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

class dsVideoPort_test4_VerifyResolution(dsVideoPortHelperClass):
    """
    Class for testing and verifying video resolution on supported ports.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test4_VerifyResolution test .

        Args:
            None.
        """
        self.testName  = "test4_VerifyResolution"
        self.qcID      = '4'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyResolution(self, manual=False,resolution:dict= None):
        """
        Verifies the Resolutions .

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool: True if verification successful, False otherwise.
        """
        if manual == True:
            # Query user confirmation for each resolution attribute and combine results logically
            result = True
            result &= self.testUserResponse.getUserYN(f'is {resolution.get("pixelResolution")} on HDMI Analyzer (Y/N): ')
            result &= self.testUserResponse.getUserYN(f'is {resolution.get("aspectRatio")} on HDMI Analyzer (Y/N): ')
            result &= self.testUserResponse.getUserYN(f'is {resolution.get("stereoScopicMode")} on HDMI Analyzer (Y/N): ')
            result &= self.testUserResponse.getUserYN(f'is {resolution.get("frameRate")} on HDMI Analyzer (Y/N): ')
            result &= self.testUserResponse.getUserYN(f'is {resolution.get("interlaced")} on HDMI Analyzer (Y/N): ')
            return result
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Main test function to verify resolution across all supported video ports.

        Returns:
            bool: Final result of the test.
        """

        # Enable each port and verify resolution
        for port, index in self.testdsVideoPort.getSupportedPorts():
            self.testEnablePort(port, index)
            for resolution in self.testdsVideoPort.getResolutions():
                self.log.stepStart(f'Verify {resolution} ')
                self.testdsVideoPort.select_Resolution(port, index, resolution)
                result = self.testVerifyResolution(manual=True, resolution=resolution)
                self.log.stepResult(result, "Resolution verified via HDMI Analyzer")

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test4_VerifyResolution(summeryLog)
    test.run(False)