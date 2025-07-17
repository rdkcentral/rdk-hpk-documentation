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
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class dsHdmiInPortType(Enum):
      dsHDMI_IN_PORT_0      = 0
      dsHDMI_IN_PORT_1      = auto()
      dsHDMI_IN_PORT_2      = auto()

class hdmiInSignalStatustype(Enum):
      dsHDMI_IN_SIGNAL_STATUS_NOSIGNAL = 0
      dsHDMI_IN_SIGNAL_STATUS_UNSTABLE = 1
      dsHDMI_IN_SIGNAL_STATUS_NOTSUPPORTED = 2
      dsHDMI_IN_SIGNAL_STATUS_STABLE = 3

class hdmiVideoResolution(Enum):
      dsVIDEO_PIXELRES_720x480   = 0     #720x480 Resolution
      dsVIDEO_PIXELRES_720x576   = 1     #720x576 Resolution
      dsVIDEO_PIXELRES_1280x720  = 2     #1280x720 Resolution
      dsVIDEO_PIXELRES_1366x768  = 3     #1366x768 Resolution
      dsVIDEO_PIXELRES_1920x1080 = 4     #1920x1080 Resolution
      dsVIDEO_PIXELRES_3840x2160 = 5     #3840x2160 Resolution
      dsVIDEO_PIXELRES_4096x2160 = 6     #3840x2160 Resolution
      dsVIDEO_PIXELRES_MAX = 7           #dsVIDEO_PIXELRES_MAX

class hdmiVideoAspectRatio(Enum):
      dsVIDEO_ASPECT_RATIO_4x3 = 0    #< 4:3 aspect ratio
      dsVIDEO_ASPECT_RATIO_16x9 = 1   #< 16:9 aspect ratio
      dsVIDEO_ASPECT_RATIO_MAX = 2    #< Out of range

class hdmiVideoStereoScopicMode(Enum):
      dsVIDEO_SSMODE_UNKNOWN = 0            #Unknown mode
      dsVIDEO_SSMODE_2D = 1                #2D mode
      dsVIDEO_SSMODE_3D_SIDE_BY_SIDE = 2    #3D side by side (L/R) stereo mode
      dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM = 3  #3D top & bottom stereo mode
      dsVIDEO_SSMODE_MAX = 4                #Out of range

class hdmiVideoFrameRate(Enum):
      dsVIDEO_FRAMERATE_UNKNOWN = 0  #Unknown frame rate
      dsVIDEO_FRAMERATE_24 = 1       # Played at 24 frames per second
      dsVIDEO_FRAMERATE_25 = 2       # Played at 25 frames per second
      dsVIDEO_FRAMERATE_30 = 3       # Played at 30 frames per second
      dsVIDEO_FRAMERATE_60 = 4       # Played at 60 frames per second
      dsVIDEO_FRAMERATE_23dot98 = 5  # Played at 23.98 frames per second
      dsVIDEO_FRAMERATE_29dot97 = 6  # Played at 29.97 frames per second
      dsVIDEO_FRAMERATE_50 = 7       # Played at 50 frames per second
      dsVIDEO_FRAMERATE_59dot94 = 8  # Played at 59.94 frames per second
      dsVIDEO_FRAMERATE_MAX = 9      # Out of range

class hdmiInAviContentType(Enum):
      dsAVICONTENT_TYPE_GRAPHICS = 0   #/*!< Content type Graphics. */
      dsAVICONTENT_TYPE_PHOTO = 1      #/*!< Content type Photo */
      dsAVICONTENT_TYPE_CINEMA = 2     #/*!< Content type Cinema */
      dsAVICONTENT_TYPE_GAME = 3       #/*!< Content type Game */
      dsAVICONTENT_TYPE_INVALID = 4    #/*!< Content type Invalid */
      dsAVICONTENT_TYPE_MAX = 5        #/*!< Out of range */

