#version 150

uniform sampler2D texture_sampler_0;

in vec3 light_direction;
in vec3 viewer_direction;
in vec3 fragment_normal;

in vec3 fragment_ambient_intensity;//Amount of reflected ambient light, Ia*Ka
in vec3 fragment_diffuse_intensity;//Id*kd
in vec3 fragment_specular_intensity;//Is*Ks


in vec2 texture_coordinates;

out vec4 fragment_Color;

void main()
{
	float dotProductNL = dot(fragment_normal,light_direction);

	vec3 reflection_direction = normalize(2 * dotProductNL * fragment_normal - light_direction);
	float alfa = 6.0;
	float dotProductRV_alfa = pow(max(dot(reflection_direction,viewer_direction), 0.0),alfa);

	vec3 text = texture2D(texture_sampler_0, texture_coordinates).xyz;
	fragment_Color = vec4(text*fragment_ambient_intensity, 0.0) + vec4(text*fragment_diffuse_intensity*max(dotProductNL,0.0),0.0)  + vec4(text*fragment_specular_intensity*dotProductRV_alfa,0.0);
	//fragment_Color = vec4(text*fragment_diffuse_intensity,0.0);
}