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
#include "dsVideoPort.h"


dsError_t dsVideoPortInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsVideoPortTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsGetVideoPort(dsVideoPortType_t type, int index, intptr_t *handle)
{
  /*TODO: Implement Me!*/
  (void)type;
  (void)index;
  (void)handle;
  return (dsError_t)0;
}

dsError_t dsIsVideoPortEnabled(intptr_t handle, bool* enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsIsDisplayConnected(intptr_t handle, bool* connected)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)connected;
  return (dsError_t)0;
}

dsError_t dsIsDisplaySurround(intptr_t handle, bool* surround)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)surround;
  return (dsError_t)0;
}

dsError_t dsGetSurroundMode(intptr_t handle, int* surround)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)surround;
  return (dsError_t)0;
}

dsError_t dsVideoFormatUpdateRegisterCB(dsVideoFormatUpdateCB_t cb)
{
  /*TODO: Implement Me!*/
  (void)cb;
  return (dsError_t)0;
}

dsError_t dsIsVideoPortActive(intptr_t handle, bool* active)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)active;
  return (dsError_t)0;
}

dsError_t dsEnableHDCP(intptr_t handle, bool contentProtect, char* hdcpKey, size_t keySize)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)contentProtect;
  (void)hdcpKey;
  (void)keySize;
  return (dsError_t)0;
}

dsError_t dsIsHDCPEnabled(intptr_t handle, bool* pContentProtected)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)pContentProtected;
  return (dsError_t)0;
}

dsError_t dsEnableVideoPort(intptr_t handle, bool enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsSetResolution(intptr_t handle, dsVideoPortResolution_t* resolution)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)resolution;
  return (dsError_t)0;
}

dsError_t dsGetResolution(intptr_t handle, dsVideoPortResolution_t* resolution)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)resolution;
  return (dsError_t)0;
}

dsError_t dsRegisterHdcpStatusCallback(intptr_t handle, dsHDCPStatusCallback_t cb)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)cb;
  return (dsError_t)0;
}

dsError_t dsGetHDCPStatus(intptr_t handle, dsHdcpStatus_t* status)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)status;
  return (dsError_t)0;
}

dsError_t dsGetHDCPProtocol(intptr_t handle, dsHdcpProtocolVersion_t* protocolVersion)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)protocolVersion;
  return (dsError_t)0;
}

dsError_t dsGetHDCPReceiverProtocol(intptr_t handle, dsHdcpProtocolVersion_t* protocolVersion)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)protocolVersion;
  return (dsError_t)0;
}

dsError_t dsGetHDCPCurrentProtocol(intptr_t handle, dsHdcpProtocolVersion_t* protocolVersion)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)protocolVersion;
  return (dsError_t)0;
}

dsError_t dsGetTVHDRCapabilities(intptr_t handle, int* capabilities)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)capabilities;
  return (dsError_t)0;
}

dsError_t dsSupportedTvResolutions(intptr_t handle, int* resolutions)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)resolutions;
  return (dsError_t)0;
}

dsError_t dsSetForceDisable4KSupport(intptr_t handle, bool disable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)disable;
  return (dsError_t)0;
}

dsError_t dsGetForceDisable4KSupport(intptr_t handle, bool* disable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)disable;
  return (dsError_t)0;
}

dsError_t dsGetVideoEOTF(intptr_t handle, dsHDRStandard_t* video_eotf)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)video_eotf;
  return (dsError_t)0;
}

dsError_t dsGetMatrixCoefficients(intptr_t handle, dsDisplayMatrixCoefficients_t* matrix_coefficients)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)matrix_coefficients;
  return (dsError_t)0;
}

dsError_t dsGetColorDepth(intptr_t handle, unsigned int* color_depth)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)color_depth;
  return (dsError_t)0;
}

dsError_t dsGetColorSpace(intptr_t handle, dsDisplayColorSpace_t* color_space)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)color_space;
  return (dsError_t)0;
}

dsError_t dsGetQuantizationRange(intptr_t handle, dsDisplayQuantizationRange_t* quantization_range)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)quantization_range;
  return (dsError_t)0;
}

dsError_t dsGetCurrentOutputSettings(intptr_t handle, dsHDRStandard_t* video_eotf, dsDisplayMatrixCoefficients_t* matrix_coefficients, dsDisplayColorSpace_t* color_space, unsigned int* color_depth, dsDisplayQuantizationRange_t* quantization_range)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)video_eotf;
  (void)matrix_coefficients;
  (void)color_space;
  (void)color_depth;
  (void)quantization_range;
  return (dsError_t)0;
}

dsError_t dsIsOutputHDR(intptr_t handle, bool* hdr)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)hdr;
  return (dsError_t)0;
}

dsError_t dsResetOutputToSDR(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsSetHdmiPreference(intptr_t handle, dsHdcpProtocolVersion_t* hdcpCurrentProtocol)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)hdcpCurrentProtocol;
  return (dsError_t)0;
}

dsError_t dsGetHdmiPreference(intptr_t handle, dsHdcpProtocolVersion_t* hdcpCurrentProtocol)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)hdcpCurrentProtocol;
  return (dsError_t)0;
}

dsError_t dsGetIgnoreEDIDStatus(intptr_t handle, bool* status)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)status;
  return (dsError_t)0;
}

dsError_t dsSetBackgroundColor(intptr_t handle, dsVideoBackgroundColor_t color)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)color;
  return (dsError_t)0;
}

dsError_t dsSetForceHDRMode(intptr_t handle, dsHDRStandard_t mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsColorDepthCapabilities(intptr_t handle, unsigned int* colorDepthCapability)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)colorDepthCapability;
  return (dsError_t)0;
}

dsError_t dsGetPreferredColorDepth(intptr_t handle, dsDisplayColorDepth_t* colorDepth)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)colorDepth;
  return (dsError_t)0;
}

dsError_t dsSetPreferredColorDepth(intptr_t handle, dsDisplayColorDepth_t colorDepth)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)colorDepth;
  return (dsError_t)0;
}

