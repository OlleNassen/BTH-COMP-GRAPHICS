#pragma once
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"

class model
{
private:
	shader* shader;
	texture* texture;
	mesh mesh;

	glm::mat4* model;

public:
	model(shader* shader, texture* texture);
	~model();

	void render(const glm::mat4& viewProjection);
};