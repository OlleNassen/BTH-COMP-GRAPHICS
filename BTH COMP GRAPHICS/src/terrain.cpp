#include "terrain.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>

namespace scene
{

struct terrain_vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

terrain::terrain(float x, float y, float z)
	: node(x, y, z)
	, terrain_texture{"images/ground.png",
        wrap::REPEAT, filter::LINEAR, format::RGBA}
{
	auto* begin =
        stbi_load("images/heightmap.jpg", &width, &height, &channels, 1);
    auto* end = begin + width * height;
    heights.resize(end - begin);
    std::copy(begin, end, heights.begin());
    stbi_image_free(begin);

	std::vector<terrain_vertex> vertices(heights.size());
	for (auto i = 0; i < vertices.size(); ++i)
	{
        glm::vec2 v{i % width, i / height};
        vertices[i].position = {v.x, heights[i] * 0.1f, v.y};
        vertices[i].texture = {v.x / width, v.y / height};
	}

	for (auto i = 0; i < width * height; i += 3)
	{
        auto u = vertices[i].position - vertices[i+1].position;
        auto v = vertices[i].position - vertices[i+2].position;
        auto normal = glm::cross(u, v);

        vertices[i].normal = normal;
        vertices[i+1].normal = normal;
        vertices[i+2].normal = normal;
	}

    std::vector<unsigned int> indices((width-1) * (height-1) * 6);
	for (auto i = 0; i < (width-1) * (height-1); ++i)
	{
        indices[++draw_count] = i;
        indices[++draw_count] = i + 1;
        indices[++draw_count] = i + width;

        indices[++draw_count] = i + width + 1;
        indices[++draw_count] = i + width;
        indices[++draw_count] = i + 1;
	}

	terrain_array.bind();

	terrain_vbo.data(sizeof(terrain_vertex) * vertices.size(),
        &vertices[0], GL_STATIC_DRAW);
	terrain_ebo.data(sizeof(unsigned int) * indices.size(),
        &indices[0], GL_STATIC_DRAW);

	terrain_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), nullptr);

    terrain_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), buffer_offset<glm::vec3>(1));

	terrain_array.attribute_pointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), buffer_offset<glm::vec3>(2));
}

float terrain::calculate_camera_y(float x, float z) const
{
    int x_index = x - this->x - this->x * 0.25f;
    int z_index = z - this->z + this->z * 0.25f;
    return heights[x_index + z_index * width] * 0.1f + y;
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
	terrain_texture.uniform(shader, "diffuse", 0);
	terrain_array.bind();
	glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
