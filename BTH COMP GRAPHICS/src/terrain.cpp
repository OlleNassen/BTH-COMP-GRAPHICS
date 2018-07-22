#include "terrain.hpp"
#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using terrain_vertex = std::pair<glm::vec3, glm::vec2>; // pos, tex

terrain::terrain(float x, float y, float z)
	: scene_node(x, y, z)
    , draw_count(0)
	, terrain_vbo(target::ARRAY_BUFFER), terrain_ebo(target::ELEMENT_ARRAY_BUFFER)
{
	terrain_texture = new texture("images/ground.png", wrap::REPEAT, filter::LINEAR, format::RGBA);

	auto textureWidth = 0;
	auto textureHeight = 0;
	auto nrChannels = 0;
	data = stbi_load("images/heightmap.jpg", &textureWidth, &textureHeight, &nrChannels, 1);

	std::vector<int> heights;
	heights.reserve(textureWidth * textureHeight);

	if (data)
	{
		for (auto i = 0; i < textureWidth * textureHeight; i++)
		{
			heights.push_back(static_cast<int>(data[i]));
		}
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}

	width = textureWidth;
	depth = textureHeight;

	std::vector<terrain_vertex> vertices;
	std::vector<unsigned int> indices;

	auto heightIndex = 0;

	for (auto x = 0; x < depth; x++)
	{
		for (auto z = 0; z < width; z++)
		{
			vertices.push_back({ glm::vec3(x, heights[heightIndex] * 0.1f, z), glm::vec2(x, z) });
			heightIndex++;
		}
	}

	for (auto i = 0; i < depth - 1; i++)
	{
		for (auto j = 0; j < width - 1; j++)
		{
			auto pos = j + (i * width);

			indices.push_back(pos);
			indices.push_back(pos + 1);
			indices.push_back(pos + width);

			indices.push_back(pos + width + 1);
			indices.push_back(pos + width);
			indices.push_back(pos + 1);

			draw_count += 6;
		}
	}

	terrain_array.bind();
	terrain_vbo.data(sizeof(float) * 5 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	terrain_ebo.data(sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	int offset = 0;
	terrain_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), BUFFER_OFFSET(offset));
	offset += sizeof(glm::vec3);
	terrain_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), BUFFER_OFFSET(offset));
	offset += sizeof(glm::vec2);
	x = 0;
	y = 0;
	z = 0;
}


terrain::~terrain()
{
	stbi_image_free(data);
}

float terrain::calculate_camera_y(float x, float z) const
{
    int x_index = x - this->x - this->x * 0.25f;
    int z_index = z - this->z + this->z * 0.25f;

    return data[x_index + z_index * width] * 0.1f + y;
}

void terrain::update_current(const std::chrono::milliseconds delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
    x = world_transform[3][0];
    y = world_transform[3][1];
    z = world_transform[3][2];
}

void terrain::render_current(const shader & shader, const glm::mat4 & world_transform) const
{
	shader.uniform("model", world_transform);
	terrain_texture->uniform(shader, "diffuse", 0);
	terrain_array.bind();
	glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
