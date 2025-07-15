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

class dsAudio_test10_MS12DRCMode(dsAudioHelperClass):
    """
    Test case class for testing the MS12 Dynamic Range Control (DRC) Mode.

    This class inherits from dsAudioHelperClass and encapsulates the setup,
    execution, and validation of the DRC feature in the MS12 audio system.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test10_MS12DRCMode test instance.

        This constructor sets up necessary sessions and configurations
        for the DRC mode test.

        Args:
            None.
        """
        # Class variables
        self.qcID = '10'
        self.testName  = "test10_MS12DRCMode"
        self.ms12DAPFeature = "DRCMode"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyDRCMode(self, stream, port, mode, manual=False):
        """
        Verifies the audio output when the DRC mode is applied.

        This method checks if the audio is being processed correctly based on the mode.

        Args:
            stream (str) : The audio stream used for testing.
            port (str) : The audio port to verify.
            mode (bool): The current DRC mode (0: Line Mode, 1: RF Mode).
            manual (bool, optional): If True, prompts for manual verification;
                                     if False, uses automated verification methods. Defaults to False.

        Returns:
            bool : Returns the verification status of the audio output (True for success, False for failure).
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} {mode} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the test for the MS12 DRC Mode.

        This method orchestrates
        - Play the Audio Stream
        - Apply the DRC modes for supported ports
        - The main verification steps for testing the DRC feature.

        Returns:
            bool : The final verification result of the DRC mode test.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                if self.testdsAudio.getMS12DAPFeatureSupport(port, index, self.ms12DAPFeature):
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:DRC Line Mode Port:{port} Index:{index} Stream:{stream}')

                    # Set DRC Line Mode
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":0})

                    # Start the stream playback
                    self.testPlayer.play(stream)

                    result = self.testVerifyDRCMode(stream, port, 0, True)

                    # Stop the stream playback
                    self.testPlayer.stop()

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:DRC RF Mode Port:{port} Index:{index} Stream:{stream}')

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    # Set DRC RF Mode
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":1})

                    # Start the stream playback
                    self.testPlayer.play(stream)

                    result = self.testVerifyDRCMode(stream, port, 1, True)

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
    test = dsAudio_test10_MS12DRCMode(summeryLog)
    test.run(False)
