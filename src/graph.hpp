#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "shader.hpp"
#include "keyClicked.hpp"
#include "GraphApp.hpp"

class GraphApp;

struct parametricObject
{
    std::pair<float, float> (*function)(float);
    float minRange;
    float maxRange;
};

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 700
#define VERTEX_SHADER_FILE_NAME "./shaders/vertex_shader.vs"
#define FRAGMENT_SHADER_FILE_NAME "./shaders/fragment_shader.fs"

namespace GraphUtilities
{
    extern float DEFAULT_FRAG;
    extern float DEFAULT_YRATIO;
    extern float DEFAULT_ASPECT_RATIO;
    extern float DEFAULT_PIXEL_PER_UNIT;
    extern float DEFAULT_PAN_Y_OFFSET;
    extern float DEFAULT_PAN_X_OFFSET;
    extern float MAX_PIXEL_PER_UNIT;
    extern float MIN_PIXEL_PER_UNIT;
    extern float PIXEL_INCREMENT_STEP;
    extern float DEFAULT_PAN_INCREMENT_STEP;
    extern int DEFAULT_STEPS;

    float genSin(float x);
    float genCos(float x);
    float genTan(float x);
    float genCosec(float x);
    float genSec(float x);
    float genCot(float x);
    float toRadians(float degree);
};

class Graph : public KeyClicked
{
private:
    Graph();
    static Graph *instance;

    // destructor for graph object
    
    // Delete copy and move operations
    Graph(const Graph &) = delete;
    Graph(Graph &&) = delete;
    Graph &operator=(const Graph &) = delete;
    Graph &operator=(Graph &&) = delete;
    
    private:
    std::vector<float> gridLines;
    std::vector<float> boxVertices;

    std::vector<float> vertices;
    std::vector<float> verticesRadians;
    std::vector<float> verticesParametric;

    std::vector<int> verticesSizes;
    std::vector<int> verticeSizeRadians;
    std::vector<int> verticesSizeParametric;
    
    std::vector<float (*)(float)> functions;
    std::vector<float (*)(float)> functionsRadians;
    std::vector<parametricObject> functionParametric;
    
    std::vector<std::vector<float>> graphColors;
    std::vector<std::vector<float>> graphColorsRadians;
    std::vector<std::vector<float>> graphColorsParametric;

    void initBox();
    
    GraphApp *app;
    GLFWwindow *window;
    
    GLuint shader_id;
    
    int windowWidth;
    int windowHeight;
    
    int prevWidth, prevHeight;
    
    bool isFullscreen = false;
    float width = WINDOW_WIDTH, height = WINDOW_HEIGHT;

    Shader *shader;

    float unitX = 1.0f, unitY = 1.0f;

    float scale = 2.5f;
    
    float frag = 1.0f;
    int prevPosX = 0.0f, prevPosY = 0.0f;
    float pixel_per_unit = GraphUtilities::DEFAULT_PIXEL_PER_UNIT;
    float panOffsetX = GraphUtilities::DEFAULT_PAN_X_OFFSET, panOffsetY = GraphUtilities::DEFAULT_PAN_Y_OFFSET;
    float aspect_ratio;
    float yratio = GraphUtilities::DEFAULT_YRATIO;
    int steps;
    
    public:
    // --- Singleton Access Point ---
    ~Graph() {};
    static Graph *getInstance();
    void onKeyPressedOnceCallback(const KeyEvent &) override;
    static void frame_size_buffer_callback(GLFWwindow*, int, int);

    void setFYA(float, float, float, float);

    inline float maxPanXX() const;
    inline float maxPanYY() const;

    void insertVertices(float(float));
    void insertVertices(std::vector<float (*)(float)>);
    void insertVerticesRadians(float(float));
    void insertVerticesRadians(std::vector<float (*)(float)>);
    void insertVerticesParametric(std::pair<float, float>(float), float, float);
    void insertVerticesParametric(std::vector<std::pair<float, float> (*)(float)>, float, float);
    // void updateVertices();
    void toggleFullscreen();
    float normalizeX(float x);
    float normalizeY(float y);
    void draw();
    void drawBox();
    void drawAxis();
    void generateGrid();
    void drawGrid();
    void run();
};

#endif