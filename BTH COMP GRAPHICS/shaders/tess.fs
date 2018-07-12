#version 430

in vec3 fs_world_position;
in vec2 fs_texcoord;

out vec4 frag_color;

uniform sampler2D diffuse;


void main()
{
	frag_color = vec4(texture(diffuse, fs_texcoord).rgb, 1);
	//frag_color = vec4(1,0,0,1);

}