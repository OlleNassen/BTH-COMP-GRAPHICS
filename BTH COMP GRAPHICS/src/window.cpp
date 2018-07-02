#include "window.hpp"
#include <iostream>
#include <map>

std::function<void(float, float)> mouse_callback;
std::map<std::string, std::function<void()>> keymap_pressed;
std::map<std::string, std::function<void()>> keymap_released;
std::map<key, std::string> keybinds;

void bind_key(const std::string& name, key keybind)
{
    keybinds.insert(std::pair<key, std::string>(keybind, name));
}

void key_callback(GLFWwindow* window, int key_id, int scancode, int action, int mods)
{
    std::string name = keybinds[static_cast<key>(key_id)];

    if(action == GLFW_PRESS)
    {
        std::function<void()> callback = keymap_pressed[name];
        if(callback)
        {
            callback();
        }
    }
    if(action == GLFW_RELEASE)
    {
        std::function<void()> callback = keymap_released[name];
        if(callback)
        {
            callback();
        }
    }

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(mouse_callback)
    {
        mouse_callback(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

void window::print_test1()
{
    std::cout << "FUNKAR!!!" << std::endl;
}

void window::print_test2(float x, float y)
{
    std::cout << "FUNKAR!!!" << std::endl;
}

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

    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);
    glfwSetKeyCallback(glfw_window, key_callback);

    assign_mouse_callback(std::bind(&window::print_test2, this,
        std::placeholders::_1, std::placeholders::_2));

    //bind_key("attack", key::U);
    //assign_key_callback("attack", std::bind(&window::print_test1, this), std::bind(&window::print_test1, this));

    bind_key("up", key::W);
    bind_key("up", key::S);
    bind_key("left", key::A);
    bind_key("right", key::D);
}

window::~window()
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

void window::assign_mouse_callback(std::function<void(float, float)> callback)
{
    mouse_callback = callback;
}

void window::assign_key_callback(const std::string& name,
        std::function<void()> pressed,
        std::function<void()> released)
{
    keymap_pressed.insert(std::pair<std::string,
        std::function<void()>>(name, pressed));

    keymap_released.insert(std::pair<std::string,
        std::function<void()>>(name, released));
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
