#---- Compiler Settings -------------------------------------------------------
CXX      := g++
CXXFLAGS := -std=c++20 \
            -Wall -Wextra -Wpedantic \
            -Wshadow -Wconversion \
            -O2 \
            -Iinclude           # include search path

#---- Directory Definitions ---------------------------------------------------
SRC_DIR       := src
INC_DIR       := include
BUILD_DIR     := build
BIN_DIR       := bin
TEST_DIR      := tests
TEST_BIN_DIR  := $(BIN_DIR)/tests

#---- Auto collect sources, objects and dependencies -------------------------
SRCS    := $(wildcard $(SRC_DIR)/*.cpp)
OBJS    := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS    := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

#---- Library sources (exclude main.cpp) for linking into tests --------------
LIB_SRCS := $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))
LIB_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))

#---- Test files and targets (auto-detected and managed separately) ----------
# Extract all *.cpp file names in tests/ (without path or suffix)
TEST_NAMES := $(basename $(notdir $(wildcard $(TEST_DIR)/*.cpp)))
# Compose full source paths and binary output paths
TEST_SRCS  := $(addprefix $(TEST_DIR)/, $(addsuffix .cpp, $(TEST_NAMES)))
TEST_BINS  := $(addprefix $(TEST_BIN_DIR)/, $(TEST_NAMES))

#---- Targets -----------------------------------------------------------------
.PHONY: all clean test

all: $(BIN_DIR)/gomoku

# Link final executable
$(BIN_DIR)/gomoku: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp → .o and generate .d
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

#---- Build each test executable ----------------------------------------------
# tests/foo.cpp -> bin/tests/foo
$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.cpp $(LIB_OBJS)
	@mkdir -p $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run all tests
test: $(TEST_BINS)
	@for t in $^; do \
	  echo "Running $$t"; \
	  $$t; \
	done

# Remove all build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
