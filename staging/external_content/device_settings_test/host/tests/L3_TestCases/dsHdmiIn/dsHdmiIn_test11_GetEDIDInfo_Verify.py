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

class dsHdmiIn_test11_GetEDIDInfo_Verify(dsHdmiInHelperClass):
    """
    A test class for verifying the EDID information from HDMI input devices.

    This class extends `dsHdmiInHelperClass` and provides functionality
    to test the EDID info of HDMI inputs on the device.
    """


    def __init__(self, log:logModule=None):
        """
        Initializes the test11_GetEDID Info_Verify test .

        Args:
            None.
        """
        self.testName  = "test11_GetEDIDInfo_Verify"
        self.qcID = '11'
        super().__init__(self.testName, self.qcID, log)

    def testFunction(self):
        """
        The main test function to verify EDID information on HDMI input ports.

        This function:
        - Initializes the HDMI input module.
        - Loops through each supported HDMI port.
        - Checks the active status of each port and retrieves its EDID info.

        Returns:
            None
        """

        # Initialize the dsHDMIIn module
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

            #get EDID Info on particular Hdmi input to true and false
            edid_values = self.testdsHdmiIn.getEdidInfo(port)
            if edid_values:
                edid_list = self.moduleConfigProfile.fields.get("edidBytes")
                if edid_values[8] == str(edid_list[8]) and edid_values[9] == str(edid_list[9]):
                    self.log.stepResult(True,f'Verified EDID Info received on {port}')
                    result &= True
            else:
                self.log.stepResult(False,f' EDID Info Not received on {port}')
                result &= False

        self.log.stepResult(result,f"Verified EDID Info ")

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test11_GetEDIDInfo_Verify(summeryLog)
    test.run(False)
