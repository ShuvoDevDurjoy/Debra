# Use g++ for compiling C++ code and gcc for C code
CXX = g++
CC = gcc

CXXFLAGS = -Wall -std=c++20 -O2
CFLAGS = -Wall -O2
LDFLAGS = -lpthread -lglfw 
#  -lGLEW -lGL -lX11 -lm -lfreeimage

# Path to the glfw and glew libraries
LIB_DIR = include/lib

# Path to the include directories of glfw and glew
INCLUDE_DIR = include

# Add glad.c file for GLAD (for OpenGL loader)
GLAD_SRC = include/glad.c
GLAD_OBJ = $(OBJ_DIR)/glad.o

# Name of the output executable
OUTPUT = shuvo

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
# Clean target
clean:
	rm -f $(OBJ_DIR)/*.o $(GLAD_OBJ) $(OUTPUT)
