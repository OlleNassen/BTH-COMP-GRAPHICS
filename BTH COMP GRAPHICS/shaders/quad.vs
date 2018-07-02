#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexture;
layout(location = 2) in vec3 aColor;


out vec2 texture;
out vec3 color;

uniform mat4 mvp;

void main()
{
	texture = aTexture;
	color = aColor;

	gl_Position = mvp * vec4(aPos, 1);


}