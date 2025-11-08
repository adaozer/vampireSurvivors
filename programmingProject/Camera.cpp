#include "Camera.h"

Camera::Camera(float _posX, float _posY) : GameObject(_posX, _posY) {} // Simple game object with only an X and Y coordinate

void Camera::center(float targetX, float targetY, GamesEngineeringBase::Window& canvas, World& w) {
    // Keep the camera centered on the player. Target is the player so it keeps it centered.
     posX = targetX - canvas.getWidth() / 2;
     posY = targetY - canvas.getHeight() / 2;

     if (mode == 1) { // Only when the map is finite
         // This is so the camera doesn't show black space beyond the rendered tiles. It stops centering on the player when the edge of the map is reached.
         if (posX < 0) posX = 0;
         if (posY < 0) posY = 0;
         if (posX + canvas.getWidth() > w.getWorldWidth()) posX = w.getWorldWidth() - canvas.getWidth();
         if (posY + canvas.getHeight() > w.getWorldHeight()) posY = w.getWorldHeight() - canvas.getHeight();
     }
 }