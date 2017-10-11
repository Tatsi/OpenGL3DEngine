#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
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

	mat4 M = transpose(inverse(modelMatrix));
 
	vec3 cameraSpace_vertex_position = (modelViewMatrix * vec4(vertex_position,1)).xyz;

	// To transform the vertex normal to camera space we cant use modelViewMatrix as the model matrix
	// might contain scaling and thus our normal would not be perpendicular to the vertex anymore.
	// If it wouldn't contain scaling we could use modelViewMatrix if we notice that the length
	// might change and would therefore normalize the vector afterwards.
	vec3 cameraSpace_normal = (normalMatrix * vertex_normal);

	vec3 worldSpace_light_position = vec3(10.0, 10.0, 10.0);
	vec3 cameraSpace_light_position = vec4(viewMatrix * vec4(worldSpace_light_position,1)).xyz;
	L = normalize(cameraSpace_light_position - cameraSpace_vertex_position);
	V = -normalize(cameraSpace_vertex_position.xyz); // vector from cameraSpace_position to camera
	
	// TODO
	// This indicates that there is bug. We should not have to negate the normal. Debug by drawing vertice normals and see if theyre correct
	N = -cameraSpace_normal;

	texture_coordinates = texture_coordinate;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex_position, 1.0);
	//gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}