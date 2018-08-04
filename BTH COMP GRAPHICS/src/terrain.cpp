#include "terrain.hpp"
#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>

namespace scene
{

struct terrain_vertex
{
    glm::vec3 position;
    glm::vec2 texture;
};

terrain::terrain(float x, float y, float z)
	: node(x, y, z)
{
	terrain_texture = new texture("images/ground.png",
        wrap::REPEAT, filter::LINEAR, format::RGBA);

	auto nrChannels = 0;
	data = stbi_load("images/heightmap.jpg",
        &width, &height, &nrChannels, 1);

	std::vector<int> heights(width * height);
	if (data)
	{
		for (auto i = 0; i < width * height; i++)
		{
			heights[i] = static_cast<int>(data[i]);
		}
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}

	std::vector<terrain_vertex> vertices(width * height);
	auto index = 0;
	for (auto x = 0; x < height; ++x)
	{
		for (auto z = 0; z < width; ++z)
		{
			vertices[index] =
            { {x, heights[index] * 0.1f, z}, {x, z} };
			++index;
		}
	}

    std::vector<unsigned int> indices((width-1) * (height-1) * 6);
	for (auto i = 0; i < height - 1; ++i)
	{
		for (auto j = 0; j < width - 1; ++j)
		{
			auto pos = j + (i * width);

			indices[++draw_count] = pos;
			indices[++draw_count] = pos + 1;
			indices[++draw_count] = pos + width;

			indices[++draw_count] = pos + width + 1;
			indices[++draw_count] = pos + width;
			indices[++draw_count] = pos + 1;
		}
	}

	terrain_array.bind();
	terrain_vbo.data(sizeof(terrain_vertex) * vertices.size(),
        &vertices[0], GL_STATIC_DRAW);
	terrain_ebo.data(sizeof(unsigned int) * indices.size(),
        &indices[0], GL_STATIC_DRAW);

	terrain_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), nullptr);
	terrain_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), buffer_offset<glm::vec3>(3u));
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

void terrain::update_current(milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
    x = world_transform[3][0];
    y = world_transform[3][1];
    z = world_transform[3][2];
}

void terrain::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	terrain_texture->uniform(shader, "diffuse", 0);
	terrain_array.bind();
	glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
