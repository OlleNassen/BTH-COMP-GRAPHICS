#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <GL/glew.h>

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
    buffer(const target buffer_target);
    ~buffer();

    void bind() const;

    void data(const int size, const void* data, const unsigned int usage) const;

private:
    unsigned int buffer_target;
    unsigned int id;

};

#endif // BUFFER_HPP
