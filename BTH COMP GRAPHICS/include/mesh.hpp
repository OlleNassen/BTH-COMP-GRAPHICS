#ifndef mesh_HPP
#define mesh_HPP
#include <gl\glew.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv)
	{
		this->position = position;
		this->normal = normal;
		this->uv = uv;
	}
	~Vertex()
	{

	}
};

struct ColoredVertex :public Vertex
{
	glm::vec3 color;
};

class mesh
{
private:
	GLuint vao;
	GLuint ebo;
	GLuint bufferObjectID;
public:
	mesh();
	~mesh();

	void draw();
	void loadColoredVertices(ColoredVertex* vertices, GLuint* indices = nullptr, GLuint numIndices = 0);
	void loadVertices(Vertex* vertices, GLuint* indices = nullptr, GLuint numIndices = 0);
};

#endif