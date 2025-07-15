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

class dsHdmiIn_test05_AllmChangeCallback_Verify(dsHdmiInHelperClass):
    """
    A test class to verify the Auto Low Latency Mode (ALLM) change callbacks for HDMI input devices.

    This class inherits from `dsHdmiInHelperClass` and focuses on:
    - Verifying the active state of HDMI input devices.
    - Testing ALLM status updates through callback mechanisms.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the dsHdmiIn_test05_AllmChangeCallback_Verify test case.

        Sets the test name and calls the superclass constructor.
        """
        self.testName  = "test05_AllmChangeCallback_Verify"
        self.qcID = '5'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def CheckDeviceStatusAndEnableAllm(self, manual=False, port_type:str=0, allm_change:str=0, allm_input:str=0):
        """
        Checks if the HDMI input device is powered on and prompts for enabling ALLM.

        Args:
            manual (bool, optional): Indicates whether manual verification is required.
                                     Defaults to False (for automation methods).
            port_type (str): HDMI port type as a string.
            allm_change (bool, optional): Indicates if ALLM mode change is being requested.
                                           Defaults to False.
            allm_input (str): Desired ALLM input state ('ON' or 'OFF').

        Returns:
            bool: True if verification is successful; otherwise, False.
        """
        if manual == True and allm_change != True:
            self.testUserResponse.getUserYN(f'Please connect the {port_type} and press Enter:')
            time.sleep(3)
            return self.testUserResponse.getUserYN(f'Is HdmiIn device connected and Displayed is ON {port_type} press Y/N:')
        elif manual == True and allm_change != False and allm_input == 'ON':
            return self.testUserResponse.getUserYN(f'Change ALLM mode on Hdmi In device connected to {port_type} to {allm_input} if its in OFF and press Enter:')
        elif manual == True and allm_change != False and allm_input != 'ON':
            return self.testUserResponse.getUserYN(f'Change ALLM mode on Hdmi In device connected to {port_type} to {allm_input} and press Enter:')
        else:
            #TODO: Add automation verification methods
            return False

    def verify_allm_status(self, port, expected_status):
        """
        Checks if the HDMI input device is powered on and prompts for enabling ALLM.

        Args:
            manual (bool, optional): Indicates whether manual verification is required.
                                     Defaults to False (for automation methods).
            port_type (str): HDMI port type as a string.
            allm_change (bool, optional): Indicates if ALLM mode change is being requested.
                                           Defaults to False.
            allm_input (str): Desired ALLM input state ('ON' or 'OFF').

        Returns:
            bool: True if verification is successful; otherwise, False.
        """
        allmStatus = self.testdsHdmiIn.getAllmCallbackStatus()
        if allmStatus != None:
            actual_port, actual_status = allmStatus
        else:
            self.log.stepResult(False,f"No callbacks found on port: {port}")
            return False

        if actual_port == port and actual_status == expected_status:
            self.log.stepResult(True,f"ALLM mode: {actual_status} on port: {actual_port} confirmed in Callback")
            return True
        else:
            self.log.stepResult(False,f"Unexpected ALLM mode: {actual_status} on port: {actual_port} in Callback")
            return False

    def testFunction(self):
        """
        The main test function that verifies the ALLM status of HDMI input devices.

        This function:
        - Initializes logging for the test.
        - Initializes the HDMI input module.
        - Loops through each supported HDMI input port:
          - Checks if the HDMI device is powered on.
          - Selects the appropriate HDMI port.
          - Enables ALLM and verifies its status.
          - Disables ALLM and verifies its status.
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

            status = self.CheckDeviceStatusAndEnableAllm(True, port, False)
            if not status:
                self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
                time.sleep(5)
                self.log.step(f'Port Selected {port}')

            # Change ALLM to ON if it is OFF and verify
            self.CheckDeviceStatusAndEnableAllm(manual=True, port_type=port, allm_change=True, allm_input="ON")
            result &= self.verify_allm_status(port, "True")

            # Change ALLM to OFF and verify
            self.CheckDeviceStatusAndEnableAllm(manual=True, port_type=port, allm_change=True, allm_input="OFF")
            result &= self.verify_allm_status(port, "False")

        self.log.stepResult(result,f"ALLM mode: Verified ")

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test05_AllmChangeCallback_Verify(summeryLog)
    test.run(False)
