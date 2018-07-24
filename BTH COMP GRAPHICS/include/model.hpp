#ifndef MODEL_HPP
#define MODEL_HPP

#include "animation.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>

static aiMatrix4x4 glm_to_ai(glm::mat4 mat)
{
    return aiMatrix4x4(mat[0][0],mat[0][1],mat[0][2],mat[0][3],
        mat[1][0],mat[1][1],mat[1][2],mat[1][3],
        mat[2][0],mat[2][1],mat[2][2],mat[2][3],
        mat[3][0],mat[3][1],mat[3][2],mat[3][3]);
}

static glm::mat4 ai_to_glm(aiMatrix4x4& in_mat)
{
    glm::mat4 tmp;
    tmp[0][0] = in_mat.a1;
    tmp[1][0] = in_mat.b1;
    tmp[2][0] = in_mat.c1;
    tmp[3][0] = in_mat.d1;

    tmp[0][1] = in_mat.a2;
    tmp[1][1] = in_mat.b2;
    tmp[2][1] = in_mat.c2;
    tmp[3][1] = in_mat.d2;

    tmp[0][2] = in_mat.a3;
    tmp[1][2] = in_mat.b3;
    tmp[2][2] = in_mat.c3;
    tmp[3][2] = in_mat.d3;

    tmp[0][3] = in_mat.a4;
    tmp[1][3] = in_mat.b4;
    tmp[2][3] = in_mat.c4;
    tmp[3][3] = in_mat.d4;
    return tmp;
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

    void update(const std::chrono::milliseconds delta_time);

    void draw(const shader& shader) const;

private:
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    skeleton joints;
    std::array<glm::mat4, 50> offset;
    std::array<glm::mat4, 50> world_joints;

    glm::mat4 global_inverse_transform;

    animation current;

    buffer model_buffer;
    buffer element_buffer;
    vertex_array model_array;

};

#endif // MODEL_HPP
