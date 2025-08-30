# Use g++ for compiling C++ code and gcc for C code
CXX = g++
CC = gcc

CXXFLAGS = -Wall -std=c++20 -O2
CFLAGS = -Wall -O2

#lflags
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lglfw3dll -lgdi32 -lopengl32 -luser32 -lkernel32 -lwinmm
else
    LDFLAGS = -lglfw -lGL -lpthread -ldl
endif


# Path to the glfw libraries
LIB_DIR = include/lib

# Path to the include directories of glfw and glew
INCLUDE_DIR = include

# Add glad.c file for GLAD (for OpenGL loader)
GLAD_SRC = include/glad.c
GLAD_OBJ = $(OBJ_DIR)/glad.o

# Name of the output executable
OUTPUT = graph

# Source files and object files
OBJ_DIR = obj
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: $(OUTPUT)

# Linking final executable
$(OUTPUT): $(OBJ) $(GLAD_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L $(LIB_DIR) $(LDFLAGS)

# Compiling C++ source files with g++
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

# Compiling GLAD (C file) with gcc
$(GLAD_OBJ): $(GLAD_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Debug target
debug: CXXFLAGS += -DDEBUG -O0 -g
debug: clean all

# Clean target
clean:
	@echo Cleaning...
ifeq ($(OS),Windows_NT)
	@if exist $(OBJ_DIR) ( \
		for %%f in ($(OBJ_DIR)\*.o) do del /q "%%f" \
	)
	@if exist $(GLAD_OBJ) del /q "$(GLAD_OBJ)"
	@if exist $(OUTPUT).exe del /q "$(OUTPUT).exe"
else
	@rm -f $(OBJ_DIR)/*.o $(GLAD_OBJ) $(OUTPUT)
endif
	@echo cleaned...
