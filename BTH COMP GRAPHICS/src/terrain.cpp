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
	:draw_count(0), terrain_vbo(target::ARRAY_BUFFER), terrain_ebo(target::ELEMENT_ARRAY_BUFFER),
	scene_node(x, y, z)
{
	int textureWidth, textureHeight, nrChannels;
	unsigned char* data = stbi_load("images/heightmap.jpg", &textureWidth, &textureHeight, &nrChannels, 1);

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

	stbi_image_free(data);

	int width = textureWidth;
	int depth = textureHeight;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	int heightIndex = 0;

	for (int x = 0; x < depth; x++)
	{
		for (int z = 0; z < width; z++)
		{
			vertices.push_back(Vertex(glm::vec3(x, heights[heightIndex] * 0.1f, z), glm::vec2(0, 0)));
			heightIndex++;
		}
		//heightIndex++;
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

	/*
	box_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	box_array.bind();
	box_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	box_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	box_array.attribute_pointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	*/
	terrain_vbo.data(sizeof(float) * 5 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	terrain_ebo.data(sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	terrain_array.bind();
	int offset = 0;
	terrain_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	offset += sizeof(glm::vec3);
	terrain_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	offset += sizeof(glm::vec2);
	/*
	glGenVertexArrays(1, &terrain_array);
	glBindVertexArray(terrain_array);

	glGenBuffers(1, &terrain_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * vertices.size(), &vertices[0], GL_STATIC_DRAW); // 5 is number of elements in vertex

	glGenBuffers(1, &terrain_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	GLuint offset = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(0);
	offset += sizeof(glm::vec3);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(1);
	offset += sizeof(glm::vec2);
	*/
}


terrain::~terrain()
{
}

void terrain::update_current(float delta_time, const glm::mat4 & world_transform, glm::mat4 & transform)
{
}

void terrain::render_current(const shader & shader, const glm::mat4 & world_transform) const
{
	shader.uniform("model", world_transform);
	terrain_array.bind();
	glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void terrain::draw()
{
	terrain_array.bind();
	glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
