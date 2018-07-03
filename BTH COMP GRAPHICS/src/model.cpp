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

void model::render(const shader& shader, const camera& camera)
{
    shader.use();
    shader.uniform("model", model_matrix);
    shader.uniform("model_view_projection",
        camera.model_view_projection(model_matrix));

    render(shader);
}

void model::render(const shader& shader)
{

}

void model::render(const glm::mat4& view, const glm::mat4& projection)
{
	switch (mesh.get_type())
	{
	case mesh_type::SKYBOX:
		mesh.draw(model_matrix, view, projection, *shader_ptr);
		break;
	default:
		shader_ptr->use();
		shader_ptr->uniform("model", model_matrix);
		mesh.draw(projection * view * model_matrix, *shader_ptr);
		break;
	}

}

void model::move(const glm::vec3 & offset)
{
	model_matrix = glm::translate(model_matrix, offset);
}

void model::set_position(const glm::vec3 & position)
{
	model_matrix = glm::mat4(1);
	model_matrix = glm::translate(model_matrix, position);
}

void model::set_texture(const texture_type & type_texture)
{
	mesh.set_texture(type_texture);
}
