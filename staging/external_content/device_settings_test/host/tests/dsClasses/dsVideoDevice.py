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

# Add parent outside of the class directory
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class dsVideoZoomMode(Enum):
    dsVIDEO_ZOOM_NONE              = 0
    dsVIDEO_ZOOM_FULL              = auto()  # Full screen
    dsVIDEO_ZOOM_LB_16_9           = auto()  # 16:9 Letterbox
    dsVIDEO_ZOOM_LB_14_9           = auto()  # 14:9 Letterbox
    dsVIDEO_ZOOM_CCO               = auto()  # Center Cut-out
    dsVIDEO_ZOOM_PAN_SCAN          = auto()  # Pan & Scan
    dsVIDEO_ZOOM_LB_2_21_1_ON_4_3  = auto()  # 2.21:1 Letterbox on 4:3
    dsVIDEO_ZOOM_LB_2_21_1_ON_16_9 = auto()  # 2.21:1 Letterbox on 16:9
    dsVIDEO_ZOOM_PLATFORM          = auto()  # Control over the decoder format conversions is managed by the platform
    dsVIDEO_ZOOM_16_9_ZOOM         = auto()  # 16:9 Zoom
    dsVIDEO_ZOOM_PILLARBOX_4_3     = auto()  # Pillarbox 4:3
    dsVIDEO_ZOOM_WIDE_4_3          = auto()  # Wide 4:3

class dsVideoCodec(Enum):
    dsVIDEO_CODEC_MPEGHPART2  = (0x01 << 0)
    dsVIDEO_CODEC_MPEG4PART10 = (0x01 << 1)
    dsVIDEO_CODEC_MPEG2       = (0x01 << 2)

