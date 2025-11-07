#pragma once
#include "Character.h"
#include "Camera.h"
#include "Helper.h"

class Player;
class Bullet;

class Ranged : public Character {
public:
    Bullet* enemyBarr[bulletSize]; // A bullet array for its bullets it shoots
    float shootingSpeed = 1.f; // How fast it shoots
    float shootTimer = 0.f;

    Ranged(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage);

    void draw(GamesEngineeringBase::Window& canvas, Camera& cam);

    void shoot(Player& p);

    void update(float dt, Player& p, GamesEngineeringBase::Window& canvas, Camera& cam);

};
