#version 330 core

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

//in vec3 vertex_position;
//in vec3 vertex_normal;
//in vec2 texture_coordinate;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coordinate;
layout(location = 2) in vec3 vertex_normal;

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
	mat4 M = transpose(inverse(modelMatrix));
    //vec3 world_position = (M * vec4(vertex_position,1)).xyz;
	//vec3 world_normal = normalize((M * vec4(vertex_normal,0)).xyz);
	vec3 world_position = (modelViewMatrix * vec4(vertex_position,1)).xyz;
	vec3 world_normal = normalize((modelViewMatrix * vec4(vertex_normal,0)).xyz);

	vec3 light_position = vec3(10.0, 10.0, 10.0);
	vec3 light_position_world = vec4(modelViewMatrix * vec4(light_position,1)).xyz;
	L = -normalize(light_position_world - world_position);
	//V = normalize(viewing_point - world_position);
	V = (modelViewMatrix * vec4(vertex_position,1)).xyz;
	N = world_normal;

	texture_coordinates = texture_coordinate;

	//gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex_position, 1.0);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}