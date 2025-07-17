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

class dsHdmiIn_test12_GetSpdInfo_Verify(dsHdmiInHelperClass):
    """
    A test class for verifying the SPD information from HDMI input devices.

    This class extends `dsHdmiInHelperClass` and provides functionality
    to test the SPD info of HDMI inputs on the device.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test12_GetSpdInfo_Verify test .

        Args:
            None.
        """
        self.testName  = "test12_GetSpdInfo_Verify"
        self.qcID = '12'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def CheckDeviceAndInfoStatus(self, manual = False, port_type:str=0, spd_info:str=0):
        """
        Verifies the status of the device and whether SPD info is received on the specified port.

        Args:
            manual (bool, optional): Manual verification flag (True: manual, False: automated).
            port_type (str): The type of port being checked.
            spd_info (Optional[str]): SPD information status.

        Returns:
            bool: True if the status is verified, False otherwise.
        """
        if manual == True and spd_info == False:
            self.testUserResponse.getUserYN(f'Please connect the {port_type} and press Enter:')
            time.sleep(3)
            return self.testUserResponse.getUserYN(f'Is HdmiIn device connected and Displayed is ON {port_type} press Y/N:')
        elif manual == True and spd_info == True:
            return self.testUserResponse.getUserYN(f'check the Spd Info received of device connected to {port_type} and press Enter:')
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        The main test function to verify SPD information on HDMI input ports.

        This function:
        - Initializes the HDMI input module.
        - Loops through each supported HDMI port.
        - Checks the active status of each port and retrieves its SPD info.

        Returns:
            None
        """

        # Initialize the dsHDMIIn module
        self.testdsHdmiIn.initialise()
        result = True

        # Loop through the supported HdmiIn ports
        for port in self.testdsHdmiIn.getSupportedPorts():
            self.log.stepStart(f'{port} Port')

            # Check the HdmiIn device connected to is active
            status = self.CheckDeviceStatus(True,port)
            #self.log.step(f'Hdmi In Device is active on {port}')
            if not status:
                # Select the HdmiIn port
                self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
                time.sleep(5)
                self.log.step(f'Selected port {port} Port')

            self.testUserResponse.getUserYN(f'Please set the SPD Info {port} press Y/N:')
            #Getting Spd Info on particular Hdmi input port
            spd_values= self.testdsHdmiIn.getSpdInfo(port)
            if spd_values:
                spd_list = self.moduleConfigProfile.fields.get("spdInfo")
                if spd_values[13] == str(spd_list[13]) and spd_values[14] == str(spd_list[14]):
                    self.log.stepResult(True,f'Verified SPD Info received on {port}')
                    result &= True
                else:
                    self.log.stepResult(False,f'Wrong SPD Info received on {port}')
                    result &= False
            else:
                self.log.stepResult(False,f'SPD Info Not received on {port}')
                result &= False

        self.log.stepResult(result,f"Verified SPD Info ")

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test12_GetSpdInfo_Verify(summeryLog)
    test.run(False)
