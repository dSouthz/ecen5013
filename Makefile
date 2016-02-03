ARCH = $(shell uname -m)
#TODO: change this to hf when we get new BBB....
ARM_XCC := arm-linux-gnueabi

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

# Define warning, optimization, debug options

#TODO:
.PHONY: test mysetup clean

test:
	echo "Test"

setup:
	@echo "setup"
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)


#TODO: clean up and generic-ize
include project1/sources.mk


#ifndef PROJECT
#	#Default target
#	PROJECT = project1
#	include project1/sources.mk
#else
#	include $(PROJECT)/sources.mk
#endif
