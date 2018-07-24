#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <array>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "joint.hpp"

namespace anim
{

using namespace std::literals::chrono_literals;
using milliseconds = std::chrono::milliseconds;

struct pose
{
    glm::vec3 position;
    glm::quat rotation;
};

struct key_frame
{
    milliseconds time_point;
    skeleton_array<pose> poses;
};

class animation
{
public:
    void load(const std::vector<key_frame>& key_frames);
    animation();

    void update(const milliseconds delta_time, skeleton& joints);

private:
    milliseconds time;
    milliseconds length;
    std::vector<key_frame> key_frames;
    std::vector<key_frame>::iterator previous;
    std::vector<key_frame>::iterator next;

    void update_key_frame();
    void update_pose(skeleton& joints);
    float
        calculate_progression(
        const milliseconds previous,
        const milliseconds next) const;
};

}

#endif // ANIMATION_HPP
