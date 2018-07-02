#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "shader.hpp"
#include <gl/glew.h>
#include <GLFW/glfw3.h>

class camera
{
public:
    camera(float left, float right, float bottom, float top);
    camera(float fovy, float width, float height, float near, float far);

    void update(float delta_time);
    glm::mat4 model_view_projection(const glm::mat4& model) const;
    void bind(const shader& shader);
	void set_window_copy(GLFWwindow* window_copy);
private:
    float yaw;
    float pitch;

    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;

    glm::vec2 mouse;
    glm::vec2 mouse_last;

	double lastX;
	double lastY;

	GLFWwindow* window_copy;
};

#endif // CAMERA_HPP
