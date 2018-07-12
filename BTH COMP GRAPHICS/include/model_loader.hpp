#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP


#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <iterator>
#include <string>
#include "GL/glew.h"
#include "tinyxml2.h"
#include "model.hpp"


struct SourceData
{
    GLenum type;
    unsigned int size;
    unsigned int stride;
    void* data;
};

typedef std::map<std::string, SourceData> SourceMap;

struct ColGeom
{
    std::string name;
    SourceMap map;
    GLenum primitive;
    int index_count;
    unsigned short* indices;
};

SourceData readSource(tinyxml2::XMLElement *source);

class model_loader
{
public:
    model_loader(const std::string& path);

    void readGeometries(std::vector<ColGeom> *v, const char *filename);
    void freeGeometries(std::vector<ColGeom> *v);


private:
    std::vector<ColGeom> v;
};

#endif // MODEL_LOADER_HPP
