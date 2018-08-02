#include "skeletal.hpp"

skeletal::skeletal()
{
	constexpr float vertices[] =
	{
		// positions     // uv
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.f, 0.0f, 1.0f
	};

	objects[0].bind();
	buffers[0].data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	objects[0].attribute_pointer(0, 3, GL_FLOAT,
		GL_FALSE, 5 * sizeof(float), nullptr);
	objects[0].attribute_pointer(1, 2, GL_FLOAT,
		GL_FALSE, 5 * sizeof(float), scene::buffer_offset<float>(3u));

	objects[1].bind();
	buffers[1].data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	objects[1].attribute_pointer(0, 3, GL_FLOAT,
		GL_FALSE, 5 * sizeof(float), nullptr);
	objects[1].attribute_pointer(1, 2, GL_FLOAT,
		GL_FALSE, 5 * sizeof(float), scene::buffer_offset<float>(3u));

	for (auto& transf : transforms)
	{
		transf = glm::mat4(1.f);
		transf[3][0] = -5.f;
		transf[3][2] = -20.f;
	}
	for (auto& rot : rotations)
	{
		rot = glm::quat( 1.f, 0.f, 0.f, 0.f );
	}
}

skeletal::~skeletal()
{
}

void skeletal::render_current(const shader& shader,
	const glm::mat4& world_transform)
{
	transforms[1] = transforms[1] * transforms[0];

	transforms[0][3][1] = glm::sin(1*glfwGetTime());
	rotations[0] = glm::rotate(rotations[0], 0.00001f, glm::vec3(1, 0, 0));

	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		glm::mat4 rot = glm::mat4_cast(rotations[i]);
		transforms[i] *= rot;

		shader.uniform("model", transforms[i]);
		//quad_texture->uniform(shader, "object_material.diffuse", 0);
		objects[i].bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}
