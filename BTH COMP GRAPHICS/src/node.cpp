#include "node.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

namespace scene
{

node::node(float x, float y, float z)
    : local{glm::translate(glm::mat4{1.0f}, {x, y, z})}
    , world{local}
{

}

void node::attach_child(node* child)
{
    children.push_back(child);
}

void node::clear()
{
    children.clear();
}

void node::sort(glm::vec3& pos)
{
    std::sort(children.begin(), children.end(),
        [&pos](const node* l, const scene::node* r) -> bool
    {
        return l->distance_to(pos) < r->distance_to(pos);
    });

    sort_children(pos);
}

void node::update(const milliseconds delta_time)
{
    glm::mat4 world_transform{1.0f};
    update(delta_time, world_transform);
}

void node::prepare_render(const shader& shader) const
{
    glm::mat4 m = world;
    prepare_render_current(shader, m);
	prepare_render_children(shader, m);
}

void node::render(const shader& shader) const
{
    glm::mat4 m = world;
    render_current(shader, m);
	render_children(shader, m);
}

void node::update(const milliseconds delta_time, glm::mat4& world_transform)
{
    world = world_transform * local;

    update_current(delta_time,
        world, local);
    update_children(delta_time, world);
}

void node::update_current(const std::chrono::milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{

}

void node::prepare_render_current(const shader& shader,
    const glm::mat4& world_transform) const
{

}


void node::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{

}

void node::sort_children(glm::vec3& pos)
{
    for(auto* child : children)
    {
        child->sort(pos);
    }
}

void node::update_children(milliseconds delta_time, glm::mat4& world_transform)
{
    for(auto* child : children)
    {
        child->update(delta_time, world_transform);
    }
}

void node::prepare_render_children(const shader& shader,
    glm::mat4& world_transform) const
{
    for(const auto* child : children)
    {
        child->prepare_render(shader);
    }
}

void node::render_children(const shader& shader,
    glm::mat4& world_transform) const
{
    for(const auto* child : children)
    {
        child->render(shader);
    }
}

float node::distance_to(const glm::vec3& other) const
{
    glm::vec3 position{world[3]};
    return glm::length(position - other);
}

}


