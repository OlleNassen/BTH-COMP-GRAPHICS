#include "animation.hpp"
#include <iostream>

void animation::load(const std::vector<key_frame>& key_frames)
{
    this->key_frames = key_frames;
    length = key_frames.back().time_point;
}

animation::animation()
    : current_key_frame(0)
{

}

void animation::update(const std::chrono::milliseconds delta_time, skeleton& joints)
{
    time += delta_time;

    if(!key_frames.empty())
    {
        update_key_frame();
        update_pose(joints);
    }
}

void animation::update_key_frame()
{
    if(time > key_frames[current_key_frame].time_point)
    {
        ++current_key_frame;

        if(current_key_frame >= key_frames.size())
        {
            current_key_frame = 1;
        }

        if(time > length)
        {
            time = 0s;
        }
    }
}

void animation::update_pose(skeleton& joints)
{
    auto& previous = key_frames[current_key_frame - 1];
    auto& next = key_frames[current_key_frame];

    auto progression =
        calculate_progression(
        previous.time_point,
        next.time_point);

    std::cout << progression << std::endl;

    for(auto i = 0u; i < joints.size(); ++i)
    {
        auto new_position =
            glm::mix(previous.poses[i].position,
            next.poses[i].position, progression);

        auto new_rotation =
            glm::slerp(previous.poses[i].rotation,
            next.poses[i].rotation, progression);

        glm::mat4 new_transform(1.0f);
        new_transform *= glm::translate(new_transform, new_position);
        new_transform *= glm::mat4_cast(new_rotation);

        joints[i] = joint(new_transform);

    }
}

float
    animation::calculate_progression(
    const std::chrono::milliseconds previous,
    const std::chrono::milliseconds next) const
{
    using namespace std::chrono;
    duration<float> total_time = next - previous;
    duration<float> current_time = time - previous;

    return current_time.count() / total_time.count();
}
