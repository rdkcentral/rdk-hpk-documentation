#!/bin/bash

# validate-hfp.sh - Validate HFP YAML files against official schema
# Usage: ./validate-hfp.sh -t <audio|video> -v <version> -f <yaml-file>

set -e

# Default values
TYPE=""
VERSION="main"
YAML_FILE=""
BASE_URL="https://raw.githubusercontent.com/rdkcentral/rdk-hpk-documentation"

# Parse arguments
while getopts "t:v:f:h" opt; do
  case $opt in
    t) TYPE="$OPTARG" ;;
    v) VERSION="$OPTARG" ;;
    f) YAML_FILE="$OPTARG" ;;
    h)
      echo "Usage: $0 -t <audio|video> -v <version> -f <yaml-file>"
      echo "  -t: Type of decoder (audio or video)"
      echo "  -v: HPK release version/tag (e.g., 3.1.0, main)"
      echo "  -f: Path to your YAML file to validate"
      exit 0
      ;;
    *) 
      echo "Invalid option. Use -h for help."
      exit 1
      ;;
  esac
done

# Validate required arguments
if [ -z "$TYPE" ]; then
  echo "Error: Type (-t) is required. Use 'audio' or 'video'"
  exit 1
fi

if [ "$TYPE" != "audio" ] && [ "$TYPE" != "video" ]; then
  echo "Error: Type must be 'audio' or 'video'"
  exit 1
fi

if [ -z "$YAML_FILE" ]; then
  echo "Error: YAML file (-f) is required"
  exit 1
fi

if [ ! -f "$YAML_FILE" ]; then
  echo "Error: File '$YAML_FILE' not found"
  exit 1
fi

# Check if pykwalify is installed
if ! command -v pykwalify &> /dev/null; then
  echo "Error: pykwalify is not installed"
  echo "Install it with: pip install pykwalify"
  exit 1
fi

# Set decoder type
if [ "$TYPE" == "audio" ]; then
  DECODER="audiodecoder"
else
  DECODER="videodecoder"
fi

# Create temporary directory
TEMP_DIR=$(mktemp -d)
trap "rm -rf $TEMP_DIR" EXIT

echo "Downloading HFP $TYPE decoder schema (version: $VERSION)..."

# Download schema file
SCHEMA_URL="$BASE_URL/$VERSION/hfp/$DECODER/hfp-${DECODER}-schema.yaml"
curl -fsSL "$SCHEMA_URL" -o "$TEMP_DIR/hfp-${DECODER}-schema.yaml"

if [ $? -ne 0 ]; then
  echo "Error: Failed to download schema from $SCHEMA_URL"
  exit 1
fi

echo "Validating $(basename $YAML_FILE) against schema..."

# Run validation
pykwalify -d "$YAML_FILE" -s "$TEMP_DIR/hfp-${DECODER}-schema.yaml"

if [ $? -eq 0 ]; then
  echo "✓ Validation successful!"
else
  echo "✗ Validation failed"
  exit 1
fi
