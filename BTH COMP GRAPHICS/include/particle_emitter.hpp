#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP
#include <array>
#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class particle_emitter : public node
{
public:
	particle_emitter(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	virtual ~particle_emitter();

	void set_texture(const std::string& path);
private:
	vertex_array quad_array;
	buffer quad_vbo;
	buffer instance_vbo;
	texture* quad_texture;

	static constexpr int MAX_NUM_PARTICLES = 10000;
    std::array<glm::vec3, MAX_NUM_PARTICLES> offsets;
    std::array<bool, MAX_NUM_PARTICLES> going_up;

	virtual void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	virtual void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
