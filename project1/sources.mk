# Makefile for project 1
# Included based on PROJECT variable passed into top level makefile

# Basic definitions
PROJECT_NAME = project1
PROJECT_OUTPUT_NAME = project
PROJECT_OUTPUT_NAME_TEST = project_test
PROJ_DIR = $(TOP_LEVEL)/$(PROJECT_NAME)
PROJ_OUT_DIR = $(OUT_DIR)/$(PROJECT_NAME)
PROJ_SYM_LINK = $(TOP_LEVEL)/$(PROJECT_OUTPUT_NAME)

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
build: compile-all
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $(addprefix $(PROJ_OUT_DIR)/, $(OBJECTS)) \
		 -o $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME)
	$(Q)ln -sf $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME) $(PROJ_SYM_LINK)
	@tput setaf 2
	@echo "All files built and linked. Output: $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME)"
	@echo "Map file output: $(MAP_FILE)"
	@tput setaf 7
	@echo -e "Program size is "
	$(Q)$(SIZE) $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME)
	@tput setaf 3
	@echo "You may also run ./project to run this project (only on target)"
	@tput setaf 7

compile-all: $(OBJECTS)

%.o: $(PROJ_DIR)/src/%.c setup
	$(Q)$(CC) $(CFLAGS) -c $< -o $(PROJ_OUT_DIR)/$@
	@echo "Build of $@ complete. Output: $(PROJ_OUT_DIR)/$@"

%.S: $(PROJ_DIR)/src/%.c setup
	$(Q)$(CC) $(CFLAGS) -S $< -o $(PROJ_OUT_DIR)/$@
	@echo "Build of $@ complete. Output: $(PROJ_OUT_DIR)/$@"

%.asm: $(PROJ_DIR)/src/%.c setup
	$(Q)$(CC) $(CFLAGS) -S $< -o $(PROJ_OUT_DIR)/$@
	@echo "Build of $@ complete. Output: $(PROJ_OUT_DIR)/$@"

upload: build
	@tput setaf 4
	@echo "Uploading $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME) to device"
	@tput setaf 7
	$(Q)ssh $(SCP_LOGIN) mkdir -p $(SCP_DIR)
	$(Q)scp $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME) $(SCP_LOGIN):$(SCP_DIR)
	@tput setaf 2
	@echo "Upload complete! You may run your program at $(SCP_DIR)/$(PROJECT_OUTPUT_NAME)"
	@tput setaf 7

test: $(TEST_OBJECTS)
	$(Q)$(CC) $(LDFLAGS) $(CFLAGS) $(addprefix $(PROJ_OUT_DIR)/, $(TEST_OBJECTS)) \
		 -o $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME_TEST)
	@tput setaf 2
	@echo "Test files built and linked. Output: $(PROJ_OUT_DIR)/$(PROJECT_OUTPUT_NAME_TEST)"
	@tput setaf 7
	@echo "Be sure to run this on the device and not on a host computer"

# Used in conjunction with clean and clean-all, from top level Makefile
proj-clean:
	rm -rf $(PROJ_SYM_LINK)
