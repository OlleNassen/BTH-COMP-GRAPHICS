#version 430

layout(vertices = 3) out;
in vec3 position[];
out vec3 cs_position[];
uniform float inner_level;
uniform float outer_level;
uniform float tessellation;


#define ID gl_InvocationID

void main()
{
    cs_position[ID] = position[ID];
    if (ID == 0)
    {
        gl_TessLevelInner[0] = tessellation;

        gl_TessLevelOuter[0] = tessellation * 2;
        gl_TessLevelOuter[1] = tessellation * 2;
        gl_TessLevelOuter[2] = tessellation * 2;
    }
}