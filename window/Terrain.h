#ifndef TERRAIN_H
#define TERRAIN_H

//This structure stores two points of square
//that surrounds the approximated triangle. These
//points need to be two opposite corners.
//This also stores corner points of the
//surrounded triangle
struct squareSurroundingTriangle {
	vec3 min;
	vec3 max;
	vec3 triangleVertices[3];
	vec3 triangle_normal;
};

/*
This class represents game terrain. The terrain model must not be transformed or at least
player will end up in the wrong place.
*/
class Terrain
{
public:
	Terrain();
	
	void load();
	
	void unload();
	
	//This function returns the the terrain triangles which are below or above the given 2D-point.
	//	x = x coordinate of point
	//	z = z coordinate of point
	//returns triangles in form ABCABCABC.. where A,B and C are triangles vertices
	std::vector<vec3> Terrain::findTrianglesBelowOrAbove(float x, float z);

	//This function checks whether movement vector intersects at least one terrain triangle.
	//This movement vector must go on the xz-plane at the heigth of y.
	//	oldX,oldZ = coordinates of old position
	//	newX,newZ = coordinates of new position
	//	y = height of the plane
	//This is no longer used in collision detection, but this may be used later for camera.
	bool movementIntersectsTerrainTriangle(float oldX, float oldZ, float newX, float newZ, float y);

	//This function checks if axis-aligned bounding box collisions with a terrain triangle
	//	box_center - Center of the players box
	//	half_width - half of the width of the players bounding box
	//	half_height - half of the height of the players bounding box
	//Returns true if collision occurs.
	bool boundingBoxIntersectsTerrainTriangle(vec3 box_center, float half_width, float half_height);

	//This function checks if axis-aligned bounding box collisions with one of the terrain game objects bounding boxes. 
	//GameObjects BoundingBoxWidth boolean determines whether players collision is calculated as triangle or bounding box level.
	//
	//	box_center - Center of the players box
	//	half_width - half of the width of the players bounding box
	//	half_height - half of the height of the players bounding box
	//Returns true if collision occurs.
	bool Terrain::boundingBoxIntersectsTerrainBoundingBox(vec3 box_center, float half_width, float half_height);

	//Adds a new collisionable and drawable game object to list of terrain objects. The terrain floor and other
	//terrain objects should be loaded (when the level is loaded in LevelManager class) using this function.
	void addTerrainObject(GameObject terrainObject);

	std::vector<GameObject>& getGameObjects();

private:
	std::vector<GameObject> terrainObjects; //Gameobjects that are still, visible and collisionable. Includes the terrain base

	//This stores the square approximations of terrain triangles. This structure can be used to
	//quickly find the triangles that include certain point on xz-plane.
	std::vector<squareSurroundingTriangle> squares;
};

#endif