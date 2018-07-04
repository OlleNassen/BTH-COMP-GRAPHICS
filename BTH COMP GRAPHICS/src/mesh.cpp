#include "mesh.hpp"

void vertex_bone::add_bone_data(unsigned int bone_index, float weight)
{
    for(unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
    {
        if(weights[i] == 0.0f)
        {
            IDs[i] = bone_index;
            weights[i] = weight;
            return;
        }
    }
}

mesh::mesh(const std::string& filename)
    : index_buffer(target::ELEMENT_ARRAY_BUFFER)
    , position_buffer(target::ARRAY_BUFFER)
    , normal_buffer(target::ARRAY_BUFFER)
    , texture_coordinate_buffer(target::ARRAY_BUFFER)
    , bone_buffer(target::ARRAY_BUFFER)
{
    num_bones = 0;

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    scene = importer.ReadFile(filename.c_str(),
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs);

    if(scene)
    {
        global_inverse_transform =
            scene->mRootNode->mTransformation;
        global_inverse_transform.Inverse();
        init_from_scene(scene, filename);
    }

    glBindVertexArray(0);
}

mesh::~mesh()
{
    //dtor
}

void mesh::init_from_scene(const aiScene* scene, const std::string& filename)
{
    entries.resize(scene->mNumMeshes);
    entries.reserve(sizeof(texture) * scene->mNumMaterials);

    unsigned int num_vertices = 0;
    unsigned int num_indices = 0;

    /* Count the number of vertices and indices */
    for (unsigned int i = 0 ; i < entries.size() ; i++)
    {
        entries[i].material_index = scene->mMeshes[i]->mMaterialIndex;
        entries[i].num_indices = scene->mMeshes[i]->mNumFaces * 3;
        entries[i].base_vertex = num_vertices;
        entries[i].base_index = num_indices;

        num_vertices += scene->mMeshes[i]->mNumVertices;
        num_indices  += entries[i].num_indices;
    }

    /* Reserve space in the vectors for the vertex attributes and indices */
    std::vector<glm::vec3> positions(num_vertices);
    std::vector<glm::vec3> normals(num_vertices);
    std::vector<glm::vec2> texture_coordinates(num_vertices);
    std::vector<unsigned int> indices(num_vertices);
    std::vector<vertex_bone> bones;
    bones.resize(num_vertices);

    /* Initialize the meshes in the scene one by one */
    for (unsigned int i = 0 ; i < entries.size(); i++)
    {
        const aiMesh* ai_mesh = scene->mMeshes[i];
        init_mesh(i, ai_mesh, positions,
            normals, texture_coordinates, bones, indices);
    }

    init_materials(scene, filename);

    /* Generate and populate the buffers with vertex attributes and the indices */
  	position_buffer.data(sizeof(positions[0]) *
        positions.size(), &positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	texture_coordinate_buffer.data(sizeof(texture_coordinates[0]) *
        texture_coordinates.size(), &texture_coordinates[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


   	normal_buffer.data(sizeof(normals[0]) *
              normals.size(), &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


   	bone_buffer.data(sizeof(bones[0]) *
        bones.size(), &bones[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4,
        GL_INT, sizeof(vertex_bone), (const GLvoid*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
        sizeof(vertex_bone), (const GLvoid*)16);


	index_buffer.data(sizeof(indices[0]) *
        indices.size(), &indices[0], GL_STATIC_DRAW);
}

void mesh::init_mesh(unsigned int mesh_index,
    const aiMesh* ai_mesh,
    std::vector<glm::vec3>& positions,
    std::vector<glm::vec3>& normals,
    std::vector<glm::vec2>& texture_coordinates,
    std::vector<vertex_bone>& bones,
    std::vector<unsigned int>& indices)
{
    const aiVector3D zero(0.0f, 0.0f, 0.0f);

    /* Populate the vertex attribute vectors */
    for (unsigned int i = 0 ; i < ai_mesh->mNumVertices ; i++)
    {
        const aiVector3D* position = &(ai_mesh->mVertices[i]);
        const aiVector3D* normal = &(ai_mesh->mNormals[i]);
        const aiVector3D* texture_coordinate =
            ai_mesh->HasTextureCoords(0)
            ? &(ai_mesh->mTextureCoords[0][i]) : &zero;

        positions.push_back(glm::vec3(position->x, position->y, position->z));
        normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
        texture_coordinates.push_back(
            glm::vec2(texture_coordinate->x, texture_coordinate->y));
    }

    load_bones(mesh_index, ai_mesh, bones);

    /* Populate the index buffer */
    for (unsigned int i = 0 ; i < ai_mesh->mNumFaces ; i++)
    {
        const aiFace& face = ai_mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

void mesh::load_bones(
        unsigned int mesh_index,
        const aiMesh* mesh,
        std::vector<vertex_bone>& bones)
{
    for(unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        unsigned int bone_index = 0;
        std::string bone_name(mesh->mBones[i]->mName.data);
        if(bone_mapping.find(bone_name) == bone_mapping.end())
        {
            bone_index = num_bones;
            num_bones++;
            bone_info info;
            bone_infos.push_back(info);
            bone_infos[bone_index].bone_offset = mesh->mBones[i]->mOffsetMatrix;
            bone_mapping[bone_name] = bone_index;
        }
        else
        {
            bone_index = bone_mapping[bone_name];

        }
        for(unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            unsigned int vertex_index = entries[mesh_index].base_vertex +
                mesh->mBones[i]->mWeights[j].mVertexId;

            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            bones[vertex_index].add_bone_data(bone_index, weight);
        }
    }
}

void mesh::init_materials(const aiScene* scene, const std::string& filename)
{
    /* Extract the directory part from the file name */
    std::string::size_type slash_index = filename.find_last_of("/");
    std::string dir;

    if (slash_index == std::string::npos)
    {
        dir = ".";
    }
    else if (slash_index == 0)
    {
        dir = "/";
    }
    else
    {
        dir = filename.substr(0, slash_index);
    }

    /* Initialize the materials */
    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++) {
        const aiMaterial* material = scene->mMaterials[i];


        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path,
                NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string p(path.data);

                if (p.substr(0, 2) == ".\\")
                {
                    p = p.substr(2, p.size() - 2);
                }

                std::string full_path = dir + "/" + p;
                textures.push_back(texture(full_path.c_str())); // OBS KAN CRASHA!!!
            }
        }
    }
}
