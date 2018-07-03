#ifndef GAME_HPP
#define GAME_HPP
#include "model.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "directional_light.hpp"

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
	shader basic;
	shader shadow;
	shader terrain;
	shader noob;
	shader quad;
	camera game_camera;
	directional_light light;
	std::vector<model*> models;

	void render();
	void update(float delta_time);
};

#endif
