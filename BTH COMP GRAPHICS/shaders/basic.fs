#version 440 core

struct directional_light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform directional_light light;

struct point_light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 1
uniform point_light point_lights[NR_POINT_LIGHTS];

struct material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform material object_material;

uniform vec3 view_position;

uniform sampler2D shadow_map;

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coordinate;
in vec4 fragment_position_in_light_space;

out vec4 frag_color;


vec3 calculate_directional_light(directional_light light, vec3 normal, vec3 view_direction);
vec3 calculate_point_light(point_light light, vec3 normal, vec3 fragment_position, vec3 view_direction);
float calculate_shadows(vec4 fragment_in_light_space);

void main()
{
    vec3 normalized_normal = normalize(normal);
    vec3 view_direction = normalize(view_position - fragment_position);


    /* directional light */
    vec3 result = calculate_directional_light(light, normalized_normal, view_direction);


    /* point light */
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calculate_point_light(point_lights[i],
            normalized_normal, fragment_position, view_direction);;
    }

    frag_color = vec4(result, 1.0);
}


vec3 calculate_directional_light(directional_light light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);

    /* diffuse */
    float diffuse_value = max(dot(normal, light_direction), 0.0);

    /* specular */
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), object_material.shininess);


    vec3 ambient  = light.ambient  * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 diffuse  = light.diffuse  * diffuse_value * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 specular = light.specular * specular_value * vec3(texture2D(object_material.specular, texture_coordinate));

    float shadow = calculate_shadows(fragment_position_in_light_space);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
    //return (ambient + diffuse + specular);
}


vec3 calculate_point_light(point_light light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position);

    /* diffuse */
    float diffuse_value = max(dot(normal, light_direction), 0.0);

    /* specular */
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_value = pow(max(dot(view_direction, reflect_direction), 0.0), object_material.shininess);

    /* attenuation */
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant +
        light.linear * distance +
        light.quadratic * distance * distance);


    vec3 ambient  = light.ambient  * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 diffuse  = light.diffuse  * diffuse_value * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 specular = light.specular * specular_value * vec3(texture2D(object_material.specular, texture_coordinate));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

float calculate_shadows(vec4 fragment_in_light_space)
{
    vec3 projection_coordinate = fragment_in_light_space.xyz / fragment_in_light_space.w;
    projection_coordinate = projection_coordinate * 0.5 + 0.5;
    float closest_depth = texture2D(shadow_map, projection_coordinate.xy).r;
    float current_depth = projection_coordinate.z;
    float bias = max(0.05 * (1.0 - dot(normal, light.direction)), 0.005);
    float shadow = current_depth - bias > closest_depth  ? 1.0 : 0.0;

    return shadow;
}

