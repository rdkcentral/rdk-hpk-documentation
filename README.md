# HPK Public Documentation

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Description](#description)
- [Hardware Porting Kit](#hardware-porting-kit)
- [How to build the test suite](#how-to-build-the-test-suite)
- [Running the test suite on the target](#running-the-test-suite-on-the-target)
- [Release History](#release-history)

## Acronyms, Terms and Abbreviations

- `HPK` - Hardware Porting Kit
- `HAL` - Hardware Abstraction Layer
- `API` - Application Programming Interface

## Description

This is the public documentation to support the Hardware Porting Kit (`HPK`), for any vendor / third party to read. This document will be the consolidating jump point for all documentation with links to other open documentations.

## Hardware Porting Kit

The Hardware Porting Kit (`HPK`) is a collection resources that simplify the process of adapting existing software to run on a new hardware platform. It provides a framework for making the necessary changes to the software to account for the differences between the hardware vendors.

It will provide both Hardware Abstraction Layer (`API`) Header files, and software tests to ensure compatibility with the RDK Middleware Stack.

## How to build the test suite

In order to build the test suits, there is a requirement to supply the toolchain, either as a vendor independent toolchain, an SDK, or an RDK toolchain.

- export the toolchain path, e.g.:

```bash
export TOOLCHAIN_PATH=/opt/toolchains/rdk-glibc-x86_64/sysroots/armv7at2hf-neon-rdk-linux-gnueabi
export CC="arm-rdk-linux-gnueabi-gcc  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard --sysroot=$TOOLCHAIN_PATH"
```

- Clone the `hal` repo, e.g.:

```bash
git clone git@github.com:rdkcentral/rdk-halif-hdmi_cec.git
```

- There are two targets for the platform

  1. linux - (default)
  2. arm - TARGET=arm

```bash
build_ut.sh TARGET=arm
```

- `build_ut.sh` script will clone the hal-test repo, ut-core and generate a `hal_test` binary in `ut/bin/` upon successful completion.

- Installing the Python Environment for L3 and above tests
  - Goto `ut/host` folder. This folder contains the script files required for installing python
  - Run script files to download the python environment
  - For more details refer [Guide-for-Setting-up-the-Python-RAFT-Testing-Suite](https://github.com/rdkcentral/ut-raft/wiki/Guide-for-Setting-up-the-Python-RAFT-Testing-Suite)

```bash
./install.sh
source ./activate_venv.sh
./install.sh
```

## Running the test suite on the target

Copy files from `bin/*` to the target.

Log into the target.

Assuming the files that are copied are in the `/home/root` directory, then the following export is required.

- All external library dependencies are copied into the `ut/bin` directory

```bash
export LD_LIBRARY_PATH=/usr/lib:/lib:/home/root:./
```

or use the `run.sh`, which is in the same directory

Now the hal test can be executed, `-h` for help is supported.

```bash
./hal_test  -h
Help
-c - Console Mode (Default)
-a - Automated Mode
-b - Basic Mode
-t - List all tests run to a file
-l - Set the log Path
-p - <profile_filename> - specify the profile to load YAML or JSON, also used by kvp_assert
-h - Help
```

### Modes of operation

1. Console Mode - will open the interactive console
2. Automated Mode - will output in xUnit form as a .xml file
3. Basic Mode - All tests will be ran and the output redirected to the shell

## Release History

The latest release information is available [here](./RELEASE.md "Release History").
