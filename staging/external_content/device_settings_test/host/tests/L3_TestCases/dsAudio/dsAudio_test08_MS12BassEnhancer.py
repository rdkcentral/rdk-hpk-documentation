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

class dsAudio_test08_MS12BassEnhancer(dsAudioHelperClass):
    """
    Test class for verifying the MS12 Bass Enhancer feature.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test08_MS12BassEnhancer test class.

        Sets up the necessary sessions and configurations for the test.

        Args:
            None.
        """
        # Class variables
        self.qcID = '8'
        self.testName  = "test08_MS12BassEnhancer"
        self.ms12DAPFeature = "BassEnhancer"

        # List of bass enhancer values for testing
        self.boostValues = [0, 50, 100]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyBassEnhancer(self, stream, port, boost, manual=False):
        """
        Verifies the audio quality with the Bass Enhancer applied.

        Args:
            stream (str): The audio stream used for testing.
            port (str): The audio port to verify.
            boost (int): The boost value for the Bass Enhancer (0-100).
            manual (bool, optional): Whether to use manual verification.
                                     Defaults to False (automated methods).

        Returns:
            bool: Status indicating whether the audio is satisfactory.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} boost {boost} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the test for the MS12 Bass Enhancer feature.

        This function orchestrates the test execution, including:
        - Plays audio streams
        - Verifying audio quality across various boost settings.
        - Cleans up the assets after the test completes.

        Returns:
            bool: Status of the last verification result.
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

                    for boost in self.boostValues:
                        self.log.stepStart(f'MS12 {self.ms12DAPFeature} boot:{boost} Port:{port} Index:{index} Stream:{stream}')

                        # Set the BassEnhancer
                        self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":boost})

                        result = self.testVerifyBassEnhancer(stream, port, boost, True)

                        self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} level:{boost} Port:{port} Index:{index} Stream:{stream}')

                    # Set the BassEnhancer to 0
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
    test = dsAudio_test08_MS12BassEnhancer(summeryLog)
    test.run(False)
