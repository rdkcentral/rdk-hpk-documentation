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
MY_PATH="$(realpath ${BASH_SOURCE[0]})"
MY_DIR="$(dirname ${MY_PATH})"
VENV_NAME="python_venv"
DOCS_DIR="${MY_DIR}/.."
VENV_DIR="${DOCS_DIR}/${VENV_NAME}"  # Default virtual environment directory name
EXTERNAL_CONTENT_DIR="${DOCS_DIR}/external_content"

NO_COLOR="\e[0m"
RED="\e[0;31m"
CYAN="\e[0;36m"
YELLOW="\e[1;33m"
GREEN="\e[0;32m"
RED_BOLD="\e[1;31m"
BLUE_BOLD="\e[1;34m"
YELLOW_BOLD="\e[1;33m"

set -e # return on errors

function DUMP_VAR()
{
    local var_name="$1"
    local var_content="${!var_name}"
    echo -e ${CYAN}$var_name:[${var_content}]${NO_COLOR}
}

function ECHO()
{
    echo -e "$*"
}

function DEBUG()
{
    # if set -x is in use debug messages are useless as whole stript will be shown
    if [[ "$-" =~ "x" ]]; then
        return
    fi
    if [[ "${DEBUG_FLAG}" == "1" ]];then
        ECHO "${BLUE_BOLD}DEBUG: ${CYAN}$*${NO_COLOR}" > /dev/stderr
    fi
}

function INFO()
{
    ECHO "${GREEN}$*${NO_COLOR}"
}

function WARNING()
{
    ECHO "${YELLOW_BOLD}Warning: ${YELLOW}$*${NO_COLOR}" > /dev/stderr
}

function ERROR()
{
    ECHO "${RED_BOLD}ERROR: ${RED}$*${NO_COLOR}"
    return 1
}

install_pip_requirements() {
    local requirements_file="$1"
    local requirements_sha_dir="${VENV_DIR}/requirements"
    local requirements_sha_file="${requirements_sha_dir}${requirements_file}.sha256"
    
    if [ ! -f "${requirements_file}" ]; then
        WARNING "No ${requirements_file} found"
        return
    fi
    
    INFO "install_pip_requirements( ${requirements_file} ):"
    
    # Get the directory part of the requirements file path
    local requirements_file_dir=$(dirname "${requirements_sha_file}")
    
    # Create the SHA directory and the requirements file's directory if they don't exist.
    mkdir -p "${requirements_sha_dir}"
    mkdir -p "${requirements_file_dir}" # create requirements file directory if it doesn't exist.
    
    # Calculate current SHA
    local current_sha=$(sha256sum "${requirements_file}" | awk '{print $1}')
    
    # Check if SHA file exists and compare
    if [ -f "${requirements_sha_file}" ]; then
        local stored_sha=$(cat "${requirements_sha_file}")
        if [ "${current_sha}" == "${stored_sha}" ]; then
            INFO "SHA matches, skipping pip install."
            return 0
        else
            INFO "SHA mismatch. Reinstalling."
        fi
    else
        INFO "No SHA file found. Installing and creating one."
    fi
    
    # Install only if SHA doesn't match or file doesn't exist
    if pip install -r "${requirements_file}" >/dev/null 2>&1; then
        INFO "pip install completed"
        # Update SHA file after successful install
        echo "${current_sha}" > "${requirements_sha_file}"
        INFO "SHA file updated: ${requirements_sha_file}"
        return 0
    else
        ERROR "process_and_update_sha(): pip install failed."
        return 1
    fi
}

function clone_repo()
{
    # Requirment it to clone only if not present.
    local repo_url="$1"
    local path="$2"
    local version="$3"
    local message="$4"
    
    if [[ -z "${repo_url}" ]]; then
        ERROR "clone_repo:A url for a repository must be passed to the clone repo function"
    fi
    if [[ -z "${version}" ]]; then
        ERROR "clone_repo:Version not specified"
    fi
    if [[ ! -z "${path}" ]]; then
        if [[ ! -d "${path}" ]]; then
            INFO "git clone ${repo_url} @ ${version} ${CYAN}${message}${NO_COLOR}"
            git clone ${repo_url} "${path}" > /dev/null 2>&1
            cd ${path}
            #INFO "git checkout ${version}"
            git checkout ${version} > /dev/null 2>&1
            cd - > /dev/null
        fi
    fi
}

