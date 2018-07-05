#include "point_light.hpp"

point_light::point_light()
{
	ambient = glm::vec3(0.4, 0.4, 0.4);
	diffuse = glm::vec3(1, 1, 0.1);
	specular = glm::vec3(0.1, 1, 0.1);

	this->attach_child(&da_box);
}

point_light::~point_light()
{
}

void point_light::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
}

void point_light::render_current(const shader & shader, const glm::mat4 & world_transform) const
{

}
