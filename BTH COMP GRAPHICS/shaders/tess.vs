#version 430

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_texcoord;


out vec3 cs_position;
out vec2 cs_texcoord;

uniform mat4 model;

void main()
{
    cs_position = vs_position;
    cs_texcoord = vs_texcoord;
} 