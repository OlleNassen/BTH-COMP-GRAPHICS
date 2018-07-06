#include "model.hpp"

model::model()
    : model_buffer(target::ARRAY_BUFFER)
{
    /*float vertices[] =
	{
		// positions     // uv
		0.0f, 256.f, 0.f, 0.0f, 1.0f,
		256.f, 0.0f, 0.f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.f, 0.0f, 0.0f,

		0.0f, 256.f, 0.f, 0.0f, 1.0f,
		256.f, 256.f, 0.f, 1.0f, 1.0f,
		256.f, 0.0f, 0.f, 1.0f, 0.0f
	};*/

    model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    model_array.attribute_pointer(4, 4, GL_INT, 8 * sizeof(float), (void*)(8 * sizeof(float)));
    model_array.attribute_pointer(5, 4, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(8 * sizeof(float) + 4 * sizeof(int)));
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
    shader.uniform("joints", skel.transforms());
    model_array.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
