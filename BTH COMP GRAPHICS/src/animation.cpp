#include "animation.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

animation::animation()
{
    current_key_frame = 0;
    std::vector<joint_transform> pose;
    glm::mat4 rot(1.0f);

    for(int i = 0; i < 50; i++)
    {
        //rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        pose.push_back({0, glm::vec3(0,0,0), glm::quat_cast(rot)});
    }

    key_frame f;
    f.time = 10.0;
    f.pose = pose;
    key_frames.push_back(f);

    for(int i = 0; i < 50; i++)
    {
        //rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        pose.push_back({0, glm::vec3(10,0,0), glm::quat_cast(rot)});
    }

    f.time = 10.0;
    f.pose = pose;
    key_frames.push_back(f);
    key_frames.push_back(f);

}

animation::~animation()
{
    //dtor
}

void animation::update(float delta_time, std::vector<glm::mat4>& joints)
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

void animation::update_pose(std::vector<glm::mat4>& joints)
{
    key_frame& previous = key_frames[current_key_frame];
    key_frame& next = key_frames[current_key_frame + 1];

    float total_time = next.time - previous.time;
    float current_time = time - previous.time;
    float progression = current_time / total_time;

    for(int i = 0; i < joints.size(); i++)
    {
        glm::vec3 new_position = previous.pose[i].position;
        glm::quat new_rotation = previous.pose[i].rotation;

        if(previous.pose[i].position != next.pose[i].position)
        {
            new_position =
                glm::mix(previous.pose[i].position,
                next.pose[i].position, progression);
        }
        if(previous.pose[i].rotation != next.pose[i].rotation)
        {
            new_rotation =
                glm::slerp(previous.pose[i].rotation,
                next.pose[i].rotation, progression);
        }

        glm::mat4 new_transform = glm::mat4_cast(new_rotation);
        new_transform = glm::translate(new_transform, new_position);

        joints[i] = new_transform;
    }
}
