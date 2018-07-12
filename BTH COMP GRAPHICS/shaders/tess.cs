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
        gl_TessLevelInner[0] = 1.0;
        gl_TessLevelInner[1] = 1.0;
                               
        gl_TessLevelOuter[0] = 1.0;
        gl_TessLevelOuter[1] = 1.0;
        gl_TessLevelOuter[2] = 1.0;
        gl_TessLevelOuter[3] = 1.0;
    }

    //gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}