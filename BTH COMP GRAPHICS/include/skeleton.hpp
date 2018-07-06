#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>


class skeleton
{
public:
    skeleton();
    ~skeleton();

    void update(float delta_time);
    const std::vector<glm::mat4>& transforms() const;


private:
    std::vector<unsigned int> parents;
    std::vector<glm::mat4> joints;
    std::vector<glm::mat4> world_joints;

};

#endif // SKELETON_HPP
