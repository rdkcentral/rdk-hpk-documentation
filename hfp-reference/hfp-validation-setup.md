# HFP Validation Setup

This guide explains how to set up the environment and validate Hardware Feature Profile (HFP) configuration files for audio and video decoders.

## Overview

The HPK repository provides a validation script (`validate-hfp.sh`) that automatically downloads the appropriate schema version from the repository and validates your YAML configuration files against it. The script requires `pykwalify`, a Python YAML/JSON schema validator.

## Installing pykwalify

Choose one of the following installation methods based on your workflow:

### Option 1: Using Setup Scripts (Recommended for Repository Users)

If you have cloned the HPK documentation repository, use the provided setup scripts to create an isolated validation environment:

```bash
# Clone the repository if not already done
git clone https://github.com/rdkcentral/rdk-hpk-documentation.git
cd rdk-hpk-documentation/scripts

# Run the setup script to create virtual environment and install pykwalify
./setup-hfp-validation.sh

# Activate the environment (must be sourced from the scripts directory)
source ./activate-hfp-env.sh

# Now you can run validation
./validate-hfp.sh -t audio -v 3.1.0 -f ../path/to/your-file.yaml

# When done, deactivate the environment
deactivate
```

**Benefits:**
- Isolated environment that doesn't affect system Python packages
- Automatically installs the correct version of pykwalify
- Easy to activate/deactivate as needed

**Important:** These scripts must be run from the `rdk-hpk-documentation/scripts` directory. If you need to validate files from other directories, use Option 2 or Option 3 instead.

### Option 2: Manual Virtual Environment Setup

If you prefer to manage your own virtual environment or are working outside the repository:

```bash
# Create a virtual environment
python3 -m venv hfp-validation-env

# Activate the virtual environment
# On macOS/Linux:
source hfp-validation-env/bin/activate
# On Windows:
# hfp-validation-env\Scripts\activate

# Install pykwalify
pip install pykwalify

# Run validation (after downloading the script - see below)
./validate-hfp.sh -t audio -v 3.1.0 -f hfp-audiodecoder.yaml

# When done, deactivate
deactivate
```

**Benefits:**
- Full control over the environment location and configuration
- Can be placed anywhere in your filesystem
- Standard Python virtual environment workflow

### Option 3: System-wide or User Installation

Install pykwalify directly without a virtual environment:

```bash
# System-wide installation (may require sudo/admin privileges)
pip install pykwalify

# OR user installation (no admin required)
pip install --user pykwalify

# OR using pip3 explicitly
pip3 install pykwalify
```

**Benefits:**
- Simple, one-time installation
- Available system-wide for all projects
- No activation/deactivation needed

**Note:** This method may conflict with other Python packages if you have version dependencies in other projects.

### Verify Installation

After installation, verify that pykwalify is available:

```bash
pykwalify --version
```

Expected output: `pykwalify 1.8.0` or similar

## Using the Validation Script

The validation script (`validate-hfp.sh`) automatically downloads the appropriate schema version and validates your YAML file.

### For Repository Users

If you have the repository cloned:

```bash
cd rdk-hpk-documentation/scripts

# Activate the validation environment (if using Option 1)
source ./activate-hfp-env.sh

# Validate your file
./validate-hfp.sh -t audio -v 3.1.0 -f /path/to/hfp-audiodecoder.yaml
./validate-hfp.sh -t video -v 3.1.0 -f /path/to/hfp-videodecoder.yaml

# Deactivate when done
deactivate
```

### For Standalone Use (Download Script)

If you don't have the repository cloned, download the script directly:

```bash
# Download the script
curl -fsSL https://raw.githubusercontent.com/rdkcentral/rdk-hpk-documentation/main/scripts/validate-hfp.sh -o validate-hfp.sh
chmod +x validate-hfp.sh

# Validate against a specific release version
./validate-hfp.sh -t audio -v 3.1.0 -f hfp-audiodecoder.yaml
./validate-hfp.sh -t video -v 3.1.0 -f hfp-videodecoder.yaml

# Validate against a branch (e.g., main, develop)
./validate-hfp.sh -t audio -v main -f hfp-audiodecoder.yaml
./validate-hfp.sh -t video -v develop -f hfp-videodecoder.yaml
```

**Note:** For security, always review scripts before execution, especially from remote sources.

### Direct Execution (Use with Caution)

**⚠️ WARNING:** Piping remote scripts directly to bash is a security risk. This method executes code without allowing you to review it first. Only use this in trusted environments where you understand the risks.

