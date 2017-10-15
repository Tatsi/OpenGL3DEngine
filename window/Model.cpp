#include "stdafx.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

model_data Model::load()
{
	std::string fileName = "Models/";
	fileName.append(name);
	fileName.append("/");
	fileName.append(name);

	std::pair<GLint, GLint> texture_size;

	std::string textureFileName = fileName;
	textureFileName.append(".raw"); // Texture file name is Models/model_name/model_name.raw

	if (name == "bunny") //Load these from files later
	{

		texture_size.first = 256;
		texture_size.second = 256;
		//Set up material properties
		ambient_light_reflection_constant[0] = 0.4;
		ambient_light_reflection_constant[1] = 0.4;
		ambient_light_reflection_constant[2] = 0.4;
		diffuse_light_reflection_constant[0] = 0.7;
		diffuse_light_reflection_constant[1] = 0.7;
		diffuse_light_reflection_constant[2] = 0.7;
		specular_light_reflection_constant[0] = 0.9;
		specular_light_reflection_constant[1] = 0.9;
		specular_light_reflection_constant[2] = 0.9;
	}
	else if (name == "terrain")
	{
		texture_size.first = 4096;
		texture_size.second = 4096;
		//Set up material properties
		ambient_light_reflection_constant[0] = 0.3;
		ambient_light_reflection_constant[1] = 0.3;
		ambient_light_reflection_constant[2] = 0.3;
		diffuse_light_reflection_constant[0] = 0.9;
		diffuse_light_reflection_constant[1] = 0.9;
		diffuse_light_reflection_constant[2] = 0.9;
		specular_light_reflection_constant[0] = 0.5;
		specular_light_reflection_constant[1] = 0.5;
		specular_light_reflection_constant[2] = 0.5;
	}
	else if (name == "box")
	{
		texture_size.first = 512;
		texture_size.second = 512;
		//Set up material properties
		ambient_light_reflection_constant[0] = 0.3;
		ambient_light_reflection_constant[1] = 0.3;
		ambient_light_reflection_constant[2] = 0.3;
		diffuse_light_reflection_constant[0] = 0.7;
		diffuse_light_reflection_constant[1] = 0.7;
		diffuse_light_reflection_constant[2] = 0.7;
		specular_light_reflection_constant[0] = 0.9;
		specular_light_reflection_constant[1] = 0.9;
		specular_light_reflection_constant[2] = 0.9;
	}
	else if (name == "1box")
	{
		texture_size.first = 1024;
		texture_size.second = 1024;
		// Set up material properties
		ambient_light_reflection_constant[0] = 0.3;
		ambient_light_reflection_constant[1] = 0.3;
		ambient_light_reflection_constant[2] = 0.3;
		diffuse_light_reflection_constant[0] = 0.7;
		diffuse_light_reflection_constant[1] = 0.7;
		diffuse_light_reflection_constant[2] = 0.7;
		specular_light_reflection_constant[0] = 0.9;
		specular_light_reflection_constant[1] = 0.9;
		specular_light_reflection_constant[2] = 0.9;
	}
	else if (name == "knot")
	{
		texture_size.first = 256;
		texture_size.second = 256;
		// Set up material properties
		ambient_light_reflection_constant[0] = 0.4;
		ambient_light_reflection_constant[1] = 0.4;
		ambient_light_reflection_constant[2] = 0.4;
		diffuse_light_reflection_constant[0] = 0.7;
		diffuse_light_reflection_constant[1] = 0.7;
		diffuse_light_reflection_constant[2] = 0.7;
		specular_light_reflection_constant[0] = 1.9;
		specular_light_reflection_constant[1] = 1.9;
		specular_light_reflection_constant[2] = 1.9;
	}
	else if (name == "player")
	{
		texture_size.first = 1024;
		texture_size.second = 1024;
		// Set up material properties
		ambient_light_reflection_constant[0] = 0.4;
		ambient_light_reflection_constant[1] = 0.4;
		ambient_light_reflection_constant[2] = 0.4;
		diffuse_light_reflection_constant[0] = 0.8;
		diffuse_light_reflection_constant[1] = 0.8;
		diffuse_light_reflection_constant[2] = 0.8;
		specular_light_reflection_constant[0] = 1.9;
		specular_light_reflection_constant[1] = 1.9;
		specular_light_reflection_constant[2] = 1.9;
	}

	 // Init shader program
	 initShaderProgram(vertexShaderFileName, fragmentShaderFileName);
    
     // Load model data
	 std::string normalfilename = fileName;
	 normalfilename.append(".normal");
	 
	 // Test if normal data has been calculated
	 //std::fstream stream(normalfilename.c_str(), std::fstream::in|std::fstream::binary);
    
	 //if (!stream.good()) // Normal data has not been calculated. Need to calculate it and store it
     //{
		 //stream.close();
	data = loadAndCreateTexturedModelData(fileName);
	 //}
	 //else // Normal data has been calculated
	 //{
		 // TODO This does not work
		 //stream.close();
		 //data = loadTexturedModelData(fileName);
	 //}
	 
	 faceCount = data.faceCount;
	 vertexCount = data.vertexCount;

	 glEnableClientState(GL_VERTEX_ARRAY);
     
	 glGenVertexArrays(1, &vaoID); // Create Vertex array
     glBindVertexArray(vaoID);	// Bind vertex array

	 // Init vertex buffer
     glGenBuffers(1,  &vertexVBOID);
     glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
     glBufferData(GL_ARRAY_BUFFER, 3*data.vertexCount*sizeof(float), data.vertexData, GL_STATIC_DRAW);
	 
	 //glEnableVertexAttribArray(glsl_locations.location_coordinates); // Bind vertex data to vertex_Position GLSL-param
     //glVertexAttribPointer(glsl_locations.location_coordinates, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set-up pointer
     //glBindBuffer(GL_ARRAY_BUFFER, 0); 
     
	 // Init indice buffer
     glGenBuffers(1, &indiceIBOID);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceIBOID);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*data.faceCount*sizeof(ushort), data.faceData, GL_STATIC_DRAW);
     //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
	 glm::vec3 * vertices;
	 vertices = new glm::vec3[data.vertexCount];
	 glm::vec3 * normals;
	 normals = new glm::vec3[data.vertexCount];

	 for (int j = 0; j < data.vertexCount; j++) {
		 glm::vec3 vertex;
		 vertex.x = data.vertexData[j * 3];
		 vertex.y = data.vertexData[j * 3 + 1];
		 vertex.z = data.vertexData[j * 3 + 2];
		 //vertices.push_back(vertex);
		 vertices[j] = vertex;

		 glm::vec3 normal;
		 normal.x = data.vertexNormalData[j * 3];
		 normal.y = data.vertexNormalData[j * 3 + 1];
		 normal.z = data.vertexNormalData[j * 3 + 2];

		 normals[j] = normal;
	 }


	 // Init normal buffer
     glGenBuffers(1,  &normalVBOID);
     glBindBuffer(GL_ARRAY_BUFFER, normalVBOID);
     glBufferData(GL_ARRAY_BUFFER, 3*data.vertexCount*sizeof(float), data.vertexNormalData, GL_STATIC_DRAW);
	 //glBufferData(GL_ARRAY_BUFFER, data.vertexCount * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	 //glEnableVertexAttribArray(glsl_locations.location_normals); // Bind location data to vertex_Location shader-param
     //glVertexAttribPointer(glsl_locations.location_normals, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set-up pointer
     //glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // Init texture coordinate buffer
	 glGenBuffers(1,  &texcordVBOID);
	 glBindBuffer(GL_ARRAY_BUFFER, texcordVBOID);
     glBufferData(GL_ARRAY_BUFFER, 2*data.vertexCount*sizeof(float), data.textureCoordinates, GL_STATIC_DRAW);
	 //glEnableVertexAttribArray(glsl_locations.location_texture_coordinates); // Bind location data to texture_coordinates shader-param
     //glVertexAttribPointer(glsl_locations.location_texture_coordinates, 2, GL_FLOAT, GL_FALSE, 0, 0); // Set-up pointer
     //glBindBuffer(GL_ARRAY_BUFFER, 0); 
     
	 glBindVertexArray(0); // Unbind vertex array

	 glDisableClientState(GL_VERTEX_ARRAY);

	 // Init texture data
	 Bitmap texture;
	 texture.loadFromRaw(textureFileName, texture_size.first, texture_size.second);

	 glGenTextures(1, &textureID);
	 glBindTexture(GL_TEXTURE_2D, textureID);	 

	 gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture_size.first, texture_size.second, GL_RGB, GL_UNSIGNED_BYTE, texture.getData());
	 glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	 glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	 glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is actually the default value, other option is GL_CLAMP
	 glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // This is actually the default value, other option is GL_CLAMP

	 glBindTexture(GL_TEXTURE_2D, 0);
	 
	 bitmap.unload(); // This may sometimes cause an error

	 //Check for GL errors
	 checkForGLErrors("Model::load()");

	 return data; // The return value is only used when initializing terrain model. Otherwise returned pointers are NULL pointers
}