class dsVideoDeviceClass():

    """
    Device Settings Videodevice Class

    This module provides common extensions for device Settings Videodevice Module.
    """
        
    def __init__(self, moduleConfigProfileFile :str, session=None, targetWorkspace="/tmp"):
        """
        Initializes the dsVideodevice class function.
        """
        self.moduleName = "dsVideoDevice"
        self.testConfigFile =  os.path.join(dir_path, "dsVideoDevice_testConfig.yml")
        self.testSuite  = "L3 dsVideoDevice"

        self.deviceProfile = ConfigRead( moduleConfigProfileFile , self.moduleName)
        
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.utMenu        = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession   = session
        self.devicePrefix = "VideoDevice"

        self.utils         = utBaseUtils()

        for artifact in self.testConfig.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utils.rsync(self.testSession, filesPath, targetWorkspace)


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
            - The pattern should contain at least one capturing group (parentheses).
            - If no match is found, None is returned.
        """
        match = re.search(pattern, haystack)
        if match:
            return match.group(1)
        return None

    def initialise(self, device_type:int = 0):
        """
        Initializes the device settings VideoDevice module.

        Args:
            device_type (int, optional): 0 - sink device, 1 - source device. Defaults to sink.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select Device Type[0: Sink, 1: Source]:",
                    "input": str(device_type)
                }
        ]
        result = self.utMenu.select( self.testSuite, "VideoDevice_Init",promptWithAnswers)

    def terminate(self):
        """
        Terminate the VideoDevice.

        Args:
            None.

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "VideoDevice_Term")

    def setZoomMode(self, device:str="VideoDevice0", mode:str="dsVIDEO_ZOOM_NONE"):
        """
        sets the zoom mode.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0
            mode (str, optional) : Zoom mode to set, defaults to dsVIDEO_ZOOM_NONE

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                },
                {
                    "query_type": "list",
                    "query": " Select the Zoom mode:",
                    "input": mode
                }
        ]

        result = self.utMenu.select(self.testSuite, "SetZoomMode", promptWithAnswers)

    def setDisplayFramerate(self, device:str="VideoDevice0", framerate:str="3840x2160px60"):
        """
        Sets display framerate.

        Args:
           device (str, optional) : Video Device, defaults to VideoDevice0
           framerate(str, optional) : Display Framerate to set, defaults to 3840x2160px60

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                },
                {
                    "query_type": "direct",
                    "query": " Provide Display Framerate :",
                    "input": framerate
                }
        ]

        result = self.utMenu.select(self.testSuite, "SetDisplayFramerate", promptWithAnswers)

    def setFRFMode(self, device:str="VideoDevice0", mode:str="Enable"):
        """
        Sets display FRF mode.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0
            mode (str, optional) : FRF mode, defaults to "Enable"

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                },
                {
                    "query_type": "list",
                    "query": "Select the Display FRF Mode :",
                    "input": mode
                }
        ]

        result = self.utMenu.select(self.testSuite, "SetFRFMode", promptWithAnswers)

    def getFrameratePrePostChangeCallbackStatus(self):
        """
        Retrieves the display Framerate Prechange status using a callback.

        Args:
            None.
        Returns:
            dict: Returns dictionary of pre and post framerate changes in seconds.
                  { Pre: <pre framerate change in seconds>,
                    Post: <post framerate change in seconds>}
        """
        timeChange = {}
        result = self.testSession.read_until("FrameratePreChange callback tSecond: ")
        framerateprechange = r"FrameratePreChange callback tSecond: (\d+)"
        frameratepostchange = r"FrameratePostChange callback tSecond: (\d+)"
        preMatch = re.search(framerateprechange, result)
        postMatch = re.search(frameratepostchange, result)

        if preMatch:
            tSecond = preMatch.group(1)
            timeChange["Pre"] = int(tSecond)

        if postMatch:
            tSecond = postMatch.group(1)
            timeChange["Post"] = int(tSecond)

        return timeChange

    def getFrameratePostchangeCallbackStatus(self):
        """
        Retrieves the display Framerate Postchange status using a callback.

        Args:
            None.
        Returns:
            tSecond: Time in seconds as a integer.
            None: If no matching signal status is found.
        """
        result = self.testSession.read_until("FrameratePostChange callback tSecond: ")
        frameratepostchange = r"FrameratePostChange callback tSecond: (\d+)"
        match = re.search(frameratepostchange, result)

        if match:
            tSecond = match.group(1)
            return tSecond

        return None

    def getVideoCodecInfo(self, device:str="VideoDevice0", codec:str="dsVIDEO_CODEC_MPEG4PART10"):
        """
        Gets the Video codec information.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0
            codec (str, optional) : Video codec, defaults to dsVIDEO_CODEC_MPEG4PART10

        Returns:
            tuple - Codec information
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                },
                {
                    "query_type": "list",
                    "query": " Select the Codec for Info :",
                    "input": codec
                }
        ]

        result = self.utMenu.select(self.testSuite, "GetVideoCodecInfo", promptWithAnswers)
        codecInfoPattern = r"Result dsGetVideoCodecInfo\(IN:Handle:\[.*\],IN:Codec\[(\w+)\], OUT:Codec number of Entires\[(\w+)\]"
        codecInfo = re.search(codecInfoPattern,result)
        if codecInfo:
            count = codecInfo.group(2)

        codecpattern = r"OUT:CodecProfile\[(dsVIDEO_CODEC_\w+)\]"
        pattern = re.search(codecpattern,result)

        codecValues = ''
        if pattern:
            codecValues = pattern.group(0)

        return  int(count), codecValues

    def getSupportedVideoCodingFormat(self, device:str="VideoDevice0"):
        """
        Gets the Supported VideoCoding Format.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0

        Returns:
            List of video codec formats
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                }
        ]

        result = self.utMenu.select(self.testSuite, "GetSupportedVideoCodingFormat", promptWithAnswers)
        supportedCodecPattern = r"Result dsGetSupportedVideoCodingFormats\(IN:Handle[.*\],OUT:supportedFormat[.*\]\)"
        supportedCodec = self.searchPattern(result, supportedCodecPattern)

        return supportedCodec

    def getFRFMode(self, device:str="VideoDevice0"):
        """
        Gets the FRF mode.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0

        Returns:
            Returns current FRF mode
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                }
        ]

        result = self.utMenu.select(self.testSuite, "dsGetFRFMode", promptWithAnswers)
        frfModepattern = r"Result dsGetFRFMode\(IN:Handle\[.*\],OUT:frfMode\[(\w+)\]\)"
        match = re.search(frfModepattern, result)

        if match:
            mode = match.group(1)
            return mode

        return None

    def getCurrentDisplayframerate(self, device:str="VideoDevice0"):
        """
        Gets the current display framerate.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0

        Returns:
            Returns current display frame rate set
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                }
        ]

        result = self.utMenu.select(self.testSuite, "GetCurrentDisplayframerate", promptWithAnswers)
        frameRatePattern = r"Result dsGetCurrentDisplayframerate\(IN:Handle\[.*\],OUT:currentFrameRate\[(3840x\w+)\]\)"
        frameRate = self.searchPattern(result, frameRatePattern)

        return frameRate

    def getZoomMode(self, device:str="VideoDevice0"):
        """
        Gets the Zoom mode.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0

        Returns:
            Returns current zoom mode set
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select the Video Device:",
                    "input": device
                }
        ]

        result = self.utMenu.select(self.testSuite, "GetZoomMode", promptWithAnswers)
        zoomModePattern = r"Result dsGetDFC\(\(IN:Handle\[.*\],OUT:CurrentZoomMode\[(dsVIDEO_ZOOM_\w+)\]\)"
        zoomMode = self.searchPattern(result, zoomModePattern)

        return zoomMode

    def getSupportedVideoDevice(self):
        """
        Returns a list of video devices.

        Args:
            None

        Returns:
            Returns list of supported video devices.
        """

        supportedDevices = []
        devices = self.deviceProfile.get("NumVideoDevices")
        if not devices:
            return supportedDevices  # Handle empty ports list

        for i in range(0, devices):
            supportedDevices.append(self.devicePrefix + str(i))
        return supportedDevices

    def getDeviceType(self):
        """
        Returns the device type.

        Args:
            None.

        Returns:
            returns the device type (0-Sink device, 1-Source device)
        """

        type = self.deviceProfile.get("Type")
        if type == "sink":
            return 0
        elif type == "source":
            return 1
        else:
            return None

    def __del__(self):
        """
        De-Initializes the dsVideo helper function.

        Args:
            None.

        Returns:
            None
        """
        self.utMenu.stop()

    def getSupportedFrameRates(self, device:str="VideoDevice0"):
        """
        Returns the supported Framerate Formats on device.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0

        Returns:
            returns the supported Framerate Formats
        """

        deviceNum = int(device.replace(self.devicePrefix, "")) + 1
        
        device_data = self.deviceProfile.fields.get("Device")
        if isinstance(device_data, dict):
            device_entry = device_data.get(deviceNum)
        if isinstance(device_entry, dict):
                return device_entry.get("SupportedDisplayFramerate")
        
    
    def getsupportedCodingFormats(self):
        """
        Returns the supported Framerate Formats on device.

        Args:
            None.

        Returns:
            returns the supported Framerate Formats
        """
        supported_CodingFormats = []

        Device = self.deviceProfile.get("Device")
        for i in range(1, len(Device)+1):
            entry = Device[i]
            SupportedVideoCodingFormats = entry['SupportedVideoCodingFormats']

            codec_list = list(dsVideoCodec)

            for i, codec in enumerate(codec_list, start=1):
                if SupportedVideoCodingFormats & codec.value:
                    supported_CodingFormats.append(codec.name)

        return supported_CodingFormats

    def getSupportedZoomModes(self, device:str="VideoDevice0"):
        """
        Returns the supported Zoom modes on device.

        Args:
            device (str, optional) : Video Device, defaults to VideoDevice0

        Returns:
            returns the list of supported Zoom modes
        """
        supported_ZoomModes = []

        deviceNum = int(device.replace(self.devicePrefix, "")) + 1
        
        device_data = self.deviceProfile.fields.get("Device")
        if isinstance(device_data, dict):
            device_entry = device_data.get(deviceNum)
        if isinstance(device_entry, dict):
              zoomModes = device_entry.get("SupportedDFCs")


        if zoomModes is None or zoomModes == 0:
            return supported_ZoomModes

        for zoomMode in zoomModes:
            supported_ZoomModes.append(dsVideoZoomMode(zoomMode).name)

        return supported_ZoomModes

# Test and example usage code
if __name__ == '__main__':

    shell = InteractiveShell()
    shell.open()

    platformProfile = dir_path + "/../../../profiles/sink/Sink_4K_VideoDevice.yaml"
    # test the class assuming that it's optional
    test = dsVideoDeviceClass(platformProfile, shell)

    test.initialise()
    ports = test.getVideoDevice()

    test.terminate()

    shell.close()