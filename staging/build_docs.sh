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

cd ..

# ----------------------------------------------------------------------------
# Function to enhance echo
# ----------------------------------------------------------------------------
function ECHO()
{
    echo -e "$*"
}

# ----------------------------------------------------------------------------
# Function to clone a repository if it doesn't have a.done file
# ----------------------------------------------------------------------------
function clone_repo
{
  repo_url="$1"
  target_dir="$2"
  done_file="$target_dir/.done"

  # Check if directory exists (using -d)
  if [ ! -d "$target_dir" ]; then
    mkdir -p "$target_dir"
  fi

  # Check if.done file exists (using -f)
  if [ ! -f "$done_file" ]; then
    echo "Cloning $repo_url to $target_dir"
    git clone "$repo_url" "$target_dir"
    touch "$done_file"  # Create.done file
  else
    echo "$target_dir already cloned. Skipping."
  fi
}

# ----------------------------------------------------------------------------
# Clone repositories (call the function directly)
# ----------------------------------------------------------------------------
#clone_repo "https://github.com/rdkcentral/rdk-halif-rmf_audio_capture.git" "docs/external_content/rmf_audio_capture"
#clone_repo "https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture.git" "docs/external_content/rmf_audio_capture_test"
#clone_repo "https://github.com/rdkcentral/L4-vendor_system_tests/wiki" "docs/external_content/L4-vendor_system_tests-wiki"
#clone_repo "https://github.com/rdkcentral/another-repo.git" "docs/external_content/another-repo"  # Example
#... add more repo clones like this...

# ----------------------------------------------------------------------------
# usage() : Print the help/usage text.
# ----------------------------------------------------------------------------
function usage() 
{
  cat <<EOF
Usage: $0 [COMMAND] [OPTIONS]

Commands:
  serve        Serve the site locally (mkdocs serve).
  build        Build the site into the "site" directory (mkdocs build).
  deploy       Deploy the site to the "gh-pages" branch (mkdocs gh-deploy).
  help         Show this help message.

Options:
  -h, --help   Show this help message.

Examples:
  $0 serve          Serve your MkDocs site on localhost:8000
  $0 build          Build your MkDocs site into the 'site' directory
  $0 deploy         <version> - Deploy your MkDocs site to GitHub Pages
  $0 set-default    <version> - Set the default version of the deployed docs
  $0 delete         <version> - Delete a version of your MK Docs
  $0 help           Show this help message
EOF
}

# ----------------------------------------------------------------------------
# main() : Main entry point. Handle command-line arguments, then run commands.
# ----------------------------------------------------------------------------
function main() 
{
  local CMD=$1
  shift || true  # Shift off the first argument to allow further options

  case "${CMD}" in
    serve)
      #${PWD}/docs/scripts/sync_src.sh  --quiet
      echo "[INFO] Serving MkDocs locally..."
      mkdocs serve "$@"
      ;;

    build)
      ${PWD}/docs/scripts/sync_src.sh  --quiet
      echo "[INFO] Building MkDocs site..."
      mkdocs build 
      ;;

    deploy)
      echo "[INFO] Deploying MkDocs site to gh-pages..."
      #${PWD}/docs/scripts/sync_src.sh --quiet
      # Check if the second argument (the version) is provided
      # "$#" is the number of positional parameters
      # If "$2" is empty, it means no version was provided after "deploy"
      VERSION_TO_DEPLOY="$1"      
      echo VERSION:[$VERSION_TO_DEPLOY] @:{$@}
      if [ -z "$VERSION_TO_DEPLOY" ]; then
          echo "[ERROR] Missing version argument for 'deploy'. Usage: $0 deploy <version> [alias...]"
          exit 1 # Exit with an error code
      fi
      # Extract the version from the arguments
      mike deploy "${VERSION_TO_DEPLOY}" --push
      mike set-default ${VERSION_TO_DEPLOY} --push
      ;;

    set-default)
      echo "[INFO] Deploying MkDocs site to gh-pages..."
      # Check if the second argument (the version) is provided
      # "$#" is the number of positional parameters
      # If "$2" is empty, it means no version was provided after "deploy"
      VERSION_TO_DEPLOY="$1"      
      echo VERSION:[$VERSION_TO_DEPLOY] @:{$@}
      if [ -z "$VERSION_TO_DEPLOY" ]; then
          echo "[ERROR] Missing version argument for 'set-default'. Usage: $0 set-default <version> [alias...]"
          exit 1 # Exit with an error code
      fi
      # Extract the version from the arguments
      mike set-default ${VERSION_TO_DEPLOY} --push
      ;;

    delete)
      echo "[INFO] Delelting <$1> in gh-pages..."
      # Check if the second argument (the version) is provided
      # "$#" is the number of positional parameters
      # If "$2" is empty, it means no version was provided after "deploy"
      VERSION_TO_DEPLOY="$1"      
      echo VERSION:[$VERSION_TO_DEPLOY] @:{$@}
      if [ -z "$VERSION_TO_DEPLOY" ]; then
          echo "[ERROR] Missing version argument for 'delete'. Usage: $0 delete <version> [alias...]"
          exit 1 # Exit with an error code
      fi
      # Extract the version from the arguments
      mike delete ${VERSION_TO_DEPLOY} --push
      ;;

    help|-h|--h|--help|"")
      # If user typed 'help', '-h', '--help', or gave no args -> show usage
      usage
      ;;

    *)
      echo "[ERROR] Unknown command: '${CMD}'"
      usage
      exit 1
      ;;
  esac
}

# ----------------------------------------------------------------------------
# Setup and run the install and the venv
# ----------------------------------------------------------------------------
{
  cd ./docs
  ${PWD}/scripts/install.sh --quiet
  source ${PWD}/scripts/activate_venv.sh
}

# ----------------------------------------------------------------------------
# Run main() with all script arguments.
# ----------------------------------------------------------------------------
cd ..
main "$@"

# ----------------------------------------------------------------------------
# deactivate pyenv
# ----------------------------------------------------------------------------
deactivate
