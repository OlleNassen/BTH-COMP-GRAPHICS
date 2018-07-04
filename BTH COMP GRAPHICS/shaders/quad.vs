#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexture;
layout(location = 2) in vec3 aColor;


out vec2 texture;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texture = aTexture;
	color = aColor;

	gl_Position = projection * view * model * vec4(position, 1);


}
