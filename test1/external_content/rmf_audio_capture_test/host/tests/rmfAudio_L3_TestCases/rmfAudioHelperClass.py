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
import librosa
import numpy as np
import paramiko
import io

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.utPlayer import utPlayer
from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.core.logModule import logModule
from rmfAudioClasses.rmfAudio import rmfAudioClass

class rmfAudioHelperClass(utHelperClass):
    """
    A helper class to support RMF audio capture testing functionalities, including device setup,
    configuration loading, asset management, and user interactions for manual test validation.

    This class is initialized with test-specific parameters and includes methods to:
    - Download test artifacts
    - Run prerequisite and post-requisite commands
    - Interact with the RMF audio capture device for manual checks
    - Manage test assets on the Device Under Test (DUT)
    """

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the rmfAudioHelperClass with test-specific details, including test name,
        configuration paths, and logging setup.

        Args:
            testName (str): Name of the test.
            qcId (str): Quality control ID for the test.
            log (logModule, optional): Logging module instance. Defaults to None.
        """
        self.testName  = ""
        self.testSetupPath = os.path.join(dir_path, "rmfAudio_L3_testSetup.yml")
        self.moduleName = "rmfaudiocapture"
        self.rackDevice = "dut"
        self.testsuite  = "L3 rmfAudioCapture"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        # Open Session for hal test
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")
        self.player_session = self.dut.getConsoleSession("ssh_player")

        cmds = self.testSetup.get("assets").get("device").get(self.testName).get("postcmd")
        if cmds is not None:
            for cmd in cmds:
                self.hal_session.write(cmd)

        # Set up paths and URLs for device test setup
        deviceTestSetup = self.cpe.get("test")
        socVendor = self.cpe.get("soc_vendor")

        # Create player Class
        self.testPlayer = utPlayer(self.player_session, socVendor)

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))
        self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = os.path.join(self.targetWorkspace, self.moduleName)
        self.streamDownloadURL = deviceTestSetup.get("streams_download_url")

    def testDownloadAssets(self):
        """
        Downloads the test artifacts and streams listed in the test setup configuration.

        This function retrieves audio streams and other necessary files and
        saves them on the DUT (Device Under Test).

        Args:
            None
        """

        # List of streams with path
        self.testStreams = []
        url = []

        streamPaths = self.testSetup.get("assets").get("device").get(self.testName).get("streams")

        # Download test streams to device
        if streamPaths and self.streamDownloadURL:
            for streamPath in streamPaths:
                url.append(os.path.join(self.streamDownloadURL, streamPath))
                self.testStreams.append(os.path.join(self.targetWorkspace, os.path.basename(streamPath)))
            self.downloadToDevice(url, self.targetWorkspace, self.rackDevice)

    def testCleanAssets(self):
        """
        Removes the downloaded assets and test streams from the DUT after test execution.

        Args:
            None
        """
        self.deleteFromDevice(self.testStreams)

    def testRunPrerequisites(self):
        """
        Executes prerequisite commands listed in the test setup configuration file on the DUT.

        Args:
            None
        """
        # Run commands as part of test prerequisites
        test = self.testSetup.get("assets").get("device").get(self.testName)
        cmds = test.get("execute")
        if cmds is not None:
            for cmd in cmds:
                self.writeCommands(cmd)

    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Creates dsHdmiInClass

        Returns:
            bool
        """

        # Download the assets listed in test setup configuration file
        self.testDownloadAssets()

        # Run Prerequisites listed in the test setup configuration file
        self.testRunPrerequisites()

        # Create the rmfaudiocapture class
        self.testrmfAudio = rmfAudioClass(self.moduleConfigProfileFile, self.hal_session, self.testsuite, self.targetWorkspace)

        return True

    def extract_pitch(self, y):
        """
        Extracts the pitch from an audio signal using the YIN algorithm.
    
        This function applies the YIN algorithm to estimate the pitch 
        of the given audio signal over time. The pitch values are returned 
        as a NumPy array.

        Args:
            y (NumPy array) : The audio time series (waveform) as a 1D NumPy array.

        Returns:
            A NumPy array containing the estimated pitch values 
                (in Hz) for each frame. Frames with no detected pitch 
                will be represented as NaN.
        """
        # Apply the YIN algorithm to estimate the pitch
        pitches = librosa.yin(y, fmin=librosa.note_to_hz('C2'), fmax=librosa.note_to_hz('C7'))
        
        # Use NaN for frames with no detected pitch
        return np.array(pitches)

    def compare_pitches(self, pitch1, pitch2):
        """
        Compares two pitch arrays and calculates the correlation.
    
        This function aligns two pitch arrays to the same length and 
        computes the Pearson correlation coefficient between them.

        Args:
            pitch1, pitch2 (NumPy array) : contains the pitch values from two audio files

        Returns:
            correlation: The Pearson correlation coefficient between the two pitch 
                arrays, ranging from -1 to 1, where 1 indicates a perfect 
                positive correlation, -1 indicates a perfect negative 
                correlation, and 0 indicates no correlation.
        """
        # Align lengths
        min_length = min(len(pitch1), len(pitch2))
        pitch1 = pitch1[:min_length]
        pitch2 = pitch2[:min_length]

        # Calculate correlation or mean squared error
        correlation = np.corrcoef(pitch1, pitch2)[0, 1]
        return correlation

    def compareWavFiles(self, url, file_path):
        """
        Compares the pitch of two audio files and determines if they match.

        This function loads two audio files from remote location, extracts their pitch using the YIN algorithm,
        and calculates the correlation between the extracted pitches. If the correlation 
        exceeds a specified threshold, it concludes that the audio files match.

        Args:
            file1, file2: The paths to audio files for comparison.

        Returns:
            None
        """
        threshold=0.95 # Set threshold for comparison

        host = self.hal_session.address
        port = self.hal_session.port
        username = self.hal_session.username
        password = self.hal_session.password

        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(hostname=host, port=port, username=username, password=password)

        sftp = ssh.open_sftp()

        with sftp.open(url, 'r') as remote_file:
            ref_content = remote_file.read()

        with sftp.open(file_path, 'r') as remote_file:
            wav_content = remote_file.read()

        sftp.close()
        ssh.close()

        # Create a BytesIO object from the bytes
        wav_file_content = io.BytesIO(wav_content)
        ref_file_content = io.BytesIO(ref_content)

        # Load the audio files
        y1, sr1 = librosa.load(ref_file_content)
        y2, sr2 = librosa.load(wav_file_content)

        # Extract pitch from both audio files
        pitch1 = self.extract_pitch(y1)
        pitch2 = self.extract_pitch(y2)

        # Compare pitches
        correlation = self.compare_pitches(pitch1, pitch2)
        
        # Determine if they match based on the threshold
        if correlation >= threshold:
            print(f"The audio files match with a correlation of {correlation:.2f}")
            return True
        else:
            print(f"The audio files do not match (correlation: {correlation:.2f})")
            return False

    def testEndFunction(self, powerOff=True):
        # Clean the assets downloaded to the device
        self.testCleanAssets()

        # Clean up the rmfAudio instance
        del self.testrmfAudio

    def testExceptionCleanUp (self):
        # Clean the assets downloaded to the device
        self.testCleanAssets()
