#pragma once
#include "Player.h"

class Ranged : public Character {
public:
    Bullet* enemyBarr[bulletSize];
    float shootingSpeed = 1.f;
    int currentSize = 0;
    float shootTimer = 0.f;

    Ranged(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage);

    void draw(GamesEngineeringBase::Window& canvas, Camera& cam);

    void shoot(Player& p);

    void update(float dt, Player& p, GamesEngineeringBase::Window& canvas, Camera& cam);

};
