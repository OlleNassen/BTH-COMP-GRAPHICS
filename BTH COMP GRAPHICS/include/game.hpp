#ifndef GAME_HPP
#define GAME_HPP
#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "directional_light.hpp"
#include "node.hpp"
#include "box.hpp"
#include "skybox.hpp"
#include "terrain.hpp"
#include "quad.hpp"
#include "point_light.hpp"
#include "temp_box.hpp"
#include "normal_quad.hpp"
#include "particle_emitter.hpp"
#include "model.hpp"
#include "quad_tess.hpp"
#include "icosahedron.hpp"
#include <iostream>
#include <chrono>
#include "race.hpp"
#include "text.hpp"

std::ostream& operator<<(std::ostream& os, const glm::mat4& value);
using namespace std::literals::chrono_literals;

static constexpr auto timestep = 16ms;
static constexpr auto WIDTH = 1280u;
static constexpr auto HEIGHT = 720u;

class game
{
public:
	game();

	void run();

private:
	window game_window{WIDTH, HEIGHT, "VOILA"};
	shader basic_shader{"shaders/basic.vs", "shaders/basic.fs"};
	shader skybox_shader{"shaders/skybox.vs", "shaders/skybox.fs"};
	shader phong_shader{"shaders/phong.vs", "shaders/phong.fs"};
	shader normal_shader{"shaders/normal.vs", "shaders/normal.fs"};
	shader anim{"shaders/anim.vs", "shaders/anim.fs"};
	shader billboard_shader{"shaders/billboard.vs", "shaders/billboard.fs"};
	shader terrain_shader{"shaders/terrain.vs", "shaders/terrain.fs"};
	shader environment_shader{"shaders/environment_mapping.vs", "shaders/environment_mapping.fs"};
	shader text_shader{"shaders/text.vs", "shaders/text.fs"};
	shader tess_shader{"shaders/tess.vs", "shaders/tess.cs",
        "shaders/tess.es", "shaders/tess.geo", "shaders/tess.fs"};


	camera game_camera;
	directional_light light;

	anim::model temp_model;

	scene::node scene;
	scene::skybox sky;
	scene::temp_box* temp; // PHONG cube
	scene::temp_box* environment; // ENVIRONMENT CUBE
	scene::quad* quad; // phong
	scene::normal_quad* normal_quad; // normal mapping
	scene::particle_emitter* particles;
	scene::terrain* terrain;
	scene::quad_tess* quad_tess;
	scene::icosahedron* ico;

	text* temp_text;

	int race_index = 0;
    race current_race;
	std::vector<scene::icosahedron*> icos;

	glm::vec3 light_pos;
	glm::vec3 phong_pos;
	std::chrono::duration<float> seconds = 0s;
	std::chrono::milliseconds color_timer = 0ms;

	void render();
	void update(std::chrono::milliseconds delta_time);
};

#endif
