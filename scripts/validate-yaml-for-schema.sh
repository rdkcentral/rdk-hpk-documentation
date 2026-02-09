#!/bin/bash

# validate-yaml-for-schema.sh - Generic YAML validation against a schema file
# Usage: ./validate-yaml-for-schema.sh -f <yaml-file> -s <schema-file>

set -e

# Default values
YAML_FILE=""
SCHEMA_FILE=""

# Color output for better readability
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Parse arguments
while getopts "f:s:h" opt; do
  case $opt in
    f) YAML_FILE="$OPTARG" ;;
    s) SCHEMA_FILE="$OPTARG" ;;
    h)
      echo "Usage: validate-yaml-for-schema.sh -f <yaml-file> -s <schema-file>"
      echo ""
      echo "Options:"
      echo "  -f: Path to YAML file to validate"
      echo "  -s: Path to schema file"
      echo "  -h: Show this help message"
      echo ""
      echo "Examples:"
      echo "  validate-yaml-for-schema.sh -f config.yaml -s schema.yaml"
      echo "  validate-yaml-for-schema.sh -f /path/to/data.yaml -s /path/to/schema.yaml"
      exit 0
      ;;
    *) 
      echo -e "${RED}Error: Invalid option. Use -h for help.${NC}" >&2
      exit 1
      ;;
  esac
done

# Validate required arguments
if [ -z "$YAML_FILE" ]; then
  echo -e "${RED}Error: YAML file (-f) is required${NC}" >&2
  exit 1
fi

if [ -z "$SCHEMA_FILE" ]; then
  echo -e "${RED}Error: Schema file (-s) is required${NC}" >&2
  exit 1
fi

if [ ! -f "$YAML_FILE" ]; then
  echo -e "${RED}Error: YAML file '$YAML_FILE' not found${NC}" >&2
  exit 1
fi

if [ ! -f "$SCHEMA_FILE" ]; then
  echo -e "${RED}Error: Schema file '$SCHEMA_FILE' not found${NC}" >&2
  exit 1
fi

# Check if pykwalify is installed
if ! command -v pykwalify &> /dev/null; then
  echo -e "${RED}Error: pykwalify is not installed${NC}" >&2
  echo -e "${YELLOW}Install it with: pip install pykwalify${NC}" >&2
  exit 1
fi

echo "Validating $(basename "$YAML_FILE") against $(basename "$SCHEMA_FILE")..."
echo ""

# Run validation and forward all output
if pykwalify -d "$YAML_FILE" -s "$SCHEMA_FILE" 2>&1; then
  echo ""
  echo -e "${GREEN}✓ Validation successful!${NC}"
  echo "The YAML file is valid according to the schema"
  exit 0
else
  echo ""
  echo -e "${RED}✗ Validation failed${NC}"
  echo "Please check the errors above and fix your YAML file"
  exit 1
fi
