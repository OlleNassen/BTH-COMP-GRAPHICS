#include "../include/game.hpp"
#include <iostream>
#include "shader.hpp"
#include "buffer.hpp"
#include "vertex_array.hpp"
#include "directional_light.hpp"
#include "texture.hpp"
#include "frame_buffer.hpp"
//#include "mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

game::game()
    : game_window(WIDTH, HEIGHT, "EdvardGame")
    , camera(glm::radians(45.0f), WIDTH, HEIGHT, 0.1f, 100.0f)
{
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error glew init failed" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

game::~game()
{

}

void game::run()
{
    shader shadow_shader("shaders/shadow.vs", "shaders/shadow.fs");

    //mesh mesh("models/boblampclean.md5");

    glm::vec3 light_direction(0.0f, -1.0f, 0.0f);
    glm::vec3 ambient_light(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse_light(0.5f, 0.5f, 0.5f);
    glm::vec3 specular_light(1.0f, 1.0f, 1.0f);
    directional_light light(light_direction,
        ambient_light, diffuse_light, specular_light);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH /(float)HEIGHT, 0.1f, 100.0f);

    texture texture_diffuse("images/container2.png");
    texture texture_specular("images/container2_specular.png");
    texture texture_ground("images/ground.png");

    shader shader("shaders/basic.vs", "shaders/basic.fs");
    shader.use();

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };


    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    buffer vertex_buffer(target::ARRAY_BUFFER);
    vertex_buffer.data(sizeof(vertices), vertices, GL_STATIC_DRAW);

    buffer element_buffer(target::ELEMENT_ARRAY_BUFFER);
    element_buffer.data(sizeof(indices), indices, GL_STATIC_DRAW);

    vertex_array vertex_array;
    vertex_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    vertex_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vertex_array.attribute_pointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    shadow_shader.use();

    texture buffer_texture(1024, 1024, wrap::CLAMP_TO_EDGE,
        filter::NEAREST, format::DEPTH_COMPONENT, type::FLOAT);
    frame_buffer frame_buffer;
    frame_buffer.bind_texture(buffer_texture);

    while (game_window.is_open())
    {
        float delta_time = 0.016f;

        camera.update(delta_time);

        float near_plane = 1.0f, far_plane = 50.0f;//7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        shadow_shader.use();
        shadow_shader.uniform("light_space_matrix", lightSpaceMatrix);


        glViewport(0, 0, 1024, 1024);
        frame_buffer.bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        vertex_array.bind();

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model(1);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shadow_shader.uniform("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shader.uniform("model", model);

        glm::mat4 mvp = camera.model_view_projection(model);
        shader.uniform("model_view_projection", mvp);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        texture_ground.uniform(shader, "object_material.diffuse", 0);
        texture_ground.uniform(shader, "object_material.specular", 1);

        model = glm::mat4(1);
        glm::vec3 floor_position(0.0f, -53.0f, 0.0f);
        model = glm::translate(model, floor_position);
        model = glm::scale(model, glm::vec3(100.0f));
        shader.uniform("model", model);

        mvp = camera.model_view_projection(model);
        shader.uniform("model_view_projection", mvp);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        texture_diffuse.uniform(shader, "object_material.diffuse", 0);
        texture_specular.uniform(shader, "object_material.specular", 1);

        buffer_texture.uniform(shader, "shadow_map", 2);

        camera.bind(shader);
        light.bind(shader);

        shader.uniform("light_space_matrix", lightSpaceMatrix);

        shader.uniform("point_lights[0].position", pointLightPositions[0]);
        shader.uniform("point_lights[0].ambient", ambient_light);
        shader.uniform("point_lights[0].diffuse", diffuse_light);
        shader.uniform("point_lights[0].specular", specular_light);
        shader.uniform("point_lights[0].constant", 1.0f);
        shader.uniform("point_lights[0].linear", 0.09f);
        shader.uniform("point_lights[0].quadratic", 0.032f);


        shader.uniform("object_material.shininess", 32.0f);

        vertex_array.bind();

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model(1);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.uniform("model", model);

            glm::mat4 mvp = camera.model_view_projection(model);
            shader.uniform("model_view_projection", mvp);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shader.uniform("model", model);

        mvp = camera.model_view_projection(model);
        shader.uniform("model_view_projection", mvp);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        texture_ground.uniform(shader, "object_material.diffuse", 0);
        texture_ground.uniform(shader, "object_material.specular", 1);

        model = glm::mat4(1);
        model = glm::translate(model, floor_position);
        model = glm::scale(model, glm::vec3(100.0f));
        shader.uniform("model", model);

        mvp = camera.model_view_projection(model);
        shader.uniform("model_view_projection", mvp);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        game_window.swap_buffers();

        game_window.poll_events();
    }
}
