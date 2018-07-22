#include "particle_emitter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#include <ctime>
#include <iostream>

particle_emitter::particle_emitter(float x, float y, float z)
	: scene_node(x, y, z)
	, quad_vbo(target::ARRAY_BUFFER), instance_vbo(target::ARRAY_BUFFER), quad_texture(new texture("images/edvard.png"))
{
	srand(time(NULL));

	for(auto& offset : offsets)
    {
        offset = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    for(auto& up : going_up)
    {
        up = true;
    }

	instance_vbo.data(sizeof(glm::vec3) * offsets.size(), offsets.data(), GL_STATIC_DRAW);
	auto size = 0.3f;
	float vertices[] =
	{
		// positions //Texcoords     // colors
		0.f,  size, 0.f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, 0.f, 0.f, 1.0f, 0.0,  0.0f, 1.0f, 0.0f,
		0.f, 0.f, 0.f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		0.f,  size, 0.f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, size, 0.f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size,  0.f, 0.f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	quad_array.bind();
	quad_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	auto offset = 0;
	quad_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;
	quad_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;
	quad_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;

	offset = 0;
	instance_vbo.bind();
	quad_array.attribute_pointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(offset));
	quad_array.attribute_divisor(3, 1);
}

particle_emitter::~particle_emitter()
{
}

void particle_emitter::update_current(const std::chrono::milliseconds delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
	instance_vbo.data(sizeof(glm::vec3) * offsets.size(), offsets.data(), GL_STATIC_DRAW);

	for (auto i = 0u; i < offsets.size(); i++)
	{
		float randomX = (rand() % 100 - 50);
		float randomY = (rand() % 100);
		float randomZ = (rand() % 100 - 50);

		if (offsets[i].y < 100 && going_up[i])
		{
			randomX /= 1000;
			randomY /= 100;
			randomZ /= 1000;

			offsets[i] = glm::vec3(offsets[i].x + randomX, offsets[i].y + randomY, offsets[i].z + randomZ);
		}
		else if (offsets[i].y < 200 && going_up[i])
		{
			randomX /= 100;
			randomY /= 100;
			randomZ /= 100;

			offsets[i] = glm::vec3(offsets[i].x + randomX, offsets[i].y + randomY, offsets[i].z + randomZ);
		}
		else if (going_up[i])
		{
			if (offsets[i].y > 400)
			{
				going_up[i] = false;
			}
			randomX /= 50;
			randomY /= 100;
			randomZ /= 50;

			offsets[i] = glm::vec3(offsets[i].x + randomX, offsets[i].y + randomY, offsets[i].z + randomZ);
		}
		else if (!going_up[i])
		{
			if (offsets[i].y < 0)
			{
				offsets[i] = glm::vec3(0, 0, 0);
				going_up[i] = true;
			}
			randomX /= 100;
			randomY /= 50;
			randomZ /= 100;

			offsets[i] = glm::vec3(offsets[i].x + randomX, offsets[i].y + -randomY, offsets[i].z + randomZ);
		}
	}
}

void particle_emitter::render_current(const shader & shader, const glm::mat4 & world_transform) const
{
	shader.uniform("model", world_transform);
	quad_texture->uniform(shader, "diffuseMap", 0);
	quad_array.bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, offsets.size());
	glBindVertexArray(0);
}

void particle_emitter::set_texture(const std::string & path)
{
	delete quad_texture;
	quad_texture = new texture(path, wrap::REPEAT, filter::LINEAR, format::RGB);
}
