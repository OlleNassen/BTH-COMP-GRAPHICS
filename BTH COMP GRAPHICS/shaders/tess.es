#version 430

layout(triangles, equal_spacing, ccw) in;

in vec3 es_position[];
in vec2 es_texcoord[];

out vec3 fs_position;
out vec2 fs_texcoord;
out vec2 fs_patch_distance;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

void main()
{
    vec3 p0 = gl_TessCoord.x * es_position[0];
    vec3 p1 = gl_TessCoord.y * es_position[1];
    vec3 p2 = gl_TessCoord.z * es_position[2];

	fs_texcoord = interpolate2D(es_texcoord[0], es_texcoord[1], es_texcoord[2]);

    fs_patch_distance = gl_TessCoord.xy;
    fs_position = normalize(p0 + p1 + p2);
    gl_Position = projection * view * model * vec4(fs_position, 1);
}