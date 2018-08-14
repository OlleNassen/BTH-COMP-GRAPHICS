#version 430

in vec3 fragment_pos;
in vec3 normal;

out vec4 frag_color;

uniform vec3 obj_color;
uniform vec3 light_color;

uniform vec3 light_pos;
uniform vec3 view_position; // World cordinates

void main()
{
	vec3 normalized_normal = normalize(normal);
	vec3 light_direction = normalize(light_pos - fragment_pos);

	vec3 view_direction = normalize(view_position - fragment_pos);
	vec3 reflection_direction = reflect(-light_direction, normalized_normal); //Change direction

	float specular_value = 0.5;
	float spec = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
	vec3 specular = light_color * spec * specular_value;

	float diff_value = max(dot(normalized_normal, light_direction), 0);
	vec3 diffuse = diff_value * light_color;

	float ambient_value = 0.1;
	vec3 ambient = light_color * ambient_value;

	vec3 result = (ambient + diffuse + specular) * obj_color;

	frag_color = vec4(result, 1);
}
