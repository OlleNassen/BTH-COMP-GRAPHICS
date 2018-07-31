#version 440 core

in vec3 fragment_pos;
in vec3 norm;
in vec2 texture_coord;

out vec4 frag_color;

void main()
{

    frag_color = vec4(vec3(1,0,0), 1.0);
}