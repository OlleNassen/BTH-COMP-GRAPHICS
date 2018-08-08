#ifndef SCENE_LOADER_HPP
#define SCENE_LOADER_HPP

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "tinyxml2.h"

struct vertex2
{

};

class scene_loader
{
public:
    scene_loader(const std::string& filename);
    ~scene_loader();

    std::vector<vertex2> vertices;

private:
};

#endif // SCENE_LOADER_HPP
