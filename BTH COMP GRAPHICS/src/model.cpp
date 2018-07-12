#include "model.hpp"
#include <algorithm>
#include <iostream>
#include "game.hpp"

void load_mesh(const aiMesh* mesh, std::vector<vertex>& vertices, std::vector<unsigned int>& indices)
{
	vertices.resize(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i].position.x = mesh->mVertices[i].x;
		vertices[i].position.y = mesh->mVertices[i].y;
		vertices[i].position.z = mesh->mVertices[i].z;

		vertices[i].texture_coordinate.x = mesh->mTextureCoords[0][i].x;
		vertices[i].texture_coordinate.y = mesh->mTextureCoords[0][i].y;

		vertices[i].normal.x = mesh->mNormals[i].x;
		vertices[i].normal.y = mesh->mNormals[i].y;
		vertices[i].normal.z = mesh->mNormals[i].z;
		vertices[i].normal.z = mesh->mNormals[i].z;
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		aiBone* bone = mesh->mBones[i];
		for (unsigned int j = 0; j < bone->mNumWeights; j++)
		{
            if(vertices[bone->mWeights[j].mVertexId].joints.x == 0)
            {
                vertices[bone->mWeights[j].mVertexId].joints.x = i;
                vertices[bone->mWeights[j].mVertexId].weights.x = bone->mWeights[j].mWeight;
            }
            else if(vertices[bone->mWeights[j].mVertexId].joints.y == 0)
            {
                vertices[bone->mWeights[j].mVertexId].joints.y = i;
                vertices[bone->mWeights[j].mVertexId].weights.y = bone->mWeights[j].mWeight;
            }
            else if(vertices[bone->mWeights[j].mVertexId].joints.z == 0)
            {
                vertices[bone->mWeights[j].mVertexId].joints.z = i;
                vertices[bone->mWeights[j].mVertexId].weights.z = bone->mWeights[j].mWeight;
            }
            else if(vertices[bone->mWeights[j].mVertexId].joints.w == 0)
            {
                vertices[bone->mWeights[j].mVertexId].joints.w = i;
                vertices[bone->mWeights[j].mVertexId].weights.w = bone->mWeights[j].mWeight;
            }
		}
	}
}

void load_skeleton(const aiMesh* mesh, skeleton& joints)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		aiMatrix3x3 mat3;
		aiMatrix4x4 mat4 = mesh->mBones[i]->mOffsetMatrix;
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
		glm::quat rot(quat.x, quat.y, quat.z, quat.w);

		joints[i].position = pos;
		joints[i].rotation = rot;
	}
}

void load_key_frames(const aiAnimation* anim, std::vector<key_frame>& key_frames)
{
	for (unsigned int i = 0; i < anim->mNumChannels; i++)
	{

	}
}

void import_model(const std::string& path,
	std::vector<vertex>& vertices,
	skeleton& joints,
	std::vector<key_frame>& key_frames)
{
	/*Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(),
	aiProcess_Triangulate |
	aiProcess_SortByPType |
	aiProcess_GenSmoothNormals |
	aiProcess_FlipUVs);
	load_mesh(scene, vertices);*/
}

void import_model(const std::string& path,
	std::vector<vertex>& vertices,
	std::vector<unsigned int>& indices,
	skeleton& joints)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs);

    std::vector<key_frame> k;
	load_mesh(scene->mMeshes[0], vertices, indices);
	load_skeleton(scene->mMeshes[0], joints);
	load_key_frames(scene->mAnimations[0], k);
}

model::model()
	: model_buffer(target::ARRAY_BUFFER)
	, element_buffer(target::ELEMENT_ARRAY_BUFFER)
{
	/*vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,1,0), glm::vec2(1,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });

	vertices.push_back({ glm::vec3(0,0,0), glm::vec2(0,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });*/

	import_model("models/boblampclean.md5mesh", vertices, indices, joints);

	int stride = 12 * sizeof(float) + 4 * sizeof(int);

	model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	element_buffer.data(sizeof(unsigned int) * indices.size(), &indices.front(), GL_STATIC_DRAW);
	model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	model_array.attribute_pointer(3, 4, GL_INT, stride, (void*)(8 * sizeof(float)));
	model_array.attribute_pointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float) + 4 * sizeof(int)));

	joints.fill({ 0, glm::vec3(0.0f), glm::quat_cast(glm::mat4(1.0f)) });
	world_joints.fill(glm::mat4(1.0f));
}

model::~model()
{
	//dtor
}

void model::update(float delta_time)
{
	current.update(delta_time, joints);

	for (unsigned int i = 0; i < joints.size(); i++)
	{
		glm::mat4 new_transform = glm::mat4_cast(joints[i].rotation);
		new_transform = glm::translate(new_transform, joints[i].position);

		world_joints[i] = glm::mat4(1.0f);//new_transform;

		for (int j = joints[i].parent;
			joints[j].parent != 0;
			j = joints[j].parent)
		{
			glm::mat4 parent_transform = glm::mat4_cast(joints[j].rotation);
			parent_transform = glm::translate(new_transform, joints[j].position);

			world_joints[i] *= parent_transform;
		}
	}
}

void model::draw(const shader& shader) const
{
	shader.uniform("joint_transforms", world_joints);
	model_array.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}
