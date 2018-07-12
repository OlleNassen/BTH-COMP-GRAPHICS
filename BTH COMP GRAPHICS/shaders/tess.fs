#version 430

in vec3 fs_world_position;
in vec2 fs_texcoord;
in vec3 fs_normal;

out vec4 frag_color;


void main()
{
	frag_color = vec4(1,0,0,1);
}