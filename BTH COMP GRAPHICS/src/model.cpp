#include "model.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

model::model(const mesh_type& type, shader * shader_ptr)
	:mesh(type)
{
	model_matrix = glm::mat4(1.);
	this->shader_ptr = shader_ptr;
}

model::~model()
{
}

void model::render(const glm::mat4& view_projection)
{
	shader_ptr->use();
	if (mesh.has_textures())
	{
		mesh.use_textures(shader_ptr);
	}

	//model_matrix = glm::translate(model_matrix, glm::vec3(0.f, 0.f, 0.f));

	glm::mat4 mvp = view_projection * model_matrix;

	shader_ptr->uniform("mvp", mvp);

	mesh.draw();
}
