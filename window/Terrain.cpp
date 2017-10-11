#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain() 
{ }

void Terrain::load()
{
	// Well add triangles (and their approximations) of each terrain object to single buffer called "squares" for later use
	// in collision detection.
	for (std::vector<GameObject>::const_iterator it = terrainObjects.begin(); it < terrainObjects.end(); it++)
	{
		if (it->calculateCollisionsOnTriangleLevel())
		{
			model_data data = it->getModel().getModelData(); // Buffer modeldata
			vec3 position = it->getPosition(); // We need to transform triangles positions with current objects transformations
			GLfloat scale = it->getScale(); 

			// Init square approximation vector. The vector consists of nodes
			// that hold the triangle coordinates and the rectangle approximation.
			// The approximations are used when searching for the triangles below
			// or above certain 2D-point. This makes calculation faster.
			for (int i = 0; i < data.faceCount; i++)
			{
				squareSurroundingTriangle tmp;

				// Store faces vertices temporarily
				vec3 A; vec3 B; vec3 C;
				A.x = scale* data.vertexData[3*data.faceData[3*i]] + position.x;
				A.y = scale* data.vertexData[3*data.faceData[3*i]+1] + position.y;
				A.z = scale* data.vertexData[3*data.faceData[3*i]+2] + position.z;
				B.x = scale* data.vertexData[3*data.faceData[3*i+1]] + position.x;
				B.y = scale* data.vertexData[3*data.faceData[3*i+1]+1] + position.y;
				B.z = scale* data.vertexData[3*data.faceData[3*i+1]+2] + position.z;
				C.x = scale* data.vertexData[3*data.faceData[3*i+2]] + position.x;
				C.y = scale* data.vertexData[3*data.faceData[3*i+2]+1] + position.y;
				C.z = scale* data.vertexData[3*data.faceData[3*i+2]+2] + position.z;

				// Store corner points of actual triangle
				tmp.triangleVertices[0] = A;
				tmp.triangleVertices[1] = B;
				tmp.triangleVertices[2] = C;

				//Calculate triangle normal
				vec3 AB, AC;
				AB.x = B.x - A.x;
				AB.y = B.y - A.y;
				AB.z = B.z - A.z;
				AC.x = C.x - A.x;
				AC.y = C.y - A.y;
				AC.z = C.z - A.z;

				float ACxAB_x = AC.y * AB.z - AC.z * AB.y; //Cross product of two side vectors of face
				float ACxAB_y = AC.z * AB.x - AC.x * AB.z;
				float ACxAB_z = AC.x * AB.y - AC.y * AB.x;

				float length = sqrtf(powf(ACxAB_x, 2.f)+powf(ACxAB_y, 2.f)+powf(ACxAB_z, 2.f));
				//tmp.triangle_normal.x = ACxAB_x / length;
				//tmp.triangle_normal.y = ACxAB_y / length;
				//tmp.triangle_normal.z = ACxAB_z / length;
				tmp.triangle_normal.x = ACxAB_x;
				tmp.triangle_normal.y = ACxAB_y;
				tmp.triangle_normal.z = ACxAB_z;
		
				//Calculate coordinates for two corner points of surrounding rectangle
				GLfloat minX = 10000.0;
				GLfloat minY = 10000.0;
				GLfloat minZ = 10000.0;
				GLfloat maxX = -10000.0;
				GLfloat maxY = -10000.0;
				GLfloat maxZ = -10000.0;

				for (int j = 0; j < 3; j++) 
				{
					if (minX > tmp.triangleVertices[j].x) { minX = tmp.triangleVertices[j].x; }
					if (minY > tmp.triangleVertices[j].y) { minY = tmp.triangleVertices[j].y; }
					if (minZ > tmp.triangleVertices[j].z) { minZ = tmp.triangleVertices[j].z; }

					if (maxX < tmp.triangleVertices[j].x) { maxX = tmp.triangleVertices[j].x; }
					if (maxY < tmp.triangleVertices[j].y) { maxY = tmp.triangleVertices[j].y; }
					if (maxZ < tmp.triangleVertices[j].z) { maxZ = tmp.triangleVertices[j].z; }
				}

				tmp.min.x = minX;
				tmp.min.y = minY;
				tmp.min.z = minZ;
				tmp.max.x = maxX;
				tmp.max.y = maxY;
				tmp.max.z = maxZ;

				squares.push_back(tmp);//Store triangle approximation
			}
		}
	}
}

