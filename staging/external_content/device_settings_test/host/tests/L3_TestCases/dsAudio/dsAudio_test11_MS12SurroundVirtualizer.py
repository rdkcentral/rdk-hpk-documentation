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

class dsAudio_test11_MS12SurroundVirtualizer(dsAudioHelperClass):
    """
    Test class for MS12 Surround Virtualizer audio feature
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test11_MS12SurroundVirtualizer test.

        Sets up the required sessions and configurations for testing.

        Args:
            None.
        """
        # Class variables
        self.qcID = '11'
        self.testName  = "test11_MS12SurroundVirtualizer"
        self.ms12DAPFeature = "SurroundVirtualizer"

        #List of Volume modes for testing
        self.volumeModes = [0, 1, 2]
        self.boostValues = [0, 40, 96]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifySurroundVirtualizer(self, stream, port, mode, level, manual=False):
        """
        Verifies whether the audio playback is functioning correctly with the Surround Virtualizer settings.

        This can be performed through user input for manual verification or other methods.

        Args:
            stream (str) : The audio stream being tested.
            port (str) : The audio port to verify.
            mode (str): The SurroundVirtualizer mode being tested.
            level (int): The SurroundVirtualizer level to verify.
            manual (bool, optional): If True, indicates that manual verification is used.
                                     Defaults to False, meaning automatic verification methods will be applied.

        Returns:
            bool : The status of the audio verification (True if audio is fine, False otherwise).
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} mode {mode} level {level} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Tests the functionality of the MS12 Surround Virtualizer.

        This function:
        - Play the Audio Stream
        - Apply the Surround Virtualizer modes for supported ports
        - The main verification steps for testing the Surround Virtualizer feature.

        Returns:
            bool : The result of the last audio verification.
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

                    mode = 2 #leveller is Auto
                    boost = 0
                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} boost:{boost} Port:{port} Index:{index} Stream:{stream}')

                    # Set the SurroundVirtualizer
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":[mode, boost]})

                    result = self.testVerifySurroundVirtualizer(stream, port, mode, boost, True)

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} boost:{boost} Port:{port} Index:{index} Stream:{stream}')

                    mode = 1 #SurroundVirtualizer is On
                    boost = 0
                    for boost in self.boostValues:
                        self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} boost:{boost} Port:{port} Index:{index} Stream:{stream}')

                        # Set the SurroundVirtualizer
                        self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":[mode, boost]})

                        result = self.testVerifySurroundVirtualizer(stream, port, mode, boost, True)

                        self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} boost:{boost} Port:{port} Index:{index} Stream:{stream}')

                    mode = 0 #SurroundVirtualizer is OFF
                    boost = 0
                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} boost:{boost} Port:{port} Index:{index} Stream:{stream}')

                    # Set the SurroundVirtualizer
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":[mode, boost]})

                    result = self.testVerifySurroundVirtualizer(stream, port, mode, boost, True)

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} boost:{boost} Port:{port} Index:{index} Stream:{stream}')

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
    test = dsAudio_test11_MS12SurroundVirtualizer(summeryLog)
    test.run(False)
