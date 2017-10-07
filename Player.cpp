#include "stdafx.h"
#include "Player.h"

Player Player::instance = Player();

void Player::init()
{
  speed = 0.03; //Moving speed
  rotationSpeed = 0.05;
  falling_speed = 0.09;
  angle = 0.0;
  height = 0.3;
  BB_width = 0.1;
  BB_height = 0.2;//Not used currently
  jump_height = 0.5;
  vx = 0.0;
  vz = -2.0;
  distanceToTriangleBelow = 0.0;

  x = 0.0;
  y = 4.0;
  z = 0.0;

  moving_up = false;
  moving_down = false;
  moving_left = false;
  moving_right = false;
}
void Player::draw()
{
	glPushMatrix();
        glTranslatef(Player::get().getX(), Player::get().getY(), Player::get().getZ());
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, -height, 0.0);
		glEnd();
    glPopMatrix();

	float max_width = sqrtf( (BB_width/2.0)*(BB_width/2.0)+(BB_width/2.0)*(BB_width/2.0) );

	glColor3f(0.0, 1.0, 0.0);
	for (int i = 0; i < 360; i++)
	{
	  glPushMatrix();
	  
		glTranslatef(x, y, z);
		glRotatef(i, 0.0, 2.0, 0.0);

		glBegin(GL_POLYGON);
			glVertex3f(-max_width, BB_height/2.0, 0.0);
			glVertex3f(max_width, BB_height/2.0, 0.0);
			glVertex3f(max_width, -BB_height/2.0, 0.0);
			glVertex3f(-max_width, -BB_height/2.0, 0.0);
		glEnd();

	  glPopMatrix();
	}

}
void Player::move()
{
	if (moving_up && !moving_down && !moving_right && !moving_left)//Move north
	{
		moveForward();
	}
	else if (moving_up && !moving_down && moving_right && !moving_left)//Move north-east
	{
		moveNorthEast();
	}
	else if (!moving_up && !moving_down && moving_right && !moving_left)//Move east
	{
		strafeRight();
	}
	else if (!moving_up && moving_down && moving_right && !moving_left)//Move south-east
	{
		moveSouthEast();
	}
	else if (!moving_up && moving_down && !moving_right && !moving_left)//Move south
	{
		moveBackwards();
	}
	else if (!moving_up && moving_down && !moving_right && moving_left)//Move south-west
	{
		moveSouthWest();
	}
	else if (!moving_up && !moving_down && !moving_right && moving_left)//Move west
	{
		strafeLeft();
	}
	else if (moving_up && !moving_down && !moving_right && moving_left)//north-west
	{
		moveNorthWest();
	}
}
float Player::getX()
{
	return x;
}
float Player::getY()
{
	return y;
}
float Player::getZ()
{
	return z;
}
vec3 Player::getPosition()
{
	vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}
