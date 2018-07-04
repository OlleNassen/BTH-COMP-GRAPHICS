#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <stb_image.h>
#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"

class terrain : public scene_node
{
public:
	terrain();
	virtual ~terrain();
	void draw();

private:
	unsigned int draw_count;

	vertex_array terrain_array;
	buffer terrain_vbo;
	buffer terrain_ebo;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

#endif