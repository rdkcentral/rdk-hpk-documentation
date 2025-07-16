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
#include "dsDisplay.h"


dsError_t dsDisplayInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsGetDisplay(dsVideoPortType_t vType, int index, intptr_t* handle)
{
  /*TODO: Implement Me!*/
  (void)vType;
  (void)index;
  (void)handle;
  return (dsError_t)0;
}

dsError_t dsGetEDID(intptr_t handle, dsDisplayEDID_t* edid)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)edid;
  return (dsError_t)0;
}

dsError_t dsGetEDIDBytes(intptr_t handle, unsigned char* edid, int* length)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)edid;
  (void)length;
  return (dsError_t)0;
}

dsError_t dsGetDisplayAspectRatio(intptr_t handle, dsVideoAspectRatio_t* aspectRatio)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)aspectRatio;
  return (dsError_t)0;
}

dsError_t dsDisplayTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsRegisterDisplayEventCallback(intptr_t handle, dsDisplayEventCallback_t cb)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)cb;
  return (dsError_t)0;
}

