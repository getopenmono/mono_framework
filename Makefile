RELEASE_DIR=dist
ARCH="arm-none-eabi-"

FLASH_SIZE=262144
FLASH_ROW_SIZE=256
FLASH_ARRAY_SIZE=65536
EE_ARRAY=64
EE_ROW_SIZE=16
OPTIMIZATION = -Os
BUILD_DIR=build
FRAMEWORK_PATH=src
PSOC5_PATH=src/cypress
MBED_PATH=src/mbedcomp
MBED_FS_PATH=libraries/fs
COMP_LIB=$(PSOC5_PATH)/lib/CyComponentLibrary.a
INCLUDE_DIR=$(PSOC5_PATH)/psoc5
CYPRESS_LIB=$(BUILD_DIR)/monoCyLib.a

MBED_INCLUDES_REL =	api \
					hal \
					target_cypress \
					$(MBED_FS_PATH)/fat \
					$(MBED_FS_PATH)/fat/ChaN \
					$(MBED_FS_PATH)/sd

MBED_INCLUDES = $(foreach PATH, $(MBED_INCLUDES_REL), $(MBED_PATH)/$(PATH))


MONO_OBJECTS =	$(patsubst %.c,%.o,$(wildcard $(FRAMEWORK_PATH)/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/display/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/display/ui/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/display/ili9225g/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/wireless/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/media/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/io/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/sensors/*.cpp))

MONO_INCLUDES_REL = . \
					display \
					display/ili9225g \
					display/ui \
					display/Fonts \
					io \
					wireless \
					media \
					sensors

MONO_INCLUDES =	$(foreach PATH, $(MONO_INCLUDES_REL), $(FRAMEWORK_PATH)/$(PATH))

MONO_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/, $(MONO_OBJECTS))

MONO_INCLUDE_FILES = $(foreach FILE,$(MONO_INCLUDES),$(wildcard $(FILE)/*.h))

CYLIB_INCLUDE_FILES  = $(foreach FILE,$(INCLUDE_DIR),$(wildcard $(FILE)/*.h))
CYLIB_INCLUDE_FILES += $(foreach FILE,$(INCLUDE_DIR),$(wildcard $(FILE)/*.ld))

MBED_LIB=$(BUILD_DIR)/mbedlib.a
MONO_LIB=$(BUILD_DIR)/monolib.a
MONO_FRAMEWORK=$(BUILD_DIR)/mono_framework.a

ifeq ($(OS),Windows_NT)
	RM = cmd //C del //Q //F
	RRM = cmd //C rmdir //Q //S
else
	RM = rm -f
	RRM = rm -f -r
endif
CC=$(ARCH)gcc
CXX=$(ARCH)g++
LD=$(ARCH)gcc
AS=$(ARCH)gcc
AR=$(ARCH)ar
RANLIB=$(ARCH)ranlib
STRIP=$(ARCH)strip
OBJCOPY=$(ARCH)objcopy
OBJDUMP=$(ARCH)objdump
COPY=cp
MKDIR=mkdir
MONOPROG=monoprog
ELFTOOL='C:\Program Files (x86)\Cypress\PSoC Creator\3.1\PSoC Creator\bin\cyelftool.exe'
INCS = -I . $(addprefix -I, $(MONO_INCLUDES) $(MBED_INCLUDES) $(INCLUDE_DIR))
CDEFS=
ASDEFS=
AS_FLAGS = -c -g -Wall -mcpu=cortex-m3 -mthumb -mthumb-interwork -march=armv7-m
CC_FLAGS = -c -g -Wall -mcpu=cortex-m3 -mthumb $(OPTIMIZATION) -mthumb-interwork -fno-common -fmessage-length=0 -ffunction-sections -fdata-sections -march=armv7-m
ONLY_C_FLAGS = -std=gnu99
ONLY_CPP_FLAGS = -std=gnu++98 -fno-rtti -fno-exceptions
LDSCRIPT = -T $(LINKER_SCRIPT)
LD_FLAGS = -g -mcpu=cortex-m3 -mthumb -march=armv7-m -fno-rtti -Wl,--gc-sections -specs=nano.specs
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys
COPY_FLAGS = -j .text -j .eh_frame -j .rodata -j .ramvectors -j .noinit -j .data -j .bss -j .stack -j .heap -j .cyloadablemeta

# Makro for using newlines in rules.
define \n


endef

.PHONY: all
all: unittests $(MONO_FRAMEWORK)

.PHONY: release
release: all
	# @echo "-- ! --"
	# @echo "Icons must be converted to header files, remember to set that up in the build system!"
	# @exit 1
	@echo "Copying to release folder..."
	$(MKDIR) -p $(RELEASE_DIR)/mono/include
	$(COPY) $(MONO_FRAMEWORK) $(RELEASE_DIR)/mono
	$(COPY) $(COMP_LIB) $(RELEASE_DIR)/mono
	$(COPY) $(CYPRESS_LIB) $(RELEASE_DIR)/mono
	$(COPY) $(MBED_LIB) $(RELEASE_DIR)/mono
	$(COPY) -r $(BUILD_DIR)/include/. $(RELEASE_DIR)/mono/include
	@echo "Converting icon files..."
	make -C resources -f icons.mk all

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

.s.o:
	@echo "Assembling: $(notdir $<)"
	@$(AS) $(AS_FLAGS) $(INCS) -o $(BUILD_DIR)/$(notdir $@) $<

.c.o:
	@echo "Compiling C: $(notdir $<)"
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $(BUILD_DIR)/$(notdir $@) $<

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) -Wno-char-subscripts $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(MONO_LIB): $(MONO_TARGET_OBJECTS)
	@echo "Linking Mono Framework..."
	@$(AR) rcs $@ $^

$(MONO_FRAMEWORK): $(CYPRESS_LIB) $(MBED_LIB) $(MONO_TARGET_OBJECTS)
	@echo "Linking Mono Framework Release..."
	@$(AR) rcs $@ $(MONO_TARGET_OBJECTS)
	@echo "Copying linker and header files to include dir"
	@$(foreach PATH, $(MONO_INCLUDES_REL), $(MKDIR) -p $(BUILD_DIR)/include/$(PATH)$(\n))
	@$(foreach PATH, $(MONO_INCLUDES_REL), $(COPY) -r $(FRAMEWORK_PATH)/$(PATH)/*.h $(BUILD_DIR)/include/$(PATH)$(\n))
	$(foreach PATH, $(MBED_INCLUDES_REL), $(MKDIR) -p $(BUILD_DIR)/include/mbed/$(PATH)$(\n))
	$(foreach PATH, $(MBED_INCLUDES_REL), $(COPY) -r $(MBED_PATH)/$(PATH)/*.h $(BUILD_DIR)/include/mbed/$(PATH)$(\n))
	@$(COPY) $(CYLIB_INCLUDE_FILES) $(BUILD_DIR)/include/mbed/target_cypress

$(CYPRESS_LIB):
	@echo "Building Cypress library..."
	@make -C $(PSOC5_PATH)

$(MBED_LIB):
	@echo "Building mbed library..."
	@make -C $(MBED_PATH)

.PHONY: icons
icons:
	@echo "Converting icon files..."
	make -C resources -f icons.mk all

.PHONY: clean
clean:
	$(RRM) $(BUILD_DIR)
	$(RRM) $(RELEASE_DIR)

include $(FRAMEWORK_PATH)/unittests/unittests.mk

# Debugging this Makefile

monoFiles:
	@echo "MONO_TSRGET_OBJECTS --> $(MONO_TARGET_OBJECTS)"

monoIncludes:
	@echo "MONO_INCLUDE_FILES --> $(MONO_INCLUDE_FILES)"

includeFiles:
	@echo "INCS --> $(INCS)"
