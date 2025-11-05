#pragma once
#include "GameObject.h"

class Character : public GameObject {
public:
    int health, speed, damage;
    bool alive = true;

    Character(float _posX, float _posY, std::string _filename, int _health, int _speed, int _damage);

    void takeDamage(int dmg);

    void die();

    int getHealth();
    bool isAlive();
};