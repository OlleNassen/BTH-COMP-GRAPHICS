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
    last_x = 0.0f;
    last_y = 0.0f;
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

void camera::on_mouse_moved(float x, float y)
{
    if(first)
    {
        last_x = x;
        last_y = y;
        first = false;
    }

    float xoffset = x - last_x;
    float yoffset = last_y - y;
    last_x = x;
    last_y = y;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(front);
}

void camera::update(float delta_time)
{
	if (glfwGetKey(window_copy, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_copy, true);

	float velocity = 10.0f;
	//TURBO SPEED
	if (glfwGetKey(window_copy, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		velocity *= 10;
	}
	velocity *= delta_time;

    if (glfwGetKey(window_copy, GLFW_KEY_W) == GLFW_PRESS)
        position += forward * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(forward, up)) * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_S) == GLFW_PRESS)
        position -= forward * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(forward, up)) * velocity;
}

glm::mat4 camera::model_view_projection(const glm::mat4& model) const
{
    return projection * view * model;
}

glm::mat4 camera::get_view() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 camera::get_projection() const
{
	return projection;
}



void camera::bind(const shader& shader)
{
    shader.uniform("view_position", position);
}

void camera::set_window_copy(GLFWwindow * window_copy)
{
	this->window_copy = window_copy;
}