void Model::initShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
    int compiled; // Temporary storage for compile statuses
    
    // Vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint vertexShaderLength; // Store length of shader text
	char* vertexShaderSource = loadShaderFromFile(vertexShaderFileName, vertexShaderLength);
    const char* vv = vertexShaderSource;
    glShaderSource(vertexShader, 1, &vv, &vertexShaderLength);
    glCompileShader(vertexShader); // Compile vertex shader
    delete vertexShaderSource; // Clear data
    vertexShaderSource = NULL; // To null for safety
    vv = NULL; // Set to null for safety
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);//Check status
    if (!compiled)
    {
		printShaderlog(vertexShader);
        throw std::logic_error("Compiling vertex shader failed.");
    }
    // Fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint fragmentShaderLength;
    char* fragmentShaderSource = loadShaderFromFile(fragmentShaderFileName, fragmentShaderLength);
    const char* ff = fragmentShaderSource;
    glShaderSource(fragmentShader, 1, &ff, &fragmentShaderLength);
    glCompileShader(fragmentShader); // Compile fragment shader
    delete fragmentShaderSource; // Free text from mem
    fragmentShaderSource = NULL; // To null for safety
    ff = NULL; // Set to null, no longer used
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);// Check status
    if (!compiled)
    {
		printShaderlog(fragmentShader);
        throw std::logic_error("Compiling fragment shader failed.");
    }
    // Shader program
    shaderProgram = glCreateProgram(); // Create shader program
    glAttachShader(shaderProgram, vertexShader); // Attach vert shader to program
    glAttachShader(shaderProgram, fragmentShader); // Attach frag shader to program
    glLinkProgram(shaderProgram); // Link
    
	glUseProgram(shaderProgram);

	// Parameter handling
    glsl_locations.location_coordinates = glGetAttribLocation(shaderProgram,"vertex_position");
    glsl_locations.location_normals = glGetAttribLocation(shaderProgram,"vertex_normal");
	glsl_locations.location_texture_coordinates = glGetAttribLocation(shaderProgram,"texture_coordinate");

	// Uniform attributes. These values can only change between objects (glBegin..glEnd)
	glsl_locations.location_projection_matrix = glGetUniformLocation(shaderProgram, "projectionMatrix"); // Projection matrix
	glsl_locations.location_modelview_matrix = glGetUniformLocation(shaderProgram, "modelViewMatrix"); // Modelview matrix
	glsl_locations.location_model_matrix = glGetUniformLocation(shaderProgram, "modelMatrix"); // Model matrix
	glsl_locations.location_normal_matrix = glGetUniformLocation(shaderProgram, "normalMatrix"); // Normal matrix
	glsl_locations.location_view_matrix = glGetUniformLocation(shaderProgram, "viewMatrix"); // View matrix
	glsl_locations.location_viewing_point = glGetUniformLocation(shaderProgram, "viewing_point");
	glsl_locations.location_light_position = glGetUniformLocation(shaderProgram, "light_position"); // Not used
	glsl_locations.location_ambient_light_intensity = glGetUniformLocation(shaderProgram, "ia"); // Intensity/color of ambient light
	glsl_locations.location_ambient_light_reflection_constant = glGetUniformLocation(shaderProgram, "ka");;// reflection constant of ambient light
	glsl_locations.location_diffuse_light_intensity = glGetUniformLocation(shaderProgram, "id"); // Intensity/color of diffuse light
	glsl_locations.location_diffuse_light_reflection_constant = glGetUniformLocation(shaderProgram, "kd");
	glsl_locations.location_specular_light_intensity = glGetUniformLocation(shaderProgram, "is"); // Intensity/color of specular light
	glsl_locations.location_specular_light_reflection_constant = glGetUniformLocation(shaderProgram, "ks");	

	glsl_locations.location_texture_sampler_0 = glGetUniformLocation(shaderProgram, "texture_sampler_0");
	glUniform1i(glsl_locations.location_texture_sampler_0, 0); // 0 for texturing unit 0
	glUseProgram(0);
}

