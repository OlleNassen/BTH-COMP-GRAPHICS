#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class skybox : public scene_node
{
public:
    skybox();
    virtual ~skybox();

private:
    buffer vbo;
    vertex_array vao;
    texture sky;

    virtual void update_current(const std::chrono::milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform) override;

    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const override;
};

#endif // SKYBOX_HPP
