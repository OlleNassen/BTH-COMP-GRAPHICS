#include "particle_emitter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

particle_emitter::particle_emitter(float x, float y, float z)
	: quad_vbo(target::ARRAY_BUFFER), instance_vbo(target::ARRAY_BUFFER), quad_texture(new texture("images/edvard.png"))
	, scene_node(x, y, z)
{
	for (int i = 0; i < MAX_NUM_PARTICLES; i++)
	{
		offsets[i].x = i;
		offsets[i].y = 0;
	}

	instance_vbo.data(sizeof(glm::vec2) * MAX_NUM_PARTICLES, (void*)offsets, GL_STATIC_DRAW);
	float size = 3.f;
	float vertices[] =
	{
		// positions //Texcoords     // colors
		0.f,  size, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, 0.f, 1.0f, 0.0,  0.0f, 1.0f, 0.0f,
		0.f, 0.f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		0.f,  size, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, size, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size,  0.f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	quad_array.bind();
	quad_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	int offset = 0;
	quad_array.attribute_pointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;
	quad_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;
	quad_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;
	
	offset = 0;
	instance_vbo.bind();
	quad_array.attribute_pointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(offset));
	quad_array.bind();
	quad_array.attribute_divisor(3, 1);
	
	/*
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_NUM_PARTICLES, (void*)offsets, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float size = 3.f;

	float vertices[] = {
		// positions //Texcoords     // colors
		0.f,  size, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, 0.f, 1.0f, 0.0,  0.0f, 1.0f, 0.0f,
		0.f, 0.f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		0.f,  size, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, size, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size,  0.f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int offset = 0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;


	// also set instance data
	offset = 0;
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(offset));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
	*/
}

particle_emitter::~particle_emitter()
{
}

void particle_emitter::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{

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
