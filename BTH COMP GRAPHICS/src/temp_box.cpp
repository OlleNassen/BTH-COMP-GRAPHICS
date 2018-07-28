#include "temp_box.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{

temp_box::temp_box(float x, float y, float z)
	: node(x, y, z)
{
    constexpr float vertices[] =
	{
		//pos, uv, normal
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,


		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	};

	box_array.bind();
	box_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	box_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	box_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), buffer_offset<float>(3u));

	color = glm::vec3(1.0f, 0.5f, 0.31f);
}

temp_box::~temp_box()
{

}

void temp_box::update_current(milliseconds delta_time,
	const glm::mat4& world_transform, glm::mat4& transform)
{
	glm::vec3 pos = glm::vec3(world_transform[0][0], world_transform[1][0], world_transform[2][0]);
	transform = glm::translate(transform, pos);
}

void temp_box::render_current(const shader& shader,
	const glm::mat4& world_transform) const
{
	shader.uniform("obj_color", color);
	shader.uniform("model", world_transform);
	box_array.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

}
