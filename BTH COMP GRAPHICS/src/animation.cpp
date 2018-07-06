#include "animation.hpp"
#include <glm/gtc/matrix_transform.hpp>

animation::animation()
{
    //ctor
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

    if(joints.size() >= key_frames[0].pose[0].joint)
    {

    }
}

void animation::update_key_frame()
{
    if(time > key_frames[current_key_frame].time)
    {
        current_key_frame =
            (current_key_frame + 1)
            % key_frames.size();
    }
}

void animation::update_pose(std::vector<glm::mat4>& joints)
{
    key_frame& previous = key_frames[current_key_frame];
    key_frame& next = key_frames[current_key_frame+1];

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

        glm::mat4 new_transform = glm::mat4_cast(new_rotation);
        new_transform = glm::translate(new_transform, new_position);

        joints[i] = new_transform;
    }
}
