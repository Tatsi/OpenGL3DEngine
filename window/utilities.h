#ifndef ÚTILITIES_H
#define UTILITIES_H

struct vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

typedef unsigned short int ushort;

void writeToLog(std::string componentName, std::string message);

//This function checks whether there has been a noticeable error since last call and in case prints it
void checkForGLErrors(std::string caller);

void reshape(GLsizei w, GLsizei h);

//This function can be used to print shader log (if compiling shaders fails)
void printShaderlog(int shader);

//This function returns true if given point (px, py) is within (or on one of the edges) the given 2D-triangle
bool pointInTriangle2D(float px, float py, float Ax, float Ay, float Bx, float By, float Cx, float Cy);

//This fuction returns true if given point (px, py) is within the rectangle defined by (minX,minY) and (maxX,maxY).
//These points are the opposite corners
bool pointInRectangle2D(float px, float py, float minX, float minY, float maxX, float maxY);


//This function returns true if two given aix-aligned bounding boxes intersect.
bool axisAlignedBoundingBoxIntersection(vec3 bb1min, vec3 bb1max, vec3 bb2min, vec3 bb3max);

//This function returns true if given triangle shares at least one edge with the given set of triangles
//A,B,C = triangles vertices
//triangles = set of triangles to compare to. These are in format ABCABCABC... where A,B and C are one triangles corner points
bool triangleSharesAnEdge(vec3 A, vec3 B, vec3 C, std::vector<vec3> triangles);

//This function is used to find the closest triangle below the point given as parameter.
//Each triangle in the set of triangles to be compared needs to include this point in x-z plane.
//
//vec3 point = closest triangle below this point is returned. In form ABC.
//std::vector<vec3> triangles = set of triangles to be compared. In form ABCABCABC...
//
//Returns vector containing the vertices of the closest triangle below
std::vector<vec3> findClosestTriangleBelowPoint(vec3 point, std::vector<vec3> triangles);

#endif