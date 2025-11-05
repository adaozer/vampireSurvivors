#pragma once
#include "GamesEngineeringBase.h"

class GameObject {
public:
	float posX, posY;
	GamesEngineeringBase::Image image;

	GameObject(float _posX, float _posY);
	GameObject(float _posX, float _posY, std::string filename);

	void updatePos(float _posX, float _posY);
	float getX();
	float getY();
};
