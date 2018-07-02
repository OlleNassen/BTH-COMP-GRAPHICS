#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>

class shader
{
public:
    shader(const std::string& vertex_path, const std::string& fragment_path);
    ~shader();

    void use() const;

    void uniform(const std::string& name, int value) const;
    void uniform(const std::string& name, float value) const;
    void uniform(const std::string& name, const glm::vec2& value) const;
    void uniform(const std::string& name, const glm::vec3& value) const;
    void uniform(const std::string& name, const glm::vec4& value) const;
    void uniform(const std::string& name, const glm::mat2& value) const;
    void uniform(const std::string& name, const glm::mat3& value) const;
    void uniform(const std::string& name, const glm::mat4& value) const;

private:
    unsigned int id;

    std::string load(const std::string& path) const;
    unsigned int create(unsigned int shader_type, const char* shader_code) const;
};

#endif // SHADER_HPP
