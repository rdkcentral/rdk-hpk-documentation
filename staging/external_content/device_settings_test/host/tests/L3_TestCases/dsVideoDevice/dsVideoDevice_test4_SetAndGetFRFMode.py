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
import time

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

from L3_TestCases.dsVideoDevice.dsVideoDeviceHelperClass import dsVideoDeviceHelperClass
from raft.framework.core.logModule import logModule

class dsVideoDevice_test4_SetAndGetFRFMode(dsVideoDeviceHelperClass):
    """
    Class to perform set and get the FRF mode on video device.

    Inherits from utHelperClass to leverage common test functionalities.

    Attributes:
        testName (str): Name of the test case.
        testSetupPath (str): Path to the test setup configuration file.
        moduleName (str): Name of the module being tested.
        rackDevice (str): Identifier for the device under test.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test4_FRFMode test .

        Args:
            None.
        """
        self.testName  = "test4_FRFMode"
        self.qcID = '4'
        self.testStreamFrameRates = ["23.98", "24", "25", "29.97", "30", "50", "59.94", "60"]
        self.exptectedMode = ["3840x2160px48", "3840x2160px48", "3840x2160px50", "3840x2160px60", "3840x2160px60", "3840x2160px50", "3840x2160px60", "3840x2160px60"]
        super().__init__(self.testName, self.qcID, log)


    def testVerifyFrameRateMode(self, frfmode, expectedMode:str, streamFramerate:str, manual=False):
        """
        Verifies the FRF mode on specified video device.

        Args:
            frfmode (bool) : FRF mode
            expectedMode (str): Expected display mode
            manual (bool, optional): If True, requires manual confirmation from the user.
                                     Defaults to False.

        Returns:
            bool: True if selected FRF mode is visible in output device; otherwise, False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has the display refresh rate been changed to match Stream Framerate:{streamFramerate} (Suggested mode is {expectedMode})? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the set and get FRF mode tests on the video device.

        This function performs the following steps:
        - Initializes the dsVideoDevice module.
        - Get the number of video devices
        - Set the FRF mode to Enable
        - Play the different frame rate video streams
        - Check the if the display rate changes according to the video framerate

        Returns:
            bool: Status of the last verification (True if successful, False otherwise).
        """

        # Initialize the dsVideoDevice module
        self.testdsVideoDevice.initialise(self.testdsVideoDevice.getDeviceType())

        result = True
        SupportedDevices = self.testdsVideoDevice.getSupportedVideoDevice()
        for device in SupportedDevices:
            self.testdsVideoDevice.setFRFMode(device, 'Enable')

            for streamUrl, frameRate, exptectedMode in zip(self.streamPaths, self.testStreamFrameRates, self.exptectedMode):
                streamPath = self.testDownloadSingleStream(streamUrl)
                self.testPlayer.play(streamPath)
                time.sleep(5)
                self.log.stepStart(f'Check Auto FRF mode device:{device}, Framerate:{frameRate}')
                result = self.testVerifyFrameRateMode(True, exptectedMode, frameRate, True)
                self.log.stepResult(result, f'Check Auto FRF mode device:{device}, Framerate:{frameRate}')

                self.testPlayer.stop()
                self.testDeleteSingleStream(streamPath)

            self.testdsVideoDevice.setFRFMode(device, 'Disable')

        # Terminate dsVideoDevice Module
        self.testdsVideoDevice.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoDevice_test4_SetAndGetFRFMode(summeryLog)
    test.run(False)
