#ifndef GAME_HPP
#define GAME_HPP

#include <GL/glew.h>
#include "window.hpp"
#include "camera.hpp"


class game
{
public:
	game();
	~game();

	void run();

private:
	void draw(shader& s);

	static const unsigned int WIDTH = 1280;
	static const unsigned int HEIGHT = 720;

	window game_window;

	camera camera;
};

#endif // GAME_HPP