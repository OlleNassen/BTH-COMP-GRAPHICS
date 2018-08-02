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

	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		objects[i].bind();
		buffers[i].data(sizeof(vertices), &vertices, GL_STATIC_DRAW);
		objects[i].attribute_pointer(0, 3, GL_FLOAT,
			GL_FALSE, 5 * sizeof(float), nullptr);
		objects[i].attribute_pointer(1, 2, GL_FLOAT,
			GL_FALSE, 5 * sizeof(float), scene::buffer_offset<float>(3u));
	}

	transforms[0][3][0] = -5.f;
	transforms[0][3][2] = -20.f;

	for (auto& rot : rotations)
	{
		rot = glm::quat( 1.f, 0.f, 0.f, 0.f );
	}
}

skeletal::~skeletal()
{
}
#include <iostream>
void skeletal::render_current(const shader& shader,
	const glm::mat4& world_transform)
{
	//transforms[1] *= glm::mat4_cast(rotations[0]);

	//std::cout << transforms[1][3][0] << " " << transforms[1][3][1] << " " << transforms[1][3][2] << '\n';

	transforms[0][3][1] = glm::sin(1*glfwGetTime());
	rotations[0] = glm::rotate(rotations[0], 0.00001f, glm::vec3(1, 0, 0));

	for (int i = 0; i < NUM_OBJECTS; i++)
	{

		glm::mat4 rot = glm::mat4_cast(rotations[i]);
		transforms[i] =  transforms[i] * rot;

		if (i != 0)
		{
			//transforms[i] = transforms[0] * transforms[i];
		}

		shader.uniform("model", transforms[i]);
		//quad_texture->uniform(shader, "object_material.diffuse", 0);
		objects[i].bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}
