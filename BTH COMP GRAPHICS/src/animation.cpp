#include "animation.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

animation::animation()
{
    current_key_frame = 0;
    std::vector<joint_transform> pose;
    for(int i = 0; i < 50; i++)
    {
        pose.push_back({0, glm::vec3(0,0,0), glm::quat_cast(glm::mat4(1.0f))});
    }

    key_frame f;
    f.time = 1.0;
    f.pose = pose;
    key_frames.push_back(f);
    key_frames.push_back(f);
    key_frames.push_back(f);
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
        glm::vec3 new_position =
            glm::mix(previous.pose[i].position,
            next.pose[i].position, progression);

        glm::quat new_rotation =
            glm::mix(previous.pose[i].rotation,
            next.pose[i].rotation, progression);

        new_position = glm::vec3(0,0,0);
        new_rotation = glm::quat_cast(glm::mat4(1.0f));

        glm::mat4 new_transform = glm::mat4_cast(new_rotation);
        new_transform = glm::translate(new_transform, new_position);

        joints[i] = new_transform;
    }
}
