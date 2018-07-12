#version 430

layout(vertices = 4) out;

in vec3 cs_position[];
in vec2 cs_texcoord[];

out vec3 es_position[];
out vec2 es_texcoord[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

#define ID gl_InvocationID

void main()
{
    es_position[ID] = cs_position[ID];
    es_texcoord[ID] = cs_texcoord[ID];

    if (ID == 0)
    {
        gl_TessLevelInner[0] = 2.0;
        gl_TessLevelInner[1] = 2.0;

        gl_TessLevelOuter[0] = 4.0;
        gl_TessLevelOuter[1] = 4.0;
        gl_TessLevelOuter[2] = 4.0;
        gl_TessLevelOuter[3] = 4.0;
    }
}