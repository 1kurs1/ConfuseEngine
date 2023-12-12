CFLAGS = -std=c++17 -O2

DEFINITIONS = -D CE_PLATFORM_LINUX -D CE_BUILD_LIB
INCLUDE = -I Confuse/src/

WORKSPACE_ENGINE_DIR = Confuse
WORKSPACE_SANDBOX_DIR = Sandbox

PROJECT_NAME = ConfuseEngine

CONFIGURATION = debug
PLATFORM = x64
BIN = bin/$(CONFIGURATION)/$(PLATFORM)/$(PROJECT_NAME)

#build sandbox
SANDBOX = Sandbox
$(SANDBOX): $(WORKSPACE_SANDBOX_DIR)/src/*.cpp
	clang++ $(CFLAGS) $(DEFINITIONS) $(INCLUDE) -o $(BIN)/$(SANDBOX).o $(WORKSPACE_SANDBOX_DIR)/src/*.cpp $(BIN)/ConfuseEngine.lib
	./$(BIN)/$(SANDBOX).o

#build engine
TARGET = ConfuseEngine
$(TARGET): $(WORKSPACE_ENGINE_DIR)/src/Confuse/*.cpp
	clang++ --shared -fPIC $(CFLAGS) $(DEFINITIONS) -o $(BIN)/$(TARGET).lib $(WORKSPACE_ENGINE_DIR)/src/Confuse/*.cpp


.PHONY: run

run: ConfuseEngine
	echo "build"

# .PHONY: run clean

# run: Confuse
# 	.$(BIN)/ConfuseEngine

# clean:
# 	rm -r $(BIN)/