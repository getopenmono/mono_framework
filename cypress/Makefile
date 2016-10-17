ARCH="/usr/local/gcc-arm-none-eabi-5_2-2015q4/bin/arm-none-eabi-"
BUILD_DIR=../build
CYPRESS_LIB=$(BUILD_DIR)/monoCyLib.a

FLASH_SIZE=262144
FLASH_ROW_SIZE=256
FLASH_ARRAY_SIZE=65536
EE_ARRAY=64
EE_ROW_SIZE=16
OPTIMIZATION = -Os
INCLUDE_DIR=include
CYPRESS_DIR=psoc5
LINKER_SCRIPT=${INCLUDE_DIR}/cm3gcc.ld
MONO_FRAMEWORK_PATH =../mono_framework
MBED_PATH=../mbedcomp
COMP_LIB=lib/CyComponentLibrary.a
MONO_LIB=lib/monoCyLib.a

SYS_OBJECTS = 	$(patsubst %.c,%.o,$(wildcard $(CYPRESS_DIR)/*.c)) \
				$(patsubst %.s,%.o,$(wildcard $(CYPRESS_DIR)/*Gnu.s))

SYS_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/, $(SYS_OBJECTS))

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

all: $(BUILD_DIR) $(CYPRESS_LIB)

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p ./$(BUILD_DIR)

$(BUILD_DIR)/%.o: %.s
	@echo "Assembling: $<"
	@$(MKDIR) -p $(dir $@)
	@$(AS) $(AS_FLAGS) $(INCS) -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(CYPRESS_LIB): $(COMP_LIB) $(SYS_TARGET_OBJECTS)
	@echo "Linking static library"
	@$(AR) rcs $@ $^

.PHONY: clean
clean:
	$(RRM) $(BUILD_DIR)


# Debugging this Makefile

systemFiles:
	@echo "SYS_OBJECTS --> $(SYS_OBJECTS)"
	@echo "SYS_TARGET_OBJECTS --> $(SYS_TARGET_OBJECTS)"

includeFiles:
	@echo $(INCS)
