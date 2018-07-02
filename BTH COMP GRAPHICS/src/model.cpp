#include "model.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

model::model(shader * shader, texture* texture)
{
	this->shader = shader;
	this->texture = texture;
}

model::~model()
{
}

void model::render(const glm::mat4& viewProjection)
{
	shader->use();

	shader->uniform("image", 0);
	texture->uniform(*shader,0);

	glm::mat4 model(1.f);

	model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));

	glm::mat4 mvp = viewProjection * model;

	shader->uniform("MVP", mvp);

	mesh.draw();
}
