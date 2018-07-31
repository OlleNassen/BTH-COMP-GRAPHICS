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




frame_buffer::frame_buffer()
{
    glGenFramebuffers(1, &id);
}

frame_buffer::~frame_buffer()
{
    glDeleteFramebuffers(1, &id);
}

void frame_buffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void frame_buffer::bind_texture(const texture& texture) const
{
    bind();
    texture.bind_to_buffer();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
