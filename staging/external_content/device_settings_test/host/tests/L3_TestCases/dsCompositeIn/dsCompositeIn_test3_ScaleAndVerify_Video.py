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

from dsCompositeInHelperClass import dsCompositeInHelperClass
from raft.framework.core.logModule import logModule

class dsCompositeIn_test3_ScaleAndVerify_Video(dsCompositeInHelperClass):
    """
    Test class to scale and verify the video in CompositeIn device

    This class uses the `dscompositeInClass` to interact with the device's compositeIn ports,
    downloading necessary test assets, selecting
    CompositeIn ports, and performing verification of compositeIn output.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test3_ScaleAndVerify_Video test .

        Args:
            None.
        """
        self.testName  = "test3_ScaleAndVerify_Video"
        self.qcID = '3'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def CheckDeviceStatusAndVerifyVideoScale(self, manual=False, port_type:str=0, videoscale:str=0):
        """
        Verifies whether the particular port video scaled or not.

        Args:
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool
        """
        if manual == True and videoscale == False:
            return self.testUserResponse.getUserYN(f"Check CompositeIn source device connected&active on port {port_type} and press Y:")
        elif manual == True and videoscale == True:
            return self.testUserResponse.getUserYN(f"Check video scaled on {port_type} and press (Y/N)")
        else:
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        The main test function test scale video of dsCompositeIn device.

        This function:
        - Verifies video scaling .

        Returns:
            bool: Final result of the test.
        """

        # Initialize the dsCompositeIn module
        self.testdsCompositeIn.initialise()

        # x-coordiante, y-coordinate, width, height list
        videoScale_argList = [[0,0,720,576], [500,500,500,500], [0,0,1920,1080]]
        # Loop through the supported CompositeIn ports
        for port in self.testdsCompositeIn.getSupportedPorts():
            self.log.stepStart(f'Select {port} Port')

            # Check the ComposteIn device connected to is active
            portstr = f"dsCOMPOSITE_IN_PORT_{port}"
            # Select the ComposteIn port
            result = self.CheckDeviceStatusAndVerifyVideoScale(True,portstr,False)
            self.log.stepResult(result,f'CompositeIn Device connected {result} on {portstr}')
            self.testdsCompositeIn.selectPort(portstr)

            # video scaling of ComposteIn port
            for xvalue, yvalue, width, height in videoScale_argList:
                self.log.stepStart(f'Scale video playing on compositeIn device')
                self.testdsCompositeIn.scaleVideo(xvalue, yvalue, width, height)
                result = self.CheckDeviceStatusAndVerifyVideoScale(True,portstr,True)
                self.log.stepResult(result, f'CompositeIn Video Scale Verification {portstr} Port')

        # Terminate dsCompositeIn Module
        self.testdsCompositeIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsCompositeIn_test3_ScaleAndVerify_Video(summeryLog)
    test.run(False)