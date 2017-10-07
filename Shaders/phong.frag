#version 150

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
	float LdotN = max(0, dot(L, N));
	vec3 diffuse = LdotN * vec3(1.0,1.0,1.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	if(LdotN > 0.0)
	{
		vec3 R = -normalize(reflect(L, N));
		specular = pow(max(0, dot(R, V)), 3.0) * fragment_specular_intensity;
	}
	//vec4 ambi = vec4(fragment_ambient_intensity * text, 0.0);
	//vec4 spec = vec4(fragment_specular_intensity * dotProductRV_alfa * text, 0.0);

	vec3 text = texture2D(texture_sampler_0, texture_coordinates).xyz;

	fragment_Color = vec4(diffuse, 1.0); // Only diff now
	//fragment_Color = vec4(text, 1.0); // Only diff now
}