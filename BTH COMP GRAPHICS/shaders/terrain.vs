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
	vec4 world_pos = model * vec4(aPos, 1);

	mat4 new_model = view * model * translate(vec4(aPos, 1));

	if(abs(world_pos.x - view[0][3]) < 5 && abs(world_pos.z - view[2][3]) < 5)
	{
		new_model[1][3] = view[1][3];
	}

	gl_Position = projection * new_model;
	texture = aTexture;

	// Hardcoded color values:

	
	color = vec3(0.7,0.7,0.7);

	/*
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
	*/
}