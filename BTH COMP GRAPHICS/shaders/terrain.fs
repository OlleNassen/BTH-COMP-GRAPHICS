#version 330 core

in vec3 normal;
in vec2 texture;

uniform sampler2D diffuse;

out vec4 fragment_color;

void main()
{
	vec3 normalized_normal = normalize(normal);
	float slope = 1.0 - normalized_normal.y;

	fragment_color = texture2D(diffuse, texture);

	if (slope >= 0.7)
        fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
