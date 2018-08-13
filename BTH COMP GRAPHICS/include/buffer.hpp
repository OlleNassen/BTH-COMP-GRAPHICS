#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <array>
#include <vector>
#include <tuple>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "texture.hpp"

class buffer
{
public:
    buffer(unsigned int buffer_target)
        : buffer()
    {
        this->buffer_target = buffer_target;
    }
    buffer(){glGenBuffers(1, &id);}
    ~buffer(){glDeleteBuffers(1, &id);}
    void bind() const{glBindBuffer(buffer_target, id);}

    void data(int size, const void* data, unsigned int usage) const
    {
        bind();
        glBufferData(buffer_target, size, data, usage);
    }

    void sub_data(int offset, int size,
        const void* data) const
    {
        glBufferSubData(buffer_target, offset, size, data);
    }

    template<typename T, unsigned int N>
    void data(const std::array<T, N>& data, unsigned int usage) const
    {
        bind();
        glBufferData(buffer_target,
            sizeof(T) * data.size(), data.data(), usage);
    }

    template<typename T>
    void data(const std::vector<T>& data, unsigned int usage) const
    {
        bind();
        glBufferData(buffer_target,
            sizeof(T) * data.size(), data.data(), usage);
    }

private:
    unsigned int buffer_target = GL_ARRAY_BUFFER;
    unsigned int id = 0u;

};

class frame_buffer
{
public:
	frame_buffer()
	{
		glGenFramebuffers(1, &id);
	}
	~frame_buffer()
	{
		glDeleteFramebuffers(1, &id);
	}
	void bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}
	void bind_texture(const texture& texture) const
	{
		bind();
		texture.bind_to_buffer();
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

private:
	unsigned int id;

};

template<typename T, unsigned int N>
class attrib
{
public:
    int size = sizeof(T);
    unsigned int type = N;
};

constexpr attrib<float, GL_FLOAT> attrib_f;
constexpr attrib<int, GL_INT> attrib_i;
constexpr attrib<unsigned int, GL_UNSIGNED_INT> attrib_ui;

constexpr attrib<glm::vec2, GL_FLOAT> attrib_v2;
constexpr attrib<glm::vec3, GL_FLOAT> attrib_v3;
constexpr attrib<glm::vec4, GL_FLOAT> attrib_v4;

constexpr attrib<glm::ivec2, GL_INT> attrib_iv2;
constexpr attrib<glm::ivec3, GL_INT> attrib_iv3;
constexpr attrib<glm::ivec4, GL_INT> attrib_iv4;

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
		int stride, const void* pointer) const
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


    template <typename Head, typename... Tail>
    void calculate_stride(int& stride, const Head& head, const Tail&... tail)
    {
        if(sizeof...(tail))
        {
            stride += head.size;
            calculate_stride(stride, tail...);
        }
    }

    template <typename Head, typename... Tail>
    void for_each_argument(int i, int stride, int offset,
        const Head& head, const Tail&... tail)
    {
        if(sizeof...(tail))
        {
            glEnableVertexAttribArray(i);
            if(head.type == GL_INT || head.type == GL_UNSIGNED_INT)
            {
                glVertexAttribIPointer(i, head.size, head.type,
                    stride, &offset);
            }
            else
            {
                glVertexAttribPointer(i, head.size, head.type,
                    GL_FALSE, stride, &offset);
            }

            for_each_argument(++i, stride, head.size + offset, tail...);
        }
    }


    template<typename... Args>
    void attributes(Args... args)
    {
        bind();
        int stride = 0;
        calculate_stride(stride, args...);
        for_each_argument(0,stride,0, args...);
    }

private:
	unsigned int id;
};

#endif // BUFFER_HPP
