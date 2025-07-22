/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "dsCompositeIn.h"


dsError_t dsCompositeInInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsCompositeInTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsCompositeInGetNumberOfInputs(uint8_t* pNumberOfInputs)
{
  /*TODO: Implement Me!*/
  (void)pNumberOfInputs;
  return (dsError_t)0;
}

dsError_t dsCompositeInGetStatus(dsCompositeInStatus_t* pStatus)
{
  /*TODO: Implement Me!*/
  (void)pStatus;
  return (dsError_t)0;
}

dsError_t dsCompositeInSelectPort(dsCompositeInPort_t Port)
{
  /*TODO: Implement Me!*/
  (void)Port;
  return (dsError_t)0;
}

dsError_t dsCompositeInScaleVideo(int32_t x, int32_t y, int32_t width, int32_t height)
{
  /*TODO: Implement Me!*/
  (void)x;
  (void)y;
  (void)width;
  (void)height;
  return (dsError_t)0;
}

dsError_t dsCompositeInRegisterConnectCB(dsCompositeInConnectCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsCompositeInRegisterSignalChangeCB(dsCompositeInSignalChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsCompositeInRegisterStatusChangeCB(dsCompositeInStatusChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsCompositeInRegisterVideoModeUpdateCB(dsCompositeInVideoModeUpdateCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

