#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "shader.hpp"
#include "camera.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"

class directional_light
{
public:
    directional_light(
        glm::vec3 direction,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular);

    void bind(const shader& shader);

    void shadows_bind();

private:
    glm::mat4 light_space_matrix;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    camera shadow_camera;
    texture shadow_texture;
    frame_buffer shadow_buffer;
};

#endif // DIRECTIONAL_LIGHT_HPP
