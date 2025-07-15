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
import os
import sys
from enum import Enum, auto
import re
import yaml

# Add parent directory to the system path for module imports
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class dsAudioPortType(Enum):
    """Enumeration for different audio port types."""
    dsAUDIOPORT_TYPE_ID_LR     = 0
    dsAUDIOPORT_TYPE_HDMI      = auto()
    dsAUDIOPORT_TYPE_SPDIF     = auto()
    dsAUDIOPORT_TYPE_SPEAKER   = auto()
    dsAUDIOPORT_TYPE_HDMI_ARC  = auto()
    dsAUDIOPORT_TYPE_HEADPHONE = auto()

class dsAudioStereoMode(Enum):
    """Enumeration for audio stereo modes."""
    dsAUDIO_STEREO_MONO     = 1
    dsAUDIO_STEREO_STEREO   = auto()
    dsAUDIO_STEREO_SURROUND = auto()
    dsAUDIO_STEREO_PASSTHRU = auto()
    dsAUDIO_STEREO_DD       = auto()
    dsAUDIO_STEREO_DDPLUS   = auto()

class dsMS12Capabilities(Enum):
    """Enumeration for audio processing capabilities."""
    DolbyVolume          = 0x01
    IntelligentEqualizer = 0x02
    DialogueEnhancer     = 0x04
    Volumeleveller       = 0x08
    BassEnhancer         = 0x10
    SurroundDecoder      = 0x20
    DRCMode              = 0x40
    SurroundVirtualizer  = 0x80
    MISteering           = 0x100
    GraphicEqualizer     = 0x200
    LEConfig             = 0x400

