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
from enum import Enum, auto

# Add parent outside of the class directory
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path+"/../")

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class WakeupSources(Enum):
    PWRMGR_WAKEUPSRC_VOICE = 0
    PWRMGR_WAKEUPSRC_PRESENCE_DETECTION = auto()
    PWRMGR_WAKEUPSRC_BLUETOOTH = auto()
    PWRMGR_WAKEUPSRC_WIFI = auto()
    PWRMGR_WAKEUPSRC_IR = auto()
    PWRMGR_WAKEUPSRC_POWER_KEY = auto()
    PWRMGR_WAKEUPSRC_TIMER = auto()
    PWRMGR_WAKEUPSRC_CEC = auto()
    PWRMGR_WAKEUPSRC_LAN = auto()

class powerManagerClass():

    """
    Power Class

    This module provides common extensions for Power Module.

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

        self.moduleName = "power"
        self.testConfigFile =  os.path.join(dir_path, "power_testConfig.yml")
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.testSuite = "L3 power manager "

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
        Initializes the Power manager module.

        Args:
            None

        Returns:
            None
        """
        result = self.utMenu.select( self.testSuite, "Initialize Power Manger")

    def setPowerMode(self, mode:int):
        """
        Set Power Mode.

        Args:
            mode (int): This enumeration defines the power state.
                        Refer to the PWRMGR_PowerState_t enum for valid options

        Returns:
            None
        """
        promptWithAnswers = [
            {
                "query_type": "direct",
                "query": "Power State",
                "input": str(mode)
            }
        ]

        # Convert input arguments to strings and update the prompts
        result = self.utMenu.select( self.testSuite, "Set Power State", promptWithAnswers)

    def getPowerMode(self):
        """
        Get Power Mode.

        Args:
            None
        Returns:
            powerState (str) : Returns current state of the device
        """
        result = self.utMenu.select( self.testSuite, "Get Power State")

        powerStatePattern = r'Result PLAT_API_GetPowerState\(OUT:State\[(?P<State>[^\]]+)\] pmStatus_t:\[%s\]\)'
        powerState = re.search(powerStatePattern, result)

        return powerState

    def setWakeupSource(self, source:int, enable:int=1):
        """
        Set wakeup source.

        Args:
            source: int: This enumeration defines the wakeup source.
                            Refer to the PWRMGR_WakeupSrcType_t enum for valid options
            enable: int: Enable/Disable Wakeup Source

        Returns:
            None
        """

        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select Wake Up Source Type: ",
                "input": source
            },
            {
                "query_type": "direct",
                "query": "Enable/Disable Wakeup Source",
                "input": str(enable)
            }
        ]

        # Convert input arguments to strings and update the prompts
        result = self.utMenu.select( self.testSuite, "Set Wakeup Source", promptWithAnswers)

    def getWakeupSource(self, source:int):
        """
        Get whether a wake up source is enabled or not.

        Args:
            source: int: This enumeration defines the wakeup source.
                            Refer to the PWRMGR_WakeupSrcType_t enum for valid options

        Returns:
            enable (str) : Returns whether the wakeup soruce is enabled or not
        """

        promptWithAnswers = [
            {
                "query_type": "direct",
                "query": "Select Wake Up Source Type: ",
                "input": str(source)
            }
        ]

        # Convert input arguments to strings and update the prompts
        result = self.utMenu.select( self.testSuite, "Get Wakeup Source", promptWithAnswers)

        wakeUpEnabledPattern = r'Result PLAT_API_GetWakeupSrc\(IN:srcType\[%s\], OUT:enable\[(?P<enable>[^\]]+)\] pmStatus_t:\[%s\]\)'

        enable = self.searchPattern(result, wakeUpEnabledPattern)

        return enable

    def getSupportedWakeupSources(self):
        """
        Get supported wakeup sources.

        Args:
            None

        Returns:
            supportedWakeupSources(list) : Returns list of Wakeup sources
        """
        supportedWakeupSources = []
        wakeupSources = self.deviceProfile.get("WakeupSources")
        if not wakeupSources:
            return supportedWakeupSources

        for source in wakeupSources:
            supportedWakeupSources.append(WakeupSources(source).name)
        return supportedWakeupSources

    def terminate(self):
        """
        Terminates the Power manager module.

        Args:
            None.

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "Terminate Power Manager")

    def __del__(self):
        """
        De-Initializes the power manager helper function.

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

    platformProfile = dir_path + "/../../../profiles/sinkWakeUpSources.yaml" #Does this work? Do we need something else?
    # test the class assuming that it's optional
    test = powerManagerClass(platformProfile, shell)

    test.initialise()
    test.setPowerMode(1)
    test.getPowerMode()
    test.setWakeupSource(1, 1)
    test.getWakeupSource(1)
    test.terminate()

    shell.close()