function setup_and_enable_venv()
{
    # Check if virtual environment directory exists, create if not
    if [[ ! -d "$VENV_DIR" ]]; then
        ECHO "Creating Virtual environment ${YELLOW}'$VENV_NAME'${NO_COLOR}"
        python3 -m venv "$VENV_DIR"
        ECHO "Virtual environment created."
    fi
    
    # Request that the user re-run this script from the vendor
    # Check if already inside a virtual environment
    if [[ ! -n "$VIRTUAL_ENV" ]]; then
        # Activate virtual environment
        if [ $QUIET == 0 ]; then
            ECHO "please run the following to ensure setup:"
            ECHO ${YELLOW}". ./activate_venv.sh"${NO_COLOR}
        fi
        exit 0  # Exit the function if already in a venv
    fi
    
    if [ -f "${VENV_DIR}/.installed" ]; then
        return
    fi
    
    # Upgrade pip
    #python3 -m pip install --upgrade pip
    #echo "pip upgraded within the virtual environment."
    
    touch ${VENV_DIR}/.installed
}

function create_symlinks_for_docs() 
{
    local base_dir="external_content"
    local target_subpath="docs/pages"

    # Iterate over all subdirectories in external_content/
    for dir in "$base_dir"/*/; do
        local folder_name
        folder_name=$(basename "$dir")

        local search_path="$dir$target_subpath"
        if [ -d "$search_path" ]; then
            find "$search_path" -maxdepth 1 -name "*.md" | while read -r md_file; do
                # Skip symlinks
                if [ -L "$md_file" ]; then
                    echo "Skipping symlink: $md_file"
                    continue
                fi

                # Ensure it's a regular file
                if [ ! -f "$md_file" ]; then
                    echo "Skipping non-regular file: $md_file"
                    continue
                fi

                # Get the file name only
                local md_filename
                md_filename=$(basename "$md_file")

                # Relative symlink target (omit external_content/)
                local relative_target="${folder_name}/$target_subpath/$md_filename"

                # Symlink path in external_content/
                local symlink_path="$base_dir/$md_filename"

                # Create the symlink if it doesn't exist
                if [ -L "$symlink_path" ] || [ -e "$symlink_path" ]; then
                    echo "Skipping: $symlink_path already exists."
                else
                    ln -s "$relative_target" "$symlink_path"
                    echo "Created symlink: $symlink_path -> $relative_target"
                fi
            done
        else
            echo "No $target_subpath found in $dir"
        fi
    done
}

QUIET=0
if [ "$1" == "--quiet" ]; then
    QUIET=1
fi

## Setup and start venv
setup_and_enable_venv

### Clone required repos ###
# Clone ut-core docs for reference material
mkdir -p ${EXTERNAL_CONTENT_DIR}
#clone_repo "https://github.com/rdkcentral/ut-core.wiki.git" "${EXTERNAL_CONTENT_DIR}/ut-core-wiki" "main"
clone_repo "https://github.com/rdkcentral/rdk-halif-rmf_audio_capture.git" "${EXTERNAL_CONTENT_DIR}/rmf_audio_capture" "main"
clone_repo "https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture.git" "${EXTERNAL_CONTENT_DIR}/rmf_audio_capture_test" "main"

# create symlinks for markdown files inside external_content
create_symlinks_for_docs

# Setup Pip Env
install_pip_requirements ${DOCS_DIR}/requirements.txt

## Install your own sub git repo's in here as required
#
if [ ${QUIET} == 0 ]; then
    INFO "Run "${YELLOW}./build_docs.sh${NO_COLOR}" to generate the documentation"
fi


