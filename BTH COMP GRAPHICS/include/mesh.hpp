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

enum class box_texture_type
{
	CONTAINER
};

class mesh
{
public:
	mesh(const mesh_type& type);
	~mesh();

	void draw(const glm::mat4& mvp, const shader& shader_ptr);
	void use_textures(const shader& shader_ptr);

	void set_box_texture(const box_texture_type& box_texture);

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int id;
	unsigned int draw_count = 0;
	mesh_type type;
	box_texture_type box_type; // Inheritance doesn't work with enums.. find another fix?
	std::vector<texture*> texture_ptrs;

	void load_quad();
	void load_box();
	void load_terrain();
	void load_custom();
};

#endif