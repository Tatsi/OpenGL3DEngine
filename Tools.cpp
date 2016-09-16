#include "stdafx.h"

char* loadShaderFromFile(std::string filename, GLint& length)
{
	std::ifstream stream(filename.c_str(), std::ifstream::in|std::ifstream::binary);
    char* source;
    
    if (stream.good())
    {
        stream.seekg(0,std::ios::end);
        length = stream.tellg();
        stream.seekg (0, std::ios::beg);
        source = (char*) malloc(length*sizeof(char));
        stream.read(source, length);
    }
    else
	{
        throw std::logic_error("Error reading shader file. Could not open file.");
    }
	
    stream.close();
    return source;
}

model_data loadAndCreateTexturedModelData(std::string filename)
{
    model_data data;
	//Init BB min and max points
	data.BB_min.x = 1000.0;
	data.BB_min.y = 1000.0;
	data.BB_min.z = 1000.0;
	data.BB_max.x = -1000.0;
	data.BB_max.y = -1000.0;
	data.BB_max.z = -1000.0;

	std::string normalFileName = filename;
	normalFileName.append(".normal");//Used for opening normal
	filename.append(".model");//Used for opening model file
    
	//Open stream to normal data
	std::fstream normalstream(normalFileName.c_str(), std::ifstream::out);
	if(!normalstream.good())
	{
		throw std::logic_error("Error writing normal data to file.");
	}

	//Open stream to model file
    std::ifstream stream(filename.c_str(), std::ifstream::in);

    if (stream.good())
    {
        try {
            char tmp[100];

            //Read header info
            stream.getline(tmp, 100);        
            data.vertexCount = atoi(strtok(tmp, " "));
            data.faceCount = atoi(strtok(NULL, "\n"));
            
            //Read vertex & texture data
            data.vertexData = (float*) malloc(data.vertexCount*3*sizeof(float));
			data.textureCoordinates = (float*) malloc(data.vertexCount*2*sizeof(float));

            for (int i = 0; i < data.vertexCount; i++)
            {
                stream.getline(tmp, 100);
                data.vertexData[i*3] = (float) atof(strtok(tmp, " "));//X
                data.vertexData[i*3+1] = (float) atof(strtok(NULL, " "));//Y
                data.vertexData[i*3+2] = (float) atof(strtok(NULL, " "));//Z

				data.textureCoordinates[i*2] = (float) atof(strtok(NULL, " "));//U
                data.textureCoordinates[i*2+1] = (float) atof(strtok(NULL, " "));//V

				//Update BB values if necessary
				if (data.vertexData[i*3]   < data.BB_min.x){ data.BB_min.x = data.vertexData[i*3];   }
				if (data.vertexData[i*3+1] < data.BB_min.y){ data.BB_min.y = data.vertexData[i*3+1]; }
				if (data.vertexData[i*3+2] < data.BB_min.z){ data.BB_min.z = data.vertexData[i*3+2]; }
				if (data.vertexData[i*3]   > data.BB_max.x){ data.BB_max.x = data.vertexData[i*3];   }
				if (data.vertexData[i*3+1] > data.BB_max.y){ data.BB_max.y = data.vertexData[i*3+1]; }
				if (data.vertexData[i*3+2] > data.BB_max.z){ data.BB_max.z = data.vertexData[i*3+2]; }
            }
            
            //Read face data
            data.faceData = (ushort*) malloc(data.faceCount*3*sizeof(ushort));
            
            for (int i = 0; i < data.faceCount; i++)
            {
                stream.getline(tmp, 100);
                data.faceData[i*3] = (ushort) atoi(strtok(tmp, " "));
                data.faceData[i*3+1] = (ushort) atoi(strtok(NULL, " "));
                data.faceData[i*3+2] = (ushort) atoi(strtok(NULL, " "));
            }

            //Calculate normal vector data. This is done by calculating normal vectors
            //of each face that includes the specific vertex. These vectors are
            //normalized to unit vectors and summed. The sum is divided by the length
            //of the obtained vector and this value is stored for each vertex.
            data.vertexNormalData = (float*) malloc(data.vertexCount*3*sizeof(float));
            for (int i = 0; i < data.vertexCount; i++)
            {
                float sum_x = 0;
                float sum_y = 0;
                float sum_z = 0;
                
                for (int f = 0; f < data.faceCount; f++)
                {
                    if (i == data.faceData[f*3] || i == data.faceData[f*3+1] || i == data.faceData[f*3+2])
                    {
                        //Calculate x,y and z positions for corner points (A,B,C) of current face
                        float A_x = data.vertexData[3*data.faceData[f*3]];
                        float A_y = data.vertexData[3*data.faceData[f*3]+1];
                        float A_z = data.vertexData[3*data.faceData[f*3]+2];
                        
                        float B_x = data.vertexData[3*data.faceData[f*3+1]];
                        float B_y = data.vertexData[3*data.faceData[f*3+1]+1];
                        float B_z = data.vertexData[3*data.faceData[f*3+1]+2];
                        
                        float C_x = data.vertexData[3*data.faceData[f*3+2]];
                        float C_y = data.vertexData[3*data.faceData[f*3+2]+1];
                        float C_z = data.vertexData[3*data.faceData[f*3+2]+2];
                        //AB x AC
                        float AB_x = B_x - A_x; //Vector from A to B
                        float AB_y = B_y - A_y;
                        float AB_z = B_z - A_z;
                        float AB_length = sqrtf(powf(AB_x, 2.f)+powf(AB_y, 2.f)+powf(AB_z, 2.f));
                        AB_x = AB_x / AB_length; //to unit vector
                        AB_y = AB_y / AB_length; //to unit vector
                        AB_z = AB_z / AB_length; //to unit vector
                        
                        float AC_x = C_x - A_x; //Vector from A to C
                        float AC_y = C_y - A_y;
                        float AC_z = C_z - A_z;
                        float AC_length = sqrtf(powf(AC_x, 2.f)+powf(AC_y, 2.f)+powf(AC_z, 2.f));
                        AC_x = AC_x / AC_length; //to unit vector
                        AC_y = AC_y / AC_length; //to unit vector
                        AC_z = AC_z / AC_length; //to unit vector

                        float ABxAC_x = AB_y*AC_z-AB_z*AC_y; //Cross product of two side vectors of face
                        float ABxAC_y = AB_z*AB_x-AB_x*AC_z;
                        float ABxAC_z = AB_x*AC_y-AB_y*AC_x;
                        float ABxAC_length = sqrtf(powf(ABxAC_x, 2.f)+powf(ABxAC_y, 2.f)+powf(ABxAC_z, 2.f));
                        ABxAC_x = ABxAC_x / ABxAC_length; //to unit vector
                        ABxAC_y = ABxAC_y / ABxAC_length; //to unit vector
                        ABxAC_z = ABxAC_z / ABxAC_length; //to unit vector

                        sum_x+=ABxAC_x; //add values of obtained normal vector to sum
                        sum_y+=ABxAC_y;
                        sum_z+=ABxAC_z;
                    }
                }
                //Calculate length of obtained normal vector
                float sum_length = sqrtf(powf(sum_x, 2.f)+powf(sum_y, 2.f)+powf(sum_z, 2.f));
                //Store normal vector as unit vector by dividing components with length
                data.vertexNormalData[i*3] = sum_x / sum_length;
                data.vertexNormalData[i*3+1] = sum_y / sum_length;
                data.vertexNormalData[i*3+2] = sum_z / sum_length;
				//Save results in 'filename.normal' file
				normalstream << data.vertexNormalData[i*3] << " " << data.vertexNormalData[i*3+1] << " " << data.vertexNormalData[i*3+2] << std::endl;
            }
        
        } catch (...) {
            throw std::logic_error("Error reading model file.");
        }
    }
    else { throw std::logic_error("Error reading vertex file. Could not open file."); }


	normalstream.close();
    stream.close();    
    return data;
}

