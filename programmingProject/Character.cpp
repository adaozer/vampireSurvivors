#include "Character.h"

Character::Character(float _posX, float _posY, std::string _filename, int _health, int _speed, int _damage) : GameObject(_posX, _posY, _filename), health(_health), speed(_speed), damage(_damage) {}

void Character::takeDamage(int dmg) {
    if (!alive) return;
    health -= dmg;
    if (health <= 0) {
        die();
    }
}

void Character::die() {
    alive = false;
}

int Character::getHealth() { return health; }
bool Character::isAlive() { return alive; }