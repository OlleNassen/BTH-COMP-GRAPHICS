#include "skeleton.hpp"

skeleton::skeleton()
{
    //ctor
}

skeleton::~skeleton()
{
    //dtor
}

void skeleton::fn(const shader& shader)
{
    for(int i = 0; i < joints.size(); i++)
    {
        world_joints[i] = joints[i];
        for(int j = parents[i]; parents[j] != 0; j = parents[j])
        {
            world_joints[i] *= joints[j];
        }
    }
    shader.uniform("joints", world_joints);
}
