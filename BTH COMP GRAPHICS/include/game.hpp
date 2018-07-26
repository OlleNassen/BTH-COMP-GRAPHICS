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

class game
{
public:
	game();
	~game();

	void run();
private:
	static constexpr auto WIDTH = 1280u;
	static constexpr auto HEIGHT = 720u;

	window game_window;
	shader basic_shader;
	shader skybox_shader;
	shader phong_shader;
	shader normal_shader;
	shader anim;
	shader billboard_shader;
	shader terrain_shader;
	shader environment_shader;
	shader tess_shader;
	shader text_shader;

	camera game_camera;
	directional_light light;
	scene::node scene;
	scene::skybox sky;

	//mesh_factory factory;

	anim::model temp_model;

	//TEMP VARIABLES, PUT IN SCENE GRAPH WHEN FIXED
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
	std::chrono::duration<float> seconds;
	std::chrono::milliseconds color_timer;

	void render();
	void update(std::chrono::milliseconds delta_time);
};

#endif
