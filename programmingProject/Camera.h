#pragma once
#include "GameObject.h"

class Camera : public GameObject {
public:

    Camera(float _posX, float _posY);

    void center(float targetX, float targetY, GamesEngineeringBase::Window& canvas);
};