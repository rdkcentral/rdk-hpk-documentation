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

class dsAudio_test22_AudioFormat(dsAudioHelperClass):
    """
    Test class for validating audio format functionalities.

    This class is designed to test the audio formats supported by the device.
    It extends the utHelperClass to leverage its utility methods for device interaction
    and test execution.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test22_AudioFormat test.

        This constructor sets up the necessary configurations and prepares the
        test environment by initializing player and user response classes.

        Args:
            None
        """
        # Class variables
        self.qcID = '22'
        self.testName  = "test22_AudioFormat"

        # List of audio formats for testing
        self.audioFormats = ["PCM", "AC3", "EAC3", "AAC", "VORBIS", "WMA", "AC4", "EAC3_ATMOS", "AC4_ATMOS"]

        super().__init__(self.testName, self.qcID, log)

    def testFunction(self):
        """
        Executes the audio format test.

        This method:
        - Runs through the various audio formats
        - Playing associated streams
        - Verifying that the audio formats are correctly reported by the device.

        Returns:
            bool: Always returns True upon successful execution of the test.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        self.log.stepStart(f'Audio Format NONE Test')

        # Get the Audio Format
        audioFormat = self.testdsAudio.getAudioFormat()

        self.log.stepResult("NONE" in audioFormat, f'Audio Format NONE Test')

        for format, stream in zip(self.audioFormats, self.testStreams):
            # Start the stream playback
            self.testPlayer.play(stream)
            time.sleep(3)

            self.log.stepStart(f'Audio Format {format} Callback Test')

            cbAudioFormat = self.testdsAudio.getAudioFormatCallbackStatus()

            self.log.stepResult(cbAudioFormat and format in cbAudioFormat, f'Audio Format {format} Callback Test')

            self.log.stepStart(f'Audio Format {format} Test')

            # Get the Audio Format
            audioFormat = self.testdsAudio.getAudioFormat()

            self.log.stepResult(format in audioFormat, f'Audio Format {format} Test')

            # Stop the stream playback
            self.testPlayer.stop()

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return True

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test22_AudioFormat(summeryLog)
    test.run(False)
