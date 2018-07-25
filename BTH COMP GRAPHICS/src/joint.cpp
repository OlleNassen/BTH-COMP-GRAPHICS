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
	local_transform = glm::inverse(local_transform);
	global_transform = (glm::inverse(global_transform));

}

void joint::transform(const glm::mat4& transform)
{
    local_transform = transform;
    global_transform =
        parent->global_transform
        * local_transform;
}

glm::mat4 joint::world_transform() const
{
    return global_transform * inverse_bind_pose;
}

}
