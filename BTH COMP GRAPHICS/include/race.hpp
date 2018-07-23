#ifndef RACE_HPP
#define RACE_HPP

#include <iostream>
#include <chrono>
#include <array>
#include <glm/glm.hpp>

class race
{
public:
    race();

    int lap() const;
    void update(const glm::vec3& position);

private:
    class sphere
    {
    public:
        constexpr sphere(const glm::vec3& position
            = glm::vec3(0.0f, 0.0f, 0.0f),
            float radius = 0.0f);

        bool contains(const glm::vec3& other) const;

        glm::vec3 position;
        float radius;
    };
    using sphere_array = std::array<sphere, 10>;
    sphere_array checkpoints;
    sphere_array::const_iterator current_checkpoint;
    int current_lap;
};

#endif // RACE_HPP
