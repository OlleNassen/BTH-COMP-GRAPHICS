#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <glm/glm.hpp>
#include "skeleton.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>

struct vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinate;
    glm::vec3 normal;
    glm::ivec4 joints;
    glm::vec4 weights;
};

void import_model(const std::string& path,
    std::vector<vertex>& vertices,
    std::vector<unsigned int> parents,
    std::vector<glm::mat4> joints,
    std::vector<key_frame>& key_frames);

class model
{
public:
    model();
    ~model();

    void update(float delta_time);

    void draw(const shader& shader) const;

private:
    std::vector<vertex> vertices;

    skeleton skel;

    buffer model_buffer;
    vertex_array model_array;

};

#endif // MODEL_HPP
