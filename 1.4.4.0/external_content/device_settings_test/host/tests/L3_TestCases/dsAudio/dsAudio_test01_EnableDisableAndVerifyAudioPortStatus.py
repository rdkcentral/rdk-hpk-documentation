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

class dsAudio_test01_EnableDisableAndVerifyAudioPortStatus(dsAudioHelperClass):
    """
    Test class to enable, disable, and verify the status of audio ports on a device.

    This class uses the `dsAudioClass` to interact with the device's audio ports,
    downloading necessary test assets, playing audio streams, enabling and disabling
    audio ports, and performing verification of audio output.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            None
        """
        # Class variables
        self.testName  = "test01_EnableDisableAndVerifyAudioPortStatus"
        self.qcID = '1'

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudio(self, port, manual=False):
        """
        Verifies if the audio is working on the specified port.

        For manual verification, it prompts the user to confirm if audio is heard on the port.

        Args:
            port (str): The name of the audio port to verify.
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.

        Returns:
            bool: True if audio verification succeeds, False otherwise.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is audio playing on the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        The main test function that verifies audio ports by enabling and disabling them.

        This function:
        - Plays an audio stream and verifies audio output for each supported port.
        - Enables and disables audio ports while performing verification.

        Returns:
            bool: Final result of the test.
        """

        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        # Loop through the supported audio ports
        for port,index in self.testdsAudio.getSupportedPorts():
            # Start the stream playback
            self.testPlayer.play(self.testStreams[0])

            # Port Enable test
            self.log.stepStart(f'Enable {port} Port')
            # Enable the audio port
            self.testdsAudio.enablePort(port, index)
            result = self.testVerifyAudio(port, True)
            self.log.stepResult(result, f'Audio Verification {port} Port')

            # Port Disable test
            self.log.stepStart(f'Disable {port} Port')
            # Disable the audio port
            self.testdsAudio.disablePort(port, index)
            result = self.testVerifyAudio(port, True)
            self.log.stepResult(not result, f'Audio Verification {port} Port')

            # Stop the stream playback
            self.testPlayer.stop()

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test01_EnableDisableAndVerifyAudioPortStatus(summeryLog)
    test.run(False)
