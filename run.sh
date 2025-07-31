#!/bin/bash

# Project name
PROJECT_NAME="VideoPlayerApp"

# Directories to create
DIRECTORIES=(
    "$PROJECT_NAME/build"
    "$PROJECT_NAME/src"
    "$PROJECT_NAME/include"
    "$PROJECT_NAME/shaders"
    "$PROJECT_NAME/assets"
)

# Files to create
FILES=(
    "$PROJECT_NAME/src/main.cpp"
    "$PROJECT_NAME/src/video_player.cpp"
    "$PROJECT_NAME/src/ui.cpp"
    "$PROJECT_NAME/src/shader.cpp"
    "$PROJECT_NAME/src/utils.cpp"
    "$PROJECT_NAME/include/video_player.h"
    "$PROJECT_NAME/include/ui.h"
    "$PROJECT_NAME/include/shader.h"
    "$PROJECT_NAME/include/utils.h"
    "$PROJECT_NAME/shaders/vertex_shader.glsl"
    "$PROJECT_NAME/shaders/fragment_shader.glsl"
    "$PROJECT_NAME/assets/font.ttf"
    "$PROJECT_NAME/assets/sample_video.mp4"
    "$PROJECT_NAME/CMakeLists.txt"
    "$PROJECT_NAME/Makefile"
    "$PROJECT_NAME/README.md"
)

# Create the directories
echo "Creating directories..."
for dir in "${DIRECTORIES[@]}"; do
    mkdir -p "$dir"
    echo "Created: $dir"
done

# Create the files
echo "Creating files..."
for file in "${FILES[@]}"; do
    touch "$file"
    echo "Created: $file"
done

# Write a basic README file
cat > "$PROJECT_NAME/README.md" <<EOL
# Video Player App in C++ with OpenGL

## Overview
This project is a basic video player written in pure C++ using OpenGL for rendering. It includes a custom UI without any external UI libraries or frameworks.

## Project Structure
- **src/**: Contains source code for the application.
- **include/**: Header files.
- **shaders/**: OpenGL shaders for rendering the video.
- **assets/**: Sample video and font files for UI elements.
- **build/**: Compiled binaries and object files.

## Build Instructions
To build the project, use CMake or Makefile:

### Using CMake:
\`\`\`
mkdir build
cd build
cmake ..
make
\`\`\`

### Using Makefile:
\`\`\`
make
\`\`\`

## Running the Application
Once built, you can run the application from the build directory:
\`\`\`
./video_player
\`\`\`

EOL

# Write a basic CMakeLists.txt file
cat > "$PROJECT_NAME/CMakeLists.txt" <<EOL
cmake_minimum_required(VERSION 3.10)
project(VideoPlayerApp)

set(CMAKE_CXX_STANDARD 17)

# Add the source files
set(SOURCES
    src/main.cpp
    src/video_player.cpp
    src/ui.cpp
    src/shader.cpp
    src/utils.cpp
)

# Include directories
include_directories(include)

# Find OpenGL and link libraries
find_package(OpenGL REQUIRED)
include_directories(\${OPENGL_INCLUDE_DIR})

# Add the executable
add_executable(video_player \${SOURCES})

# Link OpenGL library
target_link_libraries(video_player \${OPENGL_LIBRARIES})
EOL

echo "Project structure created successfully."

