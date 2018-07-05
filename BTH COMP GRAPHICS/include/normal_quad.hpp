#ifndef NORMAL_QUAD_HPP
#define NORMAL_QUAD_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class normal_quad : public scene_node
{
public:
	normal_quad(float x = 0.f, float y = 0.f, float z = 0.f);
	virtual ~normal_quad();

	void set_texture(const std::string& path);
private:
	vertex_array quad_array;
	buffer quad_vbo;
	texture* quad_texture;
	texture* quad_normal;

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

#endif