void Terrain::unload()
{
	//Nothing to unload yet
}

std::vector<vec3> Terrain::findTrianglesBelowOrAbove(float x, float z)
{
	std::vector<vec3> v; // Strore triangles that are below or above player here

	// loop through terrain squares to find the one below player point
	for(std::vector<squareSurroundingTriangle>::const_iterator it = squares.begin(); it < squares.end(); it++)
	{
		// Check if point is in rectangle approximation of current triangle
		if (pointInRectangle2D(x, z, it->min.x, it->min.z, it->max.x, it->max.z))
		{
			// Check if the actual triangle is above or below point. If so, add this triangle to v.
			if (pointInTriangle2D(x, z, it->triangleVertices[0].x, it->triangleVertices[0].z, 
					it->triangleVertices[1].x, it->triangleVertices[1].z, it->triangleVertices[2].x, it->triangleVertices[2].z))
			{
				v.push_back(it->triangleVertices[0]);
				v.push_back(it->triangleVertices[1]);
				v.push_back(it->triangleVertices[2]);
			}
		}
	}
	if (v.size() >= 3) // At least one triangle was found
	{
		return v; // Return all terrain triangles that are below or above
	}

	writeToLog("Terrain::findTrianglesBelowOrAbove", "There is no triangle below or above the player. ERROR!");
	throw std::logic_error("There is no terrain triangle below or above the player. Player is outside the terrain.");
}

bool Terrain::movementIntersectsTerrainTriangle(float oldX, float oldZ, float newX, float newZ, float y)
{
	// Loop through all the triangles in the terrain
	for(std::vector<squareSurroundingTriangle>::const_iterator it = squares.begin(); it < squares.end(); it++)
	{
		// Check if current triangles minY and maxY are on different sides of the xz-plane. Only then the vector may intersect with the triangle.
		// We assume that if min or max is on the plane the plane doesn't intersect with the triangle (because of < and >).
		if (it->min.y < y && it->max.y > y)
		{
			// Calculate the intersection point of the movement line and triangles plane.
			// We need to find the corner point which is on the other side of the plane as the other corner points.
			// That is because then we can use this point later to calculate the points where the triangles sides intersect the
			// xz-plane. Now we only need to calculate the plane equation, but noticing this now helps us later.
			
			std::vector<vec3> upperCorners; // We store the corners above the xz-plane here
			std::vector<vec3> lowerCorners; // We store the corners below the xz-plane here
			std::vector<vec3> onPlaneCorners; // We store the corners that are on the plane here, if necessary
			
			for (int i = 0; i < 3; i++) // loop through the triangles vertices and sort them
			{
				if (it->triangleVertices[i].y < y)
				{
					lowerCorners.push_back(it->triangleVertices[i]);
				}
				else if (it->triangleVertices[i].y > y)
				{
					upperCorners.push_back(it->triangleVertices[i]);
				}
				else
				{
					writeToLog("Terrain::movementIntersectsTerrainTriangle", "Triangles vertice was on the xz-plane.");
					onPlaneCorners.push_back(it->triangleVertices[i]);
				}
			}
			// Now we have sorted the triangles vertices. Now we need find the one that is alone on one side of the plane.
			// We call this corner A. Other corners are called B and C.
			vec3 A, B, C;

			if (lowerCorners.size() == 2)
			{
				A = upperCorners[0];
				B = lowerCorners[0];
				C = lowerCorners[1];
			}
			else if (upperCorners.size() == 2)
			{
				A = lowerCorners[0];
				B = upperCorners[0];
				C = upperCorners[1];
			}
			else // One of the corners is on the xz-plane so we can choose either lower or upper to be the source point of triangles side vectors
			{
				A = lowerCorners[0];
				B = upperCorners[0];
				C = onPlaneCorners[0];
			}
			// Now we can form side vectors for triangles plane 
			vec3 V1, V2;
			V1.x = B.x - A.x; V1.y = B.y - A.y; V1.z = B.z - A.z;
			V2.x = C.x - A.x; V2.y = C.y - A.y; V2.z = C.z - A.z;
			// Define movement vector from (oldX, oldZ) to (newX, newZ)
			vec3 M;
			M.x = newX - oldX;
			M.y = 0.0;
			M.z = newZ - oldZ;
			// Calculate dot product of movement vector and normal. If it equals zero, there is 90 degrees angle and the line and triangle never intersect.
			if ( M.x*it->triangle_normal.x + M.z*it->triangle_normal.z  < 0.0)//??Miksi pienempää kuin nolla
			{
				float s = -( it->triangle_normal.x * (oldX-A.x) + it->triangle_normal.y * (y-A.y) + it->triangle_normal.z * (oldZ-A.z) ) /
					(it->triangle_normal.x * M.x + it->triangle_normal.y * M.y + it->triangle_normal.z * M.z);
				// If s is negative it doesnt hit the triangle
				if (s >= 0.0)
				{
					// Now we can calculate the intersection point
					vec3 intersection;
					intersection.x = oldX + s*M.x;
					intersection.y = y;
					intersection.z = oldZ + s*M.z;

					// If the intersection is between old and new player positions we continue checking
					if (pointInRectangle2D(intersection.x, intersection.z, min(oldX,newX), min(oldZ,newZ), max(oldX,newX), max(oldZ,newZ) ) )
					{
						// If we got this far we need to calculate the intersection points of triangle and xz-plane.
						vec3 point1, point2;
						float scale1 = abs((y-A.y)/(B.y-A.y));
						point1.x = A.x + scale1 * V1.x;
						point1.y = A.y + scale1 * V1.y;
						point1.z = A.z + scale1 * V1.z;
						float scale2 = abs((y-A.y)/(C.y-A.y));
						point2.x = A.x + scale2 * V2.x;
						point2.y = A.y + scale2 * V1.y;
						point2.z = A.z + scale2 * V2.z;

						// If intersection point is between these two points the movement vector intersects with this triangle
						if (pointInRectangle2D(intersection.x, intersection.z, min(point1.x,point2.x), min(point1.z,point2.z), max(point1.x, point2.x), max(point1.z,point2.z) ) )
						{
							return true;
						}
					}
				}
			}
		}	
	}
	return false;
}

