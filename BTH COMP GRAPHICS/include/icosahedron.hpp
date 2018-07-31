#ifndef ICOSAHEDRON_HPP
#define ICOSAHEDRON_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class icosahedron : public node
{
public:
	icosahedron(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	void set_color(const glm::vec3& color);

private:
	vertex_array v_array;
	buffer vbo;
	buffer ebo{target::ELEMENT_ARRAY_BUFFER};

	glm::vec3 temp_color;

	unsigned int index_count;

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
