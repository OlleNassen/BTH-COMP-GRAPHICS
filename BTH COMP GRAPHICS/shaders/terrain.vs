#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexture;

out vec2 texture;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1);
	texture = aTexture;

	// Hardcoded color values:

	color = vec3(0,0,0.3);

	if(aPos.y > 2)
	{
		color = vec3(.3,1,0.3);
	}

	if(aPos.y > 18)
	{
		color = vec3(1,0.3,0.3);
	}

	if(aPos.y < 2)
	{
		color = vec3(0.7,0.7,0.7);
	}

}