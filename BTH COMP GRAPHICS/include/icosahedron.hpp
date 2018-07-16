#ifndef ICOSAHEDRON_HPP
#define ICOSAHEDRON_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"


class icosahedron :public scene_node
{
public:
	icosahedron(float x = 0.f, float y = 0.f, float z = 0.f);
	virtual ~icosahedron();

private:
	GLuint vao;
	vertex_array v_array;
	buffer vbo;
	buffer ebo;
	//texture displacement_map;
	//texture tess_texture;

	unsigned int index_count;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

#endif