For quick validation in trusted environments, you can pipe directly to bash:

```bash
curl -fsSL https://raw.githubusercontent.com/rdkcentral/rdk-hpk-documentation/main/scripts/validate-hfp.sh | \
  bash -s -- -t audio -v 3.1.0 -f hfp-audiodecoder.yaml
```

## Script Usage

The validation script accepts the following parameters:

```bash
./validate-hfp.sh -t <audio|video> -v <version> -f <yaml-file>
```

**Parameters:**
- `-t`: Type of decoder - `audio` or `video` (required)
- `-v`: HPK release version/tag or branch name (e.g., `3.1.0`, `main`, `develop`) (required)
- `-f`: Path to your YAML file to validate (required)
- `-h`: Show help message

**Examples:**

```bash
# Validate audio decoder against version 3.1.0
./validate-hfp.sh -t audio -v 3.1.0 -f platform-audio-decoder.yaml

# Validate video decoder against main branch
./validate-hfp.sh -t video -v main -f hfp-videodecoder.yaml

# Show help
./validate-hfp.sh -h
```

## Requirements

- **Python 3**: The validation environment requires Python 3.x
- **pykwalify**: Must be installed (see installation options above)
- **Network access**: Required to download schemas from the GitHub repository
- **curl**: Used by the script to download schemas

## Validation Best Practices

- **Always validate before committing**: Run validation on your configuration files before committing changes to your repository
- **Use pre-commit hooks**: Set up Git pre-commit hooks to automatically validate HFP files
- **Use specific versions**: Validate against the specific HPK version your platform targets, not just `main` or `develop`
- **Version alignment**: Ensure the `interfaceVersion` in your HFP file matches the HAL interface version your platform implements
- **Schema updates**: When adding new configuration attributes, ensure corresponding schema files are updated
- **Local testing**: Test configuration changes locally before pushing to your platform

## Troubleshooting

### pykwalify not found

If you see an error that pykwalify is not installed:

```bash
# If using virtual environment, make sure it's activated
source ./activate-hfp-env.sh  # or your venv activation path

# If not using venv, install pykwalify
pip install pykwalify
```

### Schema download fails

If the script fails to download schemas:

- **Check network connectivity**: Ensure you can access GitHub
- **Verify version exists**: The version/tag specified with `-v` must exist in the repository
- **Check branch name**: If using a branch name, ensure it's spelled correctly (e.g., `main`, not `master`)

### Permission denied

If you get "permission denied" when running the script:

```bash
# Make the script executable
chmod +x validate-hfp.sh
```

### Virtual environment activation fails

If sourcing the activation script fails:

```bash
# Make sure you're using 'source' or '.'
source ./activate-hfp-env.sh

# NOT this (will fail):
# ./activate-hfp-env.sh
```

## Advanced Usage

### Pre-commit Hook Setup

To automatically validate HFP files before committing changes, set up a Git pre-commit hook. This ensures that invalid configuration files are caught before they enter the repository.

**Setup Instructions:**

1. Edit the precommit hook to configure your HPK version and file patterns:

```bash
# Edit .git/hooks/pre-commit
# Update these variables:
HPK_VERSION="3.1.0"  # Your target HPK version
SCRIPT_PATH="./scripts/validate-hfp.sh"  # Path to validation script
```

2. Ensure pykwalify is installed (the hook will check and notify if missing)

**What the hook does:**
- Automatically validates all staged HFP YAML files before commit
- Detects audio and video decoder files by naming patterns
- Blocks the commit if validation fails
- Shows clear error messages for validation failures

**Testing the hook:**

```bash
# Stage an HFP file
git add configs/hfp-audiodecoder.yaml

# Try to commit (hook will run automatically)
git commit -m "Update audio decoder config"
```

If validation fails, fix the issues and try again. To bypass the hook temporarily (not recommended):

```bash
git commit --no-verify -m "Update config"
```

### Validating Multiple Files

To validate multiple files, you can use a simple loop:

```bash
# Bash loop
for file in configs/*.yaml; do
    ./validate-hfp.sh -t audio -v 3.1.0 -f "$file"
done
```

## Related Documentation

- [HPK HFP Configuration](./hpk-hfp-configuration.md) - Overview of HFP configuration files and schema
- [HAL Feature Profiles](https://rdkcentral.github.io/rdk-halif-aidl/0.12.0/halif/key_concepts/hal/hal_feature_profiles/) - Conceptual documentation for HFPs
