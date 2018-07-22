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
	/*
    "images/skybox3/petesoasis_rt.tga", // RIGHT
    "images/skybox3/petesoasis_lf.tga", // LEFT
    "images/skybox3/petesoasis_up.tga", // TOP
    "images/skybox3/petesoasis_dn.tga", // BOTTOM
    "images/skybox3/petesoasis_bk.tga", // BACK
    "images/skybox3/petesoasis_ft.tga" // FRONT
	*/

	"images/skybox3/negx.jpg", // RIGHT
	"images/skybox3/posx.jpg", // LEFT
	"images/skybox3/posy.jpg", // TOP
	"images/skybox3/negy.jpg", // BOTTOM
	"images/skybox3/negz.jpg", // BACK
	"images/skybox3/posz.jpg" // FRONT


};

skybox::skybox()
    : vbo(target::ARRAY_BUFFER)
    , sky(faces, wrap::CLAMP_TO_EDGE,
        filter::LINEAR, format::RGB)
{
    vao.bind();
    vbo.data(sizeof(skybox_vertices), &skybox_vertices[0], GL_STATIC_DRAW);
    vao.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

skybox::~skybox()
{
    //dtor
}

void skybox::update_current(const std::chrono::milliseconds delta_time,
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
