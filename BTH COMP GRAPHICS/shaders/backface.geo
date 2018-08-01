#version 440

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 position[];
in vec3 normal[];
in vec2 texture_coordinate[];

out vec3 fragment_pos;
out vec3 norm;
out vec2 texture_coord;

void main()
{
	vec3 uno = normalize(position[1].xyz - position[0].xyz);
	vec3 dos = normalize(position[2].xyz - position[0].xyz);

	vec3 cross_product = cross(uno, dos);

	if(dot(cross_product, -position[0].xyz) > 0.0)
	{
		for(int i = 0;i < 3;i++)
	    {
			texture_coord = texture_coordinate[i];
			norm = normal[i];
			fragment_pos = position[i].xyz;
			gl_Position = projection * position[i];
			EmitVertex();
	    }
	    EndPrimitive();
	}

}
