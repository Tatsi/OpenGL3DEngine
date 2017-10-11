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
	float LdotN = max(dot(normalize(L), normalize(N)), 0.0);
	float diffuse = LdotN * fragment_diffuse_intensity[0];
	float specular = 0.0;

	if(LdotN > 0.0)
	{
		vec3 R = normalize(reflect(normalize(-L), normalize(N)));
		specular = fragment_specular_intensity[0] * pow(max(0.0, dot(R, normalize(V))), 3);
		specular = clamp(specular, 0.0, 1.0);
	}

	vec3 tex = texture2D(texture_sampler_0, texture_coordinates).xyz;

	float attenuation_constant = 0.1;
	float light_distance = length(L);
	float attenuation = 1.0 / (1.0 + attenuation_constant * pow(light_distance, 2.0));

	float light = fragment_ambient_intensity[0] + attenuation * (diffuse + specular);

	fragment_Color = vec4(light * tex, 1.0);
}