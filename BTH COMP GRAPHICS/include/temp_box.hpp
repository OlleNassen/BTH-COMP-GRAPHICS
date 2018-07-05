#ifndef TEMP_BOX_HPP
#define TEMP_BOX_HPP
#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class temp_box : public scene_node
{
public:
	temp_box(float x = 0.f, float y = 0.f, float z = 0.f);
	virtual ~temp_box();

private:
	vertex_array box_array;
	buffer box_vbo;
	glm::vec3 color;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

#endif