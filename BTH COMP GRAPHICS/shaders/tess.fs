#version 430

in vec3 geo_face_normal;
in vec3 geo_patch_distance;
in vec3 geo_tri_distance;

out vec4 frag_color;

uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;

uniform vec3 color;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
	/*
    vec3 N = normalize(geo_face_normal);
    vec3 L = LightPosition;
    float df = abs(dot(N, L));
    vec3 color = AmbientMaterial + df * DiffuseMaterial;

    float d1 = min(min(geo_tri_distance.x, geo_tri_distance.y), geo_tri_distance.z);
    float d2 = min(min(geo_patch_distance.x, geo_patch_distance.y), geo_patch_distance.z);
    color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;
	*/
    frag_color = vec4(color, 1.0);
}
