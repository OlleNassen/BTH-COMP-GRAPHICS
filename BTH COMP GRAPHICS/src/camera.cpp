#include "camera.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

camera::camera(float left, float right, float bottom, float top)
    : projection(glm::ortho(left, right, bottom, top))
{
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        position += forward * velocity;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        position -= forward * velocity;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        position -= glm::normalize(glm::cross(forward, up)) * velocity;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        position += glm::normalize(glm::cross(forward, up)) * velocity;

    sf::Vector2i new_position = sf::Mouse::getPosition();
	sf::Vector2f current_position = sf::Vector2f(new_position - mouse_position);
	sf::Mouse::setPosition(sf::Vector2i(250, 250));
    mouse_position = sf::Mouse::getPosition();

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

void camera::bind(const shader& shader)
{
    shader.uniform("view_position", position);
}
