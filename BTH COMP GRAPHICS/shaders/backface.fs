#version 440 core

in vec3 fragment_pos;
in vec3 norm;
in vec2 texture_coord;

out vec4 frag_color;

float edgeFactor()
{
    vec3 d = fwidth(fragment_pos);
    vec3 a3 = smoothstep(vec3(0.0), d*1.5, fragment_pos);
    return min(min(a3.x, a3.y), a3.z);
}

void main()
{
    frag_color = vec4(vec3(1,0,0), 1.0);

	float frag_val = fragment_pos.x + fragment_pos.y + fragment_pos.z;

	frag_color = vec4(0.0, 0.0, 0.0, (1.0-edgeFactor())*0.95);
}