class hdmiInZoomMode(Enum):
      dsVIDEO_ZOOM_NONE = 0
      dsVIDEO_ZOOM_FULL = 1
      dsVIDEO_ZOOM_LB_16_9 = 2
      dsVIDEO_ZOOM_LB_14_9 = 3
      dsVIDEO_ZOOM_CCO = 4
      dsVIDEO_ZOOM_PAN_SCAN = 5
      dsVIDEO_ZOOM_LB_2_21_1_ON_4_3 = 6
      dsVIDEO_ZOOM_LB_2_21_1_ON_16_9 = 7
      dsVIDEO_ZOOM_PLATFORM = 8
      dsVIDEO_ZOOM_16_9_ZOOM = 9
      dsVIDEO_ZOOM_PILLARBOX_4_3 = 10
      dsVIDEO_ZOOM_WIDE_4_3 = 11
      dsVIDEO_ZOOM_MAX = 12

class hdmiEdidVersion(Enum):
      HDMI_EDID_VER_14 = 0
      HDMI_EDID_VER_20 = 1
      HDMI_EDID_VER_MAX = 2


class dsHdmiInClass():
    """
    Device Settings HdmiIn Class

    This module provides common extensions for device Settings HdmiIn Module.
    """
    def __init__(self, moduleConfigProfileFile :str, session=None, targetWorkspace="/tmp"):
        """
        Initializes the dsHdmiIn class function with configuration settings.

        Args:
            moduleConfigProfileFile  (str): Path to the device profile configuration file.
            session: Optional; session object for the user interface.
        """
        self.moduleName    = "dsHdmiIn"
        self.testConfigFile    =  os.path.join(dir_path, "dsHdmiIn_testConfig.yml")
        self.testSuite     = "L3 dsHdmiIn"

        # Load configurations for device profile and menu
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
            return match.group(1)
        return None

    def initialise(self, device_type:int=0):
        """
        Initializes the device settings HdmiIn module.

        Args:
            device_type (int, optional): 0 - sink device, 1 - source device. Defaults to sink.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select Device:",
                    "input": str(device_type)
                }
        ]
        result = self.utMenu.select( self.testSuite, "Initialize HdmiIn", promptWithAnswers)

    def terminate(self):
        """
        Terminate the HdmiIn port.

        Args:
            None.

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "Terminate HdmiIn")

    def getHDMIConnectionCallbackStatus(self):
        """
        Retrieves the HDMI connection status from the device using a callback.

        This function reads the output from the device session to detect the
        HDMI port connection status. The callback message contains the port number
        and the connection status ("True" or "alse"). The function parses the message
        and returns the port and connection status as a boolean value.

        Args:
            None.
        Returns:
            tuple:
                - port (str): The HDMI port number as a string.
                - connection (bool): True if the HDMI port is connected, False otherwise.
            None: If no matching connection status is found.
        """
        result = self.testSession.read_until("Received Connection status callback port:")
        connectioncallpattern = r"Received Connection status callback port: \[(\w+)\], Connection: \[(\w+)\]"
        match = re.search(connectioncallpattern, result)

        if match:
            port = match.group(1)
            connection = match.group(2)
            return port, connection

        return None

    def getSignalChangeCallbackStatus(self):
        """
        Retrieves the HDMI signal status from the device using a callback.

        This function reads the output from the device session to detect the
        signal  status. The callback message contains the port number
        and the signal status (dsHDMI_IN_SIGNAL_STATUS_NOSIGNAL or
        dsHDMI_IN_SIGNAL_STATUS_UNSTABLE or dsHDMI_IN_SIGNAL_STATUS_NOTSUPPORTED
        or dsHDMI_IN_SIGNAL_STATUS_STABLE). The function parses the message
        and returns the port ,signal status and retrieved data present in list
        as "true" or None if not present.

        Args:
            None.
        Returns:
            tuple:
                - port (str): The HDMI port number as a string.
                - signalstatus (str): Signal status as a string.
                - found(str): true if matches with hdmiInSignalStatustype
            None: If no matching signal status is found.
        """
        result = self.testSession.read_until("Received SignalChange status callback port:")
        connectioncallpattern = r"Received SignalChange status callback port:\s*\[(.*?)\].*?sigstatus:\s*\[(.*?)\]"
        match = re.search(connectioncallpattern, result)

        if match:
            port = match.group(1)
            signalstatus = match.group(2)
            return port, signalstatus

        return None

    def getHdmiInPortCallbackStatus(self):
        """
        Retrieves the HDMI In port status using a callback.

        This function reads the output from the device session to detect the
        port  status. The callback message contains the port number
        and the status of port (ispresented , activeport). The function parses
        the message and returns the port status.

        Args:
            None.
        Returns:
            tuple:
                - port (str): The HDMI port number as a string.
                - ispresented (str): true or false.
                - activeport(str):Active Port number as string
            None: If no matching signal status is found.
        """
        result = self.testSession.read_until("Received statuschange callback isPresented:")
        portstatuspattern = r"Received statuschange callback isPresented:\s*\[(.*?)\].*?activeport:\s*\[(.*?)\]"
        match = re.search(portstatuspattern, result)

        if match:
            ispresented = match.group(1)
            activeport = match.group(2)
            return ispresented, activeport

        return None

    def getVideoModeCallbackStatus(self):
        """
        Retrieves the HDMI In Video Mode update using a callback.

        This function reads the output from the device session to detect the
        video mode update. The callback message contains the port number
        and the video mode update (pixelresolution , aspectratio). The function parses
        the message and returns the video mode update.

        Args:
            None.
        Returns:
            tuple:
                - port (str): The HDMI port number as a string.
                - ispresented (str): true or false.
                - activeport(str):Active Port number as string
            None: If no matching signal status is found.
        """
        result = self.testSession.read_until("Result dsHdmiInGetCurrentVideoMode OUT:")
        # The keyword you're searching for
        search_string = "dsHdmiInGetCurrentVideoMode"

        # Find all occurrences
        match = list(re.finditer(search_string, result))


        if match:
            last_match = match[-1]
            start_index = last_match.start()
            end_index = last_match.end()
            # Extracting context around the last match
            context = result[max(0, start_index - 300):end_index + 300]
            print(context)
            videomodepattern = r'Result dsHdmiInGetCurrentVideoMode OUT:port:\s*\[([^]]+)\].*?pixelResolution:\s*\[([^]]+)\].*?aspectRatio\[\s*([^]]+)\]'
            matches = re.search(videomodepattern, context,re.DOTALL)
            if matches:
                port = matches.group(1)
                pixelresolution = matches.group(2)
                aspectratio = matches.group(3)
                return port, pixelresolution, aspectratio

        return None

    def getAllmCallbackStatus(self):
        """
        Retrieves the HDMI ALLM status from the device using a callback.

        This function reads the output from the device session to detect the
        HDMI allm status. The callback message contains the port number
        and the allm status ("true" or "false"). The function parses the message
        and returns the port and allm status as a boolean value.

        Args:
            None.
        Returns:
            tuple:
                - port_type (str): The HDMI port number as a string.
                - allm_status (bool): True if the HDMI allm bit enabled, False otherwise.
            None: If no matching connection status is found.
        """
        result = self.testSession.read_until("Received AllmChange status callback port:")
        allmpattern = r"Received AllmChange status callback port:\s*\[(.*?)\].*?allm_mode:\s*\[(.*?)\]"
        match = re.search(allmpattern, result)

        if match:
            porttype = match.group(1)
            allm_status = match.group(2)
            return porttype, allm_status

        return None

    def getAVlatencyCallbackStatus(self):
        """
        Retrieves the HDMI Audio Video latency from the device using a callback.

        This function reads the output from the device session to detect the
        audio video latency. The callback message contains the audio video latency
        in milliseconds. The function parses the message and returns the audio and
        video latency as strings.

        Args:
            None.
        Returns:
            tuple:
                - audio_latency (str): The Audio latency as a string.
                - video_latency (str): The Video latency as a string.
            None: If no matching connection status is found.
        """
        result = self.testSession.read_until("Received AVlatencyChange status callback audio_latency: video_latency:")
        avlatencypattern = r"Received AVlatencyChange status callback audio_latency:\s*\[(.*?)\].*?video_latency:\s*\[(.*?)\]"
        match = re.search(avlatencypattern, result)

        if match:
            audio_latency = match.group(1)
            video_latency = match.group(2)
            return audio_latency, video_latency

        return None

    def getAVIContentCallbackStatus(self):
        """
        Retrieves the HDMI AVI content change from the device using a callback.

        This function reads the output from the device session to detect the
        AVI content change status. The callback message contains the port number
        and the AVI content type. The function parses the message
        and returns the port and AVI content type as strings.

        Args:
            None.
        Returns:
            tuple:
                - porttype (str): The HDMI port number as a string.
                - allm_content_type (str): The AVI content type as a string.
            None: If no matching connection status is found.
        """
        result = self.testSession.read_until("Received AviContentType change callback port:")
        avipattern = r"Received AviContentType change callback port:\s*\[(.*?)\].*?avi_content_type:\s*\[(.*?)\]"
        match = re.search(avipattern, result)

        if match:
            porttype = match.group(1)
            avi_content_type = match.group(2)
            return porttype, avi_content_type

        return None

    def getHDMIInPortStatus(self):
        """
        Retrieves the HDMI In port status.

        This function reads the output from the device session to
        detect the port status. The callback message contains the
        port status like (ispresented , active). The function parses
        the message and returns ispresented and active as strings.

        Args:
            None.
        Returns:
            tuple:
                - isPresented (str): true if presented or flase otherwise.
                - activeport (str): The HDMIIN port as a string.
            None: If no matching connection status is found.
        """
        result = self.utMenu.select( self.testSuite, "Get Status")
        typeStatusPattern = r"Result dsHdmiInGetStatus OUT:inputstatus:isPresented:\s*\[(.*?)\].*?activeport:\s*\[(.*?)\]"
        match = re.search(typeStatusPattern, result)
        if match:
            isPresented = match.group(1)
            activeport = match.group(2)
            return isPresented, activeport

        return None

    def selectHDMIInPort(self, hdmiin_port:str, audMix:int=0, videoPlane:int=0, topmost:int=1):
        """
        Selects specified HDMI In port.

        Args:
            hdmiin_port (str):HDMIIN port.
            audmix (int, optional): audmix .
            videoplane(int, optional): videoplane.
            topmost(int, optional): topmost.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "List of supported ports:",
                    "input": str(hdmiin_port)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the audiomix to select:",
                    "input": str(audMix)
                },
                {
                    "query_type": "direct",
                    "query": "Supported  the videoplane type:",
                    "input": str(videoPlane)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the topmost to select",
                    "input": str(topmost)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Select Port", promptWithAnswers)

    def scaleHdmiInVideo(self, xCord:int=0, yCord:int=0, width:int=0, height:int=0):
        """
        Scale video of specified HdmiIn port.

        Args:
            xCord (int, optional): x coordinate. Defaults to 0
            yCord (int, optional): y coordinate. Defaults to 0
            width (int, optional): width. Defaults to 0
            height(int, optional): height. Defaults to 0

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Enter the x coordinate to select",
                    "input": str(xCord)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the y coordinate to select",
                    "input": str(yCord)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the width to select",
                    "input": str(width)
                },
                {
                    "query_type": "direct",
                    "query": "Enter the height to select",
                    "input": str(height)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Scale Video", promptWithAnswers)

    def getSupportedPorts(self):
        """
        Returns a list of supported Hdmi In ports on the device.

        Args:
            None

        Returns:
            list: A list of tuples containing the port name.
        """

        result = self.utMenu.select(self.testSuite, "Get Input Port")
        portPattern = r'Result dsHdmiInGetNumberOfInputs\(OUT:numInputs:\[(\d+)\]'
        ports =  self.searchPattern(result, portPattern)
        if ports:
            numOfPorts = int(ports)
        else:
            return 0

        portList = []
        for i in range(0, numOfPorts):
            portList.append(f'dsHDMI_IN_PORT_{i}')

        return portList

    def getVideoZoomModeList(self):
        """
        gets supported Zoom Mode as list.

        Args:
            None.

        Returns:
            A list of Zoom modes please refer dsvideozoommode enum class.
        """

        videoZoomModeList = []
        for modeindex in hdmiInZoomMode:
            videoZoomModeList.append(hdmiInZoomMode(modeindex).name)

        return videoZoomModeList


    def setHdmiInZoomMode(self, zoom_mode:str=0):
        """
        sets zoommode  on a HdmiIn port.

        Args:
            zoommode (str, optional): zoommode. Defaults to 0

        Returns:
            None.
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Selected Zoom Mode",
                "input": zoom_mode
            }
        ]

        result = self.utMenu.select(self.testSuite, "Zoom Mode", promptWithAnswers)

    def getEDIDVersionList(self):
        """
        gets supported EDID versions as list.

        Args:
            None.

        Returns:
            A list of EDID list please refer  enum class.
        """

        edidList = []
        for edidindex in hdmiEdidVersion:
            edidList.append(hdmiEdidVersion(edidindex).name)

        return edidList

    def setEdidVersion(self, port_type:str=0, edidversion:str=0):
        """
        sets edid version  on a particular HdmiIn port.

        Args:
            hdmiin_port (str): Defaults to 0
            edidversion (str, optional): edidversion. Defaults to 0

        Returns:
            None.
        """
        promptWithAnswers = [
             {
                "query_type": "list",
                "query": "List of supported ports:",
                "input": str(port_type)
             },
             {
                "query_type": "list",
                "query": "Please select from availabe edid versions",
                "input": edidversion
             }
        ]

        result = self.utMenu.select(self.testSuite, "Set EdidVersion", promptWithAnswers)

    def getEdidVersion(self, port_type:str=0):
        """
        Gets edidversion.

        Args:
            None.
        Returns:
            Edid version as string.
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "List of supported ports:",
                "input": str(port_type)
            }
        ]

        result = self.utMenu.select( self.testSuite, "Get EdidVersion", promptWithAnswers)

        typeStatusPattern = r"Result dsGetEdidVersion IN:port:\[(\w+)\]:\[.*\] OUT:edidver:\[(HDMI_EDID_VER_\w+)\]:\[.*\],dsError_t:\[(dsERR_\w+)\]"
        match = re.search(typeStatusPattern, result)
        if match:
            edidversion = match.group(2)
            return edidversion

        return None

    def getEdidInfo(self, port_type:str=0):
        """
        Gets EDID info of a port.

        Args:
            None.
        Returns:
            True if matches , otherwise None.
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "List of supported ports:",
                "input": str(port_type)
            }
        ]

        result = self.utMenu.select( self.testSuite, "Get Edid", promptWithAnswers)
        pattern = r'edidbyte:\[(\w+)\]:\[(\w{2})\]'
        matches = re.findall(pattern, result)
        if matches:
            edid_values = [item[1] for item in matches]
            return edid_values

        return None

    def getSpdInfo(self, port_type:str=0):
        """
        Gets Spd info of a port.

        Args:
            None.
        Returns:
            Spd Info Info as string.
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "List of supported ports:",
                "input": str(port_type)
            }
        ]

        result = self.utMenu.select( self.testSuite, "Get Spdinfo", promptWithAnswers)
        pattern = r'spdinfo:\[(\w+)\]:\[(\w{2})\]'
        matches = re.findall(pattern, result)
        if matches:
            spd_info = [item[1] for item in matches]
            return spd_info

        return False

    def setEdid2Allm(self, port_type:str=0, allm_support:int=0):
        """
        sets edid2allm support  on a particular HdmiIn port.

        Args:
            allm_support (int, optional): allm_support. Defaults to 0

        Returns:
            None.
        """
        promptWithAnswers = [
            {
                "query_type": "direct",
                "query": "List of supported ports:",
                "input": str(port_type)
            },
            {
                "query_type": "direct",
                "query": "Enter the allmsupport to select",
                "input": str(allm_support)
            }

        ]

        result = self.utMenu.select(self.testSuite, "Set Edid 2 Allm Support", promptWithAnswers)

    def getEdid2Allm(self, port_type:str=0):
        """
        Gets edid 2 allm support.

        Args:
            None.
        Returns:
            true if sets to true  otherwise false.
        """
        promptWithAnswers = [
            {
                "query_type": "direct",
                "query": "List of supported ports:",
                "input": str(port_type)
            }
        ]

        result = self.utMenu.select( self.testSuite, "Get Edid 2 Allm Support", promptWithAnswers)

        typeStatusPattern = r"Result dsGetEdid2AllmSupport IN:port:\[(\w+)\]:\[.*\] OUT:allmsupport:\[(\w+)\]"
        match = re.search(typeStatusPattern, result)
        if match:
            edid2allm = match.group(2)
            return edid2allm

        return None

    def __del__(self):
        """
        De-Initializes the dsHdmiIn helper function.

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

    platformProfile = dir_path + "/../../../profiles/sink/Sink_HDMIIN.yaml"
    # test the class assuming that it's optional
    test = dsHdmiInClass(platformProfile, shell)

    test.initialise()
    ports = test.getSupportedPorts()

    test.selectport()
    test.getportstatus()

    test.terminate()

    shell.close()
