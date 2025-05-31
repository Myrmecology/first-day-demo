# ASCII Fire Effect Simulator - Makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O2
DEBUG_FLAGS = -g -DDEBUG
LIBS = -lncurses

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET = fire_simulator

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build target
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
	@echo "🔥 Fire simulator compiled successfully!"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "🐛 Debug build completed!"

# Run the program
run: $(TARGET)
	@echo "🚀 Starting fire simulator..."
	./$(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "📦 Installing dependencies..."
	sudo apt-get update
	sudo apt-get install build-essential libncurses5-dev libncursesw5-dev

# Install dependencies (macOS)
install-deps-mac:
	@echo "📦 Installing dependencies for macOS..."
	brew install ncurses

# Test build (compile without running)
test: $(TARGET)
	@echo "✅ Build test successful!"

# Memory check (requires valgrind)
memcheck: debug
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(BUILD_DIR) $(TARGET)

# Full clean (including backup files)
distclean: clean
	@echo "🗑️  Deep cleaning..."
	find . -name "*.o" -delete
	find . -name "*~" -delete
	find . -name "*.swp" -delete

# Show project info
info:
	@echo "🔥 ASCII Fire Effect Simulator"
	@echo "   Compiler: $(CXX)"
	@echo "   Flags: $(CXXFLAGS)"
	@echo "   Sources: $(SOURCES)"
	@echo "   Target: $(TARGET)"

# Help target
help:
	@echo "🔥 ASCII Fire Effect Simulator - Available targets:"
	@echo ""
	@echo "  make              - Build the fire simulator"
	@echo "  make run          - Build and run the simulator"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make test         - Test compilation"
	@echo "  make clean        - Remove build files"
	@echo "  make distclean    - Deep clean all generated files"
	@echo "  make install-deps - Install dependencies (Ubuntu/Debian)"
	@echo "  make install-deps-mac - Install dependencies (macOS)"
	@echo "  make memcheck     - Run memory leak check"
	@echo "  make info         - Show project information"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "🚀 Quick start: make run"

# Phony targets
.PHONY: all debug run install-deps install-deps-mac test memcheck clean distclean info help