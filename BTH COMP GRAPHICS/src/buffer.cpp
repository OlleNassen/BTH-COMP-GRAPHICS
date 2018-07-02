#include "buffer.hpp"

buffer::buffer(target buffer_target)
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

void buffer::data(int size, const void* data, unsigned int usage) const
{
    bind();
    glBufferData(buffer_target, size, data, usage);
}
