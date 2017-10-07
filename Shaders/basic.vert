#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

uniform vec3 viewing_point;

uniform vec3 amb_light_inte; //Ambient lights intensity/color
uniform vec3 amb_light_refl; //Ambient lights reflection constant

uniform vec3 dif_light_inte; //Diffuse lights intensity/color
uniform vec3 dif_light_refl; //Diffuse lights reflection constant

uniform vec3 spe_light_inte; //Specular lights intensity/color
uniform vec3 spe_light_refl; //Specular lights reflection constant

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 texture_coordinate;

out vec3 light_direction;//unit vector from vertex to light position
out vec3 viewer_direction;//Unit vector from vertex to viewer


out vec3 fragment_normal;

out vec3 fragment_ambient_intensity; // Amount of reflected ambient light, Ia * Ka
out vec3 fragment_diffuse_intensity; // Id * kd
out vec3 fragment_specular_intensity; // Is * ks

out vec2 texture_coordinates;

void main()
{	
	// Reflected ambient light
	fragment_ambient_intensity = amb_light_inte * amb_light_refl; 

	// Calculate parameters for diffuse light
    vec3 vertex_absolute_position = (modelViewMatrix * vec4(vertex_position, 1.0)).xyz;
	vec3 camera_absolute_position = (modelViewMatrix * vec4(viewing_point, 1.0)).xyz;
	
	//light direction has currently been hard coded to come almost straight from the sky
	//The vector points towards the light source aka. sky
	light_direction = normalize((modelViewMatrix * vec4(0.0,1.0,0.0,0.0)).xyz);


	fragment_normal = normalize((modelViewMatrix * vec4(vertex_normal, 0.0)).xyz);
	fragment_diffuse_intensity = dif_light_inte * dif_light_refl; // Id * kd

	// Calculate parameters for specular light
	fragment_specular_intensity = spe_light_inte * spe_light_refl;

	viewer_direction = normalize(camera_absolute_position - vertex_absolute_position);

	texture_coordinates = texture_coordinate;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex_position, 1.0);  
}