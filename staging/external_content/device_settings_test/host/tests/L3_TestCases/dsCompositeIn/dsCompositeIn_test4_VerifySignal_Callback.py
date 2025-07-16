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
sys.path.append(os.path.join(dir_path, "../../"))

from dsCompositeInHelperClass import dsCompositeInHelperClass
from raft.framework.core.logModule import logModule

class dsCompositeIn_test4_VerifySignal_Callback(dsCompositeInHelperClass):
    """
    Test class to verify the signal change callback of CompositeIn device

    This class uses the `dscompositeInClass` to interact with the device's compositeIn ports,
    downloading necessary test assets, selecting
    CompositeIn ports, and performing verification of compositeIn output.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test4_VerifySignal_Callback test .

        Args:
            None.
        """
        self.testName  = "test4_VerifySignal_Callback"
        self.qcID = '4'
        super().__init__(self.testName, self.qcID, log)

     #TODO: Current version supports only manual verification.
    def CheckDeviceStatus(self, manual=False, connect=False, port_type:str=0):
        """
        Verifies whether Composite Source Device connected or not

        Args:
            port_type (str) : CompositeIn port
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool
        """
        if manual == True and connect == True:
            return self.testUserResponse.getUserYN(f"Check if CompositeIn source is connected to {port_type} and press Y:")
        elif manual == True and connect == False:
            return self.testUserResponse.getUserYN(f"Check if CompositeIn source is disconnected to {port_type}/set to standby and press Y:")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        The main test function that verifies signal status of CompositeIn device.

        This function:
        - Verifies CompositeIn signal status through callbacks.

        Returns:
            bool: Final result of the test.
        """

        # Initialize the dsCompositeIn module
        self.testdsCompositeIn.initialise()

        # Loop through the supported CompositeIn ports
        for port in self.testdsCompositeIn.getSupportedPorts():

            # Check the CompositeIn device connected to is active
            portstr = f"dsCOMPOSITE_IN_PORT_{port}"
            for i in range(0, 2):
                self.log.stepStart(f'Select {port} Port')
                connect = True
                if ( i == 1): connect = False
                result = self.CheckDeviceStatus(True, connect, portstr)
                self.log.stepResult(result,f'CompositeIn Device connected {result} on {portstr}')

                self.testdsCompositeIn.selectPort(portstr)
                self.log.stepStart(f'Port Selcted {portstr}')

                status = self.testdsCompositeIn.getSignalChangeCallbackStatus()
                if status != None and portstr == status[0]:
                    self.log.stepResult(True,f'Signal status {status[1]} found in Callback')
                else:
                    self.log.stepResult(False,f'Signal status not found in Callback')

        # Terminate testdsCompositeIn Module
        self.testdsCompositeIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsCompositeIn_test4_VerifySignal_Callback(summeryLog)
    test.run(False)