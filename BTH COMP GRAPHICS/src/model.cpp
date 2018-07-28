#include "model.hpp"
#include <algorithm>
#include <iostream>
#include "game.hpp"

namespace anim
{

skeleton_array<glm::mat4> offsets;

static constexpr glm::mat4 ai_to_glm(const aiMatrix4x4& mat)
{
    return
    {
        mat.a1, mat.b1, mat.c1, mat.d1,
        mat.a2, mat.b2, mat.c2, mat.d2,
        mat.a3, mat.b3, mat.c3, mat.d3,
        mat.a4, mat.b4, mat.c4, mat.d4
    };

     /*return
     {
        mat.a1, mat.a2, mat.a3, mat.a4,
        mat.b1, mat.b2, mat.b3, mat.b4,
        mat.c1, mat.c2, mat.c3, mat.c4,
        mat.d1, mat.d2, mat.d3, mat.d4
    };*/
}

void load_mesh(const aiMesh* mesh, std::vector<vertex>& vertices, std::vector<unsigned int>& indices)
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
        vertex.joints = glm::ivec4(0, 0, 0, 0);
        vertex.weights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

	for (auto i = 0u; i < mesh->mNumBones; ++i)
	{
		auto* bone = mesh->mBones[i];

		offsets[i] = ai_to_glm(bone->mOffsetMatrix);

		for (auto j = 0u; j < bone->mNumWeights; ++j)
		{
            auto& weight = bone->mWeights[j];

            if(vertices[weight.mVertexId].weights.x == 0.0f)
            {
                vertices[weight.mVertexId].joints.x = i;
                vertices[weight.mVertexId].weights.x = weight.mWeight;
            }
            else if(vertices[weight.mVertexId].weights.y == 0.0f)
            {
                vertices[weight.mVertexId].joints.y = i;
                vertices[weight.mVertexId].weights.y = weight.mWeight;
            }
            else if(vertices[weight.mVertexId].weights.z == 0.0f)
            {
                vertices[weight.mVertexId].joints.z = i;
                vertices[weight.mVertexId].weights.z = weight.mWeight;
            }
            else if(vertices[weight.mVertexId].weights.w == 0.0f)
            {
                vertices[weight.mVertexId].joints.w = i;
                vertices[weight.mVertexId].weights.w = weight.mWeight;
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

        auto& joint = joints[index - 1];
        if(&joint == &joints.front())
        {
            joint.local_transform = ai_to_glm(node.mTransformation);
            joint.global_transform = joint.local_transform;
            joint.inverse_bind_pose = glm::inverse(joint.global_transform);
        }
    }

    for (auto i = 0u; i < names.size(); ++i)
    {
        if(names[i].compare(node.mParent->mName.C_Str()) == 0)
        {
            auto& joint = joints[index - 1];
            auto* parent = &joints[i];

            joint.parent = parent;
            joint.local_transform = ai_to_glm(node.mTransformation);
            joint.global_transform = parent->global_transform * joint.local_transform;
            joint.inverse_bind_pose = glm::inverse(joint.global_transform);
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
            if(i == 0u)
            {
                using namespace std::chrono;
                key_frames[j].time_point =
                    duration_cast<milliseconds>(duration<float>
                    (channel->mPositionKeys[j].mTime));
            }

            aiVector3D v = channel->mPositionKeys[j].mValue;
            key_frames[j].poses[i].position = glm::vec3(v.x, v.y, v.z);

            aiQuaternion q = channel->mRotationKeys[j].mValue;
            key_frames[j].poses[i].rotation = glm::quat(q.w, q.x, q.y, q.z);
		}
	}
}

void import_model(const std::string& path,
	std::vector<vertex>& vertices,
	std::vector<unsigned int>& indices,
	skeleton& joints,
	std::vector<key_frame>& key_frames)
{
	Assimp::Importer importer;
	auto* scene = importer.ReadFile(path.c_str(),
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
        aiProcess_CalcTangentSpace |
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices);

    load_mesh(scene->mMeshes[0], vertices, indices);
	load_skeleton(scene->mRootNode, joints);
	load_key_frames(scene->mAnimations[0], key_frames);
}

model::model()
{
    std::vector<key_frame> key_frames;

	import_model("models/boblampclean.md5mesh",
        vertices, indices, joints,
        key_frames);

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

void model::update(milliseconds delta_time)
{
	current.update(delta_time, joints);

	for (auto i = 0u; i < joints.size(); ++i)
	{
        world_joints[i] = joints[i].world_transform();
    }
}

void model::draw(const shader& shader) const
{
	shader.uniform("joint_transforms", world_joints);
	model_array.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
