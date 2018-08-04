#version 330 core

in vec2 texture;

uniform sampler2D diffuse;

out vec4 fragment_color;

void main()
{
	fragment_color = texture2D(diffuse, texture);
}
