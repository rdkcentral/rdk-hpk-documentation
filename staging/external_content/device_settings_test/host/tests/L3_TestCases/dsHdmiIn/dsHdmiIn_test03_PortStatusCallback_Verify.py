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

class dsHdmiIn_test03_PortStatusCallback_Verify(dsHdmiInHelperClass):
    """
    A test class to verify the port status callbacks from HDMI input devices.

    This class inherits from `dsHdmiInHelperClass` and is responsible for:
    - Checking the status of HDMI input ports.
    - Verifying that the status callbacks accurately reflect the active ports.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the dsHdmiIn_test03_PortStatusCallback_Verify test case.

        Sets the test name and invokes the superclass constructor.
        """
        self.testName  = "test03_PortStatusCallback_Verify"
        self.qcID = '3'
        super().__init__(self.testName, self.qcID, log)

    def testFunction(self):
        """
        The main test function that verifies the status of HDMI input ports.

        This function performs the following actions:
        - Initiates logging for the test.
        - Initializes the HDMI input module.
        - Loops through each supported HDMI input port:
          - Checks if the connected HDMI device is active.
          - Selects the appropriate HDMI port.
          - Verifies the port status through callback mechanisms.
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
            status = self.CheckDeviceStatus(True,port)
            #self.log.stepResult(result,f'Hdmi In Device is active {result} on {port}')
            if not status:
                # Select the HdmiIn port
                self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
                self.log.step(f'Selected port {port} Port')
            # video scaling of HdmiIn port

            status = self.testdsHdmiIn.getHdmiInPortCallbackStatus()
            if status !=None and status[1] == port:
                self.log.stepResult(True,f'Port Status isPresented:{status[0]} activePort:{status[1]} found in Callback')
                result &= True
            else:
                self.log.stepResult(False,f'Port Status CALLBACK NOT found and Result{result}')
                result &= False

        self.log.stepResult(result,f'Port Status Verified with Callbacks')

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test03_PortStatusCallback_Verify(summeryLog)
    test.run(False)
