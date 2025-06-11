#---- Compiler settings -------------------------------------------------------
# Allow overriding the compiler from the environment
CXX ?= g++

WARNINGS   := -Wall -Wextra -Wpedantic -Wshadow -Wconversion
CXXFLAGS   := $(WARNINGS) -std=c++20 -O3 -flto -pthread

INC_DIRS := core/include ui/include players/include \
            players/strategies/include include
CPPFLAGS  := $(INC_DIRS:%=-I%)

#---- Directory layout --------------------------------------------------------
SRC_DIRS := core/src ui/src players/src players/strategies/src src

BUILD_DIR    := build
BIN_DIR      := bin
TEST_DIR     := tests
TEST_BIN_DIR := $(BIN_DIR)/tests

#---- Source discovery --------------------------------------------------------
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.cpp))
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Library objects for tests (exclude main.cpp)
LIB_SRCS := $(filter-out src/main.cpp,$(SRCS))
LIB_OBJS := $(LIB_SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Test executables
TEST_NAMES := $(basename $(notdir $(wildcard $(TEST_DIR)/*.cpp)))
TEST_BINS  := $(addprefix $(TEST_BIN_DIR)/,$(TEST_NAMES))

#---- Targets -----------------------------------------------------------------
.PHONY: all clean test

all: $(BIN_DIR)/gomoku

$(BIN_DIR)/gomoku: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.cpp $(LIB_OBJS)
	@mkdir -p $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

test: $(TEST_BINS)
	@for t in $^; do \
	echo "Running $$t"; \
	$$t; \
	done

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
