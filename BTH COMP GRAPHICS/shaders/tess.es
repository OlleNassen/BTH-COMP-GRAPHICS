#version 430

layout(quads, cw) in;

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
	vec3 p0 = mix(es_position[0], es_position[3], gl_TessCoord.x);

    vec3 p1 = mix(es_position[1], es_position[2], gl_TessCoord.x);

    vec3 pos = normalize(mix(p0, p1, gl_TessCoord.y));

	fs_texcoord = interpolate2D(es_texcoord[0], es_texcoord[1], es_texcoord[2]);

    fs_patch_distance = gl_TessCoord.xy;

    gl_Position = projection * view * model * vec4(normalize(pos.xyz), 1);
}