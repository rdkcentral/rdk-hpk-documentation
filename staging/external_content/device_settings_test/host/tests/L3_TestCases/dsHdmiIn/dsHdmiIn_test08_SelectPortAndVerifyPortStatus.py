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
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from L3_TestCases.dsHdmiIn.dsHdmiInHelperClass import dsHdmiInHelperClass
from raft.framework.core.logModule import logModule

class dsHdmiIn_test08_SelectPortAndVerifyPortStatus(dsHdmiInHelperClass):
    """
    A test class for selecting HDMI input ports and verifying their status.

    This class extends the `dsHdmiInHelperClass` and performs the following:
    - Selects supported HDMI input ports.
    - Verifies the status of the selected ports.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test08_SelectPortAndVerifyPortStatus test.

        Sets the test name and calls the superclass constructor.
        """
        self.testName  = "test08_SelectPortAndVerifyPortStatus"
        self.qcID = '8'
        super().__init__(self.testName, self.qcID, log)


    def testFunction(self):
        """
        The main test function that selects HDMI input ports and verifies their statuses.

        This function:
        - Initializes the HDMI input module.
        - Loops through each supported HDMI input port to verify selection status.

        Returns:
            bool: Final result of the test, indicating success or failure.
        """

        # Initialize the dsHdmiIn module
        self.testdsHdmiIn.initialise()
        result = True

        # Loop through the supported HdmiIn ports
        for port in self.testdsHdmiIn.getSupportedPorts():
            self.log.stepStart(f'Select {port} Port')
            self.log.step(f'Select {port} Port')

            # Check the HdmiIn device connected to is active
            status = self.CheckDeviceStatus(True, port)
            if not status:
                self.testdsHdmiIn.selectHDMIInPort(port, audMix=0, videoPlane=0, topmost=1)
                time.sleep(5)
                self.log.step(f'Port Selected {port}')
            portStatus = self.testdsHdmiIn.getHDMIInPortStatus()
            if portStatus != None and port == portStatus[1]:
                self.log.stepResult(True,f'HdmiIn Select Verification isPresented:{portStatus[0]} activePort:{portStatus[1]} and Result {result}')
                result &= True
            else:
                self.log.stepResult(False,f'Unable to get the Port status ')
                result &= False

        self.log.stepResult(result,f"Port Status Verified ")

        # Terminate dsHdmiIn Module
        self.testdsHdmiIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsHdmiIn_test08_SelectPortAndVerifyPortStatus(summeryLog)
    test.run(False)
