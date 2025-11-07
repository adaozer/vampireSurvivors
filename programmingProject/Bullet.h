#pragma once
#include "Camera.h"

class Player;

class Bullet : public GameObject {
public:
    bool friendly; // Check if its shot by the player or an enemy, this will help us have different velocity and damage values for hero and enemies and also check collision 
    int damage; // Damage of the bullet
    bool alive = true; // To keep track of if the bullet is still going

    float velocity; // This is to move the bullet
    float vx, vy;

    int radius; // To check collision and to draw the bullet

    float age, maxAge; // So enemy bullets don't live forever (player bullets get deleted if they leave the screen)

    Bullet(float _posX, float _posY, float towardX, float towardY, bool _friendly, int _damage);
    void update(float dt, Player& p, GamesEngineeringBase::Window& canvas, Camera& cam);
    void draw(GamesEngineeringBase::Window& canvas, Camera& cam);
};

