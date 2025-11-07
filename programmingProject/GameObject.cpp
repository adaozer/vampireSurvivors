#include "GameObject.h"
#include <iostream>

GameObject::GameObject(float _posX, float _posY, std::string filename) : posX(_posX), posY(_posY) {
    image.load(filename);
}

GameObject::GameObject(float _posX, float _posY) : posX(_posX), posY(_posY) {}

void GameObject::updatePos(float _posX, float _posY) {
    // Update position function to move stuff on the screen
        posX += _posX;
        posY += _posY;
    }

float GameObject::getX() { return posX; }
float GameObject::getY() { return posY; }