#version 440 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 light_space_matrix;

void main()
{
    gl_Position = light_space_matrix * model * vec4(position, 1.0);
}
