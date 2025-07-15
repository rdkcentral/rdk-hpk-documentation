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


class dsFPD_test03_SetVerifyLEDIndicatorsBlink(dsFPDHelperClass):

    def __init__(self,log:logModule=None):
        """
        Initializes the test03_SetVerifyLEDIndicatorsBlink test .

        Args:
            None.
        """
        self.testName  = "test03_SetVerifyLEDIndicatorsBlink"
        self.duration = 1000
        self.iteration = 5
        self.qcID = '3'
        super().__init__(self.testName, self.qcID,log)


    #TODO: Current version supports only manual verification.
    def testVerifyIndicatorBlink(self, indicator, manual=False):
        """
        Verifies whether the indicaotr is blinkig as per the values provided.

        Args:
            indicator (str) : indicator to verify
            manual (bool, optional): Manual verification (True: manual, False: automated).
                                     Defaults to False

        Returns:
            bool : Returns the status of the indicator blink verification.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is {indicator} blinking {self.iteration} times with {self.duration/1000} sec ? (Y/N):")
        else :
            #todo: add automation verification methods
            return False
    def testFunction(self):
        """tests the indicator blink functionality.

        Returns:
            bool: final result of the test.
        """

        # initialize the dsFPD module
        self.testdsFPD.initialise()

        # Loop through the supported Indicators
        for indicator in self.testdsFPD.getSupportedIndicators():
            self.log.stepStart(f'Set {indicator.name} State ON')
            # Enable the indicator
            self.testdsFPD.setState(indicator.name,dsFPDState.dsFPD_STATE_ON.name)
            result = self.testVerifyIndicator(indicator.name,dsFPDState.dsFPD_STATE_ON.name, True)
            self.log.stepResult(result, f'Indicator State Verification {indicator.name} indicator')
            # blink the indicator in specified rate.
            self.testdsFPD.blinkIndicator(indicator.name,self.duration,self.iteration)
            result = self.testVerifyIndicatorBlink(indicator.name,True)
            self.log.stepResult(result, f'Indicator State Verification {indicator.name} indicator')

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
    test = dsFPD_test03_SetVerifyLEDIndicatorsBlink(summeryLog)
    test.run(False)
