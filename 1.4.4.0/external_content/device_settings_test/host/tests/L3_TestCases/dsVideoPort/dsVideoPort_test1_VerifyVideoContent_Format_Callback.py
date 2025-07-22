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

# Get directory path and append to system path
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

# Import required classes from modules
from dsVideoPortHelperClass import dsVideoPortHelperClass
from raft.framework.core.logModule import logModule

class dsVideoPort_test1_VerifyVideoContent_Format_Callback(dsVideoPortHelperClass):

    """
    A test class for verifying video content formats via different video ports.

    Attributes:
        testName (str): Name of the test.
        qcID (str): Quality control ID for tracking.
        HDRFormats (list): List of supported HDR formats for testing.
    """

    def __init__(self, log:logModule=None):
        """
        Initializes the test class with necessary attributes and resources.

        Args:
            log (logModule): Logger instance for logging test activities.
        """
        self.testName  = "test1_VerifyVideoContent_Format_Callback"
        self.qcID      = '1'

        super().__init__(self.testName, self.qcID, log)
        self.HDRFormats = ["NONE", "HDR10", "HLG", "DolbyVision", "TechnicolorPrime", "HDR10PLUS", "SDR"]

    #TODO: Current version supports only manual verification.
    def find_VideoFormat_Status(self, input_str: str, status: str) -> bool:
        """
        Checks if a specific video format status is present in the input string.

        Args:
            input_str (str): The string to search in.
            status (str): The video format status to look for.

        Returns:
            bool: True if the status is found, False otherwise.
        """
        if status in input_str:
            return True
        return False

    def testFunction(self):
        """
        Main test method to verify video formats on supported ports.

        - Iterates through video ports and HDR formats.
        - Tests enabling/disabling ports and playing/stopping streams.
        - Validates the presence of expected video format callbacks.

        Returns:
            bool: Final result of video format callback verification.
        """

        # Step 3: Loop through the supported video ports and test HDR formats
        for port, index in self.testdsVideoPort.getSupportedPorts():
            self.testEnablePort(port, index)

            # Loop through HDR formats, download stream, play it, and then delete it
            for i, format in enumerate(self.HDRFormats):
                self.log.stepStart(f'Enable {port} Port')
                if format not in ["NONE", "TechnicolorPrime"]:
                    # Step 3.1: Download the stream for this HDR format
                    stream_path = self.test_downloadSingleStream(i)

                    # Step 3.2: Play the corresponding stream
                    self.testPlayer.play(stream_path)
                    time.sleep(10)
                result = self.testdsVideoPort.read_Callbacks("Video Format Callback dsHDRStandard_t:")

                if format not in "TechnicolorPrime":
                    self.log.stepResult(self.find_VideoFormat_Status(result, f'dsHDRSTANDARD_{format}'),f'{format} VideoFormat Callback found')

                if format not in ["NONE", "TechnicolorPrime"]:
                    # Stop the stream playback
                    self.testPlayer.stop()
                    time.sleep(0.1)
                    self.testCleanSingleAsset()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsVideoPort_test1_VerifyVideoContent_Format_Callback(summeryLog)
    test.run(False)
