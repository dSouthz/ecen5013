ARCH = $(shell uname -m)

# Student's note: I have two Beaglebone Blacks
# One is revision A5B, which does not support gnueabihf,
# and the other is a revision C, which does support gnueabihf
#
# Default to gnueabihf
#
# This can also be expanded for other devices as the class pogresses
ifeq ($(DEVICE), BBB-A5B)
	ARM_XCC = arm-linux-gnueabi
else
	ARM_XCC := arm-linux-gnueabihf
endif

## Environment setup ##
TOP_LEVEL := $(abspath $(shell pwd))
OUT_DIR	  := $(TOP_LEVEL)/out

# Define build binaries
ifeq ($(ARCH), x86_64)
	# Compiling on Host
	SIZE 	= $(ARM_XCC)-size
	OBJDUMP = $(ARM_XCC)-objdump
	CC 		= $(ARM_XCC)-gcc
	GPP 	= $(ARM_XCC)-g++
	AR		= $(ARM_XCC)-ar
	LD		= $(ARM_XCC)-ld
	READELF = $(ARM_XCC)-readelf
else
	# Compiling on device - use built in names
	SIZE 	= size
	OBJDUMP = objdump
	CC 		= gcc
	GPP 	= g++
	AR		= ar
	LD		= ld
	READELF = readelf
endif

# Enables/suppresses verbose command output
ifdef VERBOSE
	Q=
else
	export Q=@
endif

# Flags common to all projects
CFLAGS= -std=c99
LDFLAGS=

# Include project specific makefiles
ifdef PROJECT
	include $(TOP_LEVEL)/$(PROJECT)/sources.mk
else
	# Default project - will change as course progresses
	PROJECT = project1
	include $(TOP_LEVEL)/$(PROJECT)/sources.mk
endif

# Targets for all projects
.PHONY: setup clean clean-all

setup:
	$(Q)mkdir -p $(OUT_DIR)
	$(Q)mkdir -p $(PROJ_OUT_DIR)

clean: proj-clean
	rm -rf $(PROJ_OUT_DIR)

clean-all: clean
	rm -rf $(OUT_DIR)
