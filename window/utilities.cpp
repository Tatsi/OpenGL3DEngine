#include "stdafx.h"

void writeToLog(std::string componentName, std::string message)
{
	std::cout << "[" << componentName + "] " << message << std::endl;
}

void reshape(GLsizei w, GLsizei h)
{
    glMatrixMode (GL_PROJECTION);    /* prepare for and then */ 
    glLoadIdentity ();               /* define the projection */
    gluPerspective(45, 1, 0.5, 20);
    glMatrixMode (GL_MODELVIEW);  /* back to modelview matrix */
    glViewport (0, 0, w, h);      /* define the viewport */
}

void printShaderlog(int shader){
	int infoLogLen = 0, charsWritten = 0;
	char *infoLog = 0;
	glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0) {
		infoLog = (char *)malloc (infoLogLen);
		glGetShaderInfoLog (shader, infoLogLen, &charsWritten, infoLog);
		std::cout.write(infoLog,infoLogLen);
		OutputDebugString(infoLog);
		free (infoLog);
	} 
}

void checkForGLErrors(std::string caller)
{
	GLenum code = glGetError();
	const GLubyte *errString;

	if ( code != GL_NO_ERROR) {
		errString = gluErrorString(code);
		std::cout << "There has been an error in OpenGL at " << caller << ", error message: " << errString << std::endl;
		//fprintf (stderr, "There has been an error in OpenGL: %s\n", errString);
	}
}

bool pointInTriangle2D(float px, float py, float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{
	//Calculate vector V1 from A to B
	float V1_x = Bx - Ax;
	float V1_y = By - Ay;
	
	//Calculate vector V2 from A to C
	float V2_x = Cx - Ax;
	float V2_y = Cy - Ay;

	//Calculate cross product V1xV2
	float V1xV2 = V1_x*V2_y - V1_y*V2_x;

	//Calculate term a=((VxV2)-(V0xV2))/(V1xV2), V0 is (Ax,Ay) and V is (px, py)
	float a = ( (px*V2_y - py*V2_x) - (Ax*V2_y - Ay*V2_x) ) / V1xV2;
	//Calculate term b=((VxV1)-(V0xV1))/(V1xV2), V0 is (Ax,Ay) and V is (px, py)
	float b = -( (px*V1_y - py*V1_x) - (Ax*V1_y - Ay*V1_x) ) / V1xV2;

	//If a,b >= 0 and a+b <= 1 then point is within triangle (or on the edges)
	//If we want to exclude edges we would calculate a > 0.0 && b > 0.0 && a+b < 1.0
	if (a >= 0.0 && b >= 0.0 && a+b <= 1.0)
	{
		return true;
	}
	return false;
}

bool pointInRectangle2D(float px, float py, float minX, float minY, float maxX, float maxY)
{
	return px >= minX && px <= maxX &&
			py >= minY && py <= maxY;
}

bool axisAlignedBoundingBoxIntersection(vec3 bb1min, vec3 bb1max, vec3 bb2min, vec3 bb2max)
{
	if(bb1min.x > bb2max.x) return false;
    if(bb1min.y > bb2max.y) return false;
    if(bb1min.z > bb2max.z) return false;
    if(bb1max.x < bb2min.x) return false;
    if(bb1max.y < bb2min.y) return false;
    if(bb1max.z < bb2min.z) return false;
	return true;
}

std::vector<vec3> findClosestTriangleBelowPoint(vec3 point, std::vector<vec3> triangles)
{
	std::vector<vec3> closestTriangle(3);

	float distanceToClosestTriangle = -1.0;

	for (int i = 0; i < triangles.size() / 3; i++)
	{
		//V0 is here the vector from origo to first vertex of the triangle i.e. triangles[i*3]
		//V1 is vector from V0 to second vertex
		//V2 is vector from V0 to third vertex

		vec3 V1;
		V1.x = triangles[i*3+1].x - triangles[i*3].x; 
		V1.y = triangles[i*3+1].y - triangles[i*3].y; 
		V1.z = triangles[i*3+1].z - triangles[i*3].z;
		vec3 V2;
		V2.x = triangles[i*3+2].x - triangles[i*3].x; 
		V2.y = triangles[i*3+2].y - triangles[i*3].y; 
		V2.z = triangles[i*3+2].z - triangles[i*3].z;

		//Some of these values may equal zero. This will cause dividing by zero. Well fix this here
		if (V1.x == 0.0) { V1.x = 0.00001; }
		if (V1.y == 0.0) { V1.y = 0.00001; }
		if (V1.z == 0.0) { V1.z = 0.00001; }
		if (V2.x == 0.0) { V2.x = 0.00001; }
		if (V2.y == 0.0) { V2.y = 0.00001; }
		if (V2.z == 0.0) { V2.z = 0.00001; }

		float a = (point.x-triangles[i*3].x) / (V1.x + ( (V2.x*V1.z) / V2.z) ) - ( (V2.x*point.x - V2.x*triangles[i*3].z) / (V1.x*V2.z + V2.x*V1.z) );
		
		float b = (point.z-triangles[i*3].z) / (V2.z + ( (V1.z*V2.x) / V1.x) ) - ( (V1.z*point.x - V1.z*triangles[i*3].x) / (V1.x*V2.z + V2.x*V1.z) );

		float k = triangles[i*3].y+a*V1.y+b*V2.y;

		//Now well calculate the distance from point to triangle along the y-axis.
		float distance = point.y - k;

		//Well update the current closest triangle only if this triangle is below
		if (distance >= 0.0)
		{
			//Well update the current closest triangle if a. its empty (-1) OR current closest distance is bigger than k
			if (distanceToClosestTriangle == -1.0 || distanceToClosestTriangle > distance)
			{
				distanceToClosestTriangle = distance; //Update distance to closest triangle

				closestTriangle[0] = triangles[i*3];
				closestTriangle[1] = triangles[i*3+1];
				closestTriangle[2] = triangles[i*3+2];
			}
		}
	}
	return closestTriangle;	
}