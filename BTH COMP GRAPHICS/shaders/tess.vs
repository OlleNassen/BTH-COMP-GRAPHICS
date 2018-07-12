#version 430

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec2 vs_texcoord;
layout (location = 2) in vec3 vs_normal;


out vec3 cs_world_position;
out vec2 cs_texcoord;
out vec3 cs_normal;

uniform mat4 model;

void main()
{
    cs_world_position = (model * vec4(vs_position, 1.0)).xyz;
    cs_texcoord = vs_texcoord;
    cs_normal = (model * vec4(vs_normal, 0.0)).xyz;
} 