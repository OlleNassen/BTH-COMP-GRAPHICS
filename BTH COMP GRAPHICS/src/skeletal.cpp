#include "skeletal.hpp"

skeletal::skeletal()
{
	robot = new cube_robot;
}

skeletal::~skeletal()
{
}

void skeletal::render_current(const shader& shader,
	const glm::mat4& world_transform)
{
	robot->update(0.000016f);
	robot->draw(shader);

	/*
	shader.uniform("model", transforms[i]);
	//quad_texture->uniform(shader, "object_material.diffuse", 0);
	objects[i].bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	*/
}
void skeletal_node::update(float delta_time)
{
    if (parent)
    {
        world_transform = parent->world_transform * transform;
    }
    else
    {
        world_transform = transform;
    }

    for (auto& child : children)
    {
        child->update(delta_time);
    }
}

void cube_robot::update(float delta_time)
{
    std::cout << "NO";
    std::cout << transform[3][0] << '\n';
    transform = transform *
        glm::rotate(transform, delta_time / 10.0f,
            glm::vec3(0.f, 1.f, 0.f));

    head->set_transform(head->get_transform() *
        glm::rotate(head->get_transform(), -delta_time / 10.0f,
            glm::vec3(0.f, 1.f, 0.f)));

    left_arm->set_transform(left_arm->get_transform() *
        glm::rotate(left_arm->get_transform(), -delta_time / 10.0f,
            glm::vec3(1.f, 0.f, 0.f)));

    right_arm->set_transform(right_arm->get_transform() *
        glm::rotate(right_arm->get_transform(), delta_time / 10.0f,
            glm::vec3(1.f, 0.f, 0.f)));

    skeletal_node::update(delta_time);
}
