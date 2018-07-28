#include "buffer.hpp"

buffer::buffer()
{
    glGenBuffers(1, &id);
}

buffer::buffer(target buffer_target)
    : buffer()
{
    this->buffer_target = buffer_target;
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id);
}

void buffer::bind() const
{
    glBindBuffer(static_cast<unsigned int>(buffer_target), id);
}

void buffer::data(int size, const void* data, unsigned int usage) const
{
    bind();
    glBufferData(static_cast<unsigned int>(buffer_target), size, data, usage);
}
