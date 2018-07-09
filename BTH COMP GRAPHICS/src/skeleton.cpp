#include "skeleton.hpp"
#include "game.hpp"

skeleton::skeleton()
{
    for(int i = 0; i < 50; i++)
    {
        parents.push_back(0);
        joints.push_back(glm::mat4(1.0f));
        world_joints.push_back(glm::mat4(1.0f));
    }
}

skeleton::~skeleton()
{

}

void skeleton::update(float delta_time)
{
    current.update(delta_time, joints);

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