bool Terrain::boundingBoxIntersectsTerrainTriangle(vec3 box_center, float half_width, float half_height)
{
	// Loop through all terrain triangles
	for(std::vector<squareSurroundingTriangle>::const_iterator it = squares.begin(); it < squares.end(); it++)
	{
		// Move triangle towards origo by distracting box center from triangles vertices
		vec3 A, B, C;
		A.x = it->triangleVertices[0].x - box_center.x;
		A.y = it->triangleVertices[0].y - box_center.y;
		A.z = it->triangleVertices[0].z - box_center.z;
		B.x = it->triangleVertices[1].x - box_center.x;
		B.y = it->triangleVertices[1].y - box_center.y;
		B.z = it->triangleVertices[1].z - box_center.z;
		C.x = it->triangleVertices[2].x - box_center.x;
		C.y = it->triangleVertices[2].y - box_center.y;
		C.z = it->triangleVertices[2].z - box_center.z;

		// Do same for the triangles bounding box
		vec3 triangle_min, triangle_max;
		triangle_min.x = it->min.x - box_center.x;
		triangle_min.y = it->min.y - box_center.y;
		triangle_min.z = it->min.z - box_center.z;
		triangle_max.x = it->max.x - box_center.x;
		triangle_max.y = it->max.y - box_center.y;
		triangle_max.z = it->max.z - box_center.z;

		// Box normals
		vec3 e[3];
		e[0].x = 1.0;
		e[0].y = 0.0;
		e[0].z = 0.0;
		e[1].x = 0.0;
		e[1].y = 1.0;
		e[1].z = 0.0;
		e[2].x = 0.0;
		e[2].y = 0.0;
		e[2].z = 1.0;

		// Phase 1: approximate triangle with axis-aligned bounding box. Check their intersection.
		if (!(half_width < triangle_min.x) && !(- half_width > triangle_max.x) &&
			!(half_height < triangle_min.y) && !(- half_height > triangle_max.y) &&
			!(half_width < triangle_min.z) && !(- half_width > triangle_max.z) )
		{
			// Phase 2: Well calculate points n (the most backward to the plane) and p (the most forward to the plane)
			// Then well calculate vectors from these points to some point of the triangle. Then we calculate dot products of these
			// vectors with triangles normal.
			// If the signs are same, the plane (and ofc neither the triangle) doesn't intersect the cube.
			vec3 p, n, pv, nv;

			p.x = -half_width;
			p.y = -half_height;
			p.z = -half_width;
			if (it->triangle_normal.x >= 0.0) { p.x = half_width; }
			if (it->triangle_normal.y >= 0.0) { p.y = half_height; }
			if (it->triangle_normal.z >= 0.0) { p.z = half_width; }
			
			n.x = half_width;
			n.y = half_height;
			n.z = half_width;
			if (it->triangle_normal.x >= 0.0) { n.x = -half_width; }
			if (it->triangle_normal.y >= 0.0) { n.y = -half_height; }
			if (it->triangle_normal.z >= 0.0) { n.z = -half_width; }

			// Form vectors from p and n to some corner point of the triangle. We select corner A.
			pv.x = A.x - p.x;
			pv.y = A.y - p.y;
			pv.z = A.z - p.z;
			nv.x = A.x - n.x;
			nv.y = A.y - n.y;
			nv.z = A.z - n.z;

			float dot_p = it->triangle_normal.x * pv.x + it->triangle_normal.y * pv.y + it->triangle_normal.z * pv.z;
			float dot_n = it->triangle_normal.x*nv.x + it->triangle_normal.y*nv.y + it->triangle_normal.z*nv.z;

			// If the signs of dot products are different, the box and the triangle may intersect.
			if ( !(dot_p < 0.0 && dot_n < 0.0) && !(dot_p > 0.0 && dot_n  > 0.0) && !(dot_p == 0.0 && dot_n  == 0.0) )
			{
				// Phase 3:
				vec3 f[3];
				f[0].x = B.x - A.x;
				f[0].y = B.y - A.y;
				f[0].z = B.z - A.z;
				f[1].x = C.x - B.x;
				f[1].y = C.y - B.y;
				f[1].z = C.z - B.z;
				f[2].x = A.x - C.x;
				f[2].y = A.y - C.y;
				f[2].z = A.z - C.z;

				int passes = 0;

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						vec3 a; // Calculate crossproduct Ei x Fj
						a.x = e[i].y * f[j].z - e[i].z * f[j].y;
						a.y = e[i].z * f[j].x - e[i].x * f[j].z;
						a.z = e[i].x * f[j].y - e[i].y * f[j].x;
						
						float p0 = a.x * A.x + a.y * A.y + a.z * A.z;
						float p1 = a.x * B.x + a.y * B.y + a.z * B.z;
						float p2 = a.x * C.x + a.y * C.y + a.z * C.z;

						float r = half_width * abs(a.x) + half_height * abs(a.y) + half_width * abs(a.z);

						if (min(min(p0,p1),p2) > r || max(max(p0,p1),p2) < -r)
						{
							// This failed so we can stop investigating this triangle
							i = 3;
							j = 3;
						}
						else
						{
							passes++;
						}
					}
				}
				if (passes == 9)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool Terrain::boundingBoxIntersectsTerrainBoundingBox(vec3 box_center, float half_width, float half_height)
{
	vec3 player_min, player_max;
	player_min.x = box_center.x - half_width;
	player_min.y = box_center.y - half_height;
	player_min.z = box_center.z - half_width;
	player_max.x = box_center.x + half_width;
	player_max.y = box_center.y + half_height;
	player_max.z = box_center.z + half_width;

	for (std::vector<GameObject>::const_iterator it = terrainObjects.begin(); it < terrainObjects.end(); it++)
	{
		if (!it->calculateCollisionsOnTriangleLevel())
		{
			vec3 pos = it->getPosition();
			
			vec3 bb_min = it->getModel().getBBMin();
			vec3 bb_max = it->getModel().getBBMax();

			vec3 terrain_object_min, terrain_object_max;
			// TODO
			terrain_object_min.x = pos.x + it->getScale() * bb_min.x;
			terrain_object_min.y = pos.y + it->getScale() * bb_min.y;
			terrain_object_min.z = pos.z + it->getScale() * bb_min.z;
			terrain_object_max.x = pos.x + it->getScale() * bb_max.x;
			terrain_object_max.y = pos.y + it->getScale() * bb_max.y;
			terrain_object_max.z = pos.z + it->getScale() * bb_max.z;

			if(axisAlignedBoundingBoxIntersection(player_min, player_max, terrain_object_min, terrain_object_max))
			{
				return true;
			}
		}
	}
	return false;
}

void Terrain::addTerrainObject(GameObject terrainObject)
{
	terrainObjects.push_back(terrainObject);
}

std::vector<GameObject>& Terrain::getGameObjects()
{
	return terrainObjects;
}