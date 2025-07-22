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
import re
from enum import Enum, auto

# Add parent directory to the system path for module imports
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class dsVideoPortType(Enum):
    dsVIDEOPORT_TYPE_RF          = 0
    dsVIDEOPORT_TYPE_BB          = auto()
    dsVIDEOPORT_TYPE_SVIDEO      = auto()
    dsVIDEOPORT_TYPE_1394        = auto()
    dsVIDEOPORT_TYPE_DVI         = auto()
    dsVIDEOPORT_TYPE_COMPONENT   = auto()
    dsVIDEOPORT_TYPE_HDMI        = auto()
    dsVIDEOPORT_TYPE_HDMI_INPUT  = auto()
    dsVIDEOPORT_TYPE_INTERNAL    = auto()
    dsVIDEOPORT_TYPE_MAX         = auto()

class dsVideoPorthdcp_version(Enum):
    dsHDCP_VERSION_1X            = 0
    dsHDCP_VERSION_2X            = auto()
    dsHDCP_VERSION_MAX           = auto()

class dsHDRStandard(Enum):
    dsHDRSTANDARD_NONE = 0x0               # When No video format is decoded
    dsHDRSTANDARD_HDR10 = 0x01             # Video Format HDR
    dsHDRSTANDARD_HLG = 0x02               # Video Format HLG
    dsHDRSTANDARD_DolbyVision = 0x04       # Video Format Dolby Vision
    dsHDRSTANDARD_TechnicolorPrime = 0x08  # Video Format Technicolor Prime
    dsHDRSTANDARD_HDR10PLUS = 0x10         # Video Format HDRPlus
    dsHDRSTANDARD_SDR = 0x20               # Video Format SDR
    dsHDRSTANDARD_Invalid = 0x80           # When invalid value observed

class dsDisplayColorDepth(Enum):
    dsDISPLAY_COLORDEPTH_UNKNOWN = 0x0  # Unknown color depth
    dsDISPLAY_COLORDEPTH_8BIT = 0x01    # 8-bit color depth
    dsDISPLAY_COLORDEPTH_10BIT = 0x02   # 10-bit color depth
    dsDISPLAY_COLORDEPTH_12BIT = 0x04   # 12-bit color depth
    dsDISPLAY_COLORDEPTH_AUTO = 0x08    # Automatic color depth

class dsVideoResolution(Enum):
    dsVIDEO_PIXELRES_720x480 = 0      # 720x480 Resolution
    dsVIDEO_PIXELRES_720x576 = 1      # 720x576 Resolution
    dsVIDEO_PIXELRES_1280x720 = 2     # 1280x720 Resolution
    dsVIDEO_PIXELRES_1366x768 = 3     # 1366x768 Resolution
    dsVIDEO_PIXELRES_1920x1080 = 4    # 1920x1080 Resolution
    dsVIDEO_PIXELRES_3840x2160 = 5    # 3840x2160 Resolution
    dsVIDEO_PIXELRES_4096x2160 = 6    # 4096x2160 Resolution
    dsVIDEO_PIXELRES_MAX = 7          # Out of range

class dsVideoAspectRatio(Enum):
    dsVIDEO_ASPECT_RATIO_4x3 = 0     # 4:3 aspect ratio
    dsVIDEO_ASPECT_RATIO_16x9 = 1    # 16:9 aspect ratio
    dsVIDEO_ASPECT_RATIO_MAX = 2     # Out of range

class dsVideoFrameRate(Enum):
    dsVIDEO_FRAMERATE_UNKNOWN = 0         # Unknown frame rate
    dsVIDEO_FRAMERATE_24 = 1              # Played at 24 frames per second
    dsVIDEO_FRAMERATE_25 = 2              # Played at 25 frames per second
    dsVIDEO_FRAMERATE_30 = 3              # Played at 30 frames per second
    dsVIDEO_FRAMERATE_60 = 4              # Played at 60 frames per second
    dsVIDEO_FRAMERATE_23dot98 = 5         # Played at 23.98 frames per second
    dsVIDEO_FRAMERATE_29dot97 = 6         # Played at 29.97 frames per second
    dsVIDEO_FRAMERATE_50 = 7              # Played at 50 frames per second
    dsVIDEO_FRAMERATE_59dot94 = 8         # Played at 59.94 frames per second
    dsVIDEO_FRAMERATE_MAX = 9             # Out of range

