#include "game.hpp"
#include <iostream>
#include "input.hpp"

game::game()
	: game_window(WIDTH, HEIGHT, "VOILA")
	, basic("shaders/basic.vs", "shaders/basic.fs")
	, shadow("shaders/shadow.vs", "shaders/shadow.fs")
	, terrain("shaders/terrain.vs", "shaders/terrain.fs")
	, noob("shaders/noob.vs", "shaders/noob.fs")
	, quad("shaders/quad.vs", "shaders/quad.fs")
	, skybox("shaders/skybox.vs", "shaders/skybox.fs")
	, game_camera(glm::radians(45.0f), WIDTH, HEIGHT, 0.1f, 200.0f)
	, light(glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f))
{
	input::assign_window(game_window);

	input::assign_mouse_callback(
        std::bind(
        &camera::on_mouse_moved,
        &game_camera,
        std::placeholders::_1,
        std::placeholders::_2));

    input::bind_key("up", input::key::W);
    input::bind_key("down", input::key::S);
    input::bind_key("left", input::key::A);
    input::bind_key("right", input::key::D);
    input::bind_key("shift", input::key::Q);
    input::bind_key("escape", input::key::ESCAPE);

    input::assign_key_callback("escape",
        std::bind(&window::on_escape, &game_window),
        nullptr);

    input::assign_key_callback("up",
        std::bind(&camera::up_pressed, &game_camera),
        std::bind(&camera::up_released, &game_camera));

    input::assign_key_callback("down",
        std::bind(&camera::down_pressed, &game_camera),
        std::bind(&camera::down_released, &game_camera));

    input::assign_key_callback("left",
        std::bind(&camera::left_pressed, &game_camera),
        std::bind(&camera::left_released, &game_camera));

    input::assign_key_callback("right",
        std::bind(&camera::right_pressed, &game_camera),
        std::bind(&camera::right_released, &game_camera));

    input::assign_key_callback("shift",
        std::bind(&camera::fast_pressed, &game_camera),
        std::bind(&camera::fast_released, &game_camera));

	models.push_back(new model(mesh_type::BOX, &basic));
	models.back()->set_texture(texture_type::CONTAINER_BOX);

	models.push_back(new model(mesh_type::TERRAIN, &terrain));

	models.push_back(new model(mesh_type::QUAD, &quad));
	models.back()->set_texture(texture_type::EDVARD_QUAD);
	//Phong test
	models.push_back(new model(mesh_type::BOX, &noob));
	models.back()->set_position(glm::vec3(0, 50, 10));
	//Skybox
	models.push_back(new model(mesh_type::SKYBOX, &skybox));
	models.back()->set_texture(texture_type::JUNGLE_SKYBOX);

}

game::~game()
{
}

void game::run()
{
	float delta_time = 0.0f;
	float last_frame = 0.0f;

	while (game_window.is_open())
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		update(delta_time);

		render();

		game_window.poll_events();
	}
}

void game::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1024, 1024);
	shadow.use();
	light.bind(shadow);
	light.shadows_bind();

	for (const auto& model : models)
	{
		//model->render(game_camera.get_view(), game_camera.get_projection());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);

	basic.use();
	game_camera.bind(basic);
	light.bind(basic);

	for (const auto& model : models)
	{
		model->render(game_camera.get_view(), game_camera.get_projection());
	}

	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	float temp_speed = 5.f;
	//models[1]->move(glm::vec3(0, -(temp_speed * delta_time), 0));

	/*if (glfwGetKey(game_window.glfw_window, GLFW_KEY_ENTER) == GLFW_PRESS)
		models[1]->set_position(glm::vec3(0, 0, 0));*/

	game_camera.update(delta_time);
}
