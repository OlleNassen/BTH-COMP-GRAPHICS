#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP
#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"
class particle_emitter:public scene_node
{
public:
	particle_emitter(float x = 0.f, float y = 0.f, float z = 0.f);
	virtual ~particle_emitter();

	void set_texture(const std::string& path);
private:
	vertex_array quad_array;
	buffer quad_vbo;
	buffer instance_vbo;
	texture* quad_texture;

	static const unsigned int MAX_NUM_PARTICLES = 10000;
	glm::vec3 offsets[MAX_NUM_PARTICLES];
	bool going_up[MAX_NUM_PARTICLES];

	virtual void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

#endif