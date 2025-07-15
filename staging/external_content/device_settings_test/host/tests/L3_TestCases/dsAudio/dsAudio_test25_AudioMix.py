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

class dsAudio_test25_AudioMix(dsAudioHelperClass):
    """
    Class to perform audio mixing tests for the dsAudio module.

    This test checks the behavior of primary and system audio levels while playing audio streams.
    It verifies if the audio output behaves as expected for different volume settings.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test25_AudioMix test instance.

        Sets up the necessary sessions and reads the configuration for the test.

        Args:
            None.
        """
        # Class variables
        self.qcID = '25'
        self.testName  = "test25_AudioMix"

        # List of primary and secondary volume levels for testing
        self.primaryVolume = [0, 25, 50, 75, 100]
        self.systemVolume = [0, 25, 50, 75, 100]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudio(self, port, primary_volume, system_volume, manual=False):
        """
        Verifies if the audio is functioning as expected at given volume levels.

        Args:
            port (str) : The audio port to verify.
            primary_volume (int): Primary audio volume level (0-100).
            system_volume (int): System audio volume level (0-100).
            manual (bool, optional): Indicates if manual verification is required.
                                     Defaults to False (automated verification).

        Returns:
            bool: True if audio plays as expected, otherwise False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is Audio playing on {port} as expected with Mixing: Primary Volume: {primary_volume} System Volume: {system_volume}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the audio mixing test by verifying audio output with various volume settings.

        This function handles the overall test flow, including:
        - Plays the primary and secondary streams
        - setting audio levels
        - verifying audio output for different combinations of primary and system volumes.

        Returns:
            bool: True if the test executes successfully, otherwise False.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for port,index in self.testdsAudio.getSupportedPorts():
            # Enable the audio port
            self.testdsAudio.enablePort(port, index)

            for i in range(0, len(self.testStreams) - 1, 2):
                primaryStream = self.testStreams[i]
                systemStream = self.testStreams[i+1]

                # Start the primary stream playback
                self.testPlayer.play(primaryStream)

                # Start the system stream playback
                self.testSecondaryPlayer.play(systemStream)

                for prime in self.primaryVolume:
                    for system in self.systemVolume:
                        self.log.stepStart(f'Audio Mixing Stream: Port:{port} Primary Voulme: {prime}, System Volume: {system}')

                        self.testdsAudio.setAudioMixerLevels("dsAUDIO_INPUT_PRIMARY", prime)

                        self.testdsAudio.setAudioMixerLevels("dsAUDIO_INPUT_SYSTEM", system)

                        result = self.testVerifyAudio(port, prime, system, True)

                        self.log.stepResult(result, f'Audio Mixing Stream: Port:{port} Primary Voulme: {prime}, System Volume: {system}')

                self.testSecondaryPlayer.stop()

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
    test = dsAudio_test25_AudioMix(summeryLog)
    test.run(False)
