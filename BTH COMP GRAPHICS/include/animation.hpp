#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

struct joint
{
    unsigned int parent;
    glm::vec3 position;
    glm::quat rotation;
};

using skeleton = std::array<joint, 50>;

struct key_frame
{
    float time;
    skeleton pose;
};

class animation
{
public:
    void load(const std::vector<key_frame>& key_frames);
    animation();
    ~animation();

    void update(float delta_time, skeleton& joints);

private:
    float time;
    float length;
    int current_key_frame;
    std::vector<key_frame> key_frames;

    void update_key_frame();
    void update_pose(skeleton& joints);
};

#endif // ANIMATION_HPP
