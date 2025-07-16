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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "rmfAudioCapture.h"

RMF_AudioCapture_Settings primary;
RMF_AudioCapture_Settings auxiliary;

static const size_t DEFAULT_FIFO_SIZE = 64 * 1024;
static const size_t DEFAULT_THRESHOLD = 8 * 1024;
#define DATA_RATE 192000    // Bytes per second = sampling rate x num channels x bytes per second = 48000 * 2 * 2
int exitFlag_primary = 0;
int exitFlag_auxiliary = 0;

rmf_Error RMF_AudioCapture_Open_Type(RMF_AudioCaptureHandle* handle, RMF_AudioCaptureType rmfAcType)
{
  rmf_Error result = RMF_SUCCESS;
  if(NULL != handle)
  {
    if(0 == strncmp(RMF_AC_TYPE_PRIMARY, rmfAcType, strlen(RMF_AC_TYPE_PRIMARY)))
    {
        *handle = (void *)&primary;
    }
    else if(0 == strncmp(RMF_AC_TYPE_AUXILIARY, rmfAcType, strlen(RMF_AC_TYPE_AUXILIARY)))
    {
        *handle = (void *)&auxiliary;
    }
    else
      result = RMF_INVALID_PARM;
  }
  else
    result = RMF_INVALID_PARM;
  return result;
}

rmf_Error RMF_AudioCapture_Open(RMF_AudioCaptureHandle* handle)
{
  rmf_Error result = RMF_SUCCESS;
  if(NULL != handle)
  {
    *handle = (void *)&primary;
  }
  else
    result = RMF_INVALID_PARM;
  return result;
}

rmf_Error RMF_AudioCapture_GetStatus(RMF_AudioCaptureHandle handle, RMF_AudioCapture_Status* status)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)status;
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_GetDefaultSettings(RMF_AudioCapture_Settings* settings)
{
  settings->format = racFormat_e16BitStereo;
  settings->samplingFreq = racFreq_e48000;
  settings->fifoSize = DEFAULT_FIFO_SIZE;
  settings->threshold = DEFAULT_THRESHOLD;
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_GetCurrentSettings(RMF_AudioCaptureHandle handle, RMF_AudioCapture_Settings* settings)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)settings;
  return (rmf_Error)0;
}

/* Function reads raw audio data from the input wav file */
size_t readRawAudio(const char *filename, char **buffer) {
    size_t headerSize = 0;
    uint32_t dataSize = 0;
    char wavHeader[44] = {0};
    size_t bytesRead = 0;

    FILE *file = fopen(filename, "rb");
    if (!file) 
    {
        printf("%s,  %d : Failed to open input wav file", __FILE__, __LINE__);
        return 0;
    }

    headerSize = fread(wavHeader, sizeof(char), 44, file);

    if (headerSize < 44) 
    {
        printf ("%s,  %d : Could not read the complete WAV header", __FILE__, __LINE__);
        fclose(file);
        return 0;
    }

    // Check for a valid WAV file
    if (wavHeader[0] != 'R' || wavHeader[1] != 'I' || 
        wavHeader[2] != 'F' || wavHeader[3] != 'F') 
    {
        printf("%s,  %d : Not a valid input WAV file", __FILE__, __LINE__);
        fclose(file);
        return 0;
    }
    // Get the size of the audio data
    dataSize = *((uint32_t *)(wavHeader + 40)); // Offset for data size in WAV header

    *buffer = (char *)malloc(dataSize);
    if (*buffer == NULL) 
    {
        printf("%s,  %d : Failed to allocate memory to read from wav file", __FILE__, __LINE__);
        fclose(file);
        return 0;
    }

    bytesRead = fread(*buffer, 1, dataSize, file);
    fclose(file);

    return bytesRead;
}

