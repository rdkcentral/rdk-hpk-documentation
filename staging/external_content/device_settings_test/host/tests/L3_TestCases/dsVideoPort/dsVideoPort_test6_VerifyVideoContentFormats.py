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

class dsVideoPort_test6_VerifyVideoContentFormats(dsVideoPortHelperClass):
    """
    Test class for verifying video content formats on supported video ports.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test6_VerifyVideoContentFormats test case with required setup.

        Args:
            None.
        """
        self.testName  = "test6_VerifyVideoContentFormats"
        self.qcID      = '6'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyHDRFormat(self, manual=False,hdr_mode:str=0):
        """
        Verifies the HDR Format .

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool: True if verification successful, False otherwise.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f'is {hdr_mode} displayed on Analyzer (Y/N): ')
        else :
            #TODO: Add automation verification methods
            return False

    def testVerifyFormats(self, port, index):
        """
        Enables the given video port and verifies supported HDR formats.

        Args:
            port (str): The name of the video port.
            index (int): The port index.

        Returns:
            bool: Result of the HDR format verification.
        """

        # Verify HDR capabilities
        supported_formats = self.testdsVideoPort.getHDRCapabilities()
        if supported_formats:
            for hdr_format in supported_formats:
                self.testdsVideoPort.select_HDRModes(port, index, hdr_format)
                self.log.stepStart(f'Verify HDR {hdr_format} with Analyzer')
                result = self.testVerifyHDRFormat(manual=True, hdr_mode=hdr_format)

                # Log the verification result
                self.log.stepResult(result, f'HDR {hdr_format} verification result')
        else:
            self.log.error("No HDR formats available for verification.")
            result = False

        return result

    def testFunction(self):
        """
        Main test function to verify video ports by enabling/disabling them and verifying HDR formats.

        Returns:
            bool: Final test result.
        """

        # Loop through supported video ports and verify HDR formats
        for port, index in self.testdsVideoPort.getSupportedPorts():
            self.testEnablePort(port, index)
            result = self.testVerifyFormats(port, index)

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test6_VerifyVideoContentFormats(summeryLog)
    test.run(False)