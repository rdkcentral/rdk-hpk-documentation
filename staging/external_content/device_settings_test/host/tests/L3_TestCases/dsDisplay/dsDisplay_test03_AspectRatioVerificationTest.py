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

from dsDisplayHelperClass import dsDisplayHelperClass
from raft.framework.core.logModule import logModule

class dsDisplay_test03_AspectRatioVerificationTest(dsDisplayHelperClass):

    """
    Test class to retrieve and verify the display aspect ratio.

    This class uses the `dsDisplayClass` to interact with the device's display,
    downloading necessary test assets, retrieving the aspect ratio, and performing verification.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test03_GetDisplayAspectRatio test .

        Args:
            None.
        """
        # Class variables
        self.testApectRatios = ["16x9", "4x3"]
        self.testName  = "test03_AspectRatioVerificationTest"
        self.qcID = '3'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testChangeDisplayAspectRatio(self, port:str, aspectRatio:str, manual=False):
        """
        Gets the aspectratio of the display.
        Args:
            port (str): Port name
            aspectRatio (str) : aspectRatio ration eg: 4x3, 16x9
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                    Defaults to other verification methods
        Returns:
            None
        """
        self.testUserResponse.getUserYN(f"Set the aspectratio {aspectRatio} to Port: {port} and Press Enter:")

    def testFunction(self):
        """
        This function will test the Display by getting the aspectratio of the display.

        This function:
        - Retrieves aspectratio for each supported port and verifies them.
        - Cleans up assets after the test.

        Returns:
            bool: Final result of the test.
        """

        # Initialize the dsDisplay module
        self.testdsDisplay.initialise()

        result = False
        # Loop through the supported video ports
        for port, index in self.testdsDisplay.getSupportedPorts():
            self.testdsDisplay.selectDisplayPort(port, index)

            for aspectRatio in self.testApectRatios:
                self.log.stepStart(f'Test Display Apect Ration {aspectRatio} Port: {port}')
                self.testChangeDisplayAspectRatio(port, aspectRatio, True)
                ratio = self.testdsDisplay.getAspectRatio()
                result = ratio and aspectRatio in ratio
                self.log.stepResult(result, f'Test Display Apect Ration {aspectRatio} Port: {port}')

        #Terminate dsDisplay Module
        self.testdsDisplay.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsDisplay_test03_AspectRatioVerificationTest(summeryLog)
    test.run(False)
