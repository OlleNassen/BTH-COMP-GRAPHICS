#ifndef RACE_HPP
#define RACE_HPP

#include <iostream>
#include <chrono>
#include <array>
#include <glm/glm.hpp>

class sphere
{
public:
    sphere(const glm::vec3& position
        = glm::vec3(0.0f, 0.0f, 0.0f),
        float radius = 0.0f);

    bool contains(const glm::vec3& other) const;

    glm::vec3 position;
    float radius;
};

class race
{
public:
    race();
    using sphere_array = std::array<sphere, 10>;

    sphere_array::const_iterator begin() const;
    sphere_array::const_iterator end() const;
    sphere_array::const_reference operator[](sphere_array::size_type pos) const;
    sphere_array::reference operator[](sphere_array::size_type pos);

    int lap() const;
    void update(const glm::vec3& position);

private:


    sphere_array checkpoints;
    sphere_array::const_iterator current_checkpoint;
    int current_lap;
};

#endif // RACE_HPP