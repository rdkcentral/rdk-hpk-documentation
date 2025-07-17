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
import struct

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.core.logModule import logModule
from dsClasses.dsDisplay import dsDisplayClass

class dsDisplayHelperClass(utHelperClass):

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            testName (str) : name of the test
            qcId (str): QC ID of the test.
            log (class, optional): Parent log class. Defaults to None.
        """
        self.testName  = ""
        self.testSetupPath = os.path.join(dir_path, "dsDisplay_L3_testSetup.yml")
        self.monitorDeatilsPath = os.path.join(dir_path, "dsDisplay_test_MonitorDetails.yml")
        self.moduleName = "dsDisplay"
        self.rackDevice = "dut"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        # Load test setup configuration
        self.testMonitorConfig = ConfigRead(self.monitorDeatilsPath, "Monitor")
        self.testMonitorNameDetails = self.testMonitorConfig.fields

        #open Hal Session
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")

        deviceTestSetup = self.cpe.get("test")

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))

        self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = os.path.join(self.targetWorkspace, self.moduleName)

    def extractMonitorName(self, edid_data):
        """
        Extracts the monitor name from the given EDID data.

        Args:
            edid_data (bytes): EDID data in binary format (128 bytes).

        Returns:
            str: Monitor name if found, otherwise None.
        """
        # Check that we have a valid EDID length
        if len(edid_data) < 128:
            return None

        # Descriptor blocks start at byte 54
        for i in range(4):
            start = 54 + (i * 18)
            block = edid_data[start:start + 18]

            # Check if the block is a monitor name descriptor
            if block[0:5] == b'\x00\x00\x00\xFC\x00':
                # The monitor name is in bytes 5 to 17
                monitorName = block[5:].decode('ascii', errors='ignore').strip()
                return monitorName

        # Return an empty string if no monitor name descriptor was found
        return None

    def extractEdidPhysicalAddress(self, edid_data):
        """
        Extracts the HDMI physical address from the EDID data.

        Args:
            edid_data (bytes): EDID data in binary format (should be at least 128 bytes).

        Returns:
            str: HDMI physical address in the format 'A.B.C.D', or 'Not found' if not available.
        """
        if len(edid_data) < 128:
            return None

        # Number of extensions, typically in byte 126 of the base block
        num_extensions = edid_data[126]

        # Check if there's at least one extension block
        if num_extensions < 1 or len(edid_data) < 128 * (num_extensions + 1):
            return None

        # Locate the CEA Extension block
        cea_block = edid_data[128:256]  # Typically starts right after base block

        # Confirm this is a CEA extension block (0x02 at the start of the block)
        if cea_block[0] != 0x02:
            return None

        # Parse data blocks in the CEA extension (starting from byte 4)
        index = 4
        while index < len(cea_block):
            # Length of the data block in the lower 5 bits of the first byte
            block_length = cea_block[index] & 0x1F
            tag_code = cea_block[index] >> 5

            # Check for HDMI Vendor-Specific Data Block (VSDB)
            if tag_code == 0x03 and cea_block[index + 1:index + 4] == b'\x03\x0C\x00':
                # Physical Address is at offset 4-5 within this block
                addr1, addr2 = cea_block[index + 4], cea_block[index + 5]
                physical_address = f"{(addr1 >> 4) & 0xF}.{addr1 & 0xF}.{(addr2 >> 4) & 0xF}.{addr2 & 0xF}"
                return physical_address

            # Move to the next block
            index += block_length + 1

        return None

    def parseEdidData(self, edidData):
        """
        Parses the edid buffer and returns the details as dictionary
        Args:
            edidData (list): list containing edid data
        Returns:
            Dictionary
            {
                "manufacturerId": "",
                "productCode": "",
                "serialNumber": "",
                "manufactureWeek": "",
                "manufactureYear": "",
                "version": "",
                "revision": "",
                "videoInputType": "",
                "maxHorizontalSize": "",
                "maxVerticalSize": "",
                "gamma": "",
                "standbyMode": "",
                "suspendMode": "",
                "activeOff": "",
                "monitorName": "",
                "physicalAddress": ""
            }
        """
        edidBytes = bytes(edidData)
        edidInfo = {}

        # EDID is usually 128 bytes long
        if len(edidBytes) < 128:
            raise ValueError("EDID data is too short")

        # Manufacturer ID: bytes 8-9, encoded as 5-bit letters
        edidInfo["manufacturerId"] = ''.join([
            chr(((edidBytes[8] >> 2) & 0x1F) + ord('A') - 1),
            chr((((edidBytes[8] & 0x03) << 3) | ((edidBytes[9] >> 5) & 0x07)) + ord('A') - 1),
            chr((edidBytes[9] & 0x1F) + ord('A') - 1)
        ])

        # Product ID: bytes 10-11, little-endian
        edidInfo["productCode"] = struct.unpack('<H', edidBytes[10:12])[0]

        # Serial Number: bytes 12-15, little-endian
        edidInfo["serialNumber"] = struct.unpack('<I', edidBytes[12:16])[0]

        # Manufacture Week and Year: bytes 16-17
        edidInfo["manufactureWeek"] = edidBytes[16]
        edidInfo["manufactureYear"] = edidBytes[17] + 1990

        # EDID version: bytes 18-19
        edidInfo["version"] = edidBytes[18]
        edidInfo["revision"] = edidBytes[19]

        # Basic Display Parameters: byte 20
        edidInfo["videoInputType"] = "Digital" if (edidBytes[20] & 0x80) else "Analog"
        edidInfo["maxHorizontalSize"] = edidBytes[21]  # in cm
        edidInfo["maxVerticalSize"] = edidBytes[22]    # in cm
        edidInfo["gamma"] = (edidBytes[23] + 100) / 100  # gamma = (value + 100) / 100

        # Supported features: byte 24
        edidInfo["standbyMode"] = bool(edidBytes[24] & 0x80)
        edidInfo["suspendMode"] = bool(edidBytes[24] & 0x40)
        edidInfo["activeOff"] = bool(edidBytes[24] & 0x20)

        # Extract monitor name
        monitorName = self.extractMonitorName(edidBytes)
        if monitorName:
            edidInfo["monitorName"] = monitorName

        physicalAddress = self.extractEdidPhysicalAddress(edidBytes)
        if physicalAddress:
            edidInfo["physicalAddress"] = physicalAddress

        return edidInfo

    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Creates dsDisplayClass

        Returns:
            bool
        """

        # Create the dsDisplay class
        self.testdsDisplay = dsDisplayClass(self.moduleConfigProfileFile, self.hal_session, self.targetWorkspace)

        return True

    def testEndFunction(self, powerOff=True):

        # Clean up the dsDisplay instance
        del self.testdsDisplay
