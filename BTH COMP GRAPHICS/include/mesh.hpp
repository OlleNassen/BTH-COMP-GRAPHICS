#ifndef mesh_HPP
#define mesh_HPP
#include <gl\glew.h>
#include "texture.hpp"
#include <stb_image.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texture;

	Vertex(const glm::vec3& position, const glm::vec2& texture)
	{
		this->position = position;
		this->texture = texture;
	}
};

enum class mesh_type
{
	quad, box, terrain, custom
};

class mesh
{
public:
	mesh(const mesh_type& type);
	~mesh();

	void draw();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int id;
	unsigned int draw_count = 0;
	mesh_type type;

	void load_quad();
	void load_box();
	void load_terrain();
	void load_custom();
};

#endif