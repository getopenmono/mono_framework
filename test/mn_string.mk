CXX=g++
CC=gcc
FRM_DIR=../src
EMUNO_DIR=../../emuno
OPTIMIZATION=0
TARGET=tests/string_test.cpp
BUILD_DIR=build
CDEFS=-DEMUNO -DTEST_CASE -DCATCH_CONFIG_MAIN

INCLUDES=	$(FRM_DIR) \
			$(FRM_DIR)/display \
			$(FRM_DIR)/io \
			$(FRM_DIR)/tests \
			$(FRM_DIR)/mbedcomp/api \
			$(FRM_DIR)/mbedcomp/hal \
			$(EMUNO_DIR)/vmbed \
			$(EMUNO_DIR)/vmbed/target_emuno

LIB_SOURCES=mn_string.cpp

INCS= $(foreach DIR,$(INCLUDES), $(addprefix -I, $(DIR)))
TARGET_OBJECT=$(addprefix $(BUILD_DIR)/, $(patsubst %.cpp,%.o,$(TARGET)))
LIB_OBJECTS = $(foreach FILE,$(LIB_SOURCES),$(addprefix $(BUILD_DIR)/, $(patsubst %.cpp,%.o,$(FILE))))
LIB_DEPS = $(foreach FILE,$(LIB_SOURCES),$(addprefix $(FRM_DIR)/, $(patsubst %.cpp,%.o,$(FILE))))

all: $(BUILD_DIR) $(BUILD_DIR)/slre.o $(TARGET_OBJECT)

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/slre.o: $(FRM_DIR)/slre.c
	@echo "Compiling library C++: $<"
	@mkdir -p $(dir $@)
	$(CC) -c $(CDEFS) $(INCS) -g -O$(OPTIMIZATION) -o $@ $<

$(BUILD_DIR)/%.o: $(FRM_DIR)/%.cpp
	@echo "Compiling library C++: $<"
	@mkdir -p $(dir $@)
	$(CXX) -c $(CDEFS) $(INCS) -g -O$(OPTIMIZATION) -o $@ $<

$(TARGET_OBJECT): $(FRM_DIR)/$(TARGET) $(LIB_OBJECTS)
	@echo "Compiling target C++: $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CDEFS) $(INCS) -g -O$(OPTIMIZATION) -o $@ $< $(LIB_OBJECTS) $(BUILD_DIR)/slre.o

.PHONY: run
run: all
	./$(TARGET_OBJECT)

.PHONY: objs
objs:
	@echo $(TARGET_OBJECT)
	@echo $(LIB_OBJECTS)
	@echo $(LIB_DEPS)
	@echo $(INCS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)