TARGET = $(APP_DIR_PATH)/$(APP_NAME)
ARCH="arm-none-eabi-"
FLASH_SIZE=262144
FLASH_ROW_SIZE=256
FLASH_ARRAY_SIZE=65536
EE_ARRAY=64
EE_ROW_SIZE=16
OPTIMIZATION = -O0
BUILD_DIR=$(APP_DIR_PATH)/build
FRAMEWORK_PATH=../../../src
PSOC5_PATH=../../../src/cypress
MBED_PATH=../../../src/mbedcomp
MBED_FS=$(MBED_PATH)/libraries/fs
COMP_LIB=$(PSOC5_PATH)/lib/CyComponentLibrary.a
INCLUDE_DIR=$(PSOC5_PATH)/psoc5
CYPRESS_LIB=$(BUILD_DIR)/monoCyLib.a
LINKER_SCRIPT=$(INCLUDE_DIR)/cm3gcc.ld
COMPILE_TIMESTAMP=$(shell date +%Y-%m-%dT%H:%M:%S%z)

OBJECTS =		$(patsubst %.c,%.o,$(wildcard $(APP_DIR_PATH)/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(APP_DIR_PATH)/*.cpp))

MBED_OBJECTS =	$(patsubst %.cpp,%.o,$(wildcard $(MBED_PATH)/*.cpp)) \
				$(patsubst %.c,%.o,$(wildcard $(MBED_PATH)/common/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_PATH)/common/*.cpp)) \
				$(patsubst %.c,%.o,$(wildcard $(MBED_PATH)/target_cypress/*.c))

MBED_FS_OBJECTS=$(patsubst %.cpp,%.o,$(wildcard $(MBED_FS)/sd/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_FS)/fat/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(MBED_FS)/fat/ChaN/*.cpp))

MBED_INCLUDES =	\
				$(MBED_PATH)/api \
				$(MBED_PATH)/hal \
				$(MBED_PATH)/target_cypress \
				$(MBED_FS)/sd \
				$(MBED_FS)/fat \
				$(MBED_FS)/fat/ChaN

MONO_OBJECTS =	$(patsubst %.c,%.o,$(wildcard $(FRAMEWORK_PATH)/*.c)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/display/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/display/ili9225g/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/display/ui/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/wireless/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/media/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/net/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/io/*.cpp)) \
				$(patsubst %.cpp,%.o,$(wildcard $(FRAMEWORK_PATH)/sensors/*.cpp))


MONO_INCLUDES =	$(FRAMEWORK_PATH) \
				$(FRAMEWORK_PATH)/display \
				$(FRAMEWORK_PATH)/display/ui \
				$(FRAMEWORK_PATH)/display/ili9225g \
				$(FRAMEWORK_PATH)/io \
				$(FRAMEWORK_PATH)/media \
				$(FRAMEWORK_PATH)/net \
				$(FRAMEWORK_PATH)/wireless \
				$(FRAMEWORK_PATH)/sensors


SYS_OBJECTS = 	$(patsubst %.c,%.o,$(wildcard $(INCLUDE_DIR)/*.c)) \
				$(patsubst %.s,%.o,$(wildcard $(INCLUDE_DIR)/*Gnu.s))

PSOC_INCLUDES =	$(INCLUDE_DIR)

TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/, $(OBJECTS))
MBED_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/mbed, $(subst $(MBED_PATH),,$(MBED_OBJECTS)))
MBED_FS_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/mbed, $(subst $(MBED_FS),,$(MBED_FS_OBJECTS)))
MONO_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/mono, $(subst $(FRAMEWORK_PATH),, $(MONO_OBJECTS)))
SYS_TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/cypress, $(subst $(PSOC5_PATH)/psoc5,,$(SYS_OBJECTS)))

TARGET_HEADERS =	$(wildcard $(APP_DIR_PATH)/*.h) \
					$(wildcard $(APP_DIR_PATH)/*.hpp)
MBED_HEADERS =		$(foreach path,$(MBED_INCLUDES),$(wildcard $(path)/*.h))
MONO_HEADERS =		$(foreach path,$(MONO_INCLUDES),$(wildcard $(path)/*.h))
PSOC_HEADERS =		$(foreach path,$(PSOC_INCLUDES),$(wildcard $(path)/*.h))

CC=$(ARCH)gcc
CXX=$(ARCH)g++
LD=$(ARCH)gcc
AS=$(ARCH)gcc
AR=$(ARCH)ar
RANLIB=$(ARCH)ranlib
STRIP=$(ARCH)strip
OBJCOPY=$(ARCH)objcopy
OBJDUMP=$(ARCH)objdump
GDB=$(ARCH)gdb
COPY=cp
MKDIR=mkdir
MONOPROG=monomake monoprog
ELFTOOL='C:\Program Files (x86)\Cypress\PSoC Creator\3.1\PSoC Creator\bin\cyelftool.exe'
INCS = -I . $(addprefix -I, $(MONO_INCLUDES) $(MBED_INCLUDES) $(INCLUDE_DIR))
CDEFS=-DMONO_COMPILE_TIMESTAMP="\"$(COMPILE_TIMESTAMP)\""
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

ifeq ($(OS),Windows_NT)
	RM = cmd //C del //Q //F
	RRM = cmd //C rmdir //Q //S
else
	RM = rm -f
	RRM = rm -f -r
endif

all: $(BUILD_DIR) $(TARGET).elf

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p ./$(BUILD_DIR)

# %.cpp: %.h
# 	@echo "Header dependency $<"

# $(BUILD_DIR)/mono/slre.o: $(FRAMEWORK_PATH)/slre.c
# 	@echo "Compiling SLRE C: $<"
# 	@$(MKDIR) -p $(dir $@)
# 	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) -Wno-char-subscripts $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mono/%.o: $(FRAMEWORK_PATH)/%.c $(MONO_HEADERS)
	@echo "Compiling C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) -Wno-char-subscripts $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mono/%.o: $(FRAMEWORK_PATH)/%.cpp $(MONO_HEADERS)
	@echo "Compiling mono C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mbed/%.o: $(MBED_PATH)/%.c $(MBED_HEADERS)
	@echo "Compiling mbed C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mbed/%.o: $(MBED_PATH)/%.cpp $(MBED_HEADERS)
	@echo "Compiling mbed C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mbed/fat/%.o: $(MBED_FS)/fat/%.cpp $(MBED_HEADERS)
	@echo "Compiling mbed fs C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/mbed/sd/%.o: $(MBED_FS)/sd/%.cpp $(MBED_HEADERS)
	@echo "Compiling mbed fs C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/%.o: %.c $(TARGET_HEADERS)
	@echo "Compiling C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/%.o: %.cpp $(TARGET_HEADERS)
	@echo "Compiling C++: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CXX) $(CC_FLAGS) $(ONLY_CPP_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(TARGET).elf: $(CYPRESS_LIB) $(COMP_LIB) $(MBED_TARGET_OBJECTS) $(MBED_FS_TARGET_OBJECTS) $(MONO_TARGET_OBJECTS) $(TARGET_OBJECTS)
	@echo "Linking $(notdir $@)"
	@$(LD) -Wl,--start-group $(LD_FLAGS) -o $@ $^ -mthumb -march=armv7-m -mfix-cortex-m3-ldrd "-Wl,-Map,mono_project.map" -T $(LINKER_SCRIPT) -g "-u\ _printf_float" $(LD_SYS_LIBS) -Wl,--gc-sections -Wl,--end-group

$(TARGET).hex: $(TARGET).elf
	$(ELFTOOL) -C $^ --flash_size $(FLASH_SIZE) --flash_row_size $(FLASH_ROW_SIZE)
	$(OBJCOPY) -O ihex $(COPY_FLAGS) $< $@
	$(ELFTOOL) -B $^ --flash_size $(FLASH_SIZE) --flash_array_size $(FLASH_ARRAY_SIZE) --flash_row_size $(FLASH_ROW_SIZE) --ee_array $(EE_ARRAY) --ee_row_size $(EE_ROW_SIZE)

$(TARGET):  $(OBJS)  ${LINKER_SCRIPT}
	@echo "Other link: $(OBJS)"
	$(LD) $(LDSCRIPT) $(OBJS) -o $@

$(BUILD_DIR)/cypress/%.o: $(INCLUDE_DIR)/%.s $(PSOC_HEADERS)
	@echo "Assembling Cypress PSoC5: $<"
	@$(MKDIR) -p $(dir $@)
	@$(AS) $(AS_FLAGS) $(INCS) -o $@ $<

$(BUILD_DIR)/cypress/%.o: $(INCLUDE_DIR)/%.c $(PSOC_HEADERS)
	@echo "Compiling Cypress PSoC5 C: $<"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(ONLY_C_FLAGS) $(CDEFS) $(INCS) -o $@ $<

$(CYPRESS_LIB): $(COMP_LIB) $(SYS_TARGET_OBJECTS)
	@echo "Linking Cypress PSoC5 static library"
	@$(AR) rcs $@ $^

$(MBED_LIB):
	@echo "Building mbed library..."
	@make -C $(MBED_PATH)


systemFiles:
	@echo $(SYS_OBJECTS)

appFiles:
	@echo $(TARGET_OBJECTS)

mbedFiles:
	@echo $(MBED_TARGET_OBJECTS)

monoFiles:
	@echo $(MONO_TARGET_OBJECTS)

includeFiles:
	@echo $(MONO_INCLUDES) $(MBED_INCLUDES) $(PSOC_INCLUDES)

headers:
	@echo $(TARGET_HEADERS) $(MONO_HEADERS) $(MBED_HEADERS) $(PSOC_HEADERS)

install: $(TARGET).elf
	@echo "Programming app to device..."
	$(MONOPROG) -p $(TARGET).elf --verbose=2

debug: $(TARGET).elf
	@echo "Launching GDB... (Make sure OpenOCD is running!)"
	echo "target extended-remote :3333\nmonitor reset halt\n" > gdb_script
	$(GDB) -x gdb_script $(TARGET).elf -tui

clean:
	$(RRM) $(BUILD_DIR)
	$(RRM) $(RELEASE_DIR)

summary: $(TARGET).elf
	$(ELFTOOL) -S $(TARGET).elf


## $(LD) -Wl,--start-group $(LD_FLAGS) libs/CyCompLib.a $(LDSCRIPT) -o $@ $^ -Wl,--end-group $(LD_SYS_LIBS)
## $(ELFTOOL) -C $@ --flash_size $(FLASH_SIZE) --flash_row_size $(FLASH_ROW_SIZE)