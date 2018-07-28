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
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
};

struct key_frame
{
    milliseconds time_point{0ms};
    skeleton_array<pose> poses;
};

class animation
{
public:
    void load(const std::vector<key_frame>& key_frames);

    void update(milliseconds delta_time, skeleton& joints);

private:
    milliseconds time{0ms};
    std::vector<key_frame> key_frames;
    std::vector<key_frame>::iterator previous;
    std::vector<key_frame>::iterator next;

    void update_key_frame();
    void update_pose(skeleton& joints);
    float calculate_progression(
        milliseconds previous,
        milliseconds next) const;
};

}

#endif // ANIMATION_HPP
