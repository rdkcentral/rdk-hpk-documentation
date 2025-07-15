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

class dsAudio_test18_AudioLevel(dsAudioHelperClass):
    """
    Test class for validating audio gain levels on the device under test (DUT).

    This class inherits from `dsAudioHelperClass` and implements a series of tests
    to verify the audio gain levels across different audio ports.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the `dsAudio_test18_AudioLevel` instance.

        Sets up the test configuration, initializes necessary sessions,
        and prepares the player and user response handling classes.

        Args:
            None.
        """
        # Class variables
        self.qcID = '18'
        self.testName  = "test18_AudioLevel"

        # List of gain values for testing
        self.gainLevels = [0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudioGainLevel(self, port, gain, manual=False):
        """
        Verifies whether the specified audio gain level is correctly applied.

        Args:
            port (str): Audio port to verify.
            gain (float): Gain value to check.
            manual (bool, optional): Indicates whether to use manual verification.
                                     Defaults to False, which uses automated methods.

        Returns:
            bool: True if the audio gain level is correctly applied, otherwise False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has audio gain level {gain} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Main test function for validating audio gain levels.

        This method performs the following actions:
        - Initializes the audio module.
        - Plays the audio stream
        - Vlidating gain levels on supported audio ports.

        Returns:
            bool: The final status of the output mode tests.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Start the stream playback
            self.testPlayer.play(stream)

            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                if "HEADPHONE" in port or "SPEAKER" in port:
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    for gain in self.gainLevels:
                        self.log.stepStart(f'Gain Level:{gain} Port:{port} Index:{index} Stream:{stream}')

                        # Set the gain level
                        self.testdsAudio.setGainLevel(port, index, gain)

                        result = self.testVerifyAudioGainLevel(port, gain, True)

                        self.log.stepResult(result, f'Gain Level:{gain} Port:{port} Index:{index} Stream:{stream}')

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
    test = dsAudio_test18_AudioLevel(summeryLog)
    test.run(False)
