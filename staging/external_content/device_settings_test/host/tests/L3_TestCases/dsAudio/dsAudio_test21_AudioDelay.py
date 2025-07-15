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

class dsAudio_test21_AudioDelay(dsAudioHelperClass):
    """
    Test case for validating audio delay settings on the Device Under Test (DUT).
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test21_AudioDelay test .

        Args:
            None.
        """
        # Class variables
        self.qcID = '21'
        self.testName  = "test21_AudioDelay"

        # List of delay (msec) values for testing
        self.delayList = [0, 100, 200]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudioDelay(self, port, delay, manual=False):
        """
        Verifies whether the specified audio delay is correctly applied.

        Args:
            port (str): The audio port to verify.
            delay (float): The delay value (in milliseconds) to check.
            manual (bool, optional): Flag indicating whether to perform manual verification.
                                     Defaults to False (automated methods).

        Returns:
            bool: True if the audio delay is confirmed, False otherwise.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has Audio Delay {delay} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """Executes the audio delay test logic.

        This function performs the following steps:
        - Initializes the dsAudio module.
        - Plays the audio stream
        - Sets various audio delay on all supported audio ports.
        - Cleans up the downloaded assets after testing.

        Returns:
            bool: The result of the final audio verification.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Start the stream playback
            self.testPlayer.play(stream)

            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                if "ARC" in port or "SPDIF" in port or "HDMI" in port:
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    for delay in self.delayList:
                        self.log.stepStart(f'Audio Delay {delay} Port:{port} Index:{index} Stream:{stream}')

                        # Set the Audio Delay
                        self.testdsAudio.setAudioDelay(port, index, delay)

                        result = self.testVerifyAudioDelay(port, delay, True)

                        self.log.stepResult(result, f'Audio Delay {delay} Port:{port} Index:{index} Stream:{stream}')

                    # Disable the audio port
                    self.testdsAudio.disablePort(port, index)

            # Stop the stream playback
            self.testPlayer.stop()

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test21_AudioDelay(summeryLog)
    test.run(False)
