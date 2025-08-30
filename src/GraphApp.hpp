#ifndef __GRAPHAPP_HPP__
#define __GRAPHAPP_HPP__
#include <iostream>
#include <vector>
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "graph.hpp"
#include "shader.hpp"

#define VERTEX_SHADER_FILE_NAME "./shaders/vertex_shader.vs"
#define FRAGMENT_SHADER_FILE_NAME "./shaders/fragment_shader.fs"

class Graph;

class GraphApp
{
private:
    int window_width = 1200;
    int window_height = 600;
    std::string vertex_shader_file = VERTEX_SHADER_FILE_NAME;
    std::string fragment_shader_file = FRAGMENT_SHADER_FILE_NAME;
    GLuint VBO, VAO;
    Shader *shader;
    GLFWwindow *window;

private:
    int InitWindow();
    int loadGLAD();
    static void frame_buffer_size_callback(GLFWwindow *, int, int);
    void process_input();
    void setCallback();
    void mainLoop(Graph *);
    void cleanUp(Graph *);

public:
    GraphApp();
    ~GraphApp() {};
    GLFWwindow *getWindow() { return window; };
    Shader *getShader() { return shader; };
    void refreshOpenGL(std::vector<float> &, int, int);
    void setColor(float, float, float);
    void run(Graph *);
};

#endif