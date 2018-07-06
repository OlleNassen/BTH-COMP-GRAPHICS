#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

namespace glm
{
class vec2
{
public:
};
class vec3
{
public:
};
class vec4
{
public:
};
class ivec4
{
public:
};
}

struct vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinate;
    glm::vec3 normal;
    glm::ivec4 joints;
    glm::vec4 weights;
};

class model
{
public:
    model();
    ~model();

private:
    std::vector<vertex> vertices;

};

#endif // MODEL_HPP
