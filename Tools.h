#ifndef TOOLS_H
#define TOOLS_H

struct model_data{
    int vertexCount;
    int faceCount;
    float* vertexData;
    float* vertexNormalData;
	float* textureCoordinates;
    ushort* faceData;
	vec3 BB_min;//Bounding box min point
	vec3 BB_max;
};

char* loadShaderFromFile(std::string filename, GLint& length);

//Depricated
//model_data loadAndCreateModelData(std::string filename);//This loads model data and calculates & saves the normal data
//model_data loadModelData(std::string filename); //This is used when precalculated file for normals exists

model_data loadTexturedModelData(std::string fileName);//This loads a model that has precalculated normals and texture UVs
model_data loadAndCreateTexturedModelData(std::string filename);

#endif