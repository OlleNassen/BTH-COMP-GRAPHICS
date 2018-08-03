#include "skeletal.hpp"

skeletal::skeletal()
{
	robot = new cube_robot;
}

skeletal::~skeletal()
{
}

void skeletal::render_current(const shader& shader,
	const glm::mat4& world_transform)
{
	robot->update(0.000016f);
	robot->draw(shader);

	/*
	shader.uniform("model", transforms[i]);
	//quad_texture->uniform(shader, "object_material.diffuse", 0);
	objects[i].bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	*/
}