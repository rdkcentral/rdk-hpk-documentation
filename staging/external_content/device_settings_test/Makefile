#* ******************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2023 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BIN_DIR := $(ROOT_DIR)/bin
TOP_DIR := $(ROOT_DIR)

RED:='\033[0;31m'
GREEN:='\033[0;32m'
YELLOW:='\033[0;33m'
NC:='\033[0m'
ECHOE = /bin/echo -e

SRC_DIRS = $(ROOT_DIR)/src
INC_DIRS := $(ROOT_DIR)/../include
INC_DIRS += $(ROOT_DIR)/profiles/include
HAL_LIB := dshal
SKELETON_SRCS := $(ROOT_DIR)/skeletons/src/*

# Check if TARGET is unset
ifeq ($(TARGET),)
    $(info TARGET NOT SET )
    $(info TARGET FORCED TO linux)
    TARGET = linux
endif

# Handle specific TARGET values
ifeq ($(TARGET), linux)
    SRC_DIRS += $(ROOT_DIR)/skeletons/src
    CC := gcc -ggdb -o0 -Wall
endif


$(info TARGET [$(TARGET)])

ifeq ($(TARGET),arm)
# Build and link a skeleton library which can be overriden by the real one
HAL_LIB_DIR := $(ROOT_DIR)/libs
YLDFLAGS = -Wl,-rpath,$(HAL_LIB_DIR) -L$(HAL_LIB_DIR) -l$(HAL_LIB)
ifeq ("$(wildcard $(HAL_LIB_DIR)/lib$(HAL_LIB).so)","")
SETUP_SKELETON_LIBS := skeleton
endif
endif

.PHONY: clean list all

export YLDFLAGS
export BIN_DIR
export SRC_DIRS
export INC_DIRS
export TARGET
export TOP_DIR
export HAL_LIB_DIR

.PHONY: clean list build cleanlibs clean cleanall skeleton

build: $(SETUP_SKELETON_LIBS)
	@echo UT [$@]
	make -C ./ut-core TARGET=${TARGET}
	rm -rf $(BIN_DIR)/lib$(HAL_LIB).so
	rm -rf $(ROOT_DIR)/libs/lib$(HAL_LIB).so

#Build against the real library leads to the SOC library dependency also.SOC lib dependency cannot be specified in the ut Makefile, since it is supposed to be common across may platforms. So in order to over come this situation, creating a template SKELETON library with empty templates so that the template library wont have any other Soc dependency. And in the real platform mount copy bind with the actual library will work fine.
skeleton:
	@echo Skeleton Building [$@]
	mkdir -p $(HAL_LIB_DIR)
	$(CC) -fPIC -shared -I$(ROOT_DIR)/../include $(SKELETON_SRCS) -o $(HAL_LIB_DIR)/lib$(HAL_LIB).so

list:
	@${ECHOE} --------- ut - list ----------------
	@${ECHOE}
	@${ECHOE} ${YELLOW}CC:${NC} $(CC)
	@${ECHOE}
	@${ECHOE} ${YELLOW}TOP_DIR:${NC} $(TOP_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}BIN_DIR:${NC} $(BIN_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}HAL_LIB_DIR:${NC} $(HAL_LIB_DIR)
	@${ECHOE}
	@${ECHOE} ${YELLOW}YLDFLAGS:${NC} $(YLDFLAGS)
	@${ECHOE}
	@${ECHOE} ${YELLOW}TARGET:${NC} $(TARGET)
	@${ECHOE}
	make -C ./ut-core TARGET=${TARGET} list

printenv:
	@${ECHOE} ${YELLOW}"Environment variables: [UT]"${NC}
	@${ECHOE} ${YELLOW}"---------------------------"${NC}
	@$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
	@${ECHOE} ${YELLOW}"---------------------------"${NC}
	make -C ./ut-core TARGET=${TARGET} printenv

cleanlibs:
	rm -rf $(BIN_DIR)/lib$(HAL_LIB).so
	rm -rf $(HAL_LIB_DIR)/libs/lib$(HAL_LIB).so

clean: cleanlibs
	@echo clean [$@]
	make -C ./ut-core TARGET=${TARGET} clean

cleanall: cleanlibs
	@echo cleanall [$@]
	make -C ./ut-core TARGET=${TARGET} cleanall
