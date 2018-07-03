#pragma once
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "texture.hpp"

class model
{
public:
	model(const mesh_type& type, shader* shader_ptr);
	~model();

	void render(const shader& shader, const camera& camera);
	void render(const shader& shader);

	void render(const glm::mat4& view, const glm::mat4& projection);
	void move(const glm::vec3& offset);
	void set_position(const glm::vec3& position);
	void set_texture(const texture_type& type_texture);
private:
	shader* shader_ptr;
	mesh mesh;
	glm::mat4 model_matrix;
};
