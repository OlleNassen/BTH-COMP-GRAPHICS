#ifndef MESH_FACTORY_HPP
#define MESH_FACTORY_HPP
#include <string>

class mesh;

class mesh_factory
{
public:
	mesh* load_mesh(const std::string & path);
private:


};

#endif