#version 430 core

in vec2 texture;
in vec3 color;

out vec4 fragColor;

void main()
{
	fragColor = vec4(color, 1);
}