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

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../../"))

from L3_TestCases.dsFPD.dsFPDHelperClass import dsFPDHelperClass
from raft.framework.core.logModule import logModule
from dsClasses.dsFPD import dsFPDState


class dsFPD_test02_SetVerifyLEDIndicatorsBrightness(dsFPDHelperClass):

    def __init__(self,log:logModule=None):
        """
        Initializes the test02_SetVerifyLEDIndicatorsBrightness test .

        Args:
            None.
        """
        self.testName  = "test02_SetVerifyLEDIndicatorsBrightness"
        self.qcID = '2'
        super().__init__(self.testName, self.qcID,log)


    #TODO: Current version supports only manual verification.
    def testVerifyIndicatorBrightness(self, indicator, brightness, manual=False):
        """
        Verifies whether the indicator is showing in teh specified brightness.

        Args:
            indicator (str) : front panel indicator
            state (str) : state of the indicator
            manual (bool, optional): Manual verification (True: manual, False: automated).
                                     Defaults to False

        Returns:
            bool : Returns the status of the indicator brightness.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is {indicator} brightness {brightness}%? (Y/N):")
        else :
            #todo: add automation verification methods
            return False
    def testFunction(self):
        """tests the brightness of the LED indicator.

        Returns:
            bool: final result of the test.
        """

        # initialize the dsFPD module
        self.testdsFPD.initialise()

        # Loop through the supported frontpanel indicators
        for indicator in self.testdsFPD.getSupportedIndicators():
            # Enable indicator test
            self.log.stepStart(f'Set {indicator.name} State ON')
            # Enable the indicator
            self.testdsFPD.setState(indicator.name,dsFPDState.dsFPD_STATE_ON.name)
            result = self.testVerifyIndicator(indicator.name,dsFPDState.dsFPD_STATE_ON.name, True)
            self.log.stepResult(result, f'Indicator State Verification {indicator.name} indicator')
            # Read the brightness
            minBrightness = self.testdsFPD.getMinBrightnessValue(indicator.value)
            maxBrightness = self.testdsFPD.getMaxBrightnessValue(indicator.value)
            avgBrightness = (minBrightness+maxBrightness)/2
            #set the brightness
            self.testdsFPD.setBrightness(indicator.name,minBrightness)
            result = self.testVerifyIndicatorBrightness(indicator.name,minBrightness, True)
            self.log.stepResult(result, f'Indicator Brightness Verification {indicator.name} indicator')
            if result ==True:
                brightness = self.testdsFPD.getBrightness(indicator.name)
                verify = False
                if int(brightness) == minBrightness:
                    verify = True 
                self.log.stepResult(verify, f'Indicator Get Brightness Verification {indicator} indicator {brightness}%')

            self.testdsFPD.setBrightness(indicator.name,avgBrightness)
            result = self.testVerifyIndicatorBrightness(indicator.name,avgBrightness, True)
            self.log.stepResult(result, f'Indicator Brightness Verification {indicator} indicator')
            if result ==True:
                brightness = self.testdsFPD.getBrightness(indicator.name)
                verify = False
                if int(brightness) == avgBrightness:
                    verify = True 
                self.log.stepResult(verify, f'Indicator Get Brightness Verification {indicator} indicator {brightness}%')

            self.testdsFPD.setBrightness(indicator.name,maxBrightness)
            result = self.testVerifyIndicatorBrightness(indicator.name,maxBrightness, True)
            self.log.stepResult(result, f'Indicator Brightness Verification {indicator} indicator')
            if result == True:
                brightness = self.testdsFPD.getBrightness(indicator.name)
                verify = False
                if int(brightness) == maxBrightness:
                    verify = True
                self.log.stepResult(verify, f'Indicator Get Brightness Verification {indicator} indicator {brightness}%')
            # Indicator Disable test
            self.log.stepStart(f'Set {indicator.name} state OFF')
            # Disable the Indicator
            self.testdsFPD.setState(indicator.name,dsFPDState.dsFPD_STATE_OFF.name)
            result = self.testVerifyIndicator(indicator.name,dsFPDState.dsFPD_STATE_OFF.name,True)
            self.log.stepResult(result, f'Indicator State Verification {indicator.name} indicator')

        # Terminate dsFPD Module
        self.testdsFPD.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsFPD_test02_SetVerifyLEDIndicatorsBrightness(summeryLog)
    test.run(False)
