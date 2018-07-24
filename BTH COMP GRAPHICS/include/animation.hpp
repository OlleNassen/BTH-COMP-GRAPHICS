#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <array>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "joint.hpp"

using namespace std::literals::chrono_literals;

struct pose
{
    glm::vec3 position;
    glm::quat rotation;
};

struct key_frame
{
    std::chrono::milliseconds time_point;
    skeleton_array<pose> poses;
};

class animation
{
public:
    void load(const std::vector<key_frame>& key_frames);
    animation();

    void update(const std::chrono::milliseconds delta_time, skeleton& joints);

private:
    std::chrono::milliseconds time;
    std::chrono::milliseconds length;
    std::vector<key_frame> key_frames;
    std::vector<key_frame>::iterator previous;
    std::vector<key_frame>::iterator next;

    void update_key_frame();
    void update_pose(skeleton& joints);
    float
        calculate_progression(
        const std::chrono::milliseconds previous,
        const std::chrono::milliseconds next) const;
};

#endif // ANIMATION_HPP
