# Makefile for project 1
# Included based on PROJECT variable passed into top level makefile

# Basic definitions
PROJECT_NAME = project1
PROJECT_OUTPUT_NAME = project
PROJECT_OUTPUT_NAME_TEST = project_test
PROJ_DIR = $(TOP_LEVEL)/$(PROJECT_NAME)
PROJ_OUT_DIR = $(OUT_DIR)/$(PROJECT_NAME)
MAP_FILE = $(PROJ_OUT_DIR)/output.map

# Compilation specific flags
CFLAGS += -I$(PROJ_DIR)/inc/
CFLAGS += -fno-builtin -fno-builtin-memcpy -fno-builtin-memmove

LDFLAGS += -Xlinker -Map -Xlinker $(MAP_FILE)

# Build files
OBJECTS = memory.o project_1.o main.o
TEST_OBJECTS = memory.o project_1.o test_memory.o

# Debug flags
ifeq ($(BUILD), RELEASE)
	# Leave room for future optimization levels
else
	# Debug build. Default
	CFLAGS += -Wall -g -O0
endif

# Default BBB logins
ifndef SCP_LOGIN
	SCP_LOGIN = root@192.168.7.2
endif

ifndef SCP_DIR
	SCP_DIR = /home/debian/bin
endif

# Make targets
compile-all: $(OBJECTS)

%.o: $(PROJ_DIR)/src/%.c setup
	$(Q)$(CC) $(CFLAGS) -c $< -o $(PROJ_OUT_DIR)/$@
	$(Q)echo "Build of $@ complete. Output: $(PROJ_OUT_DIR)/$@"

%.S: $(PROJ_DIR)/src/%.c setup
	$(Q)$(CC) $(CFLAGS) -S $< -o $(PROJ_OUT_DIR)/$@
	$(Q)echo "Build of $@ complete. Output: $(PROJ_OUT_DIR)/$@"

%.asm: $(PROJ_DIR)/src/%.c setup
	$(Q)$(CC) $(CFLAGS) -S $< -o $(PROJ_OUT_DIR)/$@
	$(Q)echo "Build of $@ complete. Output: $(PROJ_OUT_DIR)/$@"

build: compile-all
	$(Q)tput setaf 4
	$(Q)echo "Building and linking all files"
	$(Q)tput setaf 7
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $(addprefix $(PROJ_OUT_DIR)/, $(OBJECTS)) \
		 -o $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME)
	$(Q)tput setaf 2
	$(Q)echo "All files built and linked. Output: $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME)"
	$(Q)echo "Map file output: $(MAP_FILE)"
	$(Q)tput setaf 7
	$(Q)echo -e "Program size is "
	$(Q)$(SIZE) $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME)

upload: build
	$(Q)tput setaf 4
	$(Q)echo "Uploading $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME) to device"
	$(Q)tput setaf 7
	$(Q)ssh $(SCP_LOGIN) mkdir -p $(SCP_DIR)
	$(Q)scp $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME) $(SCP_LOGIN):$(SCP_DIR)
	$(Q)tput setaf 2
	$(Q)echo "Upload complete! You may run your program at $(SCP_DIR)/$(PROJECT_OUTPUT_NAME)"
	$(Q)tput setaf 7

test: $(TEST_OBJECTS)
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $(addprefix $(PROJ_OUT_DIR)/, $(TEST_OBJECTS)) \
		 -o $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME_TEST)
	$(Q)tput setaf 2
	$(Q)echo "Test files built and linked. Output: $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME_TEST)"
	$(Q)tput setaf 7
	$(Q)echo "Be sure to run this on the device and not on a host computer"
