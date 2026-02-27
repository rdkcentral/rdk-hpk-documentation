#!/bin/bash

# Test script for validate-yaml-for-schema.sh
# Simulates usage from another repository that has cloned these scripts

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}=== Testing validate-yaml-for-schema.sh ===${NC}"
echo ""
echo "This test simulates using the validation scripts from another repository."
echo "Following the setup instructions from Option 1 (Using Setup Scripts)"
echo ""

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"

# Check if validation script exists
VALIDATION_SCRIPT="$SCRIPT_DIR/validate-yaml-for-schema.sh"
if [ ! -f "$VALIDATION_SCRIPT" ]; then
  echo -e "${RED}✗ Error: validate-yaml-for-schema.sh not found${NC}"
  exit 1
fi

# Reference test files from the repository
AUDIO_YAML="$REPO_ROOT/hfp-reference/audiodecoder/hfp-audiodecoder.yaml"
AUDIO_SCHEMA="$REPO_ROOT/hfp-reference/audiodecoder/hfp-audiodecoder-schema.yaml"
VIDEO_YAML="$REPO_ROOT/hfp-reference/videodecoder/hfp-videodecoder.yaml"
VIDEO_SCHEMA="$REPO_ROOT/hfp-reference/videodecoder/hfp-videodecoder-schema.yaml"

# Check if test files exist
if [ ! -f "$AUDIO_YAML" ] || [ ! -f "$AUDIO_SCHEMA" ]; then
  echo -e "${RED}✗ Error: Audio test files not found${NC}"
  exit 1
fi

if [ ! -f "$VIDEO_YAML" ] || [ ! -f "$VIDEO_SCHEMA" ]; then
  echo -e "${RED}✗ Error: Video test files not found${NC}"
  exit 1
fi

# Check if pykwalify is installed
if ! command -v pykwalify &> /dev/null; then
  echo -e "${YELLOW}pykwalify not found - setting up validation environment${NC}"
  echo ""
  
  # Check if setup script exists
  SETUP_SCRIPT="$SCRIPT_DIR/setup-validation.sh"
  if [ ! -f "$SETUP_SCRIPT" ]; then
    echo -e "${RED}✗ Error: setup-validation.sh not found${NC}"
    exit 1
  fi
  
  # Run setup script
  echo -e "${CYAN}Running setup-validation.sh...${NC}"
  if ! bash "$SETUP_SCRIPT"; then
    echo -e "${RED}✗ Error: Setup failed${NC}"
    exit 1
  fi
  
  echo ""
  echo -e "${CYAN}Activating validation environment...${NC}"
  
  # Source the activation script
  ACTIVATE_SCRIPT="$SCRIPT_DIR/activate-env.sh"
  if [ ! -f "$ACTIVATE_SCRIPT" ]; then
    echo -e "${RED}✗ Error: activate-env.sh not found${NC}"
    exit 1
  fi
  
  # Source the activation script in the current shell context
  source "$ACTIVATE_SCRIPT"
  
  # Verify pykwalify is now available
  if ! command -v pykwalify &> /dev/null; then
    echo -e "${RED}✗ Error: pykwalify still not available after setup${NC}"
    exit 1
  fi
  
  echo ""
fi

# Test counter
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Function to run a test
run_test() {
  local test_name="$1"
  local yaml_file="$2"
  local schema_file="$3"
  
  TESTS_RUN=$((TESTS_RUN + 1))
  
  echo -e "${CYAN}Test $TESTS_RUN: $test_name${NC}"
  echo "  YAML:   $(basename "$yaml_file")"
  echo "  Schema: $(basename "$schema_file")"
  echo ""
  
  if bash "$VALIDATION_SCRIPT" -f "$yaml_file" -s "$schema_file"; then
    TESTS_PASSED=$((TESTS_PASSED + 1))
    echo -e "${GREEN}✓ Test passed${NC}"
  else
    TESTS_FAILED=$((TESTS_FAILED + 1))
    echo -e "${RED}✗ Test failed${NC}"
  fi
  
  echo ""
  echo "---"
  echo ""
}

# Function to run a negative test (expects validation to fail)
run_negative_test() {
  local test_name="$1"
  local yaml_file="$2"
  local schema_file="$3"
  
  TESTS_RUN=$((TESTS_RUN + 1))
  
  echo -e "${CYAN}Test $TESTS_RUN: $test_name (NEGATIVE TEST - expects failure)${NC}"
  echo "  YAML:   $(basename "$yaml_file")"
  echo "  Schema: $(basename "$schema_file")"
  echo ""
  
  if bash "$VALIDATION_SCRIPT" -f "$yaml_file" -s "$schema_file"; then
    TESTS_FAILED=$((TESTS_FAILED + 1))
    echo -e "${RED}✗ Test failed - validation should have failed but passed${NC}"
  else
    TESTS_PASSED=$((TESTS_PASSED + 1))
    echo -e "${GREEN}✓ Test passed - validation correctly failed as expected${NC}"
  fi
  
  echo ""
  echo "---"
  echo ""
}

# Run tests
echo -e "${CYAN}Running validation tests...${NC}"
echo ""

run_test "Audio Decoder Validation" "$AUDIO_YAML" "$AUDIO_SCHEMA"
run_test "Video Decoder Validation" "$VIDEO_YAML" "$VIDEO_SCHEMA"

# Negative test: USAC with empty profiles (should fail)
INVALID_USAC_YAML="$SCRIPT_DIR/test-invalid-usac-empty-profiles.yaml"
if [ ! -f "$INVALID_USAC_YAML" ]; then
  echo -e "${RED}✗ Error: Invalid USAC test file not found: $INVALID_USAC_YAML${NC}"
  exit 1
fi
run_negative_test "Audio Decoder - USAC with empty profiles" "$INVALID_USAC_YAML" "$AUDIO_SCHEMA"

# Summary
echo -e "${CYAN}=== Test Summary ===${NC}"
echo "Tests run:    $TESTS_RUN"
echo -e "Tests passed: ${GREEN}$TESTS_PASSED${NC}"
if [ $TESTS_FAILED -gt 0 ]; then
  echo -e "Tests failed: ${RED}$TESTS_FAILED${NC}"
  exit 1
else
  echo -e "Tests failed: $TESTS_FAILED"
  echo ""
  echo -e "${GREEN}✓ All tests passed!${NC}"
  exit 0
fi
