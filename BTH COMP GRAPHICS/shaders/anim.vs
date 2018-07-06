#version 440 core

#define MAX_JOLINTS 50
#define MAX_WEIGHTS 4

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 joint;
layout (location = 4) in vec4 weight;

out vec3 normal_out;

uniform mat4 joint_transforms[MAX_JOLINTS];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 total_local_position = vec4(0.0);
    vec4 total_normal = vec4(0.0);

    for(int i = 0; i < MAX_WEIGHTS; i++)
    {
        vec4 pose_position = joint_transforms[i] * vec4(position, 1.0);
        total_local_position += pose_position * weight[i];

        vec4 world_normal = joint_transforms[i] * vec4(normal, 1.0);
        total_normal += world_normal * weight[i];
    }

    normal_out = total_normal.xyz;
    gl_Position = projection * view * total_local_position;
}
