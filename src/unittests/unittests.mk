# This software is part of OpenMono, see http://developer.openmono.com
# Released under the MIT license, see LICENSE.txt

##
## List all (implementation) source files to be tested here
##
UNITTESTS_SOURCES := \
	sensors/dht.cpp

UNITTESTS_PATH := $(FRAMEWORK_PATH)/unittests

unittests: $(BUILD_DIR)/unittests
	$<

unittests-libsources := $(wildcard $(UNITTESTS_PATH)/lib/*.cpp $(UNITTESTS_PATH)/lib/*.c)
unittests-libheaders := $(wildcard $(UNITTESTS_PATH)/lib/*.hpp)
unittests-sources := $(wildcard $(UNITTESTS_PATH)/*.cpp) $(wildcard $(UNITTESTS_PATH)/*.hpp)

$(BUILD_DIR)/unittests: $(unittests-sources) $(unittests-libsources) $(unittests-libheaders)
	-mkdir -p $(BUILD_DIR)
	g++ -Wall -Wno-unused-result \
		-g -Wl,-map,$(BUILD_DIR)/unittests.map \
		-I $(UNITTESTS_PATH)/lib \
		$(INCS) \
		-o $@ \
		$(foreach SOURCE,$(UNITTESTS_SOURCES),$(FRAMEWORK_PATH)/$(SOURCE)) \
		$(filter %.cpp,$^) $(filter %.c,$^)

.PHONY: clean-unittests
unittests-clean:
	$(RM) -r $(BUILD_DIR)/unittests*

.PHONY: unittests-debug
unittests-debug:
	@echo "unittests-libsources = $(unittests-libsources)"
	@echo "unittests-libheaders = $(unittests-libheaders)"
	@echo "unittests-sources = $(unittests-sources)"
