#ifndef QUAD_TESS_HPP
#define QUAD_TESS_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class quad_tess : public scene_node
{
public:
	quad_tess(float x = 0.f, float y = 0.f, float z = 0.f);
	virtual ~quad_tess();

private:
	vertex_array quad_array;
	buffer quad_vbo;
	buffer quad_ebo;
	texture displacement_map;
	texture tess_texture;

	virtual void update_current(const std::chrono::milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

#endif
