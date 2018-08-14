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
//uniform mat3 NormalMatrix;

void main()
{
	mat4 modelView = view * model;
	mat3 NormalMatrix = mat3(modelView[0][0], modelView[1][0], modelView[2][0],
							modelView[0][1], modelView[1][1], modelView[2][1],
							modelView[0][2], modelView[1][2], modelView[2][2]);

    vec3 A = es_position[2] - es_position[0];
    vec3 B = es_position[1] - es_position[0];
    geo_face_normal = NormalMatrix * normalize(cross(A, B));
    
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
