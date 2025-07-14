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

import yaml
import os
import sys
from enum import Enum, auto
import re

# Add parent directory to the system path for module imports
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class rmfAudioClass():
    """
    RMF Audio Capture Class.

    This module provides common functionalities and extensions for RMF Audio Capture Module.
    """

    def __init__(self, moduleConfigProfileFile:str, session=None, testSuite:str="L3 rmfAudioCapture", targetWorkspace="/tmp", copyArtifacts:bool=True ):
        """
        Initializes the rmfAudioClass instance with configuration settings.

        Args:
            moduleConfigProfileFile (str): Path to the device profile configuration file.
            session: Optional; session object for the user interface.

        Returns:
            None
        """
        self.moduleName = "rmfaudiocapture"
        self.testConfigFile = os.path.join(dir_path, "rmfAudio_testConfig.yml")
        self.testSuite = testSuite

        # Load configurations for device profile and menu
        self.moduleConfigProfile = ConfigRead( moduleConfigProfileFile , self.moduleName)
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.testConfig.test.execute = self.testConfig.test.execute + f" -p {os.path.basename(moduleConfigProfileFile)}"
        self.utMenu        = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession   = session
        self.utils         = utBaseUtils()

        if copyArtifacts:
            for artifact in self.testConfig.test.artifacts:
                filesPath = os.path.join(dir_path, artifact)
                self.utils.rsync(self.testSession, filesPath, targetWorkspace)

            # Copy the profile file to the target
            self.utils.scpCopy(self.testSession, moduleConfigProfileFile, targetWorkspace)

        # Start the user interface menu
        self.utMenu.start()

    def searchPattern(self, haystack, pattern):
        """
        Searches for the first occurrence of a specified pattern in the provided string.

        Args:
            haystack (str): The string to be searched.
            pattern (str): The regular expression pattern to search for.

        Returns:
            str: The first capturing group of the match if found; otherwise, None.

        Notes:
            - If no match is found, None is returned.
        """
        match = re.search(pattern, haystack)
        if match:
            return match.group()
        return None

    def checkAuxiliarySupport(self):
        """
        Check auxiliary interface support based on profile file

        Args:
            None

        Returns:
            bool : true/false based on auxsupport value in profile file yaml
        """
        return self.moduleConfigProfile.get("features").get("auxsupport")

    def runTest(self, test_case:str=None):
        """
        Runs the test case passed to this funtion
        Args:
            test_case (str, optional): test case name to run, default runs all test
        Returns:
            bool: True - test pass, False - test fails
        """
        output = self.utMenu.select( self.testSuite, test_case, None, 25)
        results = self.utMenu.collect_results(output)
        if results == None:
            results = False
        return results

    def openHandle(self, capture_type:int=1):
        """
        Opens the RMF audio capture interface and gets a handle.

        Args:
            capture_type (int, optional):
                - 1 for primary data capture (default).
                - 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]
        result = self.utMenu.select( self.testSuite, "Open RMF Audio Capture Handle", promptWithAnswers)

    def closeHandle(self, capture_type:int=1):
        """
        Closes RMF Audio Capture interface.

        Args:
            capture_type (int, optional):
                - 1 for primary data capture (default).
                - 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]
        result = self.utMenu.select(self.testSuite, "Close RMF Audio Capture Handle", promptWithAnswers)

    def updateSettings(self, capture_type:int=1, settings_update:int=0, capture_format:int=1, sampling_rate:int=5, fifo_size:int=65536, threshold:int=8192):
        """
        Update RMF_AudioCapture_Settings values if required.

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            settings_update (int, optional): Choice whether default settings needs to be updated (default is 0).
            capture_format (int, optional): RMF Audio Capture Format for audio data (default is racFormat_e16BitStereo i.e 1).
            sampling_rate (int, optional): RMF Audio Capture Sampling Rate (default is racFreq_e48000 i.e 5).
            fifo_size (int, optional): FIFO size in bytes (default is 65k, 65536)
            threshold (int, optional): FIFO data callback threshold in bytes (default is 8k, 8192)

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Do you want to update default settings ? (0 for No, 1 for Yes)",
                    "input": str(settings_update)
                }
        ]

        # If the user chooses update settings, prompt for settings update
        if settings_update == 1:
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Select the capture format to update, use -1 to retain default value :",
                    "input": str(capture_format)
                })
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Select the Sampling Rate, use -1 to retain default value :",
                    "input": str(sampling_rate)
                })
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Enter FIFO size in bytes, use -1 to retain default value :",
                    "input": str(fifo_size)
                })
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Enter data callback threshold in bytes, used to check jitter (max 1/4th of FIFO), use -1 to retain default value :",
                    "input": str(threshold)
                })

        result = self.utMenu.select(self.testSuite, "Get and update default settings", promptWithAnswers)

    def selectTestType(self, capture_type:int=1, test_type:int=1, datacapture_duration:int=10):
        """
        Selects the type of test, to set up data callbacks for counting, capture tests

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            test_type (int, optional): Type of test about to be run : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.
            datacapture_duration (int, optional) : Test duration in seconds to capture audio data (default is 10 seconds)

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Select the type of test: ",
                    "input": str(test_type)
                },
        ]

        if test_type == 2:
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Enter test duration in seconds for data capture test :",
                    "input": str(datacapture_duration)
                })

        result = self.utMenu.select(self.testSuite, "Select the type of test", promptWithAnswers)

    def startCapture(self, capture_type:int=1):
        """
        Starts RMF Audio Capture

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Start RMF Audio Capture", promptWithAnswers)

    def checkBytesReceived(self):
        """
        Checks bytes received for both primary and auxiliary data captures

        Args:
            None

        Returns:
            int, int(optional) : Bytes received from primary data capture. Included auxiliary data capture if it is supported.
        """
        result = self.utMenu.select(self.testSuite, "Check Bytes Received")
        bytes_received_string = r"Bytes Received for \w+ capture (\d+)(?:[^0-9]*Bytes Received for \w+ capture (\d+))?"

        matches = re.findall(bytes_received_string, result)
        primary_capture = matches[0] if len(matches) > 0 else None
        auxiliary_capture = matches[1] if len(matches) > 1 else None
    
        return primary_capture, auxiliary_capture

    def writeWavFile(self, capture_type:int=1, file_path:str="/tmp/output.wav"):
        """ 
        Writes captured audio data to a wav file

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            file_path (string, optional): File name and path of the output wav file to create (default is "/tmp/output.wav").

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Enter file name and location to create output filename (example - /tmp/output.wav) :",
                    "input": str(file_path)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Write output wav file", promptWithAnswers)

    def startJitterTest(self, capture_type:int=1, threshold:int=16384, jitter_interval:int=100000, jitter_test_duration:int=120):
        """
        Starts jitter test with required parameters for test

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            threshold (int, optional): Threshold to check jitter against (default is 16k, 16384).
            jitter_interval (int, optional): Interval in microseconds to monitor buffer for jitter (default is 100ms, 100000).
            jitter_test_duration (int, optional): Test duration in seconds for jitter test (default is 120 seconds).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Enter minimum threshold in bytes to check jitter : ",
                    "input": str(threshold)
                },
                {
                    "query_type": "direct",
                    "query": "Enter interval in microseconds to monitor buffer for jitter : ",
                    "input": str(jitter_interval)
                },
                {
                    "query_type": "direct",
                    "query": "Enter test duration in seconds for jitter test : ",
                    "input": str(jitter_test_duration)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Start Jitter test", promptWithAnswers)

    def checkJitterTestResult(self, capture_type:int=1):
        """
        Gets the result of jitter test

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            bool : true/false based on jitter detected strings from C-test
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Check jitter test result", promptWithAnswers)
        jitter_pattern = "Jitter Detected !"

        jitter_result = self.searchPattern(result, jitter_pattern)
        if jitter_result:
            return False
        else:
            return True

    def getCurrentSettings(self, capture_type:int=1):
        """
        Gets current RMF audio capture settings

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Get current settings", promptWithAnswers)
        curr_settings_string = r'settings\.\w+:\[([^\]]+)\]'

        current_settings = re.findall(curr_settings_string, result)
        # Returns in this order of settings : cbBufferReady, cbBufferReadyParm, cbStatusChange, cbStatusParm, fifoSize, threshold, racFormat, racFreq, delayCompensation_ms
        return current_settings

    def getStatus(self, capture_type:int=1):
        """
        Gets RMF audio capture status

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Get RMF Audio Capture status", promptWithAnswers)
        curr_status_string = r'status\.\w+:\[([^\]]+)\]'

        current_status = re.findall(curr_status_string, result)
        # Returns in this order of status : started, racFormat, racFreq, fifoDepth, overflows, underflows
        return current_status

    def stopCapture(self, capture_type:int=1):
        """
        Stops audio capture

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Stop RMF Audio Capture", promptWithAnswers)

    def __del__(self):
        """
        Cleans up and de-initializes the dsAudio helper by stopping the test menu.

        Args:
            None.

        Returns:
            None
        """
        self.utMenu.stop()

# Test and example usage code
if __name__ == '__main__':

    shell = InteractiveShell()
    shell.open()

    platformProfile = dir_path + "/../../../profiles/rmfAudioCaptureAuxSupported.yaml"
    # test the class assuming that it's optional
    test = rmfAudioClass(platformProfile, shell)

    test.openHandle(1)

    test.closeHandle(1)

    shell.close()
