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

class rmfAudio_test03_independentDataCheck(rmfAudioHelperClass):
    """
    Test class to test primary and auxiliary captures run independently.

    This class uses the `rmfAudioClass` to test primary and auxiliary data capture,
    downloading necessary test assets, setting up RMF Audio Capture
    and performing both capture interfaces run independently.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            None
        """
        self.testName  = "test03_independentDataCheck"
        super().__init__(self.testName, '3', log)

    def testFunction(self):
        """
        The main test function that verifies primary and auxiliary data captures are independent.

        This function:
        - Starts Primary and Auxiliary Data captures.
        - Tracks bytes received for both interfaces
        - Writes the captured audio data to a wav file.
        - Stops, starts in a mixed sequence to make sure the bytes received shows capture is independent

        Returns:
            bool: Final result of the test.
        """
        result = False
        # 1 for primary data capture (default), 2 for auxiliary data capture.
        capture_type_primary = 1
        capture_type_auxiliary = 2
        sleep_duration = 2 #2 seconds to test
        settings_update = 0 # settings_update=0 (no updates to default settings), capture_format=1, sampling_rate=1, fifo_size=1, threshold=1
        # For settings update, set settings_update to 1, followed by value for capture_format, Sampling Frequency, FIFO size, threshold. 
        # Use -1 for settings parameter that you want to retain default value for.
        # Sample shows 1 for settings update, 0(racFormat_e16BitStereo) for capture format, 5(racFreq_e48000) for sampling rate, 16384 for FIFO and -1 to retain default threshold value.
        # sample : self.testrmfAudio.updateSettings(capture_type, 1, 0, 5, 16384, -1)
        test_type = 1 # test_type : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.
        aux_support = self.testrmfAudio.checkAuxiliarySupport()

        if aux_support is True:
            self.log.stepStart('Independent data test - check auxiliary runs independently')
            self.testrmfAudio.openHandle(capture_type_primary)
            # Retain default Settings in this sample
            self.testrmfAudio.updateSettings(capture_type_primary, settings_update)
            self.testrmfAudio.selectTestType(capture_type_primary, test_type)

            self.testrmfAudio.openHandle(capture_type_auxiliary)
            # Retain default Settings in this sample
            self.testrmfAudio.updateSettings(capture_type_auxiliary, settings_update)
            self.testrmfAudio.selectTestType(capture_type_auxiliary, test_type)

            ## TODO : Aux feature supported only in mock implementation now, enable below only for aux supported devices.
            ##self.testPlayer.play(self.testStreams[0])
            self.testrmfAudio.startCapture(capture_type_primary)
            self.testrmfAudio.startCapture(capture_type_auxiliary)
            
            time.sleep(sleep_duration)
            self.testrmfAudio.stopCapture(capture_type_primary)
            #Primary stopped, auxiliary running. Verify if bytes received for auxiliary has increased.
            primary_bytes_received_before, auxiliary_bytes_received_before = self.testrmfAudio.checkBytesReceived()
            time.sleep(sleep_duration)

            primary_bytes_received_after, auxiliary_bytes_received_after = self.testrmfAudio.checkBytesReceived()
            if (primary_bytes_received_before == primary_bytes_received_after) and (auxiliary_bytes_received_before < auxiliary_bytes_received_after):
                result = True
            else:
                result = False
            
            self.log.stepResult(result, 'Independent data test - check auxiliary runs independently')

            self.testrmfAudio.stopCapture(capture_type_auxiliary)
            self.log.stepStart('Independent data test - check primary runs independently')

            self.testrmfAudio.startCapture(capture_type_primary)
            self.testrmfAudio.startCapture(capture_type_auxiliary)

            time.sleep(sleep_duration)

            self.testrmfAudio.stopCapture(capture_type_auxiliary)
            #Primary running, auxiliary stopped. Verify if bytes received for primary has increased
            primary_bytes_received_before, auxiliary_bytes_received_before = self.testrmfAudio.checkBytesReceived()
            time.sleep(sleep_duration)

            primary_bytes_received_after, auxiliary_bytes_received_after = self.testrmfAudio.checkBytesReceived()
            if (primary_bytes_received_before < primary_bytes_received_after) and (auxiliary_bytes_received_before == auxiliary_bytes_received_after):
                result = True
            else:
                result = False

            self.log.stepResult(result, 'Independent data test - check primary runs independently')
            self.testrmfAudio.stopCapture(capture_type_primary)

            ## TODO : Aux feature supported only in mock implementation now, enable below only for aux supported devices.
            ##self.testPlayer.stop()

            self.testrmfAudio.closeHandle(capture_type_primary)
            self.testrmfAudio.closeHandle(capture_type_auxiliary)
        else:
            self.log.stepResult(result, 'Auxiliary support in configuration file is False. Independent data test not run')

        return result

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summary"
    summaryLog = logModule(summaryLogName, level=logModule.INFO)
    test = rmfAudio_test03_independentDataCheck(summaryLog)
    test.run(False)
