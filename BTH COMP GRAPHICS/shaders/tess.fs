#version 430

in vec3 fs_world_position;
in vec2 fs_texcoord;
in vec3 fs_normal;

out vec4 frag_color;

uniform sampler2D displacement_map;


void main()
{
	frag_color = texture(displacement_map, fs_texcoord);
}