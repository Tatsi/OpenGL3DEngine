#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
public:
	GameObject(std::string id, Model& model, vec3 position, GLfloat modelScale, bool collisionsOnTriangleLevel, GLfloat boundingBoxSideLength) : id(id), model(model),
		position(position), scale(modelScale), collisionsOnTriangleLevel(collisionsOnTriangleLevel) 
	{ 
		//Game object should contain reference to model instead of getting it all the time..
	}

	std::string getId() const;
	Model& getModel() const;
	std::string getModelName() const;
	vec3 getPosition() const;
	GLfloat getScale() const;
	boolean calculateCollisionsOnTriangleLevel() const; //Returns if collisions are calculated against models triangles instead of bounding box approximation
private:
	std::string id;
	//std::string modelName;
	Model& model;
	vec3 position;
	GLfloat scale;
	bool collisionsOnTriangleLevel;
	//vec3 BB_min; BB values are values are relative to position
	//vec3 BB_max;
};

#endif