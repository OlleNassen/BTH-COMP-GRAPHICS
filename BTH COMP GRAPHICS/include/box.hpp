#ifndef BOX_HPP
#define BOX_HPP

#include "scene_node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"

class box : public scene_node
{
public:
    box();
    virtual ~box();

private:
	vertex_array box_array;
	buffer box_vbo;

     virtual void update_current(float delta_time,
        const glm::mat4& world_transform, glm::mat4& transform) override;

    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const override;
};

#endif // BOX_HPP
