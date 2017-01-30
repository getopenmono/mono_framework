CC=clang
CXX=clang++
MKDIR=mkdir
RELEASE_DIR=bin
BUILD_DIR=build
FRM_BUILD_DIR=buildlib
CDEFS=-DCATCH_CONFIG_MAIN -DEMUNO
FRAMEWORK_PATH=..
EMUNO_PATH=../../../emuno
INCLUDES=	$(FRAMEWORK_PATH) 
			# $(FRAMEWORK_PATH)/display \
			# $(FRAMEWORK_PATH)/mbedcomp/api \
			# $(FRAMEWORK_PATH)/mbedcomp/hal \
			# $(EMUNO_PATH)/vmbed \
			# $(EMUNO_PATH)/vmbed/target_emuno

DEPENDENTS= mn_string.o queue.o regex.o slre.o

OBJECTS = queue_test.o string_test.o http_client_test.o
INCS= $(addprefix -I, $(INCLUDES))
TARGET_OBJECTS = $(addprefix $(BUILD_DIR)/, $(OBJECTS))
TARGET_DEPS= $(addprefix $(FRM_BUILD_DIR)/, $(DEPENDENTS))

all: tests

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

$(FRM_BUILD_DIR):
	@echo "creating library build directory"
	@mkdir -p $(FRM_BUILD_DIR)

$(RELEASE_DIR):
	@echo "creating release directory"
	@mkdir -p $(RELEASE_DIR)

$(FRM_BUILD_DIR)/%.o: $(FRAMEWORK_PATH)/%.c
	@echo "Compiling library C: $<"
	@$(MKDIR) -p $(dir $@)
	$(CC) -c $(CDEFS) $(INCS) -o $@ $<

$(FRM_BUILD_DIR)/%.o: $(FRAMEWORK_PATH)/%.cpp
	@echo "Compiling library C++: $<"
	@$(MKDIR) -p $(dir $@)
	$(CXX) -c $(CDEFS) $(INCS) -o $@ $<

$(BUILD_DIR)/%.o: %.cpp $(RELEASE_DIR)
	@echo "Compiling test case C++: $<"
	@$(MKDIR) -p $(dir $@)
	$(CXX) $(CDEFS) $(INCS) -o $(RELEASE_DIR)/$(notdir $(patsubst %.cpp,$(basename %),$@)) $< $(TARGET_DEPS)

.PHONY: tests
tests: $(TARGET_DEPS) $(TARGET_OBJECTS)
	@echo "Building tests..."

.PHONY:
run: $(tests)
	@echo "Running tests..."
	@echo "$(foreach file, $(addprefix $(RELEASE_DIR)/, $(OBJECTS)), $(shell \"./$(file)\" ))"

.PHONY: objs
objs:
	@echo "$(OBJECTS)"

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm -r $(FRM_BUILD_DIR)
	rm -r $(RELEASE_DIR)
