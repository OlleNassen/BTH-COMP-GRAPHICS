#include "scene_loader.hpp"

std::vector<vertex2> load_vertices(tinyxml2::XMLNode* node)
{
    std::vector<std::string> names;
    std::vector<glm::vec4> weights;

    std::cout << "funkar!!!" << std::endl;
    std::vector<vertex2> v;
    return v;
}

std::vector<std::string> load_names(tinyxml2::XMLNode* node)
{
	return std::vector<std::string>();
}

scene_loader::scene_loader(const std::string& filename)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    vertices = load_vertices(doc.FirstChildElement("library_controllers"));
}

scene_loader::~scene_loader()
{
    //dtor
}
