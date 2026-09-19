CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
LDFLAGS  := -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)

# Diretórios de inclusão para a raiz de src e subpastas de cada estrutura
INCLUDE  := -Isrc -Isrc/KDTree -Isrc/Patricia -Isrc/Splay -Isrc/Treap -Isrc/Trie

TARGET_MAIN      := main
TARGET_BENCHMARK := benchmark

.PHONY: all main benchmark run run-benchmark clean debug release

all: build $(APP_DIR)/$(TARGET_MAIN) $(APP_DIR)/$(TARGET_BENCHMARK)

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

# Compilação do programa principal de demonstração
$(APP_DIR)/$(TARGET_MAIN): src/main.cpp
	@mkdir -p $(APP_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< $(LDFLAGS) -o $@

# Compilação do programa de experimentos/benchmark
$(APP_DIR)/$(TARGET_BENCHMARK): src/benchmark.cpp
	@mkdir -p $(APP_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< $(LDFLAGS) -o $@

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

run: $(APP_DIR)/$(TARGET_MAIN)
	$(APP_DIR)/$(TARGET_MAIN)

run-benchmark: $(APP_DIR)/$(TARGET_BENCHMARK)
	$(APP_DIR)/$(TARGET_BENCHMARK)

clean:
	-@rm -rf $(BUILD)
