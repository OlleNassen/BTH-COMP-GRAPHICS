#include "animation.hpp"
#include <iostream>
#include "game.hpp"

namespace anim
{

void animation::load(const std::vector<key_frame>& key_frames)
{
    this->key_frames = key_frames;
    previous = this->key_frames.begin();
    next = this->key_frames.begin();
    ++next;
}

animation::animation()
    : time(0ms)
{

}

void animation::update(milliseconds delta_time, skeleton& joints)
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

        if(next == key_frames.end())
        {
            time = 0ms;
            previous = key_frames.begin();
            next = key_frames.begin();
            ++next;
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
        auto& prev_pose = previous->poses[i];
        auto& next_pose = next->poses[i];

        auto new_position =
            glm::mix(prev_pose.position,
            next_pose.position, progression);

        auto new_rotation =
            glm::mix(prev_pose.rotation,
            next_pose.rotation, progression);

        glm::mat4 new_transform(1.0f);
        new_transform *= glm::translate(new_transform, new_position);
        new_transform *= glm::mat4_cast(new_rotation);

        if(i != 0u)
        {
            joints[i].transform(new_transform);
        }
        else
        {
            joints[i].local_transform = new_transform;
            joints[i].global_transform = joints[i].local_transform;
        }
        if(i == 25u)
        {
            std::cout << new_position.x << std::endl;
            std::cout << joints[i].local_transform << std::endl;
        }
    }
}

float animation::calculate_progression(
    milliseconds previous,
    milliseconds next) const
{
    auto total_time = next - previous;
    auto current_time = time - previous;

    return
        static_cast<float>(current_time.count()) /
        static_cast<float>(total_time.count());
}

}
