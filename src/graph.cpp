#include "graph.hpp"
#include <cmath>

Graph *Graph::instance = nullptr;

namespace GraphUtilities
{
    float DEFAULT_FRAG = 1.0f;
    float DEFAULT_YRATIO = 1.0f;
    float DEFAULT_ASPECT_RATIO = 1.0f;
    float DEFAULT_PIXEL_PER_UNIT = 10.0f;
    float DEFAULT_PAN_Y_OFFSET = 0.0f;
    float DEFAULT_PAN_X_OFFSET = 0.0f;
    float MAX_PIXEL_PER_UNIT = 400.0f;
    float MIN_PIXEL_PER_UNIT = 20.0f;
    float PIXEL_INCREMENT_STEP = 5.0f;
    float DEFAULT_PAN_INCREMENT_STEP = 1.0f;
    int DEFAULT_STEPS = 10.0f;

    float genSin(float x)
    {
        float radians = x * M_PI / 2.0f;
        return sin(radians);
    }
    float genCos(float x)
    {
        float radians = x * M_PI / 2.0f;
        return cos(radians);
    }
    float genTan(float x)
    {
        float radians = x * M_PI / 2.0f;
        return tan(radians);
    }
    float genCosec(float x)
    {
        float radians = x * M_PI / 2.0f;
        return (1.0f / sin(radians));
    }
    float genSec(float x)
    {
        float radians = x * M_PI / 2.0f;
        return (1.0f / cos(radians));
    }
    float genCot(float x)
    {
        float radians = x * M_PI / 2.0f;
        return (1.0f / tan(radians));
    }

    float toRadians(float degree)
    {
        return (degree * M_PI * 1.0f) / 180.0f;
    }

};

Graph::Graph()
{
    app = new GraphApp();
    this->window = app->getWindow();
    this->shader = app->getShader();

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    this->prevHeight = windowHeight;
    this->prevWidth = windowWidth;

    this->aspect_ratio = (windowWidth * 1.0f) / (windowHeight * 1.0f);
    this->steps = (windowWidth / GraphUtilities::MIN_PIXEL_PER_UNIT);

    this->unitX = normalizeX(1.0f);
    this->unitY = normalizeY(1.0f);

    initBox();
    generateGrid();
    glfwSetFramebufferSizeCallback(window, frame_size_buffer_callback);
}

void Graph::frame_size_buffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    instance->windowWidth = width;
    instance->windowHeight = height;
    instance->aspect_ratio = (width * 1.0f) / (height * 1.0f);
}

void Graph::initBox()
{
    this->boxVertices = {
        -0.997f, -0.996f,
        0.997f, -0.996f,

        0.997f, -0.996f,
        0.997f, 0.997f,

        0.997f, 0.997f,
        -0.997f, 0.997f,

        -0.997f, 0.997f,
        -0.997f, -0.996f};
}

void Graph::toggleFullscreen()
{
    if (!window)
        return;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    if (!isFullscreen)
    {

        glfwGetWindowPos(window, &prevPosX, &prevPosY);

        glfwGetFramebufferSize(window, &prevWidth, &prevHeight);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;

        glfwPollEvents();

        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    }
    else
    {

        glfwSetWindowMonitor(window, nullptr, prevPosX, prevPosY, prevWidth, prevHeight, 0);
        isFullscreen = false;

        windowWidth = prevWidth;
        windowHeight = prevHeight;
    }

    glViewport(0, 0, windowWidth, windowHeight);
}

inline float Graph::maxPanXX() const
{
    //  (steps/2) * (scale-1)/scale   →  0 when scale==1, grows as you zoom‑in
    return (steps * 0.5f) * (scale - 1.0f) / scale;
}

inline float Graph::maxPanYY() const
{
    // Steps in Y direction are derived from visible vertical units,
    // which depend on aspect ratio and pixel_per_unit.
    int visibleYUnits = windowHeight / (2.0f * GraphUtilities::MIN_PIXEL_PER_UNIT);
    return visibleYUnits * (scale - 1.0f) / scale;
}

