#include "quad_tess.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

quad_tess::quad_tess(float x, float y, float z)
	:quad_vbo(target::ARRAY_BUFFER)
	, displacement_map("images/displacement_map.png")
{
	float vertices[] =
	{
		//pos, uv, normal
		-0.5f, -0.5f, 0.f, 0.0f, 0.0f, 0.f, 0.0f, -1.f,
		-0.5f, 0.5f, 0.f, 0.0f, 1.0f, 0.f, 0.0f, -1.f,
		0.5f, -0.5f, 0.f, 1.0f, 0.0f, 0.f, 0.0f, -1.f,

		-0.5f, 0.5f, 0.f, 0.0f, 1.0f, 0.f, 0.0f, -1.f,
		0.5f, 0.5f, 0.f, 1.0f, 1.0f, 0.f, 0.0f, -1.f,
		0.5f, -0.5f, 0.f, 1.0f, 0.0f, 0.f, 0.0f, -1.f
	};

	quad_array.bind();
	quad_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	int offset = 0;
	quad_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;
	quad_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;
	quad_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;

}

quad_tess::~quad_tess()
{

}

void quad_tess::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
}

void quad_tess::render_current(const shader & shader, const glm::mat4 & world_transform) const
{
	shader.uniform("model", world_transform);
	shader.uniform("disp_factor", 2.f);
	displacement_map.uniform(shader, "displacement_map", 0);
	quad_array.bind();
	glDrawArrays(GL_PATCHES, 0, 6);
	glBindVertexArray(0);
}
