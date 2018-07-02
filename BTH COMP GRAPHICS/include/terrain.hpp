#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <stb_image.h>

class terrain
{
public:
	terrain();
	~terrain();
	void draw();
private:
	unsigned int vao;
	unsigned int buffer_object;
	unsigned int ebo;

	unsigned int draw_count;
};

#endif