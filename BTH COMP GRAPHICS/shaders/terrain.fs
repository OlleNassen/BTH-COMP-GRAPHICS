#version 330 core

in vec3 normal;
in vec2 texture;

uniform sampler2D diffuse;

out vec4 fragment_color;

void main()
{
	vec3 slope = vec3(1.0) - normal;

	fragment_color = texture2D(diffuse, texture);

	if (slope.y < 0.3)
        fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
