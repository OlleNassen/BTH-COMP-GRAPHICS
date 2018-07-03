#version 430 core

in vec2 texture;
in vec3 color;

uniform sampler2D image;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(image, texture);
}