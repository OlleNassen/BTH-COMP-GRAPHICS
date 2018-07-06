#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec2 vertex_offset;

out vec2 vs_position;
out vec2 vs_texcoord;
out vec3 vs_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 ModelView = view * model;

	ModelView[0][0] = 1;
	ModelView[0][1] = 0;
	ModelView[0][2] = 0;

	ModelView[1][0] = 0;
	ModelView[1][1] = 1;
	ModelView[1][2] = 0;

	ModelView[2][0] = 0;
	ModelView[2][1] = 0;
	ModelView[2][2] = 1;
	
	vs_texcoord = vertex_texcoord;
    vs_color = vertex_color;
    gl_Position = projection * ModelView * vec4(vertex_position + vertex_offset, 1.0);
}