#version 430

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 obj_color;
uniform vec3 light_color;

uniform vec3 lightPos;
uniform vec3 view_position; // World cordinates

void main()
{
	vec3 normNormal = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	vec3 viewDir = normalize(view_position - fragPos);
	vec3 reflectionDir = reflect(-lightDir, normNormal); //Change direction

	float specularStrength = 0.5;
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
	vec3 specular = light_color * spec * specularStrength;

	float diff = max(dot(normNormal, lightDir), 0);
	vec3 diffuse = diff * light_color;

	float ambientStrength = 0.1;
	vec3 ambient = light_color * ambientStrength;

	vec3 result = (ambient + diffuse + specular) * obj_color;

	fragColor = vec4(result, 1);
}