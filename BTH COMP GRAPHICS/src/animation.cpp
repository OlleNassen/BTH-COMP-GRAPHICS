#include "animation.hpp"
#include <iostream>

namespace anim
{

void animation::load(const std::vector<key_frame>& key_frames)
{
    this->key_frames = key_frames;
    length = key_frames.back().time_point;
    previous = this->key_frames.begin();
    next = this->key_frames.begin();
    ++next;
}

animation::animation()
    : time(0ms)
    , length(0ms)
{

}

void animation::update(const milliseconds delta_time, skeleton& joints)
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
    if(time > next->time_point)
    {
        ++previous;
        ++next;

        if(next != key_frames.end())
        {
            previous = key_frames.begin();
            next = key_frames.begin();
            ++next;
        }

        if(time > length)
        {
            time = 0ms;
        }
    }
}

void animation::update_pose(skeleton& joints)
{
    auto progression =
        calculate_progression(
        previous->time_point,
        next->time_point);

    for(auto i = 0u; i < joints.size(); ++i)
    {
        auto new_position =
            glm::mix(previous->poses[i].position,
            next->poses[i].position, progression);

        auto new_rotation =
            glm::slerp(previous->poses[i].rotation,
            next->poses[i].rotation, progression);

        glm::mat4 new_transform(1.0f);
        new_transform *= glm::translate(new_transform, new_position);
        new_transform *= glm::mat4_cast(new_rotation);

        if(i == 0u)
        {
            joints[i].global_transform = new_transform;
        }
        else
        {
            joints[i].transform(new_transform);
        }
    }
}

float
    animation::calculate_progression(
    const milliseconds previous,
    const milliseconds next) const
{
    using namespace std::chrono;
    duration<float> total_time = next - previous;
    duration<float> current_time = time - previous;

    return current_time.count() / total_time.count();
}

}
