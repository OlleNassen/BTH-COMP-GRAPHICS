#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 es_position[3];
in vec3 patch_distance[3];

out vec3 geo_face_normal;
out vec3 geo_patch_distance;
out vec3 geo_tri_distance;

uniform mat4 model;
uniform mat4 view;
//uniform mat3 normal_matrix;

void main()
{
	mat4 model_view = view * model;
	mat3 normal_matrix = mat3(model_view[0][0], model_view[1][0], model_view[2][0],
							model_view[0][1], model_view[1][1], model_view[2][1],
							model_view[0][2], model_view[1][2], model_view[2][2]);

    vec3 A = es_position[2] - es_position[0];
    vec3 B = es_position[1] - es_position[0];
    geo_face_normal = normal_matrix * normalize(cross(A, B));
    
    geo_patch_distance = patch_distance[0];
    geo_tri_distance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position;
	EmitVertex();

    geo_patch_distance = patch_distance[1];
    geo_tri_distance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position;
	EmitVertex();

    geo_patch_distance = patch_distance[2];
    geo_tri_distance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position;
	EmitVertex();

    EndPrimitive();
}
