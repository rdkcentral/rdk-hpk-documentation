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
#include "plat_power.h"


pmStatus_t PLAT_INIT(void)
{
  /*TODO: Implement Me!*/
  return (pmStatus_t)0;
}

pmStatus_t PLAT_API_SetPowerState(PWRMgr_PowerState_t newState)
{
  /*TODO: Implement Me!*/
  (void)newState;
  return (pmStatus_t)0;
}

pmStatus_t PLAT_API_GetPowerState(PWRMgr_PowerState_t* curState)
{
  /*TODO: Implement Me!*/
  (void)curState;
  return (pmStatus_t)0;
}

pmStatus_t PLAT_API_SetWakeupSrc(PWRMGR_WakeupSrcType_t srcType, bool enable)
{
  /*TODO: Implement Me!*/
  (void)srcType;
  (void)enable;
  return (pmStatus_t)0;
}

pmStatus_t PLAT_API_GetWakeupSrc(PWRMGR_WakeupSrcType_t srcType, bool* enable)
{
  /*TODO: Implement Me!*/
  (void)srcType;
  (void)enable;
  return (pmStatus_t)0;
}

/* PLAT_Reset() is deprecated */
pmStatus_t PLAT_Reset(PWRMgr_PowerState_t newState)
{
  /*TODO: Implement Me!*/
  (void)newState;
  return (pmStatus_t)0;
}

pmStatus_t PLAT_TERM(void)
{
  /*TODO: Implement Me!*/
  return (pmStatus_t)0;
}

