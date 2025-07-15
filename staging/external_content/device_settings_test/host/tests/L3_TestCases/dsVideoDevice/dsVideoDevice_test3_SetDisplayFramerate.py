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

class dsVideoDevice_test3_SetDisplayFramerate(dsVideoDeviceHelperClass):
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
        Initializes the test3_SetDisplayFramerate test .

        Args:
            None.
        """
        self.testName = "test3_SetDisplayFramerate"
        self.qcID = '3'
        self.testStreamFrameRates = ["23.98", "24", "25", "29.97", "30", "50", "59.94", "60"]
        super().__init__(self.testName, self.qcID, log)


    def testVerifyDisplayFramerate(self, StreamFrameRate:str, displayFramerate:str, manual=False):
        """
        Verifies the Display framerate on specified video device.

        Args:
            StreamFrameRate(str) : Stream frame rate
            displayFramerate(str): display frame rate
            manual (bool, optional): If True, requires manual confirmation from the user.
                                     Defaults to False.

        Returns:
            bool: True if selected FRF mode is visible in output device; otherwise, False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is a displayRate {displayFramerate} with a streamRate {StreamFrameRate} acceptable? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False


    def testFunction(self):
        """
        Executes the set display framerate tests on the video device.

        This function performs the following steps:
        - Set Display framerate
        - Cleans up the downloaded assets after testing.

        Returns:
            bool: Status of the last verification (True if successful, False otherwise).
        """

        # Initialize the dsVideoDevice module
        self.testdsVideoDevice.initialise(self.testdsVideoDevice.getDeviceType())

        # set the display framerate
        SupportedDevices = self.testdsVideoDevice.getSupportedVideoDevice()

        for device in SupportedDevices:
            #Disable Auto FRF mode
            self.testdsVideoDevice.setFRFMode(device, 'Disable')

            supported_framerate = self.testdsVideoDevice.getSupportedFrameRates()

            for displayFramerate in supported_framerate:
                self.testdsVideoDevice.setDisplayFramerate(device, displayFramerate)

                for streamUrl, StreamFrameRate in zip(self.streamPaths, self.testStreamFrameRates):
                    #Download the stream to device
                    streamPath = self.testDownloadSingleStream(streamUrl)
                    self.testPlayer.play(streamPath)
                    time.sleep(5)

                    self.log.stepStart(f'Check Framerate device:{device}, StreamFrameRate: {StreamFrameRate}, Display Framerate:{displayFramerate}')
                    result = self.testVerifyDisplayFramerate(StreamFrameRate, displayFramerate, True)
                    self.log.stepResult(result, f'Check Framerate device:{device}, StreamFrameRate: {StreamFrameRate}, Display Framerate:{displayFramerate}')

                    self.testPlayer.stop()
                    # Delete the stream
                    self.testDeleteSingleStream(streamPath)

        # Terminate dsVideoDevice Module
        self.testdsVideoDevice.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoDevice_test3_SetDisplayFramerate(summeryLog)
    test.run(False)
