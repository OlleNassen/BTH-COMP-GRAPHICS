#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "shader.hpp"

class camera
{
public:
    camera(float left, float right, float bottom, float top);
    camera(float fovy, float width, float height, float near, float far);

    void update(float delta_time);
    glm::mat4 model_view_projection(const glm::mat4& model) const;
    void bind(const shader& shader);

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
};

#endif // CAMERA_HPP
