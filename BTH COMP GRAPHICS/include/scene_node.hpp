#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include "shader.hpp"

using namespace std::literals::chrono_literals;

template< typename T>
static constexpr char* buffer_offset(unsigned int offset)
{
    return reinterpret_cast<char*>(offset * sizeof(T));
}

class scene_node
{
public:
    scene_node(float x = 0, float y = 0, float z = 0);
    virtual ~scene_node();
    void attach_child(scene_node* child);

    void update(const std::chrono::milliseconds delta_time);
    void prepare_render(const shader& shader) const;
    void render(const shader& shader) const;

private:
    std::vector<scene_node*> children;
    glm::mat4 transform;

    void update(const std::chrono::milliseconds delta_time, glm::mat4& world_transform);
    void prepare_render(const shader& shader, glm::mat4& world_transform) const;
    void render(const shader& shader, glm::mat4& world_transform) const;

    virtual void update_current(const std::chrono::milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform);
    virtual void prepare_render_current(const shader& shader, const glm::mat4& world_transform) const;
    virtual void render_current(const shader& shader, const glm::mat4& world_transform) const;

    void update_children(const std::chrono::milliseconds delta_time, glm::mat4& world_transform);
    void prepare_render_children(const shader& shader, glm::mat4& world_transform) const;
    void render_children(const shader& shader, glm::mat4& world_transform) const;
};

#endif // SCENE_NODE_HPP
