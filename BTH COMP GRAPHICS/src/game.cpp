#include "game.hpp"
#include <iostream>
#include "input.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& os, const glm::mat4& value)
{
    return os
        << value[0][0] << " " << value[1][0] << " " << value[2][0] << " " << value[3][0] << std::endl
        << value[1][1] << " " << value[1][1] << " " << value[2][1] << " " << value[3][1] << std::endl
        << value[2][2] << " " << value[1][2] << " " << value[2][2] << " " << value[3][2] << std::endl
        << value[3][3] << " " << value[1][3] << " " << value[2][3] << " " << value[3][3] << std::endl;
}

game::game()
	: game_window(WIDTH, HEIGHT, "VOILA")
	, basic_shader("shaders/basic.vs", "shaders/basic.fs")
	, shadow_shader("shaders/shadow.vs", "shaders/shadow.fs")
	, skybox_shader("shaders/skybox.vs", "shaders/skybox.fs")
	, phong_shader("shaders/phong.vs", "shaders/phong.fs")
	, normal_shader("shaders/normal.vs", "shaders/normal.fs")
	, anim("shaders/anim.vs", "shaders/anim.fs")
	, billboard_shader("shaders/billboard.vs", "shaders/billboard.fs")
	, terrain_shader("shaders/terrain.vs", "shaders/terrain.fs")
	, game_camera(glm::radians(45.0f), WIDTH, HEIGHT, 0.1f, 10000.0f)
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

	scene.attach_child(new point_light());
	scene.attach_child(new box(20, 30, 40));
	//scene_node* terr_temp = new terrain;
	//terr_temp->attach_child(new particle_emitter); //Add this, fix shader problems
	//scene.attach_child(new terrain(10, 10, 10));
	scene.attach_child(new quad(20, 20, 20));

	quad1 = new quad(0, 5, -20);
	quad2 = new normal_quad(50, 5, -20);
	quad1->update(0.0016);
	quad1->set_texture("images/brickwall.jpg");
	//quad2->update(0.0016);
	quad2->set_texture("images/brickwall.jpg");

	particles = new particle_emitter(75, 35,75);
	terror = new terrain(10, 10, 10);

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
	shadow_shader.use();
	light.bind(shadow_shader);
	light.shadows_bind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);

	/*phong_shader.use();
	game_camera.bind(phong_shader);
	phong_shader.uniform("light_color", glm::vec3(1.f, 1.f, 1.f));
	phong_shader.uniform("lightPos", glm::vec3(2, 0, 5));
	temp.render(phong_shader);*/

	basic_shader.use();
	game_camera.bind(basic_shader);
	light.bind(basic_shader);
	scene.render(basic_shader);
	quad1->render(basic_shader);

	terrain_shader.use();
	game_camera.bind(terrain_shader);
	terror->render(terrain_shader);

	//Billboard particles
	billboard_shader.use();
	game_camera.bind(billboard_shader);
	particles->render(billboard_shader);

	//Normal mapping
	static glm::vec3 light_pos(50, 5, -15);
	light_pos.x += sin(glfwGetTime() * 2.0f);
	light_pos.y += sin(glfwGetTime() * 0.7f);
	//std::cout << light_pos.x << " " << light_pos.y << '\n';
	normal_shader.use();
	game_camera.bind(normal_shader);
	normal_shader.uniform("lightPos", light_pos);
	quad2->render(normal_shader);

	skybox_shader.use();
	game_camera.bind(skybox_shader);
	skybox_shader.uniform("view",
        glm::mat4(
        glm::mat3(game_camera.get_view())));

	sky.render(skybox_shader);

	anim.use();
	game_camera.bind(anim);
	temp_model.draw(anim);

	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	game_camera.update(delta_time);
	particles->update(delta_time);
	temp_model.update(delta_time);
}
