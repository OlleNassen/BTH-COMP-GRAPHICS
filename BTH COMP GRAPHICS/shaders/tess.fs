#version 430

in vec3 fs_world_position;
in vec2 fs_texcoord;

out vec4 frag_color;

uniform sampler2D texture;


void main()
{
	//frag_color = texture(texture, fs_texcoord);
	frag_color = vec4(1,0,0,1);

}