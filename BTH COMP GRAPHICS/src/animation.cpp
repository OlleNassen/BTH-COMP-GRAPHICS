#include "animation.hpp"

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

animation::~animation()
{
    //dtor
}

void animation::update(float delta_time, skeleton& joints)
{
    time += delta_time;

    update_key_frame();
    update_pose(joints);
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
    key_frame& previous = key_frames[current_key_frame];
    key_frame& next = key_frames[current_key_frame + 1];

    for(int i = 0; i < joints.size(); i++)
    {
        glm::vec3 new_position = previous.pose[i].position;
        glm::quat new_rotation = previous.pose[i].rotation;

        new_position =
            glm::mix(previous.pose[i].position,
            next.pose[i].position, time);

        new_rotation =
            glm::slerp(previous.pose[i].rotation,
            next.pose[i].rotation, time);

        joints[i].position = new_position;
        joints[i].rotation = new_rotation;
    }
}
