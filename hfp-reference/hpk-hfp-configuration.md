# HPK - Hardware Feature Profile Configuration

## Overview

This document describes how the RDK Hardware Porting Kit (HPK) uses Hardware Abstraction Layer (HAL) Feature Profile (HFP) configuration files to declare hardware capabilities, with a current focus on audio and video decoder features.

In RDK, a HAL Feature Profile is a YAML-based declaration provided by OEMs to describe the capabilities of their vendor-layer HAL implementation (for example, codec support, resource limits and mandatory/optional components). For the full conceptual description of HFPs, including schema examples and versioning, see the [HAL Feature Profiles documentation](https://rdkcentral.github.io/rdk-halif-aidl/0.12.0/halif/key_concepts/hal/hal_feature_profiles/).

Within HPK, these HFP files are consumed to expose decoder capabilities (such as supported codecs) to test suites and higher-level components. They act as a temporary mechanism for advertising hardware decoder capabilities until a formal runtime API is available to expose this information directly.

**Important:** The HFP files provided here are reference implementations demonstrating maximum capabilities and proper schema structure. While the schema files are used to validate all HFP configurations across platforms, the actual HFP files are platform-specific and must be defined by OEMs/vendors based on their platform's hardware capabilities and requirements. Each platform's HFP files should be created and maintained according to the specific decoder capabilities available on that platform.

### Version Fields

HFP configuration files include two important version fields:

- **`interfaceVersion`**: Denotes the [rdk-halif-aidl](https://github.com/rdkcentral/rdk-halif-aidl) version that defines the HAL interface specification. This version should match the HAL interface version your platform implements.

- **`schemaVersion`**: Indicates the version of the HFP schema used for validation and allows for schema evolution.

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

- **Cross-vendor Accessibility**: Schema files are available to all vendors across the vendor layer for use in documentation and test infrastructure
- **Validation**: Enables automated validation of configuration files to ensure compliance with the expected format
- **Version Management**: Configuration files include an `interfaceVersion` field to support configuration evolution across releases
- **Extensibility**: Schema can be updated to accommodate new additions and changes to configuration YAMLs

## Testing Configuration Files

You can validate your configuration files against their respective schemas using the automated validation script provided in the repository.

For detailed instructions on setting up the validation environment and using the validation script, please refer to the [HFP Validation Setup Guide](./hfp-validation-setup.md).

### Validation Best Practices

- Always validate configuration files before committing changes
- Set up pre-commit hooks to validate HFP files
- Ensure schema files are updated when adding new configuration attributes
- Run validation as part of your CI/CD pipeline to catch configuration errors early
- Keep the `interfaceVersion` in sync between configuration files and schema definitions

## Updating an Existing Schema

If you are an architect updating an existing HFP schema (for example, adding new fields, modifying validation rules, or aligning with a new HAL interface version), follow this process:

### 1. Update the Schema File

Modify the schema file to reflect the required changes:

- Edit `hfp-<module>-schema.yaml` in the module directory (e.g., `hfp-reference/audiodecoder/`)
- Add new fields, update validation rules, or modify constraints as needed
- Increment the `schemaVersion` in the schema file to reflect the change
- Ensure `schemaVersion` aligns with the `interfaceVersion` from the corresponding [rdk-halif-aidl](https://github.com/rdkcentral/rdk-halif-aidl) release
- Document changes with comments explaining new fields or modified behavior

### 2. Update Reference YAML Files

Update the reference implementation to match the new schema:

- Edit `hfp-<module>.yaml` in the same directory
- Add values for any new required fields
- Update existing values if field definitions have changed
- Ensure `schemaVersion` and `interfaceVersion` match the schema file
- Add comments explaining new fields for vendor guidance

### 3. Validate the Changes

Test your updated schema and reference YAML using the validation script:

```bash
# From the repository root
./scripts/validate-hfp.sh hfp-reference/<module>/hfp-<module>.yaml hfp-reference/<module>/hfp-<module>-schema.yaml
```

Resolve any validation errors. See the [HFP Validation Setup Guide](./hfp-validation-setup.md) for detailed validation instructions.

### 4. Update Documentation

Update relevant documentation to reflect the schema changes:

- Update this file if new modules or significant structural changes were made
- Document breaking changes and migration guidance for vendors
- Update version information in the Version Fields section if needed

### 5. Update Release History

Prepare release documentation:

- Update `CHANGELOG.md` with detailed description of schema changes, new fields added, modified validation rules, and breaking changes (if any)
- Update `RELEASE.md` with new version number, release date, and summary of changes
- Include migration instructions for vendors if schema changes require updates to existing HFP files

### 6. Release and Tag

Publish the updated schema:

- Commit all changes with descriptive commit message referencing the HAL interface version
- Tag the repository with semantic version (e.g., `v1.2.0`) following the versioning strategy
- Create a GitHub release with release notes highlighting schema changes and version alignment
- Reference the corresponding `rdk-halif-aidl` version in release notes

### 7. Notify Stakeholders

Communicate the schema update:

- Notify vendor teams about the schema update and any required actions
- Highlight breaking changes that require vendor HFP file updates
- Share validation script location and updated schema files
- Provide timeline for vendors to migrate to new schema version (if applicable)

### Key Considerations

- **Version Alignment**: Always ensure `schemaVersion` aligns with the `interfaceVersion` from the HAL interface specification
- **Backward Compatibility**: Avoid breaking changes when possible; if unavoidable, provide clear migration path
- **Validation Required**: Never commit schema changes without successful validation of reference YAMLs
- **Vendor Impact**: Consider the impact on vendors - breaking changes require vendor coordination and migration time
