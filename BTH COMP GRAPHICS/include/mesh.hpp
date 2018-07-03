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
	QUAD, BOX, TERRAIN, SKYBOX, CUSTOM
};

enum class texture_type
{
	CONTAINER_BOX, EDVARD_QUAD, JUNGLE_SKYBOX
};

class mesh
{
public:
	mesh(const mesh_type& type);
	~mesh();

	void draw(const glm::mat4& mvp, const shader& shader_ptr);
	void use_textures(const shader& shader_ptr);

	void set_texture(const texture_type& tex);

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int id;
	unsigned int draw_count = 0;
	mesh_type type;
	texture_type type_texture;

	std::vector<texture*> texture_ptrs;

	void load_quad();
	void load_box();
	void load_terrain();
	void load_skybox();
	void load_custom();
};

#endif