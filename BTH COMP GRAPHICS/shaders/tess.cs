#version 430

layout(vertices = 3) out;
in vec3 vPosition[];
out vec3 tcPosition[];
uniform float TessLevelInner;
uniform float TessLevelOuter;
uniform float tessellation;


#define ID gl_InvocationID

void main()
{
    tcPosition[ID] = vPosition[ID];
    if (ID == 0)
    {
        gl_TessLevelInner[0] = tessellation;
        gl_TessLevelOuter[0] = tessellation * 2;
        gl_TessLevelOuter[1] = tessellation * 2;
        gl_TessLevelOuter[2] = tessellation * 2;
    }
}