void Graph::onKeyPressedOnceCallback(const KeyEvent &event)
{
    float maxPanX = (steps / 2.0f) * (scale - 1.0f);

    float maxPanY = ((windowHeight / GraphUtilities::MIN_PIXEL_PER_UNIT) / 2.0f) * (scale - 1.0f);

    switch (event.key)
    {
    case GLFW_KEY_HOME:
        panOffsetX = GraphUtilities::DEFAULT_PAN_X_OFFSET;
        panOffsetY = GraphUtilities::DEFAULT_PAN_Y_OFFSET;
        pixel_per_unit = GraphUtilities::DEFAULT_PIXEL_PER_UNIT;
        break;
    case GLFW_KEY_EQUAL:
        if (scale < 50.0f)
        {
            scale += 0.25;
            float maxpanx = maxPanXX();
            panOffsetX = std::clamp(panOffsetX, -maxpanx, maxpanx);
        }
        break;
    case GLFW_KEY_MINUS:
        if (scale > 1.0f)
        {
            scale -= 0.25;
            float maxpanx = maxPanXX();
            panOffsetX = std::clamp(panOffsetX, -maxpanx, maxpanx);
        }
        break;
    case GLFW_KEY_F11:
    {
        toggleFullscreen();
    }
    break;
    case GLFW_KEY_LEFT:
        if ((panOffsetX * scale) > -maxPanX)
            panOffsetX -= GraphUtilities::DEFAULT_PAN_INCREMENT_STEP;
        break;

    case GLFW_KEY_RIGHT:
        if ((panOffsetX * scale) < maxPanX)
            panOffsetX += GraphUtilities::DEFAULT_PAN_INCREMENT_STEP;
        break;

    case GLFW_KEY_UP:
        if ((panOffsetY * scale) < maxPanY)
        {
            panOffsetY += GraphUtilities::DEFAULT_PAN_INCREMENT_STEP;
        }
        break;

    case GLFW_KEY_DOWN:
        if ((panOffsetY * scale) > -maxPanY)
        {
            panOffsetY -= GraphUtilities::DEFAULT_PAN_INCREMENT_STEP;
        }
        break;

    default:
        break;
    }
    // if (changed)
    //     updateVertices();
}

void Graph::generateGrid()
{
    gridLines.clear();
    for (int x = -steps / 2; x <= steps / 2; x += 1)
    {
        gridLines.push_back(normalizeX(x));
        gridLines.push_back(1.0f);
        gridLines.push_back(normalizeX(x));
        gridLines.push_back(-1.0f);
    }
    float stepy = windowHeight / GraphUtilities::MIN_PIXEL_PER_UNIT;
    for (int y = -stepy / 2; y <= stepy / 2; y += 1)
    {
        gridLines.push_back(-1.0f);
        gridLines.push_back(normalizeY(y));
        gridLines.push_back(1.0f);
        gridLines.push_back(normalizeY(y));
    }
}

float Graph::normalizeX(float x)
{
    return (x * 2.0f) / steps;
}

float Graph::normalizeY(float y)
{
    return (2.0f * y) / (windowHeight / GraphUtilities::MIN_PIXEL_PER_UNIT);
}

void Graph::insertVertices(float fn(float))
{
    functions.push_back(fn);
    // float centerX = windowWidth / 2.0f;
    float r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    float g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    float b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    graphColors.push_back({r, g, b});
    for (float i = -(steps / 2.0f) - 1.0f; i <= (steps / 2.0f) + 1.0f; i += 0.01f)
    {
        float x = normalizeX(i);
        float y = normalizeY(fn(i));
        vertices.push_back(x);
        vertices.push_back(y);
    }
    int laterSize = vertices.size();
    verticesSizes.push_back(laterSize);
}

void Graph::insertVerticesRadians(float fn(float))
{
    functionsRadians.push_back(fn);
    float r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    float g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    float b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    graphColorsRadians.push_back({r, g, b});
    for (float i = 0.0f; i <= GraphUtilities::toRadians(360.0f); i += GraphUtilities::toRadians(0.010f))
    {
        float radius = fn(i);

        float x = normalizeX(radius * cos(i));
        float y = normalizeY(radius * sin(i));
        verticesRadians.push_back(x);
        verticesRadians.push_back(y);
    }
    verticeSizeRadians.push_back(verticesRadians.size());
}

void Graph::insertVertices(std::vector<float (*)(float)> fns)
{
    for (auto &fn : fns)
    {
        insertVertices(fn);
    }
}

void Graph::insertVerticesRadians(std::vector<float (*)(float)> fns)
{
    for (auto &fn : fns)
    {
        insertVerticesRadians(fn);
    }
}

void Graph::insertVerticesParametric(std::pair<float, float> fn(float), float minRange = 0.0f, float maxRange = 4.0f)
{
    functionParametric.push_back({fn, minRange, maxRange});

    float r = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    float g = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    float b = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
    graphColorsParametric.push_back({r, g, b});

    float step = 0.01f;
    for (float t = minRange * M_PI; t <= maxRange * M_PI; t += step)
    {
        std::pair<float, float> pt = fn(t);
        float x = normalizeX(pt.first);
        float y = normalizeY(pt.second);
        verticesParametric.push_back(x);
        verticesParametric.push_back(y);
    }

    verticesSizeParametric.push_back(verticesParametric.size());
}

