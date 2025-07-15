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
import re

# Add parent outside of the class directory
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class deepsleepClass():

    """
    Deepsleep Manager Class

    This module provides common extensions for Deepsleep Manager Module.

    """
    def __init__(self, moduleConfigProfileFile:str, session=None, targetWorkspace="/tmp" ):
        """
        Initializes the Deepsleep Manager class function.
        Args:
            moduleConfigProfileFile  (str): Path to the device profile configuration file.
            session (Optional): session object for the user interface. Defaults to None
            targetWorkspace (str): Target workspace folder on device. Defaults to "/tmp"
        Returns:
            None
        """

        self.moduleName = "deepsleep"
        self.testConfigFile =  os.path.join(dir_path, "deepsleep_testConfig.yml")
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.testSuite = "L3 Deepsleep manager "

        self.deviceProfile = ConfigRead( moduleConfigProfileFile, self.moduleName)
        self.utMenu        = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession   = session
        self.utils         = utBaseUtils()

        for artifact in self.testConfig.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utils.rsync(self.testSession, filesPath, targetWorkspace)

        self.utMenu.start()

    def searchPattern(self, haystack, pattern):
        """
        Search pattern in the given string
        Args:
            haystack  (str): String.
            pattern (str): Pattern to search in haystack
        Returns:
            None
        """
        match = re.search(pattern, haystack)
        if match:
            return match.group(1)
        return None

    def initialise(self):
        """
        Initializes the deepsleep manager module.

        Args:
            None

        Returns:
            None
        """
        result = self.utMenu.select( self.testSuite, "Initialize Deepsleep Manager")

    def triggerDeepsleep(self, timeout:int=0, networkStandby:int=0):
        """
        Trigger deepsleep.

        Args:
            timeout: Timeout in seconds between 0 and 604800. 0 for no timeout.
            networkStandby: Network Standby enabled/disabled [0/1]

        Returns:
            None
        """

        # Convert input arguments to strings and update the prompts
        promptWithAnswers = [
            {
                "query_type": "direct",
                "query": "Deep direct Timeout in seconds between 0 and 604800. 0 for no timeout.",
                "input": str(timeout)
            },
            {
                "query_type": "direct",
                "query": "Network Standby enabled/disabled [0/1]",
                "input": str(networkStandby)
            }
        ]
        result = self.utMenu.select( self.testSuite, "Trigger Deepsleep", promptWithAnswers)

    def deepsleepWakeup(self):
        """
        Post deepsleep wakeup.

        Args:
            None

        Returns:
            None
        """
        result = self.utMenu.select( self.testSuite, "Deepsleep Wakeup")

    def getLastWakeupReason(self):
        """
        Get last wakeup reason

        Args:
            None

        Return:
            wakeupReason (str) : Wakeup reason
        """
        result = self.utMenu.select( self.testSuite, "Get last wakeup reason")

        wakeupReasonPattern = r'Result PLAT_DS_GetLastWakeupReason\(wakeupReason:\[(?P<wakeupReason>[^\]]+)\]\)'

        wakeupReason = self.searchPattern(result, wakeupReasonPattern)
        return wakeupReason

    def getLastWakeupKeycode(self):
        """
        Get last wakeup keycode.

        Args:
            None

        Returns:
            wakeupKeycode (str) : Return wakeup code
        """
        result = self.utMenu.select( self.testSuite, "Get last wakeup keycode")

        wakeupKeycodePattern = r'Result PLAT_DS_GetLastWakeupKeyCode\(wakeupKeyCode:\[(?P<wakeupKeycode>\d+)\]\)'

        wakeupKeycode = self.searchPattern(result, wakeupKeycodePattern)
        return wakeupKeycode

    def terminate(self):
        """
        Terminates the deepsleep manager module.

        Args:
            None.

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "Terminate Deepsleep Manager")

    def __del__(self):
        """
        De-Initializes the deepsleep helper function.

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

    platformProfile = dir_path + "/../../../profiles/deepsleepmanagerExtendedEnumsNotSupported.yaml"
    # test the class assuming that it's optional
    test = deepsleepClass(platformProfile, shell)

    test.initialise()
    test.triggerDeepsleep(60, 1)
    test.deepsleepWakeup()
    test.getLastWakeupReason()
    test.getLastWakeupKeycode()
    test.terminate()
    shell.close()
