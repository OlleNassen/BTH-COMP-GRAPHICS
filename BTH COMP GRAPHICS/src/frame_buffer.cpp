#include "frame_buffer.hpp"
#include <GL/glew.h>

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
