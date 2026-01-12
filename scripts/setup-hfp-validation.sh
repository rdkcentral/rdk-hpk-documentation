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

# Setup script for HFP validation environment
# Creates a virtual environment and installs pykwalify

set -e

MY_PATH="$(realpath ${BASH_SOURCE[0]})"
SCRIPTS_DIR="$(dirname ${MY_PATH})"
VENV_NAME="hfp-validation-env"
VENV_DIR="${SCRIPTS_DIR}/${VENV_NAME}"
REQUIREMENTS_FILE="${SCRIPTS_DIR}/hfp-requirements.txt"

# Colors
NO_COLOR="\e[0m"
GREEN="\e[0;32m"
YELLOW="\e[1;33m"
CYAN="\e[0;36m"

echo -e "${CYAN}Setting up HFP validation environment...${NO_COLOR}"

# Check if virtual environment exists
if [[ ! -d "$VENV_DIR" ]]; then
    echo -e "${YELLOW}Creating virtual environment '$VENV_NAME'...${NO_COLOR}"
    python3 -m venv "$VENV_DIR"
    echo -e "${GREEN}Virtual environment created.${NO_COLOR}"
else
    echo -e "${GREEN}Virtual environment already exists.${NO_COLOR}"
fi

# Activate virtual environment
echo -e "${YELLOW}Activating virtual environment...${NO_COLOR}"
source "$VENV_DIR/bin/activate"

# Upgrade pip
echo -e "${YELLOW}Upgrading pip...${NO_COLOR}"
pip install --upgrade pip > /dev/null 2>&1

# Install requirements
if [[ -f "$REQUIREMENTS_FILE" ]]; then
    echo -e "${YELLOW}Installing pykwalify...${NO_COLOR}"
    pip install -r "$REQUIREMENTS_FILE" > /dev/null 2>&1
    echo -e "${GREEN}pykwalify installed successfully.${NO_COLOR}"
else
    echo -e "${YELLOW}Requirements file not found. Installing pykwalify directly...${NO_COLOR}"
    pip install pykwalify > /dev/null 2>&1
    echo -e "${GREEN}pykwalify installed successfully.${NO_COLOR}"
fi

# Verify installation
echo ""
echo -e "${GREEN}✓ Setup complete!${NO_COLOR}"
echo ""
echo "To use the validation environment:"
echo -e "  ${YELLOW}source ${SCRIPTS_DIR}/activate-hfp-env.sh${NO_COLOR}"
echo ""
echo "After activation, you can run:"
echo -e "  ${YELLOW}./validate-hfp.sh -t audio -v 3.1.0 -f your-file.yaml${NO_COLOR}"
echo ""
echo "To deactivate the environment when done:"
echo -e "  ${YELLOW}deactivate${NO_COLOR}"
