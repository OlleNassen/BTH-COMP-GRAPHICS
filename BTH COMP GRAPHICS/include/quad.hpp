#ifndef QUAD_HPP
#define QUAD_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class quad : public scene_node
{
public:
	quad(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual ~quad();

	void set_texture(const std::string& path);
private:
	vertex_array quad_array;
	buffer quad_vbo;
	texture* quad_texture;

	virtual void update_current(const std::chrono::milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

#endif
