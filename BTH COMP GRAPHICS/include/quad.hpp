#ifndef QUAD_HPP
#define QUAD_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"

class quad : public scene_node
{
public:
	quad();
	virtual ~quad();

private:
	vertex_array quad_array;
	buffer quad_vbo;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

#endif