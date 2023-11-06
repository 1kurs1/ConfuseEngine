include .env

CFLAGS = -std=c++17 -O2 -I$(TINYOBJ_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
INC = -I../headers/

vertSources = $(shell find ../shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ../shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = ConfuseEditor
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): ../src/*.cpp ../src/core/*.cpp ../src/editor/*.cpp ../src/render/*.cpp ../src/window/*.cpp ../src/render/RenderSystems/*.cpp ../src/input/*.cpp ../headers/core/*.hpp ../headers/editor/*.hpp ../headers/render/*.hpp ../headers/window/*.hpp ../headers/render/RenderSystems/*.hpp ../headers/input/*.hpp ../headers/utils/*.hpp
	clang++ $(CFLAGS) $(INC) -o $(TARGET) ../src/*.cpp ../src/core/*.cpp ../src/editor/*.cpp ../src/render/*.cpp ../src/window/*.cpp ../src/render/RenderSystems/*.cpp ../src/input/*.cpp $(LDFLAGS)

%.spv: %
	${GLSLC} $< -o $@

.PHONY: run clean

run: ConfuseEditor
	./ConfuseEditor

clean: 
	rm -f ConfuseEditor
	rm -f *.spv