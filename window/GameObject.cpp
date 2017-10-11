#include "stdafx.h"
#include "GameObject.h"


std::string GameObject::getId() const
{
	return id;
}
Model& GameObject::getModel() const
{
	return model;
}
vec3 GameObject::getPosition() const
{
	return position;
}
GLfloat GameObject::getScale() const
{
	return scale;
}

boolean GameObject::calculateCollisionsOnTriangleLevel() const
{
	return collisionsOnTriangleLevel;
}
