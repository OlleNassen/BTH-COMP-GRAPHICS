#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>
#include <GL/glew.h>
#include "shader.hpp"

enum class wrap
{
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE
};

enum class filter
{
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

enum class format
{
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA
};

enum class type
{
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    BYTE = GL_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SHORT = GL_SHORT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    INT = GL_INT,
    FLOAT = GL_FLOAT
};

class texture
{
public:
    texture(const int width, const int height,
        const wrap wrap_parameter = wrap::REPEAT,
        const filter filter_parameter = filter::LINEAR,
        const format format_parameter = format::RGBA,
        const type type_parameter = type::UNSIGNED_BYTE);

    texture(const std::string& path,
        const wrap wrap_parameter = wrap::REPEAT,
        const filter filter_parameter = filter::LINEAR,
        const format format_parameter = format::RGBA,
        const type type_parameter = type::UNSIGNED_BYTE);

    texture(const std::vector<std::string>& paths,
        const wrap wrap_parameter = wrap::REPEAT,
        const filter filter_parameter = filter::LINEAR,
        const format format_parameter = format::RGBA,
        const type type_parameter = type::UNSIGNED_BYTE);

    ~texture();

    void uniform(const shader& shader, const std::string& name, const int texture_index = 0) const;
    void skybox(const shader& shader) const;
    void bind_to_buffer() const;

private:
    unsigned int id;

    void generate(const wrap wrap_parameter, const filter filter_parameter);
};

#endif // TEXTURE_HPP
