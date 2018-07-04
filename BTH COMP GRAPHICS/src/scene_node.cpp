#include "scene_node.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

scene_node::scene_node(float x, float y, float z)
    : children()
    , transform(
        translate(glm::mat4(1.0f),
        glm::vec3(x, y, z)))
{

}

scene_node::~scene_node()
{
    for(auto* child : children)
    {
        delete child;
    }
}

void scene_node::attach_child(scene_node* child)
{
    children.push_back(child);
}

void scene_node::update(float delta_time)
{
    glm::mat4 world_transform(1.0f);
    update(delta_time, world_transform);
}

void scene_node::render(const shader& shader) const
{

    glm::mat4 world_transform(1.0f);
    render(shader, world_transform);
}

void scene_node::update(float delta_time, glm::mat4& world_transform)
{
    world_transform *= transform;

    update_current(delta_time,
        world_transform, transform);
    update_children(delta_time, world_transform);
}

void scene_node::render(const shader& shader, glm::mat4& world_transform) const
{
    world_transform *= transform;

    render_current(shader, world_transform);
	render_children(shader, world_transform);
}

void scene_node::update_current(float delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{

}

void scene_node::render_current(const shader& shader, const glm::mat4& world_transform) const
{

}

void scene_node::update_children(float delta_time, glm::mat4& world_transform)
{
    for(auto* child : children)
    {
        child->update(delta_time, world_transform);
    }
}

void scene_node::render_children(const shader& shader, glm::mat4& world_transform) const
{
    for(const auto& child : children)
    {
        child->render(shader, world_transform);
    }
}
