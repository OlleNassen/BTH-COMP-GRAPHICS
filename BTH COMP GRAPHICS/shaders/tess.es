#version 430

layout(triangles, equal_spacing, ccw) in;

in vec3 es_world_position[];
in vec2 es_texcoord[];
in vec3 es_normal[];

out vec3 fs_world_position;
out vec2 fs_texcoord;
out vec3 fs_normal;

uniform mat4 view;
uniform mat4 projection;
uniform sampler2D displacement_map;
uniform float disp_factor;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 

void main()
{

	fs_texcoord = interpolate2D(es_texcoord[0], es_texcoord[1], es_texcoord[2]);
    fs_normal = interpolate3D(es_normal[0], es_normal[1], es_normal[2]);
    fs_normal = normalize(fs_normal);
    fs_world_position = interpolate3D(es_world_position[0], es_world_position[1], es_world_position[2]);

	float Displacement = texture(displacement_map, fs_texcoord.xy).x;
    fs_world_position += fs_normal * Displacement * disp_factor;
    gl_Position = projection * view * vec4(fs_world_position, 1.0);
}