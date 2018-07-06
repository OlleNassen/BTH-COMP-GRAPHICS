#include "model.hpp"

model::model()
    : model_buffer(target::ARRAY_BUFFER)
{
    model_array.bind();
	model_buffer.data(sizeof(vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    model_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    model_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    model_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    //model_array.attribute_pointer_int(4, 4, GL_INT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //model_array.attribute_pointer(5, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

model::~model()
{
    //dtor
}
