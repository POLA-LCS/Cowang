CXX       := g++
CXXFLAGS  := -std=c++23 -Wall -I./src
SRC_DIRS  := . ./src/error ./src/lexer ./src/parser
BUILD_DIR := build
TARGET    := cowang.exe

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(pathsubst %.cpp, $(BUILD_DIR)/)# ========== CONFIG ==========
CXX := g++
CXXFLAGS := -std=c++23 -Wall -I./src
SRC_DIRS := . ./src/error ./src/lexer ./src/parser
BUILD_DIR := build
TARGET := cowang.exe

# ========== SOURCES & OBJECTS ==========
SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# ========== RULES ==========

# Target principal
all: $(TARGET)

# Linkeo final
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) -o $@

# Compilar cada archivo fuente a objeto
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@
# ========== CONFIG ==========
CXX := g++
CXXFLAGS := -std=c++23 -Wall -I./src -MMD -MP
SRC_DIRS := . ./src/error ./src/lexer ./src/parser
BUILD_DIR := build
TARGET := cowang.exe

# ========== FILE DETECTION ==========
SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

# ========== RULES ==========

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CXX) $(OBJS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET)

# Incluir archivos .d generados por -MMD (dependencias de headers)
-include $(DEPS)

.PHONY: all clean

# Limpiar objetos y ejecutable
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
