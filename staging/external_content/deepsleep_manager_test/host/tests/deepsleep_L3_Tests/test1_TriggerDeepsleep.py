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
import socket
import subprocess

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))
sys.path.append(os.path.join(dir_path, "../"))

from deepsleepClasses.deepsleep import deepsleepClass
from deepsleepClasses.power import powerManagerClass
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.core.logModule import logModule

class deepsleep_test1_TriggerDeepsleep(utHelperClass):

    def __init__(self, log:logModule=None):
        """
        Initializes the test1_TriggerDeepsleep test .

        Args:
            None.
        """
        self.testName  = "test1_TriggerDeepsleep"
        self.testSetupPathPower = dir_path + "/power_L3_testSetup.yml"
        self.testSetupPathDeepsleep = dir_path + "/deepsleep_L3_testSetup.yml"
        self.moduleNameDeepsleep = "deepsleep"
        self.moduleNamePower = "power"
        self.rackDevice = "dut"
        self.qcID = '1'
        self.testWakeupSources = ["TIMER", "POWER_KEY", "IR", "CEC", "LAN", "WIFI"]
        self.testWakeupReason = ["TIMER", "FRONT_PANEL", "IR", "CEC", "LAN", "WLAN"]
        self.testTimer = 60 #one minute

        super().__init__(self.testName, self.qcID, log)

        # Test Setup configuration file
        self.testSetupPower = ConfigRead(self.testSetupPathPower, self.moduleNamePower)
        self.testSetupDeepsleep = ConfigRead(self.testSetupPathDeepsleep, self.moduleNameDeepsleep)

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Open Session for hal test
        self.hal_session_deepsleep = self.dut.getConsoleSession("ssh_hal_deepsleep_test")
        self.hal_session_power = self.dut.getConsoleSession("ssh_hal_power_test")

        # Get path to device profile file
        deviceTestSetup = self.cpe.get("test")
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))
        self.targetWorkspaceDeepsleep = self.cpe.get("target_directory")
        self.targetWorkspaceDeepsleep = os.path.join(self.targetWorkspaceDeepsleep, self.moduleNameDeepsleep)

        self.targetWorkspacePower = self.cpe.get("target_directory")
        self.targetWorkspacePower = os.path.join(self.targetWorkspacePower, self.moduleNamePower)

    def isDeviceAwake(self, maxAttempts=3, delay=5):
        """
        Check if the device is awake by executing a simple command via SSH.

        Args:
            max_attempts (int): Maximum number of attempts to check if the device is awake.
            delay (int): Delay (in seconds) between attempts.
        Return:
            True if the device is awake, False otherwise.
        """
        for attempt in range(maxAttempts):
            # Test the SSH connection by running a simple command
            self.session.write("echo 'Device is awake'")
            result = self.session.read_until("Device is awake")

            if "Device is awake" in result:
                return True

            # Wait before the next attempt
            time.sleep(delay)

        return False

    def getDeviceNetworkMACDetails(self):
        """
        Function to get the device network interface details.

        Args:
            None
        Return:
            dict - dictionary containing network interface details
        """
        # Execute the command to list network interfaces and IP addresses
        self.session.write("ip addr show")

        # Fetch the output and errors
        result = self.session.read_until("ip")

        # Parse the output for interface names, IPs, and MAC addresses
        interfaces = {}
        current_interface = None

        for line in result.splitlines():
            if line.startswith(" "):  # Details of the current interface
                if "link/ether" in line:  # MAC address
                    parts = line.split()
                    mac_address = parts[1]
                    if current_interface:
                        interfaces[current_interface]["MAC"] = mac_address
                elif "inet" in line:  # IPv4 address
                    parts = line.split()
                    ip_address = parts[1].split('/')[0]
                    if current_interface:
                        interfaces[current_interface]["IPs"].append(ip_address)
            else:  # Interface line
                parts = line.split(": ")
                if len(parts) > 1:
                    current_interface = parts[1].split("@")[0]
                    interfaces[current_interface] = {"MAC": None, "IPs": []}

        return interfaces

    def wolwowlSendMagicPacket(self, mac_address):
        """
        This function sends a Wake-on-LAN magic packet to a device with the given MAC address

        Args:
            mac_address (str): MAC address of the device.
        Return:
            None
        """
        # Format MAC address (remove colons or dashes)
        mac_address = mac_address.replace(":", "").replace("-", "")

        if len(mac_address) != 12:
            self.log.fatal("Invalid mac address")

        # Create the magic packet (6 x FF followed by 16 x MAC address)
        magic_packet = bytes.fromhex("FF" * 6 + mac_address * 16)

        # Send the packet to the broadcast address
        broadcast_address = ("255.255.255.255", 9)
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            sock.sendto(magic_packet, broadcast_address)

    def testVerifyWakeOnTimer(self, timeOut:int):
        """
        Function to verify whether the device woke up after a timeout.

        Args:
            timeOut (int) : Time out value in seconds.
        """
        # Wait till the time out
        time.sleep(timeOut)
        return self.isDeviceAwake()

    def testVerifyWakeOnPowerkey(self, manual=False):
        """
        Function to verify whether the device woke up after pressing the PowerKey.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via PowerKey and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False

        return self.isDeviceAwake()

    def testVerifyWakeOnIR(self, manual=False):
        """
        Function to verify whether the device woke up after receiving an IR signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via IR and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False

        return self.isDeviceAwake()

    def testVerifyWakeOnCEC(self, manual=False):
        """
        Function to verify whether the device woke up after receiving a CEC signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via CEC and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False

        return self.isDeviceAwake()

    def testVerifyWakeOnLAN(self, mac:str, manual=False):
        """
        Function to verify whether the device woke up after a LAN event.

        Args:
            mac (str) : MAC address of the device
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via LAN and Press Enter:")
        else:
            # Wait for the device to go to deep sleep
            time.sleep(30)

            self.log.step(f"Send magic packet to the device to wakeup")

            self.wolwowlSendMagicPacket(mac)

            # Wait for the device to go to wakeup
            time.sleep(5)

        return self.isDeviceAwake()

    def testVerifyWakeOnWIFI(self, mac:str, manual=False):
        """
        Function to verify whether the device woke up after a Wi-Fi event.

        Args:
            mac (str) : MAC address of the device
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via WIFI and Press Enter:")
        else:
            # Wait for the device to go to deep sleep
            time.sleep(30)

            self.log.step(f"Send magic packet to the device to wakeup")

            self.wolwowlSendMagicPacket(mac)

            # Wait for the device to go to wakeup
            time.sleep(5)

        return self.isDeviceAwake()

    def testPrepareFunction(self):
        """
        This function creates classes for powermanager and deepsleepmanager

        Args:
            None
        Returns:
            bool : Test results
        """
        # Create the deepsleep manager class
        self.testDeepsleep = deepsleepClass(self.moduleConfigProfileFile, self.hal_session_deepsleep, self.targetWorkspaceDeepsleep)
        # Create the Power Manager Class
        self.testPower = powerManagerClass(self.moduleConfigProfileFile, self.hal_session_power, self.targetWorkspacePower)

        return True

    def testFunction(self):
        """This function will test the device deepsleep functionality

        Args:
            None
        Returns:
            bool : Test results
        """
        interfaces = self.getDeviceNetworkMACDetails()

        # Initialize the deepsleep manager and power manager
        self.testPower.initialise()
        self.testDeepsleep.initialise()

        self.supportedWakeupsources = self.testPower.getSupportedWakeupSources()

        result = False
        finalResult = True

        for source, wakeReason in zip(self.testWakeupSources, self.testWakeupReason):
            result = False
            testSource = None

            for supported in self.supportedWakeupsources:
                if source in supported:
                    testSource = supported
                    break

            if not testSource:
                self.log.warn(f"Device doesn't support {source} Wakeup")
                continue

            self.log.stepStart(f"Triggering wake up test for {source}")

            self.log.step(f"Setting wakeup source to {source}")
            self.testPower.setWakeupSource(testSource, 1)

            self.log.step("Triggering deepsleep")
            if "TIMER" in source:
                self.testDeepsleep.triggerDeepsleep(self.testTimer, 1)
            else:
                self.testDeepsleep.triggerDeepsleep(0, 1)

            if "VOICE" in source:
                # Wake on Voice
                # Not supported in this version
                pass
            elif "PRESENCE_DETECTION" in source:
                # Wake on Presence detection
                # Not supported in this version
                pass
            elif "BLUETOOTH" in source:
                # Wake on bluetooth
                # Not supported in this version
                pass
            elif "WIFI" in source:
                # Wake on Wifi
                wlan0Interface = interfaces.get("wlan0")
                if wlan0Interface:
                    wlan0MAC = wlan0Interface.get("MAC")
                    if wlan0MAC:
                        result = self.testVerifyWakeOnWIFI(wlan0MAC)
            elif "IR" in source:
                # Wake on IR
                result = self.testVerifyWakeOnIR(True)
            elif "POWER_KEY" in source:
                # Wake on Power key
                # Send power key
                result = self.testVerifyWakeOnPowerkey(True)
            elif "TIMER" in source:
                # Wake on Timer
                result = self.testVerifyWakeOnTimer(self.testTimer)
            elif "CEC" in source:
                # Wake on CEC
                result = self.testVerifyWakeOnCEC(True)
            elif "LAN" in source:
                 # Wake on LAN
                eth0Interface = interfaces.get("eth0")
                if eth0Interface:
                    eth0MAC = eth0Interface.get("MAC")
                    if eth0MAC:
                        result = self.testVerifyWakeOnLAN(eth0MAC)

            self.log.stepResult(result, f"Triggering wake up test for {source}")

            self.log.stepStart(f"Check wakeup reason for {wakeReason}")
            lastWakeupReason = self.testDeepsleep.getLastWakeupReason()
            self.log.stepResult(wakeReason in lastWakeupReason, f"Check wakeup reason for {wakeReason}")

            self.testPower.setWakeupSource(testSource, 0)
            finalResult &= result

        return finalResult

    def testEndFunction(self, powerOff=True):
        """This function deletes the test classes

        Args:
            powerOff (bool) : Flag to power off the device
        Returns:
            bool : Test results
        """
        # Delete the deepsleep and power classes
        del self.testDeepsleep
        del self.testPower

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = deepsleep_test1_TriggerDeepsleep(summeryLog)
    test.run(False)
