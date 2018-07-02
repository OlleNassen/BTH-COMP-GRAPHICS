#include "model.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

model::model(const mesh_type& type, shader * shader_ptr)
	:mesh(type)
{
	model_matrix = glm::mat4(1.);
	this->shader_ptr = shader_ptr;
}

model::~model()
{
}

void model::render(const glm::mat4& view, const glm::mat4& projection)
{
	model_matrix = glm::mat4(1.);
	mesh.draw(projection * view * model_matrix, *shader_ptr);
}