model_data loadTexturedModelData(std::string filename)
{
	//Open stream for reading normal data
	std::string normalfilename = filename;
	normalfilename.append(".normal");
	filename.append(".model");

	std::fstream normalstream(normalfilename.c_str(), std::fstream::in|std::fstream::binary);
	if (!normalstream.good()) //Check that normal data file really exists
	{
		throw std::logic_error("Error reading normal file.");
	}

    model_data data;
	
	//Init BB min and max points
	data.BB_min.x = 1000.0;
	data.BB_min.y = 1000.0;
	data.BB_min.z = 1000.0;
	data.BB_max.x = -1000.0;
	data.BB_max.y = -1000.0;
	data.BB_max.z = -1000.0;

    std::ifstream stream(filename.c_str(), std::ifstream::in);
    
    if (stream.good())
    {
        try
		{
            char tmp[100];

            //Read header info
            stream.getline(tmp, 100);        
            data.vertexCount = atoi(strtok(tmp, " "));
            data.faceCount = atoi(strtok(NULL, "\n"));
            
            //Read vertex & texture data
            data.vertexData = (float*) malloc(data.vertexCount*3*sizeof(float));
			data.textureCoordinates = (float*) malloc(data.vertexCount*2*sizeof(float));

            for (int i = 0; i < data.vertexCount; i++)
            {
                stream.getline(tmp, 100);
                data.vertexData[i*3] = (float) atof(strtok(tmp, " "));//X
                data.vertexData[i*3+1] = (float) atof(strtok(NULL, " "));//y
                data.vertexData[i*3+2] = (float) atof(strtok(NULL, " "));//Z

				data.textureCoordinates[i*2] = (float) atof(strtok(NULL, " "));//U
                data.textureCoordinates[i*2+1] = (float) atof(strtok(NULL, " "));//V

				//Update BB values if necessary
				if (data.vertexData[i*3]   < data.BB_min.x){ data.BB_min.x = data.vertexData[i*3];   }
				if (data.vertexData[i*3+1] < data.BB_min.y){ data.BB_min.y = data.vertexData[i*3+1]; }
				if (data.vertexData[i*3+2] < data.BB_min.z){ data.BB_min.z = data.vertexData[i*3+2]; }
				if (data.vertexData[i*3]   > data.BB_max.x){ data.BB_max.x = data.vertexData[i*3];   }
				if (data.vertexData[i*3+1] > data.BB_max.y){ data.BB_max.y = data.vertexData[i*3+1]; }
				if (data.vertexData[i*3+2] > data.BB_max.z){ data.BB_max.z = data.vertexData[i*3+2]; }
            }
            
            //Read face data
            data.faceData = (ushort*) malloc(data.faceCount*3*sizeof(ushort));
            
            for (int i = 0; i < data.faceCount; i++)
            {
                stream.getline(tmp, 100);
                data.faceData[i*3] = (ushort) atoi(strtok(tmp, " "));
                data.faceData[i*3+1] = (ushort) atoi(strtok(NULL, " "));
                data.faceData[i*3+2] = (ushort) atoi(strtok(NULL, " "));
            }

			//Read normal data from different file (example bunny.normal)
			data.vertexNormalData = (float*) malloc(data.vertexCount*3*sizeof(float));

			for (int i = 0; i < data.vertexCount; i++)
            {
                normalstream.getline(tmp, 100);
				data.vertexNormalData[i*3] = (float) atoi(strtok(tmp, " "));
				data.vertexNormalData[i*3+1] = (float) atoi(strtok(NULL, " "));
				data.vertexNormalData[i*3+2] = (float) atoi(strtok(NULL, " "));
            }
        } catch (...) {
            throw std::logic_error("Error reading model file.");
        }
    }
    else { throw std::logic_error("Error reading vertex file. Could not open file."); }
    
    stream.close();    
    return data;
}