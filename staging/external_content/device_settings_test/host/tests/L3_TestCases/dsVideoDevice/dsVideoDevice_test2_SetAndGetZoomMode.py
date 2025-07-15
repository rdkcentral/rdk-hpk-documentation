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

from L3_TestCases.dsVideoDevice.dsVideoDeviceHelperClass import dsVideoDeviceHelperClass
from raft.framework.core.logModule import logModule

class dsVideoDevice_test2_SetAndGetZoomMode(dsVideoDeviceHelperClass):
    """
    Class to perform set and get the Zoom mode on video device.

    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test2_ZoomMode test .

        Args:
            None.
        """
        self.testName = "test2_ZoomMode"
        self.qcID = '2'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyZoomMode(self, device:str, zoomMode:str, manual=False):
        """
        Verifies the zoom mode on specified video device.

        Args:
            device (str) : video device
            zoomMode (str): zoom mode applied
            manual (bool, optional): If True, requires manual confirmation from the user.
                                     Defaults to False.

        Returns:
            bool: True if zoommode applied; otherwise, False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is the Zoom mode {zoomMode} applied in the device: {device}? (Y/N):")
        else :
            return False

    def testFunction(self):
        """
        Executes the set and get tests on the video device.

        This function performs the following steps:
        - Set and Get the Zoom Mode and verify.

        Returns:
            bool: Status of the last verification (True if successful, False otherwise).
        """

        result = True

        # Initialize the dsVideoDevice module
        self.testdsVideoDevice.initialise(self.testdsVideoDevice.getDeviceType())

        SupportedDevices = self.testdsVideoDevice.getSupportedVideoDevice()
        for device in SupportedDevices:

            for stream in self.streamPaths:
                supportedZoomModes = self.testdsVideoDevice.getSupportedZoomModes(device)

                for zoomMode in supportedZoomModes:
                    streamPath = self.testDownloadSingleStream(stream)
                    self.testPlayer.play(streamPath)
                    self.log.stepStart(f'Zoom Mode test, device:{device}, zoomMode:{zoomMode}')
                    self.testdsVideoDevice.setZoomMode(device, zoomMode)
                    result = self.testVerifyZoomMode(device, zoomMode, True)
                    mode = self.testdsVideoDevice.getZoomMode(device)
                    self.log.stepResult(result and zoomMode in mode, f'Zoom Mode test, device:{device}, zoomMode:{zoomMode}')
                    self.testPlayer.stop()
                self.testDeleteSingleStream(streamPath)

        # Terminate dsVideoDevice Module
        self.testdsVideoDevice.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoDevice_test2_SetAndGetZoomMode(summeryLog)
    test.run(False)
