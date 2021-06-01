SRC_DIR=$(CURDIR)/src
BUILD_DIR=$(CURDIR)/build

######################################################################
.PHONY: Setup-RelWithDebInfo
Setup-RelWithDebInfo:
		mkdir -p $(BUILD_DIR)/RelWithDebInfo && \
		cmake -B $(BUILD_DIR)/RelWithDebInfo \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=RelWithDebInfo

.PHONY: Setup-MinSizeRel
Setup-MinSizeRel:
		mkdir -p $(BUILD_DIR)/MinSizeRel && \
		cmake -B $(BUILD_DIR)/MinSizeRel \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=MinSizeRel

.PHONY: Setup-Debug
Setup-Debug:
		mkdir -p $(BUILD_DIR)/Debug && \
		cmake -B $(BUILD_DIR)/Debug \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug

.PHONY: Setup-Release
Setup-Release:
		mkdir -p $(BUILD_DIR)/Release && \
		cmake -B $(BUILD_DIR)/Release \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=Release

.PHONY: Setup
Setup: Setup-RelWithDebInfo Setup-MinSizeRel Setup-Debug Setup-Release

######################################################################
.PHONY: RelWithDebInfo
RelWithDebInfo:
		cmake --build $(BUILD_DIR)/RelWithDebInfo

.PHONY: MinSizeRel
MinSizeRel:
		cmake --build $(BUILD_DIR)/MinSizeRel

.PHONY: Debug
Debug:
		cmake --build $(BUILD_DIR)/Debug

.PHONY: Release
Release:
		cmake --build $(BUILD_DIR)/Release

.PHONY: All
All: RelWithDebInfo MinSizeRel Debug Release

.PHONY: Clean
Clean:
		rm -rf build