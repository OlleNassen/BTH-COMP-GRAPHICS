#include "game.hpp"
#include <iostream>
#include <chrono>
#include "input.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std::literals::chrono_literals;

constexpr auto timestep = 16ms;

std::ostream& operator<<(std::ostream& os, const glm::mat4& value)
{
    return os
        << value[0][0] << " " << value[1][0] << " " << value[2][0] << " " << value[3][0] << std::endl
        << value[0][1] << " " << value[1][1] << " " << value[2][1] << " " << value[3][1] << std::endl
        << value[0][2] << " " << value[1][2] << " " << value[2][2] << " " << value[3][2] << std::endl
        << value[0][3] << " " << value[1][3] << " " << value[2][3] << " " << value[3][3] << std::endl;
}

game::game()
	: game_window(WIDTH, HEIGHT, "VOILA")
	, basic_shader("shaders/basic.vs", "shaders/basic.fs")
	, skybox_shader("shaders/skybox.vs", "shaders/skybox.fs")
	, phong_shader("shaders/phong.vs", "shaders/phong.fs")
	, normal_shader("shaders/normal.vs", "shaders/normal.fs")
	, anim("shaders/anim.vs", "shaders/anim.fs")
	, billboard_shader("shaders/billboard.vs", "shaders/billboard.fs")
	, terrain_shader("shaders/terrain.vs", "shaders/terrain.fs")
	, environment_shader("shaders/environment_mapping.vs", "shaders/environment_mapping.fs")
	, tess_shader("shaders/tess.vs", "shaders/tess.cs", "shaders/tess.es", "shaders/tess.geo", "shaders/tess.fs")
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
	scene.attach_child(new quad(20, 20, 20));

	quad1 = new quad(0, 5, -20);
	quad2 = new normal_quad(50, 5, -20);
	quad1->update(16ms);
	quad1->set_texture("images/brickwall.jpg");
	quad2->set_texture("images/brickwall.jpg");

	particles = new particle_emitter(75, 35,75);
	terror = new terrain(10, 10, 10);

	temp = new temp_box(0, 10, 0);

	environment = new temp_box(10, 2, 0);

	tess = new quad_tess();

	ico = new icosahedron();

	light_pos = glm::vec3(50, 5, -15);
    phong_pos = glm::vec3(0, 10, 5);

    for(auto& checkpoint : current_race)
    {
		static float test = 0.f;
        checkpoint =
        sphere(glm::vec3(test,
        10.0f, 0.0f), 2.5f);
		test += 5;
    }

	for (int i = 0; i < 10; ++i)
	{
		icos[i] = new icosahedron(current_race[i].position.x, current_race[i].position.y, current_race[i].position.z);
	}

	//factory.load_mesh("models/banner.obj");
}

game::~game()
{
}

void game::run()
{
	using clock = std::chrono::steady_clock;
    auto last_time = clock::now();
    auto delta_time = 0ns;

	while (game_window.is_open())
	{
		delta_time += clock::now() - last_time;
        last_time = clock::now();

        if(delta_time > timestep)
        {
            delta_time = 0ns;
            update(timestep);
        }

		render();
		game_window.poll_events();
	}
}

void game::render()
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);

	phong_shader.use();
	game_camera.bind(phong_shader);
	phong_shader.uniform("light_color", glm::vec3(1.f, 1.f, 1.f));
	phong_shader.uniform("lightPos", phong_pos);
	temp->render(phong_shader);

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

	environment_shader.use();
	game_camera.bind(environment_shader);
	environment->render(environment_shader);

	tess_shader.use();
	game_camera.bind(tess_shader);
	ico->render(tess_shader);
	for (auto& ics : icos)
	{
		ics->render(tess_shader);
	}

	anim.use();
	game_camera.bind(anim);
	temp_model.draw(anim);

	game_window.swap_buffers();
}

void game::update(const std::chrono::milliseconds delta_time)
{
    terror->update(delta_time);
    game_camera.move_on_terrain(*terror);

    game_camera.update(delta_time);
	glm::vec3 cam_pos{ game_camera.get_view()[3][0], game_camera.get_view()[3][1], game_camera.get_view()[3][2] };
    current_race.update(cam_pos);

    particles->update(delta_time);
    temp_model.update(delta_time);

    seconds += delta_time;
    light_pos.x += glm::sin(seconds.count() * 2.0f);
	light_pos.y += glm::sin(seconds.count() * 0.7f);
	phong_pos.x += sin(seconds.count() * 2.0f) / 10.0f;
}
