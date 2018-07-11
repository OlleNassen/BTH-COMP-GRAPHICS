#include "mesh_factory.hpp"
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <vector>

mesh* mesh_factory::load_mesh(const std::string & path)
{
	std::vector <glm::vec3> out_vertices;
	std::vector <glm::vec2> out_uvs;
	std::vector <glm::vec3> out_normals;

	std::ifstream in(path);

	if (!in)
	{
		std::cout << "SHIT AIN'T WORKING";
	}


	while (!in.eof())
	{
		std::string temp;

		in >> temp;

		if (temp[0] == '\n')
		{
			static int index = 0;
			std::cout << index++ << '\n';
		}

		else if (temp[0] == '#')
		{

		}

		else if (temp[0] == 'v')
		{
			glm::vec3 vertex;
			in >> vertex.x;
			in >> vertex.y;
			in >> vertex.z;
			out_vertices.push_back(vertex);
			//std::cout << vertex.x << " " << vertex.y << " " << vertex.z << '\n';
		}

		else if (temp[0] == 'vt')
		{
			glm::vec2 uv;
			in >> uv.x;
			in >> uv.y;
			out_uvs.push_back(uv);
		}

		else if (temp[0] == 'vn')
		{
			glm::vec3 normal;
			in >> normal.x;
			in >> normal.y;
			in >> normal.z;
			out_normals.push_back(normal);
		}

		else if (temp[0] == 'f')
		{
			unsigned int index = 1;

			glm::ivec3 vertex_index;
			glm::ivec3 uv_index;
			glm::ivec3 normal_index;

			/*
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			*/

			in >> vertex_index.x;
			in >> vertex_index.y;
			in >> vertex_index.z;

			std::cout << vertex_index.x << " " << vertex_index.y << " " << vertex_index.z << '\n';

		}


	}

	return nullptr;
}
