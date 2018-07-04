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
	shader terrain_shader;
	shader noob_shader;
	shader quad_shader;
	shader skybox_shader;
	camera game_camera;
	directional_light light;
	scene_node scene;
	skybox sky;

	void render();
	void update(float delta_time);
};

#endif
