#include "Camera.h"

Camera::Camera(float _posX, float _posY) : GameObject(_posX, _posY) {} // Simple game object with only an X and Y coordinate

void Camera::center(float targetX, float targetY, GamesEngineeringBase::Window& canvas) {
     posX = targetX - canvas.getWidth() / 2;
     posY = targetY - canvas.getHeight() / 2;
 }