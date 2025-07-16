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

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from L3_TestCases.dsHdmiIn.dsHdmiInHelperClass import dsHdmiInHelperClass
from raft.framework.core.logModule import logModule

class dsHdmiIn_test02_SignalChangeCallback_Verify(dsHdmiInHelperClass):
    """
    A test class to verify signal change callbacks from HDMI input devices.

    This class inherits from `dsHdmiInHelperClass` and is responsible for:
    - Checking the connection status of HDMI devices.
    - Verifying signal status changes through callback mechanisms.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the dsHdmiIn_test02_SignalChangeCallback_Verify test case.

        Sets the test name and invokes the superclass constructor.
        """
        # Class variables
        self.testName  = "test02_SignalChangeCallback_Verify"
        self.qcID = '2'
        super().__init__(self.testName, self.qcID, log)

     #TODO: Current version supports only manual verification.
    def connectDevice(self, manual=False, port_type:str=0):
        """
        Checks Device Power status is ON.

        Args:
            port (str) : HDMI port
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods
        Returns:
            bool
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f'connect device to {port_type} and press Enter:')
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        The main test function that verifies the signal status of HDMI input devices.

        This function performs the following actions:
        - Initiates the logging of the test.
        - Initializes the HDMI input module.
        - Loops through supported HDMI input ports to check signal statuses:
          - Prompts user to connect devices manually.
          - Selects the appropriate HDMI port.
          - Verifies the signal status through callback mechanisms.
        - Executes post-requisite commands defined in the test setup.
        - Cleans up by terminating the HDMI input module.

        Returns:
            bool: Final result of the test, True if all checks are successful, otherwise False.
        """

        # Initialize the dsHdmiIn module
        self.testdsHdmiIn.initialise()
        result = True

        # Loop through the supported HdmiIn ports
        for port in self.testdsHdmiIn.getSupportedPorts():
            self.log.stepStart(f'Select {port} Port')
            self.log.step(f'Select {port} Port')

            # Check the HdmiIn device connected to is active
            status = self.connectDevice(True, port)
            self.log.step(f'Hdmi In Device is active {result} on {port}')

            self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
            self.log.step(f'Port Selected {port}')

            status = self.testdsHdmiIn.getSignalChangeCallbackStatus()
            if status != None and port == status[0]:
                self.log.stepResult(True,f'Signal status {status[1]} found in Callback and Result{result}')
                result &= True

            else:
                self.log.stepResult(False,f'Signal status not found in Callback found and Result{result}')
                result &= False

        self.log.stepResult(result,f'Signal status Callbacks Verified')

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test02_SignalChangeCallback_Verify(summeryLog)
    test.run(False)
