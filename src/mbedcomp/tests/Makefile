TARGET = unit_tests
ARCH=#"/usr/local/gcc-arm-none-eabi-4_8/bin/arm-none-eabi-"
OPTIMIZATION = -O0

LINKER_SCRIPT=${INCLUDE_DIR}/cm3gcc.ld
BUILD_DIR=build
MONO_FRAMEWORK_PATH =../mono_framework
MBED_PATH=.
MBED_FS=../mbed/libraries/fs
COMP_LIB=../mono_buildsystem/lib/CyComponentLibrary.a
MONO_LIB=../mono_buildsystem/lib/monoCyLib.a

# OBJECTS =		$(patsubst %.c,%.o,$(wildcard *.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard *.cpp))

MBED_OBJECTS =	$(patsubst %.cpp,%.o,$(wildcard $(MBED_PATH)/*.cpp)) \
				$(patsubst %.c,%.o,$(wildcard $(MBED_PATH)/common/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_PATH)/common/*.cpp)) \
				$(patsubst %.c,%.o,$(wildcard $(MBED_PATH)/target_cypress/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_FS)/sd/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_FS)/fat/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_FS)/fat/ChaN/*.cpp))
				
MBED_INCLUDES =	$(MBED_PATH) \
				$(MBED_PATH)/api \
				$(MBED_PATH)/hal \
				$(MBED_PATH)/target_cypress \
				$(MBED_FS)/sd \
				$(MBED_FS)/fat \
				$(MBED_FS)/fat/ChaN

MBED_INCLUDE_FILES = $(foreach FILE,$(MBED_INCLUDES),$(wildcard $(FILE)/*.h))

# MONO_OBJECTS =	$(patsubst %.c,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/*.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/*.cpp)) \
# 				$(patsubst %.c,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/*.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/*.cpp)) \
# 				$(patsubst %.c,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ui/*.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ui/*.cpp)) \
# 				$(patsubst %.c,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ili9225g/*.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/display/ili9225g/*.cpp)) \
# 				$(patsubst %.c,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/wireless/*.c)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/wireless/*.cpp)) \
# 				$(patsubst %.cpp,%.o,$(wildcard $(MONO_FRAMEWORK_PATH)/media/*.cpp))
#
# MONO_INCLUDES =	$(MONO_FRAMEWORK_PATH) \
# 				$(MONO_FRAMEWORK_PATH)/display \
# 				$(MONO_FRAMEWORK_PATH)/display/ili9225g \
# 				$(MONO_FRAMEWORK_PATH)/display/ui \
# 				$(MONO_FRAMEWORK_PATH)/wireless \
# 				$(MONO_FRAMEWORK_PATH)/media
#
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
INCS = -I . -I ${CYPRESS_DIR} -I$(INCLUDE_DIR) $(addprefix -I, $(MBED_INCLUDES) $(MONO_INCLUDES))
CDEFS=
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

all: $(BUILD_DIR) mbed

library: monoCyLib.a

mbed: mbedlib.a

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p ./$(BUILD_DIR)

.s.o: $(BUILD_DIR)
	@echo "Assembling: $(notdir $<)"
	@$(AS) $(AS_FLAGS) $(INCS) -o $(BUILD_DIR)/$(notdir $@) $<

.c.o: $(BUILD_DIR)
	@echo "Compiling C: $(notdir $<)"
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $(BUILD_DIR)/$(notdir $@) $<

.cpp.o: $(BUILD_DIR)
	@echo "Compiling C++: $(notdir $<)"
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $(BUILD_DIR)/$(notdir $@) $<

mbedlib.a: $(MBED_OBJECTS)
	@echo "Linking mbed framework ..."
	$(AR) rcs $@ $(addprefix $(BUILD_DIR)/, $(notdir $^))
	@echo "Copying header files to include dir..."
	@$(MKDIR) -p include
	@$(COPY) $(MBED_INCLUDE_FILES) include/.

mbedFiles:
	@echo $(MBED_OBJECTS)

mbedIncludes:
	@echo $(MBED_INCLUDE_FILES)

includeFiles: 
	@echo $(INCS)


clean:
	$(RM) $(addprefix $(BUILD_DIR)/, $(notdir $(MBED_OBJECTS))) include/*.h mbedlib.a
	
	

## $(LD) -Wl,--start-group $(LD_FLAGS) libs/CyCompLib.a $(LDSCRIPT) -o $@ $^ -Wl,--end-group $(LD_SYS_LIBS)
## $(ELFTOOL) -C $@ --flash_size $(FLASH_SIZE) --flash_row_size $(FLASH_ROW_SIZE)