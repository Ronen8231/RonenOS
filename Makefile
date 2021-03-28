CC=i686-elf-gcc
CFLAGS=-ffreestanding -g -std=gnu99 -Wall -Wextra -O3
#OBJS=start.o kernel.o gdt.o
QEMU= qemu-system-i386
QEMU_FLAGS=-kernel
LDFLAGS=-nostdlib -T linker.ld -lgcc


# all: kernel.elf

# gdt.o: gdt.c start.o
# 	${CC} ${CFLAGS} -c $? -o $@

# start.o: start.s
# 	${CC} ${CFLAGS} -c $? -o $@

# kernel.o: kernel.c
# 	${CC} ${CFLAGS} -c $? -o $@

# kernel.elf: linker.ld ${OBJS}
# 	${CC} ${CFLAGS} -nostdlib -T $< ${OBJS} -o kernel.elf -lgcc

# run: kernel.elf
# 	${QEMU} ${QEMU_FLAGS} $<

# clean:
# 	rm -f $(wildcard *.o *.elf)


TARGET_EXEC := kernel.elf

BUILD_DIR := ./build
SRC_DIRS := ./src ./asm
INC_DIRS := ./inc
# Find all the C and C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
#INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# kernel.elf:	linker.ld ${OBJS}
# 	${CC} ${CFLAGS} -nostdlib -T $< ${OBJS} -o kernel.elf -lgcc



.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
