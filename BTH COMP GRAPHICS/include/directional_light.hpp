#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "shader.hpp"

class directional_light
{
public:
    directional_light(
        glm::vec3 direction,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular);

    void bind(const shader& shader);

private:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif // DIRECTIONAL_LIGHT_HPP
