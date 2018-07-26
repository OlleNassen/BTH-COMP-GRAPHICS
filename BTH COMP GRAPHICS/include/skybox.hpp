#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class skybox : public node
{
public:
    skybox();
    virtual ~skybox();

private:
    buffer vbo;
    vertex_array vao;
    texture sky;

    virtual void update_current(const milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform) override;

    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const override;
};

}

#endif // SKYBOX_HPP
