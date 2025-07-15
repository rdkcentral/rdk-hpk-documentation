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
#include "dsHost.h"


dsError_t __attribute__((weak)) dsHostInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t __attribute__((weak)) dsHostTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t __attribute__((weak)) dsGetCPUTemperature(float* cpuTemperature)
{
  /*TODO: Implement Me!*/
  (void)cpuTemperature;
  return (dsError_t)0;
}

dsError_t __attribute__((weak)) dsGetSocIDFromSDK(char* socID)
{
  /*TODO: Implement Me!*/
  (void)socID;
  return (dsError_t)0;
}

dsError_t __attribute__((weak)) dsGetHostEDID(unsigned char* edid, int* length)
{
  /*TODO: Implement Me!*/
  (void)edid;
  (void)length;
  return (dsError_t)0;
}

