#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <string>
#include "tinyxml2.h"
#include "model.hpp"

class model_loader
{
public:
    model_loader(const std::string& path);


private:
    std::vector<vertex> vertices;
};

#endif // MODEL_LOADER_HPP
