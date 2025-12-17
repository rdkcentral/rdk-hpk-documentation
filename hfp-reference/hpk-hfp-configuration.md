# HPK - Hardware Feature Profile Configuration

## Overview

This document describes how the RDK Hardware Porting Kit (HPK) uses HAL Feature Profile (HFP) configuration files to declare hardware capabilities, with a current focus on audio and video decoder features.

In RDK, a HAL Feature Profile is a YAML-based declaration provided by OEMs to describe the capabilities of their vendor-layer HAL implementation (for example, codec support, resource limits and mandatory/optional components). For the full conceptual description of HFPs, including schema examples and versioning, see the [HAL Feature Profiles documentation](https://rdkcentral.github.io/rdk-halif-aidl/0.12.0/halif/key_concepts/hal/hal_feature_profiles/).

Within HPK, these HFP files are consumed to expose decoder capabilities (such as supported codecs) to test suites and higher-level components. They act as a temporary mechanism for advertising hardware decoder capabilities until a formal runtime API is available to expose this information directly.

**Important:** The HFP files provided here are reference implementations demonstrating maximum capabilities and proper schema structure. While the schema files are used to validate all HFP configurations across platforms, the actual HFP files are platform-specific and must be defined by OEMs/vendors based on their platform's hardware capabilities and requirements. Each platform's HFP files should be created and maintained according to the specific decoder capabilities available on that platform.

## Configuration Files

The HFP configuration is organized into the following directories:

| Module | HFP | HFP Schema |
|--------|-----|------------|
| audiodecoder | [hfp-audiodecoder.yaml](./audiodecoder/hfp-audiodecoder.yaml) | [hfp-audiodecoder-schema.yaml](./audiodecoder/hfp-audiodecoder-schema.yaml) |
| videodecoder | [hfp-videodecoder.yaml](./videodecoder/hfp-videodecoder.yaml) | [hfp-videodecoder-schema.yaml](./videodecoder/hfp-videodecoder-schema.yaml) |

## Schema Purpose

The schema files are designed to standardize the YAML configuration format across different vendor implementations for both STB (Set-Top Box) and TV platforms.

### Key Benefits

A formal schema definition ensures consistency in configuration file structure and attributes. The schema provides:

- **Cross-vendor Accessibility**: Schema is accessible across the vendor layer to be used as documentation and for test infrastructure
- **Validation**: Enables automated validation of configuration files to ensure compliance with the expected format
- **Version Management**: Configuration files include an `interfaceVersion` field to support configuration evolution across releases
- **Extensibility**: Schema can be updated to accommodate new additions and changes to configuration YAMLs

## Testing Configuration Files

You can validate your configuration files against their respective schemas using `pykwalify`, a Python-based YAML/JSON schema validator.

### Installation

```bash
pip install pykwalify
```

### Validation Examples

**Audio Decoder Validation:**
```bash
pykwalify -d hfp-audiodecoder.yaml -s hfp-audiodecoder-schema.yaml
```

Expected output:
```
INFO - validation.valid
```

**Video Decoder Validation:**
```bash
pykwalify -d hfp-videodecoder.yaml -s hfp-videodecoder-schema.yaml
```

Expected output:
```
INFO - validation.valid
```

### Validation Best Practices

- Always validate configuration files before committing changes
- Ensure schema files are updated when adding new configuration attributes
- Run validation as part of your CI/CD pipeline to catch configuration errors early
- Keep the `interfaceVersion` in sync between configuration files and schema definitions
