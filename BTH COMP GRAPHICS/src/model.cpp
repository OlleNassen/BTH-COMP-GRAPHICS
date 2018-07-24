#include "model.hpp"
#include <algorithm>
#include <iostream>
#include "game.hpp"

void load_mesh(const aiMesh* mesh, std::vector<vertex>& vertices, std::vector<unsigned int>& indices, std::array<glm::mat4, 50>& offset)
{
	vertices.resize(mesh->mNumVertices);

	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		vertices[i].position.x = mesh->mVertices[i].x;
		vertices[i].position.y = mesh->mVertices[i].y;
		vertices[i].position.z = mesh->mVertices[i].z;

		vertices[i].texture_coordinate.x = mesh->mTextureCoords[0][i].x;
		vertices[i].texture_coordinate.y = mesh->mTextureCoords[0][i].y;

		vertices[i].normal.x = mesh->mNormals[i].x;
		vertices[i].normal.y = mesh->mNormals[i].y;
		vertices[i].normal.z = mesh->mNormals[i].z;
	}

	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		auto face = mesh->mFaces[i];
		for (auto j = 0u; j < face.mNumIndices; ++j)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	for(auto& vertex : vertices)
    {
        vertex.joints = glm::ivec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

	for (auto i = 0u; i < mesh->mNumBones; ++i)
	{
		offset[i] = glm::transpose(ai_to_glm(mesh->mBones[i]->mOffsetMatrix));

		auto* bone = mesh->mBones[i];
		for (auto j = 0u; j < bone->mNumWeights; ++j)
		{
            if(vertices[bone->mWeights[j].mVertexId].joints.x == 0.0f)
            {
                vertices[bone->mWeights[j].mVertexId].joints.x = i;
                vertices[bone->mWeights[j].mVertexId].weights.x = bone->mWeights[j].mWeight;
            }
            else if(vertices[bone->mWeights[j].mVertexId].joints.y == 0.0f)
            {
                vertices[bone->mWeights[j].mVertexId].joints.y = i;
                vertices[bone->mWeights[j].mVertexId].weights.y = bone->mWeights[j].mWeight;
            }
            else if(vertices[bone->mWeights[j].mVertexId].joints.z == 0.0f)
            {
                vertices[bone->mWeights[j].mVertexId].joints.z = i;
                vertices[bone->mWeights[j].mVertexId].weights.z = bone->mWeights[j].mWeight;
            }
            else if(vertices[bone->mWeights[j].mVertexId].joints.w == 0.0f)
            {
                vertices[bone->mWeights[j].mVertexId].joints.w = i;
                vertices[bone->mWeights[j].mVertexId].weights.w = bone->mWeights[j].mWeight;
            }
		}
	}
}

void load_parent_names(const aiNode& node, std::vector<std::string>& names)
{
    if(node.mName.C_Str()[0] != '<')
    {
        names.emplace_back(node.mName.C_Str());
    }

    for (auto i = 0u; i < node.mNumChildren; ++i)
	{
        load_parent_names(*node.mChildren[i], names);
	}
}

void load_parent_indices(const aiNode& node, const std::vector<std::string>& names, int& index, skeleton& joints)
{
    if(node.mName.C_Str()[0] != '<')
    {
        ++index;
    }

    aiMatrix3x3 mat3;
    aiMatrix4x4 mat4 = node.mTransformation;
    glm::vec3 pos;

    pos.x = mat4.d1;
    pos.y = mat4.d2;
    pos.z = mat4.d3;

    mat3.a1 = mat4.a1;
    mat3.a2 = mat4.a2;
    mat3.a3 = mat4.a3;
    mat3.b1 = mat4.b1;
    mat3.b2 = mat4.b2;
    mat3.b3 = mat4.b3;
    mat3.c1 = mat4.c1;
    mat3.c2 = mat4.c2;
    mat3.c3 = mat4.c3;

    aiQuaternion quat(mat3);
    glm::quat rot(quat.w, quat.x, quat.y, quat.z);

    joints[index].position = pos;
    joints[index].rotation = rot;

    for (auto i = 0u; i < names.size(); ++i)
    {
        if(names[i].compare(node.mParent->mName.C_Str()) == 0)
        {
            joints[index - 1].parent = i;
        }
    }

    for (auto i = 0u; i < node.mNumChildren; ++i)
    {
        load_parent_indices(*node.mChildren[i], names, index, joints);
    }
}

