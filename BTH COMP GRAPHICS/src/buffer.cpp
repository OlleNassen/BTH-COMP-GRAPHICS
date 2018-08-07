#include "buffer.hpp"
#include <GL/glew.h>

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

void vertex_array::attribute_divisor(
	unsigned int index,
	unsigned int divisor) const
{
	bind();
	glVertexAttribDivisor(index, divisor);
}