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

class dsAudio_test09_MS12SurroundDecoder(dsAudioHelperClass):
    """
    Test case for the MS12 Surround Decoder feature in the dsAudio class
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test09_MS12SurroundDecoder test case.

        Sets up necessary configurations, sessions, and instances
        for testing the MS12 Surround Decoder.

        Args:
            None.
        """
        # Class variables
        self.qcID = '9'
        self.testName  = "test09_MS12SurroundDecoder"
        self.ms12DAPFeature = "SurroundDecoder"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifySurroundDecoder(self, stream, port, mode, manual=False):
        """
        Verifies the Surround Decoder's audio output.

        Args:
            stream (str): The audio stream used for testing.
            port (str): The audio port to verify.
            mode (bool): Indicates the Surround Decoder mode (True/False).
            manual (bool, optional): If True, prompts user for manual verification.

        Returns:
            bool: The status of the audio verification.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} {mode} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Main function to execute the MS12 Surround Decoder test.

        This Function:
        - Initializes the dsAudio class
        - Plays the Audio stream
        - Enables disables the Surround Decoder
        - Performs audio stream playback with verification.

        Returns:
            bool: The overall result of the test execution.
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

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    # Enable SurroundDecoder mode
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":True})

                    result = self.testVerifySurroundDecoder(stream, port, True, True)

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    # Disable SurroundDecoder mode
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":False})

                    result = self.testVerifySurroundDecoder(stream, port, False, True)

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} :{False} Port:{port} Index:{index} Stream:{stream}')

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
    test = dsAudio_test09_MS12SurroundDecoder(summeryLog)
    test.run(False)
