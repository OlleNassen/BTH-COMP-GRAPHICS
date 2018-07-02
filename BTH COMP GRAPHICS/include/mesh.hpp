#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>

#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

struct bone_info
{
    aiMatrix4x4 bone_offset;
    aiMatrix4x4 final_transformation;
};

#define NUM_BONES_PER_VERTEX 4
struct vertex_bone
{
    unsigned int IDs[NUM_BONES_PER_VERTEX];
    float weights[NUM_BONES_PER_VERTEX];
    void add_bone_data(unsigned int bone_index, float weight);
};

#define INVALID_MATERIAL 0xFFFFFFFF
struct mesh_entry
{
    mesh_entry()
    {
        num_indices = 0;
        base_vertex = 0;
        base_index = 0;
        material_index = INVALID_MATERIAL;
    }

    unsigned int num_indices;
    unsigned int base_vertex;
    unsigned int base_index;
    unsigned int material_index;
};

class mesh
{
public:
    mesh(const std::string& filename);
    ~mesh();

private:
    void init_from_scene(const aiScene* scene, const std::string& filename);

    void init_mesh(unsigned int mesh_index,
        const aiMesh* ai_mesh,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texture_coordinates,
        std::vector<vertex_bone>& bones,
        std::vector<unsigned int>& indices);

    void load_bones(
        unsigned int mesh_index,
        const aiMesh* mesh,
        std::vector<vertex_bone>& bones);

    void init_materials(const aiScene* scene, const std::string& filename);

    buffer index_buffer;
    buffer position_buffer;
    buffer normal_buffer;
    buffer texture_coordinate_buffer;
    buffer bone_buffer;

    GLuint vertex_array;

    std::vector<bone_info> bone_infos;
    std::vector<mesh_entry> entries;
    //std::vector<texture> textures;
    std::map<std::string, unsigned int> bone_mapping;
    unsigned int num_bones;

    aiMatrix4x4 global_inverse_transform;

    const aiScene* scene;
    Assimp::Importer importer;
};

#endif // MESH_HPP
