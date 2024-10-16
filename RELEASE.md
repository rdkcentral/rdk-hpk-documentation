# Release Information

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Release History](#release-history)
  - [1.4.0](#140)
  - [1.3.0](#130)
  - [1.2.1](#121)
  - [1.2.0](#120)
  - [1.1.0](#110)
  - [1.0.0](#100)

## Acronyms, Terms and Abbreviations

- `L1`  - Functional Tests
- `L2`  - Module functional Testing
- `L3`  - Module testing with External Stimulus is required to validate and control device
- `HAL` - Hardware Abstraction Layer
- `RDK-V` - Reference Design Kit - Video
- `HDMI` - High-Definition Multimedia Interface
- `CEC` - Consumer Electronics Control
- `RMF` - RDK Media Framework
- `DRM` - Direct Rendering Manager
- `NYA` - Not yet available
- `NA` - Not Applicable

## Release History

### 1.4.0

**Release date (YYYY-MM-DD)** : `2024-10-16`  
**Release Version**: `1.4.0`  
**Previous Version**: `1.3.0`  

| #  | Component Name | HAL Interface Version | | HAL Testing Suite Version | |
| --- | -------------- | --------------------- | ---------------- | ---------------------- | -------------------- |
|     |                | **Current**           | **Previous**     | **Current**            | **Previous**         |
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` | [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) | [`1.2.3`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/5) | [1.2.2](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/3) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | `No change`|[1.3.1](https://github.com/rdkcentral/rdk-halif-test-power_manager/milestone/4) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) | [`4.0.0`](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/5) | [3.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/1) | [`3.2.0`](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/13) |[3.1.2](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/4) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | `No change` | [1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/milestone/1) |[`1.3.1`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/17) | [1.3.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/6) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | [1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | `No change` |[`1.3.1`](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/milestone/3) |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) | [`2.0.0`](https://github.com/rdkcentral/rdkv-halif-tvsettings/milestone/3) | [1.3.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/milestone/1) | [`2.0.0`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/8) |[1.3.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/4) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change` | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change` | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | |
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | |
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | |

**Summary of current release:**

- `L3` test suites are available for dsAudio Port and dsVideo Port.
- `L1` & `L2` bug fixes

### 1.3.0

**Release date (YYYY-MM-DD)** : `2024-08-15`  
**Release Version**: `1.3.0`  
**Previous Version**: `1.2.1`  

| #  | Component Name | HAL Interface Version | | HAL Testing Suite Version | |
| --- | -------------- | --------------------- | ---------------- | ---------------------- | -------------------- |
|     |                | **Current**           | **Previous**     | **Current**            | **Previous**         |
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` | [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) | [`1.2.2`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/3) | [1.2.0](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/4) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | [`1.3.1`](https://github.com/rdkcentral/rdk-halif-test-power_manager/milestone/4) | [1.2.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/milestone/3) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) | `No change` | [3.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/1) | [`3.1.2`](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/4) | [3.0.1](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/5) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | `No change` | [1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/milestone/1) | [`1.3.0`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/6) | [1.2.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/9) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | [1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | [`1.3.1`](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/milestone/3) | [1.2.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/milestone/4) |
| 6   | [Closed Captions](https://github.com/rdkcentral/rdk-halif-closed_captions) | `No change` | [2.0.0](https://github.com/rdkcentral/rdk-halif-closed_captions/blob/2.0.0) | `NYA` | |
| 7   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) | `No change` | [1.3.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/milestone/1) | [`1.3.0`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/4) | [1.2.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/5) |
| 8   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change` | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change` | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 9   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | |
| 10  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | |
| 11  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | |

**Summary of current release:**

- Updated `L1` test suites with profile configurations.

### 1.2.1

**Release date (YYYY-MM-DD)** : `2024-07-15`  
**Release Version**: `1.2.1`  
**Previous Version**: `1.2.0`  

| # | Component Name | HAL Interface Version || HAL Testing suite Version| |
| --- | -------------- | --------------|--------------------- | -------------------------|-------------------- |
|     |     |**Current**|**Previous**|**Current**|**Previous**|
|1|[Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager)|`No change`|[1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1)|`No change`|[1.2.0](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/4)|
|2|[Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager)|[1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1)|`1.0.3`|`No change`|[1.2.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/milestone/3)|
|3|[Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/)|`No change`|[3.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/1)|`No change`|[3.0.1](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/5)|
|4|[HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec)|`No change`|[1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/milestone/1)|`No change`|[1.2.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/9)|
|5|[RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture)| [1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1)| [1.0.4](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/1.0.4) |[1.2.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/milestone/4)|[1.1.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.1.0)|
|6|[Closed Captions](https://github.com/rdkcentral/rdk-halif-closed_captions)| [2.0.0](https://github.com/rdkcentral/rdk-halif-closed_captions/blob/2.0.0)|`NA` |`NYA`||
|7|[`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings)|[1.3.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/milestone/1)| [1.2.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/1.2.0) |[1.2.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/5)|[1.0.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/blob/1.0.0)|
|8|[`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi)[^1]|`No change`|[2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0)|`No change`|[1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0)|
|9|[LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm)|`No change`|[1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0)|`NYA`||
|10|[AvSync](https://github.com/rdkcentral/rdk-halif-avsync)|`No change`|[1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0)|`NYA`||
|11|[V4L2](https://github.com/rdkcentral/rdk-halif-v4l2)|`No change`|[1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0)|`NYA`||

**Summary of the release:**

- Improved `HAL` header documentation with additional details for source and sink devices.
- Deprecated & Removed the `API`s from `HAL` Header.
- Added High level test specification & `L2` low level specification documentation.
- Introduced `L2` test suites with profile configurations.

### 1.2.0

**Release date (YYYY-MM-DD)** : `2024-06-28`  
**Release Version**: `1.2.0`  
**Previous Version**: `1.1.0`  

|#|Component Name|HAL Interface Version| HAL Testing suite  Version|
|---|--------------|-----------------|-------------------------|
|1|[Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager)|[`1.0.4`](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/blob/1.0.4)|[`1.2.0`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/blob/1.2.0)|
|2|[Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager)|[`1.0.3`](https://github.com/rdkcentral/rdk-halif-power_manager/blob/1.0.3/)|[`1.2.0`](https://github.com/rdkcentral/rdk-halif-test-power_manager/blob/1.2.0)|
|3|[Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/)|[`3.0.0`](https://github.com/rdkcentral/rdk-halif-device_settings/blob/3.0.0)|[`3.0.1`](https://github.com/rdkcentral/rdk-halif-test-device_settings/blob/3.0.1)|
|4|[HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec)|[`1.3.9`](https://github.com/rdkcentral/rdk-halif-hdmi_cec/blob/1.3.9)|[`1.2.0`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/blob/1.2.0)|
|5|[RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture)| [1.0.4](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/1.0.4)|[`1.1.0`](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/1.1.0)|
|6|[`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi)[^1]|[2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0)|[1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0)|
|7|[`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings)|[`1.2.0`](https://github.com/rdkcentral/rdkv-halif-tvsettings/blob/1.2.0)|[1.0.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/blob/1.0.0)|
|8|[LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm)|[1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0)|`NYA`|
|9|[AvSync](https://github.com/rdkcentral/rdk-halif-avsync)|[1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0)|`NYA`|
|10|[v4l2](https://github.com/rdkcentral/rdk-halif-v4l2)|[`1.0.0`](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0)|`NYA`|

### 1.1.0

**Release date (YYYY-MM-DD)** : `2024-02-20`  
**Release Version**: `1.1.0`  
**Previous Version**: `1.0.0`  

|SNo|Component Name|HAL Interface Version|HAL Testing suite  Version|
|---|--------------|-----------------|-------------------------|
|1|[Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager)|[1.0.3](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/blob/1.0.3)|[1.0.4](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/blob/1.0.4)|
|2|[Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager)|[1.0.2](https://github.com/rdkcentral/rdk-halif-power_manager/blob/1.0.2/)|[1.0.4](https://github.com/rdkcentral/rdk-halif-test-power_manager/blob/1.0.4)|
|3|[Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/)|[1.0.8](https://github.com/rdkcentral/rdk-halif-device_settings/blob/1.0.8)|[1.0.3](https://github.com/rdkcentral/rdk-halif-test-device_settings/blob/1.0.3)|
|4|[HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec)|[1.3.7](https://github.com/rdkcentral/rdk-halif-hdmi_cec/blob/1.3.7)|[1.0.3](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/blob/1.0.3)|
|5|[RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture)| [1.0.4](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/1.0.4)|[1.0.4](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/1.0.4)|
|6|[`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi)[^1]|[2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0)|[1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0)|
|7|[`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings)|[1.1.7](https://github.com/rdkcentral/rdkv-halif-tvsettings/blob/1.1.7)|[1.0.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/blob/1.0.0)|
|8|[LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm)|[1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0)|`NYA`|
|9|[AvSync](https://github.com/rdkcentral/rdk-halif-avsync)|[1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0)|`NYA`|

### 1.0.0

**Release date (YYYY-MM-DD)** : `2023-12-12`  
**Release Version**: `1.0.0`  

|SNo|Component Name|HAL Interface Version|HAL Testing suite  Version|
|---|--------------|-----------------|-------------------------|
|1|[Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager)|[1.0.3](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/blob/1.0.3)|[1.0.2](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/blob/1.0.2)|
|2|[Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager)|[1.0.2](https://github.com/rdkcentral/rdk-halif-power_manager/blob/1.0.2/)|[1.0.2](https://github.com/rdkcentral/rdk-halif-test-power_manager/blob/1.0.2)|
|3|[Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/)|[1.0.8](https://github.com/rdkcentral/rdk-halif-device_settings/blob/1.0.8)|[1.0.3](https://github.com/rdkcentral/rdk-halif-test-device_settings/blob/1.0.3)|
|4|[HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec)|[1.3.7](https://github.com/rdkcentral/rdk-halif-hdmi_cec/blob/1.3.7)|[1.0.3](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/blob/1.0.3)|
|5|[RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture)| [1.0.4](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/1.0.4)|[1.0.2](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/1.0.2)|
|6|[`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi)[^1]|[2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0)|`NYA`|
|7|[LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm)|[1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0)|`NYA`|
|8|[AvSync](https://github.com/rdkcentral/rdk-halif-avsync)|[1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0)|`NYA`|

[^1]: WiFi HAL must be used only if the platform does not support Wi-Fi Supplicant.
