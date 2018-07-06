#include "particle_emitter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#include <ctime>
#include <iostream>

particle_emitter::particle_emitter(float x, float y, float z)
	: quad_vbo(target::ARRAY_BUFFER), instance_vbo(target::ARRAY_BUFFER), quad_texture(new texture("images/edvard.png"))
	, scene_node(x, y, z)
{
	srand(time(NULL));
	for (int i = 0; i < MAX_NUM_PARTICLES; i++)
	{
		offsets[i].x = 0;
		offsets[i].y = 0;
		offsets[i].z = 0;
	}

	instance_vbo.data(sizeof(glm::vec3) * MAX_NUM_PARTICLES, (void*)offsets, GL_STATIC_DRAW);
	float size = 0.3f;
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
	int offset = 0;
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

void particle_emitter::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
	instance_vbo.data(sizeof(glm::vec3) * MAX_NUM_PARTICLES, (void*)offsets, GL_STATIC_DRAW);
	/*
	static float pos = 0;
	transform = glm::translate(transform, glm::vec3(0, pos, 0));
	pos += 0.0000005;
	*/
	for (int i = 0; i < MAX_NUM_PARTICLES; i++)
	{
		offsets[i] = glm::vec3(offsets[i].x, offsets[i].y + ((rand() % 100 + 1) / 10), offsets[i].z);
		if (offsets[i].y > 100)
		{
			offsets[i].y = 0;
		}
	}
}

void particle_emitter::render_current(const shader & shader, const glm::mat4 & world_transform) const
{
	shader.uniform("model", world_transform);
	quad_texture->uniform(shader, "diffuseMap", 0);
	quad_array.bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MAX_NUM_PARTICLES);
	glBindVertexArray(0);
}

void particle_emitter::set_texture(const std::string & path)
{
	delete quad_texture;
	quad_texture = new texture(path, wrap::REPEAT, filter::LINEAR, format::RGB);
}
