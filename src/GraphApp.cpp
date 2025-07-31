#include "GraphApp.hpp"

GraphApp::GraphApp()
{
    InitWindow();
    loadGLAD();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    shader = new Shader(vertex_shader_file, fragment_shader_file);
    process_input();
    setCallback();
}

int GraphApp::InitWindow()
{
    // initialize glfw
    glfwInit();

    // define window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    // create window
    window = glfwCreateWindow(window_width, window_height, "Shuvo Dev Durjoy", nullptr, nullptr);

    // check if the window is create successfully or return out of the programe terminating the glfw
    if (!window)
    {
        std::cout << "Window Creation Failed" << std::endl;
        glfwTerminate();
        return -1;
    }


    // if window creating is successfull then head to creating current context
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, 400,400, GLFW_DONT_CARE, GLFW_DONT_CARE);
    return 0;
}

int GraphApp::loadGLAD()
{
    // load glad to load opengl function and pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error with Loading the GLAD" << std::endl;
        return -1;
    }
    return 0;
}

void GraphApp::frame_buffer_size_callback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}

void GraphApp::process_input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void GraphApp::setCallback()
{
    // callback for frame buffer size
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void GraphApp::mainLoop(Graph *graph)
{
    while (!glfwWindowShouldClose(window))
    {
        process_input();
        graph->onceKeyPressedPollEvents(window);

        glClearColor(0.06f, 0.07f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        graph->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GraphApp::cleanUp(Graph *graph)
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shader->getID());
    delete graph;
    glfwTerminate();
}

void GraphApp::refreshOpenGL(std::vector<float> &vertices, int initial, int size)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (size - initial) * sizeof(float), vertices.data() + initial, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void GraphApp::setColor(float r, float g, float b){
    shader->setVec3("color", r, g, b);
}

void GraphApp::run(Graph *graph)
{
    mainLoop(graph);
    cleanUp(graph);
}