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

class dsAudio_test16_ARCSAD(dsAudioHelperClass):
    """
    Test class for verifying the ARC SAD (Source Audio Descriptor) functionality.
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the dsAudio_test16_ARCSAD test.

        Args:
            None.
        """
        # Class variables
        self.qcID = '16'
        self.testName  = "test16_ARCSAD"

        """
        Summary of the 3-byte SAD Format:
        Byte    Bit Fields  Description
        Byte 1  Bits 0-2:   Audio Format Code: Type of audio format (PCM, AC-3, DTS, etc.)
                Bits 3-6:   Maximum Number of Channels	Number of audio channels supported
                Bit 7:      Reserved
        Byte 2	Bits 0-6:   Sampling Frequencies: Supported sampling frequencies (32 kHz, 48 kHz, etc.)
                Bit 7:      Reserved
        Byte 3	For LPCM: Bit Depths (16, 20, 24-bit) Supported bit depths for PCM
                For Compressed Formats: Maximum Bitrate	Maximum supported bitrate for compressed formats
        """
        # 0x0040382A - AC3 6 channels sampling rates (48, 96, 192 kHz), Max bitrate (512 kbps)
        # 0x00070709 - PCM 2 channels sampling rates (32, 44.1, 48 kHz), bit depth (16, 20, 24 bit per sample)
        self.sadList = [[0x00070709],[0x0040382A]]

        super().__init__(self.testName, self.qcID, log)

    #TODO: Current version supports only manual.
    def testARCSAD(self, sad, manual=False):
        """
        Verifies the ARC Source Audio Descriptor (SAD).

        Args:
            sad (list): List of ARC SAD values to verify.
            manual (bool, optional): Indicates whether to use manual control.
                                     Defaults to False (automatic methods).

        Returns:
            bool: True if the SAD verification was successful, False otherwise.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Has SAD:{sad} sent to the ARC device [Y/N]:")
        else :
            #TODO: Add automation verification methods
            return False

    def testFunction(self):
        """
        Executes the test for verifying ARC SAD functionality.

        This method performs the following steps:
        - Initializes the dsAudio module and player.
        - Tests each supported audio port for ARC functionality.
        - Stops the stream playback and cleans up.

        Returns:
            bool: True if the test ran successfully, False otherwise.
        """
        # Initialize the dsAudio module
        self.testdsAudio.initialise(self.testdsAudio.getDeviceType())

        # Start the stream playback
        self.testPlayer.play(self.testStreams[0])

        # Loop through the supported audio ports and initialize ARC port
        for port,index in self.testdsAudio.getSupportedPorts():
            if "ARC" in port:
                # Enable the audio port
                self.testdsAudio.enablePort(port, index)

                for sad in self.sadList:
                    self.log.stepStart(f'ARC SAD {sad} Test')

                    self.testdsAudio.setARCSAD(sad)

                    # Verify ARC SAD
                    result = self.testARCSAD(sad, True)

                    self.log.stepResult(result, f'ARC SAD {sad} Test')

                # Disable the audio port
                self.testdsAudio.disablePort(port, index)

        # Stop the stream playback
        self.testPlayer.stop()

        # Terminate dsAudio Module
        self.testdsAudio.terminate()

        return True

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsAudio_test16_ARCSAD(summeryLog)
    test.run(False)
