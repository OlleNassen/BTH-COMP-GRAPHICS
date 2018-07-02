#include "camera.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

camera::camera(float left, float right, float bottom, float top)
    : projection(glm::ortho(left, right, bottom, top))
{
    yaw = -80.0f;
    pitch = 0.0f;
    position = glm::vec3(0.0f, 0.0f,  3.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);

    first = true;
}

camera::camera(float fovy, float width, float height, float near, float far)
    : projection(glm::perspective(fovy, width / height, near, far))
{
    yaw = -80.0f;
    pitch = 0.0f;
    position = glm::vec3(0.0f, 0.0f,  3.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
}


void camera::update(float delta_time)
{
    float velocity = 10.0f * delta_time;
    if (glfwGetKey(window_copy, GLFW_KEY_W) == GLFW_PRESS)
        position += forward * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(forward, up)) * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_S) == GLFW_PRESS)
        position -= forward * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(forward, up)) * velocity;

    if(first)
    {
        glfwSetCursorPos(window_copy, 250.0, 250.0);
    }

    glm::vec<2, double, glm::highp> new_position;
    glfwGetCursorPos(window_copy,
        &new_position.x, &new_position.y);

    glm::vec<2, double, glm::highp> current_position = new_position - mouse_position;

    if(first)
    {
        current_position.x = 0;
        current_position.y = 0;
        first = false;
    }

    glfwGetCursorPos(window_copy,
        &mouse_position.x, &mouse_position.y);

    float sensitivity = 0.1f;
    current_position *= sensitivity;
    yaw   += current_position.x;
    pitch -= current_position.y;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    forward = glm::normalize(front);

    view = glm::lookAt(position, position + forward, up);
}

glm::mat4 camera::model_view_projection(const glm::mat4& model) const
{
    return projection * view * model;
}

glm::mat4 camera::get_view_projection() const
{
	return projection * view;
}

void camera::bind(const shader& shader)
{
    shader.uniform("view_position", position);
}

void camera::set_window_copy(GLFWwindow * window_copy)
{
	this->window_copy = window_copy;
}
