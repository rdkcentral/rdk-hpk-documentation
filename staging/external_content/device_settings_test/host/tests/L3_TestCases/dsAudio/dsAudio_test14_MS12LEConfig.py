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

class dsAudio_test14_MS12LEConfig(dsAudioHelperClass):
    """
    This class implements the audio test for MS12 LEConfig functionality.

    It inherits from dsAudioHelperClass and manages the setup, execution, and
    verification of the audio configuration tests for the LEConfig feature.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test14_MS12LEConfig test.

        This constructor sets up the test environment, including loading the
        configuration, opening necessary sessions, and preparing the player.

        Args:
            None
        """
        # Class variables
        self.qcID = '14'
        self.testName  = "test14_MS12LEConfig"
        self.ms12DAPFeature = "LEConfig"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyLEConfig(self, stream, port, mode, manual=False):
        """
        Verifies the functionality of the LEConfig feature.

        This method checks whether the audio output is correct based on the
        applied LEConfig settings.

        Args:
            stream (str): The audio stream used for testing.
            port (str): The audio port to verify.
            mode (bool): The state of LEConfig (enabled/disabled).
            manual (bool, optional): Flag for manual verification.
                                     Defaults to False (automated methods).

        Returns:
            bool: Status indicating whether the audio is functioning correctly
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} {mode} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the main test function for the MS12 LEConfig feature.

        This method orchestrates
        - Initializing the audio module
        - Play the Audio Stream
        - Testing the LEConfig feature
        - Cleaning up afterward.

        Returns:
            bool: Overall result of the test execution.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                if self.testdsAudio.getMS12DAPFeatureSupport(port, index, self.ms12DAPFeature):
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    # Enable LEConfig
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":True})

                    # Start the stream playback
                    self.testPlayer.play(stream)

                    result = self.testVerifyLEConfig(stream, port, True, True)

                    # Stop the stream playback
                    self.testPlayer.stop()

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    # Disable LEConfig
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":False})

                    # Start the stream playback
                    self.testPlayer.play(stream)

                    result = self.testVerifyLEConfig(stream, port, False, True)

                    # Stop the stream playback
                    self.testPlayer.stop()

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} :{False} Port:{port} Index:{index} Stream:{stream}')

                    # Disable the audio port
                    self.testdsAudio.disablePort(port, index)

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test14_MS12LEConfig(summeryLog)
    test.run(False)
