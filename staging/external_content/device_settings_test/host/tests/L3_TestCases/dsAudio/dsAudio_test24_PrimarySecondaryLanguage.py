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

class dsAudio_test24_PrimarySecondaryLanguage(dsAudioHelperClass):
    """
    Test class for verifying the functionality of Primary and secondary languages.

    Attributes:
        testName (str): Name of the test.
        streamLanguage (dict): List of Dictionary of languages.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test24_PrimarySecondaryLanguage test .

        Args:
            None.
        """
        # Class variables
        self.qcID = '24'
        self.testName  = "test24_PrimarySecondaryLanguage"

        # List of lanuguages for testing
        self.streamLanguage = [{"Primary": ["eng", "spa", "zho"], "Secondary": ["eng", "spa", "zho"]}]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudio(self, port, language_type:str, language:str, manual=False):
        """
        Verifies the audio stream in the specified language (Primary or Secondary).

        The verification can be performed manually or through an automated method (if available).
        Currently, the manual method prompts the user to confirm whether the audio is playing
        correctly.

        Args:
            port (str) : The audio port to verify.
            language_type (str): Specifies the language type, either "Primary" or "Secondary".
            language (str): The 3-letter ISO 639-3 code representing the language (e.g., 'eng' for English).
            manual (bool, optional): If True, prompts the user for manual verification.
                                     Defaults to automated verification (when implemented).

        Returns:
            bool: The status of the audio verification (True if audio is correct, False otherwise).
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is Audio playing on {port} as expected with {language_type} {language} Language? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Tests the Primary and Secondary audio language switching functionality.

        This method validates the audio playback for both primary and secondary languages
        by configuring the audio streams as per the test setup, running the test, and verifying
        the results manually.

        Returns:
            bool: True if the test completes successfully.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for port,index in self.testdsAudio.getSupportedPorts():
            # Enable the audio port
            self.testdsAudio.enablePort(port, index)

            for i, stream in enumerate(self.testStreams):

                self.testdsAudio.enableAssociateAudioMixig(False)

                # Start the stream playback
                self.testPlayer.play(stream)

                for secondary in self.streamLanguage[i]["Secondary"]:
                    self.log.stepStart(f'Secondary Language Test Stream: {stream} Language: {secondary}')

                    self.testdsAudio.setPrimarySecondaryLanguage("Secondary", secondary)

                    result = self.testVerifyAudio(port, "Secondary", secondary, True)

                    self.log.stepResult(result, f'Secondary Language Test Stream: {stream} Language: {secondary}')

                # Stop the stream playback
                self.testPlayer.stop()

                time.sleep(3)

                # Start the stream playback
                self.testPlayer.play(stream)

                for primary in self.streamLanguage[i]["Primary"]:
                    self.log.stepStart(f'Primary Language Test Stream: {stream} Language: {primary}')

                    self.testdsAudio.setPrimarySecondaryLanguage("Primary", primary)

                    result = self.testVerifyAudio(port, "Primary", primary, True)

                    self.log.stepResult(result, f'Primary Language Test Stream: {stream} Language: {primary}')

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
    test = dsAudio_test24_PrimarySecondaryLanguage(summeryLog)
    test.run(False)
