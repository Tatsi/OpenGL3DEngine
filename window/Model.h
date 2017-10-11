#ifndef MODEL_H
#define MODEL_H



struct glsl_parameter_locations {
	GLint location_coordinates; // Vertex coordinates
    GLint location_normals; // Vertex normals
	GLint location_texture_coordinates;
	GLint location_texture_sampler_0;
	GLint location_projection_matrix; // Projection matrix
	GLint location_modelview_matrix; // Modelview matrix
	GLint location_normal_matrix; // Modelview matrix
	GLint location_model_matrix; // Modelview matrix
	GLint location_view_matrix; // View matrix
	GLint location_viewing_point; // Player "eye" position
	GLint location_light_position; // Absolute position of light
	GLint location_ambient_light_intensity; // Intensity/color of ambient light
	GLint location_ambient_light_reflection_constant; // reflection constant of ambient light
	GLint location_diffuse_light_intensity; // Intensity/color of diffuse light
	GLint location_diffuse_light_reflection_constant;// reflection constant of diffuse light
	GLint location_specular_light_intensity; // Intensity/color of specular light
	GLint location_specular_light_reflection_constant;// reflection constant of specular light
};

class Model
{
public:
	// Constructor:
	// name = name of the model
	Model(std::string name, std::string vertex_shader_file_name, std::string fragment_shader_file_name) : name(name), 
		vertexShaderFileName(vertex_shader_file_name), fragmentShaderFileName(fragment_shader_file_name) { }

	model_data load(); // return value is only used with terrains models

	void initShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
	void unload();
	void clearModelData(); // Clears the model_data that was used in GPU initialization and by terrain class if this is a terrain model.  
	model_data getModelData();

	std::string getName() const;
	GLuint getFaceCount() const;
	GLuint getVertexCount() const;
	GLuint getVaoID() const;
	GLuint getVertexVboID() const;
	GLuint getTextureCoordinatesVBOID();
	GLuint getIndiceIboID() const;
	GLuint getNormalVboID() const;
	GLuint getTextureID() const;
	glsl_parameter_locations getGlslParameterLocations() const;
	GLuint getShaderProgram() const;

	// Get material properties
	vec3 getAmbientLightReflectionConstant() const;
	vec3 getDiffuseLightReflectionConstant() const;
	vec3 getSpecularLightReflectionConstant() const;

	vec3 getBBMin() const;
	vec3 getBBMax() const;
private:
	std::string name;
	std::string vertexShaderFileName;
	std::string fragmentShaderFileName;

	// int modelNumber;
	Bitmap bitmap;// Bitmap file for texture

	GLuint faceCount;
	GLuint vertexCount;

	GLuint vaoID;
	GLuint vertexVBOID;
	GLuint indiceIBOID;
	GLuint normalVBOID;
	GLuint textureID;
	GLuint texcordVBOID;

	model_data data; // These are temporary model data
	GLfloat* vertexData;// These are cleared after models are loaded and terrain is initialized,
	ushort* faceData;// These are also set to nul

	vec3 BB_min; // Min point of bounding box
	vec3 BB_max; // Max point of bounding box

	// Model material properties
	GLfloat ambient_light_reflection_constant[3];
	GLfloat diffuse_light_reflection_constant[3];
	GLfloat specular_light_reflection_constant[3];

	//##############################################
	// Variables for storing shaders
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	//##############################################
	// Variables for storing GLSL parameter locations
	glsl_parameter_locations glsl_locations;
};

#endif

