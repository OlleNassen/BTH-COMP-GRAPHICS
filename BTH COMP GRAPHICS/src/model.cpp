#include "model.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

model::model(const mesh_type& type, shader * shader_ptr, texture* texture_ptr)
	:mesh(type)
{
	model_matrix = glm::mat4(1.);
	this->shader_ptr = shader_ptr;
	this->texture_ptr = texture_ptr;
}

model::~model()
{
}

void model::render(const glm::mat4& viewProjection)
{
	shader_ptr->use();
	if (texture_ptr)
	{
		shader_ptr->uniform("image", 0);
		texture_ptr->uniform(*shader_ptr,0);
	}

	model_matrix = glm::translate(model_matrix, glm::vec3(0.f, 0.f, 0.f));

	glm::mat4 mvp = viewProjection * model_matrix;

	shader_ptr->uniform("mvp", mvp);

	mesh.draw();
}
