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

class dsAudio_test07_MS12Volumeleveller(dsAudioHelperClass):
    """
    Test class for MS12 Volume Leveller functionality.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test case for MS12 Volume leveller.

        Sets up necessary sessions, config files, and utility classes required for the test.

        Args:
            None.
        """
        # Class variables
        self.qcID = '7'
        self.testName  = "test07_MS12Volumeleveller"
        self.ms12DAPFeature = "Volumeleveller"

        # List of volume modes for testing
        self.volumeModes = [0, 1, 2]
        self.volumeLevels = [0, 5, 10]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyVolumeleveller(self, stream, port, mode, level, manual=False):
        """
        Verifies whether the Volume Leveller feature is functioning correctly.

        Args:
            stream (str): Stream used for testing
            port (str): Audio port to verify
            mode (int): Volume leveller mode
            level (int): Volume leveller level
            manual (bool, optional): Manual verification option (True: manual, False: automated).
                                     Defaults to False.

        Returns:
            bool: Status indicating whether the audio verification was successful.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} mode {mode} level {level} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
       Main function to test the MS12 Volume Leveller.

        This function:
        - Plays audio streams
        - Apply  Volume Leveller modes on supported audio ports
        - verify the results.
        - Cleans up the assets after the test completes.

        Returns:
            bool: Result of the final verification.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        for stream in self.testStreams:

            # Loop through the supported audio ports
            for port,index in self.testdsAudio.getSupportedPorts():
                if self.testdsAudio.getMS12DAPFeatureSupport(port, index, self.ms12DAPFeature):
                    # Enable the audio port
                    self.testdsAudio.enablePort(port, index)

                    mode = 2 #leveller is Auto
                    level = 0
                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} level:{level} Port:{port} Index:{index} Stream:{stream}')

                    # Set the volume leveller
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":[mode, level]})

                    # Start the stream playback
                    self.testPlayer.play(stream)

                    result = self.testVerifyVolumeleveller(stream, port, mode, level, True)

                    # Stop the stream playback
                    self.testPlayer.stop()

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} level:{level} Port:{port} Index:{index} Stream:{stream}')

                    mode = 1 #leveller is On
                    level = 0
                    for level in self.volumeLevels:
                        self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} level:{level} Port:{port} Index:{index} Stream:{stream}')

                        # Set the volume leveller
                        self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":[mode, level]})

                        # Start the stream playback
                        self.testPlayer.play(stream)

                        result = self.testVerifyVolumeleveller(stream, port, mode, level, True)

                        # Stop the stream playback
                        self.testPlayer.stop()

                        self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} level:{level} Port:{port} Index:{index} Stream:{stream}')

                    mode = 0 #leveller is OFF
                    level = 0
                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} mode:{mode} level:{level} Port:{port} Index:{index} Stream:{stream}')

                    # Set the volume leveller
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":[mode, level]})

                    # Start the stream playback
                    self.testPlayer.play(stream)

                    result = self.testVerifyVolumeleveller(stream, port, mode, level, True)

                    # Stop the stream playback
                    self.testPlayer.stop()

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} mode:{mode} level:{level} Port:{port} Index:{index} Stream:{stream}')

                    # Disable the audio port
                    self.testdsAudio.disablePort(port, index)


        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test07_MS12Volumeleveller(summeryLog)
    test.run(False)
