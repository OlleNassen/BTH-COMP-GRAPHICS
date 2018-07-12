#include "model.hpp"
#include <iostream>

model::model()
    : model_buffer(target::ARRAY_BUFFER)
{
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,1,0), glm::vec2(1,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });

	vertices.push_back({ glm::vec3(0,0,0), glm::vec2(0,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });
	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,1,0,0), glm::vec4(0.5,0.5, 0,0) });

    int stride = 12 * sizeof(float) + 4 * sizeof(int);

    model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

    model_array.attribute_pointer(3, 4, GL_INT, stride, (void*)(8 * sizeof(float)));
    model_array.attribute_pointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float) + 4 * sizeof(int)));

    joints.fill({ 0, glm::vec3(0.0f), glm::quat_cast(glm::mat4(1.0f)) });
    world_joints.fill(glm::mat4(1.0f));
}

model::~model()
{
    //dtor
}

void model::update(float delta_time)
{
    current.update(delta_time, joints);

    for(unsigned int i = 0; i < joints.size(); i++)
    {
        glm::mat4 new_transform = glm::mat4_cast(joints[i].rotation);
        new_transform = glm::translate(new_transform, joints[i].position);

        world_joints[i] = new_transform;

        for(int j = joints[i].parent;
            joints[j].parent != 0;
            j = joints[j].parent)
        {
            glm::mat4 parent_transform = glm::mat4_cast(joints[j].rotation);
            parent_transform = glm::translate(new_transform, joints[j].position);

            world_joints[i] *= parent_transform;
        }
    }
}

void model::draw(const shader& shader) const
{
    shader.uniform("joint_transforms", world_joints);
    model_array.bind();
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}
