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

class dsHdmiIn_test04_VideoModeChangeCallback_Verify(dsHdmiInHelperClass):
    """
    A test class to verify video mode change callbacks for HDMI input devices.

    This class inherits from `dsHdmiInHelperClass` and focuses on:
    - Validating the active state of HDMI input devices.
    - Verifying the correct video mode updates through callback mechanisms when the resolution changes.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the dsHdmiIn_test04_VideoModeChangeCallback_Verify test case.

        Sets the test name and calls the superclass constructor.
        """
        # Class variables
        self.testName  = "test04_VideoModeChangeCallback_Verify"
        self.qcID = '4'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def CheckDeviceStatusAndResolutionChange(self, manual=False, port_type:str=0, resolution_info:str=0):
       """
        Checks the status of the connected HDMI device and prompts for a resolution change.

        Args:
            manual (bool, optional): Indicates whether manual verification is required.
                                     Defaults to False (automation methods).
            port_type (str): HDMI port type as a string.
            resolution_info (bool, optional): Indicates whether to check for resolution change.
                                               Defaults to False.

        Returns:
            bool: True if the device is connected and/or resolution change is verified, otherwise False.
        """
       if manual == True and resolution_info == False:
           self.testUserResponse.getUserYN(f'Please connect the {port_type} and press Enter:')
           return self.testUserResponse.getUserYN(f'Is HDMI device connected and display is ON {port_type}? Y/N:')
       elif  manual == True and resolution_info == True:
            return self.testUserResponse.getUserYN(f'Change the Resolution on device connected to {port_type} and then press Enter:')
       else:
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        The main test function that verifies video mode updates for HDMI input devices.

        This function:
        - Initializes logging for the test.
        - Initializes the HDMI input module.
        - Loops through each supported HDMI input port:
          - Checks if the HDMI device is active.
          - Selects the appropriate HDMI port.
          - Checks for resolution change and verifies video mode updates through callbacks.
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

            # Check the HdmiIn device is active
            status = self.CheckDeviceStatusAndResolutionChange(True, port , False)
            if not status:
                self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
                time.sleep(5)
                self.log.step(f'Port Selected {port}')

            self.CheckDeviceStatusAndResolutionChange(True, port, True)
            time.sleep(5)
            videoMode = self.testdsHdmiIn.getVideoModeCallbackStatus()
            if videoMode != None and port == videoMode[0]:
                self.log.step(f'hdmi video mode port:{port} pixelResolution:{videoMode[1]} aspectRatio:{videoMode[2]} in Callback found')
                status = self.testUserResponse.getUserYN(f'Is the Resolution change on device port:{port} pixelResolution:{videoMode[1]} aspectRatio:{videoMode[2]}? Y/N: ')
                if status:
                    self.log.stepResult(True,f'hdmi video mode port:{port} Resolution , pixelResolution Verified ')
                    result &= True
                else:
                    self.log.stepResult(False,f'Resolution change on Not observed :{port} ')
                    result &= False

            elif videoMode == None:
                self.log.stepResult(False,f'hdmi video mode port:{port} in Callback Not found and Result{result}')
                result &= False


        self.log.stepResult(result,f"Verified Video modes ")

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test04_VideoModeChangeCallback_Verify(summeryLog)
    test.run(False)
