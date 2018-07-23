#include "race.hpp"

sphere::sphere(const glm::vec3& position, float radius)
    : position(position)
    , radius(radius)
{

}

bool sphere::contains(const glm::vec3& other) const
{
    return glm::length(other - position) < radius;
}

race::race()
    : current_checkpoint(checkpoints.begin())
    , current_lap(0)
{

}

race::sphere_array::const_iterator race::begin() const
{
    return checkpoints.begin();
}

race::sphere_array::const_iterator race::end() const
{
    return checkpoints.end();
}

race::sphere_array::const_reference race::operator[](sphere_array::size_type pos) const
{
    return checkpoints[pos];
}

race::sphere_array::reference race::operator[](sphere_array::size_type pos)
{
    return checkpoints[pos];
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