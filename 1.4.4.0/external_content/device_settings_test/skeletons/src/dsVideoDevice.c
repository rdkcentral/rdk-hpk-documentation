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
#include "dsVideoDevice.h"


dsError_t dsVideoDeviceInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsGetVideoDevice(int index, intptr_t *handle)
{
  /*TODO: Implement Me!*/
  (void)index;
  (void)handle;
  return (dsError_t)0;
}

dsError_t dsSetDFC(intptr_t handle, dsVideoZoom_t dfc)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)dfc;
  return (dsError_t)0;
}

dsError_t dsGetDFC(intptr_t handle, dsVideoZoom_t* dfc)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)dfc;
  return (dsError_t)0;
}

dsError_t dsVideoDeviceTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsGetHDRCapabilities(intptr_t handle, int* capabilities)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)capabilities;
  return (dsError_t)0;
}

dsError_t dsGetSupportedVideoCodingFormats(intptr_t handle, unsigned int* supported_formats)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)supported_formats;
  return (dsError_t)0;
}

dsError_t dsGetVideoCodecInfo(intptr_t handle, dsVideoCodingFormat_t codec, dsVideoCodecInfo_t* info)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)codec;
  (void)info;
  return (dsError_t)0;
}

dsError_t dsForceDisableHDRSupport(intptr_t handle, bool disable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)disable;
  return (dsError_t)0;
}

dsError_t dsSetFRFMode(intptr_t handle, int frfmode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)frfmode;
  return (dsError_t)0;
}

dsError_t dsGetFRFMode(intptr_t handle, int* frfmode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)frfmode;
  return (dsError_t)0;
}

dsError_t dsGetCurrentDisplayframerate(intptr_t handle, char* framerate)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)framerate;
  return (dsError_t)0;
}

dsError_t dsSetDisplayframerate(intptr_t handle, char* framerate)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)framerate;
  return (dsError_t)0;
}

typedef void (*dsRegisterFrameratePreChangeCB_t)(unsigned int tSecond);
dsError_t dsRegisterFrameratePreChangeCB(dsRegisterFrameratePreChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

typedef void (*dsRegisterFrameratePostChangeCB_t)(unsigned int tSecond);
dsError_t dsRegisterFrameratePostChangeCB(dsRegisterFrameratePostChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

