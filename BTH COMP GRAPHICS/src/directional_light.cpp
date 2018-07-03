#include "directional_light.hpp"
#include <GL/glew.h>

directional_light::directional_light(
    glm::vec3 direction,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular)
    : direction(direction)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular)
    , shadow_camera(-10.0f, 10.0f, -10.0f, 10.0f)
    , shadow_texture(1024, 1024, wrap::REPEAT,
        filter::NEAREST, format::DEPTH_COMPONENT, type::FLOAT)
{
    shadow_buffer.bind_texture(shadow_texture);
}

void directional_light::bind(const shader& shader)
{
    shader.uniform("light.direction", direction);
    shader.uniform("light.ambient", ambient);
    shader.uniform("light.diffuse", diffuse);
    shader.uniform("light.specular", specular);
    light_space_matrix =
        shadow_camera.get_projection()
        * shadow_camera.get_view();
    shader.uniform("light_space_matrix", light_space_matrix);
}
