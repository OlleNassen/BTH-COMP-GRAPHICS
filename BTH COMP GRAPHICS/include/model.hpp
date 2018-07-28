#ifndef MODEL_HPP
#define MODEL_HPP

#include "animation.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>

namespace anim
{

static constexpr glm::mat4 ai_to_glm(const aiMatrix4x4& mat);

struct vertex
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec2 texture_coordinate{0.0f, 0.0f};
    glm::vec3 normal{0.0f, 0.0f, 0.0f};
    glm::ivec4 joints{0, 0, 0, 0};
    glm::vec4 weights{0.0f, 0.0f, 0.0f, 0.0f};
};

class model
{
public:
    model();

    void update(milliseconds delta_time);

    void draw(const shader& shader) const;

private:
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    skeleton joints;
    skeleton_array<glm::mat4> world_joints;

    animation current;

    buffer model_buffer;
    buffer element_buffer{target::ELEMENT_ARRAY_BUFFER};
    vertex_array model_array;

};

}

#endif // MODEL_HPP
