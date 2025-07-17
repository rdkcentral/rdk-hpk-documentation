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

class dsCompositeIn_test1_VerifyConnect_Callback(dsCompositeInHelperClass):
    """
    Test class to connect, disconnect and verify the status of CompositeIn device

    This class uses the `dscompositeInClass` to interact with the device's compositeIn ports,
    downloading necessary test assets, selecting
    CompositeIn ports, and performing verification of compositeIn output.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test1_VerifyConnect_Callback test .

        Args:
            None.
        """
        self.testName  = "test1_VerifyConnect_Callback"
        self.qcID = '1'
        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testPlugUnplugtCompositeIn(self, Connect:False, manual=False, port_type:str=0):
        """
        Verifies whether the compositeIn connected or not.

        Args:
            disconnect (bool): Connect or Disconnect compositeIn source device
            manual (bool, optional): Manual verification (True: manual, False: other verification methods).
                                     Defaults to other verification methods

        Returns:
            bool
        """
        if manual == True:
            if Connect == True:
                return self.testUserResponse.getUserYN(f"Connect the compositeIn source device to Port {port_type} and press Y:")

            else :
                return self.testUserResponse.getUserYN(f"Disconnect the compositeIn source device to Port {port_type} and press Y:")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """This function will verify the connect/ disconnect status of CompositeIn device.

        This function:
        - Connect / disconnect the compositeIn port.
        - Verify the status through callbacks

        Returns:
            bool: Final result of the test.
        """

        # Initialize the dsCompositeIn module
        self.testdsCompositeIn.initialise()

        # Loop through the supported ports
        for port in self.testdsCompositeIn.getSupportedPorts():
            self.log.stepStart(f'Connect Status Test for Port {port}')

            portstr = f"dsCOMPOSITE_IN_PORT_{port}"
            result = self.testPlugUnplugtCompositeIn(True,True, portstr)

            status = self.testdsCompositeIn.getConnectionCallbackStatus()
            if status != None and portstr == status[0] and status[1] == True:
                self.log.stepResult(True, f'Connect Status Test for Port {portstr}')
            else:
                self.log.stepResult(False, f'Connect Status Test for Port {portstr}')

            self.log.stepStart(f'Disconnect Status Test for Port {portstr}')

            result = self.testPlugUnplugtCompositeIn(False,True, portstr)

            status = self.testdsCompositeIn.getConnectionCallbackStatus()
            if status != None and portstr == status[0] and status[1] == False:
                self.log.stepResult(True, f'Disconnect Status Test for Port {portstr}')
            else:
                self.log.stepResult(False, f'Disconnect Status Test for Port {portstr}')

        # Terminate testdsCompositeIn Module
        self.testdsCompositeIn.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsCompositeIn_test1_VerifyConnect_Callback(summeryLog)
    test.run(False)