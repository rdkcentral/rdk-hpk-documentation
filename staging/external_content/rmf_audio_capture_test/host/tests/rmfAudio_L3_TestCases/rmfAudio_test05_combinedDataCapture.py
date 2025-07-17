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

class rmfAudio_test05_combinedDataCapture(rmfAudioHelperClass):
    """
    Test class to open capture Primary and Auxiliary Audio Data and verify it with their source.

    This class uses the `rmfAudioClass` to test data capture,
    downloading necessary test assets, setting up RMF Audio Capture
    and performing verification of captured audio.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            None
        """
        self.testName  = "test05_combinedDataCapture"
        super().__init__(self.testName, '5', log)

    def testAudioCapture(self, file_path:str, index:int):
        """
        Verifies if the captured audio data is a match with input reference

        Args:
            file_path (str): File name and path of the output wav file created
            index (int) : Index of file name to extract from streams

        Returns:
            bool: True if audio capture is a match, False otherwise.
        """
        ref_file_path = self.testStreams[index]
        return self.compareWavFiles(ref_file_path, file_path)

    def testFunction(self):
        """
        The main test function that verifies primary and auxiliary data capture with input audio.

        This function:
        - Starts Primary and Auxiliary Data capture.
        - Captures Audio data on primary and auxiliary interface at the same time.
        - Writes the captured audio data to a wav file.
        - Compares audio data captured with input audio.

        Returns:
            bool: Final result of the test.
        """
        result = False
        capture_duration = 10 # data capture duration
        settings_update = 0 # settings_update=0 (no updates to default settings), capture_format=1, sampling_rate=1, fifo_size=1, threshold=1
        # For settings update, set settings_update to 1, followed by value for capture_format, Sampling Frequency, FIFO size, threshold. 
        # Use -1 for settings parameter that you want to retain default value for.
        # Sample shows 1 for settings update, 0(racFormat_e16BitStereo) for capture format, 5(racFreq_e48000) for sampling rate, 16384 for FIFO and -1 to retain default threshold value.
        # sample : self.testrmfAudio.updateSettings(capture_type, 1, 0, 5, 16384, -1)
        test_type = 2 # test_type : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.
        wav_file_name = ["/tmp/output_primary.wav", "/tmp/output_auxiliary.wav"]
        result = []
        aux_support = self.testrmfAudio.checkAuxiliarySupport()

        if aux_support is True:
            self.log.stepStart('Combined audio capture')
            # 1 for primary data capture (default), 2 for auxiliary data capture.
            for capture_type in (1, 2):
                self.testrmfAudio.openHandle(capture_type)
                # Retain default Settings in this sample
                self.testrmfAudio.updateSettings(capture_type, settings_update)
                self.testrmfAudio.selectTestType(capture_type, test_type, capture_duration)

            for capture_type in (1, 2):
                ## TODO : Aux feature supported only in mock implementation now, enable below only for aux supported devices.
                ##index = capture_type - 1
                ##self.testPlayer.play(self.testStreams[index])
                self.testrmfAudio.startCapture(capture_type)

            time.sleep(capture_duration)

            for capture_type in (1, 2):
                self.testrmfAudio.stopCapture(capture_type)

            for capture_type in (1, 2):
                index = capture_type - 1
                self.testrmfAudio.writeWavFile(capture_type, wav_file_name[index])
                self.testrmfAudio.closeHandle(capture_type)

                result.append(self.testAudioCapture(wav_file_name[index], index))
            self.log.stepResult(all(result), 'Combined audio capture')
        else:
            self.log.stepResult(result, 'Auxiliary support in configuration file is False. Combined audio capture test not run')

        return result

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = rmfAudio_test05_combinedDataCapture(summaryLog)
    test.run(False)
