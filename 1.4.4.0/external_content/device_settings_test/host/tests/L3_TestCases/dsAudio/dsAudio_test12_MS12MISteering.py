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

class dsAudio_test12_MS12MISteering(dsAudioHelperClass):
    """
    Class to test MS12 MISteering functionality in the dsAudio module.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test12_MS12MISteering test instance.

        This sets up the test configuration and prepares sessions for
        player and device access.

        Args:
            None
        """
        # Class variables
        self.qcID = '12'
        self.testName  = "test12_MS12MISteering"
        self.ms12DAPFeature = "MISteering"

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual verification.
    def testVerifyMISteering(self, stream, port, mode, manual=False):
        """
        Verifies the functionality of the MISteering feature.

        This method checks whether the audio behaves as expected when
        the MISteering feature is applied to the specified audio port.

        Args:
            stream (str): The audio stream used for testing.
            port (str): The audio port being verified.
            mode (bool): Indicates if MISteering is enabled (True) or disabled (False).
            manual (bool, optional): Specifies whether to use manual verification.
                                     Defaults to False, using automated methods if implemented.

        Returns:
            bool: The status of the audio verification (True for success, False for failure).
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has MS12 {self.ms12DAPFeature} {mode} applied to the {port}? (Y/N):")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the main test sequence for MS12 MISteering.

        This method orchestrates:
        - Initializing the audio module
        - Play the Audio Stream
        - Apply the MI steering modes for supported ports
        - Performing the tests, and cleaning up afterward.

        Returns:
            bool: The final result of the test execution (True if successful, False otherwise).
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

                    # Enable MISteering
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":True})

                    result = self.testVerifyMISteering(stream, port, True, True)

                    self.log.stepResult(result, f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    self.log.stepStart(f'MS12 {self.ms12DAPFeature} :{True} Port:{port} Index:{index} Stream:{stream}')

                    # Disable MISteering
                    self.testdsAudio.setMS12Feature(port, index, {"name":self.ms12DAPFeature, "value":False})

                    result = self.testVerifyMISteering(stream, port, False, True)

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
    test = dsAudio_test12_MS12MISteering(summeryLog)
    test.run(False)
