#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "scene_node.hpp"

class point_light: public scene_node
{
public:
	point_light();
	virtual ~point_light();

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;
};

#endif
