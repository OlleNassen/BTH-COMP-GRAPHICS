#version 440

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 fragment_position[];
in vec3 normal[];
in vec2 texture_coordinate[];

out vec3 fragment_pos;
out vec3 norm;
out vec2 texture_coord;

void main()
{
	vec3 uno = fragment_position[0] - fragment_position[1];
	vec3 dos = fragment_position[0] - fragment_position[2];

	vec3 cross_product = cross(uno, dos);

	vec4 calc = view * vec4(cross_product, 1);

	cross_product = calc.xyz;

	vec4 position = view * vec4(fragment_position[0], 1);

	if(dot(cross_product, -position.xyz) > 0.0)
	{
		for(int i = 0;i < 3;i++)
	    {
			texture_coord = texture_coordinate[i];
			norm = normal[i];
			fragment_pos = fragment_position[i];
			gl_Position = gl_in[i].gl_Position;
			EmitVertex();
	    }
	    EndPrimitive();
	}

}