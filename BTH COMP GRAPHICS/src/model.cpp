#include "model.hpp"
#include <algorithm>

void load_mesh(const aiScene* scene, std::vector<vertex>& vertices)
{
    for(int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
    {
        vertices[i].position.x = scene->mMeshes[0]->mVertices[i].x;
        vertices[i].position.y = scene->mMeshes[0]->mVertices[i].y;
        vertices[i].position.z = scene->mMeshes[0]->mVertices[i].z;

        vertices[i].texture_coordinate.x = scene->mMeshes[0]->mTextureCoords[i]->x;
        vertices[i].texture_coordinate.y = scene->mMeshes[0]->mTextureCoords[i]->y;

        vertices[i].normal.x = scene->mMeshes[0]->mNormals[i].x;
        vertices[i].normal.y = scene->mMeshes[0]->mNormals[i].y;
        vertices[i].normal.z = scene->mMeshes[0]->mNormals[i].z;

        vertices[i].joints.x = scene->mMeshes[0]->mBones[i]->mWeights[0].mVertexId;
        vertices[i].joints.y = scene->mMeshes[0]->mBones[i]->mWeights[1].mVertexId;
        vertices[i].joints.z = scene->mMeshes[0]->mBones[i]->mWeights[2].mVertexId;
        vertices[i].joints.w = scene->mMeshes[0]->mBones[i]->mWeights[3].mVertexId;

        vertices[i].weights.x = scene->mMeshes[0]->mBones[i]->mWeights[0].mWeight;
        vertices[i].weights.y = scene->mMeshes[0]->mBones[i]->mWeights[1].mWeight;
        vertices[i].weights.z = scene->mMeshes[0]->mBones[i]->mWeights[2].mWeight;
        vertices[i].weights.w = scene->mMeshes[0]->mBones[i]->mWeights[3].mWeight;
    }
}

void import_model(const std::string& path,
    std::vector<vertex>& vertices,
    std::vector<unsigned int>& parents,
    std::vector<glm::mat4>& joints,
    std::vector<key_frame>& key_frames)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(),
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs);

    load_mesh(scene, vertices);
}

model::model()
    : model_buffer(target::ARRAY_BUFFER)
{
    /*struct vertex
    {
        glm::vec3 position;
        glm::vec2 texture_coordinate;
        glm::vec3 normal;
        glm::ivec4 joints;
        glm::vec4 weights;
    };*/
	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(0,0,0), glm::vec2(0,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });

	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,1,0), glm::vec2(1,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });

    int stride = 12 * sizeof(float) + 4 * sizeof(int);

    model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

    model_array.attribute_pointer(3, 4, GL_INT, stride, (void*)(8 * sizeof(float)));
    model_array.attribute_pointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float) + 4 * sizeof(int)));
}

model::~model()
{
    //dtor
}

void model::update(float delta_time)
{
    skel.update(delta_time);
}

void model::draw(const shader& shader) const
{
    shader.uniform("joint_transforms", skel.transforms());
    model_array.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
