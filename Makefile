# Compiler and flags
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Iinclude

# Directories
SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build

# Files
SRCS      := OrderBook.cpp ConcurrentOrderBook.cpp main.cpp
TEST_SRCS := test_orderbook.cpp

# Derived
OBJS      := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TEST_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.test.o,$(TEST_SRCS))

# Targets
.PHONY: all test clean

all: $(BUILD_DIR) main test_orderbook

# Build main executable
main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build test executable
test_orderbook: $(TEST_OBJS) $(BUILD_DIR)/OrderBook.o $(BUILD_DIR)/ConcurrentOrderBook.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source .cpp → .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test .cpp → .o
$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build dir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) main test_orderbook liborderbook.a

