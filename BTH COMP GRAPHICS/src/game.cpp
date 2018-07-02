#include "game.hpp"
#include <iostream>

game::game()
	: game_window(WIDTH, HEIGHT, "VOILA")
	, game_camera(glm::radians(45.0f), WIDTH, HEIGHT, 0.1f, 100.0f)
{
	game_camera.set_window_copy(game_window.glfw_window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error glew init failed" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	load_shaders();

	models.push_back(new model(mesh_type::BOX, &shaders[3]));
	models.push_back(new model(mesh_type::TERRAIN, &shaders[2]));
	models.push_back(new model(mesh_type::QUAD, &shaders[4]));


}

game::~game()
{
}

void game::run()
{
	float delta_time = 0.0f;	// Time between current frame and last frame
	float last_frame = 0.0f; // Time of last frame

	while (game_window.is_open())
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		handle_events();

		handle_input();

		update(delta_time);

		render();
	}
}

void game::render()
{
	for (const auto& model : models)
	{
		model->render(game_camera.get_view(), game_camera.get_projection());
	}
	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	game_camera.update(delta_time);
}

void game::handle_events()
{
	game_window.poll_events();
}

void game::handle_input()
{
}

void game::load_shaders()
{
	shaders.push_back(shader("shaders/basic.vs", "shaders/basic.fs"));
	shaders.push_back(shader("shaders/shadow.vs", "shaders/shadow.fs"));
	shaders.push_back(shader("shaders/terrain.vs", "shaders/terrain.fs"));
	shaders.push_back(shader("shaders/noob.vs", "shaders/noob.fs"));
	shaders.push_back(shader("shaders/quad.vs", "shaders/quad.fs"));

}
