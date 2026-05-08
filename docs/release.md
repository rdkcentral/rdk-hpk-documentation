## 3.1.0

**Release date (YYYY-MM-DD)** : `2026-01-12`  
**Release Version**: `3.1.0`  
**Previous Version**: `3.0.0`  

| #  | Component Name | HAL Interface Version ||              | HAL Testing Suite Version | |            |
| --- | ------------- | ------------          |-|-------------| ------------------------- |-| ----------- |
|     |                | **Current**          |**ChangeInfo**| **Previous**| **Current**|**ChangeInfo**   | **Previous**|
| 1   |  [Deep Sleep Manager](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/) |[`1.0.5`](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/tree/1.0.5)|[1.0.4...1.0.5](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/compare/1.0.4...1.0.5)|[1.0.4](https://github.com/rdkcentral/rdk-halif-deepsleep_manager/tree/1.0.4) |[`1.4.3`](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.4.3)| [1.4.2...1.4.3](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/compare/1.4.2...1.4.3) |[1.4.2](https://github.com/rdkcentral/rdk-halif-test-deepsleep_manager/tree/1.4.2) |
| 2   | [Power Manager](https://github.com/rdkcentral/rdk-halif-power_manager/) |[`1.0.4`](https://github.com/rdkcentral/rdk-halif-power_manager/tree/1.0.4)|[1.0.3...1.0.4](https://github.com/rdkcentral/rdk-halif-power_manager/compare/1.0.3...1.0.4)|[1.0.3](https://github.com/rdkcentral/rdk-halif-power_manager/tree/1.0.3)| [`1.5.4`](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.5.4)| [1.5.3...1.5.4](https://github.com/rdkcentral/rdk-halif-test-power_manager/compare/1.5.3...1.5.4)|[1.5.3](https://github.com/rdkcentral/rdk-halif-test-power_manager/tree/1.5.3) | |
| 3   | [Device Settings](https://github.com/rdkcentral/rdk-halif-device_settings/) |[`6.0.1`](https://github.com/rdkcentral/rdk-halif-device_settings/tree/6.0.1)|[6.0.0...6.0.1](https://github.com/rdkcentral/rdk-halif-device_settings/compare/6.0.0...6.0.1)|[6.0.0](https://github.com/rdkcentral/rdk-halif-device_settings/tree/6.0.0) |[`6.0.1`](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/6.0.1)| [6.0.0...6.0.1](https://github.com/rdkcentral/rdk-halif-test-device_settings/compare/6.0.0...6.0.1)| [6.0.0](https://github.com/rdkcentral/rdk-halif-test-device_settings/tree/6.0.0) |
| 4   | [HDMI CEC](https://github.com/rdkcentral/rdk-halif-hdmi_cec/) |[`1.4.0`](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.4.0)|[1.3.11...1.4.0](https://github.com/rdkcentral/rdk-halif-hdmi_cec/compare/1.3.11...1.4.0)|[1.3.11](https://github.com/rdkcentral/rdk-halif-hdmi_cec/tree/1.3.11) | [`1.6.1`](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.6.1)|[1.6.0...1.6.1](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/compare/1.6.0...1.6.1)|[1.6.0](https://github.com/rdkcentral/rdk-halif-test-hdmi_cec/tree/1.6.0) |
| 5   | [RMF Audio Capture](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/) |[`1.0.6`](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/tree/1.0.6)|[1.0.5...1.0.6](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/compare/1.0.5...1.0.6)|[1.0.5](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/tree/1.0.5) | `No change` | |[1.5.4](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/tree/1.5.4)  |
| 6   | [`RDK-V` TVSettings](https://github.com/rdkcentral/rdkv-halif-tvsettings) |`No change` || [2.4.0](https://github.com/rdkcentral//rdkv-halif-tvsettings/tree/2.4.0) |`No change` | |[2.3.2](https://github.com/rdkcentral/rdkv-halif-test-tvsettings/tree/2.3.2) |
| 7   | [Initial Setup](https://github.com/rdkcentral/rdk-halif-test-initial-setup) |  `NA` ||NA | [1.0.1](https://github.com/rdkcentral/rdk-halif-test-initial-setup/tree/1.0.1) | |`New Repo`|


**Summary of Current Release**
---

**L1 to L3:**

- **Interface Improvements**: Interface update additions in HDMI-CEC and documentation updates on Deepsleep, Powermanager and RMF Audio Capture.
- **HPK HFP Documentation** : Documentation added for Hardware Feature Profile (HFP) configuration files that expose audio and video decoder capabilities in the rdk-hpk-documentation repo.
- **HFP Validation Script**: Validation script is added that enables OEMs and vendors to validate their platform-specific HFP (Hardware Feature Profile) YAML files against official schemas from any HPK release version.
- **VTS Issue Resolutions**: Fixed test suite issues affecting the DeviceSettings.
- **VTS Python Improvements** : VTS Python updates with bug fixes and variable alignment in DeepSleep and Power Manager modules.
- **Documentation Updates**: Updated HAL documentation to align with the RDK 7 release.
- **New Repository Added**: Introduced a new repository, **rdk-halif-test-initial-setup**, dedicated to hosting tests that validate the initial setup environment after a vendor build is flashed.
---