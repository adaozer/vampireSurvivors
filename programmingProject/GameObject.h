#pragma once
#include "GamesEngineeringBase.h"

class GameObject {
public:
	float posX, posY; // Every game object will have an x and y
	GamesEngineeringBase::Image image; // Some will have a sprite but not all of them have to, so 2 different constructors.

	GameObject(float _posX, float _posY);
	GameObject(float _posX, float _posY, std::string filename);

	void updatePos(float _posX, float _posY);
	float getX();
	float getY();
};
