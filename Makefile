SRC_DIR=$(CURDIR)/src
BUILD_DIR=$(CURDIR)/build

######################################################################
.PHONY: setup-relwithdebinfo
setup-relwithdebinfo:
		mkdir -p $(BUILD_DIR)/relwithdebinfo && \
		cmake -B $(BUILD_DIR)/relwithdebinfo \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=RelWithDebInfo

.PHONY: setup-minsizerel
setup-minsizerel:
		mkdir -p $(BUILD_DIR)/minsizerel && \
		cmake -B $(BUILD_DIR)/minsizerel \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=MinSizeRel

.PHONY: setup-debug
setup-debug:
		mkdir -p $(BUILD_DIR)/debug && \
		cmake -B $(BUILD_DIR)/debug \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=Debug

.PHONY: setup-release
setup-release:
		mkdir -p $(BUILD_DIR)/release && \
		cmake -B $(BUILD_DIR)/release \
		-S $(SRC_DIR) \
		-G"MinGW Makefiles" \
		-DCMAKE_BUILD_TYPE=Release

.PHONY: setup
setup: setup-relwithdebinfo setup-MinSizeRel setup-Debug setup-Release

######################################################################
.PHONY: relwithdebinfo
relwithdebinfo:
		cmake --build $(BUILD_DIR)/relwithdebinfo && \
		cmake --install $(BUILD_DIR)/relwithdebinfo

.PHONY: minsizerel
minsizerel:
		cmake --build $(BUILD_DIR)/minsizerel && \
		cmake --install $(BUILD_DIR)/minsizerel

.PHONY: debug
debug:
		cmake --build $(BUILD_DIR)/debug && \
		cmake --install $(BUILD_DIR)/debug

.PHONY: release
release:
		cmake --build $(BUILD_DIR)/release && \
		cmake --install $(BUILD_DIR)/release

.PHONY: All
All: relwithdebinfo minsizerel debug delease

.PHONY: clean
clean:
		rm -rf build
