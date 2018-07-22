#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class terrain : public scene_node
{
public:
	terrain(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	virtual ~terrain();

	float calculate_camera_y(float x, float z) const;

	float x;
	float y;
	float z;
	int width;

private:
	unsigned int draw_count;
	unsigned char* data;

	vertex_array terrain_array;
	buffer terrain_vbo;
	buffer terrain_ebo;
	texture* terrain_texture;
	int depth;
	virtual void update_current(const std::chrono::milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

#endif
