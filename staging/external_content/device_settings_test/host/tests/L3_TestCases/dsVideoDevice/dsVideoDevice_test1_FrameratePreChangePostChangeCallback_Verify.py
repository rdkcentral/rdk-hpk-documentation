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

class dsVideoDevice_test1_FrameratePreChangePostChangeCallback_Verify(dsVideoDeviceHelperClass):
    """
    Class to verify display framerate change pre and post callback .

    Inherits from utHelperClass to leverage common test functionalities.

    Attributes:
        testName (str): Name of the test case.
        testSetupPath (str): Path to the test setup configuration file.
        moduleName (str): Name of the module being tested.
        rackDevice (str): Identifier for the device under test.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test1_FrameratePrePostChangeCallBack_Verify test .

        Args:
            None.
        """
        self.testName  = "test1_FrameratePrePostChangeCallBack_Verify"
        self.qcID = '1'
        super().__init__(self.testName, self.qcID, log)


    def checkDeviceStatus(self, manual=False):
        """
        Checks Display device is connected and Power status is ON.

        Args:
            port (str) : HDMI port
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods
        Returns:
            bool
        """
        if manual == True:
            return self.testUserResponse.getUserYN('check video display connected and in ON state and press Enter:')
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
            The main test function that verifies display framerate prechange in video device.

            This function:
            - Verifies display framerate prechange and postchange through callbacks.

            Returns:
                bool: Final result of the test.
         """


        # Initialize the dsVideoDevice module
        self.testdsVideoDevice.initialise(self.testdsVideoDevice.getDeviceType())

        result = True
        SupportedDevices = self.testdsVideoDevice.getSupportedVideoDevice()

        for device in SupportedDevices:
            self.testdsVideoDevice.setFRFMode(device, 'Enable')

            for streamUrl in self.streamPaths:
                streamPath = self.testDownloadSingleStream(streamUrl)
                self.testPlayer.play(streamPath)

                time.sleep(5)

                self.log.stepStart(f'Check Frame Rate Pre Post Change Callback device:{device}, Stream:{os.path.basename(streamUrl)}')
                frameRateChangeTime = self.testdsVideoDevice.getFrameratePrePostChangeCallbackStatus()

                if("Pre" in frameRateChangeTime):
                    preTime = frameRateChangeTime["Pre"]
                    if("Post" in frameRateChangeTime):
                        postTime = frameRateChangeTime["Post"]
                        self.log.stepResult(True, f'Check Frame Rate Pre Post Change Callback device:{device}, Stream:{os.path.basename(streamUrl)}')
                        self.log.step(f'Time Difference between Pre and Post FrameRate change: {postTime - preTime}')
                    else:
                        self.log.error("Post Framerate change callback not triggered")
                        self.log.stepResult(False, f'Check Frame Rate Pre Post Change Callback device:{device}, Stream:{os.path.basename(streamUrl)}')
                else:
                    self.log.error("Pre Framerate change callback not triggered")
                    self.log.stepResult(False, f'Check Frame Rate Pre Post Change Callback device:{device}, Stream:{os.path.basename(streamUrl)}')

                self.testPlayer.stop()

                self.testDeleteSingleStream(streamPath)

            self.testdsVideoDevice.setFRFMode(device, 'Disable')

        # Terminate dsVideoDevice Module
        self.testdsVideoDevice.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoDevice_test1_FrameratePreChangePostChangeCallback_Verify(summeryLog)
    test.run(False)
