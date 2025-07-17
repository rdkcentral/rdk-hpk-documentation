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

class dsAudio_test13_MS12GraphicEqualizer(dsAudioHelperClass):
    """
    Class to perform tests on the MS12 Graphic Equalizer feature.

    Inherits from dsAudioHelperClass and implements functionalities to test the
    Graphic Equalizer feature in the audio processing module.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test13_MS12GraphicEqualizer test.

        This constructor sets up the test environment, including configuration
        file loading, player and HAL session initialization, and user response
        handling.

        Args:
            None
        """
        # Class variables
        self.qcID = '13'
        self.testName  = "test13_MS12GraphicEqualizer"
        self.ms12DAPFeature = "GraphicEqualizer"

        # List of equalizer modes for testing
        self.equalizerModes = [0, 1, 2, 3]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyGraphicEqualizer(self, stream, port, mode, manual=False):
        """
        Verifies the effectiveness of the Graphic Equalizer feature.

        This method checks if the audio output is as expected when the Graphic
        Equalizer is set to a specific mode.

        Args:
            stream (str): The audio stream being tested.
            port (str): The audio port to verify.
            mode (int): The specific Graphic Equalizer mode to test.
            manual (bool, optional): If True, requests user confirmation for verification.
                                     Defaults to False (uses other verification methods).

        Returns:
            bool: The verification status of the audio output.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} mode {mode} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the main test for the MS12 Graphic Equalizer feature.

        This function orchestrates:
        - Initializing the audio module
        - Play the Audio Stream
        - Testing the Graphic Equalizer feature across various modes
        - Cleaning up afterward.

        Returns:
            bool: The final result of the test.
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

                        # Set the GraphicEqualizer
                        self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":mode})

                        result = self.testVerifyGraphicEqualizer(stream, port, mode, True)

                        self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} Port:{port} Index:{index} Stream:{stream}')

                    # Set the GraphicEqualizer to off
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":0})

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
    test = dsAudio_test13_MS12GraphicEqualizer(summeryLog)
    test.run(False)
