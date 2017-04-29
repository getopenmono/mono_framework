DIST=../dist
SOURCE_DIR=icons
TARGET_DIR=$(DIST)/mono/include/display/icons

IMGICON=img2icon
MKDIR=mkdir

SOURCE_ICONS=$(wildcard $(SOURCE_DIR)/*.png)
TARGET_ICONS=$(addprefix $(TARGET_DIR)/, $(patsubst icons/%.png, %.h, $(SOURCE_ICONS)))

all: $(TARGET_ICONS)

outputDir:
	$(MKDIR) -p $(TARGET_DIR)

$(TARGET_DIR)/%.h: $(SOURCE_DIR)/%.png outputDir
	@echo "Converting $<"
	@$(IMGICON) -d $@ $<

.PHONY: targetIcons
targetIcons:
	@echo "Sources:"
	@echo "$(SOURCE_ICONS)"
	@echo "Targets:"
	@echo "$(TARGET_ICONS)"
