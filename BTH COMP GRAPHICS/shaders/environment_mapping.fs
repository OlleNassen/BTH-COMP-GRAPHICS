#version 430
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 view_position; // World cordinates
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - view_position);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}