TARGET = mono_project
ARCH="/usr/local/openmono/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-"
FLASH_SIZE=262144
FLASH_ROW_SIZE=256
FLASH_ARRAY_SIZE=65536
EE_ARRAY=64
EE_ROW_SIZE=16
OPTIMIZATION = -Os
BUILD_DIR=build
MONO_FRAMEWORK_PATH=.
PSOC5_PATH=../mono_psoc5_library
MBED_PATH=../mbedcomp
MBED_FS_PATH=../mbed/libraries/fs
COMP_LIB=$(PSOC5_PATH)/lib/CyComponentLibrary.a
INCLUDE_DIR=$(PSOC5_PATH)/include
CYPRESS_LIB=$(PSOC5_PATH)/lib/monoCyLib.a
MBED_LIB=../mbedcomp/mbedlib.a
PACKAGE_TARGET=../dist

# OBJECTS =		$(patsubst %.c,%.o,$(wildcard *.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard *.cpp))

# MBED_OBJECTS =	$(patsubst %.cpp,%.o,$(wildcard $(MBED_PATH)/*.cpp)) \
# 				$(patsubst %.c,%.o,$(wildcard $(MBED_PATH)/common/*.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MBED_PATH)/common/*.cpp)) \
# 				$(patsubst %.c,%.o,$(wildcard $(MBED_PATH)/target_cypress/*.c))
#
MBED_INCLUDES_REL =	api \
					hal \
					target_cypress \
					$(MBED_FS_PATH)/fat \
					$(MBED_FS_PATH)/fat/ChaN \
					$(MBED_FS_PATH)/sd

MBED_INCLUDES = $(foreach PATH, $(MBED_INCLUDES_REL), $(MBED_PATH)/$(PATH))

MONO_OBJECTS =	$(patsubst %.c,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ui/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ili9225g/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/wireless/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/media/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/io/*.cpp))

MONO_INCLUDES_REL = . \
					display \
					display/ili9225g \
					display/ui \
					io \
					wireless \
					media

MONO_INCLUDES =	$(foreach PATH, $(MONO_INCLUDES_REL), $(MONO_FRAMEWORK_PATH)/$(PATH))

MONO_TARGET_OBJECTS = $(addprefix ./$(BUILD_DIR)/, $(MONO_OBJECTS))

MONO_INCLUDE_FILES = $(foreach FILE,$(MONO_INCLUDES),$(wildcard $(FILE)/*.h))

CYLIB_INCLUDE_FILES  = $(foreach FILE,$(INCLUDE_DIR),$(wildcard $(FILE)/*.h))
CYLIB_INCLUDE_FILES += $(foreach FILE,$(INCLUDE_DIR),$(wildcard $(FILE)/*.ld))

# SYS_OBJECTS = 	$(patsubst %.c,%.o,$(wildcard $(CYPRESS_DIR)/*.c)) \
# 				$(patsubst %.s,%.o,$(wildcard $(CYPRESS_DIR)/*Gnu.s))

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
CDEFS= #-DMONO_DISP_CTRL_HX8340
ASDEFS=
AS_FLAGS = -c -g -Wall -mcpu=cortex-m3 -mthumb -mthumb-interwork -march=armv7-m
CC_FLAGS = -c -g -Wall -mcpu=cortex-m3 -mthumb $(OPTIMIZATION) -mthumb-interwork -fno-common -fmessage-length=0 -ffunction-sections -fdata-sections -march=armv7-m
ONLY_C_FLAGS = -std=gnu99
ONLY_CPP_FLAGS = -std=gnu++98 -fno-rtti -fno-exceptions
LDSCRIPT = -T $(LINKER_SCRIPT)
LD_FLAGS = -g -mcpu=cortex-m3 -mthumb -march=armv7-m -fno-rtti -Wl,--gc-sections -specs=nano.specs
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

#"libs/CyCompLib.a"
#   -mfix-cortex-m3-ldrd -u _printf_float -u _scanf_float
COPY_FLAGS = -j .text -j .eh_frame -j .rodata -j .ramvectors -j .noinit -j .data -j .bss -j .stack -j .heap -j .cyloadablemeta

define \n


endef

all: monolib.a

release: mono_framework.a

#parallel: hx8340 monolib.a

#hx8340: CDEFS+=-DMONO_DISP_CTRL_HX8340 MONO_OBJECTS-= $(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ili9225g/*.cpp)) MONO_OBJECTS+= $(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/hx8340/*.cpp))

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p ./$(BUILD_DIR)

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

monolib.a: $(MONO_TARGET_OBJECTS)
	@echo "Linking Mono Framework..."
	@$(AR) rcs $@ $^
	@echo "Copying linker and header files to include dir"
	@$(MKDIR) -p include
	@$(COPY) $(MONO_INCLUDE_FILES) include/.

mono_framework.a: cypressLib mbedLib $(MONO_TARGET_OBJECTS)
	@echo "Linking Mono Framework Release..."
	@$(AR) rcs $@ $(MONO_TARGET_OBJECTS) #$^
	@echo "Copying linker and header files to include dir"
	@$(foreach PATH, $(MONO_INCLUDES_REL), $(MKDIR) -p include/$(PATH)$(\n))
	@$(foreach PATH, $(MONO_INCLUDES_REL), $(COPY) -r $(MONO_FRAMEWORK_PATH)/$(PATH)/*.h include/$(PATH)$(\n))
	@$(foreach PATH, $(MBED_INCLUDES_REL), $(MKDIR) -p include/mbed/$(PATH)$(\n))
	@$(foreach PATH, $(MBED_INCLUDES_REL), $(COPY) -r $(MBED_PATH)/$(PATH)/*.h include/mbed/$(PATH)$(\n))
	@$(COPY) $(CYLIB_INCLUDE_FILES) include/mbed/target_cypress
	@echo "Copying to project_template folder..."
	$(MKDIR) -p $(PACKAGE_TARGET)/mono/include
	$(COPY) mono_framework.a $(PACKAGE_TARGET)/mono
	$(COPY) $(COMP_LIB) $(PACKAGE_TARGET)/mono
	$(COPY) $(CYPRESS_LIB) $(PACKAGE_TARGET)/mono
	$(COPY) $(MBED_LIB) $(PACKAGE_TARGET)/mono
	$(COPY) -r include/. $(PACKAGE_TARGET)/mono/include

cypressLib:
	@echo "Building Cypress library..."
	@make -C $(PSOC5_PATH) library

mbedLib:
	@echo "Building mbed library..."
	@make -C $(MBED_PATH) mbed

monolib2.a: $(MONO_OBJECTS)
	@echo "Linking Mono Framework..."
	@$(AR) rcs $@ $(addprefix $(BUILD_DIR)/, $^)
	@echo "Copying linker and header files to include dir"
	@$(MKDIR) -p include
	@$(COPY) $(MONO_INCLUDE_FILES) include/.

monoFiles:
	@echo $(MONO_TARGET_OBJECTS)

monoIncludes:
	@echo $(MONO_INCLUDE_FILES)

includeFiles:
	@echo $(INCS)

clean:
	$(RM) -r $(addprefix $(BUILD_DIR)/, $(MONO_OBJECTS)) include/* monolib.a mono_framework.a



## $(LD) -Wl,--start-group $(LD_FLAGS) libs/CyCompLib.a $(LDSCRIPT) -o $@ $^ -Wl,--end-group $(LD_SYS_LIBS)
## $(ELFTOOL) -C $@ --flash_size $(FLASH_SIZE) --flash_row_size $(FLASH_ROW_SIZE)