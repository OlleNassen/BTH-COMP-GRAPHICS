#include "window.hpp"
#include <iostream>

window::window(
    unsigned int width,
    unsigned int height,
    const std::string& title)
{
    if(!glfwInit())
    {
        std::cout << "Failed to init glfw";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!glfw_window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, width, height);
}

window::~window()
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

bool window::is_open() const
{
    return !glfwWindowShouldClose(glfw_window);
}

void window::swap_buffers()
{
    glfwSwapBuffers(glfw_window);
}

void window::poll_events()
{
    glfwPollEvents();
}
