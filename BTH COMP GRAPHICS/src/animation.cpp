#include "animation.hpp"

void animation::load(const std::vector<key_frame>& key_frames)
{
    this->key_frames = key_frames;
}

animation::animation()
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
    if(time.count() > key_frames[current_key_frame].time)
    {
        current_key_frame =
            (current_key_frame + 1)
            % (key_frames.size() - 1);
        time = 0s;
    }
}

void animation::update_pose(skeleton& joints)
{
    auto& previous = key_frames[current_key_frame];
    auto& next = key_frames[current_key_frame + 1];
    auto current_time = time.count();

    for(auto i = 0u; i < joints.size(); ++i)
    {
        auto new_position =
            glm::mix(previous.poses[i].position,
            next.poses[i].position, current_time);

        auto new_rotation =
            glm::slerp(previous.poses[i].rotation,
            next.poses[i].rotation, current_time);

        glm::mat4 new_transform(1.0f);
        new_transform *= glm::translate(new_transform, new_position);
        new_transform *= glm::mat4_cast(new_rotation);

        joints[i] = joint(new_transform);

    }
}
