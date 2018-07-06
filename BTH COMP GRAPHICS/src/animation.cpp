#include "animation.hpp"

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
    key_frame& previous = key_frames[current_key_frame-1];
    key_frame& next = key_frames[current_key_frame+1];


}
