#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include "shader.hpp"

namespace scene
{

using namespace std::literals::chrono_literals;
using milliseconds = std::chrono::milliseconds;

template< typename T>
static constexpr char* buffer_offset(unsigned int offset)
{
    return reinterpret_cast<char*>(offset * sizeof(T));
}

class node
{
public:
    node(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    virtual ~node();

    void attach_child(node* child);

    void sort(glm::vec3& pos);
    void update(milliseconds delta_time);
    void prepare_render(const shader& shader) const;
    void render(const shader& shader) const;

private:
    std::vector<node*> children;
    glm::mat4 transform;


    void update(milliseconds delta_time, glm::mat4& world_transform);
    void prepare_render(const shader& shader,
        glm::mat4& world_transform) const;
    void render(const shader& shader, glm::mat4& world_transform) const;

    virtual void update_current(milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform);
    virtual void prepare_render_current(const shader& shader,
        const glm::mat4& world_transform) const;
    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const;

    void sort_children(glm::vec3& pos);
    void update_children(const milliseconds delta_time,
        glm::mat4& world_transform);
    void prepare_render_children(const shader& shader,
        glm::mat4& world_transform) const;
    void render_children(const shader& shader,
        glm::mat4& world_transform) const;

    float distance_to(const glm::vec3& other) const;
};

}

#endif // SCENE_NODE_HPP