class dsVideoStereoScopicMode(Enum):
    dsVIDEO_SSMODE_UNKNOWN = 0           # Unknown mode
    dsVIDEO_SSMODE_2D = 1                # 2D mode
    dsVIDEO_SSMODE_3D_SIDE_BY_SIDE = 2   # 3D side by side (L/R) stereo mode
    dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM = 3 # 3D top & bottom stereo mode
    dsVIDEO_SSMODE_MAX = 4               # Out of range

class dsVideoScanModeMode(Enum):
    dsVIDEO_SCANMODE_INTERLACED = 0
    dsVIDEO_SCANMODE_PROGRESSIVE  = 1

class dsVideoPortClass():

    """
    Device Settings VideoPort Class

    This module provides common extensions for device Settings VideoPort Module.
    """
    def __init__(self, moduleConfigProfileFile :str, session=None, targetWorkspace="/tmp"):
        """
        Initializes the dsVideoPort class function.
        """
        self.moduleName     = "dsVideoPort"
        self.testConfigFile = os.path.join(dir_path, "dsVideoPort_testConfig.yml")
        self.testSuite      = "L3 dsVideoPort"

        self.moduleConfigProfile     = ConfigRead(moduleConfigProfileFile , self.moduleName)
        self.testConfig              = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.utMenu                  = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession             = session
        self.utils                   = utBaseUtils()
        self.ports                   = self.moduleConfigProfile.fields.get("Ports")

        for artifact in self.testConfig.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utils.rsync(self.testSession, filesPath, targetWorkspace)

        # Start the user interface menu
        self.utMenu.start()

    def extract_output_values(self, result: str, out_pattern: str = r'OUT:[\w_]+:\[([\w_]+)\]') -> list:
        """
        Extracts values from a given result string based on a specified pattern.

        This method uses a regular expression to find and extract relevant output
        values from the provided result string. It is particularly useful for
        parsing structured output from commands or logs.

        Args:
            result (str): The input string from which to extract values.
                        This string typically contains multiple lines of output.
            out_pattern (str, optional): The regex pattern to use for matching
                                        the desired output values. Defaults to
                                        a pattern that captures values in the
                                        format 'OUT:<identifier>:[<value>]'.

        Returns:
            list: A list of extracted values that match the specified pattern.

        Example:
            extracted_values = extract_output_values(result="OUT:Example:[Value1]\nOUT:Example:[Value2]")
        """

        # Find all matches in the result string using the provided pattern
        out_values = re.findall(out_pattern, result, re.MULTILINE)

        # Return the extracted values as a list
        return list(out_values)


    def read_Callbacks(self, input_str: str) -> str:
        """
        Reads data from the menu session until a specified input string is encountered.

        This method is useful for capturing output or response data from the menu session
        until a predefined string is reached, which can be important for synchronizing
        interactions or processing command outputs.

        Args:
            input_str (str): The string that indicates where to stop reading from the session.

        Returns:
            str: The data read from the session up to the specified input string.

        Example:
            output = read_Callbacks("EndOfResponse")
        """
        result = self.testSession.read_until(input_str)
        return result

    def initialise(self, device_type:int=0):
        """
        Initializes the device settings dsVideoPort module.

        Args:
            device_type (int, optional): 0 - sink device, 1 - source device. Defaults to sink.

        Returns:
            None
        """
        result = self.utMenu.select( self.testSuite, "VideoPort Init")

    def enablePort(self, video_port:int, port_index:int=0):
        """
        Enables the specified Video port.

        Args:
        video_port (int): The enumeration value representing the video port to enable.
                           Refer to the appropriate enum for valid options.
        port_index (int, optional): The index of the specific port to enable. Defaults to 0.

        Returns:
            None

        Example:
            enablePort(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=1)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        result = self.utMenu.select(self.testSuite, "Enable VideoPort", promptWithAnswers)

    def disablePort(self, video_port:int, port_index:int=0):
        """
        Disable the specified Video port.

        Args:
        video_port (int): The enumeration value representing the video port to enable.
                           Refer to the appropriate enum for valid options.
        port_index (int, optional): The index of the specific port to enable. Defaults to 0.

        Returns:
            None

        Example:
        disablePort(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=1)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        result = self.utMenu.select(self.testSuite, "Disable VideoPort", promptWithAnswers)

    def select_HdmiPreference(self,video_port:int, port_index:int=0,hdcp_version:int=0):
        """
        Sets the preferred HDMI protocol for the specified video port.

        This method allows the user to select the HDMI version for the given video port,
        which may include various HDCP (High-bandwidth Digital Content Protection) versions.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.
            hdcp_version (int, optional): The HDCP version to be set. Defaults to 0.

        Returns:
            None

        Example:
            select_HdmiPreference(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0, hdcp_version=1)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            },
            {
                "query_type": "list",
                "query": "Supported HDCP Versions",
                "input": "dsHDCP_VERSION_1X"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)
        promptWithAnswers[2]["input"] = str(hdcp_version)

        result = self.utMenu.select(self.testSuite, "Set HdmiPreference", promptWithAnswers)

    def enable_HDCP(self,video_port:int, port_index:int=0):
        """
        Enables HDCP (High-bandwidth Digital Content Protection) for the specified video port.

        This method allows the user to enable HDCP, which is crucial for protecting digital content
        during transmission over HDMI connections.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.

        Returns:
            None

        Example:
            enable_HDCP(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)


        result = self.utMenu.select(self.testSuite, "Enable HDCP", promptWithAnswers)

    def disable_HDCP(self,video_port:int, port_index:int=0):
        """
        Disables HDCP (High-bandwidth Digital Content Protection) for the specified video port.

        This method allows users to disable HDCP, which is useful when you need to bypass
        content protection for testing or troubleshooting purposes.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.

        Returns:
            None

        Example:
            disable_HDCP(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)


        result = self.utMenu.select(self.testSuite, "Disable HDCP", promptWithAnswers)

    def select_Resolution(self,video_port:int, port_index:int=0, resolution: dict = None ):
        """
        Sets the resolution of the specified video port.

        This method configures the resolution settings for a video port, including pixel resolution,
        aspect ratio, stereoscopic mode, frame rate, and scan mode. It provides options based on
        the specified parameters or defaults to predefined values if not provided.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.
            resolution (dict, optional): A dictionary containing resolution settings:
                - "pixelResolution" (str): Desired pixel resolution (e.g., 'dsVIDEO_PIXELRES_1920x1080').
                - "aspectRatio" (str): Desired aspect ratio (e.g., 'dsVIDEO_ASPECT_RATIO_16x9').
                - "stereoScopicMode" (str): Desired stereoscopic mode (e.g., 'dsVIDEO_SSMODE_2D').
                - "frameRate" (str): Desired frame rate (e.g., 'dsVIDEO_FRAMERATE_24').
                - "interlaced" (str): Desired scan mode (e.g., 'dsVIDEO_SCANMODE_INTERLACED').

        Returns:
            None

        Example:
            select_Resolution(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0, resolution={
                "pixelResolution": "dsVIDEO_PIXELRES_3840x2160",
                "aspectRatio": "dsVIDEO_ASPECT_RATIO_16x9",
                "stereoScopicMode": "dsVIDEO_SSMODE_3D",
                "frameRate": "dsVIDEO_FRAMERATE_60",
                "interlaced": "dsVIDEO_SCANMODE_PROGRESSIVE"
            })
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }]
        """
        promptWithAnswers.append(
            {
                "query_type": "list",
                "query": "Select Resolution",
                "input": "dsVIDEO_PIXELRES_1920x1080"
            },
            {
                "query_type": "list",
                "query": "Select Aspect Ratio",
                "input": "dsVIDEO_ASPECT_RATIO_16x9"
            },
            {
                "query_type": "list",
                "query": "Select Stereo ScopicMode",
                "input": "dsVIDEO_SSMODE_2D"
            },
            {
                "query_type": "list",
                "query": "Select Frame Rates",
                "input": "dsVIDEO_FRAMERATE_24"
            },
            {
                "query_type": "list",
                "query": "Select Scan modes",
                "input": "dsVIDEO_SCANMODE_INTERLACED"
            }
        )
        """

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Check if a valid resolution dictionary is provided
        if resolution:
            # Add resolution-related prompts using the provided resolution details
            promptWithAnswers.extend([
                {
                "query_type": "list",
                "query": "Supported Resolution",
                "input": resolution.get("pixelResolution", "dsVIDEO_PIXELRES_1920x1080")  # Fallback if missing
                },
                {
                    "query_type": "list",
                    "query": "Supported Aspect Ratio",
                    "input": resolution.get("aspectRatio", "dsVIDEO_ASPECT_RATIO_16x9")  # Fallback if missing
                },
                {
                    "query_type": "list",
                    "query": "Supported Stereo ScopicMode",
                    "input": resolution.get("stereoScopicMode", "dsVIDEO_SSMODE_2D")  # Fallback if missing
                },
                {
                    "query_type": "list",
                    "query": "Supported Frame Rates",
                    "input": resolution.get("frameRate", "dsVIDEO_FRAMERATE_24")  # Fallback if missing
                },
                {
                    "query_type": "list",
                    "query": "Supported Scan modes",
                    "input": resolution.get("interlaced", "dsVIDEO_SCANMODE_INTERLACED")  # Fallback if missing
                }
            ])
        else:
            # If no resolution is provided, use defaults or provide a prompt
            print("No resolution provided, using defaults.")

        result = self.utMenu.select(self.testSuite, "Set Resolution", promptWithAnswers)

    def select_HDRModes(self,video_port:int, port_index:int=0,hdr_mode:int=0):
        """
        Sets or resets the HDR mode for the specified video port.

        This method allows you to configure the High Dynamic Range (HDR) settings
        for a particular video port, which may enhance the visual experience
        by adjusting brightness and color settings.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.
            hdr_mode (int, optional): The desired HDR mode.
                                    Refer to the dsHDRSTANDARD enum for valid options.

        Returns:
            None

        Example:
            select_HDRModes(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0, hdr_mode=dsHDRSTANDARD_HDR10)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            },
            {
                "query_type": "list",
                "query": "Select HDR modes",
                "input": "dsHDRSTANDARD_SDR"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)
        promptWithAnswers[2]["input"] = str(hdr_mode)

        result = self.utMenu.select(self.testSuite, "Set ForceHDRMode", promptWithAnswers)

    def resetOutputToSDR(self,video_port:int, port_index:int=0):
        """
        Resets the video output to Standard Dynamic Range (SDR) for the specified video port.

        This method allows you to revert the current video output settings to SDR,
        which is a common display standard. This can be useful for compatibility
        with various devices and content that do not support High Dynamic Range (HDR).

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to reset. Defaults to 0.

        Returns:
            None

        Example:
            resetOutputToSDR(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)


        result = self.utMenu.select(self.testSuite, "ResetOutputToSDR", promptWithAnswers)

    def select_PreferredColorDepth(self,video_port:int, port_index:int=0,color_depth:int=0):
        """
        Sets the preferred color depth for the specified video port.

        This method allows you to configure the color depth of a video port,
        which can affect the quality of the video output. Higher color depths
        enable more colors and smoother gradients, enhancing visual quality.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.
            color_depth (int, optional): The desired color depth to set. Defaults to 0.

        Returns:
            None

        Example:
            select_PreferredColorDepth(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0, color_depth=dsDISPLAY_COLORDEPTH_8BIT)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            },
            {
                "query_type": "list",
                "query": "Select Color depth to set",
                "input": "dsDISPLAY_COLORDEPTH_8BIT"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)
        promptWithAnswers[2]["input"] = str(color_depth)

        result = self.utMenu.select(self.testSuite, "Set PreferredColorDepth", promptWithAnswers)

    def select_BackgroundColor(self,video_port:int, port_index:int=0,background_color:int=0):
        """
        Sets the background color of the specified video port.

        This method allows you to configure the background color for a video port,
        which can enhance the visual experience or provide a specific aesthetic
        based on the content being displayed.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to configure. Defaults to 0.
            background_color (int, optional): The desired background color to set.
                                            Defaults to 0.

        Returns:
            None

        Example:
            select_BackgroundColor(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0, background_color=dsVIDEO_BGCOLOR_BLUE)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            },
            {
                "query_type": "list",
                "query": "Supported Background Color",
                "input": "dsVIDEO_BGCOLOR_BLUE"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)
        promptWithAnswers[2]["input"] = str(background_color)

        result = self.utMenu.select(self.testSuite, "Set BackgroundColor", promptWithAnswers)

    def getCurrentOutputSettings(self,video_port:int, port_index:int=0):
        """
        Retrieves the current output settings for a specified video port.

        This method fetches various output parameters for a given video port, including:
        - Color space setting
        - Color depth
        - Matrix coefficients
        - Video Electro-Optical Transfer Function (EOTF)
        - Quantization range

        Args:
            video_port (int): The identifier of the video port. Refer to the `dsVideoPortType` enumeration for valid values.
            port_index (int, optional): The index of the video device. Defaults to 0.

        Returns:
            list: A list of output settings extracted from the selected video port. The list includes:
                - Current color space
                - Current color depth
                - Current matrix coefficients
                - Current EOTF
                - Current quantization range

        Example:
            output_settings = getCurrentOutputSettings(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=1)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        result = self.utMenu.select(self.testSuite, "Get CurrentOutputSettings", promptWithAnswers)

        # Extract and return the output values from the result
        output_list = self.extract_output_values(result)
        return output_list

    def getResolution(self,video_port:int, port_index:int=0):
        """
        Retrieves the display resolution of the specified video port.

        This method allows the user to obtain the current resolution settings
        of a particular video output, which is useful for ensuring compatibility
        with display devices or for troubleshooting display issues.

        Args:
            video_port (int): The enumeration value representing the video port.
                            Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): The index of the specific port to query.
                                        Defaults to 0.

        Returns:
            list: A list containing the current resolution settings, including
                width, height, and possibly other related information.

        Example:
            resolution = getResolution(video_port=dsVIDEOPORT_TYPE_HDMI, port_index=0)
        """

        # Prepare prompts for user input to select the video port and index
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        result = self.utMenu.select(self.testSuite, "Get Resolution", promptWithAnswers)

        # Extract and return the output values from the result
        output_list = self.extract_output_values(result)

        return output_list

    def getVideoEOTF(self,video_port:int, port_index:int=0):
        """
            Gets the display resolution of specified video port.

            Args:
            video_port (int): Enum value representing the video port. Refer to dsVideoPortType.
            port_index (int, optional): The index of the video device. Defaults to 0.

        Returns:
            list: A list of EOTF settings retrieved for the specified video port.

        Example:
            eotf_settings = getVideoEOTF(video_port=1)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Convert input arguments to strings and update the prompts
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Select the desired video EOTF settings using the menu interface
        result = self.utMenu.select(self.testSuite, "Get VideoEOTF", promptWithAnswers)

        # Extract and return the output values from the result
        output_list = self.extract_output_values(result)

        return output_list

    def isOutputHDR(self,video_port:int, port_index:int=0):
        """
        Checks whether the video output for the specified video port is in HDR format.

        This method verifies if the output signal from the video port is High Dynamic Range (HDR),
        which enhances the contrast and color range compared to standard video output.

        Args:
            video_port (int): Enum value representing the video port. Refer to dsVideoPortType.
            port_index (int, optional): The index of the video device. Defaults to 0.

        Returns:
            bool: True if the video output is HDR, False otherwise.

        Example:
            is_hdr = isOutputHDR(video_port=1)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Check the output HDR status using the menu interface
        result = self.utMenu.select(self.testSuite, "IsOutputHDR", promptWithAnswers)

        # Extract the output values to determine HDR status
        output_list = self.extract_output_values(result)

        return bool(output_list)

    def getHDCPStatus(self,video_port:int, port_index:int=0):
        """
        Retrieves the current HDCP (High-bandwidth Digital Content Protection) status for the specified video port.

        HDCP is used to protect content being transmitted over digital interfaces. This method checks the status
        of HDCP on the selected video port.

        Args:
            video_port (int): Enum value representing the video port. Refer to dsVideoPortType for valid options.
            port_index (int, optional): The index of the video device. Defaults to 0.

        Returns:
            list: A list of extracted values related to HDCP status.

        Example:
            hdcp_status = getHDCPStatus(video_port=1, port_index=0)
        """
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update the prompt inputs with provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Select and execute the command to get HDCP status
        result = self.utMenu.select(self.testSuite, "Get HDCPStatus", promptWithAnswers)

        # Extract relevant output values
        output_list = self.extract_output_values(result)

        return output_list

    def getHDCPCurrentProtocol(self,video_port:int, port_index:int=0):
        """
        Retrieves the currently negotiated HDCP (High-bandwidth Digital Content Protection) protocol version for the specified video port.

        This method queries the HDCP version (e.g., HDCP 1.4, HDCP 2.2) that is actively negotiated on the selected video output.

        Args:
            video_port (int): Enum value representing the video port. Refer to the dsVideoPortType for valid values.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the current HDCP protocol version information.

        Example:
            hdcp_protocol = getHDCPCurrentProtocol(video_port=1)
        """
        # Define the prompt structure for user selection
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update the prompts with provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Execute the command to retrieve HDCP protocol information
        result = self.utMenu.select(self.testSuite, "Get HDCPCurrentProtocol", promptWithAnswers)

        # Extract relevant output values using a helper function
        output_list = self.extract_output_values(result)

        return output_list

    def getHdmiPreference(self,video_port:int, port_index:int=0):
        """
        Retrieves the preferred HDMI protocol version for the specified video port.

        This method queries the preferred HDMI protocol version (e.g., HDMI 1.4, HDMI 2.0) configured for the selected video port.

        Args:
            video_port (int): Enum value representing the video port. Refer to the dsVideoPortType for valid values.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the preferred HDMI protocol version information.

        Example:
            hdmi_preference = getHdmiPreference(video_port=1)
        """
        # Define the prompt structure for user selection
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update the prompts with provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Execute the command to retrieve the preferred HDMI protocol version
        result = self.utMenu.select(self.testSuite, "Get HdmiPreference", promptWithAnswers)

        # Extract relevant output values using a helper function
        output_list = self.extract_output_values(result)

        return output_list

    def getColorSpace(self,video_port:int, port_index:int=0):
        """
        Retrieves the color space setting of the specified video port.

        This method fetches the current color space configuration (e.g., RGB, YUV) for the provided video port.

        Args:
            video_port (int): Enum value representing the video port. Refer to dsVideoPortType enum for valid values.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the color space information.

        Example:
            color_space = getColorSpace(video_port=1)
        """
        # Define the prompt structure for user selection
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update the prompts with provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Execute the command to retrieve the color space setting
        result = self.utMenu.select(self.testSuite, "Get ColorSpace", promptWithAnswers)

        # Extract relevant output values using the helper function
        output_list = self.extract_output_values(result)
        return output_list

    def getColorDepth(self,video_port:int, port_index:int=0):
        """
        Retrieves the color depth value of the specified video port.

        This function fetches the current color depth setting (e.g., 8-bit, 10-bit) for the provided video port.

        Args:
            video_port (int): Enum value representing the video port. Refer to the dsVideoPortType enum for valid options.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the color depth information.

        Example:
            color_depth = getColorDepth(video_port=1)
        """
        # Define prompts for user interaction
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update prompts based on provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Retrieve the color depth information by executing the menu command
        result = self.utMenu.select(self.testSuite, "Get ColorDepth", promptWithAnswers)

        # Extract the relevant output values using the helper function
        output_list = self.extract_output_values(result)
        return output_list

    def getHDCPReceiverProtocol(self,video_port:int, port_index:int=0):
        """
        Retrieves the HDCP (High-bandwidth Digital Content Protection) protocol version
        used by the connected sink device on the specified video port.

        Args:
            video_port (int): Enum value representing the video port. Refer to the dsVideoPortType enum.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the HDCP protocol version used by the connected receiver.

        Example:
            hdcp_protocol = getHDCPReceiverProtocol(video_port=1)
        """
        # Define the prompts for querying the HDCP receiver protocol
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update prompts based on the provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Execute the command to get the HDCP receiver protocol
        result = self.utMenu.select(self.testSuite, "Get HDCPReceiverProtocol", promptWithAnswers)

        # Extract the protocol version from the result
        output_list = self.extract_output_values(result)

        return output_list

    def getIgnoreEDIDStatus(self,video_port:int, port_index:int=0):
        """
        Retrieves the IgnoreEDID status for the specified video port. The IgnoreEDID flag indicates
        whether the device is ignoring the Extended Display Identification Data (EDID) provided by the sink.

        Args:
            video_port (int): Enum value representing the video port. Refer to the dsVideoPortType enum.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the IgnoreEDID status of the device.

        Example:
            edid_status = getIgnoreEDIDStatus(video_port=1)
        """

        # Define prompts for querying IgnoreEDID status
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update prompts based on the provided arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Extract the protocol version from the result
        result = self.utMenu.select(self.testSuite, "Get IgnoreEDIDStatus", promptWithAnswers)

        # Extract and return the output values from the result
        output_list = self.extract_output_values(result)

        return output_list

    def getPreferredColorDepth(self,video_port:int, port_index:int=0):
        """
        Retrieves the preferred color depth setting for the specified video port.

        Args:
            video_port (int): Enum value representing the video port. Refer to dsVideoPortType enum.
            port_index (int, optional): Index of the video device. Defaults to 0.

        Returns:
            list: A list containing the preferred color depth values.

        Example:
            color_depth = getPreferredColorDepth(video_port=1)
        """
        # Define prompt structure to get preferred color depth
        promptWithAnswers = [
            {
                "query_type": "list",
                "query": "Select the Video Port",
                "input": "dsVIDEOPORT_TYPE_HDMI"
            },
            {
                "query_type": "direct",
                "query": "Select the Video Port Index[0-9]:",
                "input": "0"
            }
        ]

        # Update the prompts based on function arguments
        promptWithAnswers[0]["input"] = str(video_port)
        promptWithAnswers[1]["input"] = str(port_index)

        # Run the query and get the result
        result = self.utMenu.select(self.testSuite, "Get PreferredColorDepth", promptWithAnswers)

        # Extract and return the output list
        output_list = self.extract_output_values(result)

        return output_list

    def terminate(self):
        """
        Enables the Video port.

        Args:
            None.

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "VideoPort Term")

    def getSupportedPorts(self):
        """
        Returns the supported Video ports on device.

        Args:
            None.

        Returns:
            returns the list of supported Video ports
        """
        portLists = []

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            portLists.append([dsVideoPortType(entry['Typeid']).name, entry['Index']])

        return portLists

    def getHDCPVersion(self):
        """
        Returns the supported HDCP on device.

        Args:
            None.

        Returns:
            returns the supported hdcp_protocol_version
        """
        hdcp_protocol_version = 0

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            hdcp_protocol_version = dsVideoPorthdcp_version(entry['hdcp_protocol_version']).name

        return hdcp_protocol_version

    def getResolutions(self):
        """
        Returns the supported Resolutions on device.

        Args:
            None.

        Returns:
            returns the supported resolutions
        """
        # Store the resolutions in a list
        resolutions_list = []
        pixel_res = 0

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            num_supported_resolutions = entry["numSupportedResolutions"]
            supported_resolutions = entry["supportedResolutions"]
            for j in range(1, num_supported_resolutions + 1):
                resolutions_data = supported_resolutions.get(j)
                if resolutions_data:
                    # Convert pixelResolution value to dsVideoResolution enum and add to list
                    pixel_res = resolutions_data.get("pixelResolution")
                    resolution_enum = dsVideoResolution(pixel_res).name
                    aspectRatio_res = resolutions_data.get("aspectRatio")
                    aspectRatio_enum = dsVideoAspectRatio(aspectRatio_res).name
                    stereoScopicMode_res = resolutions_data.get("stereoScopicMode")
                    stereoScopicMode_enum = dsVideoStereoScopicMode(stereoScopicMode_res).name
                    frameRate_res = resolutions_data.get("frameRate")
                    frameRate_enum = dsVideoFrameRate(frameRate_res).name
                    scanModes_res = resolutions_data.get("interlaced")
                    scanModes_enum = dsVideoScanModeMode(scanModes_res).name

                    resolutions_list.append({
                        "pixelResolution":resolution_enum,
                        "aspectRatio":aspectRatio_enum,
                        "stereoScopicMode":stereoScopicMode_enum,
                        "frameRate":frameRate_enum,
                         "interlaced":scanModes_enum
                        })

        return resolutions_list

    def getAspectRatio(self):
        """
        Returns the supported aspectRatio on device.

        Args:
            None.

        Returns:
            returns the supported aspectRatio
        """
        # Store the resolutions in a list
        aspectRatio_list = []

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            num_supported_resolutions = entry["numSupportedResolutions"]
            supported_resolutions = entry["supportedResolutions"]
            for j in range(1, num_supported_resolutions + 1):
                aspectRatio_data = supported_resolutions.get(j)
                if aspectRatio_data:
                    # Convert aspectRatio value to dsVideoAspectRatio enum and add to list
                    aspectRatio_res = aspectRatio_data.get("aspectRatio")
                    aspectRatio_enum = dsVideoAspectRatio(aspectRatio_res).name
                    aspectRatio_list.append(aspectRatio_enum)

        return aspectRatio_list

    def getStereoScopicMode(self):
        """
        Returns the supported stereoScopicMode on device.

        Args:
            None.

        Returns:
            returns the supported stereoScopicMode
        """
        # Store the resolutions in a list
        stereoScopicMode_list = []

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            num_supported_resolutions = entry["numSupportedResolutions"]
            supported_resolutions = entry["supportedResolutions"]
            for j in range(1, num_supported_resolutions + 1):
                stereoScopicMode_data = supported_resolutions.get(j)
                if stereoScopicMode_data:
                    # Convert stereoScopicMode value to dsVideoStereoScopicMode enum and add to list
                    stereoScopicMode_res = stereoScopicMode_data.get("stereoScopicMode")
                    stereoScopicMode_enum = dsVideoStereoScopicMode(stereoScopicMode_res).name
                    stereoScopicMode_list.append(stereoScopicMode_enum)

        return stereoScopicMode_list

    def getFrameRates(self):
        """
        Returns the supported Frame rates on device.

        Args:
            None.

        Returns:
            returns the supported Frame rates
        """
        # Store the resolutions in a list
        frameRates_list = []

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            num_supported_resolutions = entry["numSupportedResolutions"]
            supported_resolutions = entry["supportedResolutions"]
            for j in range(1, num_supported_resolutions + 1):
                frameRate_data = supported_resolutions.get(j)
                if frameRate_data:
                    # Convert frameRate value to dsVideoFrameRate enum and add to list
                    frameRate_res = frameRate_data.get("frameRate")
                    frameRate_enum = dsVideoFrameRate(frameRate_res).name
                    frameRates_list.append(frameRate_enum)

        return frameRates_list

    def getScanModes(self):
        """
        Returns the supported ScanModes on device.

        Args:
            None.

        Returns:
            returns the supported ScanModes
        """
        # Store the resolutions in a list
        scanModes_list = []

        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            num_supported_resolutions = entry["numSupportedResolutions"]
            supported_resolutions = entry["supportedResolutions"]
            for j in range(1, num_supported_resolutions + 1):
                scanModes_data = supported_resolutions.get(j)
                if scanModes_data:
                    # Convert interlaced value to dsVideoScanModeMode enum and add to list
                    scanModes_res = scanModes_data.get("interlaced")
                    scanModes_enum = dsVideoScanModeMode(scanModes_res).name
                    scanModes_list.append(scanModes_enum)

        return scanModes_list

    def getHDRCapabilities(self):
        """
        Returns the supported HDR Formats on device.

        Args:
            None.

        Returns:
            returns the supported HDR Formats
        """
        supported_HDRformats = []


        for i in range(1, len(self.ports)+1):
            entry = self.ports[i]
            hdr_capabilities = entry['hdr_capabilities']
            for hdr_format in dsHDRStandard:
                if hdr_capabilities & hdr_format.value:  # Check if the format is supported
                    supported_HDRformats.append(hdr_format.name)

        return supported_HDRformats

    def getColorDepth(self):
        """
        Returns the supported Color depth.

        Args:
            None.

        Returns:
            returns the color depth
        """
        color_depth = 0

        color_depth = dsDisplayColorDepth(self.moduleConfigProfile.get("color_depth")).name
        return color_depth

    def getDeviceType(self):
        """
        Returns the supported Video ports on device.

        Args:
            None.

        Returns:
            returns the device type (0-Sink device, 1-Source device)
        """
        portLists = []

        type = self.moduleConfigProfile.get("Type")
        if type == "sink":
            return 0
        elif type == "source":
            return 1

    def __del__(self):
        """
        De-Initializes the dsVideoPort helper function.

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

    platformProfile = dir_path + "/../../../profiles/source/Source_4K_VideoPort.yaml"
    # test the class assuming that it's optional
    test = dsVideoPortClass(platformProfile, shell)

    test.initialise()
    ports = test.getSupportedPorts()

    test.enablePort(ports[0][0], ports[0][1])
    test.disablePort(ports[0][0], ports[0][1])

    test.terminate()

    shell.close()