/* Function that will run in thread and send raw audio data in required datarate  */
void* sendAudioData(void* handle) 
{
    char *rawDataBuffer;
    size_t dataSize;
    size_t offset = 0;
    int *exitFlag = NULL;
    char *filePath = NULL;
    unsigned int sleepTimeMicroseconds = 0;
    size_t chunkSize = 0;

    if(&primary == (RMF_AudioCapture_Settings *)handle) 
    {
        filePath = getenv("INPUT_PRIMARY");
        exitFlag = &exitFlag_primary;
    } else 
    {
        filePath = getenv("INPUT_AUXILIARY");
        exitFlag = &exitFlag_auxiliary;
    }

    if (filePath == NULL) 
    {
        printf("%s,  %d : Environment variable for file path not set, Set INPUT_PRIMARY and/or INPUT_AUXILIARY as required.", __FILE__, __LINE__);
        printf("%s,  %d : Not setting environment variable for input files when running with mock might result in incorrect test results !", __FILE__, __LINE__);
        return NULL;
    }
    
    if (access(filePath, F_OK) != 0 ) 
    {
        printf("%s,  %d : File does not exist", __FILE__, __LINE__);
        return NULL;
    }
    
    char buffer[DEFAULT_THRESHOLD];
    memset(buffer, 0, DEFAULT_THRESHOLD);

    // Calculate the sleep time to achieve the desired data rate
    sleepTimeMicroseconds = (DEFAULT_THRESHOLD * 1000000) / DATA_RATE;

    // Read raw audio data from wav file into a buffer
    dataSize = readRawAudio(filePath, &rawDataBuffer);
    if (dataSize == 0) 
    {
        printf("%s,  %d : Failed to read audio data or file is empty", __FILE__, __LINE__);
        free(rawDataBuffer);
        return NULL;
    }

    while (*exitFlag == 0) 
    {
        if (offset >= dataSize) 
        {
            // Restart from the beginning of the data if we have reached the end
            offset = 0;
        }

        // Calculate the size of the chunk to send
        chunkSize = (dataSize - offset >= DEFAULT_THRESHOLD) ? DEFAULT_THRESHOLD : (dataSize - offset);

        // Copy data into buffer
        memcpy(buffer, rawDataBuffer + offset, chunkSize);

        // Call buffer ready on right handle
        if(&primary == (RMF_AudioCapture_Settings *)handle) 
        {
            primary.cbBufferReady(primary.cbBufferReadyParm, (void *)buffer, sizeof(buffer));
        } else 
        {
    	    auxiliary.cbBufferReady(auxiliary.cbBufferReadyParm, (void *)buffer, sizeof(buffer));
        }

        // Simulate sending data in required data rate by sleeping
        usleep(sleepTimeMicroseconds);

        // Move to the next chunk
        offset += chunkSize;
    }
    free(rawDataBuffer);
    pthread_exit(NULL);
}

rmf_Error RMF_AudioCapture_Start(RMF_AudioCaptureHandle handle, RMF_AudioCapture_Settings* settings)
{
  pthread_t thread;
  rmf_Error result = RMF_SUCCESS;

  if(&primary == (RMF_AudioCapture_Settings *)handle)
  {
    primary = *settings;
    if(primary.cbBufferReady)
    {
        exitFlag_primary = 0;
    }
    else
      result = RMF_INVALID_PARM;
  }
  else if(&auxiliary == (RMF_AudioCapture_Settings *)handle)
  {
    auxiliary = *settings;
    if(auxiliary.cbBufferReady)
    {
        exitFlag_auxiliary = 0;
    }
    else
      result = RMF_INVALID_PARM;
  }
  else
    result = RMF_INVALID_HANDLE;

  if(RMF_SUCCESS == result)
  {
      // Create the thread to simulate sending audio data
      if (pthread_create(&thread, NULL, sendAudioData, (void *)handle) != 0) 
      {
          printf("%s,  %d : Failed to create thread to send audio data", __FILE__, __LINE__);
          result = RMF_INVALID_PARM;
      } else 
      {
          pthread_detach(thread);
      }
  }
  return result;
}

rmf_Error RMF_AudioCapture_Stop(RMF_AudioCaptureHandle handle)
{
  /*TODO: Implement Me!*/
  (void)handle;
  if(&primary == (RMF_AudioCapture_Settings *)handle) 
  {
      exitFlag_primary = 1;
  } else 
  {
      exitFlag_auxiliary = 1;
  }
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_Close(RMF_AudioCaptureHandle handle)
{
  rmf_Error result = RMF_SUCCESS;
  if((&primary == (RMF_AudioCapture_Settings *)handle) || (&auxiliary == (RMF_AudioCapture_Settings *)handle))
  {
    RMF_AudioCapture_Settings * ctx = (RMF_AudioCapture_Settings *)handle;
    ctx->cbBufferReady = NULL;
    ctx->cbBufferReadyParm = NULL;
    ctx->cbStatusChange = NULL;
  }
  else
    result = RMF_INVALID_HANDLE;
  return result;
}