void Graph::insertVerticesParametric(std::vector<std::pair<float, float> (*)(float)> fns, float minRange = 0.0f, float maxRange = 4.0f)
{
    for (auto &fn : fns)
    {
        insertVerticesParametric(fn, minRange, maxRange);
    }
}

void Graph::drawBox()
{
    app->refreshOpenGL(boxVertices, 0, boxVertices.size());
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, boxVertices.size() / 2);
}

void Graph::drawGrid()
{
    std::vector<float> axis = {
        -1.0f, normalizeY(0.0f),
        1.0f, normalizeY(0.0f),
        normalizeX(0.0f), -1.0f,
        normalizeX(0.0f), 1.0f};

    shader->setVec2("position", scale, scale);
    shader->setVec2("translate", 0.0f, 0.0f);
    app->refreshOpenGL(gridLines, 0, gridLines.size());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gridLines.size() / 2);

    shader->setVec2("position", 1.0f, 1.0f);
    shader->setVec2("translate", 0.0f, 0.0f);
    drawAxis();

    shader->setVec2("position", scale, scale);
    shader->setVec2("translate", panOffsetX * scale * unitX, panOffsetY * scale * unitY);

    app->refreshOpenGL(axis, 0, axis.size());
    glLineWidth(3.0f);
    app->setColor(0.30f, 0.34f, 0.50f);
    glDrawArrays(GL_LINES, 0, axis.size() / 2);
}

void Graph::drawAxis()
{
    std::vector<float> axisfixed = {
        -1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, -1.0f,
        0.0f, 1.0f};

    app->refreshOpenGL(axisfixed, 0, axisfixed.size());
    glLineWidth(3.0f);
    app->setColor(0.6f, 0.34f, 0.50f);
    glDrawArrays(GL_LINES, 0, axisfixed.size() / 2);
}

void Graph::draw()
{
    int initial = 0;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // shader->setVec2("position", 1.0f, 1.0f);
    // shader->setVec2("translate", 0.0f, 0.0f);
    // app->setColor(0.30f, 0.34f, 0.50f);
    // shader->setVec2("translate", 0.0f, 0.0f);
    // shader->setVec2("position", 1.0f, 1.0f);
    // drawBox();

    app->setColor(0.12f, 0.13f, 0.25f);
    drawGrid();

    shader->setVec2("position", scale, scale);
    shader->setVec2("translate", panOffsetX * scale * unitX, panOffsetY * scale * unitY);
    int counts = 0;
    for (auto &size : verticesSizes)
    {
        int count = size;

        app->refreshOpenGL(vertices, initial, size);
        glLineWidth(2.5f);

        app->setColor(graphColors[counts][0], graphColors[counts][1], graphColors[counts][2]);
        glDrawArrays(GL_LINE_STRIP, 0, (count - initial) / 2);
        initial = count;
        ++counts;
    }
    initial = 0;
    counts = 0;
    for (auto &size : verticeSizeRadians)
    {
        int count = size;

        app->refreshOpenGL(verticesRadians, initial, size);
        glLineWidth(2.5f);
        app->setColor(graphColorsRadians[counts][0], graphColorsRadians[counts][1], graphColorsRadians[counts][2]);
        glDrawArrays(GL_LINE_STRIP, 0, (count - initial) / 2);
        initial = count;
        ++counts;
    }
    initial = 0;
    counts = 0;
    for (auto &size : verticesSizeParametric)
    {
        int count = size;

        app->refreshOpenGL(verticesParametric, initial, size);
        glLineWidth(2.5f);

        app->setColor(graphColorsParametric[counts][0], graphColorsParametric[counts][1], graphColorsParametric[counts][2]);
        glDrawArrays(GL_LINE_STRIP, 0, (count - initial) / 2);
        initial = count;
        ++counts;
    }

    shader->setVec2("position", 1.0f, 1.0f);
    shader->setVec2("translate", 0.0f, 0.0f);
    app->setColor(0.40f, 0.40f, 0.70f);
    shader->setVec2("translate", 0.0f, 0.0f);
    shader->setVec2("position", 1.0f, 1.0f);
    drawBox();
}

Graph *Graph::getInstance()
{
    if (instance)
    {
        std::cerr << "\033[31mError: Graph instance already exists! Only one instance of Graph is allowed.\033[0m\n";
        std::exit(EXIT_FAILURE); // Exit immediately
    }

    instance = new Graph();
    return instance;
}

void Graph::run()
{
    app->run(instance);
}

void Graph::setFYA(float frag, float yratio, float aspect_ratio, float pixel_per_unit)
{
    this->frag = frag;
    this->yratio = yratio;
    this->aspect_ratio = aspect_ratio;
    this->pixel_per_unit = pixel_per_unit;
}
