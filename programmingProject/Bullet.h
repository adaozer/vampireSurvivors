#pragma once
#include "Player.h"

class Bullet : public GameObject {
public:
    bool friendly; // Check if its shot by the player or an enemy, this will help us have different velocity and damage values for hero and enemies and also check collision 
    int damage;
    bool alive = true;

    float velocity;
    float vx, vy;

    int radius;

    float age, maxAge;

    Bullet(float _posX, float _posY, float towardX, float towardY, bool _friendly, int _damage);
    bool isOnScreen(Camera& cam, GamesEngineeringBase::Window& canvas);
    void update(float dt, Player& p);
    void draw(GamesEngineeringBase::Window& canvas, Camera& cam);
};

