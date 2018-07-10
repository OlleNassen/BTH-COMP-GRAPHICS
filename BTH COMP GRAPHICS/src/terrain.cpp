#include "terrain.hpp"
#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

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

terrain::terrain(float x, float y, float z)
	: draw_count(0)
	, terrain_vbo(target::ARRAY_BUFFER), terrain_ebo(target::ELEMENT_ARRAY_BUFFER)
	, scene_node(x, y, z)
{
	terrain_texture = new texture("images/ground.png", wrap::REPEAT, filter::LINEAR, format::RGBA);

	int textureWidth, textureHeight, nrChannels;
	data = stbi_load("images/heightmap.jpg", &textureWidth, &textureHeight, &nrChannels, 1);

	std::vector<int> heights;

	if (data)
	{
		for (int i = 0; i < textureWidth * textureHeight; i++)
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

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	int heightIndex = 0;

	for (int x = 0; x < depth; x++)
	{
		for (int z = 0; z < width; z++)
		{
			vertices.push_back(Vertex(glm::vec3(x, heights[heightIndex] * 0.1f, z), glm::vec2(x, z)));
			heightIndex++;
		}
	}

	for (int i = 0; i < depth - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			int pos = j + (i * width);

			indices.push_back(pos);
			indices.push_back(pos + 1);
			indices.push_back(pos + width);

			indices.push_back(pos + 1);
			indices.push_back(pos + width);
			indices.push_back(pos + width + 1);

			draw_count += 6;

		}
	}

	terrain_array.bind();
	terrain_vbo.data(sizeof(float) * 5 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	terrain_ebo.data(sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	int offset = 0;
	terrain_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	offset += sizeof(glm::vec3);
	terrain_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
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
    return data[static_cast<int>(x) + static_cast<int>(z) * width] * 0.1f + y;
}

void terrain::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
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
