/**
*  If not stated otherwise in this file or this component's LICENSE
*  file the following copyright and licenses apply:
*
*  Copyright 2022 RDK Management
*
*  Licensed under the Apache License, Version 2.0 (the License);
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an AS IS BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "deepSleepMgr.h"


DeepSleep_Return_Status_t PLAT_DS_INIT(void)
{
  /*TODO: Implement Me!*/
  return (DeepSleep_Return_Status_t)0;
}

DeepSleep_Return_Status_t PLAT_DS_SetDeepSleep(uint32_t deep_sleep_timeout, bool* isGPIOWakeup, bool networkStandby)
{
  /*TODO: Implement Me!*/
  (void)deep_sleep_timeout;
  (void)isGPIOWakeup;
  (void)networkStandby;
  return (DeepSleep_Return_Status_t)0;
}

DeepSleep_Return_Status_t PLAT_DS_DeepSleepWakeup(void)
{
  /*TODO: Implement Me!*/
  return (DeepSleep_Return_Status_t)0;
}

DeepSleep_Return_Status_t PLAT_DS_GetLastWakeupReason(DeepSleep_WakeupReason_t* wakeupReason)
{
  /*TODO: Implement Me!*/
  (void)wakeupReason;
  return (DeepSleep_Return_Status_t)0;
}

DeepSleep_Return_Status_t PLAT_DS_GetLastWakeupKeyCode(DeepSleepMgr_WakeupKeyCode_Param_t* wakeupKeyCode)
{
  /*TODO: Implement Me!*/
  (void)wakeupKeyCode;
  return (DeepSleep_Return_Status_t)0;
}

DeepSleep_Return_Status_t PLAT_DS_TERM(void)
{
  /*TODO: Implement Me!*/
  return (DeepSleep_Return_Status_t)0;
}

