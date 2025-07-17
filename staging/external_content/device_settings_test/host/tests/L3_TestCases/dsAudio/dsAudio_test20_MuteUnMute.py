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

class dsAudio_test20_MuteUnMute(dsAudioHelperClass):
    """
    Class to perform mute and unmute tests on audio ports.

    Inherits from dsAudioHelperClass to leverage common test functionalities.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test20_MuteUnMute test .

        Args:
            None.
        """
        # Class variables
        self.qcID = '20'
        self.testName  = "test20_MuteUnMute"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyAudio(self, port, manual=False):
        """
        Verifies the audio output on the specified port.

        This method checks if audio is playing on the specified port and allows
        for manual confirmation if required.

        Args:
            port (str): The audio port to verify (e.g., "HDMI", "SPDIF").
            manual (bool, optional): If True, requires manual confirmation from the user.
                                     Defaults to False.

        Returns:
            bool: True if audio is playing; otherwise, False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is audio playing on the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the mute and unmute tests on the audio ports.

        This function performs the following steps:
        - Initializes the dsAudio module.
        - Plays the audio stream
        - Conducts mute and unmute tests on all supported audio ports.
        - Cleans up the downloaded assets after testing.

        Returns:
            bool: Status of the last verification (True if successful, False otherwise).
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Start the stream playback
            self.testPlayer.play(stream)
            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():

                # Enable the audio port
                self.testdsAudio.enablePort(port, index)

                self.log.stepStart(f'Mute Test Port:{port} Index:{index} Stream:{stream}')

                # Mute the Audio
                self.testdsAudio.setAudioMute(port, index, True)

                result = self.testVerifyAudio(port, True)

                self.log.stepResult(not result, f'Mute Test Port:{port} Index:{index} Stream:{stream}')

                self.log.stepStart(f'UnMute Test Port:{port} Index:{index} Stream:{stream}')

                # UnMute the Audio
                self.testdsAudio.setAudioMute(port, index, False)

                result = self.testVerifyAudio(port, True)

                self.log.stepResult(result, f'UnMute Test Port:{port} Index:{index} Stream:{stream}')

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
    test = dsAudio_test20_MuteUnMute(summeryLog)
    test.run(False)
