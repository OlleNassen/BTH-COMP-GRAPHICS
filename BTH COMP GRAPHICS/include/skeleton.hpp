#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <string>
#include <vector>

namespace glm
{
class mat4
{
public:
    void operator*=(const mat4& other)
    {

    }
};
}
class shader
{
public:
    void uniform(const std::string& name, const std::vector<glm::mat4>& value) const
    {

    }
};

class skeleton
{
public:
    skeleton();
    ~skeleton();

    void fn(const shader& shader);

private:
    std::vector<unsigned int> parents;
    std::vector<glm::mat4> joints;
    std::vector<glm::mat4> world_joints;

};

#endif // SKELETON_HPP
