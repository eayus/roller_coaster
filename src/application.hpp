#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Application {
public:
    Application();
    ~Application();

    void main_loop();

private:
    GLFWwindow* window;
};
