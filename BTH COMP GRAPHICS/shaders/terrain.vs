#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexture;

out vec2 texture;
out vec3 color;

uniform mat4 model_view_projection;

void main()
{
	gl_Position = model_view_projection * vec4(position, 1);
	texture = aTexture;

	// Hardcoded color values:

	color = vec3(0,0,0.3);

	if(position.y > 2)
	{
		color = vec3(0,0.3,0);
	}

	if(position.y > 18)
	{
		color = vec3(0.3,0,0);
	}

}
