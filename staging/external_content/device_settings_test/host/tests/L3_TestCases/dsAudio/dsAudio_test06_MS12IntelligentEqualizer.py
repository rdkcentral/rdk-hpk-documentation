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

class dsAudio_test06_MS12IntelligentEqualizer(dsAudioHelperClass):
    """
    Test case for verifying the MS12 Intelligent Equalizer feature across various modes.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test case for MS12 Intelligent Equalizer.

        Sets up necessary sessions, config files, and utility classes required for the test.

        Args:
            None.
        """
        # Class variables
        self.qcID = '6'
        self.testName  = "test06_MS12IntelligentEqualizer"
        self.ms12DAPFeature = "IntelligentEqualizer"

        #List of Equalizer modes for testing
        self.equalizerModes = ["OFF", "Open", "Rich", "Focused", "Balanced", "Warm", "Detailed"]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyIntelligentEqualizer(self, stream, port, mode, manual=False):
        """
        Verifies whether the MS12 Intelligent Equalizer feature is applied correctly.

        Args:
            stream (str): Audio stream used for testing.
            port (str): Audio port being verified.
            mode (str): Intelligent Equalizer mode being tested.
            manual (bool, optional): Set to True for manual user verification, False for other methods.

        Returns:
            bool: Result of the verification (True if successful, False otherwise).
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} mode {mode} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Main function to test the MS12 Intelligent Equalizer feature.

        This function:
        - Plays audio streams
        - Apply Intelligent Equalizer modes on supported audio ports
        - verify the results.
        - Cleans up the assets after the test completes.

        Returns:
            bool: Final test result (True if the test passes, False otherwise).
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Start the stream playback
            self.testPlayer.play(stream)

            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                if self.testdsAudio.getMS12DAPFeatureSupport(port, index, self.ms12DAPFeature):
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    for mode in self.equalizerModes:
                        self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} Port:{port} Index:{index} Stream:{stream}')

                        # Set the Interlligent equalizer mode
                        self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":mode})

                        result = self.testVerifyIntelligentEqualizer(stream, port, mode, True)

                        self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} Port:{port} Index:{index} Stream:{stream}')

                    # Set the Interlligent equalizer mode to OFF
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":"OFF"})

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
    test = dsAudio_test06_MS12IntelligentEqualizer(summeryLog)
    test.run(False)
