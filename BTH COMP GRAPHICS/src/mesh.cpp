#include "mesh.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

mesh::mesh(const mesh_type& type)
{
	switch (type)
	{
	case mesh_type::QUAD:
		this->type = type;
		load_quad();
		break;
	case mesh_type::BOX:
		this->type = type;
		load_box();

		break;
	case mesh_type::TERRAIN:
		this->type = type;
		load_terrain();
		break;

	case mesh_type::SKYBOX:
		this->type = type;
		load_skybox();
		break;

	default:

		break;

	}
}

mesh::~mesh()
{
}

void mesh::draw(const glm::mat4& mvp, const shader& shader_ptr)
{
	switch (type)
	{
	case mesh_type::QUAD:
		shader_ptr.use();
		shader_ptr.uniform("mvp", mvp);
		if (texture_ptrs.size() > 0)
			use_textures(shader_ptr);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		break;
	case mesh_type::BOX:
		shader_ptr.use();
		shader_ptr.uniform("model_view_projection", mvp);
		if(texture_ptrs.size() > 0)
			use_textures(shader_ptr);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		break;
	case mesh_type::TERRAIN:
		shader_ptr.use();
		shader_ptr.uniform("mvp", mvp);
		if (texture_ptrs.size() > 0)
			use_textures(shader_ptr);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		break;
	case mesh_type::SKYBOX:

		break;
	default:
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		break;
	}

}

void mesh::use_textures(const shader& shader_ptr)
{
	//Expand this to use more textures
	if (texture_ptrs.size() > 0)
	{
		switch (type_texture)
		{
		case texture_type::EDVARD_QUAD:
			texture_ptrs[0]->uniform(shader_ptr, "image", 0);
			break;
		case texture_type::CONTAINER_BOX:
			texture_ptrs[0]->uniform(shader_ptr, "object_material.diffuse", 0);
			texture_ptrs[1]->uniform(shader_ptr, "object_material.specular", 1);
			shader_ptr.uniform("object_material.shininess", 32.0f);
			break;
		}

	}
}

void mesh::set_texture(const texture_type& tex)
{
	switch (tex)
	{
	case texture_type::EDVARD_QUAD:
		type_texture = texture_type::EDVARD_QUAD;
		texture_ptrs.push_back(new texture("images/edvard.png"));
		break;

	case texture_type::CONTAINER_BOX:
		type_texture = texture_type::CONTAINER_BOX;
		texture_ptrs.push_back(new texture("images/container2.png"));
		texture_ptrs.push_back(new texture("images/container2_specular.png"));
		break;
	case texture_type::JUNGLE_SKYBOX:
		type_texture = texture_type::JUNGLE_SKYBOX;
		std::vector<std::string> faces
		{
			"images/skybox/highly-overrated_bk.tga",
			"images/skybox/highly-overrated_bk.tga",
			"images/skybox/highly-overrated_bk.tga",
			"images/skybox/highly-overrated_bk.tga",
			"images/skybox/highly-overrated_bk.tga",
			"images/skybox/highly-overrated_bk.tga"
		};
		texture_ptrs.push_back(new texture(faces, wrap::CLAMP_TO_EDGE, filter::LINEAR, format::RGB));
		break;
	}
}

void mesh::load_quad()
{
	float quadVertices[] =
	{
		// positions     // colors
		0.0f, 256.f, 0.f, 0.0f, 1.0f,  1.f, 1.f, 1.f,
		256.f, 0.0f, 0.f, 1.0f, 0.0f,    1.f, 1.f, 1.f,
		0.0f, 0.0f, 0.f, 0.0f, 0.0f,   1.f, 1.f, 1.f,

		0.0f, 256.f, 0.f, 0.0f, 1.0f,  1.f, 1.f, 1.f,
		256.f, 256.f, 0.f, 1.0f, 1.0f,   1.f, 1.f, 1.f,
		256.f, 0.0f, 0.f, 1.0f, 0.0f,  1.f, 1.f, 1.f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	int offset = 0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 2;

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	offset += sizeof(float) * 3;
}

void mesh::load_box()
{
	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW); // 5 is number of elements in vertex

	GLuint offset = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(0);
	offset += sizeof(float) * 3;

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(1);
	offset += sizeof(float) * 3;

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(2);
	offset += sizeof(float) * 2;
}

void mesh::load_terrain()
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
		std::cout << "Failed to load texture " << data << 'n';
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * vertices.size(), &vertices[0], GL_STATIC_DRAW); // 5 is number of elements in vertex

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	GLuint offset = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(0);
	offset += sizeof(glm::vec3);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(1);
	offset += sizeof(glm::vec2);
}

void mesh::load_skybox()
{
	std::vector<GLfloat> skybox_vertices =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices[0], GL_STATIC_DRAW);

	GLint offset = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	offset += sizeof(glm::vec3);
}
