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
{

}

void directional_light::bind(const shader& shader)
{
    shader.uniform("light.direction", direction);
    shader.uniform("light.ambient", ambient);
    shader.uniform("light.diffuse", diffuse);
    shader.uniform("light.specular", specular);
}
