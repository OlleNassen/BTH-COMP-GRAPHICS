#include "texture.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

texture::texture(int width, int height,
    wrap wrap_parameter,
    filter filter_parameter,
    format format_parameter,
    type type_parameter)
{
    generate(wrap_parameter, filter_parameter);

    glTexImage2D(GL_TEXTURE_2D, 0,
        static_cast<int>(format_parameter),
        width, height, 0,
        static_cast<unsigned int>(format_parameter),
        static_cast<unsigned int>(type_parameter), nullptr);
}

texture::texture(const std::string& path,
    wrap wrap_parameter,
    filter filter_parameter,
    format format_parameter,
    type type_parameter)
{
    generate(wrap_parameter, filter_parameter);

    int width;
    int height;
    int nr_of_channels;
    unsigned char* data = stbi_load(path.c_str(),
        &width, &height, &nr_of_channels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0,
        static_cast<int>(format_parameter),
        width, height, 0,
        static_cast<unsigned int>(format_parameter),
        static_cast<unsigned int>(type_parameter), data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
}

texture::~texture()
{
    glDeleteTextures(1, &id);
}

void texture::uniform(const shader& shader, const std::string& name, int texture_index) const
{
    shader.uniform(name, texture_index);
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, id);
}

void texture::bind_to_buffer() const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
}

void texture::generate(wrap wrap_parameter, filter filter_parameter)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrap_parameter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrap_parameter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filter_parameter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filter_parameter));

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}
