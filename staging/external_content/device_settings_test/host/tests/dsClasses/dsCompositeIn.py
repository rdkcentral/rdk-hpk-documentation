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

import subprocess
import yaml
import os
import sys
from enum import Enum, auto
import re

# Add parent outside of the class directory
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path+"/../")

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class dsCompositeInClass():

    """
    Device Settings CompositeIn Class

    This module provides common extensions for device Settings CompositeIn Module.
    """
    def __init__(self, deviceProfilePath:str, session=None, targetWorkspace="/opt/HAL" ):
        """
        Initializes the dsCompositeIn class function.
        Args:
            deviceProfilePath (str): Path to the device profile configuration file.
            session: Optional; session object for the user interface.
        Returns:
            None
        """
        self.moduleName = "dsCompositeIn"
        self.testConfigFile =  dir_path + "/dsCompositeIn_testConfig.yml"
        self.testSuite = "L3 dsCompositeIn"

        self.deviceProfile = ConfigRead( deviceProfilePath, self.moduleName)
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.utMenu        = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession   = session
        self.utils         = utBaseUtils()

        for artifact in self.testConfig.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utils.rsync(self.testSession, filesPath, targetWorkspace)

        self.utMenu.start()

    def searchPattern(self, haystack, pattern):
        match = re.search(pattern, haystack)
        if match:
            output1 = match.group(1)
            output2 = match.group(2)
            return output1, output2
        return None

    def initialise(self):
        """
        Initializes the device settings CompositeIn module.

        Args:
            None

        Returns:
            None
        """
        result = self.utMenu.select( self.testSuite, "Initialize CompositeIn")

    def terminate(self):
        """
        Terminates the device settings CompositeIn module.

        Args:
            None.

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "Terminate CompositeIn")

    def getSupportedPorts(self):
        """
        Returns a list of supported compositeIn ports on the device.

        Args:
            None

        Returns:
            list: A list of ports.
        """

        portList = []
        portnum = self.deviceProfile.get("composite_input_configurations").get("number_of_ports")
        for i in range(0, portnum):
            portList.append(i)

        return portList

    def getConnectionCallbackStatus(self):
        """
        Retrieves the CompositeIn connection status from the device using a callback.

        This function reads the output from the device session to detect the
        CompositeIn port connection status. The callback message contains the port number
        and the connection status ("true" or "false"). The function parses the message
        and returns the port and connection status as a boolean value.

        Args:
            None
        Returns:
            tuple:
                - port (str): The CompositeIn port number as a string.
                - connection (bool): True if the CompositeIn port is connected, False otherwise.
            None: If no matching connection status is found.
        """

        result = self.testSession.read_until("Received Connection status callback port:")
        callpattern = r"Received Connection status callback port: \[(\w+)\], Connection: \[(\w+)\]"
        status = self.searchPattern(result, callpattern)

        if status:
            port = status[0]
            connection = status[1]
            return port, connection == "true"

        return None

    def getSignalChangeCallbackStatus(self):
        """
        Retrieves the CompositeIn signal status from the device using a callback.

        This function reads the output from the device session to detect the
        signal  status. The callback message contains the port number
        and the signal status. The function parses the message
        and returns the port ,signal status and None if not present.

        Args:
            None.
        Returns:
            tuple:
                - port (str): The CompositeIn port number as a string.
                - signalstatus (str): Signal status as a string.
            None: If no matching signal status is found.
        """

        result = self.testSession.read_until("Received SignalChange status callback port:")
        callpattern = r"Received SignalChange status callback port: \[(\w+)\], sigstatus: \[(\w+)\]"
        status = self.searchPattern(result, callpattern)

        if status:
            return status

        return None

    def getPortCallbackStatus(self):
        """
        Retrieves the CompositeIn In port status using a callback.

        This function reads the output from the device session to detect the
        port  status. The callback message contains the port number
        and the status of port (ispresented , activeport). The function parses 
        the message and returns the port status.

        Args:
            None.
        Returns:
            tuple:
                - ispresented (str): true or false.
                - activeport(str):Active Port number as string
            None: If no matching signal status is found.
        """

        result = self.testSession.read_until("Received statuschange callback isPresented:")
        callpattern = r"Received statuschange callback isPresented: \[(\w+)\], activeport: \[(\w+)\]"
        status = self.searchPattern(result, callpattern)

        if status:
            return status

        return None

    def getVideoModeChangeCallbackStatus(self):
        """
        Retrieves the CompositeIn videomode details from the device using a callback.

        This function reads the output from the device session to detect the
        video mode details. The callback message contains the port number
        and the video resolution details. The function parses the message
        and returns the port ,resolution, framerate and None if not present.

        Args:
            None.
        Returns:
            tuple:
                - port (str): The CompositeIn port number as a string.
                - videoResolution (str): Video resolution as a string.
                - videoFramerate (str): Video framerate as a string.
            None: If no matching signal status is found.
        """

        result = self.testSession.read_until("Received VideomodeChange callback port:")
        callpattern = r"Received VideomodeChange callback port: \[(\w+)\], videoResolution: \[(\w+)\], videoFrameRate: \[(\w+)\]"
        match = re.search(callpattern, result)
        if match:
            output1 = match.group(1)
            output2 = match.group(2)
            output3 = match.group(3)
            return output1, output2, output3

        return None

    def getStatus(self):
        """
        Gets the status of compositeIn ports.

        Args:
            None.

        Returns:
            dsCompositeInStatus_t : port status
        """
        result = self.utMenu.select( self.testSuite, "Get status of ports")

    def selectPort(self, compositein_port:str=0):
        """
        Enables the compositeIn port.

        Args:
            compositein_port (int): compositein port.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Enter the port to select:",
                    "input": compositein_port
                }
        ]

        result = self.utMenu.select(self.testSuite, "Select port", promptWithAnswers)

    def scaleVideo(self, xcord:int=0, ycord:int=0, width:int=0, height:int=0):
        """
        Scale the video played in compositIn source device

        Args:
            xcord (int, optional): xcoordiante. Defaults to 0
            ycord (int, optional): ycoordiante. Defaults to 0
            width (int, optional): width. Defaults to 0
            height(int, optional): height. Defaults to 0

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Enter the x coordinate to scale:",
                    "input": str(xcord)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the y coordinate to scale:",
                    "input": str(ycord)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the width coordinate to scale:",
                    "input": str(width)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the height coordinate to scale:",
                    "input": str(height)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Scale the compositeIn video", promptWithAnswers)

    def __del__(self):
        """
        De-Initializes the dsCompositeIn helper function.

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

    platformProfile = dir_path + "/../../../profiles/sink/Sink_CompositeInput.yaml"
    # test the class assuming that it's optional
    test = dsCompositeInClass(platformProfile, shell)

    test.initialise()
    ports = test.getSupportedPorts()
    test.getStatus()
    test.terminate()

    shell.close()
