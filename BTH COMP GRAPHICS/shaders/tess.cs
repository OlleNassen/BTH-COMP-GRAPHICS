#version 430

layout(vertices = 3) out;

in vec3 cs_world_position[];
in vec2 cs_texcoord[];
in vec3 cs_normal[];

out vec3 es_world_position[];
out vec2 es_texcoord[];
out vec3 es_normal[];

uniform vec3 view_position; //camera position

float GetTessLevel(float Distance0, float Distance1)
{
    float AvgDistance = (Distance0 + Distance1) / 2.0;

    if (AvgDistance <= 2.0) {
        return 10.0;
    }
    else if (AvgDistance <= 5.0) {
        return 7.0;
    }
    else {
        return 3.0;
    }
} 

void main()
{
    es_texcoord[gl_InvocationID] = cs_texcoord[gl_InvocationID];
    es_normal[gl_InvocationID] = cs_normal[gl_InvocationID];
    es_world_position[gl_InvocationID] = cs_world_position[gl_InvocationID];
	
	
    // Calculate the distance from the camera to the three control points
    float EyeToVertexDistance0 = distance(view_position, es_world_position[0]);
    float EyeToVertexDistance1 = distance(view_position, es_world_position[1]);
    float EyeToVertexDistance2 = distance(view_position, es_world_position[2]);

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
} 