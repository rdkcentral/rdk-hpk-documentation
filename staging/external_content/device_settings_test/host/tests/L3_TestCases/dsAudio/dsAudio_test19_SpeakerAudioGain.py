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

class dsAudio_test19_AudioGain(dsAudioHelperClass):
    """
    Test class for verifying audio gain levels in the dsAudio module.

    This class extends the dsAudioHelperClass and implements test procedures
    to evaluate the audio gain functionality of the device under test (DUT).
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test19_AudioGain test .

        Args:
            None.
        """
        # Class variables
        self.qcID = '19'
        self.testName  = "test19_AudioGain"

        # List Gain values for testing
        self.gainValues = [-2080.0, -1000.0, 0.0, 250.0, 480.0]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudioGainLevel(self, port, gain, manual=False):
        """
        Verifies the audio gain level applied on the specified audio port.

        This function prompts the user for manual verification or executes
        an automated method if implemented in the future.

        Args:
            port (str): The audio port to verify.
            gain (float): The gain level to check.
            manual (bool, optional): Indicates whether to use manual verification
                                     (True) or automated methods (False).
                                     Defaults to False.

        Returns:
            bool: Status of the audio verification (True if successful,
                  False otherwise).
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
        - Vlidating gain levels on Speaker port.

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
                if "SPEAKER" in port:
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    for gain in self.gainValues:
                        self.log.stepStart(f'Gain:{gain} Port:{port} Index:{index} Stream:{stream}')

                        # Set the gain level
                        self.testdsAudio.setSpeakerGain(port, index, gain)

                        result = self.testVerifyAudioGainLevel(port, gain, True)

                        self.log.stepResult(result, f'Gain:{gain} Port:{port} Index:{index} Stream:{stream}')

                    # Resetting the gain level to default
                    self.testdsAudio.setSpeakerGain(port, index, 0)

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
    test = dsAudio_test19_AudioGain(summeryLog)
    test.run(False)