float Player::getAngle()
{
	return angle;
}
float Player::getVX()
{
	return vx;
}
float Player::getVZ()
{
	return vz;
}
float Player::getSpeed()
{
	return speed;
}
void Player::setX(float new_x)
{
	x = new_x;
}
void Player::setY(float new_y)
{
	y = new_y;
}
void Player::setZ(float new_z)
{
	z = new_z;
}
void Player::setVX(float new_vx)
{
	vx = new_vx;
}
void Player::setVZ(float new_vz)
{
	vz = new_vz;
}
void Player::increaseAngle()
{
	angle += rotationSpeed;
}
void Player::decreaseAngle()
{
	angle -= rotationSpeed;
}
void Player::updateFalling()
{
	if (distanceToTriangleBelow > falling_speed)
	{
		y -= falling_speed;
		distanceToTriangleBelow -= falling_speed;
	}
	else if (distanceToTriangleBelow > 0.0 && distanceToTriangleBelow < falling_speed)
	{
		y -= distanceToTriangleBelow;
		distanceToTriangleBelow = 0.0;
	}
}
void Player::moveForward()
{
	 vec3 pos;
	 pos.x = x + speed * vx;
	 pos.z = z + speed * vz;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::moveBackwards()
{
	 vec3 pos;
	 pos.x = x - speed * vx;
	 pos.z = z - speed * vz;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::strafeLeft()
{
	 vec3 pos;
	 pos.x = x - speed * -vz;
	 pos.z = z - speed * vx;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::strafeRight()
{
	 vec3 pos;
	 pos.x = x - speed * vz;
	 pos.z = z - speed * -vx;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::moveNorthEast()
{
	 vec3 pos;
	 pos.x = x + 0.5 * speed * vx - 0.5 * speed * vz;
	 pos.z = z + 0.5 * speed * vz - 0.5 * speed * -vx;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::moveSouthEast()
{
	 vec3 pos;
	 pos.x = x - 0.5 * speed * vx - 0.5 * speed * vz;
	 pos.z = z - 0.5 * speed * vz - 0.5 * speed * -vx;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::moveSouthWest()
{
	 vec3 pos;
	 pos.x = x - 0.5 * speed * vx - 0.5 * speed * -vz;
	 pos.z = z - 0.5 * speed * vz - 0.5 * speed * vx;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::moveNorthWest()
{
	 vec3 pos;
	 pos.x = x + 0.5 * speed * vx - 0.5 * speed * -vz;
	 pos.z = z + 0.5 * speed * vz - 0.5 * speed * vx;
	 std::pair<float,float> value = getNewPlayerY(pos.x, pos.z);
	 pos.y = value.first;

	 if (!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainTriangle(pos, BB_width/2.0, BB_height/2.0) &&
			!LevelManager::get().getTerrain().boundingBoxIntersectsTerrainBoundingBox(pos, BB_width/2.0, BB_height/2.0))
	 {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		distanceToTriangleBelow = value.second;
		Renderer::get().setCameraX(x - vx);
		Renderer::get().setCameraZ(z - vz);
	 }
}
void Player::jump()
{
	y += jump_height;
}
void Player::setMovingUp(bool val)
{
	moving_up = val;
}
void Player::setMovingDown(bool val)
{
	moving_down = val;
}
void Player::setMovingLeft(bool val)
{
	moving_left = val;
}
void Player::setMovingRight(bool val)
{
	moving_right = val;
}
void Player::updatePlayerY()
{
	std::vector<vec3> triangles = LevelManager::get().getTerrain().findTrianglesBelowOrAbove(x, z);

	float distance_to_closest = -1.0;
	float y_of_the_closest;

	for (int i = 0; i < triangles.size() / 3; i++)
	{
		float ax = triangles[i*3].x;
		float ay = triangles[i*3].y;
		float az = triangles[i*3].z;
		float bx = triangles[i*3+1].x;
		float by = triangles[i*3+1].y;
		float bz = triangles[i*3+1].z;
		float cx = triangles[i*3+2].x;
		float cy = triangles[i*3+2].y;
		float cz = triangles[i*3+2].z;

		// Calculate plane equation based on the triangle
		float A = ay * (bz - cz) + by * (cz - az) + cy * (az - bz);
		float B = az *(bx - cx) + bz *(cx - ax) + cz *(ax - bx);
		float C = ax *(by - cy) + bx *(cy - ay) + cx *(ay - by);
		float D = -(ax *(by* cz - cy* bz) + bx *(cy* az - ay* cz) + cx *(ay * bz - by * az)); 

		if (B==0.0)
		{
			writeToLog("updatePlayerY()", "B was 0.");
			B=0.001;	
		}

		float triangle_y = (-D-A*x-C*z)/B;

		// Current triangle may only be the first one below the player center point if its y value is smaller or equal to characters center y
		// Notice that if the ascent on one player move is more than players height, player falls through the ground
		if (triangle_y <= y)
		{
			// If this is the first examined triangle (i.e. the distance is not initialized) or distance is smaller than current
			// Notice that if player is standing between two triangles the other one gets filtered out here.
			if (distance_to_closest == -1.0 || distance_to_closest > y - triangle_y)
			{
				distance_to_closest =  y - triangle_y;
				y_of_the_closest = triangle_y; //Update the y
			}
		}
	}

	//#Falling is currently implemented here#
	// If distance from the feet of the player to the ground is greater than the falling speed the player falls the lenght of the falling_speed
	if (distance_to_closest - height > falling_speed)
	{
		y -= falling_speed;
	}
	else // If its smaller then the player is set to stand on the triangle
	{
		y =  y_of_the_closest + height;
	}	
}
std::pair<float,float> Player::getNewPlayerY(float newX, float newZ)
{
	std::vector<vec3> triangles = LevelManager::get().getTerrain().findTrianglesBelowOrAbove(newX, newZ);

	float distance_to_closest = -1.0;
	float y_of_the_closest;

	for (int i = 0; i < triangles.size() / 3; i++)
	{
		float ax = triangles[i*3].x;
		float ay = triangles[i*3].y;
		float az = triangles[i*3].z;
		float bx = triangles[i*3+1].x;
		float by = triangles[i*3+1].y;
		float bz = triangles[i*3+1].z;
		float cx = triangles[i*3+2].x;
		float cy = triangles[i*3+2].y;
		float cz = triangles[i*3+2].z;

		// Calculate plane equation based on the triangle
		float A = ay * (bz - cz) + by * (cz - az) + cy * (az - bz);
		float B = az *(bx - cx) + bz *(cx - ax) + cz *(ax - bx);
		float C = ax *(by - cy) + bx *(cy - ay) + cx *(ay - by);
		float D = -(ax *(by* cz - cy* bz) + bx *(cy* az - ay* cz) + cx *(ay * bz - by * az)); 

		if (B==0.0)
		{
			writeToLog("updatePlayerY()", "B was 0.");
			B=0.001;	
		}

		float triangle_y = (-D-A*newX-C*newZ)/B;

		//Current triangle may only be the first one below the player center point if its y value is smaller or equal to characters center y
		//Notice that if the ascent on one player move is more than players height, player falls through the ground
		if (triangle_y <= y)
		{
			//If this is the first examined triangle (i.e. the distance is not initialized) or distance is smaller than current
			//Notice that if player is standing between two triangles the other one gets filtered out here.
			if (distance_to_closest == -1.0 || distance_to_closest > y - triangle_y)
			{
				distance_to_closest =  y - triangle_y;
				y_of_the_closest = triangle_y; //Update the y
			}
		}
	}

	//#Falling is currently implemented here#
	//If distance from the feet of the player to the ground is greater than the falling speed the player falls the lenght of the falling_speed
	if (distance_to_closest - height > falling_speed)
	{
		return std::pair<float,float>(y, distance_to_closest - height);
	}
	else //If its smaller then the player is set to stand on the triangle
	{
		return std::pair<float,float>(y_of_the_closest + height, 0.0);
	}	
}
/*Un-used. Gets the closest terrain triangle below player.
std::vector<vec3> Player::getTriangleBelowPlayer()
{
	std::vector<vec3> triangles = LevelManager::get().getTerrain().findTrianglesBelowOrAbove(x, z);

	float distance_to_closest = -1.0;
	std::vector<vec3> closest(3);

	for (int i = 0; i < triangles.size() / 3; i++)
	{
		float ax = triangles[i*3].x;
		float ay = triangles[i*3].y;
		float az = triangles[i*3].z;
		float bx = triangles[i*3+1].x;
		float by = triangles[i*3+1].y;
		float bz = triangles[i*3+1].z;
		float cx = triangles[i*3+2].x;
		float cy = triangles[i*3+2].y;
		float cz = triangles[i*3+2].z;

		//Calculate plane equation based on the triangle
		float A = ay * (bz - cz) + by * (cz - az) + cy * (az - bz);
		float B = az *(bx - cx) + bz *(cx - ax) + cz *(ax - bx);
		float C = ax *(by - cy) + bx *(cy - ay) + cx *(ay - by);
		float D = -(ax *(by* cz - cy* bz) + bx *(cy* az - ay* cz) + cx *(ay * bz - by * az)); 

		if (B==0.0)
		{
			writeToLog("updatePlayerY()", "B was 0.");
			B=0.001;	
		}

		float triangle_y = (-D-A*x-C*z)/B;

		//Current triangle may only be the first one below the player center point if its y value is smaller or equal to characters center y
		//Notice that if the ascent on one player move is more than players height, player falls through the ground
		if (triangle_y <= y)
		{
			//If this is the first examined triangle (i.e. the distance is not initialized) or distance is smaller than current
			//Notice that if player is standing between two triangles the other one gets filtered out here.
			if (distance_to_closest == -1.0 || distance_to_closest > y - triangle_y)
			{
				distance_to_closest =  y - triangle_y;
				closest[0] = triangles[i*3];
				closest[1] = triangles[i*3+1];
				closest[2] = triangles[i*3+2];
			}
		}
	}

	return closest;
}
*/