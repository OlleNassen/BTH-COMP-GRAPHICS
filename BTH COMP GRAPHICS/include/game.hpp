#ifndef GAME_HPP
#define GAME_HPP
#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "directional_light.hpp"
#include "scene_node.hpp"
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
#include "mesh_factory.hpp"

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
	camera game_camera;
	directional_light light;
	scene_node scene;
	skybox sky;

	//mesh_factory factory;

	model temp_model;

	//TEMP VARIABLES, PUT IN SCENE GRAPH WHEN FIXED
	temp_box* temp; // PHONG cube
	temp_box* environment; // ENVIRONMENT CUBE
	quad* quad1; // phong
	normal_quad* quad2; // normal mapping
	particle_emitter* particles;
	terrain* terror;
	quad_tess* tess;
	icosahedron* ico;

	

	glm::vec3 light_pos;
	glm::vec3 phong_pos;
	std::chrono::duration<float> seconds;

	void render();
	void update(const std::chrono::milliseconds delta_time);
};

#endif
