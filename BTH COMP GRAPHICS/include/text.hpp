#ifndef TEXT_HPP
#define TEXT_HPP
#include <string>
#include <buffer.hpp>
#include "vertex_array.hpp"
#include <map>
#include <glm\glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};


class text
{
private:
	FT_Library library;
	GLuint VAO, VBO;
	std::map <GLchar, Character> Characters;


public:
	text();
	~text();
	void render_text(const std::string& text, float x, float y, float scale);
};

#endif