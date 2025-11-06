#include "Camera.h"

Camera::Camera(float _posX, float _posY) : GameObject(_posX, _posY) {} // Simple game object with only an X and Y coordinate

void Camera::center(float targetX, float targetY, GamesEngineeringBase::Window& canvas, World& w) {
     posX = targetX - canvas.getWidth() / 2;
     posY = targetY - canvas.getHeight() / 2;

     if (posX < 0) posX = 0;
     if (posY < 0) posY = 0;
     if (posX + canvas.getWidth() > w.getWorldWidth()) posX = w.getWorldWidth() - canvas.getWidth();
     if (posY + canvas.getHeight() > w.getWorldHeight()) posY = w.getWorldHeight() - canvas.getHeight();

 }