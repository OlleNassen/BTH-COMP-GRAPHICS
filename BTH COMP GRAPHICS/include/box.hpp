#ifndef BOX_HPP
#define BOX_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

class box : public scene_node
{
public:
    box(float x = 0.f, float y = 0.f, float z = 0.f);
    virtual ~box();

private:
	vertex_array box_array;
	buffer box_vbo;
	texture diffuse;
	texture specular;

     virtual void update_current(const std::chrono::milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform) override;

    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const override;
};

#endif // BOX_HPP
