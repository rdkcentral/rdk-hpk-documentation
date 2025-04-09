# Release Information

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Release History](#release-history)
  - [2.0.0](#200)
  - [1.4.5](#145)
  - [1.4.4](#144)
  - [1.4.3](#143)
  - [1.4.2](#142)
  - [1.4.1](#141)
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
- `VTS` - Vendor Layer Test Suit
- `NYA` - Not yet available
- `NA` - Not Applicable
- `AVI`- Auxiliary Video Information InfoFrame
- `ALLM` - Auto Low Latency Mode

## Release History

### 2.0.0

**Release date (YYYY-MM-DD)** : `2025-04-09`  
**Release Version**: `2.0.0`  
**Previous Version**: `1.4.5`  

| #  | Component Name | HAL Interface Version ||              | HAL Testing Suite Version | |            |
| --- | ------------- | ------------          |-|-------------| ------------------------- |-| ----------- |
|     |                | **Current**          |**ChangeInfo**| **Previous**| **Current**|**ChangeInfo**   | **Previous**|
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` || [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/tree/1.0.4) |[`1.4.1`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.4.1)| [1.3.2...1.4.1](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/compare/1.3.2...1.4.1) |[1.3.2](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.3.2) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/tree/1.0.3) | [`1.5.2`](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.5.2)| [1.4.0...1.5.2](https://github.com/rdkcentral/rdk-halif-test-power_manager/compare/1.4.0...1.5.2)|[1.4.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.4.0) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`5.0.0`](https://github.com/rdkcentral/rdk-halif-device_settings/tree/5.0.0)|[4.1.3...5.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/compare/4.1.3...5.0.0) |[4.1.3](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.3) |[`5.0.1`](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/5.0.1)| [4.0.0...5.0.1](https://github.com/rdkcentral/rdk-halif-test-device_settings/compare/4.0.0...5.0.1)| [4.0.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/4.0.0) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | `No change` | |[1.3.11](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.3.11) |[`1.5.1`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.5.1)|[1.4.1...1.5.1](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/compare/1.4.1...1.5.1)|[1.4.1](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.4.1) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | |[1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/tree/1.0.5) | [`1.5.2`](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.5.2)|[1.4.0...1.5.2](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/compare/1.4.0...1.5.1) |[1.4.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.4.0) |
| 6   |  [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |[`2.1.0`](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.1.0) ||[2.2.1](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.2.1) |[`2.1.3`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.3)||[2.2.1](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.2.1) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change`| | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | [`1.0.1`](https://github.com/rdkcentral/rdk-halif-libdrm/tree/1.0.1) |[1.0.0...1.0.1](https://github.com/rdkcentral/rdk-halif-libdrm/compare/1.0.0...1.0.1)| [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | ||
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | ||
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | [`1.0.1`](https://github.com/rdkcentral/rdk-halif-v4l2/tree/1.0.1) |[1.0.0...1.0.1](https://github.com/rdkcentral/rdk-halif-v4l2/compare/1.0.0...1.0.1) |[1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | ||

**Summary of current release:**

HAL IF summary:

- Enhanced HAL header documentation and `AVI` feature support has been introduced in the dsDisplay module for source devices.
- Removed `dsGetEdid2AllmSupport` API from dsHDMIIn.
- TVSettings interfaces have been reverted to the previous version due to compatibility issues with some deprecated ODM API interfaces.

VTS Summary:

- Automation of `L1` and `L2`.
- Binary nomenclatures for each module are now well defined with proper names to provide a scope to share the binary files as well.
- `VTS` enhancements implemented to validate the latest `HAL` interface updates.
- NULL checks were handled in few modules to avoid crashes.

Limitation:

- VTS Level 3 test for ALLM mode is not included in this release and will be provided in a future update.

### 1.4.5

**Release date (YYYY-MM-DD)** : `2025-02-17`  
**Release Version**: `1.4.5`  
**Previous Version**: `1.4.4`  

| #  | Component Name | HAL Interface Version ||              | HAL Testing Suite Version | |            |
| --- | ------------- | ------------          |-|-------------| ------------------------- |-| ----------- |
|     |                | **Current**          |**ChangeInfo**| **Previous**| **Current**|**ChangeInfo**   | **Previous**|
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` || [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) |[`1.3.2`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.3.2)| [1.3.0...1.3.2](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/compare/1.3.0...1.3.2) |[1.3.0](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.3.0) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | `No change`||[1.4.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.4.0) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`4.1.3`](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.3)|[4.1.2...4.1.3](https://github.com/rdkcentral/rdk-halif-device_settings/compare/4.1.2...4.1.3) |[4.1.2](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.2) |[`4.0.0`](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/4.0.0)| [3.5.0...4.0.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/compare/3.5.0...4.0.0)| [3.5.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/3.5.0) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | [`1.3.11`](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.3.11) | [1.3.10...1.3.11](https://github.com/rdkcentral/rdk-halif-hdmi_cec/compare/1.3.10...1.3.11)|[1.3.10](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.3.10) |[`1.4.1`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.4.1)|[1.4.0...1.4.1](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/compare/1.4.0...1.4.1)|[1.4.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.4.0) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | |[1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | `No change`| |[1.4.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.4.0) |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |[`2.2.1`](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.2.1)|[2.1.0...2.2.1](https://github.com/rdkcentral/rdkv-halif-tvsettings/compare/2.1.0...2.2.1)|[2.1.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.1.0) |[`2.2.1`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.2.1)|[2.1.3...2.2.1](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/compare/2.1.3...2.2.1)|[2.1.3](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.3) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change`| | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` || [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | ||
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | ||
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | |[1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | ||

**Summary of current release:**

- Enhanced HAL header documentation and added new APIs.
- Validated updates with the latest interface descriptions and `VTS` bug fixes.
- Currently, there are limitations when setting and retrieving ALLM mode on source devices, but these will be resolved in the upcoming version.

### 1.4.4

**Release date (YYYY-MM-DD)** : `2025-01-16`  
**Release Version**: `1.4.4`  
**Previous Version**: `1.4.3`  

| #  | Component Name | HAL Interface Version ||              | HAL Testing Suite Version | |            |
| --- | -------------- | -----------          |-|--------------| ------------------------- |-| ----------- |
|     |                | **Current**          |**ChangeInfo**| **Previous**| **Current**|**ChangeInfo**   | **Previous**|
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` || [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) |`No change`| |[1.3.0](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.3.0) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | `No change`||[1.4.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.4.0) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`4.1.2`](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.2)|[4.1.1...4.1.2](https://github.com/rdkcentral/rdk-halif-device_settings/compare/4.1.1...4.1.2) |[4.1.1](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.1) |`No change`| | [3.5.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/3.5.0) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | [`1.3.10`](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.3.10) | [1.3.9...1.3.10](https://github.com/rdkcentral/rdk-halif-hdmi_cec/compare/1.3.9...1.3.10)|[1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.3.9) |`No change`||[1.4.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.4.0) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | |[1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | `No change`| |[1.4.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.4.0) |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |`No change` ||[2.1.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.1.0) |`No change`||[2.1.3](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.3) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change`| | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` || [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | ||
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | ||
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | |[1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | ||

**Summary of current release:**

- Enhanced the `HAL` header documentation to provide clearer information about HdmiCecTx and result details.
- Added support for up to 5 `HDMI` inputs in the HDMIIn `DS` module.
- The `HDMI` `CEC` enhanced test case will be updated in the next HPK release.

### 1.4.3

**Release date (YYYY-MM-DD)** : `2025-01-02`  
**Release Version**: `1.4.3`  
**Previous Version**: `1.4.2`  

| #  | Component Name | HAL Interface Version ||              | HAL Testing Suite Version | |            |
| --- | -------------- | -----------          |-|--------------| ------------------------- |-| ----------- |
|     |                | **Current**          |**ChangeInfo**| **Previous**| **Current**|**ChangeInfo**   | **Previous**|
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` || [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) |`No change`| |[1.3.0](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.3.0) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | `No change`||[1.4.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.4.0) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`4.1.1`](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.1)|[4.1.0...4.1.1](https://github.com/rdkcentral/rdk-halif-device_settings/compare/4.1.0...4.1.1) |[4.1.0](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.0) |[`3.5.0`](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/3.5.0)|[3.4.0...3.5.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/compare/3.4.0...3.5.0) | [3.4.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/3.4.0) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | `No change` | |[1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/milestone/1) |[`1.4.0`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.4.0)|[1.3.3...1.4.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/compare/1.3.3...1.4.0)|[1.3.3](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.3.3) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | |[1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | `No change`| |[1.4.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.4.0) |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |[`2.1.0`](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.1.0) |[2.0.0...2.1.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/compare/2.0.0...2.1.0)|[2.0.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/tree/2.0.0) |[`2.1.3`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.3)|[2.1.2...2.1.3](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/compare/2.1.2...2.1.3)|[2.1.2](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.2) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change`| | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` || [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | ||
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | ||
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | |[1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | ||

**Summary of current release:**

- Enhanced `HAL` header documentation to clarify the dialog enhancement range in the audio `DS` module.
- `VTS` `L3` test cases have been added for the `HDMI` `CEC` module.
- `VTS` `L1` , `L2` & `L3` bug fixes.

### 1.4.2

**Release date (YYYY-MM-DD)** : `2024-12-02`  
**Release Version**: `1.4.2`  
**Previous Version**: `1.4.1`  

| #  | Component Name | HAL Interface Version ||              | HAL Testing Suite Version | |            |
| --- | -------------- | -----------          |-|--------------| ------------------------- |-| ----------- |
|     |                | **Current**          |**ChangeInfo**| **Previous**| **Current**|**ChangeInfo**   | **Previous**|
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` || [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) |[`1.3.0`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.3.0)|[1.2.3...1.3.0](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/compare/1.2.3...1.3.0) |[1.2.3](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.2.3) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` || [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | [`1.4.0`](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.4.0)|[1.3.1...1.4.0](https://github.com/rdkcentral/rdk-halif-test-power_manager/compare/1.3.1...1.4.0)|[1.3.1](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.3.1) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`4.1.0`](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.1.0)|[4.0.1...4.1.0](https://github.com/rdkcentral/rdk-halif-device_settings/compare/4.0.1...4.1.0) |[4.0.1](https://github.com/rdkcentral/rdk-halif-device_settings/tree/4.0.1) |[`3.4.0`](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/3.4.0)|[3.3.0...3.4.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/compare/3.3.0...3.4.0) | [3.3.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/3.3.0) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | `No change` | |[1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/milestone/1) |`No change`||[1.3.3](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/18) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | |[1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | [`1.4.0`](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.4.0) |[1.3.1...1.4.0](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/compare/1.3.1...1.4.0)|[1.3.1](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.3.1) |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |`No change` ||[2.0.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/milestone/3) |[`2.1.2`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.2)|[2.1.0...2.1.2](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/compare/2.1.0...2.1.2)|[2.1.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.1.0) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change`| | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` || [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | ||
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change`| | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | ||
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | |[1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | ||

**Summary of current release:**

- Include the callback function into the DS Composite Interface Header to handle and trigger composite video mode update events.
- `VTS` `L3` Test case are available for RMF Audio Capture, DeepSleep & power manager.
- Enhance Test suites for Device Settings and TV Settings `VTS` `L3` Modules
- `VTS` `L1` & `L2` bug fixes

### 1.4.1

**Release date (YYYY-MM-DD)** : `2024-11-08`  
**Release Version**: `1.4.1`  
**Previous Version**: `1.4.0`  

| #  | Component Name | HAL Interface Version | | HAL Testing Suite Version | |
| --- | -------------- | --------------------- | ---------------- | ---------------------- | -------------------- |
|     |                | **Current**           | **Previous**     | **Current**            | **Previous**         |
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` | [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) |`No change`| [1.2.3](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/5) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | `No change`|[1.3.1](https://github.com/rdkcentral/rdk-halif-test-power_manager/milestone/4) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`4.0.1`](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/7)| [4.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/5) |[`3.3.0`](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/15)| [3.2.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/7) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec) | `No change` | [1.3.9](https://github.com/rdkcentral/rdk-halif-hdmi_cec/milestone/1) |[`1.3.3`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/18)|[1.3.1](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/milestone/17) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture) | `No change` | [1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/milestone/1) | `No change` |[`1.3.1`](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/milestone/3) |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |`No change` |[2.0.0](https://github.com/rdkcentral/rdkv-halif-tvsettings/milestone/3) |[`2.1.0`](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/9)| [2.0.0](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/milestone/8) |
| 7   | [`RDK-V` WiFi](https://github.com/rdkcentral/rdkv-halif-wifi) [^1] | `No change` | [2.0.0](https://github.com/rdkcentral/rdkv-halif-wifi/blob/2.0.0) | `No change` | [1.0.0](https://github.com/rdkcentral/rdkv-halif-test-wifi/blob/1.0.0) |
| 8   | [LibDRM](https://github.com/rdkcentral/rdk-halif-libdrm) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-libdrm/blob/1.0.0) | `NYA` | |
| 9  | [AvSync](https://github.com/rdkcentral/rdk-halif-avsync) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-avsync/blob/1.0.0) | `NYA` | |
| 10  | [V4L2](https://github.com/rdkcentral/rdk-halif-v4l2) | `No change` | [1.0.0](https://github.com/rdkcentral/rdk-halif-v4l2/blob/1.0.0) | `NYA` | |

**Summary of current release:**

- `VTS` `L3` test suites are available for all modules within device settings and TV settings.
- `L1` & `L2` bug fixes

**limitations:**

- The L3 test suites for the TVSettings module currently support only the IP video source and are implemented with GStreamer for playback. Other video sources and playback methods are not covered in this version of the test suite.

### 1.4.0

**Release date (YYYY-MM-DD)** : `2024-10-16`  
**Release Version**: `1.4.0`  
**Previous Version**: `1.3.0`  

| #  | Component Name | HAL Interface Version | | HAL Testing Suite Version | |
| --- | -------------- | --------------------- | ---------------- | ---------------------- | -------------------- |
|     |                | **Current**           | **Previous**     | **Current**            | **Previous**         |
| 1   | [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager) | `No change` | [1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/milestone/1) | [`1.2.3`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/5) | [1.2.2](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/milestone/3) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager) | `No change` | [1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/milestone/1) | `No change`|[1.3.1](https://github.com/rdkcentral/rdk-halif-test-power_manager/milestone/4) |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) | [`4.0.0`](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/5) | [3.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/milestone/1) | [`3.2.0`](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/7) |[3.1.2](https://github.com/rdkcentral/rdk-halif-test-device_settings/milestone/4) |
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