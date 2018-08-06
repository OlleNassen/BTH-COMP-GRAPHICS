#version 330 core

in vec3 normal;
in vec2 texture;

uniform sampler2D diffuse;

out vec4 fragment_color;

void main()
{
	vec4 color1 = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 color2 = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 color3 = vec4(0.0, 0.0, 0.0, 1.0);
	float blend = 0.0;

	vec3 normalized_normal = normalize(normal);
	float slope = 1.0 - normalized_normal.y;

	fragment_color = texture2D(diffuse, texture);

    /*if (slope < 0.2)
    {
        blend = slope / 0.2f;
        fragment_color = mix(color1, color2, blend);
    }

    if (slope < 0.7 && slope >= 0.2f)
    {
        blend = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
        fragment_color = mix(color2, color3, blend);
    }

    if (slope >= 0.7)
    {
        fragment_color = color3;
    }*/
}
