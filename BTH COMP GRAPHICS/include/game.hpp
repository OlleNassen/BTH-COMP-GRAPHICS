#ifndef GAME_HPP
#define GAME_HPP
#include "model.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"

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
	camera game_camera;
	std::vector<model*> models;
	std::vector<shader*> shaders;

	void render();
	void update(float delta_time);
	void handle_events();
	void handle_input();

	void load_shaders();
};

#endif