// This is called after the model has been buffered after level load. Don't free vertex and normal
// data as they are used in normal drawing.
void Model::clearModelData()
{
	 // Free face data
	free(data.faceData);
	faceData = NULL;

	// free vertex texture coordinate data
	free(data.textureCoordinates);
    data.textureCoordinates = NULL;
}

// This is called on from unloadLevel() before program terminates
void Model::unload()
{
	// Free vertex data
	free(data.vertexData);
	vertexData = NULL;

	// free vertex normal data
	free(data.vertexNormalData);
	data.vertexNormalData = NULL;

	//Clear shader data
	glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	//Clear buffer data
	glDeleteTextures(1, &textureID);
	glDeleteBuffers(1, (GLuint*) &vertexVBOID);
    glDeleteBuffers(1, (GLuint*) &indiceIBOID);
    glDeleteBuffers(1, (GLuint*) &normalVBOID);
	glDeleteBuffers(1, (GLuint*) &texcordVBOID);
}

model_data Model::getModelData()
{
	return data;
}
std::string Model::getName() const
{
	return name;
}
GLuint Model::getFaceCount() const
{
	return faceCount;
}
GLuint Model::getVertexCount() const
{
	return vertexCount;
}
GLuint Model::getVaoID() const
{
	return vaoID;
}
GLuint Model::getVertexVboID() const
{
	return vertexVBOID;
}
GLuint Model::getIndiceIboID() const
{
	return indiceIBOID;
}
GLuint Model::getNormalVboID() const
{
	return normalVBOID;
}
GLuint Model::getTextureID() const
{
	return textureID;
}
GLuint Model::getTextureCoordinatesVBOID()
{
	return texcordVBOID;
}
glsl_parameter_locations Model::getGlslParameterLocations() const
{
	return glsl_locations;
}
GLuint Model::getShaderProgram() const
{
	return shaderProgram;
}
vec3 Model::getAmbientLightReflectionConstant() const
{
	vec3 retVal;
	retVal.x = ambient_light_reflection_constant[0];
	retVal.y = ambient_light_reflection_constant[1];
	retVal.z = ambient_light_reflection_constant[2];
	return retVal;
}
vec3 Model::getDiffuseLightReflectionConstant() const 
{
	vec3 retVal;
	retVal.x = diffuse_light_reflection_constant[0];
	retVal.y = diffuse_light_reflection_constant[1];
	retVal.z = diffuse_light_reflection_constant[2];
	return retVal;
}
vec3 Model::getSpecularLightReflectionConstant() const
{
	vec3 retVal;
	retVal.x = specular_light_reflection_constant[0];
	retVal.y = specular_light_reflection_constant[1];
	retVal.z = specular_light_reflection_constant[2];
	return retVal;
}
vec3 Model::getBBMin() const
{
	return BB_min;
}
vec3 Model::getBBMax() const
{
	return BB_max;
}