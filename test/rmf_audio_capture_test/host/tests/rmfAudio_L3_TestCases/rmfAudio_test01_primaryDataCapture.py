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
sys.path.append(os.path.join(dir_path, "../"))

from rmfAudio_L3_TestCases.rmfAudioHelperClass import rmfAudioHelperClass
from raft.framework.core.logModule import logModule

class rmfAudio_test01_primaryDataCapture(rmfAudioHelperClass):
    """
    Test class to open capture Primary Audio Data and verify it with its source.

    This class uses the `rmfAudioClass` to test primary data capture,
    downloading necessary test assets, setting up RMF Audio Capture
    and performing verification of captured audio.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            None
        """
        self.testName  = "test01_primaryDataCapture"
        super().__init__(self.testName, '1', log)

    def testAudioCapture(self, file_path:str):
        """
        Verifies if the captured audio data is a match with input reference

        Args:
            file_path (str): File name and path of the output wav file created

        Returns:
            bool: True if audio capture is a match, False otherwise.
        """
        ref_file_path = self.testStreams[0]
        return self.compareWavFiles(ref_file_path, file_path)

    def testFunction(self):
        """
        The main test function that verifies primary data capture with input audio.

        This function:
        - Starts Primary Data capture.
        - Captures Audio data on primary interface.
        - Writes the captured audio data to a wav file.
        - Compares primary data captured with input audio.

        Returns:
            bool: Final result of the test.
        """
        result = False
        #Start playing reference stream
        self.testPlayer.play(self.testStreams[0])
        # 1 for primary data capture (default), 2 for auxiliary data capture.
        capture_type = 1
        capture_duration = 10 # data capture duration
        settings_update = 0 # settings_update=0 (no updates to default settings), 1 (update settings)
        # For settings update, set settings_update to 1, followed by value for capture_format, Sampling Frequency, FIFO size, threshold. 
        # Use -1 for settings parameter that you want to retain default value for.
        # Sample shows 1 for settings update, 0(racFormat_e16BitStereo) for capture format, 5(racFreq_e48000) for sampling rate, 16384 for FIFO and -1 to retain default threshold value.
        # sample : self.testrmfAudio.updateSettings(capture_type, 1, 0, 5, 16384, -1)
        test_type = 2 # test_type : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.
        primary_wav_file_name = "/tmp/output_primary.wav"

        self.log.stepStart('Primary audio capture')
        self.testrmfAudio.openHandle(capture_type)
        # Retain default Settings in this sample
        self.testrmfAudio.updateSettings(capture_type, settings_update)
        self.testrmfAudio.selectTestType(capture_type, test_type, capture_duration)

        self.testrmfAudio.startCapture(capture_type)

        time.sleep(capture_duration)

        self.testrmfAudio.stopCapture(capture_type)
        self.testPlayer.stop()
        self.testrmfAudio.writeWavFile(capture_type, primary_wav_file_name)
        self.testrmfAudio.closeHandle(capture_type)

        result = self.testAudioCapture(primary_wav_file_name)
        self.log.stepResult(result, 'Primary audio capture')

        return result

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = rmfAudio_test01_primaryDataCapture(summaryLog)
    test.run(False)
