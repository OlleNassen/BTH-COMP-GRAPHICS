#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <GL/glew.h>
#include "texture.hpp"

enum class target
{
    ARRAY_BUFFER = GL_ARRAY_BUFFER,
    ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
    COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
    COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
    DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
    DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
    ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
    PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
    PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
    QUERY_BUFFER = GL_QUERY_BUFFER,
    SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
    TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
    TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
    UNIFORM_BUFFER = GL_UNIFORM_BUFFER
};

class buffer
{
public:
    buffer(target buffer_target)
        : buffer()
    {
        this->buffer_target = buffer_target;
    }
    buffer(){glGenBuffers(1, &id);}
    ~buffer(){glDeleteBuffers(1, &id);}

    void bind() const
    {
        glBindBuffer(static_cast<unsigned int>(buffer_target), id);
    }

    void data(int size, const void* data, unsigned int usage) const
    {
        bind();
        glBufferData(static_cast<unsigned int>(buffer_target),
            size, data, usage);
    }

private:
    target buffer_target = target::ARRAY_BUFFER;
    unsigned int id = 0u;

};




class vertex_array
{
public:
	vertex_array(){glGenVertexArrays(1, &id);}

	~vertex_array(){glDeleteVertexArrays(1, &id);}

	void bind() const{glBindVertexArray(id);}

	void attribute_pointer(unsigned int index, int size, unsigned int type,
		unsigned char normalized, int stride, const void* pointer) const
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

	void attribute_pointer(unsigned int index, int size, unsigned int type,
		int stride,const void* pointer) const
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribIPointer(index, size, type, stride, pointer);
    }

	void attribute_divisor(unsigned int index, unsigned int divisor) const
    {
        bind();
        glVertexAttribDivisor(index, divisor);
    }

private:
	unsigned int id;
};

#endif // BUFFER_HPP
