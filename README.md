# Graphing App in C++ with OpenGL

## Overview
This project is a basic graphing application written in pure C++ and C using OpenGL for rendering any kind of graph on 2D plane in Various coordinate System like Cartesian, polar and parametric. It includes a custom UI without any external UI libraries or frameworks.

## Project Structure
- **src/**: Contains source code for the application.
- **include/**: Header files.
- **shaders/**: OpenGL shaders for rendering the video.
- **obj/**: Compiled Binary Object Files.
- **CMakeLists.txt**: CMake build configuration file.

# Project Requirments: 
### On Windows
- Install C++ with Cmake [MinGW](https://sourceforge.net/projects/mingw/)

### On Linux
- Install C++
    ```
    sudo apt update
    sudo apt install build-essential
    sudo apt install cmake
    ```

## Build Instructions
To build the project, use CMake or Makefile:

## Using CMAKE to build the project:
### On Windows
```
mingw32-make all
```
### On Linux
```
make
```
## Running the Application
Once built, you can run the application using the following command:
```
./graph
```
## Cleaning the Build:
### On Windows
```
mingw32-make clean
```
### On Linux
```
make clean
```


