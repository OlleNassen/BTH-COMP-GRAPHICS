#include "buffer.hpp"

buffer::buffer(const target buffer_target)
    : buffer_target(
        static_cast<unsigned int>(buffer_target))
{
    glGenBuffers(1, &id);
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id);
}

void buffer::bind() const
{
    glBindBuffer(buffer_target, id);
}

void buffer::data(const int size, const void* data, const unsigned int usage) const
{
    bind();
    glBufferData(buffer_target, size, data, usage);
}
