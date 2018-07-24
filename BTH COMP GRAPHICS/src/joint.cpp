#include "joint.hpp"

namespace anim
{

joint::joint(const glm::mat4& transform, joint* parent)
    : parent(parent ? parent : this)
    , local_transform(transform)
    , global_transform(
        (parent ? parent->global_transform
        : glm::mat4(1.0f))
        * local_transform)
    , inverse_bind_pose(glm::inverse(global_transform))
{

}

joint& joint::operator=(const joint& other)
{
    if(this != &other)
    {
        parent = &other !=
            other.parent ?
            other.parent : parent;

        local_transform = other.local_transform;
        global_transform = parent->global_transform * local_transform;
        inverse_bind_pose = glm::inverse(global_transform);
    }

    return *this;
}

glm::mat4 joint::world_transform() const
{
    return global_transform * inverse_bind_pose;
}

}
