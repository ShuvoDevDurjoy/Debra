#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>
#include "../include/glad.h"
#include "../include/glfw3.h"
#include "shader.hpp"
#include <climits>
#include <cstdarg>
#include <cmath>
#include "keyClicked.hpp"
#include "GraphApp.hpp"

class GraphApp;

class Graph;

// parametric object for parametric curve
struct parametricObject
{
    std::pair<float, float> (*function)(float);
    float minRange;
    float maxRange;
};

// default window width and height
#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 700

// Variable for Varags to be accessible
// via this Variable
struct Var
{
private:
    // argument for the Varags that will be
    // accessible through array like indexing via
    // overloaded operator
    std::vector<float> args;
    void add(float x)
    {
        args.push_back(x);
    }

    friend class Graph;

public:
    // return size of the Var Object
    int size() { return args.size(); }

    // method overfload for array like indexing
    float operator[](int idx)
    {
        if (idx < size())
        {
            return args[idx];
        }
        return 0.0f;
    }
};

// variant that let the user to input both (float) or (float, Var) function reference
// for cartesian curve
using CartisanFunctionTypeVariant = std::variant<float (*)(float), float (*)(float, Var)>;
// variant that let the user to input both (float) or (float, Var) function reference
// for Polar curve
using RadianFunctionTypeVariant = std::variant<float (*)(float), float (*)(float, Var)>;
// variant that let the user to input both (float) or (float, Var) function reference
// for Parametric curve
using ParametricFunctionTypeVariant = std::variant<std::pair<float, float> (*)(float), std::pair<float, float> (*)(float, Var)>;

struct Ticks
{
    int tickStart;
    int tickEnd;
};

// Structure to convert Cartisian Function to Cartisain Type Variant
// which convert initializer_list to CartesianFunctionTypeVariant
struct CartesianFunctionList
{
private:
    std::vector<CartisanFunctionTypeVariant> funcs;
    friend class Graph;

public:
    template <typename... Fns>
    CartesianFunctionList(Fns... fns) : funcs{CartisanFunctionTypeVariant(fns)...} {}
};

// Structure to convert Radian Function to Radian Type Variant
// which convert initializer_list to RadianFunctionTypeVariant
struct RadianFunctionList
{
private:
    std::vector<RadianFunctionTypeVariant> funcs;
    friend class Graph;

public:
    template <typename... Fns>
    RadianFunctionList(Fns... fns) : funcs{RadianFunctionTypeVariant(fns)...} {}
};

// Structure to convert Parametric Function to Parametric Type Variant
// which convert initializer_list to ParametricFunctionTypeVariant
struct ParametricFunctionList
{
private:
    std::vector<RadianFunctionTypeVariant> funcs;
    friend class Graph;

public:
    template <typename... Fns>
    ParametricFunctionList(Fns... fns) : funcs{ParametricFunctionTypeVariant(fns)...} {}
};

namespace GraphUtilities
{
    extern float DEFAULT_FRAG;
    extern float DEFAULT_YRATIO;
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

struct GraphColor
{
public:
    float RED;
    float GREEN;
    float BLUE;

public:
    GraphColor(){}
    GraphColor(float RED, float GREEN, float BLUE)
    {
        this->RED = RED;
        this->GREEN = GREEN;
        this->BLUE = BLUE;
    }
};

enum AnimationMode{
    ONCE = 0, 
    INFINITE = 1, 
    TIMES = 2
};

struct singletonGraph
{
private:
    GraphColor *color;
    std::vector<float> points;
    int rangeSize;
public:
    AnimationMode ANIMATION_MODE = AnimationMode::INFINITE;

private:
    friend class Graph;

public:
    singletonGraph() {}
    int getSize()
    {
        return points.size();
    }

    void setColor(GraphColor *c){
        this->color = c;
    }

    GraphColor* getColor(){
        return color;
    }

    void setRangeSize(int rs){
        this->rangeSize = rs;
    }

    int getRangeSize(){
        return rangeSize;
    }

    void add(std::pair<float,float> p){
        points.push_back(p.first);
        points.push_back(p.second);
    }

    void setAnimationMode(AnimationMode M){
        this->ANIMATION_MODE = M;
    }

};

class Graph : public KeyClicked
{
private:
    Graph(float = 0.0f);
    static Graph *instance;

    // Delete copy and move operations
    Graph(const Graph &) = delete;
    Graph(Graph &&) = delete;
    Graph &operator=(const Graph &) = delete;
    Graph &operator=(Graph &&) = delete;

private:
    // contains the position vector for the grid lines(the axis) and the box(around the whole window)
    // std::vector<float> gridLines;
    singletonGraph gridLines;
    singletonGraph boxVerticess;

    std::vector<singletonGraph> graphs;

    // contain the graph colors
    std::vector<std::vector<float>> graphColors;
    std::vector<std::vector<float>> graphColorsRadians;
    std::vector<std::vector<float>> graphColorsParametric;

private:
    // contain the increment per ticks for cartisian and polar co-ordiante graph
    int cartisanReset = 0;
    int radiaReset = 0;

    GraphApp *app;
    GLFWwindow *window;
    Shader *shader;

    GLuint shader_id;

    int windowWidth;
    int windowHeight;
    int prevWidth, prevHeight;

    bool isFullscreen = false;

    float width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
    float unitX = 1.0f, unitY = 1.0f;
    float scale = 1.0f;
    int speed = 100;
    float frag = 1.0f;
    int prevPosX = 0.0f, prevPosY = 0.0f;

    float pixel_per_unit = GraphUtilities::DEFAULT_PIXEL_PER_UNIT;
    float panOffsetX = GraphUtilities::DEFAULT_PAN_X_OFFSET, panOffsetY = GraphUtilities::DEFAULT_PAN_Y_OFFSET;
    float yratio = GraphUtilities::DEFAULT_YRATIO;
    int steps;

    AnimationMode ANIMATION_MODE = AnimationMode::INFINITE;

    friend class GraphApp;

private:
    void addToVar(Var &) {}

    template <typename... Args>
    void addToVar(Var &v, float first, Args... args)
    {
        v.add(first);
        addToVar(v, args...);
    }

    void onKeyPressedOnceCallback(const KeyEvent &) override;
    static void frame_size_buffer_callback(GLFWwindow *, int, int);

    inline float maxPanXX() const;
    inline float maxPanYY() const;

    void toggleFullscreen();

    float normalizeX(float x);
    float normalizeY(float y);

    void draw(int);
    void drawBox();
    void drawAxis();

    void generateGrid();
    void drawGrid();

    void initBox();

public:
    // --- Singleton Access Point ---
    ~Graph() {};

    static Graph *getInstance(float = 0.0f);

    template <typename... T>
    void insertVertices(CartisanFunctionTypeVariant, T...);
    template <typename... T>
    void insertVerticesList(CartesianFunctionList, T...);
    template <typename... T>
    void insertVerticesRadians(RadianFunctionTypeVariant, float = 0.0f, float = 2.0f, T...);
    template <typename... T>
    void insertVerticesRadiansList(RadianFunctionList, float = 0.0f, float = 2.0f, T...);
    template <typename... T>
    void insertVerticesParametric(ParametricFunctionTypeVariant, float = 0.0f, float = 4.0f, T...);
    template <typename... T>
    void insertVerticesParametricList(ParametricFunctionList, float = 0.0f, float = 4.0f, T...);

    void setAnimationMode(AnimationMode M){
        ANIMATION_MODE = M;
    }

    void run();
};
#include "graph.tpp"

#endif