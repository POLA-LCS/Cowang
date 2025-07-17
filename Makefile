# ========== CONFIG ==========
CXX := g++
BUILD_DIR := build
TARGET := cowang.exe
SRC_DIRS := . src/error src/lexer src/parser

# Flags base
CXXFLAGS := -Wall -I./src -MMD -MP
STD := c++23
CXXFLAGS += -std=$(STD)

# Detectar compilador
COMPILER_ID := $(shell $(CXX) -dM -E -x c++ /dev/null | grep -q __clang__ && echo clang || echo gcc)

# Detectar versión
GCC_VERSION := $(shell $(CXX) -dumpversion | cut -f1 -d.)
CLANG_VERSION := $(shell $(CXX) --version | grep -o 'clang version [0-9]*' | cut -d' ' -f3)

# Verificación de versión mínima
ifeq ($(COMPILER_ID),gcc)
    ifeq ($(shell [ $(GCC_VERSION) -lt 13 ] && echo bad || echo ok),bad)
        $(error ❌ GCC $(GCC_VERSION) es demasiado viejo. Usá GCC 13 o más para C++23)
    endif
endif

ifeq ($(COMPILER_ID),clang)
    ifeq ($(shell [ $(CLANG_VERSION) -lt 16 ] && echo bad || echo ok),bad)
        $(error ❌ Clang $(CLANG_VERSION) es demasiado viejo. Usá Clang 16 o más para C++23)
    endif
endif

# Flags específicos por compilador
ifeq ($(COMPILER_ID),clang)
    CXXFLAGS += -Wno-unused-command-line-argument
endif

ifeq ($(COMPILER_ID),gcc)
    CXXFLAGS += -Wno-psabi
endif

# ========== FILES ==========
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

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)

.PHONY: all clean
