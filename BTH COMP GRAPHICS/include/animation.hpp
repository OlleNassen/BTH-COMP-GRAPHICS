#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct joint
{
    unsigned int parent;
    glm::mat4 transform;
};

struct joint_transform
{
    int joint;
    glm::vec3 position;
    glm::quat rotation;
};

struct key_frame
{
    float time;
    std::array<joint_transform, 50> pose;
};

class animation
{
public:
    animation();
    ~animation();

    void update(float delta_time, std::array<joint, 50>& joints);

private:
    float time;
    float length;
    int current_key_frame;
    std::vector<key_frame> key_frames;

    void update_key_frame();
    void update_pose(std::array<joint, 50>& joints);
};

#endif // ANIMATION_HPP
