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

from dsAudioHelperClass import dsAudioHelperClass
from raft.framework.core.logModule import logModule

class dsAudio_test23_AssociateMix(dsAudioHelperClass):
    """
    Test class for verifying the functionality of audio mixing in the dsAudio module.

    This test validates the associate audio mixing feature of the device under test (DUT)
    by adjusting fader levels and checking audio playback with and without mixing enabled.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test23_AssociateMix test class.

        Sets up the necessary configuration, player session, and user response handling.

        Args:
            None.
        """
        # Class variables
        self.qcID = '23'
        self.testName  = "test23_AssociateMix"
        self.faderValues = [-32, 0, 32]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAssociateAudioMix(self, port, mixer_status, fader, manual=False):
        """
        Verifies the audio playback quality under the specified mixing conditions.

        Args:
            port (str) : The audio port to verify.
            mixer_status (bool): True if associate audio mixing is enabled, False otherwise.
            fader (int): Fader control value (e.g., -32 for mute, 32 for full).
            manual (bool, optional): Flag for manual verification (True for manual, False for automated).
                                     Defaults to False.

        Returns:
            bool: True if audio is as expected; otherwise, False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is Audio playing on {port} as expected with Mixing: {mixer_status} fader: {fader}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the associate mixing test.

        This method performs the following steps:
        - Initializes the dsAudio module.
        - Tests audio mixing by playing each stream with various fader settings.
        - Cleans up downloaded assets and terminates the dsAudio module.

        Returns:
            bool: True if the test execution completes successfully; otherwise, False.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for port,index in self.testdsAudio.getSupportedPorts():
            # Enable the audio port
            self.testdsAudio.enablePort(port, index)

            for stream in self.testStreams:

                self.log.stepStart(f'Associate Mixing Disabled, Stream: {stream} Fader: 0')

                self.testdsAudio.enableAssociateAudioMixig(False)

                # Start the stream playback
                self.testPlayer.play(stream)

                result = self.testVerifyAssociateAudioMix(port, False, 0, True)

                self.log.stepResult(result, f'Associate Mixing Disabled, Stream: {stream} Fader: 0')

                for fade in self.faderValues:
                    self.log.stepStart(f'Associate Mixing Stream: {stream} Fader: {fade}')

                    self.testdsAudio.enableAssociateAudioMixig(True, fade)

                    result = self.testVerifyAssociateAudioMix(port, True, fade, True)

                    self.log.stepResult(result, f'Associate Mixing Stream: {stream} Fader: {fade}')

                # Stop the stream playback
                self.testPlayer.stop()

            # Disable the audio port
            self.testdsAudio.disablePort(port, index)

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return True

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test23_AssociateMix(summeryLog)
    test.run(False)
