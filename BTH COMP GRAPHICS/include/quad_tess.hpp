#ifndef QUAD_TESS_HPP
#define QUAD_TESS_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class quad_tess : public node
{
public:
	quad_tess(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f);
	virtual ~quad_tess();

private:
	vertex_array quad_array;
	buffer quad_vbo;
	buffer quad_ebo;
	texture tess_texture;
	texture displacement_map;

	virtual void update_current(const milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
