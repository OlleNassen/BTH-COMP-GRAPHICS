#ifndef TEMP_BOX_HPP
#define TEMP_BOX_HPP
#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class temp_box : public node
{
public:
	temp_box(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual ~temp_box();

private:
	vertex_array box_array;
	buffer box_vbo;
	glm::vec3 color;

	virtual void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
