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

class dsAudio_test17_OutputMode(dsAudioHelperClass):
    """
    Test case class for verifying the audio output modes of the Device Under Test (DUT).

    This class inherits from dsAudioHelperClass and implements various test methods to ensure
    proper functionality of audio output modes.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test17_OutputMode test instance.

        This constructor sets up the necessary configurations, including loading the test
        setup file, establishing console sessions for player and HAL, and initializing
        the player and user response classes.

        Args:
            None.
        """
        # Class variables
        self.qcID = '17'
        self.testName  = "test17_OutputMode"

        # List of Output modes for testing
        self.testOutputModes = [["STEREO", "PASSTHRU"], ["STEREO", "DD", "PASSTHRU"], ["STEREO", "DD", "DDPLUS", "PASSTHRU"]]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyOutputMode(self, stream, port, outMode, manual=False):
        """
        Verifies the audio output mode on the specified audio port during playback.

        Args:
            stream (str): The audio stream being tested.
            port (str): The audio port to verify the output mode.
            outMode (str): The expected output mode.
            manual (bool, optional): Flag to indicate if manual verification is required.
                                     Defaults to False (automated verification).

        Returns:
            bool: True if the output mode is correct; otherwise, False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Stream:{stream} Is output mode on the {port} {outMode}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the output mode tests for the audio ports by enabling/disabling each port.

        This method performs the following actions:
        - Initializes the audio module.
        - Plays the audio stream
        - Tests each supported audio port by setting and verifying output modes.

        Returns:
            bool: The final status of the output mode tests.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        # Loop through the supported audio ports
        for port,index in self.testdsAudio.getSupportedPorts():
            supportedModes = self.testdsAudio.getSupportedOutputModes(port,index)

            if(supportedModes == None or len(supportedModes) == 0):
                continue

            # Enable the audio port
            self.testdsAudio.enablePort(port, index)

            for i, stream in enumerate(self.testStreams):
                # Start the stream playback
                self.testPlayer.play(stream)

                for mode in self.testOutputModes[i]:
                    for smode in supportedModes:
                        if mode in smode:
                            self.log.stepStart(f'Test Outputmode Stream:{stream} Port:{port} index:{index} output mode:{smode}')
                            # Set OutputMode
                            self.testdsAudio.setOutputMode(port, index, smode)
                            result = self.testVerifyOutputMode(stream, port, smode, True)
                            self.log.stepResult(result, f'Test Outputmode Stream:{stream} Port:{port} index:{index} output mode:{smode}')
                            break
                # Stop the stream playback
                self.testPlayer.stop()
            # Disable the audio port
            self.testdsAudio.disablePort(port, index)

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test17_OutputMode(summeryLog)
    test.run(False)
