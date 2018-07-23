#include "race.hpp"

constexpr race::sphere::sphere(const glm::vec3& position, float radius)
    : position(position)
    , radius(radius)
{

}

bool race::sphere::contains(const glm::vec3& other) const
{
    return glm::length(other - position) < radius;
}

race::race()
    : current_checkpoint(checkpoints.begin())
    , current_lap(0)
{

}

int race::lap() const
{
    return current_lap;
}

void race::update(const glm::vec3& position)
{
    if(current_checkpoint->contains(position))
    {
        if(current_checkpoint != checkpoints.end())
        {
            ++current_checkpoint;
        }
        else
        {
            ++current_lap;
            current_checkpoint = checkpoints.begin();
        }
    }
}
