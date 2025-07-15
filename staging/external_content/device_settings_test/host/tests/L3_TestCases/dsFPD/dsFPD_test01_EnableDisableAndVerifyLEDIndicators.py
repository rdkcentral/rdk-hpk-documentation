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


class dsFPD_test01_EnableDisableAndVerifyLEDIndicators(dsFPDHelperClass):

    def __init__(self,log:logModule=None):
        """
        Initializes the dsFPD_test01_EnableDisableAndVerifyLEDIndicators test .

        Args:
            None.
        """
        self.testName  = "test01_EnableDisableAndVerifyLEDIndicators"
        self.qcID = '1'
        super().__init__(self.testName, self.qcID,log)



    def testFunction(self):
        """tests the front panel indicators by enabling and disabling the indicators.

        Returns:
            bool: final result of the test.
        """

        # initialize the dsFPD module
        self.testdsFPD.initialise()

        # Loop through the supported indicators
        for indicator in self.testdsFPD.getSupportedIndicators():
            # Indicator Enable test
            self.log.stepStart(f'Set {indicator.name} State ON')
            # Enable the indicator
            self.testdsFPD.setState(indicator.name,dsFPDState.dsFPD_STATE_ON.name)
            result = self.testVerifyIndicator(indicator.name,dsFPDState.dsFPD_STATE_ON.name, True)
            self.log.stepResult(result, f'Indicator State Verification {indicator.name} indicator')
            if result == True:
                verify = False
                state = self.testdsFPD.getState(indicator.name)
                if state == dsFPDState.dsFPD_STATE_ON.name:
                    verify = True
                self.log.stepResult(verify, f'Indicator {indicator.name} get  {state} state')

            # Indicator Disable test
            self.log.stepStart(f'Set {indicator.name} state OFF')
            # Disable the indicator
            self.testdsFPD.setState(indicator.name,dsFPDState.dsFPD_STATE_OFF.name)
            result = self.testVerifyIndicator(indicator.name,dsFPDState.dsFPD_STATE_OFF.name,True)
            self.log.stepResult(result, f'Indicator State Verification {indicator.name} indicator')
            if result == True:
                verify = False
                state = self.testdsFPD.getState(indicator.name)
                if state == dsFPDState.dsFPD_STATE_OFF.name:
                    verify = True
                self.log.stepResult(verify, f'Indicator {indicator.name} get  {state} state')

        # Terminate dsFPD Module
        self.testdsFPD.terminate()

        return result

if __name__ == '__main__':
    summerLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summeryLog = logModule(summerLogName, level=logModule.INFO)
    test = dsFPD_test01_EnableDisableAndVerifyLEDIndicators(summeryLog)
    test.run(False)
