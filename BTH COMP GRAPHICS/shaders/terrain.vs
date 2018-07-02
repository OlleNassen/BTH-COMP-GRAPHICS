#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexture;

out vec2 texture;
out vec3 color;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1);
	texture = aTexture;

	// Hardcoded color values:

	color = vec3(0,0,0.3);

	if(aPos.y > 2)
	{
		color = vec3(0,0.3,0);
	}

	if(aPos.y > 18)
	{
		color = vec3(0.3,0,0);
	}

}