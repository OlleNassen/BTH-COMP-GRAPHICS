#include "skeleton.hpp"

skeleton::skeleton()
{
    //ctor
}

skeleton::~skeleton()
{
    //dtor
}

void skeleton::update(float delta_time)
{
    for(int i = 0; i < joints.size(); i++)
    {
        world_joints[i] = joints[i];
        for(int j = parents[i]; parents[j] != 0; j = parents[j])
        {
            world_joints[i] *= joints[j];
        }
    }
}

const std::vector<glm::mat4>& skeleton::transforms() const
{
    return world_joints;
}
