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
from enum import Enum, auto

# Get directory path and append to system path
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

# Import required classes from modules
from dsVideoPortHelperClass import dsVideoPortHelperClass
from raft.framework.core.logModule import logModule

class dsVideoBackgroundColor(Enum):
    dsVIDEO_BGCOLOR_BLUE = 0   # Background color BLUE
    dsVIDEO_BGCOLOR_BLACK = 1  # Background color BLACK
    dsVIDEO_BGCOLOR_NONE = 2   # Background color NONE
    dsVIDEO_BGCOLOR_MAX = 3    # Out of range

class dsVideoPort_test9_VerifyBackgroundColor(dsVideoPortHelperClass):
    """
    Test class for verifying the background color of video ports.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test9_VerifyBackgroundColor test .

        Args:
            None.
        """
        self.testName  = "test9_VerifyBackgroundColor"
        self.qcID      = '9'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyBackGroundColor(self, manual=False,backgroundColor:str=0):
        """
        Verifies the HDCP Version .

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f'is {backgroundColor} displayed on Analyzer (Y/N): ')
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """This function will test the Video Ports by enabling and disabling the ports

        Returns:
            bool
        """

        # Loop through the supported Video ports
        for port,index in self.testdsVideoPort.getSupportedPorts():
            self.testEnablePort(port, index)

            for backgroundColor in list(dsVideoBackgroundColor):
                self.log.stepStart(f'Verify {backgroundColor} using Analyzer ')
                if backgroundColor != dsVideoBackgroundColor.dsVIDEO_BGCOLOR_MAX:
                    self.testdsVideoPort.select_BackgroundColor(port, index, dsVideoBackgroundColor(backgroundColor).name)
                    result = self.testVerifyBackGroundColor(True,dsVideoBackgroundColor(backgroundColor).name)
                    self.log.stepResult(result, f'{dsVideoBackgroundColor(backgroundColor).name} verified using HDMI Analyzer')

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test9_VerifyBackgroundColor(summeryLog)
    test.run(False)