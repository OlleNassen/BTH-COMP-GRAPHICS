#ifndef JOINT_HPP
#define JOINT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class joint
{
public:
    joint(const glm::mat4& transform = glm::mat4(1.0f), joint* parent = nullptr);
    joint& operator=(const joint& other);

    glm::mat4 world_transform();

private:
    joint* parent;

    glm::mat4 local_transform;
    glm::mat4 global_transform;
    glm::mat4 inverse_bind_pose;

};

#endif // JOINT_HPP
