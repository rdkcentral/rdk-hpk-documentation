#!/bin/bash

# Test script for validate-hfp.sh
# Tests validation against remote schemas from the repository
# Usage: ./test-validate-hfp.sh [-b <branch>] [-v <version-tag>]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Default values
TEST_BRANCH="${TEST_BRANCH:-main}"
VERSION_TAG="${VERSION_TAG:-}"

# Parse arguments
while getopts "b:v:h" opt; do
  case $opt in
    b) TEST_BRANCH="$OPTARG" ;;
    v) VERSION_TAG="$OPTARG" ;;
    h)
      echo "Usage: test-validate-hfp.sh [-b <branch>] [-v <version-tag>]"
      echo ""
      echo "Options:"
      echo "  -b: Branch to test against (default: main)"
      echo "  -v: Version tag to test (optional, no default)"
      echo "  -h: Show this help message"
      echo ""
      echo "Examples:"
      echo "  test-validate-hfp.sh                                    # Test against main branch only"
      echo "  test-validate-hfp.sh -b feature/30-codec-dynamic-range  # Test against feature branch"
      echo "  test-validate-hfp.sh -b main -v 3.1.0                   # Test main and version 3.1.0"
      echo "  test-validate-hfp.sh -v 4.0.0                           # Test main and version 4.0.0"
      exit 0
      ;;
    *) 
      echo -e "${RED}Error: Invalid option. Use -h for help.${NC}" >&2
      exit 1
      ;;
  esac
done

echo -e "${CYAN}=== Testing validate-hfp.sh ===${NC}"
echo ""
echo "This test validates HFP YAML files against schemas downloaded from GitHub."
echo "It tests the complete workflow including schema download and validation."
echo ""
echo -e "${CYAN}Testing against branch: ${YELLOW}$TEST_BRANCH${NC}"
echo ""

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"

# Check if validation script exists
VALIDATION_SCRIPT="$SCRIPT_DIR/validate-hfp.sh"
if [ ! -f "$VALIDATION_SCRIPT" ]; then
  echo -e "${RED}✗ Error: validate-hfp.sh not found${NC}"
  exit 1
fi

# Check if helper script exists
HELPER_SCRIPT="$SCRIPT_DIR/validate-yaml-for-schema.sh"
if [ ! -f "$HELPER_SCRIPT" ]; then
  echo -e "${RED}✗ Error: validate-yaml-for-schema.sh not found${NC}"
  echo "validate-hfp.sh requires this helper script"
  exit 1
fi

# Reference test files from the repository
AUDIO_YAML="$REPO_ROOT/hfp-reference/audiodecoder/hfp-audiodecoder.yaml"
VIDEO_YAML="$REPO_ROOT/hfp-reference/videodecoder/hfp-videodecoder.yaml"

# Check if test files exist
if [ ! -f "$AUDIO_YAML" ]; then
  echo -e "${RED}✗ Error: Audio test file not found: $AUDIO_YAML${NC}"
  exit 1
fi

if [ ! -f "$VIDEO_YAML" ]; then
  echo -e "${RED}✗ Error: Video test file not found: $VIDEO_YAML${NC}"
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

# Check network connectivity
if ! curl -fsSL --max-time 5 "https://api.github.com" > /dev/null 2>&1; then
  echo -e "${YELLOW}Warning: Cannot connect to GitHub${NC}"
  echo "Network access is required for validate-hfp.sh to download schemas"
  echo "Skipping tests that require network access"
  exit 0
fi

# Test counter
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Function to run a test
run_test() {
  local test_name="$1"
  local type="$2"
  local version="$3"
  local yaml_file="$4"
  
  TESTS_RUN=$((TESTS_RUN + 1))
  
  echo -e "${CYAN}Test $TESTS_RUN: $test_name${NC}"
  echo "  Type:    $type"
  echo "  Version: $version"
  echo "  File:    $(basename "$yaml_file")"
  echo ""
  
  if bash "$VALIDATION_SCRIPT" -t "$type" -v "$version" -f "$yaml_file"; then
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

# Run tests
echo -e "${CYAN}Running validation tests...${NC}"
echo ""

# Test against specified branch
run_test "Audio Decoder ($TEST_BRANCH branch)" "audio" "$TEST_BRANCH" "$AUDIO_YAML"
run_test "Video Decoder ($TEST_BRANCH branch)" "video" "$TEST_BRANCH" "$VIDEO_YAML"

# Test against specific version tag if provided
if [ -n "$VERSION_TAG" ]; then
  echo -e "${YELLOW}Note: Testing specific version tag: $VERSION_TAG${NC}"
  echo "If the version tag doesn't exist, these tests may fail"
  echo ""
  
  # Check if version tag exists on GitHub
  if curl -fsSL --max-time 5 "https://raw.githubusercontent.com/rdkcentral/rdk-hpk-documentation/$VERSION_TAG/hfp-reference/audiodecoder/hfp-audiodecoder-schema.yaml" > /dev/null 2>&1; then
    run_test "Audio Decoder (version $VERSION_TAG)" "audio" "$VERSION_TAG" "$AUDIO_YAML"
    run_test "Video Decoder (version $VERSION_TAG)" "video" "$VERSION_TAG" "$VIDEO_YAML"
  else
    echo -e "${YELLOW}Skipping version $VERSION_TAG tests (tag not found on GitHub)${NC}"
    echo ""
  fi
fi

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
