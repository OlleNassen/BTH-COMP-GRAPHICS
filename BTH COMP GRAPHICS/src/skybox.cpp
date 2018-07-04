#include "skybox.hpp"

float skybox_vertices[] =
{
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
    "images/skybox/highly-overrated_rt.tga", // RIGHT
    "images/skybox/highly-overrated_lf.tga", // LEFT
    "images/skybox/highly-overrated_up.tga", // TOP
    "images/skybox/highly-overrated_dn.tga", // BOTTOM
    "images/skybox/highly-overrated_bk.tga", // BACK
    "images/skybox/highly-overrated_ft.tga" // FRONT
};

skybox::skybox()
    : vbo(target::ARRAY_BUFFER)
    , sky(faces, wrap::CLAMP_TO_EDGE,
        filter::LINEAR, format::RGB)
{
    vbo.data(sizeof(skybox_vertices), &skybox_vertices[0], GL_STATIC_DRAW);
    vao.bind();
    vao.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

skybox::~skybox()
{
    //dtor
}

void skybox::update_current(float delta_time,
        const glm::mat4& world_transform, glm::mat4& transform)
{

}

void skybox::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
    sky.skybox(shader);
    shader.uniform("model", world_transform);

    glDepthFunc(GL_LEQUAL);
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
