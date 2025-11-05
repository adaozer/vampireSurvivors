#pragma once
#include "Player.h"

class Melee : public Character {
public:
    Melee(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage);

    void draw(GamesEngineeringBase::Window& canvas, Camera& cam);

    void move(Player& p, float dt);
};

