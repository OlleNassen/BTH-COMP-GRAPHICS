#version 330 core

in vec2 texture;
in vec3 color;

uniform sampler2D diffuse;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(diffuse, texture) * vec4(color, 1);
}