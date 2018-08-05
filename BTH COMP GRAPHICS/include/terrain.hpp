#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class terrain : public node
{
public:
	terrain(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	float calculate_camera_y(const float x, const float z) const;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	int width = 0;

private:
	std::vector<int> heights;
	int draw_count{-1};

	vertex_array terrain_array;
	buffer terrain_vbo;
	buffer terrain_ebo{target::ELEMENT_ARRAY_BUFFER};
	texture* terrain_texture;

	int height = 0;

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

}

#endif
