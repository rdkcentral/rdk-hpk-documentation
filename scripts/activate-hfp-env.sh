#!/usr/bin/env bash
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2025 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************

# Activation script for HFP validation environment
# This script must be sourced, not executed

# Check if the script is sourced
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "ERROR: This script must be sourced, not executed."
    echo "Usage: source ${BASH_SOURCE[0]}"
    echo "   or: . ${BASH_SOURCE[0]}"
    exit 1
fi

# Get the directory where this script is located
SCRIPT_PATH="${BASH_SOURCE[0]}"
if [[ -n "$SCRIPT_PATH" ]]; then
    SCRIPTS_DIR="$(cd "$(dirname "$SCRIPT_PATH")" && pwd)"
else
    SCRIPTS_DIR="$(pwd)"
fi

VENV_NAME="hfp-validation-env"
VENV_DIR="${SCRIPTS_DIR}/${VENV_NAME}"

# Check if virtual environment exists
if [[ ! -d "$VENV_DIR" ]]; then
    echo "ERROR: Virtual environment not found at: $VENV_DIR"
    echo "Please run setup-hfp-validation.sh first to create the environment."
    return 1
fi

# Activate virtual environment
source "$VENV_DIR/bin/activate"

# Verify pykwalify is installed
if command -v pykwalify &> /dev/null; then
    echo "HFP validation environment activated."
    echo "pykwalify version: $(pykwalify --version 2>&1 | head -1)"
    echo ""
    echo "You can now run: ./validate-hfp.sh -t <audio|video> -v <version> -f <file>"
    echo "To deactivate: deactivate"
else
    echo "WARNING: pykwalify not found in virtual environment."
    echo "Run: pip install pykwalify"
fi
