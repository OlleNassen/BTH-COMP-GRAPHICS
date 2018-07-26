#ifndef BOX_HPP
#define BOX_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"

namespace scene
{

class box : public node
{
public:
    box(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f);
    virtual ~box();

private:
	vertex_array box_array;
	buffer box_vbo;
	texture diffuse;
	texture specular;

     virtual void update_current(const milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform) override;

    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const override;
};

}

#endif // BOX_HPP
