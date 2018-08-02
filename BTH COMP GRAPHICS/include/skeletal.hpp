#ifndef SKELETAL_HPP
#define SKELETAL_HPP
#include <array>
#include "vertex_array.hpp"
#include "buffer.hpp"
#include "node.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class skeletal
{
public:
	skeletal();
	~skeletal();
	void render_current(const shader& shader,
		const glm::mat4& world_transform);

private:
	static constexpr int NUM_OBJECTS = 2;
	std::array<vertex_array, NUM_OBJECTS> objects;
	std::array<buffer, NUM_OBJECTS> buffers;
	std::array<glm::mat4, NUM_OBJECTS>transforms;
	std::array<glm::quat, NUM_OBJECTS>rotations;

};

#endif
