# Vendor Layer HFP Requirements

## Overview

This document defines the requirements for Hardware Feature Profile (HFP) configuration file deployment and management in the RDK Hardware Porting Kit (HPK) vendor layer. The vendor layer must support System-on-Chip (SoC) implementations from multiple hardware partners while maintaining consistent HFP file structure, location, and validation across all platforms.

## Purpose

HFP configuration files enable the vendor layer to declare hardware decoder capabilities to the RDK middleware stack. These files serve as the authoritative source for:
- Supported audio and video codecs and their profiles
- Maximum bitrates for both audio and video
- Sample rates, bit depth and maxChannels for audio when profile is empty
- Licensed capabilities vs. hardware capabilities
- Platform-specific decoder constraints

Vendors implementing HAL layers for different SoC partners (e.g., Broadcom, Realtek, Amlogic, MediaTek) must ensure HFP files accurately reflect each platform's licensed and hardware capabilities while adhering to standardized deployment locations and schemas.

## Requirements

### Functional Requirements

| ID | Actor | Description | Acceptance Criteria | Verification Method | Priority |
|----|-------|-------------|---------------------|---------------------|----------|
| FR-001 | Vendor Layer | Deploy HFP configuration files at standardized location | Given the vendor layer is deployed on the platform, When the HFP files are accessed at `/product/hfp/config/`, Then both `hfp-videodecoder.yaml` and `hfp-audiodecoder.yaml` exist and are readable, And file names and paths are identical across all platforms | Manual verification, Automated deployment test | Must |
| FR-002 | Vendor Layer | Validate HFP files against official schema | Given HFP configuration files exist for the platform, When the files are validated using the validation script, Then both files pass schema validation without errors, And all required fields are present and correctly formatted, And the `schemaVersion` matches the validation schema version | Automated schema validation test | Must |
| FR-003 | Vendor Layer | Reflect licensed capabilities in HFP configuration | Given the platform has licensed capabilities that differ from hardware capabilities, When the HFP configuration files are generated, Then all capability values reflect licensed limits not hardware maximums, And restricted features are limited to licensed values | Manual review, Compliance check | Must |
| FR-004 | Vendor Layer | Support multiple SoC partner implementations | Given the vendor layer supports SoCs from multiple hardware partners, When HFP configuration files are generated for each platform, Then each platform's HFP files accurately reflect that specific SoC's capabilities, And file structure and location remain consistent across all SoC implementations | Cross-platform verification | Must |

### Non-Functional Requirements

| ID | Category | Description | Acceptance Criteria | Verification Method | Priority |
|----|----------|-------------|---------------------|---------------------|----------|
| NFR-001 | Performance | HFP files must be quickly accessible at runtime | Given the middleware requests decoder capabilities, When the HFP files are read from `/product/hfp/config/`, Then file access completes within 100ms | Performance test | Must |
| NFR-002 | Reliability | HFP configuration files must be present before middleware initialization | Given the platform boots successfully, When the middleware initializes, Then HFP configuration files are already present at `/product/hfp/config/`, And files are not corrupted or incomplete | Boot sequence test, File integrity check | Must |

## File Deployment Specifications

### Location
All HFP configuration files must be deployed to:
```
/product/hfp/config/
```

### Required Files
1. **Video Decoder Configuration**: `hfp-videodecoder.yaml`
2. **Audio Decoder Configuration**: `hfp-audiodecoder.yaml`

### File Permissions
- **Owner**: root
- **Group**: system
- **Permissions**: 0644 (readable by all, writable by owner)

### Deployment Timing
HFP files must be present on the filesystem before RDK middleware initialization. Recommended deployment methods:
- Include in vendor partition during image build
- Deploy via init scripts during first boot
- Package in vendor-specific OTA updates

## Multi-Partner SoC Support

Vendors supporting multiple SoC partners must ensure:

1. **Platform-Specific Capabilities**: Each SoC implementation has accurate HFP files reflecting that hardware's true capabilities
2. **Consistent Structure**: All platforms use identical file structure, field names, and locations regardless of SoC vendor
3. **Licensed Restrictions**: Capabilities reflect licensing agreements, not theoretical hardware maximums
4. **Independent Validation**: Each platform's HFP files validate independently against the official schema
5. **Version Alignment**: `interfaceVersion` matches the HAL interface implemented for that platform

### Example Multi-Partner Scenario

A vendor supporting three SoC partners might have:
- **Broadcom BCM7252**: High-end STB with full HEVC, AV1, Dolby Vision support
- **Amlogic S905**: Mid-range STB with HEVC but limited HDR support
- **Realtek RTD1319**: Budget platform with H.264 and basic HEVC

Each platform has distinct HFP files at `/product/hfp/config/` but:
- File names remain `hfp-videodecoder.yaml` and `hfp-audiodecoder.yaml`
- Schema structure is identical
- Only capability values differ
- All validate against the same schema

## Validation Requirements

All HFP files must be validated before deployment:

## Compliance Verification

To verify vendor layer compliance with these requirements:

1. **File Presence Check**: Verify both files exist at `/product/hfp/config/`
2. **Schema Validation**: Run validation script against both files
3. **Content Verification**: Ensure all required fields are present and populated
4. **Licensed Capability Check**: Verify capabilities match licensing agreements
5. **Cross-Platform Consistency**: Compare file structures across all supported platforms
6. **Runtime Accessibility**: Verify middleware can read files during initialization

## Related Documentation

- [HPK HFP Configuration](./hpk-hfp-configuration.md) - Overview of HFP configuration files and schema
- [HFP Validation Setup](./hfp-validation-setup.md) - Instructions for setting up and using validation tools
- [HAL Feature Profiles](https://rdkcentral.github.io/rdk-halif-aidl/0.12.0/halif/key_concepts/hal/hal_feature_profiles/) - Conceptual documentation for HFPs
