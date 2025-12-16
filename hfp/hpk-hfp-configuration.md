# HPK HFP Configuration

## Overview

This document describes the Hardware Feature Profile (HFP) configuration files used to expose audio and video decoder capabilities. These configuration files serve as a temporary mechanism to provide decoder capabilities until a formal API is available to expose this information.

## Configuration Files

The HFP configuration is organized into the following directories:

### Audio Decoder Configuration
- **Location**: `hfp/audiodecoder/`
- **Configuration File**: `hfp-audiodecoder.yaml`
- **Schema File**: `hfp-audiodecoder-schema.yaml`

### Video Decoder Configuration
- **Location**: `hfp/videodecoder/`
- **Configuration File**: `hfp-videodecoder.yaml`
- **Schema File**: `hfp-videodecoder-schema.yaml`

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
pykwalify -d ./hfp-audiodecoder.yaml -s ./hfp-audiodecoder-schema.yaml
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
