#pragma once
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"

class model
{
public:
	model(const mesh_type& type, shader* shader_ptr, texture* texture_ptr = nullptr);
	~model();

	void render(const glm::mat4& view_projection);
private:
	shader* shader_ptr;
	mesh mesh;
	glm::mat4 model_matrix;
};