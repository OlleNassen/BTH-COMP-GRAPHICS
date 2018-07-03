#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <vector>
#include <glm/glm.hpp>
#include "shader.hpp"

class scene_node
{
public:
    scene_node(float x = 0, float y = 0, float z = 0);
    virtual ~scene_node();
    void attach_child(scene_node* child);

    void update(float delta_time);
    void render(const shader& shader) const;

private:
    std::vector<scene_node*> children;
    glm::mat4 transform;

    void update(float delta_time, glm::mat4& world_transform);
    void render(const shader& shader, glm::mat4& world_transform) const;

    virtual void update_current(float delta_time,
        const glm::mat4& world_transform, glm::mat4& transform);
    virtual void render_current(const shader& shader, const glm::mat4& world_transform) const;

    void update_children(float delta_time, glm::mat4& world_transform);
    void render_children(const shader& shader, glm::mat4& world_transform) const;
};

#endif // SCENE_NODE_HPP
