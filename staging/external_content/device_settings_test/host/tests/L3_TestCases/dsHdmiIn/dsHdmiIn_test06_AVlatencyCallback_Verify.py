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
from enum import Enum, auto

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from L3_TestCases.dsHdmiIn.dsHdmiInHelperClass import dsHdmiInHelperClass
from raft.framework.core.logModule import logModule

class dsHdmiIn_test06_AVlatencyCallback_Verify(dsHdmiInHelperClass):
    """
    A test class to verify the audio-video latency callback for HDMI input devices.

    This class inherits from `dsHdmiInHelperClass` and focuses on:
    - Verifying the audio and video latency updates through callback mechanisms.
    """


    def __init__(self, log:logModule=None):
        """
        Initializes the dsHdmiIn_test06_AVlatencyCallback_Verify test case.

        Sets the test name and calls the superclass constructor.
        """
        self.testName  = "test06_AVlatencyCallback_Verify"
        self.qcID = '6'
        super().__init__(self.testName, self.qcID, log)

    def testFunction(self):
        """
        The main test function that verifies audio-video latency of HDMI input devices.

        This function:
        - Initializes the HDMI input module.
        - Loops through each supported HDMI input port to verify latency.
        - Logs the results of the latency verification.

        Returns:
            bool: True if all checks are successful, otherwise False.
        """

        # Initialize the dsHdmiIn module
        self.testdsHdmiIn.initialise()
        result = True

        # Loop through the supported HdmiIn ports
        for port in self.testdsHdmiIn.getSupportedPorts():
            self.log.stepStart(f'Select {port} Port')
            self.log.step(f'Select {port} Port')

            # Check the HdmiIn device is active
            status = self.CheckDeviceStatus(True,port)
            if not status:
                self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
                time.sleep(5)
                self.log.step(f'Port Selected {port}')
            self.testUserResponse.getUserYN(f'Change the AV content for latency on {port} press Y/N:')
            avLatency = self.testdsHdmiIn.getAVlatencyCallbackStatus()
            if avLatency != None:
                self.log.step(f'audio_latency:{avLatency[0]}ms, videoLatency:{avLatency[1]}ms found in Callback')
                status = self.testUserResponse.getUserYN(f'Is AV Latency is changed on {port} press Y/N:')
                if status:
                    self.log.stepResult(True,f' AV latency callback found & Change is observed in port: {port}')
                    result &= True
                else:
                    self.log.stepResult(False,f' AV latency callback found & Change is Not observed in port: {port}')
                    result &= False
            else:
                self.log.step(f'No AV latency callback found in port: {port}')
                result &= False

        self.log.stepResult(result,f"AV Latency is change Verified ")

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test06_AVlatencyCallback_Verify(summeryLog)
    test.run(False)
