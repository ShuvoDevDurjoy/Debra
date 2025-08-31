#include "graph.hpp"
#include <cmath>

Graph *Graph::instance = nullptr;

namespace GraphUtilities
{
    float DEFAULT_FRAG = 1.0f;
    float DEFAULT_YRATIO = 1.0f;
    float DEFAULT_PIXEL_PER_UNIT = 10.0f;
    float DEFAULT_PAN_Y_OFFSET = 0.0f;
    float DEFAULT_PAN_X_OFFSET = 0.0f;
    float MAX_PIXEL_PER_UNIT = 400.0f;
    float MIN_PIXEL_PER_UNIT = 15.0f;
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
        return (degree * M_PI) / 180.0f;
    }

};

Graph::Graph(float s)
{
    app = new GraphApp();
    this->window = app->getWindow();
    this->shader = app->getShader();

    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    this->prevHeight = windowHeight;
    this->prevWidth = windowWidth;

    this->steps = (windowWidth / GraphUtilities::MIN_PIXEL_PER_UNIT);

    this->unitX = normalizeX(1.0f);
    this->unitY = normalizeY(1.0f);
    this->speed = s == 0.0f ? 0.1f * 100.0f : s * 100.0f;

    this->cartisanReset = ((steps) / 0.01f) / speed;
    this->radiaReset = ((GraphUtilities::toRadians(360.0f) / GraphUtilities::toRadians(0.01f))) / (speed);

    initBox();
    generateGrid();
    glfwSetFramebufferSizeCallback(window, frame_size_buffer_callback);
}

void Graph::frame_size_buffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    instance->windowWidth = width;
    instance->windowHeight = height;
}

void Graph::initBox()
{
    boxVerticess.setColor(new GraphColor(0.40f, 0.40f, 0.70f));
    boxVerticess.add({-0.997f, -0.997f});
    boxVerticess.add({ 0.997f, -0.997f});
    boxVerticess.add({ 0.997f, -0.997f});
    boxVerticess.add({ 0.997f,  0.997f});
    boxVerticess.add({ 0.997f,  0.997f});
    boxVerticess.add({-0.997f,  0.997f});
    boxVerticess.add({-0.997f,  0.997f});
    boxVerticess.add({-0.997f, -0.997f});
    boxVerticess.setRangeSize(boxVerticess.getSize());
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
        prevPosY -= 10.0f;

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
}

void Graph::generateGrid()
{

    gridLines.setColor(new GraphColor(0.12f, 0.13f, 0.25f));
    for (int x = -steps / 2; x <= steps / 2; x += 1)
    {
        gridLines.add({normalizeX(x), 1.0f});
        gridLines.add({normalizeX(x), -1.0f});
    }
    float stepy = windowHeight / GraphUtilities::MIN_PIXEL_PER_UNIT;
    for (int y = -stepy / 2; y <= stepy / 2; y += 1)
    {
        gridLines.add({-1.0f,normalizeY(y)});
        gridLines.add({1.0f, normalizeY(y)});
    }
    gridLines.setRangeSize(gridLines.getSize());
}

float Graph::normalizeX(float x)
{
    return (x * 2.0f) / steps;
}

float Graph::normalizeY(float y)
{
    return (2.0f * y) / (windowHeight / GraphUtilities::MIN_PIXEL_PER_UNIT);
}

void Graph::drawBox()
{
    GraphColor *gc = boxVerticess.getColor();
    app->setColor(gc->RED, gc->GREEN, gc->BLUE);
    app->refreshOpenGL(boxVerticess.points, 0, boxVerticess.getSize());
    glLineWidth(4.0f);
    glDrawArrays(GL_LINES, 0, boxVerticess.getSize() / 2);
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
    app->refreshOpenGL(gridLines.points, 0, gridLines.getSize());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gridLines.getSize() / 2);

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

void Graph::draw(int tick)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    app->setColor(0.12f, 0.13f, 0.25f);
    drawGrid();

    shader->setVec2("position", scale, scale);
    shader->setVec2("translate", panOffsetX * scale * unitX, panOffsetY * scale * unitY);
    int ticks = tick % (int)(speed * 2.0f + (speed < 100 ? 100 : speed > 1000 ? 1000 : speed));
    if(ticks == 0) ticks = 1;
    for(auto &g: graphs){
        int size = std::min(g.getSize(), g.getRangeSize() * ticks);
        if(g.ANIMATION_MODE == AnimationMode::ONCE && size == g.getSize()){
            g.setRangeSize(g.getSize());
        }
        app->refreshOpenGL(g.points, 0, size);
        glLineWidth(2.5f);
        GraphColor *c = g.getColor();
        app->setColor(c->RED, c->GREEN, c->BLUE);
        glDrawArrays(GL_LINE_STRIP, 0, size / 2);
    }
    shader->setVec2("position", 1.0f, 1.0f);
    shader->setVec2("translate", 0.0f, 0.0f);
    app->setColor(0.40f, 0.40f, 0.70f);
    shader->setVec2("translate", 0.0f, 0.0f);
    shader->setVec2("position", 1.0f, 1.0f);
    drawBox();
}

Graph *Graph::getInstance(float speed)
{
    if (instance)
    {
        std::cerr << "\033[31mError: Graph instance already exists! Only one instance of Graph is allowed.\033[0m\n";
        std::exit(EXIT_FAILURE); // Exit immediately
    }

    instance = new Graph(speed);
    return instance;
}

void Graph::run()
{
    app->run(instance);
}