class dsAudioClass():
    """
    Device Settings Audio Class.

    This module provides common functionalities and extensions for the device Settings Audio Module.
    """

    def __init__(self, moduleConfigProfileFile :str, session=None, targetWorkspace="/tmp"):
        """
        Initializes the dsAudioClass instance with configuration settings.

        Args:
            moduleConfigProfileFile  (str): Path to the device profile configuration file.
            session: Optional; session object for the user interface.

        Returns:
            None
        """
        self.moduleName = "dsAudio"
        self.testConfigFile = os.path.join(dir_path, "dsAudio_testConfig.yml")
        self.testSuite = "L3 dsAudio"

        # Load configurations for device profile and menu
        self.moduleConfigProfile = ConfigRead( moduleConfigProfileFile , self.moduleName)
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.utMenu        = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession   = session
        self.utils         = utBaseUtils()
        self.ports = self.moduleConfigProfile.fields.get("Ports")

        for artifact in self.testConfig.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utils.rsync(self.testSession, filesPath, targetWorkspace)

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
            - The pattern should contain at least one capturing group (parentheses).
            - If no match is found, None is returned.
        """
        match = re.search(pattern, haystack)
        if match:
            return match.group(1)
        return None

    def initialise(self, device_type:int=0):
        """
        Initializes the audio module settings.

        Args:
            device_type (int, optional):
                - 0 for Sink device (default).
                - 1 for Source device.

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
        result = self.utMenu.select( self.testSuite, "Initialize dsAudio", promptWithAnswers)

    def terminate(self):
        """
        Terminates the audio module

        Args:
            None

        Returns:
            None
        """
        result = self.utMenu.select(self.testSuite, "Terminate dsAudio")

    def enablePort(self, audio_port:str, port_index:int=0, arc_type:int=2):
        """
        Enables the specified audio port, with options for selecting port index and ARC type if applicable.

        Args:
            audio_port (str): The audio port to be enabled (refer dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            arc_type (int, optional): The type of Audio Return Channel (ARC) to be used (default is eArc i.e 2).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                }
        ]

        # If the selected audio port is HDMI ARC, prompt for the ARC type
        if audio_port == dsAudioPortType.dsAUDIOPORT_TYPE_HDMI_ARC.name:
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select ARC Type:",
                    "input": str(arc_type)
                })

        result = self.utMenu.select(self.testSuite, "Enable Audio Port", promptWithAnswers)

    def disablePort(self, audio_port:str, port_index:int=0):
        """
        Disables the specified audio port, with an option to select the port index.

        Args:
            audio_port (str): The audio port to be enabled (refer dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Disable Audio Port", promptWithAnswers)

    def setGainLevel(self, audio_port:str, port_index:int=0, gainLevel:float=0.0):
        """
        Sets the audio gain level for the specified audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            gainLevel (float, optional): The gain level to be applied, ranging from 0.0 to 100.0 (default is 0.0).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "direct",
                    "query": "Enter Gain Level[0.0 to 100.0]:",
                    "input": str(gainLevel)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Set Audio Level", promptWithAnswers)

    def setSpeakerGain(self, audio_port:str, port_index:int=0, gain:float=0.0):
        """
        Sets the gain level for the specified speaker audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            gain (float, optional): The gain value to be applied, ranging from -2080.0 to 480.0 (default is 0.0).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Enter Gain[-2080.0 to 480.0]:",
                    "input": str(gain)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Set Audio Gain For Speaker", promptWithAnswers)

    def setAudioMute(self, audio_port:str, port_index:int=0, mute:bool=True):
        """
        Mutes or unmutes the audio for the specified audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            mute (bool, optional): Indicates whether to mute (True) or unmute (False) the audio (default is True).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "direct",
                    "query": "Audio Mute/UnMute[1:Mute, 2:UnMute]:",
                    "input": self.boolToString(mute)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Audio Mute/UnMute", promptWithAnswers)

    def setAudioDelay(self, audio_port:str, port_index:int=0, delay:int=0):
        """
        Sets the audio delay for the specified audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            delay (int, optional): The audio delay to be applied, ranging from 0 to 200 milliseconds (default is 0).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "direct",
                    "query": "Enter Audio Delay in milli seconds[0 to 200]:",
                    "input": str(delay)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Set Audio Delay", promptWithAnswers)

    def setAudioCompression(self, audio_port:str, port_index:int=0, compression:int=0):
        """
        Sets the audio compression level for the specified audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            compression (int, optional): The audio compression level to be applied, ranging from 0 to 10 (default is 0).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "direct",
                    "query": "Select Compression[0-10]:",
                    "input": str(compression)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Audio Compression", promptWithAnswers)

    def setOutputMode(self, audio_port:str, port_index:int=0, mode:str = "dsAUDIO_STEREO_PASSTHRU"):
        """
        Sets the output mode for the specified audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            mode (str, optional): The output mode for the audio port (refer to dsAudioStereoMode; default is "dsAUDIO_STEREO_PASSTHRU").

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": audio_port
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "list",
                    "query": "Select Stereo Mode:",
                    "input": mode
                }
        ]

        result = self.utMenu.select(self.testSuite, "Set Stereo Mode", promptWithAnswers)

    def setMS12Feature(self, audio_port:str, port_index:int=0, feature:dict = None):
        """
        Sets the audio compression.

        Args:
            audio_port (str): Name of the audio port (refer to dsAudioPortType enum).
            port_index (int, optional): Port index. Defaults to 0.
            feature (dict, optional): Dictionary containing the feature to be enabled and its value.
                        Keys should be feature names (e.g., "DolbyVolume", "IntelligentEqualizer" refer: dsMS12Capabilities).
                        Values depend on the feature:
                            - Boolean features: True/False
                            - Single-value features: String or int representing the value
                            - Multi-value features: List containing two values (e.g., [mode, level])
                        Defaults to None.

        Returns:
            None
        """

        if not feature or len(feature) != 2:
            return False  # Handle missing or invalid feature data

        feature_name = feature["name"]
        feature_value = feature["value"]

        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": str(audio_port)
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "list",
                    "query": "Select MS12 DAP Feature:",
                    "input": feature_name
                }
        ]

        if(feature_name == "DolbyVolume"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Dolby Volume[1:Enable, 2:Disable]:",
                    "input": self.boolToString(feature_value)
                })
        elif(feature_name == "IntelligentEqualizer"):
            promptWithAnswers.append({
                    "query_type": "list",
                    "query": "Select IntelligentEqualizer Mode:",
                    "input": feature_value
                })
        elif(feature_name == "DialogueEnhancer"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select DialogueEnhancer Level[0-16]:",
                    "input": str(feature_value)
                })
        elif(feature_name == "Volumeleveller"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select Volumeleveller Mode[0:Off, 1:On, 2:Auto]:",
                    "input": str(feature_value[0])
                })
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select Volumeleveller Level[0-10]:",
                    "input": str(feature_value[1])
                })
        elif(feature_name == "BassEnhancer"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select BassEnhancer Boost[0-100]:",
                    "input": str(feature_value)
                })
        elif(feature_name == "SurroundDecoder"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Surround Decoder[1:Enable, 2:Disable]:",
                    "input": self.boolToString(feature_value)
                })
        elif(feature_name == "DRCMode"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select DRC Mode[0: DRC Line Mode, 1: DRC RF Mode]:",
                    "input": str(feature_value)
                })
        elif(feature_name == "SurroundVirtualizer"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select SurroundVirtualizer Mode[0:Off, 1:On, 2:Auto]:",
                    "input": str(feature_value[0])
                })
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select SurroundVirtualizer Boost[0-96]:",
                    "input": str(feature_value[1])
                })
        elif(feature_name == "MISteering"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "MI Steering[1:Enable, 2:Disable]:",
                    "input": self.boolToString(feature_value)
                })
        elif(feature_name == "GraphicEqualizer"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Select GraphicEqualizer Mode[0: OFF, 1: EQ Open, 2: EQ Rich, 3: EQ Focused]:",
                    "input": str(feature_value)
                })
        elif(feature_name == "LEConfig"):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "LE Configuration[1:Enable, 2:Disable]:",
                    "input": self.boolToString(feature_value)
                })
        else:
            return

        result = self.utMenu.select(self.testSuite, "MS12 DAP Features", promptWithAnswers)

    def setMS12AudioProfile(self, audio_port:str, port_index:int=0, profile:str='Off'):
        """
        Sets the MS12 Audio Profile for the specified audio port.

        Args:
            audio_port (str): The name of the audio port (refer to dsAudioPortType).
            port_index (int, optional): The index of the audio port (default is 0).
            profile (str, optional): The MS12 profile to be set. Supported profiles include: Off, Music, Movie, Sports, Entertainment, Night, Party, User.
                Defaults to 'Off'.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select dsAudio Port:",
                    "input": str(audio_port)
                },
                {
                    "query_type": "direct",
                    "query": "Select dsAudio Port Index[0-10]:",
                    "input": str(port_index)
                },
                {
                    "query_type": "list",
                    "query": "Select MS12 Profile:",
                    "input": profile
                }
        ]
        result = self.utMenu.select(self.testSuite, "Set MS12 Profiles", promptWithAnswers)

    def enableAssociateAudioMixig(self, enable:bool = True, fader:int = 0):
        """
        Enables or disables associated audio mixing for the system.

        Args:
            enable (bool, optional): If True, enables associated audio mixing; if False, disables it (default is True).
            fader (int, optional): Control level for the fader; range is -32 (mute associated audio) to 32 (mute main audio). Defaults to 0.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Enable/Disable Associated Audio Mixing[1:Enable, 2:Disable]:",
                    "input": self.boolToString(enable)
                }
        ]

        if enable:
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": "Set Fader Control[-32(mute associated audio) to 32(mute main audio)]:",
                    "input": str(fader)
                })

        result = self.utMenu.select(self.testSuite, "Set Associate Audio Mixing", promptWithAnswers)

    def setAudioMixerLevels(self, mixer_input:str, volume:int = 0):
        """
        Sets the audio mixer levels for primary or system audio.

        Args:
            mixer_input (str): Indicates which audio level to set; use 'primary' for primary audio or 'system' for system audio.
            volume (int, optional): The volume level to set, ranging from 0 to 100 (default is 0).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "list",
                    "query": "Select Mixer Input:",
                    "input": mixer_input
                },
                {
                    "query_type": "direct",
                    "query": "Set the Volume[0 to 100]:",
                    "input": str(volume)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Set Audio Mixer Levels", promptWithAnswers)

    def setPrimarySecondaryLanguage(self, language_type:str, language:str):
        """
        Sets the primary or secondary language for the audio system.

        Args:
            language_type (str): Specifies the language type to set; use 'Primary' for primary language or 'Secondary' for secondary language.
            language (str): The 3-letter language code as per ISO 639-3 (e.g., 'eng' for English).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the Language Type[1: Primary, 2: Secondary]:",
                    "input": "1"
                },
                {
                    "query_type": "direct",
                    "query": "Enter 3 letter long language as per ISO 639-3:",
                    "input": language
                }
        ]

        if (language_type == "Primary"):
            promptWithAnswers[0]["input"] = "1"
        else:
            promptWithAnswers[0]["input"] = "2"

        result = self.utMenu.select(self.testSuite, "Primary/Secondary Language", promptWithAnswers)

    def setARCSAD(self, sadList:list=[]):
        """
        Sets the ARC SAD (short audio description) values.

        Args:
            sadList (list): A list of SAD values to be set. The length should be between 1 and 15.

        Returns:
            None
        """
        if (len(sadList) == 0):
            return

        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Enter SAD List count[1-15]:",
                    "input": str(len(sadList))
                }
        ]

        for i, sad in enumerate(sadList):
            promptWithAnswers.append({
                    "query_type": "direct",
                    "query": f"Enter {i} SAD Value:",
                    "input": str(sad)
                })

        result = self.utMenu.select(self.testSuite, "Set SAD List", promptWithAnswers)

    def getHeadphoneConnectionStatus(self):
        """
        Retrieves the current connection status of the headphone port.

        Args:
            None.

        Returns:
            bool: True if headphones are connected, False otherwise.
        """
        result = self.utMenu.select( self.testSuite, "Headphone Connection")
        connectionStatusPattern = r"Result dsAudioOutIsConnected\(IN:handle:\[.*\], OUT:isConnected:\[(true|false)\]\)"
        isConnected = self.searchPattern(result, connectionStatusPattern)
        if(isConnected == "true"):
            return True
        return False

    def getHeadphoneConnectionCallbackStatus(self):
        """
        Retrieves the headphone connection status from a system callback.

        Args:
            None.

        Returns:
            None: If the function fails to retrieve the connection status.
            tuple: A tuple containing:
                - port (str): The name of the audio port (refer to dsAudioPortType).
                - index (int): The index of the port.
                - status (bool): The connection status (True if connected, False if disconnected).
        """
        result = self.testSession.read_until("Received Connection status callback port")
        connectionCallbackPattern = r"Received Connection status callback port: (\w+), port number: (\d+), Connection: (\w+)"
        match = re.search(connectionCallbackPattern, result)

        if match:
            port = match.group(1)
            index = int(match.group(2))
            connection = match.group(3) == "true"
            return port, index, connection
        return None

    def getAudioFormat(self):
        """
        Retrieves the current audio format being Played.

        Args:
            None.

        Returns:
            str: The audio format in use, as a string (e.g., 'dsAUDIO_FORMAT_DD', 'dsAUDIO_FORMAT_AAC').
        """
        result = self.utMenu.select( self.testSuite, "Get Audio Format")
        audioFormatPattern = r"Result dsGetAudioFormat\(IN:handle:\[.*\], OUT:audioFormat:\[(dsAUDIO_FORMAT_\w+)\]\)"
        audioFormat = self.searchPattern(result, audioFormatPattern)

        return audioFormat

    def getAudioFormatCallbackStatus(self):
        """
        Retrieves the audio format from a system callback.

        Args:
            None.

        Returns:
            None: If the function fails to retrieve the callback status.
            str: The current stream format, as indicated by the system callback.
        """
        result = self.testSession.read_until("Received Format update callback")
        formatCallbackPattern = r"Received Format update callback : (\w+)"

        return self.searchPattern(result, formatCallbackPattern)

    def getConnectedARCType(self):
        """
        Determines the ARC type (ARC, eARC, or NONE) of the connected audio device.

        Args:
            None.

        Returns:
            str: The type of ARC connection. Possible values:
                - "ARC" for standard ARC connection.
                - "eARC" for enhanced ARC connection.
                - "NONE" if no ARC connection is detected.
        """
        result = self.utMenu.select( self.testSuite, "Get ARC Type")
        typeStatusPattern = r"Result dsGetSupportedARCTypes\(IN:handle:\[.*\], OUT:types:\[(dsAUDIOARCSUPPORT_\w+)\]\)"
        type = self.searchPattern(result, typeStatusPattern)

        if "eARC" in type:
            return "eARC"
        elif "ARC" in type :
            return "ARC"
        else:
            return "NONE"

    def boolToString(self, val:bool):
        """
        Converts a boolean value to a string representation used in system menus.

        Args:
            val (bool): A boolean value where True represents 'Enable' and False represents 'Disable'.

        Returns:
            str: '1' for True (Enable) and '2' for False (Disable).
        """
        if(val):
            return "1"
        else:
            return "2"

    def getSupportedPorts(self):
        """
        Retrieves a list of supported audio ports available on the device.

        Args:
            None.

        Returns:
            list: A list of tuples, where each tuple contains:
                - port name (str): The name of the audio port (as per dsAudioPortType).
                - index (int): The index of the port.
            Returns an empty list if no ports are found.
        """
        if not self.ports:
            return []  # Handle empty ports list

        supported_ports = []
        for entry in self.ports.values():
            supported_ports.append((dsAudioPortType(entry['Typeid']).name, entry['Index']))

        return supported_ports

    def getDeviceType(self):
        """
        Retrieves the type of the audio device.

        Args:
            None.

        Returns:
            int: The type of device:
                - 0 for a sink device.
                - 1 for a source device.
                - None if the device type is unknown or unsupported.
        """
        type = self.moduleConfigProfile.fields.get("Type")
        if type == "sink":
            return 0
        elif type == "source":
            return 1
        else:
            return None

    def getMS12DAPFeatureSupport(self, audio_port: str, port_index: int = 0, feature: str = ""):
        """
        Checks whether the specified audio port supports a given MS12 DAP (Dolby Audio Processing) feature.

        Args:
            audio_port (str): The name of the audio port.
            port_index (int, optional): The index of the audio port. Defaults to 0.
            feature (str, optional): The name of the MS12 feature to check.

        Returns:
            bool: True if the feature is supported by the specified port, False otherwise.
        """
        if not self.ports:
            return False

        for entry in self.ports.values():
            if (dsAudioPortType(entry['Typeid']).name == audio_port
                and entry['Index'] == port_index
                and entry["MS12_Capabilities"] & dsMS12Capabilities[feature].value):
                return True

        return False

    def getAudioCompressionSupport(self, audio_port: str, port_index: int = 0):
        """
        Checks whether the specified audio port supports audio compression.

        Args:
            audio_port (str): The name of the audio port.
            port_index (int, optional): The index of the audio port. Defaults to 0.

        Returns:
            bool: True if the port supports audio compression, False otherwise.
        """
        if not self.ports:
            return False

        for entry in self.ports.values():
            if (dsAudioPortType(entry['Typeid']).name == audio_port
                and entry['Index'] == port_index
                and entry["number_of_supported_compressions"] > 0 ):
                return True

        return False

    def getSupportedOutputModes(self, audio_port: str, port_index: int = 0):
        """
        Retrieves the list of supported audio output modes for the specified port, based on `dsAudioStereoMode`.

        Args:
            audio_port (str): The name of the audio port.
            port_index (int, optional): The index of the audio port. Defaults to 0.

        Returns:
            list: A list of supported output modes for the specified audio port. If no ports or modes are found, returns an empty list.
        """
        if not self.ports:
            return []

        output_modes = []
        for entry in self.ports.values():
            if (dsAudioPortType(entry['Typeid']).name == audio_port
                and entry['Index'] == port_index):
                for mode in entry['stereo_modes']:
                    output_modes.append(dsAudioStereoMode(mode).name)
        return output_modes

    def getSupportedMS12Profiles(self, audio_port: str, port_index: int = 0):
        """
        Retrieves the list of supported MS12 audio profiles for the specified port.

        Args:
            audio_port (str): The name of the audio port.
            port_index (int, optional): The index of the audio port. Defaults to 0.

        Returns:
            list: A list of supported MS12 audio profiles. If no profiles are found, returns an empty list.
        """
        if not self.ports:
            return []

        for entry in self.ports.values():
            if (dsAudioPortType(entry['Typeid']).name == audio_port
                and entry['Index'] == port_index):
                return entry['MS12_AudioProfiles']
        return []

    def getDialogueEnhance(self, audio_port: str, port_index: int = 0):
        """
        Retrieves the dialogue enhancement levels supported by a specific audio port.

        This method checks the available audio ports for the specified port type and index,
        and returns the corresponding dialogue enhancement levels if they are defined.

        Args:
            audio_port (str): The type of the audio port to query (e.g., HDMI, Optical).
            port_index (int, optional): The index of the audio port to query. Defaults to 0.

        Returns:
            list: A list of dialogue enhancement levels available for the specified port.
              Returns an empty list if no matching port or enhancement levels are found.
        """
        if not self.ports:
            return []

        for entry in self.ports.values():
            if (dsAudioPortType(entry['Typeid']).name == audio_port
                and entry['Index'] == port_index):
                return entry['dialog_enhancement_level']
        return []

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

    platformProfile = dir_path + "/../../../profiles/sink/Sink_AudioSettings.yaml"
    # test the class assuming that it's optional
    test = dsAudioClass(platformProfile, shell)

    test.initialise()
    ports = test.getSupportedPorts()

    test.enablePort(ports[0][0], ports[0][1])
    test.disablePort(ports[0][0], ports[0][1])

    test.terminate()

    shell.close()
