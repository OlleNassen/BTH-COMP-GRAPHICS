#include "camera.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

camera::camera(float left, float right, float bottom, float top)
    : projection(glm::ortho(left, right, bottom, top))
{
	lastX = 1280 / 2;
	lastY = 720 / 2;
    yaw = -80.0f;
    pitch = 0.0f;
    position = glm::vec3(0.0f, 0.0f,  3.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
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
        position -= forward * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_S) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(forward, up)) * velocity;
	if (glfwGetKey(window_copy, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(forward, up)) * velocity;

	static double xpos = lastX;
	static double ypos = lastY;

	glfwGetCursorPos(window_copy, &xpos, &ypos);

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

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

void camera::bind(const shader& shader)
{
    shader.uniform("view_position", position);
}

void camera::set_window_copy(GLFWwindow * window_copy)
{
	this->window_copy = window_copy;
}
