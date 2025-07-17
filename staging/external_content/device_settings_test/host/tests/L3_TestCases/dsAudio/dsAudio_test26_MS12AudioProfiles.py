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

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path))
sys.path.append(os.path.join(dir_path, "../../"))

from dsAudioHelperClass import dsAudioHelperClass
from raft.framework.core.logModule import logModule

class dsAudio_test26_MS12AudioProfiles(dsAudioHelperClass):
    """
    This class represents the 'test26_MS12AudioProfiles' test case, which is responsible
    for verifying Dolby MS12 audio profiles on different audio ports of the Device Under Test (DUT).
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the 'test26_MS12AudioProfiles' test by setting up the test environment.

        Args:
            None.
        """
        # Class variables
        self.qcID = '26'
        self.testName  = "test26_MS12AudioProfiles"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyMS12AudioProfile(self, port, profile, manual=False):
        """
        Verifies if the specified MS12 audio profile is applied correctly on the given audio port.

        This can be done either manually (by asking the user) or through an automated verification method (not yet implemented).

        Args:
            port (str): The audio port to verify (e.g., HDMI, Optical).
            profile (str): The MS12 audio profile to be tested (e.g., Off, Music, Movie, etc.).
            manual (bool, optional): If True, manual verification is used. Defaults to False (automated verification).

        Returns:
            bool: True if the profile is correctly applied, otherwise False.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has audio profile {profile} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the 'test26_MS12AudioProfiles' test case. The test performs the following steps:

        - Initializes the dsAudio module.
        - Plays test streams and iterates through supported audio ports and profiles.
        - Verifies if the correct MS12 audio profiles are applied to the ports.
        - Cleans up the assets and terminates the dsAudio module.

        Returns:
            bool: Returns True if the test completes successfully.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:
            # Start the stream playback
            self.testPlayer.play(stream)

            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                profileList = self.testdsAudio.getSupportedMS12Profiles(port, index)
                if(profileList):
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    for profile in profileList:
                        # Set the Audio Profile
                        self.testdsAudio.setMS12AudioProfile(port, index, profile)
                        # Verify the audio
                        self.testVerifyMS12AudioProfile(port, profile, True)

                    # Enable the audio port
                    self.testdsAudio.disablePort(port, index)

            # Stop the stream playback
            self.testPlayer.stop()

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return True

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test26_MS12AudioProfiles(summeryLog)
    test.run(False)
