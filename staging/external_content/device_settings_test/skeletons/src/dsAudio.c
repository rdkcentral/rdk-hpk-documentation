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
#include "dsAudio.h"


dsError_t dsAudioPortInit(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsAudioPortTerm(void)
{
  /*TODO: Implement Me!*/
  return (dsError_t)0;
}

dsError_t dsGetAudioPort(dsAudioPortType_t type, int index, intptr_t* handle)
{
  /*TODO: Implement Me!*/
  (void)type;
  (void)index;
  (void)handle;
  return (dsError_t)0;
}

dsError_t dsGetAudioFormat(intptr_t handle, dsAudioFormat_t* audioFormat)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)audioFormat;
  return (dsError_t)0;
}

dsError_t dsGetAudioCompression(intptr_t handle, int* compression)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)compression;
  return (dsError_t)0;
}

dsError_t dsSetAudioCompression(intptr_t handle, int compression)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)compression;
  return (dsError_t)0;
}

dsError_t dsGetDialogEnhancement(intptr_t handle, int* level)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)level;
  return (dsError_t)0;
}

dsError_t dsSetDialogEnhancement(intptr_t handle, int level)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)level;
  return (dsError_t)0;
}

dsError_t dsGetDolbyVolumeMode(intptr_t handle, bool* mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsSetDolbyVolumeMode(intptr_t handle, bool mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsGetIntelligentEqualizerMode(intptr_t handle, int* mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsSetIntelligentEqualizerMode(intptr_t handle, int mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsGetVolumeLeveller(intptr_t handle, dsVolumeLeveller_t* volLeveller)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)volLeveller;
  return (dsError_t)0;
}

dsError_t dsSetVolumeLeveller(intptr_t handle, dsVolumeLeveller_t volLeveller)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)volLeveller;
  return (dsError_t)0;
}

dsError_t dsGetBassEnhancer(intptr_t handle, int* boost)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)boost;
  return (dsError_t)0;
}

dsError_t dsSetBassEnhancer(intptr_t handle, int boost)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)boost;
  return (dsError_t)0;
}

dsError_t dsIsSurroundDecoderEnabled(intptr_t handle, bool* enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsEnableSurroundDecoder(intptr_t handle, bool enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsGetDRCMode(intptr_t handle, int* mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsSetDRCMode(intptr_t handle, int mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsGetSurroundVirtualizer(intptr_t handle, dsSurroundVirtualizer_t* virtualizer)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)virtualizer;
  return (dsError_t)0;
}

dsError_t dsSetSurroundVirtualizer(intptr_t handle, dsSurroundVirtualizer_t virtualizer)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)virtualizer;
  return (dsError_t)0;
}

dsError_t dsGetMISteering(intptr_t handle, bool* enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsSetMISteering(intptr_t handle, bool enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsGetGraphicEqualizerMode(intptr_t handle, int* mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsSetGraphicEqualizerMode(intptr_t handle, int mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsGetMS12AudioProfileList(intptr_t handle, dsMS12AudioProfileList_t* profiles)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)profiles;
  return (dsError_t)0;
}

dsError_t dsGetMS12AudioProfile(intptr_t handle, char* profile)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)profile;
  return (dsError_t)0;
}

dsError_t dsGetSupportedARCTypes(intptr_t handle, int* types)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)types;
  return (dsError_t)0;
}

dsError_t dsAudioSetSAD(intptr_t handle, dsAudioSADList_t sad_list)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)sad_list;
  return (dsError_t)0;
}

dsError_t dsAudioEnableARC(intptr_t handle, dsAudioARCStatus_t arcStatus)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)arcStatus;
  return (dsError_t)0;
}

dsError_t dsGetStereoMode(intptr_t handle, dsAudioStereoMode_t* stereoMode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)stereoMode;
  return (dsError_t)0;
}

dsError_t dsSetStereoMode(intptr_t handle, dsAudioStereoMode_t mode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mode;
  return (dsError_t)0;
}

dsError_t dsGetStereoAuto(intptr_t handle, int* autoMode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)autoMode;
  return (dsError_t)0;
}

dsError_t dsSetStereoAuto(intptr_t handle, int autoMode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)autoMode;
  return (dsError_t)0;
}

dsError_t dsGetAudioGain(intptr_t handle, float* gain)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)gain;
  return (dsError_t)0;
}

dsError_t dsSetAudioGain(intptr_t handle, float gain)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)gain;
  return (dsError_t)0;
}

dsError_t dsGetAudioDB(intptr_t handle, float* db)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)db;
  return (dsError_t)0;
}

dsError_t dsGetAudioLevel(intptr_t handle, float* level)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)level;
  return (dsError_t)0;
}

