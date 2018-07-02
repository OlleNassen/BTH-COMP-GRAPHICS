#include "vertex_array.hpp"
#include <GL/glew.h>

vertex_array::vertex_array()
{
    glGenVertexArrays(1, &id);
}

vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &id);
}

void vertex_array::bind() const
{
    glBindVertexArray(id);
}

void vertex_array::attribute_pointer(
    unsigned int index,
    int size,
    unsigned int type,
    unsigned char normalized,
    int stride,
    const void* pointer) const
{
    bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}


void vertex_array::attribute_pointer(
    unsigned int index,
    int size,
    unsigned int type,
    int stride,
    const void* pointer) const
{
    bind();
    glEnableVertexAttribArray(index);
    glVertexAttribIPointer(index, size, type, stride, pointer);
}
