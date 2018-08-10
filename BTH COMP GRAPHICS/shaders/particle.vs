#version 440

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 color;

out vec4 vs_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	float size = 1.0;

	vec4 view_vec = inverse(view) * vec4(vertex_position, 0.0, 1.0) * size;
	vec4 vertex_world = vec4(position, 1.0) + view_vec * size;

    vs_color = vec4(color, 1.0);
	gl_Position = projection * view * model * vertex_world;
}
