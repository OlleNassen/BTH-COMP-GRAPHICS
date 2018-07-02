#include "input.hpp"
#include <iostream>
#include <map>

namespace input
{

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



void assign_window(const window& callback_window)
{
    //callback_window.callback(key_callback, cursor_position_callback);
    glfwSetCursorPosCallback(callback_window.glfw_window, cursor_position_callback);
    glfwSetKeyCallback(callback_window.glfw_window, key_callback);
}

void assign_mouse_callback(std::function<void(float, float)> callback)
{
    mouse_callback = callback;
}

void assign_key_callback(const std::string& name,
        std::function<void()> pressed,
        std::function<void()> released)
{
    keymap_pressed.insert(std::pair<std::string,
        std::function<void()>>(name, pressed));

    keymap_released.insert(std::pair<std::string,
        std::function<void()>>(name, released));
}

}