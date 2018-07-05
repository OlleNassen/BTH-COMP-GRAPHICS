#include "quad.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

quad::quad(float x, float y, float z)
	: quad_vbo(target::ARRAY_BUFFER), quad_texture(new texture("images/edvard.png"))
	, scene_node(x, y, z)
{
	float vertices[] =
	{
		// positions     // uv
		0.0f, 256.f, 0.f, 0.0f, 1.0f,
		256.f, 0.0f, 0.f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.f, 0.0f, 0.0f,

		0.0f, 256.f, 0.f, 0.0f, 1.0f,
		256.f, 256.f, 0.f, 1.0f, 1.0f,
		256.f, 0.0f, 0.f, 1.0f, 0.0f
	};

	quad_array.bind();
	quad_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	int offset = 0;
	quad_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;
	quad_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;
}

quad::~quad()
{
}

void quad::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
	transform = glm::scale(transform, glm::vec3(0.1,0.1,1));
}

void quad::render_current(const shader & shader, const glm::mat4 & world_transform) const
{
	shader.uniform("model", world_transform);
	quad_texture->uniform(shader, "object_material.diffuse", 0);
	quad_array.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void quad::set_texture(const std::string & path)
{
	delete quad_texture;
	quad_texture = new texture(path, wrap::REPEAT, filter::LINEAR, format::RGB);
}
