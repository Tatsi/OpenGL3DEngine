#version 330 core

uniform sampler2D texture_sampler_0;

in vec3 L;
in vec3 V;
in vec3 N;

in vec3 fragment_ambient_intensity;//Amount of reflected ambient light, Ia*Ka
in vec3 fragment_diffuse_intensity;//Id*kd
in vec3 fragment_specular_intensity;//Is*Ks

in vec2 texture_coordinates;

out vec4 fragment_Color;

void main()
{
	float LdotN = clamp(dot(normalize(L), normalize(N)), 0, 1);
	float diffuse = LdotN * fragment_diffuse_intensity[0];
	float specular;

	if(LdotN > 0.0)
	{
		vec3 R = -normalize(reflect(normalize(L), normalize(N)));
		specular = fragment_specular_intensity[0] * pow(max(0, dot(R, normalize(V))), 30.1);
		specular = clamp(specular, 0.0, 1.0);
	}

	vec3 text = texture2D(texture_sampler_0, texture_coordinates).xyz;

	float light = fragment_ambient_intensity[0] + diffuse + specular;

	fragment_Color = vec4(light*text, 1.0);
}