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

class game
{
public:
	game();
	~game();

	void run();
private:
	static const unsigned int WIDTH = 1280;
	static const unsigned int HEIGHT = 720;

	window game_window;
	shader basic_shader;
	shader shadow_shader;
	shader skybox_shader;
	shader phong_shader;
	shader normal_shader;
	shader anim;
	shader billboard_shader;
	camera game_camera;
	directional_light light;
	scene_node scene;
	skybox sky;

	temp_box temp; // PHONG cube

	quad* quad1; // phong
	normal_quad* quad2; // normal mapping

	particle_emitter* particles;

	void render();
	void update(float delta_time);
};

#endif
