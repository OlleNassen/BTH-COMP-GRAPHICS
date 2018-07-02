#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

shader::shader(const std::string& vertex_path, const std::string& fragment_path)
{
    std::string vertex_code(load(vertex_path));
    std::string fragment_code(load(fragment_path));
    unsigned int vertex_shader = create(GL_VERTEX_SHADER, vertex_code.c_str());
    unsigned int fragment_shader = create(GL_FRAGMENT_SHADER, fragment_code.c_str());

    int success;
    char infoLog[512];

    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Error linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

shader::~shader()
{
    glDeleteProgram(id);
}

void shader::use() const
{
    glUseProgram(id);
}

void shader::uniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void shader::uniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void shader::uniform(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::mat2& value) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()),
        1, GL_FALSE, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()),
        1, GL_FALSE, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
        1, GL_FALSE, glm::value_ptr(value));
}

std::string shader::load(const std::string& path) const
{
    std::string code;
    std::ifstream shader_file;
    std::stringstream shader_stream;

    shader_file.open(path);
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    code = shader_stream.str();

    return code;
}

unsigned int shader::create(GLenum shader_type, const GLchar* shader_code) const
{
    unsigned int shader_id;
    int success;
    char infoLog[512];

    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
        std::cout << "Error compile failed: " << infoLog << std::endl;
    };

    return shader_id;
}
