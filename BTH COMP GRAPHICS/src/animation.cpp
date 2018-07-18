#include "animation.hpp"

void animation::load(const std::vector<key_frame>& key_frames)
{
    this->key_frames = key_frames;
}

animation::animation()
{
    time = 0.0f;
    current_key_frame = 0;
    glm::mat4 rot(1.0f);

    //rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

    key_frame f;
    f.time = 1.0;
    f.pose.fill({0, glm::vec3(0,0,0), glm::quat_cast(rot)});
    key_frames.push_back(f);

    f.time = 1.0;
    f.pose.fill({0, glm::vec3(0,1,0), glm::quat_cast(rot)});
    key_frames.push_back(f);

    f.time = 1.0;
    f.pose.fill({0, glm::vec3(0,0,0), glm::quat_cast(rot)});
    key_frames.push_back(f);

}

void animation::update(float delta_time, skeleton& joints)
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
    if(time > key_frames[current_key_frame].time)
    {
        current_key_frame =
            (current_key_frame + 1)
            % (key_frames.size() - 1);
        time = 0;
    }
}

void animation::update_pose(skeleton& joints)
{
    auto& previous = key_frames[current_key_frame];
    auto& next = key_frames[current_key_frame + 1];

    for(auto i = 0u; i < joints.size(); i++)
    {
        auto new_position =
            glm::mix(previous.pose[i].position,
            next.pose[i].position, time);

        auto new_rotation =
            glm::slerp(previous.pose[i].rotation,
            next.pose[i].rotation, time);


        //new_rotation = glm::normalize(new_rotation);

        joints[i].position = new_position;
        joints[i].rotation = new_rotation;
    }
}
