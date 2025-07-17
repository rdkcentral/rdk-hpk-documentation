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
#include "dsHdmiIn.h"


dsError_t dsHdmiInInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsHdmiInTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsHdmiInGetNumberOfInputs(uint8_t* pNumberOfinputs)
{
  /*TODO: Implement Me!*/
  (void)pNumberOfinputs;
  return (dsError_t)0;
}

dsError_t dsHdmiInGetStatus(dsHdmiInStatus_t* pStatus)
{
  /*TODO: Implement Me!*/
  (void)pStatus;
  return (dsError_t)0;
}

dsError_t dsHdmiInSelectPort(dsHdmiInPort_t Port, bool audioMix, dsVideoPlaneType_t evideoPlaneType, bool topMost)
{
  /*TODO: Implement Me!*/
  (void)Port;
  return (dsError_t)0;
}

dsError_t dsHdmiInScaleVideo(int32_t x, int32_t y, int32_t width, int32_t height)
{
  /*TODO: Implement Me!*/
  (void)x;
  (void)y;
  (void)width;
  (void)height;
  return (dsError_t)0;
}

dsError_t dsHdmiInSelectZoomMode(dsVideoZoom_t requestedZoomMode)
{
  /*TODO: Implement Me!*/
  (void)requestedZoomMode;
  return (dsError_t)0;
}

dsError_t dsHdmiInGetCurrentVideoMode(dsVideoPortResolution_t* resolution)
{
  /*TODO: Implement Me!*/
  (void)resolution;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterConnectCB(dsHdmiInConnectCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterSignalChangeCB(dsHdmiInSignalChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterStatusChangeCB(dsHdmiInStatusChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterVideoModeUpdateCB(dsHdmiInVideoModeUpdateCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterAllmChangeCB(dsHdmiInAllmChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterAVLatencyChangeCB(dsAVLatencyChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsHdmiInRegisterAviContentTypeChangeCB(dsHdmiInAviContentTypeChangeCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsIsHdmiARCPort(dsHdmiInPort_t iPort, bool* isArcPort)
{
  /*TODO: Implement Me!*/
  (void)iPort;
  (void)isArcPort;
  return (dsError_t)0;
}

dsError_t dsGetEDIDBytesInfo(dsHdmiInPort_t iHdmiPort, unsigned char* edid, int* length)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)edid;
  (void)length;
  return (dsError_t)0;
}

dsError_t dsGetHDMISPDInfo(dsHdmiInPort_t iHdmiPort, unsigned char* data)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)data;
  return (dsError_t)0;
}

dsError_t dsSetEdidVersion(dsHdmiInPort_t iHdmiPort, tv_hdmi_edid_version_t iEdidVersion)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)iEdidVersion;
  return (dsError_t)0;
}

dsError_t dsGetEdidVersion(dsHdmiInPort_t iHdmiPort, tv_hdmi_edid_version_t* iEdidVersion)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)iEdidVersion;
  return (dsError_t)0;
}

dsError_t dsGetAllmStatus(dsHdmiInPort_t iHdmiPort, bool* allmStatus)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)allmStatus;
  return (dsError_t)0;
}

dsError_t dsGetSupportedGameFeaturesList(dsSupportedGameFeatureList_t* features)
{
  /*TODO: Implement Me!*/
  (void)features;
  return (dsError_t)0;
}

dsError_t dsGetAVLatency(int* audio_latency, int* video_latency)
{
  /*TODO: Implement Me!*/
  (void)audio_latency;
  (void)video_latency;
  return (dsError_t)0;
}

dsError_t dsSetEdid2AllmSupport(dsHdmiInPort_t iHdmiPort, bool allmSupport)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)allmSupport;
  return (dsError_t)0;
}

dsError_t dsGetEdid2AllmSupport(dsHdmiInPort_t iHdmiPort, bool *allmSupport)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)allmSupport;
  return (dsError_t)0;
}

dsError_t dsGetHdmiVersion(dsHdmiInPort_t iHdmiPort, dsHdmiMaxCapabilityVersion_t* iVersion)
{
  /*TODO: Implement Me!*/
  (void)iHdmiPort;
  (void)iVersion;
  return (dsError_t)0;
}
