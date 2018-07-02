#include "mesh.hpp"
#include <glm/glm.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

mesh::mesh()
{
	loadVertices(nullptr, nullptr, 6);
}

mesh::~mesh()
{
}

void mesh::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void mesh::loadColoredVertices(ColoredVertex * vertices, GLuint * indices, GLuint numIndices)
{
}

void mesh::loadVertices(Vertex * vertices, GLuint * indices, GLuint numIndices)
{
	//temp
	Vertex verts[] = // Pos, normal, uv
	{
		Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0, 1.0, 0.0), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(1.0, 0.3, 0), glm::vec2(0.0f, 0.0f))
	};

	GLuint indi[] =
	{
		0, 1, 2, 3, 2, 1
	};

	GLint numVertices = 4;
	numIndices = 6;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &bufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indi), indi, GL_STATIC_DRAW);

	GLint offset = 0;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(0);
	offset += sizeof(glm::vec3);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(1);
	offset += sizeof(glm::vec3);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offset));
	glEnableVertexAttribArray(2);
	offset += sizeof(glm::vec2);
}