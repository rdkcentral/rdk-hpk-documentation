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


class dsFPD_test05_SetVerifyFPPattern(dsFPDHelperClass):

    def __init__(self,log:logModule=None):
        """
        Initializes the test05_SetVerifyFPPattern test .

        Args:
            None.
        """
        self.testName  = "test05_SetVerifyFPPattern"
        self.qcID = '5'
        super().__init__(self.testName, self.qcID,log)


    #TODO: Current version supports only manual verification.
    def testVerifyIndicatorPattern(self, pattern, manual=False):
        """
        Verifies whether the indicator pattern is displayed in the device.

        Args:
            pattern (str) :  pattern set in the device.
            manual (bool, optional): Manual verification (True: manual, False: automated).
                                     Defaults to False

        Returns:
            bool : Returns the status of the indicator pattern verification.
        """
        if manual == True:
            return self.testUserResponse.getUserYN(f"Is Front panel shows {pattern}? (Y/N):")
        else :
            #todo: add automation verification methods
            return False

    def testFunction(self):
        """tests the front panel patterns supported by device.

        Returns:
            bool: final result of the test.
        """

        # initialize the dsFPD module
        self.testdsFPD.initialise()

        # Loop through the supported front panel patterns.
        supportedStates = self.testdsFPD.getSupportedFPStates()
        supportedStates = 0x1FE
        for pattern in self.testdsFPD.getSupportedStatesFromConfig():
            if not (supportedStates & (1<<pattern.value)):
                result = False
                self.log.stepResult(result, f'Front Panel Do not support {pattern.name}')
                continue
            self.testdsFPD.setLedStatePattern(pattern.name)
            result = self.testVerifyIndicatorPattern(pattern.name, True)
            self.log.stepResult(result, f'Front Panel Get Pattern Manual Verification for {pattern.name}')
            if result ==True:
                currentPattern = self.testdsFPD.getLedStatePattern()
                verify = False
                if currentPattern == pattern.name:
                    verify = True
                self.log.stepResult(verify, f'Front Panel Get Pattern Verification for {pattern.name}, retrieved Pattern {currentPattern}')
        # Terminate dsFPD Module
        self.testdsFPD.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsFPD_test05_SetVerifyFPPattern(summeryLog)
    test.run(False)
