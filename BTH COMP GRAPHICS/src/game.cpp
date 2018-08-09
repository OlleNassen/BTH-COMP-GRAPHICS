#include "game.hpp"
#include <iostream>
#include <chrono>
#include "input.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& os, const glm::mat4& value)
{
    return os
        << value[0][0] << " " << value[1][0] << " " << value[2][0] << " " << value[3][0] << std::endl
        << value[0][1] << " " << value[1][1] << " " << value[2][1] << " " << value[3][1] << std::endl
        << value[0][2] << " " << value[1][2] << " " << value[2][2] << " " << value[3][2] << std::endl
        << value[0][3] << " " << value[1][3] << " " << value[2][3] << " " << value[3][3] << std::endl;
}

game::game()
	: game_camera(glm::radians(45.0f), width, height, 0.1f, 10000.0f)
	, light(glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f))
{
	srand(time(NULL));
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

	scene.attach_child(&pl);
	scene.attach_child(&s_box);
	scene.attach_child(&s_quad);

	quad.set_texture("images/brickwall.jpg");
	normal_quad.set_texture("images/brickwall.jpg");
	light_pos = glm::vec3(50, 5, -15);
    phong_pos = glm::vec3(0, 10, 5);

    terrain.update(16ms);
    for(auto& checkpoint : current_race)
    {
		glm::vec3 v;
		v.x = 10 + rand() % 256;
		v.y = 10;
		v.z = 10 + rand() % 256;

		checkpoint =
            sphere(terrain.calculate_camera_position(v, 2.5f), 2.5f);
    }

	root = new skeletal_node;
	root->add_child(new cube_robot);

	configure_g_buffer();
	configure_lights();

	//Configure for deferred
	deferred_shader.use();
	deferred_shader.uniform("gPosition", 0);
	deferred_shader.uniform("gNormal", 1);
	deferred_shader.uniform("gAlbedoSpec", 2);
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
	glViewport(0, 0, width, height);

	phong_shader.use();
	game_camera.bind(phong_shader);
	phong_shader.uniform("light_color", glm::vec3(1.f, 1.f, 1.f));
	phong_shader.uniform("lightPos", phong_pos);
	temp.render(phong_shader);

	basic_shader.use();
	game_camera.bind(basic_shader);
	light.bind(basic_shader);
	scene.render(basic_shader);
	quad.render(basic_shader);

	for (auto i = root->get_child_iterator_start(); i < root->get_child_iterator_end(); ++i)
	{
		//Should be nothing on this level
		//(*i)->draw(basic_shader);
		for (auto k = (*i)->get_child_iterator_start(); k < (*i)->get_child_iterator_end(); ++k)
		{
			glm::mat4 scale = glm::mat4(1.f);
			scale[0][0] = (*k)->get_model_scale().x;
			scale[1][1] = (*k)->get_model_scale().y;
			scale[2][2] = (*k)->get_model_scale().z;
			glm::mat4 model_matrix = (*k)->get_world_transform() * scale;

			basic_shader.uniform("model", model_matrix);
			(*k)->draw(basic_shader);

			for (auto lol = (*k)->get_child_iterator_start(); lol < (*k)->get_child_iterator_end(); ++lol)
			{
				glm::mat4 scale = glm::mat4(1.f);
				scale[0][0] = (*lol)->get_model_scale().x;
				scale[1][1] = (*lol)->get_model_scale().y;
				scale[2][2] = (*lol)->get_model_scale().z;
				glm::mat4 model_matrix = (*lol)->get_world_transform() * scale;

				basic_shader.uniform("model", model_matrix);
				(*lol)->draw(basic_shader);
			}
		}
	}


	//Backface culling
	backface_shader.use();
	game_camera.bind(backface_shader);
	backface.render(backface_shader);

	terrain_shader.use();
	game_camera.bind(terrain_shader);
	terrain.render(terrain_shader);

	//Billboard particles
	billboard_shader.use();
	game_camera.bind(billboard_shader);
	particles.render(billboard_shader);

	//Normal mapping
	normal_shader.use();
	game_camera.bind(normal_shader);
	normal_shader.uniform("lightPos", light_pos);
	normal_quad.render(normal_shader);

	skybox_shader.use();
	game_camera.bind(skybox_shader);
	skybox_shader.uniform("view",
        glm::mat4(
        glm::mat3(game_camera.get_view())));

	sky.render(skybox_shader);

	environment_shader.use();
	game_camera.bind(environment_shader);
	environment.render(environment_shader);

	tess_shader.use();
	game_camera.bind(tess_shader);
	for (auto& ics : icos)
	{
		ics->render(tess_shader);
	}

	//Animated model
	anim.use();
	game_camera.bind(anim);
	temp_model.draw(anim);

	//Text
	if (current_race.lap() == 1)
	{
		text_shader.use();
		glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
		text_shader.uniform("projection", projection);
		text_shader.uniform("textColor", glm::vec3(1.0f, 0.3f, 0.3f));
        temp_text.render_text("FINISHED", 100, 400, 1);
	}

	game_window.swap_buffers();
}

void game::update(std::chrono::milliseconds delta_time)
{
	glm::vec3 cam_pos = game_camera.get_pos();
	scene.sort(cam_pos);

	root->update(0.016f);

	for (auto& ico : icos)
	{
		ico->set_tessellation(ico->distance_to(cam_pos));
	}

    terrain.update(delta_time);
    game_camera.move_on_terrain(terrain);

    game_camera.update(delta_time);
	current_race.update(game_camera.get_pos());

    particles.update(delta_time);
    temp_model.update(delta_time);

	if (race_index == current_race.get_checkpoint())
	{
		if(race_index != 0)
			icos[race_index - 1]->set_color(glm::vec3(0.1f, 1.0f, 0.1f));
		icos.emplace_back(new scene::icosahedron(
            current_race[race_index].position.x,
            current_race[race_index].position.y,
            current_race[race_index].position.z));
		++race_index;

		if(race_index == 10)
            race_index = 0;
	}

    color_timer += delta_time;
    if (color_timer > 500ms)
	{
        color_timer = 0ms;

		if(current_race.lap() > 0)
        {
            for (auto& obj : icos)
            {
                 obj->set_color(glm::vec3(
                    (rand() % 255) / 255.f,
                    (rand() % 255) / 255.f,
                    (rand() % 255) / 255.f));
            }
		}
	}

    seconds += delta_time;
    light_pos.x += glm::sin(seconds.count() * 2.0f);
	light_pos.y += glm::sin(seconds.count() * 0.7f);
	phong_pos.x += sin(seconds.count() * 2.0f) / 10.0f;
}

void game::configure_g_buffer()
{
	// configure g-buffer framebuffer
	// ------------------------------
	unsigned int gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	unsigned int gPosition, gNormal, gAlbedoSpec;
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void game::configure_lights()
{
	// lighting info
	// -------------
	const unsigned int NR_LIGHTS = 32;
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

}
