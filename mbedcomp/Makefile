ARCH="/usr/local/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-"
MBED_FS=./libraries/fs
BUILD_DIR=../build
PSOC5_PATH=../../mono_psoc5_library
INCLUDE_DIR=$(PSOC5_PATH)/include

MBED_PATH=.
BUILD_INCLUDE_DIR=$(BUILD_DIR)/include
MBED_OBJECTS = \
	$(patsubst $(MBED_PATH)/%.cpp,%.o,$(wildcard $(MBED_PATH)/*.cpp)) \
	$(patsubst $(MBED_PATH)/%.c,%.o,$(wildcard $(MBED_PATH)/common/*.c)) \
	$(patsubst $(MBED_PATH)/%.cpp,%.o,$(wildcard $(MBED_PATH)/common/*.cpp)) \
	$(patsubst $(MBED_PATH)/%.c,%.o,$(wildcard $(MBED_PATH)/target_cypress/*.c)) \
	$(patsubst $(MBED_PATH)/%.cpp,%.o,$(wildcard $(MBED_PATH)/libraries/fs/sd/*.cpp)) \
	$(patsubst $(MBED_PATH)/%.cpp,%.o,$(wildcard $(MBED_PATH)/libraries/fs/fat/*.cpp)) \
	$(patsubst $(MBED_PATH)/%.cpp,%.o,$(wildcard $(MBED_PATH)/libraries/fs/fat/ChaN/*.cpp))
MBED_INCLUDE_DIRS =	\
	$(MBED_PATH)/api \
	$(MBED_PATH)/hal \
	$(MBED_PATH)/target_cypress \
	$(MBED_FS)/sd \
	$(MBED_FS)/fat \
	$(MBED_FS)/fat/ChaN
MBED_INCLUDE_FILES = $(foreach FILE,$(MBED_INCLUDE_DIRS),$(wildcard $(FILE)/*.h))
MBED_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/, $(MBED_OBJECTS))
INCS = -I . $(addprefix -I, $(MBED_INCLUDE_DIRS) $(INCLUDE_DIR))

CC=$(ARCH)gcc
CXX=$(ARCH)g++
LD=$(ARCH)gcc
AS=$(ARCH)gcc
AR=$(ARCH)ar
RANLIB=$(ARCH)ranlib
STRIP=$(ARCH)strip
OBJCOPY=$(ARCH)objcopy
OBJDUMP=$(ARCH)objdump
MKDIR=mkdir
COPY=cp
AS_FLAGS = -c -g -Wall -mcpu=cortex-m3 -mthumb -mthumb-interwork -march=armv7-m
CC_FLAGS = -c -g -Wall -mcpu=cortex-m3 -mthumb $(OPTIMIZATION) -mthumb-interwork -fno-common -fmessage-length=0 -ffunction-sections -fdata-sections -march=armv7-m
ONLY_C_FLAGS = -std=gnu99
ONLY_CPP_FLAGS = -std=gnu++98 -fno-rtti -fno-exceptions
LDSCRIPT = -T $(LINKER_SCRIPT)
LD_FLAGS = -g -mcpu=cortex-m3 -mthumb -march=armv7-m -fno-rtti -Wl,--gc-sections -specs=nano.specs
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

all: $(BUILD_DIR) $(BUILD_DIR)/mbedlib.a

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p ./$(BUILD_DIR)

.s.o: $(BUILD_DIR)
	@echo "Assembling: $(notdir $<)"
	@$(AS) $(AS_FLAGS) $(INCS) -o $(BUILD_DIR)/$(notdir $@) $<

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling mbed C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling mbed C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mbedlib.a: $(MBED_TARGET_OBJECTS)
	@echo "Linking mbed framework ..."
	@$(AR) rcs $@ $^

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

# Debugging this Makefile

mbedFiles:
	@echo "MBED_OBJECTS --> $(MBED_OBJECTS)"
	@echo "MBED_TARGET_OBJECTS --> $(MBED_TARGET_OBJECTS)"

mbedIncludes:
	@echo "INCS --> $(INCS)"
	@echo "MBED_INCLUDE_FILES --> $(MBED_INCLUDE_FILES)"

