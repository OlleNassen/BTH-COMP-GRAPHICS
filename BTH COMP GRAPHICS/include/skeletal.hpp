#ifndef SKELETAL_HPP
#define SKELETAL_HPP
#include <array>
#include "vertex_array.hpp"
#include "buffer.hpp"
#include "node.hpp"
#include <glm/glm.hpp>

class skeletal
{
static const unsigned int NUM_OBJECTS = 2;
private:
	std::array<vertex_array, NUM_OBJECTS> objects;
	std::array<buffer, NUM_OBJECTS> buffers;
	std::array<glm::mat4, NUM_OBJECTS>transforms;
public:
	skeletal();
	~skeletal();
	void render_current(const shader& shader,
		const glm::mat4& world_transform)const;
};

#endif