dsError_t dsSetAudioLevel(intptr_t handle, float level)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)level;
  return (dsError_t)0;
}

dsError_t dsGetAudioDelay(intptr_t handle, uint32_t* audioDelayMs)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)audioDelayMs;
  return (dsError_t)0;
}

dsError_t dsSetAudioDelay(intptr_t handle, const uint32_t audioDelayMs)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)audioDelayMs;
  return (dsError_t)0;
}

dsError_t dsSetAudioAtmosOutputMode(intptr_t handle, bool enable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enable;
  return (dsError_t)0;
}

dsError_t dsGetSinkDeviceAtmosCapability(intptr_t handle, dsATMOSCapability_t* capability)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)capability;
  return (dsError_t)0;
}

dsError_t dsIsAudioMute(intptr_t handle, bool* muted)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)muted;
  return (dsError_t)0;
}

dsError_t dsIsAudioPortEnabled(intptr_t handle, bool* enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsEnableAudioPort(intptr_t handle, bool enabled)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enabled;
  return (dsError_t)0;
}

dsError_t dsEnableMS12Config(intptr_t handle, dsMS12FEATURE_t feature, const bool enable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)feature;
  (void)enable;
  return (dsError_t)0;
}

dsError_t dsEnableLEConfig(intptr_t handle, const bool enable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enable;
  return (dsError_t)0;
}

dsError_t dsGetLEConfig(intptr_t handle, bool* enable)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)enable;
  return (dsError_t)0;
}

dsError_t dsSetMS12AudioProfile(intptr_t handle, const char* profile)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)profile;
  return (dsError_t)0;
}

dsError_t dsSetAudioMute(intptr_t handle, bool mute)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mute;
  return (dsError_t)0;
}

dsError_t dsIsAudioMSDecode(intptr_t handle, bool* HasMS11Decode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)HasMS11Decode;
  return (dsError_t)0;
}

dsError_t dsIsAudioMS12Decode(intptr_t handle, bool* hasMS12Decode)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)hasMS12Decode;
  return (dsError_t)0;
}

dsError_t dsAudioOutIsConnected(intptr_t handle, bool* isConnected)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)isConnected;
  return (dsError_t)0;
}

dsError_t dsAudioOutRegisterConnectCB(dsAudioOutPortConnectCB_t CBFunc)
{
  /*TODO: Implement Me!*/
  (void)CBFunc;
  return (dsError_t)0;
}

dsError_t dsAudioFormatUpdateRegisterCB(dsAudioFormatUpdateCB_t cbFun)
{
  /*TODO: Implement Me!*/
  (void)cbFun;
  return (dsError_t)0;
}

dsError_t dsAudioAtmosCapsChangeRegisterCB(dsAtmosCapsChangeCB_t cbFun)
{
  /*TODO: Implement Me!*/
  (void)cbFun;
  return (dsError_t)0;
}

dsError_t dsGetAudioCapabilities(intptr_t handle, int* capabilities)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)capabilities;
  return (dsError_t)0;
}

dsError_t dsGetMS12Capabilities(intptr_t handle, int* capabilities)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)capabilities;
  return (dsError_t)0;
}

dsError_t dsSetMS12AudioProfileSetttingsOverride(intptr_t handle, const char* profileState, const char* profileName, const char* profileSettingsName, const char* profileSettingValue)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)profileState;
  (void)profileName;
  (void)profileSettingsName;
  (void)profileSettingValue;
  return (dsError_t)0;
}

dsError_t dsSetAssociatedAudioMixing(intptr_t handle, bool mixing)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mixing;
  return (dsError_t)0;
}

dsError_t dsGetAssociatedAudioMixing(intptr_t handle, bool* mixing)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mixing;
  return (dsError_t)0;
}

dsError_t dsSetFaderControl(intptr_t handle, int mixerbalance)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mixerbalance;
  return (dsError_t)0;
}

dsError_t dsGetFaderControl(intptr_t handle, int* mixerbalance)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)mixerbalance;
  return (dsError_t)0;
}

dsError_t dsSetPrimaryLanguage(intptr_t handle, const char* pLang)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)pLang;
  return (dsError_t)0;
}

dsError_t dsGetPrimaryLanguage(intptr_t handle, char* pLang)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)pLang;
  return (dsError_t)0;
}

dsError_t dsSetSecondaryLanguage(intptr_t handle, const char* sLang)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)sLang;
  return (dsError_t)0;
}

dsError_t dsGetSecondaryLanguage(intptr_t handle, char* sLang)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)sLang;
  return (dsError_t)0;
}

dsError_t dsSetAudioMixerLevels(intptr_t handle,dsAudioInput_t aInput, int volume)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)aInput;
  (void)volume;
  return (dsError_t)0;
}
