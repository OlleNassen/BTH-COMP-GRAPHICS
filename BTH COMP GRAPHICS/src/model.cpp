#include "model.hpp"

model::model()
    : model_buffer(target::ARRAY_BUFFER)
{
    /*struct vertex
    {
        glm::vec3 position;
        glm::vec2 texture_coordinate;
        glm::vec3 normal;
        glm::ivec4 joints;
        glm::vec4 weights;
    };*/
	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,0,0,0), glm::vec4(1,0,0,0) });
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,0,0,0), glm::vec4(1,0,0,0) });
	vertices.push_back({ glm::vec3(0,0,0), glm::vec2(0,0), glm::vec3(0,0,1), glm::ivec4(1,0,0,0), glm::vec4(1,0,0,0) });

	vertices.push_back({ glm::vec3(0,1,0), glm::vec2(0,1), glm::vec3(0,0,1), glm::ivec4(0,0,0,0), glm::vec4(1,0,0,0) });
	vertices.push_back({ glm::vec3(1,1,0), glm::vec2(1,1), glm::vec3(0,0,1), glm::ivec4(0,0,0,0), glm::vec4(1,0,0,0) });
	vertices.push_back({ glm::vec3(1,0,0), glm::vec2(1,0), glm::vec3(0,0,1), glm::ivec4(0,0,0,0), glm::vec4(1,0,0,0) });

    int stride = 12 * sizeof(float) + 4 * sizeof(int);

    model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

    model_array.attribute_pointer(3, 4, GL_INT, stride, (void*)(8 * sizeof(float)));
    model_array.attribute_pointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float) + 4 * sizeof(int)));
}

model::~model()
{
    //dtor
}

void model::update(float delta_time)
{
    skel.update(delta_time);
}

void model::draw(const shader& shader) const
{
    shader.uniform("joint_transforms", skel.transforms());
    model_array.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
