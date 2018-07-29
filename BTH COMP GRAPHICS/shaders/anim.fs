#version 440 core

in vec3 normal_out;

in vec4 we;

out vec4 fragment_color;

void main()
{
    fragment_color = vec4(we.xyz,1.0);
}

