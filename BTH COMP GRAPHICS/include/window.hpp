#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class window
{
public:
    window(unsigned int width, unsigned int height,
           const std::string& title);
    ~window();

    bool is_open() const;

    void swap_buffers();
    void poll_events();


private:
    GLFWwindow* glfw_window;
};

#endif // WINDOW_HPP
