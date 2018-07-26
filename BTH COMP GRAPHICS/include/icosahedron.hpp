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
	icosahedron(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f);
	virtual ~icosahedron();

	void set_color(const glm::vec3& color);

private:
	GLuint vao;
	vertex_array v_array;
	buffer vbo;
	buffer ebo;

	glm::vec3 temp_color;

	unsigned int index_count;

	virtual void update_current(const milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
