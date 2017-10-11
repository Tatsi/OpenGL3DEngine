#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	static Player& get() { return instance; }
	
	void init();

	void move();

	float getX();
	float getY();
	float getZ();
	vec3 getPosition();
	float getAngle();
	float getVX();
	float getVZ();
	float getSpeed();

	void setX(float new_x);
	void setY(float new_y);
	void setZ(float new_z);
	void setVX(float new_vx);
	void setVZ(float new_vz);
	void increaseAngle();
	void decreaseAngle();

	void draw();
	void moveForward();
	void moveBackwards();
	void strafeLeft();
	void strafeRight();
	void moveNorthEast();
	void moveSouthEast();
	void moveSouthWest();
	void moveNorthWest();
	void jump(); //This is really raw implementation of jumping. It simply increases players height by jump_height.

	//These functions can be used to set whether payer is moving to certain direction
	void setMovingUp(bool val);
	void setMovingDown(bool val);
	void setMovingLeft(bool val);
	void setMovingRight(bool val);

	//This function updates the y-position of player based on terrain triangle below player.
	//This must be called when no moving is done during update. When moving occurs Y is updated automatically.
	void updatePlayerY();

	//This function returns the new y coordinate of player when moving from x,z to newX, newZ
	//float 1 - players y when in newX, newZ. This is the old y if player is falling.
	//float 2 - distance to triangle below. This is 0 if player is not falling.
	std::pair<float,float> Player::getNewPlayerY(float newX, float newZ);

	//This function is for debug purposes. It returns the triangle that is used in updatePlayerY() i.e.
	//the triangle that is below the player. This is currently commented out.
	//std::vector<vec3> getTriangleBelowPlayer();

	//This function updates falling if distance to triangle below is > heigth
	void Player::updateFalling();

private:
	Player() {}
	Player(const Player&);
	Player& operator=(const Player&);

	//Player parameters
	float x; //Center x
	float y; //Center y
	float z; //Center z
	float height;//player height from floor to player position (x,y,z) which is also the center of the model
	float angle;//Rotation angle of player
	float vx; //Moving vector x
	float vz; //Moving vector z
	float speed;//Moving speed
	float rotationSpeed;
	float falling_speed;//Gravity
	float jump_height; //The height player can jump to
	float BB_width;//Width of the bounding box
	float BB_height;//Height of the bounding box
	float distanceToTriangleBelow;//Distance from the toes of the player to the triangle that player stands on

	bool moving_up;
	bool moving_down;
	bool moving_left;
	bool moving_right;

	static Player instance;
};

#endif