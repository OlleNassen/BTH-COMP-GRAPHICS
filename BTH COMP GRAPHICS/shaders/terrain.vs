#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_in;

out vec3 normal;
out vec2 texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    normal = normal_in;
    texture = texture_in;
	gl_Position = projection * view * model * vec4(position, 1.0);
}
