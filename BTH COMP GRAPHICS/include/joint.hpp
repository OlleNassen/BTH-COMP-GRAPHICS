#ifndef JOINT_HPP
#define JOINT_HPP

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace anim
{

class joint
{
public:
    joint() = default;
    joint(const glm::mat4& transform, joint* parent);

    void transform(const glm::mat4& new_transform);

    glm::mat4 world_transform() const;

    joint* parent = this;
    glm::mat4 local_transform = glm::mat4(1.0f);
    glm::mat4 global_transform = glm::mat4(1.0f);
    glm::mat4 inverse_bind_pose = glm::mat4(1.0f);

private:

};

template <class T>
using skeleton_array = std::array<T, 50>;
using skeleton = skeleton_array<joint>;

}

#endif // JOINT_HPP
