#ifndef mesh_HPP
#define mesh_HPP
#include <gl\glew.h>
#include "texture.hpp"
#include <stb_image.h>
#include <vector>

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
	QUAD, BOX, TERRAIN, CUSTOM
};

class mesh
{
public:
	mesh(const mesh_type& type);
	~mesh();

	void draw();

	bool has_textures();
	void use_textures(shader* shader_ptr);

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int id;
	unsigned int draw_count = 0;
	mesh_type type;
	std::vector<texture*> texture_ptrs;

	void load_quad();
	void load_box();
	void load_terrain();
	void load_custom();
};

#endif