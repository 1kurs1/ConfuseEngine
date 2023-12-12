CFLAGS = -std=c++17 -O2

WORKSPACE_ENGINE_DIR = Confuse
WORKSPACE_SANDBOX_DIR = Sandbox

PROJECT_NAME = ConfuseEngine

CONFIGURATION = debug
PLATFORM = x64
BIN = bin/$(CONFIGURATION)/$(PLATFORM)/$(PROJECT_NAME)

#build sandbox
SANDBOX = Sandbox
$(SANDBOX): $(WORKSPACE_SANDBOX_DIR)/src/*.cpp
	clang++ $(CFLAGS) -o $(BIN)/$(SANDBOX).o $(WORKSPACE_SANDBOX_DIR)/src/*.cpp $(BIN)/Confuse.o
	./$(BIN)/$(SANDBOX).o

#build engine
TARGET = Confuse
$(TARGET): $(WORKSPACE_ENGINE_DIR)/src/*.cpp 
	clang++ -shared $(CFLAGS) -o $(BIN)/$(TARGET).o $(WORKSPACE_ENGINE_DIR)/src/*.cpp
	llvm-ar rc $(BIN)/$(TARGET).o $(BIN)/Confuse.so


# .PHONY: run clean

# run: Confuse
# 	.$(BIN)/ConfuseEngine

# clean:
# 	rm -r $(BIN)/