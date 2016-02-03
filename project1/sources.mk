# Makefile for project 1
# Included based on PROJECT variable passed into top level makefile

PROJ_DIR = $(TOP_LEVEL)/project1

CFLAGS = -I$(PROJ_DIR)/inc/
CFLAGS += -fno-builtin
CLFAGS += -fno-builtin-memcpy -fno-builtin-memmove

OBJECTS = memory.o main.o project_1.o

compile-all: $(OBJECTS)

%.o: $(PROJ_DIR)/src/%.c setup
	$(CC) $(CFLAGS) -c $< -o $(OUT_DIR)/$@

%.S: $(PROJ_DIR)/src/%.c setup
	$(CC) $(CFLAGS) -S $< -o $(OUT_DIR)/$@

%.asm: $(PROJ_DIR)/src/%.c setup
	$(CC) $(CFLAGS) -S $< -o $(OUT_DIR)/$@

#TODO: provide map file
build: compile-all
	$(CC) $(CFLAGS) $(addprefix $(OUT_DIR)/, $(OBJECTS)) -o $(OUT_DIR)/project1

upload: build
	scp $(OUT_DIR)/project1 root@192.168.7.2:/home/debian/bin
