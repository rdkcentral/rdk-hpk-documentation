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
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from dsAudioHelperClass import dsAudioHelperClass
from raft.framework.core.logModule import logModule

class dsAudio_test02_PortConnectionStatus(dsAudioHelperClass):
    """
    Test class to verify the connection and disconnection status of headphone.
    - Check the connection status of headphones.
    - Test callback mechanisms for headphone connection/disconnection.
    - Perform manual or automated connection status verification.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class with test name, setup configuration, and session for HAL testing.

        Args:
            None
        """
        # Class variables
        self.testName  = "test02_PortConnectionStatus"
        self.qcID = '2'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual.
    def testWaitForConnectionChange(self, connection, manual=False):
        """
        Waits for the headphone connection or disconnection.

        Args:
            connection (bool): Set to True for connection, False for disconnection.
            manual (bool, optional): Manual control flag (True for manual user input, False for automation).
                                     Defaults to False.

        Returns:
            None
        """
        if manual == True:
            if connection == True:
                self.testUserResponse.getUserYN(f"Connect the HEADPONE and press Enter:")
            else:
                self.testUserResponse.getUserYN(f"Disconnect the HEADPONE and press Enter:")
        else :
            # TODO: Implement automated connection change detection
            return False

    def testFunction(self):
        """
        The main test function that verifies headphone connection and disconnection.

        This function:
        - Verifies headphone connection and disconnection through callbacks and direct status checks.

        Returns:
            bool: Final result of the test.
        """

        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        # Wait for headphone connection
        self.testWaitForConnectionChange(True, True)

        self.log.stepStart('Headphone Connect Callback Test')

        callbackStatus = self.testdsAudio.getHeadphoneConnectionCallbackStatus()

        # Validate headphone connection callback
        if(callbackStatus == None or "HEADPHONE" not in callbackStatus[0] or callbackStatus[2] == False):
            result = False
        else:
            result = True

        self.log.stepResult(result, 'Headphone Connect Callback Test')

        self.log.stepStart('Headphone Connect Test')

        connectionStatus = self.testdsAudio.getHeadphoneConnectionStatus()

        self.log.stepResult(connectionStatus, 'Headphone Connect Test')

        # Wait for headphone disconnection
        self.testWaitForConnectionChange(False, True)

        self.log.stepStart('Headphone Disconnect Callback Test')

        callbackStatus = self.testdsAudio.getHeadphoneConnectionCallbackStatus()

        if(callbackStatus == None or "HEADPHONE" not in callbackStatus[0] or callbackStatus[2] == True):
            result = False
        else:
            result = True

        self.log.stepResult(result, 'Headphone Disconnect Callback Test')

        self.log.stepStart('Headphone Disconnect Test')

        connectionStatus = self.testdsAudio.getHeadphoneConnectionStatus()

        self.log.stepResult(not connectionStatus, 'Headphone Disconnect Test')

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return True

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test02_PortConnectionStatus(summeryLog)
    test.run(False)
