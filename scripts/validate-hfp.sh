#!/bin/bash

# validate-hfp.sh - Validate HFP YAML files against official schema
# Usage: ./validate-hfp.sh -t <audio|video> -v <version> -f <yaml-file>

set -e

# Default values
TYPE=""
VERSION=""
YAML_FILE=""
BASE_URL="https://raw.githubusercontent.com/rdkcentral/rdk-hpk-documentation"

# Color output for better readability
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Parse arguments
while getopts "t:v:f:h" opt; do
  case $opt in
    t) TYPE="$OPTARG" ;;
    v) VERSION="$OPTARG" ;;
    f) YAML_FILE="$OPTARG" ;;
    h)
      echo "Usage: validate-hfp.sh -t <audio|video> -v <version> -f <yaml-file>"
      echo ""
      echo "Options:"
      echo "  -t: Type of decoder (audio or video)"
      echo "  -v: HPK release version/tag (e.g., 3.1.0, main)"
      echo "  -f: Path to your YAML file to validate"
      echo "  -h: Show this help message"
      echo ""
      echo "Examples:"
      echo "  validate-hfp.sh -t audio -v 3.1.0 -f my-audio-decoder.yaml"
      echo "  validate-hfp.sh -t video -v main -f platform-video-decoder.yaml"
      exit 0
      ;;
    *) 
      echo -e "${RED}Error: Invalid option. Use -h for help.${NC}" >&2
      exit 1
      ;;
  esac
done

# Validate required arguments
if [ -z "$TYPE" ]; then
  echo -e "${RED}Error: Type (-t) is required. Use 'audio' or 'video'${NC}" >&2
  exit 1
fi

if [ "$TYPE" != "audio" ] && [ "$TYPE" != "video" ]; then
  echo -e "${RED}Error: Type must be 'audio' or 'video'${NC}" >&2
  exit 1
fi

if [ -z "$VERSION" ]; then
  echo -e "${RED}Error: Version (-v) is required${NC}" >&2
  echo -e "${YELLOW}Specify a release version/tag (e.g., -v 3.1.0, -v main)${NC}" >&2
  exit 1
fi

if [ -z "$YAML_FILE" ]; then
  echo -e "${RED}Error: YAML file (-f) is required${NC}" >&2
  exit 1
fi

if [ ! -f "$YAML_FILE" ]; then
  echo -e "${RED}Error: File '$YAML_FILE' not found${NC}" >&2
  exit 1
fi

# Check if pykwalify is installed
if ! command -v pykwalify &> /dev/null; then
  echo -e "${RED}Error: pykwalify is not installed${NC}" >&2
  echo -e "${YELLOW}Install it with: pip install pykwalify${NC}" >&2
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
SCHEMA_URL="$BASE_URL/$VERSION/hfp-reference/$DECODER/hfp-${DECODER}-schema.yaml"
SCHEMA_FILE="$TEMP_DIR/hfp-${DECODER}-schema.yaml"

if ! curl -fsSL "$SCHEMA_URL" -o "$SCHEMA_FILE" 2>/dev/null; then
  echo -e "${RED}Error: Failed to download schema from $SCHEMA_URL${NC}" >&2
  echo -e "${YELLOW}Possible causes:${NC}" >&2
  echo "  - Version '$VERSION' does not exist" >&2
  echo "  - Network connectivity issues" >&2
  echo "  - Repository structure has changed" >&2
  exit 1
fi

echo "Schema downloaded successfully"
echo "Validating $(basename "$YAML_FILE") against schema..."
echo ""

# Run validation and capture output
if pykwalify -d "$YAML_FILE" -s "$SCHEMA_FILE" 2>&1; then
  echo ""
  echo -e "${GREEN}✓ Validation successful!${NC}"
  echo "Your HFP configuration is valid according to schema version: $VERSION"
  exit 0
else
  echo ""
  echo -e "${RED}✗ Validation failed${NC}"
  echo "Please check the errors above and fix your YAML file"
  exit 1
fi
