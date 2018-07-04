#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class terrain : public scene_node
{
public:
	terrain(float x = 0.f, float y = 0.f, float z = 0.f);

	virtual ~terrain();

private:
	unsigned int draw_count;

	vertex_array terrain_array;
	buffer terrain_vbo;
	buffer terrain_ebo;
	texture terrain_texture;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

#endif
