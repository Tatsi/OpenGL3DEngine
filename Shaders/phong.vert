#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec3 viewing_point;

uniform vec3 ia; //Ambient lights intensity/color
uniform vec3 ka; //Ambient lights reflection constant

uniform vec3 id; //Diffuse lights intensity/color
uniform vec3 kd; //Diffuse lights reflection constant

uniform vec3 is; //Specular lights intensity/color
uniform vec3 ks; //Specular lights reflection constant

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 texture_coordinate;

out vec3 L; // unit vector from vertex to light position, L
out vec3 V; // Unit vector from vertex to viewer, V
out vec3 N; // Normal

out vec3 fragment_ambient_intensity; // Ia*Ka
out vec3 fragment_diffuse_intensity; // Id*kd
out vec3 fragment_specular_intensity; // Is*ks

out vec2 texture_coordinates;

void main()
{	
	fragment_ambient_intensity = ia * ka; // Ambient intensity
	fragment_diffuse_intensity = id * kd; // Diffuse intensity
	fragment_specular_intensity = is * ks; // Specular intensity

	// Calculate parameters for diffuse light
    vec3 world_position = (modelMatrix * vec4(vertex_position,1.0)).xyz;
	vec3 world_normal = normalize((modelMatrix * vec4(vertex_normal, 0.0)).xyz);

	vec3 light_position = vec3(0.0, 500.0, 0.0);
	//L = normalize(light_position - world_position);
	L = world_normal;
	//L = vec3(0,100,0);
	V = normalize(viewing_point - world_position);
	N = world_normal;

	texture_coordinates = texture_coordinate;

	//gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex_position, 1.0);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}