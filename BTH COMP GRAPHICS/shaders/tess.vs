#version 430

in vec4 vertex_position;

out vec3 position;

void main()
{
    position = vertex_position.xyz;
}