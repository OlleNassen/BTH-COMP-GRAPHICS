#version 440

layout(triangles) in;
layout(triangle_strip, max_vertices=6) out; // duplicated to 6

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec2 uv[];
out vec2 frag_uv;

out vec3 normal;

in vec3 fragPos[];
out vec3 fragP;

void main()
{

//Calc triangle normal:

vec3 a = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
vec3 b = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

vec4 tri_normal = vec4(normalize(cross(a,b)), 0);

for(int i=0; i<3; i++)
{
	gl_Position = projection * view * model * gl_in[i].gl_Position;
	//gl_Position = gl_in[i].gl_Position;
	frag_uv = uv[i];
	fragP = vec3(model * gl_in[i].gl_Position);
	vec4 nn = projection * view * model * tri_normal; // Transform old normal
	normal = nn.xyz;
	EmitVertex();
}
EndPrimitive();

for(int i=0; i<3; i++)
{
	vec4 pos = gl_in[i].gl_Position + (tri_normal * 1);
	gl_Position = projection * view * model * pos;
	//gl_Position = gl_in[i].gl_Position + tri_normal;
	frag_uv = uv[i];
	fragP = vec3(model * gl_in[i].gl_Position);
	vec4 nn = projection * view * model * tri_normal; // Transform old normal
	normal = nn.xyz;
	EmitVertex();
}
EndPrimitive();

}