void load_skeleton(const aiNode* node, skeleton& joints)
{
    std::vector<std::string> names;
    for (auto i = 0u; i < node->mNumChildren; ++i)
	{
        load_parent_names(*node->mChildren[i], names);
	}

	auto index = 0;
	for (auto i = 0u; i < node->mNumChildren; ++i)
    {
        load_parent_indices(*node->mChildren[i], names, index, joints);
    }
}

void load_key_frames(const aiAnimation* anim, std::vector<key_frame>& key_frames)
{
	key_frames.resize(anim->mChannels[0]->mNumPositionKeys);

	for (auto i = 0u; i < anim->mNumChannels; ++i)
	{
        auto* channel = anim->mChannels[i];

        for (auto j = 0u; j < channel->mNumPositionKeys; ++j)
		{
            if(i == 0)
            {
                key_frames[j].time = channel->mPositionKeys[j].mTime;
            }
            else
            {
                key_frames[j].time =
                    channel->mPositionKeys[j].mTime -
                    channel->mPositionKeys[j - 1].mTime;
            }

            aiVector3D v = channel->mPositionKeys[j].mValue;
            key_frames[j].pose[i].position = glm::vec3(v.x, v.y, v.z);

            aiQuaternion q = channel->mRotationKeys[j].mValue;
            key_frames[j].pose[i].rotation = glm::quat(q.w, q.x, q.y, q.z);
		}
	}
}

void import_model(const std::string& path,
	std::vector<vertex>& vertices,
	std::vector<unsigned int>& indices,
	skeleton& joints,
	std::vector<key_frame>& key_frames,
	glm::mat4& global_inverse_transform,
	std::array<glm::mat4, 50>& offset)
{
	Assimp::Importer importer;
	auto* scene = importer.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs);

    global_inverse_transform =
        glm::inverse(ai_to_glm(scene->mRootNode->mTransformation));

    load_mesh(scene->mMeshes[0], vertices, indices, offset);

	load_skeleton(scene->mRootNode, joints);
	load_key_frames(scene->mAnimations[0], key_frames);
}

model::model()
	: model_buffer(target::ARRAY_BUFFER)
	, element_buffer(target::ELEMENT_ARRAY_BUFFER)
{
    std::vector<key_frame> key_frames;
    joints.fill({ 0, glm::vec3(0.0f), glm::quat_cast(glm::mat4(1.0f)) });

	import_model("models/boblampclean.md5mesh",
        vertices, indices, joints,
        key_frames, global_inverse_transform, offset);

	current.load(key_frames);

	auto stride = 12 * sizeof(float) + 4 * sizeof(int);

	model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	element_buffer.data(sizeof(unsigned int) * indices.size(), &indices.front(), GL_STATIC_DRAW);
	model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	model_array.attribute_pointer(3, 4, GL_INT, stride, (void*)(8 * sizeof(float)));
	model_array.attribute_pointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float) + 4 * sizeof(int)));

	world_joints.fill(glm::mat4(1.0f));
}

model::~model()
{
	//dtor
}

void model::update(const std::chrono::milliseconds delta_time)
{
	current.update(delta_time, joints);

	for (auto i = 0u; i < joints.size(); ++i)
	{
		glm::mat4 new_transform(1.0f);
        new_transform *= glm::translate(new_transform, joints[i].position);
        new_transform *= glm::mat4_cast(joints[i].rotation);

		world_joints[i] = new_transform * world_joints[joints[i].parent];
	}

	for (auto i = 0u; i < world_joints.size(); ++i)
	{
	    world_joints[i] = global_inverse_transform * world_joints[i] * offset[i];
	}
}

void model::draw(const shader& shader) const
{
	shader.uniform("joint_transforms", world_joints);
	model_array.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
