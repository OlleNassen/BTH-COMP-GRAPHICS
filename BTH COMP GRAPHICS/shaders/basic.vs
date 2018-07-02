#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 texture_coordinate_in;

out vec3 fragment_position;
out vec3 normal;
out vec2 texture_coordinate;
out vec4 fragment_position_in_light_space;


uniform mat4 model;
uniform mat4 model_view_projection;
uniform mat4 light_space_matrix;

void main()
{
    fragment_position = vec3(model * vec4(position, 1.0));
    normal = mat3(transpose(inverse(model))) * normal_in;
    texture_coordinate = texture_coordinate_in;
    fragment_position_in_light_space = light_space_matrix * vec4(fragment_position, 1.0);

    gl_Position =  model_view_projection * vec4(position, 1.0);
}
